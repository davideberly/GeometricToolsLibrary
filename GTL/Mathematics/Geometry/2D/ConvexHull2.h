// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// Compute the convex hull of 2D points using a divide-and-conquer algorithm.
// This is an O(N log N) algorithm for N input points. The only way to ensure
// a correct result for the input vertices is to use an exact predicate for
// computing signs of various expressions. The implementation uses interval
// arithmetic and rational arithmetic for the predicate. The input type T must
// satisfy std::is_floating_point<T>::value = true.

#include <GTL/Mathematics/Geometry/2D/ExactToLineExtended2.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <type_traits>
#include <vector>

namespace gtl
{
    template <typename T>
    class ConvexHull2
    {
    public:
        ConvexHull2()
            :
            mPoints(nullptr),
            mRPoints{},
            mConverted{},
            mMerged{},
            mQuery{},
            mDimension(0),
            mHull{}
        {
            static_assert(
                std::is_floating_point<T>::value,
                "The input type must be a floating-point type.");
        }

        // Compute the exact convex hull using a blend of interval arithmetic
        // and rational arithmetic. The code runs single-threaded.
        void operator()(std::size_t numPoints, Vector2<T> const* points)
        {
            GTL_ARGUMENT_ASSERT(
                numPoints > 0 && points != nullptr,
                "Invalid argument.");

            // Provide access to the points for the member functions.
            mPoints = points;

            // Allocate storage for any rational points that must be computed
            // in the exact sign predicates.
            mRPoints.resize(numPoints);
            mConverted.resize(numPoints);
            std::fill(mConverted.begin(), mConverted.end(), 0);

            // Sort the points indirectly. The mHull array is used to store
            // the unique indices.
            auto lessThanPoints = [this](std::size_t s0, std::size_t s1)
            {
                return mPoints[s0] < mPoints[s1];
            };

            auto equalPoints = [this](std::size_t s0, std::size_t s1)
            {
                return mPoints[s0] == mPoints[s1];
            };

            mHull.resize(numPoints);
            std::iota(mHull.begin(), mHull.end(), 0);
            std::sort(mHull.begin(), mHull.end(), lessThanPoints);
            auto newEnd = std::unique(mHull.begin(), mHull.end(), equalPoints);
            mHull.erase(newEnd, mHull.end());

            // Use a divide-and-conquer algorithm. The merge step computes
            // the convex hull of two convex polygons. The merge storage is
            // allocated once to avoid reallocations during the recursive
            // chain of the GetHull and Merge member functions. NOTE: If
            // ConvexHull2 is re-implemented to use multithreading, the
            // merge storage must be allocated per thread (one thread per
            // subhull).
            mMerged.resize(mHull.size());
            std::size_t i0 = 0, i1 = mHull.size() - 1;
            GetHull(i0, i1);
            mHull.resize(i1 - i0 + 1);

            mDimension = std::min(mHull.size() - 1, static_cast<std::size_t>(2));
        }

        void operator()(std::vector<Vector2<T>> const& points)
        {
            operator()(points.size(), points.data());
        }

        // The dimension is 0 (hull is a single point), 1 (hull is a line
        // segment) or 2 (hull is a convex polygon).
        inline std::size_t GetDimension() const
        {
            return mDimension;
        }

        // Get the indices into the input 'points[]' that correspond to hull
        // vertices. The returned array is organized according to the hull
        // dimension.
        //   0: The hull is a single point. The returned array has size 1 with
        //      index corresponding to that point.
        //   1: The hull is a line segment. The returned array has size 2 with
        //      indices corresponding to the segment endpoints.
        //   2: The hull is a convex polygon. The returned array has size N
        //      with indices corresponding to the polygon vertices. The
        //      vertices are ordered.
        inline std::vector<std::size_t> const& GetHull() const
        {
            return mHull;
        }

    private:
        // Supporting type for rational arithmetic used in the exact
        // predicate for the relationship between a point and a line
        // segment.
        using Rational = BSNumber<UIntegerFP32<2>>;

        // Compute the hulls of subsets of points on the recursive downward
        // traversal of the divide-and-conquer tree.
        void GetHull(std::size_t& i0, std::size_t& i1)
        {
            std::size_t numVertices = i1 - i0 + 1;
            if (numVertices > 1)
            {
                // Compute the middle index of input range.
                std::size_t mid = (i0 + i1) / 2;

                // Compute the hull of subsets (mid-i0+1 >= i1-mid).
                std::size_t j0 = i0, j1 = mid, j2 = mid + 1, j3 = i1;
                GetHull(j0, j1);
                GetHull(j2, j3);

                // Merge the convex hulls into a single convex hull.
                Merge(j0, j1, j2, j3, i0, i1);
            }
            // else: The convex hull is a single point.
        }

        // Merge the hulls on the upward traversal of the divide-and-conquer
        // tree.
        void Merge(std::size_t j0, std::size_t j1, std::size_t j2, std::size_t j3, std::size_t& i0, std::size_t& i1)
        {
            // Subhull0 is to the left of subhull1 because of the initial
            // sorting of the points by x-components. We need to find two
            // mutually visible points, one on the left subhull and one on
            // the right subhull.
            std::size_t const size0 = j1 - j0 + 1, size1 = j3 - j2 + 1;

            // Find the right-most point of the left subhull.
            Vector2<T> pmax0 = mPoints[mHull[j0]];
            std::size_t imax0 = j0;
            for (std::size_t i = j0 + 1; i <= j1; ++i)
            {
                Vector2<T> p = mPoints[mHull[i]];
                if (pmax0 < p)
                {
                    pmax0 = p;
                    imax0 = i;
                }
            }

            // Find the left-most point of the right subhull.
            Vector2<T> pmin1 = mPoints[mHull[j2]];
            std::size_t imin1 = j2;
            for (std::size_t i = j2 + 1; i <= j3; ++i)
            {
                Vector2<T> p = mPoints[mHull[i]];
                if (p < pmin1)
                {
                    pmin1 = p;
                    imin1 = i;
                }
            }

            // Get the lower tangent to hulls (LL = lower-left,
            // LR = lower-right).
            std::size_t iLL = imax0, iLR = imin1;
            GetTangent(j0, j1, j2, j3, iLL, iLR);

            // Get the upper tangent to hulls (UL = upper-left,
            // UR = upper-right).
            std::size_t iUL = imax0, iUR = imin1;
            GetTangent(j2, j3, j0, j1, iUR, iUL);

            // Construct the counterclockwise-ordered merged-hull vertices.
            std::size_t k{};
            std::size_t numMerged = 0;

            std::size_t i = iUL;
            for (k = 0; k < size0; ++k)
            {
                mMerged[numMerged++] = mHull[i];
                if (i == iLL)
                {
                    break;
                }
                i = (i < j1 ? i + 1 : j0);
            }
            GTL_RUNTIME_ASSERT(
                k < size0,
                "Unexpected condition.");

            i = iLR;
            for (k = 0; k < size1; ++k)
            {
                mMerged[numMerged++] = mHull[i];
                if (i == iUR)
                {
                    break;
                }
                i = (i < j3 ? i + 1 : j2);
            }
            GTL_RUNTIME_ASSERT(
                k < size1,
                "Unexpected condition.");

            std::size_t next = j0;
            for (k = 0; k < numMerged; ++k)
            {
                mHull[next] = mMerged[k];
                ++next;
            }

            i0 = j0;
            i1 = next - 1;
        }

        void GetTangent(std::size_t j0, std::size_t j1, std::size_t j2, std::size_t j3, std::size_t& i0, std::size_t& i1)
        {
            // In theory, the loop terminates in a finite number of steps, but
            // in case the code is not correct, limit the number of loop
            // iterations.
            std::size_t const size0 = j1 - j0 + 1, size1 = j3 - j2 + 1;
            std::size_t const isup = size0 + size1;
            std::size_t i{};
            for (i = 0; i < isup; i++)
            {
                // Get the endpoints of the potential tangent.
                std::size_t hL1 = mHull[i0];
                std::size_t hR0 = mHull[i1];

                // Walk along the left hull to find the point of tangency.
                if (size0 > 1)
                {
                    std::size_t iLm1 = (i0 > j0 ? i0 - 1 : j1);
                    std::size_t hL0 = mHull[iLm1];
                    auto const& PR0 = mPoints[hR0];
                    auto const& PL0 = mPoints[hL0];
                    auto const& PL1 = mPoints[hL1];

                    auto GetRPoints = [this, &hR0, &hL0, &hL1]()
                    {
                        return std::array<Vector2<Rational> const*, 3>
                        {
                            &GetRPoint(hR0),
                            &GetRPoint(hL0),
                            &GetRPoint(hL1)
                        };
                    };

                    auto order = mQuery(PR0, PL0, PL1, GetRPoints);
                    if (order == ExactToLineExtended2<T>::OrderType::P_RIGHT_OF_V0_V1 ||
                        order == ExactToLineExtended2<T>::OrderType::COLLINEAR_RIGHT)
                    {
                        i0 = iLm1;
                        continue;
                    }
                }

                // Walk along right hull to find the point of tangency.
                if (size1 > 1)
                {
                    std::size_t iRp1 = (i1 < j3 ? i1 + 1 : j2);
                    std::size_t hR1 = mHull[iRp1];
                    auto const& PL1 = mPoints[hL1];
                    auto const& PR0 = mPoints[hR0];
                    auto const& PR1 = mPoints[hR1];

                    auto GetRPoints = [this, &hL1, &hR0, &hR1]()
                    {
                        return std::array<Vector2<Rational> const*, 3>
                        {
                            &GetRPoint(hL1),
                            &GetRPoint(hR0),
                            &GetRPoint(hR1)
                        };
                    };

                    auto order = mQuery(PL1, PR0, PR1, GetRPoints);
                    if (order == ExactToLineExtended2<T>::OrderType::P_RIGHT_OF_V0_V1 ||
                        order == ExactToLineExtended2<T>::OrderType::COLLINEAR_LEFT)
                    {
                        i1 = iRp1;
                        continue;
                    }
                }

                // The tangent segment has been found.
                break;
            }

            // This assertion should not be triggered with correctly written
            // code.
            GTL_RUNTIME_ASSERT(
                i < isup,
                "Unexpected condition.");
        }

        // Memoized access to the rational representation of the points.
        Vector2<Rational> const& GetRPoint(std::size_t index)
        {
            if (mConverted[index] == 0)
            {
                mConverted[index] = 1;
                for (std::size_t i = 0; i < 2; ++i)
                {
                    mRPoints[index][i] = mPoints[index][i];
                }
            }
            return mRPoints[index];
        }

    private:
        // The input points to operator().
        Vector2<T> const* mPoints;

        // A blend of interval arithmetic and exact arithmetic is used to
        // ensure correctness. The rational representations are computed on
        // demand and are memoized. The mConverted array has elements 0 or 1,
        // where initially the values are 0. The first time mRPoints[i] is
        // encountered, mConverted[i] is 0. The floating-point vector
        // mPoints[i] is converted to a rational vector, after which
        // mConverted[1] is set to 1 to avoid converting again if the
        // floating-point vector is encountered in another predicate
        // computation.
        std::vector<Vector2<Rational>> mRPoints;
        std::vector<std::uint32_t> mConverted;

        // For internal computations.
        std::vector<std::size_t> mMerged;
        ExactToLineExtended2<T> mQuery;

        // The output data.
        std::size_t mDimension;
        std::vector<std::size_t> mHull;

    private:
        friend class UnitTestConvexHull2;
    };
}
