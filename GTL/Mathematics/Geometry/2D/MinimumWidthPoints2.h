// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// The width for a set of 2D points is the minimum distance between pairs
// of parallel lines, each pair bounding the points. The width for a set
// of 2D points is equal to the width for the set of vertices of the convex
// hull of the 2D points. It can be computed using the rotating calipers
// algorithm. For details about the rotating calipers algorithm and computing
// the width of a set of 2D points, see
// http://www-cgrl.cs.mcgill.ca/~godfried/research/calipers.html
// https://web.archive.org/web/20150330010154/http://cgm.cs.mcgill.ca/~orm/rotcal.html

#include <GTL/Mathematics/Geometry/2D/ConvexHull2.h>
#include <GTL/Mathematics/Geometry/2D/RotatingCalipers.h>
#include <GTL/Mathematics/Primitives/ND/Line.h>
#include <GTL/Mathematics/Primitives/ND/OrientedBox.h>
#include <cmath>
#include <cstddef>
#include <functional>
#include <limits>
#include <vector>

namespace gtl
{
    template <typename T>
    class MinimumWidthPoints2
    {
    public:
        // The return value is an oriented box in 2D. The width of the point
        // set is in the direction box.axis[0]; the width is 2*box.extent[0].
        // The corresponding height is in the direction box.axis[1] =
        // -Perp(box.axis[0]); the height is 2*box.extent[1].

        // The points are arbitrary, so the convex hull must be computed from
        // them to obtain the convex polygon whose minimum width is the
        // desired output.
        OrientedBox2<T> operator()(std::size_t numPoints, Vector2<T> const* points,
            bool useRotatingCalipers = true)
        {
            GTL_ARGUMENT_ASSERT(
                numPoints >= 3 && points != nullptr,
                "Invalid input.");

            OrientedBox2<T> box{};

            // Get the convex hull of the points.
            ConvexHull2<T> ch2{};
            ch2(numPoints, points);
            std::size_t dimension = ch2.GetDimension();

            if (dimension == 0)
            {
                box.center = points[0];
                box.axis[0] = Vector2<T>{ C_<T>(1), C_<T>(0) };
                box.axis[1] = Vector2<T>{ C_<T>(0), C_<T>(1) };
                box.extent = Vector2<T>{ C_<T>(0), C_<T>(0) };
                return box;
            }

            if (dimension == 1)
            {
                // The points lie on a line. Determine the extreme t-values
                // for the points represented as P = origin + t*direction. We
                // know that 'origin' is an input vertex, so we can start
                // both t-extremes at zero.
                auto const& hull = ch2.GetHull();
                Line2<T> line{};
                line.origin = points[hull[0]];
                line.direction = points[hull[1]] - points[hull[0]];
                Normalize(line.direction);
                T tmin = C_<T>(0), tmax = C_<T>(0);
                for (std::size_t i = 0; i < numPoints; ++i)
                {
                    Vector2<T> diff = points[i] - line.origin;
                    T t = Dot(diff, line.direction);
                    if (t > tmax)
                    {
                        tmax = t;
                    }
                    else if (t < tmin)
                    {
                        tmin = t;
                    }
                }

                box.center = line.origin + C_<T>(1, 2) * (tmin + tmax) * line.direction;
                box.extent[0] = C_<T>(0);
                box.extent[1] = C_<T>(1, 2) * (tmax - tmin);
                box.axis[0] = Perp(line.direction);
                box.axis[1] = line.direction;
                return box;
            }

            // Get the indexed convex hull as a non-indexed collection.
            std::vector<std::size_t> hull = ch2.GetHull();
            std::vector<Vector2<T>> vertices(hull.size());
            for (std::size_t i = 0; i < hull.size(); ++i)
            {
                vertices[i] = points[hull[i]];
            }

            ComputeMinWidth(vertices, useRotatingCalipers, box);
            return box;
        }

        // The points are arbitrary, so the convex hull must be computed from
        // them to obtain the convex polygon whose minimum width is the
        // desired output.
        OrientedBox2<T> operator()(std::vector<Vector2<T>> const& points,
            bool useRotatingCalipers = true)
        {
            return operator()(points.size(), points.data(), useRotatingCalipers);
        }

        // The points already form a counterclockwise, nondegenerate convex
        // polygon. If the points directly are the convex polygon, set
        // numIndices to 0 and indices to nullptr. If the polygon vertices
        // are a subset of the incoming points, that subset is identified by
        // numIndices >= 3 and indices having numIndices elements.
        OrientedBox2<T> operator()(std::size_t numPoints, Vector2<T> const* points,
            std::size_t numIndices, std::size_t const* indices, bool useRotatingCalipers = true)
        {
            GTL_ARGUMENT_ASSERT(
                numPoints >= 3 && points != nullptr &&
                ((indices == nullptr && numIndices == 0) ||
                (indices != nullptr && numIndices >= 3)),
                "Invalid input.");

            if (indices)
            {
                std::vector<Vector2<T>> compactPoints(numIndices);
                for (std::size_t i = 0; i < numIndices; ++i)
                {
                    compactPoints[i] = points[indices[i]];
                }
                return operator()(compactPoints, useRotatingCalipers);
            }
            else
            {
                return operator()(numPoints, points, useRotatingCalipers);
            }
        }

        // The points already form a counterclockwise, nondegenerate convex
        // polygon. If the points directly are the convex polygon, pass an
        // indices object with 0 elements. If the polygon vertices are a
        // subset of the incoming points, that subset is identified by
        // numIndices >= 3 and indices having numIndices elements.
        OrientedBox2<T> operator()(std::vector<Vector2<T>> const& points,
            std::vector<std::size_t> const& indices,
            bool useRotatingCalipers = true)
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

    private:
        using RotatingCalipersType = RotatingCalipers<T>;
        using AntipodeType = typename RotatingCalipersType::Antipode;
        using Rational = BSRational<UIntegerAP32>;

        void ComputeMinWidth(std::vector<Vector2<T>> const& vertices,
            bool useRotatingCalipers, OrientedBox2<T>& box)
        {
            std::function<Vector2<T>(std::size_t)> GetVertex{};
            std::vector<std::size_t> indices{};
            std::size_t numElements{}, i0Min{}, i1Min{};
            T minWidth{};

            if (useRotatingCalipers)
            {
                std::vector<AntipodeType> antipodes{};
                RotatingCalipersType::ComputeAntipodes(vertices, antipodes);
                GTL_RUNTIME_ASSERT(
                    antipodes.size() > 0,
                    "Antipodes must exist.");

                Rational minSqrWidth = ComputeSqrWidth(vertices, antipodes[0]);
                std::size_t minAntipode = 0;
                for (std::size_t i = 1; i < antipodes.size(); ++i)
                {
                    Rational sqrWidth = ComputeSqrWidth(vertices, antipodes[i]);
                    if (sqrWidth < minSqrWidth)
                    {
                        minSqrWidth = sqrWidth;
                        minAntipode = i;
                    }
                }
                minWidth = std::sqrt(minSqrWidth);

                GetVertex = [&vertices](std::size_t j) { return vertices[j]; };
                numElements = vertices.size();
                i0Min = antipodes[minAntipode].edge[0];
                i1Min = antipodes[minAntipode].edge[1];
            }
            else
            {
                // Remove duplicate and collinear vertices.
                std::size_t const numVertices = vertices.size();
                indices.reserve(numVertices);

                Vector2<T> ePrev = vertices.front() - vertices.back();
                for (std::size_t i0 = 0, i1 = 1; i0 < numVertices; ++i0)
                {
                    Vector2<T> eNext = vertices[i1] - vertices[i0];

                    T dp = DotPerp(ePrev, eNext);
                    if (dp != C_<T>(0))
                    {
                        indices.push_back(i0);
                    }

                    ePrev = eNext;
                    if (++i1 == numVertices)
                    {
                        i1 = 0;
                    }
                }

                // Iterate over the polygon edges to search for the edge that
                // leads to the minimum width.
                std::size_t const numIndices = indices.size();
                minWidth = std::numeric_limits<T>::max();
                i0Min = numIndices - 1;
                i1Min = 0;
                for (std::size_t i0 = numIndices - 1, i1 = 0; i1 < indices.size(); i0 = i1++)
                {
                    Vector2<T> const& origin = vertices[indices[i0]];
                    Vector2<T> U = vertices[indices[i1]] - origin;
                    Normalize(U);

                    T maxWidth = C_<T>(0);
                    for (std::size_t j = 0; j < numIndices; ++j)
                    {
                        Vector2<T> diff = vertices[indices[j]] - origin;
                        T width = U[0] * diff[1] - U[1] * diff[0];
                        if (width > maxWidth)
                        {
                            maxWidth = width;
                        }
                    }

                    if (maxWidth < minWidth)
                    {
                        minWidth = maxWidth;
                        i0Min = i0;
                        i1Min = i1;
                    }
                }

                GetVertex = [&vertices, &indices](std::size_t j)
                {
                    return vertices[indices[j]];
                };

                numElements = numIndices;
            }

            Vector2<T> origin{}, U{};
            T minHeight{}, maxHeight{};
            Compute(GetVertex, numElements, i0Min, i1Min,
                origin, U, minHeight, maxHeight);

            box.extent[0] = C_<T>(1, 2) * minWidth;
            box.extent[1] = C_<T>(1, 2) * (maxHeight - minHeight);
            box.axis[0] = -Perp(U);
            box.axis[1] = U;
            box.center = origin + box.extent[0] * box.axis[0] +
                (C_<T>(1, 2) * (maxHeight + minHeight)) * box.axis[1];
        }

        Rational ComputeSqrWidth(std::vector<Vector2<T>> const& vertices,
            AntipodeType const& antipode)
        {
            Vector2<T> const& V = vertices[antipode.vertex];
            Vector2<T> const& E0 = vertices[antipode.edge[0]];
            Vector2<T> const& E1 = vertices[antipode.edge[1]];
            Vector2<Rational> rV = { V[0], V[1] };
            Vector2<Rational> rE0 = { E0[0], E0[1] };
            Vector2<Rational> rE1 = { E1[0], E1[1] };
            Vector2<Rational> rU = rE1 - rE0;
            Vector2<Rational> rDiff = rV - rE0;
            Rational rDotPerp = rU[1] * rDiff[0] - rU[0] * rDiff[1];
            Rational rSqrLenU = Dot(rU, rU);
            Rational rSqrWidth = rDotPerp * rDotPerp / rSqrLenU;
            return rSqrWidth;
        }

        void Compute(std::function<Vector2<T>(std::size_t)> const& GetVertex,
            std::size_t numElements, std::size_t i0Min, std::size_t i1Min,
            Vector2<T>& origin, Vector2<T>& U, T& minHeight, T& maxHeight)
        {
            origin = GetVertex(i0Min);
            U = GetVertex(i1Min) - origin;
            Normalize(U);

            minHeight = C_<T>(0);
            maxHeight = C_<T>(0);
            for (std::size_t j = 0; j < numElements; ++j)
            {
                Vector2<T> diff = GetVertex(j) - origin;
                T height = Dot(U, diff);
                if (height < minHeight)
                {
                    minHeight = height;
                }
                else if (height > maxHeight)
                {
                    maxHeight = height;
                }
            }
        }

    private:
        friend class UnitTestMinimumWidthPoints;
    };
}