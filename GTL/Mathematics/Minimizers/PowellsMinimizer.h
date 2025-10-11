// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.11

#pragma once

// Search for a minimum using Powell's conjugate direction methods. The
// Cartesian-product domain provided to operator()(...) has minimum values
// stored in t0[0..d-1] and maximum values stored in t1[0..d-1], where d > 1
// is the number of dimensions. The domain is searched along lines through the
// current estimate of the minimum location. Each such line is searched for a
// minimum using a ParabolicInterpolationMinimizer<T> object. The inputs
// maxSubdivisions, maxBisections, epsilon and tolerance are used by the
// 1-dimensional minimizer. The input maxIterations is the number of
// iterations for Powell's method.

#include <GTL/Mathematics/Minimizers/BrentsMinimizer.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <cstddef>
#include <functional>
#include <limits>
#include <vector>

namespace gtl
{
    template <typename T>
    class PowellsMinimizer
    {
    public:
        PowellsMinimizer(std::size_t dimensions, std::size_t maxSubdivisions,
            std::size_t maxBisections, T const& epsilon, T const& tolerance)
            :
            mDimensions(dimensions),
            mDirections(dimensions + 1),
            mCurrentDirectionIndex(0),
            mCurrentConjugateIndex(dimensions),
            mCurrentT(dimensions),
            mMinimizer(maxSubdivisions, maxBisections, epsilon, tolerance)
        {
            GTL_ARGUMENT_ASSERT(
                mDimensions >= 2,
                "The number of dimensions must be at least 2.");

            for (auto& direction : mDirections)
            {
                direction.resize(dimensions);
            }
        }

        // Member access.
        inline void SetMaxSubdivisions(std::size_t maxSubdivisions)
        {
            mMinimizer.SetMaxSubdivisions(maxSubdivisions);
        }

        inline void SetMaxBisections(std::size_t maxBisections)
        {
            mMinimizer.SetMaxBisections(maxBisections);
        }

        inline void SetEpsilon(T const& epsilon)
        {
            mMinimizer.SetEpsilon(epsilon);
        }

        inline void SetTolerance(T const& tolerance)
        {
            mMinimizer.SetTolerance(tolerance);
        }

        inline std::size_t GetMaxSubdivisions() const
        {
            return mMinimizer.GetMaxSubdivisions();
        }

        inline std::size_t GetMaxBisections() const
        {
            return mMinimizer.GetMaxBisections();
        }

        inline T const& GetEpsilon() const
        {
            return mMinimizer.GetEpsilon();
        }

        inline T const& GetTolerance() const
        {
            return mMinimizer.GetTolerance();
        }

        // Find the minimum on the Cartesian-product domain whose minimum
        // values are stored in t0[0..d-1] and whose maximum values are stored
        // in t1[0..d-1], where d is 'dimensions'. An initial guess is
        // specified in tInitial[0..d-1]. The location of the minimum is
        // tMin[0..d-1] and the value of the minimum is 'fMin'. The returned
        // std::size_t is the number of iterations used in the search.
        std::size_t operator()(std::function<T(T const*)> const& F, std::size_t maxIterations,
            T const* t0, T const* t1, T const* tInitial, T* tMin, T& fMin)
        {
            // Store the initial guess so it can be updated to the new
            // starting location for each iteration of Powell's method.
            for (std::size_t d = 0; d < mDimensions; ++d)
            {
                mCurrentT[d] = tInitial[d];
            }
            Vector<T> startT = mCurrentT;

            // Initialize the search directions to the standard basis.
            for (std::size_t d = 0; d < mDimensions; ++d)
            {
                MakeUnit(d, mDirections[d]);
            }

            // Evaluate the function at the initial t-value.
            T currentF = F(tInitial);

            // Create the function G(s) = F(P+s*D) that is the restriction of
            // F(t) to the line segment P+s*D clipped to the domain [t0,t1].
            auto G = [this, &F](T const& s)
            {
                Vector<T> arg = mCurrentT + s * mDirections[mCurrentDirectionIndex];
                return F(arg.data());
            };

            // Iterate over the current set of directions to search for a
            // minimum.
            T s0 = C_<T>(0), s1 = C_<T>(0), sMin = C_<T>(0);
            std::size_t iteration{};
            for (iteration = 0; iteration < maxIterations; ++iteration)
            {
                // Find minimum in each direction and update current location.
                for (std::size_t d = 0; d < mDimensions; ++d)
                {
                    ComputeDomain(t0, t1, mCurrentT, mDirections[d], s0, s1);
                    mCurrentDirectionIndex = d;
                    mMinimizer(G, s0, s1, C_<T>(0), sMin, currentF);
                    mCurrentT += sMin * mDirections[d];
                }

                // Estimate a unit-length conjugate direction.
                auto& conjugate = mDirections[mCurrentConjugateIndex];
                conjugate = mCurrentT - startT;
                T length = Length(conjugate);
                if (length <= mMinimizer.GetTolerance())
                {
                    // The new position did not change significantly from the
                    // old one. TODO: Should there be a better convergence
                    // criterion here?
                    break;
                }
                conjugate /= length;

                // Minimize in the conjugate direction.
                ComputeDomain(t0, t1, mCurrentT, conjugate, s0, s1);
                mCurrentDirectionIndex = mCurrentConjugateIndex;
                mMinimizer(G, s0, s1, C_<T>(0), sMin, currentF);
                mCurrentT += sMin * conjugate;

                // Cycle the directions and add the conjugate direction to
                // the set of directions. TODO: Avoid the direction copies
                // by maintaining a circular queue of directions.
                mCurrentConjugateIndex = 0;
                for (std::size_t d = 0; d < mDimensions; ++d)
                {
                    mDirections[d] = mDirections[d + 1];
                }

                // Set parameters for next pass.
                startT = mCurrentT;
            }

            for (std::size_t d = 0; d < mDimensions; ++d)
            {
                tMin[d] = mCurrentT[d];
            }
            fMin = F(tMin);
            return iteration;
        }

        // Find the minimum on the Cartesian-product domain whose minimum
        // values are stored in t0[0..d-1] and whose maximum values are stored
        // in t1[0..d-1], where d is 'dimensions'. The initial guess is
        // computed internally to be the center of the aligned box defined by
        // t0[] and t1[]. The location of the minimum is tMin[0..d-1] and the
        // value of the minimum is 'fMin'. The returned std::size_t is the number
        // of iterations used in the search.
        std::size_t operator()(std::function<T(T const*)> const& F, std::size_t maxIterations,
            T const* t0, T const* t1, T* tMin, T& fMin)
        {
            Vector<T> tInitial(mDimensions);
            for (std::size_t d = 0; d < mDimensions; ++d)
            {
                tInitial[d] = C_<T>(1, 2) * (t0[d] + t1[d]);
            }
            return operator()(F, maxIterations, t0, t1, tInitial.data(), tMin, fMin);
        }

    private:
        // The current estimate of the minimum location is tCurrent and the
        // direction of the current line to search is dCurrent. The line
        // tCurrent + s * dCurrent must be clipped against the Cartesian
        // product domain. The clip result is the s-interval [s0,s1].
        static void ComputeDomain(T const* t0, T const* t1,
            Vector<T> const& tCurrent, Vector<T> const& dCurrent,
            T& s0, T& s1)
        {
            std::size_t const dimensions = tCurrent.size();
            s0 = -std::numeric_limits<T>::max();
            s1 = +std::numeric_limits<T>::max();

            for (std::size_t d = 0; d < dimensions; ++d)
            {
                T value = dCurrent[d];
                if (value != C_<T>(0))
                {
                    T b0 = t0[d] - tCurrent[d];
                    T b1 = t1[d] - tCurrent[d];
                    if (value > C_<T>(0))
                    {
                        // The valid t-interval is [b0,b1].
                        b0 /= value;
                        if (b0 > s0)
                        {
                            s0 = b0;
                        }
                        b1 /= value;
                        if (b1 < s1)
                        {
                            s1 = b1;
                        }
                    }
                    else
                    {
                        // The valid t-interval is [b1,b0].
                        b0 /= value;
                        if (b0 < s1)
                        {
                            s1 = b0;
                        }
                        b1 /= value;
                        if (b1 > s0)
                        {
                            s0 = b1;
                        }
                    }
                }
            }

            // Correction if numerical errors lead to values nearly zero.
            if (s0 > C_<T>(0))
            {
                s0 = C_<T>(0);
            }
            if (s1 < C_<T>(0))
            {
                s1 = C_<T>(0);
            }
        }

        std::size_t mDimensions;
        std::vector<Vector<T>> mDirections;
        std::size_t mCurrentDirectionIndex;
        std::size_t mCurrentConjugateIndex;
        Vector<T> mCurrentT;
        BrentsMinimizer<T> mMinimizer;

    private:
        friend class UnitTestPowellsMinimizer;
    };
}
