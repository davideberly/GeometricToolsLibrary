// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.09.19

#pragma once

#include <GTL/Utility/Exceptions.h>
#include <GTL/Mathematics/Distance/2D/DistLine2Circle2.h>
#include <GTL/Mathematics/Distance/2D/DistPoint2Circle2.h>
#include <GTL/Mathematics/Primitives/ND/Ray.h>

namespace gtl
{
    template <typename T>
    class DCPQuery<T, Ray2<T>, Circle2<T>>
    {
    public:
        using LCQuery = DCPQuery<T, Line2<T>, Circle2<T>>;
        using Output = typename LCQuery::Output;

        Output operator()(Ray2<T> const& ray, Circle2<T> const& circle)
        {
            GTL_ARGUMENT_ASSERT(
                ray.direction != Vector2<T>::Zero() &&
                circle.radius > static_cast<T>(0),
                "Invalid input.");

            // Execute the query for line-circle.
            T const zero = C_<T>(0);
            Line2<T> line(ray.origin, ray.direction);
            Output lcOutput = LCQuery{}(line, circle);

            // Test whether the closest line point is on the ray.
            if (lcOutput.numClosestPairs == 2)
            {
                // The segment connecting the line-circle intersection points
                // has parameter interval [t0,t1]. Determine how this
                // intersects with the ray interval [0,+infinity) and modify
                // lcResult accordingly.
                Update(ray, circle, lcOutput);
            }
            else // lcOutput.numClosestPairs = 1
            {
                // The line does not intersect the circle or is tangent to the
                // circle. If the closest line point to the circle has a
                // negative parameter, then the ray is outside the circle and
                // the ray origin is the closest ray point to the circle.
                if (lcOutput.parameter[0] < zero)
                {
                    Update(ray.origin, circle, lcOutput);
                }
            }

            return lcOutput;
        }

    private:
        static void Update(Ray2<T> const& ray, Circle2<T> const& circle, Output& lcOutput)
        {
            T const zero = static_cast<T>(0);
            auto const& t0 = lcOutput.parameter[0];
            auto const& t1 = lcOutput.parameter[1];

            if (t1 <= zero)
            {
                // The ray.origin is the closest point to the circle.
                Update(ray.origin, circle, lcOutput);
            }
            else if (t0 < zero)
            {
                // The ray.origin is strictly inside the circle. Remove the
                // t0-point.
                lcOutput.numClosestPairs = 1;
                lcOutput.parameter[0] = lcOutput.parameter[1];
                lcOutput.parameter[1] = zero;
                lcOutput.closest[0][0] = lcOutput.closest[1][0];
                lcOutput.closest[0][1] = lcOutput.closest[1][1];
                lcOutput.closest[1][0] = { zero, zero };
                lcOutput.closest[1][1] = { zero, zero };
            }
            else  // 0 <= t0 < t1
            {
                // The line-circle intersection points are contained by the
                // ray.
            }
        }

        static void Update(Vector2<T> const& origin, Circle2<T> const& circle, Output& lcOutput)
        {
            // Compute the closest circle point to the ray origin.
            T const zero = static_cast<T>(0);
            auto pcOutput = DCPQuery<T, Vector2<T>, Circle2<T>>{}(origin, circle);

            // Update the line-circle result for the ray origin. The ray does
            // not intersect the circle even though the line does.
            lcOutput.distance = pcOutput.distance;
            lcOutput.sqrDistance = pcOutput.sqrDistance;
            lcOutput.numClosestPairs = 1;
            lcOutput.parameter[0] = zero;
            lcOutput.parameter[1] = zero;
            lcOutput.closest[0][0] = pcOutput.closest[0];
            lcOutput.closest[0][1] = pcOutput.closest[1];
            lcOutput.closest[1][0] = { zero, zero };
            lcOutput.closest[1][1] = { zero, zero };
        }
    };
}
