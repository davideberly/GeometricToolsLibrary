// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.06

#pragma once

// Triangulate polygons using ear clipping. The algorithm is described in
// https://www.geometrictools.com/Documentation/TriangulationByEarClipping.pdf

#include <GTL/Mathematics/Geometry/2D/PolygonTree.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <memory>
#include <numeric>
#include <queue>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename InputType, typename ComputeType>
    class TriangulateEC
    {
    public:
        // The fundamental problem is to compute the triangulation of a
        // polygon tree. The outer polygons have counterclockwise ordered
        // vertices. The inner polygons have clockwise ordered vertices.
        using Polygon = std::vector<std::size_t>;

        // The class is a functor to support triangulating multiple polygons
        // that share vertices in a collection of points. The interpretation
        // of 'numPoints' and 'points' is described before each operator()
        // function. Preconditions are numPoints >= 3 and points is a nonnull
        // pointer to an array of at least numPoints elements. If they are not
        // satisfied, an exception is thrown.
        TriangulateEC(std::size_t numPoints, Vector2<InputType> const* points)
            :
            mNumPoints(numPoints),
            mPoints(points),
            mTriangles{},
            mComputePoints{},
            mConverted{},
            mVertexList{}
        {
            GTL_ARGUMENT_ASSERT(
                numPoints >= 3 && points != nullptr,
                "Invalid input.");

            mComputePoints.resize(mNumPoints);
            mConverted.resize(mNumPoints);
            std::fill(mConverted.begin(), mConverted.end(), false);
        }

        TriangulateEC(std::vector<Vector2<InputType>> const& points)
            :
            TriangulateEC(points.size(), points.data())
        {
        }

        // Access the triangulation after each operator() call.
        inline std::vector<std::array<std::size_t, 3>> const& GetTriangles() const
        {
            return mTriangles;
        }

        // The input 'points' represents an array of vertices for a simple
        // polygon. The vertices are points[0] through points[numPoints-1] and
        // are listed in counterclockwise order.
        void operator()()
        {
            mTriangles.clear();
            Polygon polygon(mNumPoints);
            std::iota(polygon.begin(), polygon.end(), 0);
            operator()(polygon);
        }

        // The input 'points' represents an array of vertices that contains
        // the vertices of a simple polygon.
        void operator()(Polygon const& polygon)
        {
            mTriangles.clear();

            // Convert InputType polygon vertices to ComputeType, the latter
            // type presumably an exact rational type.
            ConvertPoints(polygon);

            // Triangulate the simple polygon using ear clipping.
            mVertexList.DoEarClipping(*this, polygon, mTriangles);
        }

        // The input 'points' is a shared array of vertices that contains the
        // vertices for two simple polygons, an outer polygon and an inner
        // polygon. The inner polygon must be strictly inside the outer
        // polygon.
        void operator()(Polygon const& outer, Polygon const& inner)
        {
            mTriangles.clear();

            // Convert InputType polygon vertices to ComputeType, the latter
            // type presumably an exact rational type.
            ConvertPoints(outer, inner);

            // Combine the inner and outer polygon into a pseudosimple
            // polygon.
            Polygon combined{};
            CombineSingle(outer, inner, combined);

            // Triangulate the pseudosimple polygon using ear clipping.
            mVertexList.DoEarClipping(*this, combined, mTriangles);
        }

        // The input 'points' is a shared array of vertices that contains the
        // vertices for multiple simple polygons, an outer polygon and one or
        // more inner polygons. The inner polygons must be nonoverlapping and
        // strictly inside the outer polygon.
        void operator()(Polygon const& outer, std::vector<Polygon> const& inners)
        {
            mTriangles.clear();

            // Convert InputType polygon vertices to ComputeType, the latter
            // type presumably an exact rational type.
            ConvertPoints(outer, inners);

            // Combine the outer polygon and the inner polygons into a
            // pseudosimple polygon using repeated calls to CombineSimple.
            Polygon combined{};
            CombineMultiple(outer, inners, combined);

            // Triangulate the pseudosimple polygon using ear clipping.
            mVertexList.DoEarClipping(*this, combined, mTriangles);
        }

        // The input 'positions' is a shared array of vertices that contains
        // the vertices for multiple simple polygons in a tree of polygons.
        void operator()(std::shared_ptr<PolygonTree> const& tree)
        {
            mTriangles.clear();

            // Convert InputType polygon vertices to ComputeType, the latter
            // type presumably an exact rational type.
            ConvertPoints(tree);

            std::queue<std::shared_ptr<PolygonTree>> treeQueue{};
            treeQueue.push(tree);
            while (!treeQueue.empty())
            {
                std::shared_ptr<PolygonTree> outer = treeQueue.front();
                treeQueue.pop();

                std::size_t numChildren = outer->child.size();
                if (numChildren == 0)
                {
                    // The outer polygon is a simple polygon that has no
                    // nested inner polygons. Triangulate the pseudosimple
                    // polygon using ear clipping.
                    std::vector<std::array<std::size_t, 3>> combinedTriangles{};
                    mVertexList.DoEarClipping(*this, outer->polygon, combinedTriangles);
                    mTriangles.insert(mTriangles.end(), combinedTriangles.begin(), combinedTriangles.end());
                }
                else
                {
                    // Place the next level of outer polygon nodes on the
                    // queue for triangulation.
                    std::vector<Polygon> inners(numChildren);
                    for (std::size_t c = 0; c < numChildren; ++c)
                    {
                        std::shared_ptr<PolygonTree> inner = outer->child[c];
                        inners[c] = inner->polygon;
                        std::size_t numGrandChildren = inner->child.size();
                        for (std::size_t g = 0; g < numGrandChildren; ++g)
                        {
                            treeQueue.push(inner->child[g]);
                        }
                    }

                    // Combine the outer polygon and the inner polygons into a
                    // pseudosimple polygon using repeated calls to
                    // CombineSimple.
                    Polygon combined{};
                    CombineMultiple(outer->polygon, inners, combined);

                    // Triangulate the pseudosimple polygon using ear clipping.
                    std::vector<std::array<std::size_t, 3>> combinedTriangles{};
                    mVertexList.DoEarClipping(*this, combined, combinedTriangles);
                    mTriangles.insert(mTriangles.end(), combinedTriangles.begin(), combinedTriangles.end());
                }
            }
        }

    private:
        // The input vertex pool.
        std::size_t const mNumPoints;
        Vector2<InputType> const* mPoints;

        // The output triangulation.
        std::vector<std::array<std::size_t, 3>> mTriangles;

    private:
        // For a line with origin V0 and direction V1-V0, operator() returns
        //   +1, P on right of line
        //   -1, P on left of line
        //    0, P on the line
        std::int32_t ToLine(Vector2<ComputeType> const& P, Vector2<ComputeType> const& V0,
            Vector2<ComputeType> const& V1) const
        {
            ComputeType x0 = P[0] - V0[0];
            ComputeType y0 = P[1] - V0[1];
            ComputeType x1 = V1[0] - V0[0];
            ComputeType y1 = V1[1] - V0[1];
            ComputeType x0y1 = x0 * y1;
            ComputeType x1y0 = x1 * y0;
            ComputeType det = x0y1 - x1y0;
            return det.GetSign();
        }

        std::int32_t ToLine(std::size_t pIndex, std::size_t v0Index, std::size_t v1Index) const
        {
            Vector2<ComputeType> const& P = mComputePoints[pIndex];
            Vector2<ComputeType> const& V0 = mComputePoints[v0Index];
            Vector2<ComputeType> const& V1 = mComputePoints[v1Index];
            return ToLine(P, V0, V1);
        };

        // For a triangle with counterclockwise vertices V0, V1 and V2, operator()
        // returns
        //   +1, P outside triangle
        //   -1, P inside triangle
        //    0, P on triangle
        std::int32_t ToTriangle(Vector2<ComputeType> const& P, Vector2<ComputeType> const& V0,
            Vector2<ComputeType> const& V1, Vector2<ComputeType> const& V2) const
        {
            std::int32_t sign0 = ToLine(P, V1, V2);
            if (sign0 > 0)
            {
                return +1;
            }

            std::int32_t sign1 = ToLine(P, V0, V2);
            if (sign1 < 0)
            {
                return +1;
            }

            std::int32_t sign2 = ToLine(P, V0, V1);
            if (sign2 > 0)
            {
                return +1;
            }

            return ((sign0 && sign1 && sign2) ? -1 : 0);
        }

        std::int32_t ToTriangle(std::size_t pIndex, std::size_t v0Index, std::size_t v1Index, std::size_t v2Index) const
        {
            Vector2<ComputeType> const& P = mComputePoints[pIndex];
            Vector2<ComputeType> const& V0 = mComputePoints[v0Index];
            Vector2<ComputeType> const& V1 = mComputePoints[v1Index];
            Vector2<ComputeType> const& V2 = mComputePoints[v2Index];
            return ToTriangle(P, V0, V1, V2);
        }

        void ConvertPoints(Polygon const& polygon)
        {
            for (auto const& index : polygon)
            {
                if (mConverted[index] == 0)
                {
                    mConverted[index] = 1;
                    for (std::size_t j = 0; j < 2; ++j)
                    {
                        mComputePoints[index][j] = static_cast<ComputeType>(mPoints[index][j]);
                    }
                }
            }
        }

        void ConvertPoints(Polygon const& outer, Polygon const& inner)
        {
            for (auto index : outer)
            {
                if (mConverted[index] == 0)
                {
                    mConverted[index] = 1;
                    for (std::size_t j = 0; j < 2; ++j)
                    {
                        mComputePoints[index][j] = static_cast<ComputeType>(mPoints[index][j]);
                    }
                }
            }

            for (auto index : inner)
            {
                if (mConverted[index] == 0)
                {
                    mConverted[index] = 1;
                    for (std::size_t j = 0; j < 2; ++j)
                    {
                        mComputePoints[index][j] = static_cast<ComputeType>(mPoints[index][j]);
                    }
                }
            }
        }

        void ConvertPoints(Polygon const& outer, std::vector<Polygon> const& inners)
        {
            for (auto index : outer)
            {
                if (mConverted[index] == 0)
                {
                    mConverted[index] = 1;
                    for (std::size_t j = 0; j < 2; ++j)
                    {
                        mComputePoints[index][j] = static_cast<ComputeType>(mPoints[index][j]);
                    }
                }
            }

            for (auto const& inner : inners)
            {
                for (auto index : inner)
                {
                    if (mConverted[index] == 0)
                    {
                        mConverted[index] = 1;
                        for (std::size_t j = 0; j < 2; ++j)
                        {
                            mComputePoints[index][j] = static_cast<ComputeType>(mPoints[index][j]);
                        }
                    }
                }
            }
        }

        void ConvertPoints(std::shared_ptr<PolygonTree> const& tree)
        {
            std::queue<std::shared_ptr<PolygonTree>> treeQueue{};
            treeQueue.push(tree);
            while (!treeQueue.empty())
            {
                // The 'root' is an outer polygon.
                std::shared_ptr<PolygonTree> outer = treeQueue.front();
                treeQueue.pop();

                for (auto index : outer->polygon)
                {
                    if (mConverted[index] == 0)
                    {
                        mConverted[index] = 1;
                        for (std::size_t j = 0; j < 2; ++j)
                        {
                            mComputePoints[index][j] = static_cast<ComputeType>(mPoints[index][j]);
                        }
                    }
                }

                // The grandchildren of the outer polygon are also outer
                // polygons. Insert them into the queue for processing.
                std::size_t numChildren = outer->child.size();
                for (std::size_t c = 0; c < numChildren; ++c)
                {
                    // The 'child' is an inner polygon.
                    std::shared_ptr<PolygonTree> inner = outer->child[c];
                    for (auto index : inner->polygon)
                    {
                        if (mConverted[index] == 0)
                        {
                            mConverted[index] = 1;
                            for (std::size_t j = 0; j < 2; ++j)
                            {
                                mComputePoints[index][j] = static_cast<ComputeType>(mPoints[index][j]);
                            }
                        }
                    }

                    std::size_t numGrandChildren = inner->child.size();
                    for (std::size_t g = 0; g < numGrandChildren; ++g)
                    {
                        treeQueue.push(inner->child[g]);
                    }
                }
            }
        }

        // The array of points used for geometric queries. If you want to be
        // certain of a correct result, choose ComputeType to be BSRational.
        // The InputType points are converted to ComputeType points on demand.
        // The mConverted array keeps track of which input points have been
        // converted.
        std::vector<Vector2<ComputeType>> mComputePoints;
        std::vector<std::uint32_t> mConverted;

    private:
        static std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();

        // Find the edge whose intersection point I with the ray M + t * (1,0)
        // minimizes the ray parameter t >= 0. The inputs v0min, v1min and
        // endMin must be initialized to 'invalid'.
        Vector2<ComputeType> ComputeNearestOuterPolygonIntersection(
            Vector2<ComputeType> const& M, Polygon const& outer,
            std::size_t& v0min, std::size_t& v1min, std::size_t& endMin) const
        {
            auto const zero = static_cast<ComputeType>(0);
            auto const infinite = static_cast<ComputeType>(-1);
            ComputeType t = infinite;
            ComputeType tIntersect = infinite;
            for (std::size_t i0 = outer.size() - 1, i1 = 0; i1 < outer.size(); i0 = i1++)
            {
                // Test whether edge <V0,V1> is degenerate.
                auto const& V0 = mComputePoints[outer[i0]];
                auto const& V1 = mComputePoints[outer[i1]];
                GTL_RUNTIME_ASSERT(
                    V0 != V1,
                    "Edge <V[" + std::to_string(outer[i0]) + "],V[" + std::to_string(outer[i1]) + "]> is degenerate.");

                // Test whether V0 = M, which is not allowed.
                Vector2<ComputeType> D0 = V0 - M;
                GTL_RUNTIME_ASSERT(
                    D0 != Vector2<ComputeType>::Zero(),
                    "V0 = M, which violates strict outer-inner containment.");

                // Test whether V1 = M, which is not allowed.
                Vector2<ComputeType> D1 = V1 - M;
                GTL_RUNTIME_ASSERT(
                    D1 != Vector2<ComputeType>::Zero(),
                    "V1 = M, which violates strict outer-inner containment.");

                // The CCW triangle <M,V0,V1> is not degenerate. The ray is
                // parameterized by M + t * (1,0) for t >= 0. The edge is
                // parameterized by V0 + s * (V1 - V0) for 0 <= s <= 1. Solve
                // M + t * (1,0) = V0 + s * (V1 - V0). Subtract M from both
                // sides of the equation
                //   t * (1,0) = (V0 - M) + s * (V1 - V0)
                //             = (V0 - M) + s * ((V1 - M) - (V0 - M))
                //             = D0 + s * (D1 - D0)
                //   (t, 0) = (D0[0]+s*(D1[0]-D0[0]), D0[1]+s*(D1[1]-D0[1]))
                // Solve for
                //   s = D0[1] / (D0[1] - D1[1])
                //   t = D0[0] + s * (D1[0] - D0[0])
                //
                // In the cases listed next, keep in mind that M is strictly
                // left-of the candidate edge.
                // 1. D0[1] > 0: V0 is strictly above the ray, no intersection
                // 2. D1[1] < 0: V1 is strictly below the ray, no intersection
                // 3. D0[1] < 0, D1[1] > 0: s in (0,1), t > 0 (edge point I)
                // 4. D0[1] < 0, D1[1] = 0: s = 1, t = D1[0] > 0 (vertex V1)
                // 5. D0[1] = 0, D1[1] > 0: s = 0, t = D0[0] > 0 (vertex V0)
                // In the next case, the ray and edge are coincident.
                // 6. D0[1] = 0, D1[1] = 0, D0[0] < D1[0]:
                //    s = 0, t = D0[0] > 0 (vertex V0)
                // 7. D0[1] = 0, D1[1] = 0, D0[0] > D1[0]:
                //    s = 1, t = D1[0] > 0 (vertex V1)
                // The case D0[1] = 0, D1[1] = 0, D0[0] = D1[0] cannot happen
                // here because we trapped equality of V0 and V1 previously.

                if (D0[1] > zero || D1[1] < zero)
                {
                    // Cases 1 and 2.
                    continue;
                }

                std::size_t currentEndMin = invalid;
                if (D0[1] < zero)
                {
                    if (D1[1] > zero)
                    {
                        // Case 3: s in (0,1), t > 0
                        ComputeType s = D0[1] / (D0[1] - D1[1]);
                        t = D0[0] + s * (D1[0] - D0[0]);
                    }
                    else  // D1[1] = zero
                    {
                        // Case 4: s = 1, t > 0
                        t = D1[0];
                        currentEndMin = i1;
                    }
                }
                else // D1[1] = zero
                {
                    if (D1[1] > zero)
                    {
                        // Case 5: s = 0, t > 0
                        t = D0[0];
                        currentEndMin = i0;
                    }
                    else
                    {
                        if (D0[0] < D1[0])
                        {
                            // Case 6: s = 0, t > 0
                            t = D0[0];
                            currentEndMin = i0;
                        }
                        else if (D0[0] > D1[0])
                        {
                            // Case 7: s = 1, t > 0
                            t = D1[0];
                            currentEndMin = i1;
                        }
                        else
                        {
                            GTL_RUNTIME_ERROR("This is the case V0 == V1, which was trapped previously.");
                        }
                    }
                }

                if (tIntersect == infinite || (zero < t && t < tIntersect))
                {
                    // This block is always entered the first time a finite
                    // t-value is computed.
                    tIntersect = t;
                    v0min = i0;
                    v1min = i1;
                    if (currentEndMin == invalid)
                    {
                        // The current closest point is edge-interior.
                        endMin = invalid;
                    }
                    else
                    {
                        // The current closest point is a vertex.
                        endMin = currentEndMin;
                    }
                }
                else if (t == tIntersect)
                {
                    // The current closest point is a vertex shared by
                    // multiple edges; thus, endMin and currentMin refer to
                    // the same point.
                    GTL_RUNTIME_ASSERT(
                        endMin != invalid && currentEndMin == endMin,
                        "Unexpected condition.");

                    // We need to select the edge closest to M. The previous
                    // closest edge has polygon indices <outer[i0], outer[i1]>
                    // and the current candidate edge has polygon indices
                    // <outer[v0min], outer[v1min]>.
                    Vector2<ComputeType> const& shared = mComputePoints[outer[i1]];

                    // For the previous closest edge, endMin refers to a
                    // vertex of the edge. Get the index of the other vertex.
                    std::size_t other = (endMin == v0min ? v1min : v0min);

                    // The new edge is closer if the other vertex of the old
                    // edge is left-of the new edge.
                    D0 = mComputePoints[outer[i0]] - shared;
                    D1 = mComputePoints[outer[other]] - shared;
                    ComputeType dotperp = DotPerp(D0, D1);
                    if (dotperp > zero)
                    {
                        // The new edge is closer to M.
                        v0min = i0;
                        v1min = i1;
                        endMin = currentEndMin;
                    }
                }
            }

            // If you reach this assert, you might have two inner polygons
            // that share a vertex or an edge.
            GTL_RUNTIME_ASSERT(
                v0min != invalid && v1min != invalid,
                "Is this an invalid nested polygon?");

            // Return the intersection point I = M + tIntersect * (1,0).
            return Vector2<ComputeType>{ M[0] + tIntersect, M[1] };
        }

        std::size_t LocateOuterVisibleVertex(Vector2<ComputeType> const& M,
            Vector2<ComputeType> const& I, Polygon const& outer,
            std::size_t v0min, std::size_t v1min, std::size_t endMin) const
        {
            // The point mPoints[outer[oVisibleIndex]] maximizes the cosine
            // of the angle between <M,I> and <M,Q> where Q is P or a reflex
            // vertex contained in triangle <M,I,P>.
            std::size_t oVisibleIndex = endMin;
            if (endMin == invalid)
            {
                // Select mPoints[outer[v0min]] or mPoints[outer[v1min]] that
                // has an x-value larger than M.x, call this vertex P. The
                // triangle <M,I,P> must contain an outer-polygon vertex that
                // is visible to M, which is possibly P itself.
                std::array<Vector2<ComputeType>, 3> triangle{};
                std::size_t pIndex{};
                if (mComputePoints[outer[v0min]][0] > mComputePoints[outer[v1min]][0])
                {
                    auto const& P = mComputePoints[outer[v0min]];
                    triangle[0] = P;
                    triangle[1] = I;
                    triangle[2] = M;
                    pIndex = v0min;
                }
                else
                {
                    auto const& P = mComputePoints[outer[v1min]];
                    triangle[0] = P;
                    triangle[1] = M;
                    triangle[2] = I;
                    pIndex = v1min;
                }

                // If any outer-polygon vertices other than P are inside the
                // triangle <M,I,P>, then at least one of these vertices must
                // be a reflex vertex. It is sufficient to locate the reflex
                // vertex R (if any) in <M,I,P> that minimizes the angle
                // between R-M and (1,0).
                Vector2<ComputeType> diff = triangle[0] - M;
                ComputeType maxSqrLen = Dot(diff, diff);
                ComputeType maxCos = diff[0] * diff[0] / maxSqrLen;
                std::size_t const numOuter = outer.size();
                oVisibleIndex = pIndex;
                for (std::size_t i = 0; i < numOuter; ++i)
                {
                    if (i == pIndex)
                    {
                        continue;
                    }

                    std::size_t curr = outer[i];
                    std::size_t prev = outer[(i + numOuter - 1) % numOuter];
                    std::size_t next = outer[(i + 1) % numOuter];
                    if (ToLine(curr, prev, next) <= 0 &&
                        ToTriangle(mComputePoints[curr], triangle[0], triangle[1], triangle[2]) <= 0)
                    {
                        // The vertex is reflex and inside the <M,I,P>
                        // triangle.
                        diff = mComputePoints[curr] - M;
                        ComputeType sqrLen = Dot(diff, diff);
                        ComputeType cs = diff[0] * diff[0] / sqrLen;
                        if (cs > maxCos)
                        {
                            // The reflex vertex forms a smaller angle with
                            // the positive x-axis, so it becomes the new
                            // visible candidate.
                            maxSqrLen = sqrLen;
                            maxCos = cs;
                            oVisibleIndex = i;
                        }
                        else if (cs == maxCos && sqrLen < maxSqrLen)
                        {
                            // The reflex vertex has angle equal to the
                            // current minimum but the length is smaller, so
                            // it becomes the new visible candidate.
                            maxSqrLen = sqrLen;
                            oVisibleIndex = i;
                        }
                    }
                }
            }

            return oVisibleIndex;
        }

        void CombineSingle(Polygon const& outer, Polygon const& inner,
            Polygon& combined)
        {
            // Get the index into inner[] for the inner-polygon vertex M of
            // maximum x-value. The xmaxInfo pair.first is the maximum x-value
            // of the polygon vertices. The xmaxInfo pair.second is the index
            // of the vertex that generates a maximum x-value. It is not a
            // problem if the maximum is attained by more than one vertex. It
            // is sufficient to use mPoints directly because the InputType
            // comparisons are exact.
            InputType xmax = mPoints[inner[0]][0];
            size_t iVisibleIndex = 0;
            for (size_t i = 1; i < inner.size(); ++i)
            {
                InputType const& x = mPoints[inner[i]][0];
                if (x > xmax)
                {
                    xmax = x;
                    iVisibleIndex = i;
                }
            }

            // Get the inner-polygon vertex M of maximum x-value.
            std::size_t iVertexIndex = inner[iVisibleIndex];
            Vector2<ComputeType> const& M = mComputePoints[iVertexIndex];

            // Compute the closest outer-polygon point I along the ray
            // M + t *(1,0) with t > 0 so that M and I are mutually visible.
            std::size_t v0min = invalid, v1min = invalid, endMin = invalid;
            Vector2<ComputeType> I = ComputeNearestOuterPolygonIntersection(
                M, outer, v0min, v1min, endMin);

            // Locate Q = mPoints[outer[oVisibleIndex]] so that M and Q are
            // mutually visible.
            std::size_t oVisibleIndex = LocateOuterVisibleVertex(
                M, I, outer, v0min, v1min, endMin);

            InsertBridge(outer, inner, oVisibleIndex, iVisibleIndex, combined);
        }

        void CombineMultiple(Polygon const& outer, std::vector<Polygon> const& inners,
            Polygon& combined)
        {
            // Sort the inner polygons based on maximum x-values.
            using PairType = std::pair<ComputeType, std::size_t>;
            std::size_t numInners = inners.size();
            std::vector<PairType> pairs(numInners);
            for (std::size_t p = 0; p < numInners; ++p)
            {
                std::size_t numIndices = inners[p].size();
                std::size_t const* indices = inners[p].data();

                InputType xmax = mPoints[indices[0]][0];
                for (std::size_t i = 1; i < numIndices; ++i)
                {
                    InputType x = mPoints[indices[i]][0];
                    if (x > xmax)
                    {
                        xmax = x;
                    }
                }

                pairs[p].first = xmax;
                pairs[p].second = p;
            }
            std::sort(pairs.begin(), pairs.end(), std::greater<PairType>());

            Polygon currentOuter = outer;
            for (auto const& pair : pairs)
            {
                Polygon const& inner = inners[pair.second];
                Polygon currentCombined{};
                CombineSingle(currentOuter, inner, currentCombined);
                currentOuter = std::move(currentCombined);
            }
            combined = std::move(currentOuter);
        }

        // The mutually visible vertices are VI = mPoints[inner[iVisibleIndex]]
        // and VO = mPoints[outer[oVisibleIndex]]. Two coincident edges with
        // these endpoints are inserted to connect the outer and inner polygons
        // into a pseudosimple polygon.
        void InsertBridge(Polygon const& outer, Polygon const& inner,
            std::size_t oVisibleIndex, std::size_t iVisibleIndex, Polygon& combined)
        {
            std::size_t const numOuter = outer.size();
            std::size_t const numInner = inner.size();
            combined.resize(numOuter + numInner + 2);

            // Traverse the outer polygon until the outer polygon bridge.
            // point is visited.
            std::size_t cIndex = 0;
            for (std::size_t i = 0; i <= oVisibleIndex; ++i, ++cIndex)
            {
                combined[cIndex] = outer[i];
            }

            // Cross the bridge from the outer polygon to the inner polygon.
            // Traverse the inner polygon until the predecessor of the inner
            // polygon bridge point is visited.
            for (std::size_t i = 0; i < numInner; ++i, ++cIndex)
            {
                std::size_t j = (iVisibleIndex + i) % numInner;
                combined[cIndex] = inner[j];
            }

            // Inner polygon bridge point.
            combined[cIndex++] = inner[iVisibleIndex];

            // Outer polygon bridge point.
            combined[cIndex++] = outer[oVisibleIndex];

            for (std::size_t i = oVisibleIndex + 1; i < numOuter; ++i, ++cIndex)
            {
                combined[cIndex] = outer[i];
            }
        }

    private:
        // A doubly linked list for storing specially tagged vertices (convex,
        // reflex, ear). The vertex list is used for ear clipping.
        static std::size_t const negOne = std::numeric_limits<std::size_t>::max();

        using Triangulator = typename TriangulateEC<InputType, ComputeType>;

        struct Vertex
        {
            Vertex()
                :
                index(negOne),
                vPrev(negOne),
                vNext(negOne),
                sPrev(negOne),
                sNext(negOne),
                ePrev(negOne),
                eNext(negOne),
                isConvex(false),
                isEar(false)
            {
            }

            std::size_t index;           // index of vertex in mPoints array
            std::size_t vPrev, vNext;    // vertex links for polygon
            std::size_t sPrev, sNext;    // convex/reflex vertex links (disjoint lists)
            std::size_t ePrev, eNext;    // ear links
            bool isConvex, isEar;
        };

        class VertexList
        {
        public:
            VertexList()
                :
                mVertices{},
                mCFirst(negOne),
                mCLast(negOne),
                mRFirst(negOne),
                mRLast(negOne),
                mEFirst(negOne),
                mELast(negOne)
            {
            }

            void DoEarClipping(
                Triangulator& triangulator,
                Polygon const& polygon,
                std::vector<std::array<std::size_t, 3>>& triangles)
            {
                triangles.clear();

                // Initialize the vertex list for the incoming polygon.
                // The lists must be cleared in case a single VertexList
                // object is used two or more times in triangulation
                // queries. This is the case for triangulating a polygon
                // tree. It is also the case if you use a single
                // TriangulateEC object for multiple triangulation queries.
                mVertices.resize(polygon.size());
                mCFirst = negOne;
                mCLast = negOne;
                mRFirst = negOne;
                mRLast = negOne;
                mEFirst = negOne;
                mELast = negOne;

                // Create a circular list of the polygon vertices for dynamic
                // removal of vertices.
                std::size_t numVertices = polygon.size();
                std::size_t const* indices = polygon.data();
                for (std::size_t i = 0, ip1 = 1; ip1 <= numVertices; i = ip1++)
                {
                    Vertex& vertex = mVertices[i];
                    vertex.index = indices[i];
                    vertex.vPrev = (i > 0 ? i - 1 : numVertices - 1);
                    vertex.vNext = (ip1 < numVertices ? ip1 : 0);

                    // These members must be cleared in case a single
                    // VertexList object is used two or more times in
                    // triangulation queries. This is the case for
                    // triangulating a polygon tree. It is also the case if
                    // you use a single TriangulateEC object for multiple
                    // triangulation queries.
                    vertex.sPrev = negOne;
                    vertex.sNext = negOne;
                    vertex.ePrev = negOne;
                    vertex.eNext = negOne;
                    vertex.isConvex = false;
                    vertex.isEar = false;
                }

                // Create a circular list of the polygon vertices for dynamic
                // removal of vertices. Keep track of two linear sublists, one
                // for the convex vertices and one for the reflex vertices.
                // This is an O(N) process where N is the number of polygon
                // vertices.
                for (std::size_t i = 0; i < numVertices; ++i)
                {
                    if (IsConvex(i, triangulator))
                    {
                        InsertAfterC(i);
                    }
                    else
                    {
                        InsertAfterR(i);
                    }
                }

                // If the polygon is convex, create a triangle fan.
                if (mRFirst == negOne)
                {
                    for (std::size_t i = 1, ip1 = 2; ip1 < numVertices; i = ip1++)
                    {
                        triangles.push_back({ polygon[0], polygon[i], polygon[ip1] });
                    }
                    return;
                }

                // Identify the ears and build a circular list of them. Let
                // V0, V1, and V2 be consecutive vertices forming triangle InputType.
                // The vertex V1 is an ear if no other vertices of the polygon
                // lie inside InputType. Although it is enough to show that V1 is not
                // an ear by finding at least one other vertex inside InputType, it is
                // sufficient to search only the reflex vertices. This is an
                // O(C*R) process, where C is the number of convex vertices
                // and R is the number of reflex vertices with N = C+R. The
                // order is O(N^2), for example when C = R = N/2.
                for (std::size_t i = mCFirst; i != negOne; i = V(i).sNext)
                {
                    if (IsEar(i, triangulator))
                    {
                        InsertEndE(i);
                    }
                }
                V(mEFirst).ePrev = mELast;
                V(mELast).eNext = mEFirst;

                // Remove the ears, one at a time.
                bool bRemoveAnEar = true;
                while (bRemoveAnEar)
                {
                    // Add the triangle with the ear to the output list of
                    // triangles.
                    std::size_t iVPrev = V(mEFirst).vPrev;
                    std::size_t iVNext = V(mEFirst).vNext;
                    triangles.push_back({ V(iVPrev).index, V(mEFirst).index, V(iVNext).index });

                    // Remove the vertex corresponding to the ear.
                    RemoveV(mEFirst);
                    if (--numVertices == 3)
                    {
                        // Only one triangle remains, just remove the ear and
                        // copy it.
                        mEFirst = RemoveE(mEFirst);
                        iVPrev = V(mEFirst).vPrev;
                        iVNext = V(mEFirst).vNext;
                        triangles.push_back({ V(iVPrev).index, V(mEFirst).index, V(iVNext).index });
                        bRemoveAnEar = false;
                        continue;
                    }

                    // Removal of the ear can cause an adjacent vertex to
                    // become an ear or to stop being an ear.
                    Vertex& vPrev = V(iVPrev);
                    if (vPrev.isEar)
                    {
                        if (!IsEar(iVPrev, triangulator))
                        {
                            RemoveE(iVPrev);
                        }
                    }
                    else
                    {
                        bool wasReflex = !vPrev.isConvex;
                        if (IsConvex(iVPrev, triangulator))
                        {
                            if (wasReflex)
                            {
                                RemoveR(iVPrev);
                            }

                            if (IsEar(iVPrev, triangulator))
                            {
                                InsertBeforeE(iVPrev);
                            }
                        }
                    }

                    Vertex& vNext = V(iVNext);
                    if (vNext.isEar)
                    {
                        if (!IsEar(iVNext, triangulator))
                        {
                            RemoveE(iVNext);
                        }
                    }
                    else
                    {
                        bool wasReflex = !vNext.isConvex;
                        if (IsConvex(iVNext, triangulator))
                        {
                            if (wasReflex)
                            {
                                RemoveR(iVNext);
                            }

                            if (IsEar(iVNext, triangulator))
                            {
                                InsertAfterE(iVNext);
                            }
                        }
                    }

                    // Remove the ear.
                    mEFirst = RemoveE(mEFirst);
                }
            }

        private:
            Vertex& V(std::size_t i)
            {
                // If the assertion is triggered, do you have a coincident
                // vertex-edge or edge-edge pair? These violate the assumptions
                // for the algorithm.
                GTL_ARGUMENT_ASSERT(
                    i != negOne,
                    "Index out of range..");

                return mVertices[i];
            }

            bool IsConvex(std::size_t i, Triangulator& triangulator)
            {
                Vertex& vertex = V(i);
                std::size_t curr = vertex.index;
                std::size_t prev = V(vertex.vPrev).index;
                std::size_t next = V(vertex.vNext).index;
                vertex.isConvex = (triangulator.ToLine(curr, prev, next) > 0);
                return vertex.isConvex;
            }

            bool IsEar(std::size_t i, Triangulator& triangulator)
            {
                Vertex& vertex = V(i);

                if (mRFirst == negOne)
                {
                    // The remaining polygon is convex.
                    vertex.isEar = true;
                    return true;
                }

                // Search the reflex vertices and test if any are in the triangle
                // <V[prev],V[curr],V[next]>.
                std::size_t prev = V(vertex.vPrev).index;
                std::size_t curr = vertex.index;
                std::size_t next = V(vertex.vNext).index;
                vertex.isEar = true;
                for (std::size_t j = mRFirst; j != negOne; j = V(j).sNext)
                {
                    // Check if the test vertex is already one of the triangle
                    // vertices.
                    if (j == vertex.vPrev || j == i || j == vertex.vNext)
                    {
                        continue;
                    }

                    // V[j] has been ruled out as one of the original vertices of
                    // the triangle <V[prev],V[curr],V[next]>. When triangulating
                    // polygons with holes, V[j] might be a duplicated vertex, in
                    // which case it does not affect the earness of V[curr].
                    std::size_t testIndex = V(j).index;
                    Vector2<ComputeType> const& testPoint = triangulator.mComputePoints[testIndex];
                    if (testPoint == triangulator.mComputePoints[prev] ||
                        testPoint == triangulator.mComputePoints[curr] ||
                        testPoint == triangulator.mComputePoints[next])
                    {
                        continue;
                    }

                    // Test if the vertex is inside or on the triangle. When it
                    // is, it causes V[curr] not to be an ear.
                    if (triangulator.ToTriangle(testIndex, prev, curr, next) <= 0)
                    {
                        vertex.isEar = false;
                        break;
                    }
                }

                return vertex.isEar;
            }

            // Insert a convex vertex.
            void InsertAfterC(std::size_t i)
            {
                if (mCFirst == negOne)
                {
                    // Insert the first convex vertex.
                    mCFirst = i;
                }
                else
                {
                    V(mCLast).sNext = i;
                    V(i).sPrev = mCLast;
                }
                mCLast = i;
            }

            // Insert a reflex vertex.
            void InsertAfterR(std::size_t i)
            {
                if (mRFirst == negOne)
                {
                    // Insert the first reflex vertex.
                    mRFirst = i;
                }
                else
                {
                    V(mRLast).sNext = i;
                    V(i).sPrev = mRLast;
                }
                mRLast = i;
            }

            // Insert an ear at the end of the list.
            void InsertEndE(std::size_t i)
            {
                if (mEFirst == negOne)
                {
                    // Insert the first ear.
                    mEFirst = i;
                    mELast = i;
                }
                V(mELast).eNext = i;
                V(i).ePrev = mELast;
                mELast = i;
            }

            // Insert an ear after mEFirst.
            void InsertAfterE(std::size_t i)
            {
                Vertex& first = V(mEFirst);
                std::size_t currENext = first.eNext;
                Vertex& vertex = V(i);
                vertex.ePrev = mEFirst;
                vertex.eNext = currENext;
                first.eNext = i;
                V(currENext).ePrev = i;
            }

            // Insert an ear before mEFirst.
            void InsertBeforeE(std::size_t i)
            {
                Vertex& first = V(mEFirst);
                std::size_t currEPrev = first.ePrev;
                Vertex& vertex = V(i);
                vertex.ePrev = currEPrev;
                vertex.eNext = mEFirst;
                first.ePrev = i;
                V(currEPrev).eNext = i;
            }

            // Remove a vertex.
            void RemoveV(std::size_t i)
            {
                std::size_t currVPrev = V(i).vPrev;
                std::size_t currVNext = V(i).vNext;
                V(currVPrev).vNext = currVNext;
                V(currVNext).vPrev = currVPrev;
            }

            // Remove an ear.
            std::size_t RemoveE(std::size_t i)
            {
                std::size_t currEPrev = V(i).ePrev;
                std::size_t currENext = V(i).eNext;
                V(currEPrev).eNext = currENext;
                V(currENext).ePrev = currEPrev;
                return currENext;
            }

            // Remove a reflex vertex.
            void RemoveR(std::size_t i)
            {
                GTL_ARGUMENT_ASSERT(
                    mRFirst != negOne && mRLast != negOne,
                    "Reflex vertices must exist.");

                if (i == mRFirst)
                {
                    mRFirst = V(i).sNext;
                    if (mRFirst != negOne)
                    {
                        V(mRFirst).sPrev = negOne;
                    }
                    V(i).sNext = negOne;
                }
                else if (i == mRLast)
                {
                    mRLast = V(i).sPrev;
                    if (mRLast != negOne)
                    {
                        V(mRLast).sNext = negOne;
                    }
                    V(i).sPrev = negOne;
                }
                else
                {
                    std::size_t currSPrev = V(i).sPrev;
                    std::size_t currSNext = V(i).sNext;
                    V(currSPrev).sNext = currSNext;
                    V(currSNext).sPrev = currSPrev;
                    V(i).sNext = negOne;
                    V(i).sPrev = negOne;
                }
            }

            // The doubly linked list.
            std::vector<Vertex> mVertices;
            std::size_t mCFirst, mCLast;  // linear list of convex vertices
            std::size_t mRFirst, mRLast;  // linear list of reflex vertices
            std::size_t mEFirst, mELast;  // cyclical list of ears
        };

        VertexList mVertexList;
    };
}
