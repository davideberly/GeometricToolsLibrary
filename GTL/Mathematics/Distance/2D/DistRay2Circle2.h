// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

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
            // Execute the query for line-circle.
            T const zero = C_<T>(0);
            Line2<T> line(ray.origin, ray.direction);
            Output lcOutput = LCQuery{}(line, circle);

            // Test whether the closest line point is on the ray.
            if (lcOutput.numClosestPairs == 2)
            {
                // The line intersects the circle in 2 points. If the segment
                // connecting the intersection points does not overlap the
                // ray, then the ray origin is the closest point on the ray
                // to the circle. Moreover, the ray does not intersect the
                // circle even though the line does.
                if (lcOutput.parameter[0] < zero && lcOutput.parameter[1] < zero)
                {
                    Update(ray.origin, circle, lcOutput);
                }
            }
            else
            {
                // The line does not intersect the circle or is tangent to the
                // circle. If the closest point on the line has a negative
                // parameter, then the ray origin is the closest point on the
                // ray to the circle. Moreover, the ray does not intersect the
                // circle even though the line does.
                if (lcOutput.parameter[0] < zero)
                {
                    Update(ray.origin, circle, lcOutput);
                }
            }

            return lcOutput;
        }

    private:
        static void Update(Vector2<T> const& origin, Circle2<T> const& circle, Output& lcOutput)
        {
            // Compute the closest circle point to the ray origin.
            T const zero = C_<T>(0);
            auto pcOutput = DCPQuery<T, Vector2<T>, Circle2<T>>{}(origin, circle);

            // Update the line-circle output for the ray origin. The ray does
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
