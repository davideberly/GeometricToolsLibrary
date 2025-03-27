// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// Compute a minimum-area oriented box containing the specified points. The
// algorithm uses the rotating calipers method, but with a dual pair of
// calipers. For details, see
// http://www-cgrl.cs.mcgill.ca/~godfried/research/calipers.html
// https://web.archive.org/web/20150330010154/http://cgm.cs.mcgill.ca/~orm/rotcal.html
// The box is supported by the convex hull of the points, so the algorithm
// is really about computing the minimum-area box containing a convex polygon.
// The rotating calipers approach is O(n) in time for n polygon edges.
//
// A detailed description of the algorithm and implementation is found in
// https://www.geometrictools.com/Documentation/MinimumAreaRectangle.pdf
//
// NOTE: This algorithm guarantees a correct output only when ComputeType is
// an exact arithmetic type that supports division. In GTL, one such
// type is BSRational<UIntegerAP32> (arbitrary precision). Another such type
// is BSRational<UIntegerFP32<N>> (fixed precision), where N is chosen large
// enough for your input data sets. If you choose ComputeType to be 'float'
// or 'double', the output is not guaranteed to be correct.

#include <GTL/Mathematics/Primitives/ND/OrientedBox.h>
#include <GTL/Mathematics/Geometry/2D/ConvexHull2.h>
#include <GTL/Mathematics/Primitives/ND/Line.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename InputType, typename ComputeType>
    class MinimumAreaBox2
    {
    public:
        // The class is a functor to support computing the minimum-area box of
        // multiple data sets using the same class object.
        MinimumAreaBox2()
            :
            mNumPoints(0),
            mPoints(nullptr),
            mSupportIndices{ 0, 0, 0, 0 },
            mArea(C_<InputType>(0))
        {
        }

        // The points are arbitrary, so we must compute the convex hull from
        // them in order to compute the minimum-area box. The input parameters
        // are necessary for using ConvexHull2. NOTE: ConvexHull2 guarantees
        // that the hull does not have three consecutive collinear points.
        OrientedBox2<InputType> operator()(std::size_t numPoints,
            Vector2<InputType> const* points,
            bool useRotatingCalipers = !std::is_floating_point<ComputeType>::value)
        {
            mNumPoints = numPoints;
            mPoints = points;
            mHull.clear();

            // Get the convex hull of the points.
            ConvexHull2<InputType> ch2{};
            ch2(mNumPoints, mPoints);
            std::size_t dimension = ch2.GetDimension();

            OrientedBox2<InputType> minBox{};

            if (dimension == 0)
            {
                // The points are all the same.
                minBox.center = mPoints[0];
                minBox.axis[0] = Vector2<InputType>{ C_<InputType>(1), C_<InputType>(0) };
                minBox.axis[1] = Vector2<InputType>{ C_<InputType>(0), C_<InputType>(1) };
                minBox.extent = Vector2<InputType>{ C_<InputType>(0), C_<InputType>(0) };
                mHull.resize(1);
                mHull[0] = 0;
                return minBox;
            }

            if (dimension == 1)
            {
                // The points lie on a line. Determine the extreme t-values
                // for the points represented as P = origin + t*direction. We
                // know that 'origin' is an/ input vertex, so we can start
                // both t-extremes at zero.
                auto const& hull = ch2.GetHull();
                Line2<InputType> line{};
                line.origin = mPoints[hull[0]];
                line.direction = mPoints[hull[1]] - mPoints[hull[0]];
                Normalize(line.direction);
                InputType tmin = C_<InputType>(0), tmax = C_<InputType>(0);
                std::size_t imin = 0, imax = 0;
                for (std::size_t i = 0; i < mNumPoints; ++i)
                {
                    Vector2<InputType> diff = mPoints[i] - line.origin;
                    InputType t = Dot(diff, line.direction);
                    if (t > tmax)
                    {
                        tmax = t;
                        imax = i;
                    }
                    else if (t < tmin)
                    {
                        tmin = t;
                        imin = i;
                    }
                }

                minBox.center = line.origin + C_<InputType>(1, 2) * (tmin + tmax) * line.direction;
                minBox.extent[0] = C_<InputType>(1, 2) * (tmax - tmin);
                minBox.extent[1] = C_<InputType>(0);
                minBox.axis[0] = line.direction;
                minBox.axis[1] = -Perp(line.direction);
                mHull.resize(2);
                mHull[0] = imin;
                mHull[1] = imax;
                return minBox;
            }

            mHull = ch2.GetHull();
            std::vector<Vector2<ComputeType>> computePoints(mHull.size());
            for (std::size_t i = 0; i < mHull.size(); ++i)
            {
                for (std::size_t j = 0; j < 2; ++j)
                {
                    computePoints[i][j] = points[mHull[i]][j];
                }
            }

            RemoveCollinearPoints(computePoints);

            Box box{};
            if (useRotatingCalipers)
            {
                box = ComputeBoxForEdgeOrderN(computePoints);
            }
            else
            {
                box = ComputeBoxForEdgeOrderNSqr(computePoints);
            }

            ConvertTo(box, computePoints, minBox);
            return minBox;
        }

        // The points are arbitrary, so the convex hull must be computed from
        // them to obtain the convex polygon whose minimum width is the
        // desired output.
        OrientedBox2<InputType> operator()(
            std::vector<Vector2<InputType>> const& points,
            bool useRotatingCalipers = !std::is_floating_point<ComputeType>::value)
        {
            return operator()(points.size(), points.data(), useRotatingCalipers);
        }

        // The points already form a counterclockwise, nondegenerate convex
        // polygon. If the points directly are the convex polygon, pass an
        // indices object with 0 elements. If the polygon vertices are a
        // subset of the incoming points, that subset is identified by
        // numIndices >= 3 and indices having numIndices elements.
        OrientedBox2<InputType> operator()(std::size_t numPoints,
            Vector2<InputType> const* points, std::size_t numIndices, std::size_t const* indices,
            bool useRotatingCalipers = !std::is_floating_point<ComputeType>::value)
        {
            mHull.clear();

            OrientedBox2<InputType> minBox{};

            if (numPoints < 3 || !points || (indices && numIndices < 3))
            {
                // Return an invalid box where all members are zero.
                return minBox;
            }

            if (indices)
            {
                mHull.resize(numIndices);
                std::copy(indices, indices + numIndices, mHull.begin());
            }
            else
            {
                numIndices = numPoints;
                mHull.resize(numIndices);
                for (std::size_t i = 0; i < numIndices; ++i)
                {
                    mHull[i] = i;
                }
            }

            std::vector<Vector2<ComputeType>> computePoints(numIndices);
            for (std::size_t i = 0; i < numIndices; ++i)
            {
                std::size_t h = mHull[i];
                computePoints[i][0] = static_cast<ComputeType>(points[h][0]);
                computePoints[i][1] = static_cast<ComputeType>(points[h][1]);
            }

            RemoveCollinearPoints(computePoints);

            Box box{};
            if (useRotatingCalipers)
            {
                box = ComputeBoxForEdgeOrderN(computePoints);
            }
            else
            {
                box = ComputeBoxForEdgeOrderNSqr(computePoints);
            }

            ConvertTo(box, computePoints, minBox);
            return minBox;
        }

        // The points already form a counterclockwise, nondegenerate convex
        // polygon. If the points directly are the convex polygon, pass an
        // indices object with 0 elements. If the polygon vertices are a
        // subset of the incoming points, that subset is identified by
        // numIndices >= 3 and indices having numIndices elements.
        OrientedBox2<InputType> operator()(
            std::vector<Vector2<InputType>> const& points,
            std::vector<std::size_t> const& indices,
            bool useRotatingCalipers = !std::is_floating_point<ComputeType>::value)
        {
            if (indices.size() > 0)
            {
                return operator()(points.size(), points.data(), indices.size(),
                    indices.data(), useRotatingCalipers);
            }
            else
            {
                return operator()(points, useRotatingCalipers);
            }
        }

        // Member access.
        inline std::size_t GetNumPoints() const
        {
            return mNumPoints;
        }

        inline Vector2<InputType> const* GetPoints() const
        {
            return mPoints;
        }

        inline std::vector<std::size_t> const& GetHull() const
        {
            return mHull;
        }

        inline std::array<std::size_t, 4> const& GetSupportIndices() const
        {
            return mSupportIndices;
        }

        inline InputType GetArea() const
        {
            return mArea;
        }

    private:
        // The box axes are U[i] and are usually not unit-length in order to
        // allow exact arithmetic. The box is supported by mPoints[index[i]],
        // where i is one of the enumerations above. The box axes are not
        // necessarily unit length, but they have the same length. They need
        // to be normalized for conversion back to InputType.
        struct Box
        {
            Box()
                :
                U{},
                index{ 0, 0, 0, 0 },
                sqrLenU0(C_<ComputeType>(0)),
                area(C_<ComputeType>(0))
            {
            }

            std::array<Vector2<ComputeType>, 2> U;
            std::array<std::size_t, 4> index;  // order: bottom, right, top, left
            ComputeType sqrLenU0, area;
        };

        // The rotating calipers algorithm has a loop invariant that requires
        // the convex polygon not to have collinear points. Any such points
        // must be removed first. The code is also executed for the O(n^2)
        // algorithm to reduce the number of process edges.
        void RemoveCollinearPoints(std::vector<Vector2<ComputeType>>& vertices)
        {
            std::vector<Vector2<ComputeType>> tmpVertices = vertices;

            std::size_t const numVertices = vertices.size();
            std::size_t numNoncollinear = 0;
            Vector2<ComputeType> ePrev = tmpVertices[0] - tmpVertices.back();
            for (std::size_t i0 = 0, i1 = 1; i0 < numVertices; ++i0)
            {
                Vector2<ComputeType> eNext = tmpVertices[i1] - tmpVertices[i0];

                ComputeType dp = DotPerp(ePrev, eNext);
                if (dp != C_<ComputeType>(0))
                {
                    vertices[numNoncollinear++] = tmpVertices[i0];
                }

                ePrev = eNext;
                if (++i1 == numVertices)
                {
                    i1 = 0;
                }
            }

            vertices.resize(numNoncollinear);
        }

        // This is the slow O(n^2) search.
        Box ComputeBoxForEdgeOrderNSqr(std::vector<Vector2<ComputeType>> const& vertices)
        {
            Box minBox{};
            minBox.area = C_<ComputeType>(-1);
            std::size_t const numIndices = vertices.size();
            for (std::size_t i0 = numIndices - 1, i1 = 0; i1 < numIndices; i0 = i1++)
            {
                Box box = SmallestBox(i0, i1, vertices);
                if (minBox.area == C_<ComputeType>(-1) || box.area < minBox.area)
                {
                    minBox = box;
                }
            }
            return minBox;
        }

        // The fast O(n) search.
        Box ComputeBoxForEdgeOrderN(std::vector<Vector2<ComputeType>> const& vertices)
        {
            // The inputs are assumed to be the vertices of a convex polygon
            // that is counterclockwise ordered. The input points must not
            // contain three consecutive collinear points.

            // When the bounding box corresponding to a polygon edge is
            // computed, we mark the edge as visited. If the edge is
            // encountered later, the algorithm terminates.
            std::vector<bool> visited(vertices.size());
            std::fill(visited.begin(), visited.end(), false);

            // Start the minimum-area rectangle search with the edge from the
            // last polygon vertex to the first. When updating the extremes,
            // we want the bottom-most point on the left edge, the top-most
            // point on the right edge, the left-most point on the top edge,
            // and the right-most point on the bottom edge. The polygon edges
            // starting at these points are then guaranteed not to coincide
            // with a box edge except when an extreme point is shared by two
            // box edges (at a corner).
            Box minBox = SmallestBox(vertices.size() - 1, 0, vertices);
            visited[minBox.index[0]] = true;

            // Execute the rotating calipers algorithm.
            Box box = minBox;
            for (std::size_t i = 0; i < vertices.size(); ++i)
            {
                std::array<std::pair<ComputeType, std::size_t>, 4> A{};
                std::size_t numA{};
                if (!ComputeAngles(vertices, box, A, numA))
                {
                    // The polygon is a rectangle, so the search is over.
                    break;
                }

                // Indirectly sort the A-array.
                std::array<std::size_t, 4> sort = SortAngles(A, numA);

                // Update the supporting indices (box.index[]) and the box
                // axis directions (box.U[]).
                if (!UpdateSupport(A, numA, sort, vertices, visited, box))
                {
                    // We have already processed the box polygon edge, so the
                    // search is over.
                    break;
                }

                if (box.area < minBox.area)
                {
                    minBox = box;
                }
            }

            return minBox;
        }

        // Compute the smallest box for the polygon edge <V[i0],V[i1]>.
        Box SmallestBox(std::size_t i0, std::size_t i1, std::vector<Vector2<ComputeType>> const& vertices)
        {
            Box box{};
            box.U[0] = vertices[i1] - vertices[i0];
            box.U[1] = -Perp(box.U[0]);
            box.index = { i1, i1, i1, i1 };
            box.sqrLenU0 = Dot(box.U[0], box.U[0]);

            Vector2<ComputeType> const& origin = vertices[i1];
            std::array<Vector2<ComputeType>, 4> support{};
            for (std::size_t j = 0; j < 4; ++j)
            {
                support[j] = { C_<ComputeType>(0), C_<ComputeType>(0) };
            }

            std::size_t i = 0;
            for (auto const& vertex : vertices)
            {
                Vector2<ComputeType> diff = vertex - origin;
                Vector2<ComputeType> v = { Dot(box.U[0], diff), Dot(box.U[1], diff) };

                // The right-most vertex of the bottom edge is vertices[i1].
                // The assumption of no triple of collinear vertices
                // guarantees that box.index[0] is i1, which is the initial
                // value assigned at the beginning of this function.
                // Therefore, there is no need to test for other vertices
                // farther to the right than vertices[i1].

                if (v[0] > support[1][0] ||
                    (v[0] == support[1][0] && v[1] > support[1][1]))
                {
                    // New right maximum OR same right maximum but closer
                    // to top.
                    box.index[1] = i;
                    support[1] = v;
                }

                if (v[1] > support[2][1] ||
                    (v[1] == support[2][1] && v[0] < support[2][0]))
                {
                    // New top maximum OR same top maximum but closer
                    // to left.
                    box.index[2] = i;
                    support[2] = v;
                }

                if (v[0] < support[3][0] ||
                    (v[0] == support[3][0] && v[1] < support[3][1]))
                {
                    // New left minimum OR same left minimum but closer
                    // to bottom.
                    box.index[3] = i;
                    support[3] = v;
                }

                ++i;
            }

            // The comment in the loop has the implication that
            // support[0] = { 0, 0 }, so the scaled height
            // (support[2][1] - support[0][1]) is simply support[2][1].
            ComputeType scaledWidth = support[1][0] - support[3][0];
            ComputeType scaledHeight = support[2][1];
            box.area = scaledWidth * scaledHeight / box.sqrLenU0;
            return box;
        }

        // Compute (sin(angle))^2 for the polygon edges emanating from the
        // support vertices of the box. The return value is 'true' if at
        // least one angle is in [0,pi/2); otherwise, the return value is
        // 'false' and the original polygon must be a rectangle.
        bool ComputeAngles(std::vector<Vector2<ComputeType>> const& vertices,
            Box const& box, std::array<std::pair<ComputeType, std::size_t>, 4>& A,
            std::size_t& numA) const
        {
            std::size_t const numVertices = vertices.size();
            numA = 0;
            for (std::size_t k0 = 3, k1 = 0; k1 < 4; k0 = k1++)
            {
                if (box.index[k0] != box.index[k1])
                {
                    // The box edges are ordered in k1 as U[0], U[1],
                    // -U[0], -U[1].
                    Vector2<ComputeType> D = ((k0 & 2) ? -box.U[k0 & 1] : box.U[k0 & 1]);
                    std::size_t j0 = box.index[k0], j1 = j0 + 1;
                    if (j1 == numVertices)
                    {
                        j1 = 0;
                    }
                    Vector2<ComputeType> E = vertices[j1] - vertices[j0];
                    ComputeType dp = DotPerp(D, E);
                    ComputeType esqrlen = Dot(E, E);
                    ComputeType sinThetaSqr = (dp * dp) / esqrlen;
                    A[numA] = std::make_pair(sinThetaSqr, k0);
                    ++numA;
                }
            }
            return numA > 0;
        }

        // Sort the angles indirectly. The sorted indices are returned. This
        // avoids swapping elements of A[], which can be expensive when
        // ComputeType is an exact rational type.
        std::array<std::size_t, 4> SortAngles(
            std::array<std::pair<ComputeType, std::size_t>, 4> const& A,
            std::size_t numA) const
        {
            std::array<std::size_t, 4> sort = { 0, 1, 2, 3 };
            if (numA > 1)
            {
                if (numA == 2)
                {
                    if (A[sort[0]].first > A[sort[1]].first)
                    {
                        std::swap(sort[0], sort[1]);
                    }
                }
                else if (numA == 3)
                {
                    if (A[sort[0]].first > A[sort[1]].first)
                    {
                        std::swap(sort[0], sort[1]);
                    }
                    if (A[sort[0]].first > A[sort[2]].first)
                    {
                        std::swap(sort[0], sort[2]);
                    }
                    if (A[sort[1]].first > A[sort[2]].first)
                    {
                        std::swap(sort[1], sort[2]);
                    }
                }
                else  // numA == 4
                {
                    if (A[sort[0]].first > A[sort[1]].first)
                    {
                        std::swap(sort[0], sort[1]);
                    }
                    if (A[sort[2]].first > A[sort[3]].first)
                    {
                        std::swap(sort[2], sort[3]);
                    }
                    if (A[sort[0]].first > A[sort[2]].first)
                    {
                        std::swap(sort[0], sort[2]);
                    }
                    if (A[sort[1]].first > A[sort[3]].first)
                    {
                        std::swap(sort[1], sort[3]);
                    }
                    if (A[sort[1]].first > A[sort[2]].first)
                    {
                        std::swap(sort[1], sort[2]);
                    }
                }
            }
            return sort;
        }

        bool UpdateSupport(std::array<std::pair<ComputeType, std::size_t>, 4> const& A,
            std::size_t numA, std::array<std::size_t, 4> const& sort,
            std::vector<Vector2<ComputeType>> const& vertices,
            std::vector<bool>& visited, Box& box)
        {
            // Replace the support vertices of those edges attaining minimum
            // angle with the other endpoints of the edges.
            std::size_t const numVertices = vertices.size();
            auto const& amin = A[sort[0]];
            for (std::size_t k = 0; k < numA; ++k)
            {
                auto const& a = A[sort[k]];
                if (a.first == amin.first)
                {
                    if (++box.index[a.second] == numVertices)
                    {
                        box.index[a.second] = 0;
                    }
                }
            }

            std::size_t bottom = box.index[amin.second];
            if (visited[bottom])
            {
                // We have already processed this polygon edge.
                return false;
            }
            visited[bottom] = true;

            // Cycle the vertices so that the bottom support occurs first.
            std::array<std::size_t, 4> nextIndex{};
            for (std::size_t k = 0; k < 4; ++k)
            {
                nextIndex[k] = box.index[(amin.second + k) % 4];
            }
            box.index = nextIndex;

            // Compute the box axis directions.
            std::size_t j1 = box.index[0], j0 = j1 - 1;
            if (j0 == std::numeric_limits<std::size_t>::max())
            {
                j0 = numVertices - 1;
            }
            box.U[0] = vertices[j1] - vertices[j0];
            box.U[1] = -Perp(box.U[0]);
            box.sqrLenU0 = Dot(box.U[0], box.U[0]);

            // Compute the box area.
            std::array<Vector2<ComputeType>, 2> diff =
            {
                vertices[box.index[1]] - vertices[box.index[3]],
                vertices[box.index[2]] - vertices[box.index[0]]
            };
            box.area = Dot(box.U[0], diff[0]) * Dot(box.U[1], diff[1]) / box.sqrLenU0;
            return true;
        }

        // Convert the ComputeType box to the InputType box. When the
        // ComputeType is an exact rational type, the conversions are
        // performed to avoid precision loss until necessary at the last step.
        void ConvertTo(Box const& minBox,
            std::vector<Vector2<ComputeType>> const& computePoints,
            OrientedBox2<InputType>& itMinBox)
        {
            // The sum, difference, and center are all computed exactly.
            std::array<Vector2<ComputeType>, 2> sum =
            {
                computePoints[minBox.index[1]] + computePoints[minBox.index[3]],
                computePoints[minBox.index[2]] + computePoints[minBox.index[0]]
            };

            std::array<Vector2<ComputeType>, 2> difference =
            {
                computePoints[minBox.index[1]] - computePoints[minBox.index[3]],
                computePoints[minBox.index[2]] - computePoints[minBox.index[0]]
            };

            Vector2<ComputeType> center = C_<ComputeType>(1, 2) * (
                Dot(minBox.U[0], sum[0]) * minBox.U[0] +
                Dot(minBox.U[1], sum[1]) * minBox.U[1]) / minBox.sqrLenU0;

            // Calculate the squared extent using ComputeType to avoid loss of
            // precision before computing a squared root.
            Vector2<ComputeType> sqrExtent{};
            for (std::size_t i = 0; i < 2; ++i)
            {
                sqrExtent[i] = C_<ComputeType>(1, 2) * Dot(minBox.U[i], difference[i]);
                sqrExtent[i] *= sqrExtent[i];
                sqrExtent[i] /= minBox.sqrLenU0;
            }

            for (std::size_t i = 0; i < 2; ++i)
            {
                itMinBox.center[i] = static_cast<InputType>(center[i]);
                itMinBox.extent[i] = std::sqrt(static_cast<InputType>(sqrExtent[i]));

                // Before converting to floating-point, factor out the maximum
                // component using ComputeType to generate rational numbers in
                // a range that avoids loss of precision during the conversion
                // and normalization.
                Vector2<ComputeType> const& axis = minBox.U[i];
                ComputeType cmax = std::max(std::fabs(axis[0]), std::fabs(axis[1]));
                ComputeType invCMax = C_<ComputeType>(1) / cmax;
                for (std::size_t j = 0; j < 2; ++j)
                {
                    itMinBox.axis[i][j] = static_cast<InputType>(axis[j] * invCMax);
                }
                Normalize(itMinBox.axis[i]);
            }

            mSupportIndices = minBox.index;
            mArea = static_cast<InputType>(minBox.area);
        }

        // The input points to be bound.
        std::size_t mNumPoints;
        Vector2<InputType> const* mPoints;

        // The indices into mPoints for the convex hull
        // vertices.
        std::vector<std::size_t> mHull;

        // The support indices for the minimum-area box.
        std::array<std::size_t, 4> mSupportIndices;

        // The area of the minimum-area box. The ComputeType value is exact,
        // so the only rounding errors occur in the conversion from
        // ComputeType to InputType (default rounding mode is
        // round-to-nearest-ties-to-even).
        InputType mArea;

    private:
        friend class UnitTestMinimumAreaBox2;
    };
}
