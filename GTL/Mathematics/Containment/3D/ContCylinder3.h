// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.13

#pragma once

#include <GTL/Mathematics/Approximation/3D/ApprOrthogonalLine3.h>
#include <GTL/Mathematics/Distance/ND/DistPointLine.h>
#include <GTL/Mathematics/Primitives/ND/Cylinder.h>
#include <cmath>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class ContCylinder3
    {
    public:
        // Compute the cylinder axis segment using least-squares fit. The
        // radius is the maximum distance from points to the axis. The height
        // is determined by projection of points onto the axis and determining
        // the containing interval.
        static void GetContainer(std::vector<Vector3<T>> const& points, Cylinder3<T>& cylinder)
        {
            Line3<T> line{};
            (void)ApprOrthogonalLine3<T>::Fit(points, line);

            DCPQuery<T, Vector3<T>, Line3<T>> plQuery{};
            T maxRadiusSqr = C_<T>(0);
            for (auto const& point : points)
            {
                auto result = plQuery(point, line);
                if (result.sqrDistance > maxRadiusSqr)
                {
                    maxRadiusSqr = result.sqrDistance;
                }
            }

            Vector3<T> diff = points[0] - line.origin;
            T wMin = Dot(line.direction, diff);
            T wMax = wMin;
            for (std::size_t i = 1; i < points.size(); ++i)
            {
                diff = points[i] - line.origin;
                T w = Dot(line.direction, diff);
                if (w < wMin)
                {
                    wMin = w;
                }
                else if (w > wMax)
                {
                    wMax = w;
                }
            }

            cylinder.center = line.origin + (C_<T>(1, 2) * (wMax + wMin)) * line.direction;
            cylinder.direction = line.direction;
            cylinder.radius = std::sqrt(maxRadiusSqr);
            cylinder.height = wMax - wMin;
        }

        // Test for containment of a point by a cylinder.
        static bool InContainer(Vector3<T> const& point, Cylinder3<T> const& cylinder)
        {
            Vector3<T> diff = point - cylinder.center;
            T zProj = Dot(diff, cylinder.direction);
            if (C_<T>(2) * std::fabs(zProj) > cylinder.height)
            {
                return false;
            }

            Vector3<T> xyProj = diff - zProj * cylinder.direction;
            return Length(xyProj) <= cylinder.radius;
        }

    private:
        friend class UnitTestContCylinder3;
    };
}
