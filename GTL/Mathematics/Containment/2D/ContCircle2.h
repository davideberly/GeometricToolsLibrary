// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.12

#pragma once

#include <GTL/Mathematics/Primitives/2D/Circle2.h>
#include <GTL/Mathematics/MatrixAnalysis/LinearSystem.h>
#include <cmath>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class ContCircle2
    {
    public:
        // Compute the smallest bounding circle whose center is the average of
        // the input points.
        static void GetContainer(std::vector<Vector2<T>> const& points, Circle2<T>& circle)
        {
            GTL_ARGUMENT_ASSERT(
                points.size() > 0,
                "At least one input point is required.");

            circle.center = points[0];
            for (std::size_t i = 1; i < points.size(); ++i)
            {
                circle.center += points[i];
            }
            circle.center /= static_cast<T>(points.size());

            circle.radius = C_<T>(0);
            for (std::size_t i = 0; i < points.size(); ++i)
            {
                Vector2<T> diff = points[i] - circle.center;
                T radiusSqr = Dot(diff, diff);
                if (radiusSqr > circle.radius)
                {
                    circle.radius = radiusSqr;
                }
            }

            circle.radius = std::sqrt(circle.radius);
        }

        // Test for containment of a point inside a circle.
        static bool InContainer(Vector2<T> const& point, Circle2<T> const& circle)
        {
            Vector2<T> diff = point - circle.center;
            return Length(diff) <= circle.radius;
        }

        // Compute the smallest bounding circle that contains the input
        // circles.
        static void MergeContainers(Circle2<T> const& circle0, Circle2<T> const& circle1, Circle2<T>& merge)
        {
            Vector2<T> cenDiff = circle1.center - circle0.center;
            T lenSqr = Dot(cenDiff, cenDiff);
            T rDiff = circle1.radius - circle0.radius;
            T rDiffSqr = rDiff * rDiff;

            if (rDiffSqr >= lenSqr)
            {
                merge = (rDiff >= C_<T>(0) ? circle1 : circle0);
            }
            else
            {
                T length = std::sqrt(lenSqr);
                if (length > C_<T>(0))
                {
                    T coeff = C_<T>(1, 2) * (length + rDiff) / length;
                    merge.center = circle0.center + coeff * cenDiff;
                }
                else
                {
                    merge.center = circle0.center;
                }

                merge.radius = C_<T>(1, 2) * (length + circle0.radius + circle1.radius);
            }
        }

        // Compute the circle circumscribing a triangle. The function returns
        // true when successful, false otherwise (input points are collinear).
        static bool Circumscribe(Vector2<T> const& v0, Vector2<T> const& v1,
            Vector2<T> const& v2, Circle2<T>& circle)
        {
            Vector2<T> e10 = v1 - v0;
            Vector2<T> e20 = v2 - v0;
            Matrix2x2<T> A{ { e10[0], e10[1] }, { e20[0], e20[1] } };
            Vector2<T> B{ C_<T>(1, 2) * Dot(e10, e10), C_<T>(1, 2) * Dot(e20, e20) };
            Vector2<T> solution{};
            if (LinearSystem<T>::Solve(A, B, solution))
            {
                circle.center = v0 + solution;
                circle.radius = Length(solution);
                return true;
            }
            return false;
        }

        // Compute the circle inscribing a triangle. The function returns
        // true when successful, false otherwise (input points are collinear).
        static bool Inscribe(Vector2<T> const& v0, Vector2<T> const& v1,
            Vector2<T> const& v2, Circle2<T>& circle)
        {
            Vector2<T> d10 = v1 - v0;
            Vector2<T> d20 = v2 - v0;
            Vector2<T> d21 = v2 - v1;
            T len10 = Length(d10);
            T len20 = Length(d20);
            T len21 = Length(d21);
            T perimeter = len10 + len20 + len21;
            if (perimeter > C_<T>(0))
            {
                len10 /= perimeter;
                len20 /= perimeter;
                len21 /= perimeter;
                circle.center = len21 * v0 + len20 * v1 + len10 * v2;
                circle.radius = std::fabs(DotPerp(d10, d20)) / perimeter;
                return circle.radius > C_<T>(0);
            }
            return false;
        }

    private:
        friend class UnitTestContCircle2;
    };
}
