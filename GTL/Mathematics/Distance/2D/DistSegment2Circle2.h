// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

#include <GTL/Mathematics/Distance/2D/DistLine2Circle2.h>
#include <GTL/Mathematics/Distance/2D/DistPoint2Circle2.h>
#include <GTL/Mathematics/Primitives/ND/Segment.h>

namespace gtl
{
    template <typename T>
    class DCPQuery<T, Segment2<T>, Circle2<T>>
    {
    public:
        using LCQuery = DCPQuery<T, Line2<T>, Circle2<T>>;
        using Output = typename LCQuery::Output;

        Output operator()(Segment2<T> const& segment, Circle2<T> const& circle)
        {
            // Execute the query for line-circle.
            T const zero = C_<T>(0);
            T const one = C_<T>(1);
            Line2<T> line(segment.p[0], segment.p[1] - segment.p[0]);
            Output lcOutput = LCQuery{}(line, circle);

            // Test whether the closest line point is on the segment.
            if (lcOutput.numClosestPairs == 2)
            {
                // The line intersects the circle in 2 points. If the segment
                // connecting the intersection points does not overlap the
                // input segment, then a segment endpoint is the closest point
                // on the segment to the circle. Moreover, the segment does
                // not intersect the circle even though the line does.
                Update(segment, circle, lcOutput);
            }
            else
            {
                // The line does not intersect the circle or is tangent to the
                // circle. If the closest point on the line has a parameter
                // not in [0,1], then a segment endpoint is the closest point
                // on the segment to the circle. Moreover, the segment does
                // not intersect the circle even though the line does.
                if (lcOutput.parameter[0] < zero)
                {
                    Update(segment.p[0], zero, circle, lcOutput);
                }
                else if (lcOutput.parameter[0] > one)
                {
                    Update(segment.p[1], one, circle, lcOutput);
                }
            }

            return lcOutput;
        }

    private:
        static void Update(Segment2<T> const& segment, Circle2<T> const& circle, Output& lcOutput)
        {
            T const zero = C_<T>(0);
            T const one = C_<T>(1);
            auto const& t0 = lcOutput.parameter[0];
            auto const& t1 = lcOutput.parameter[1];

            if (t0 > one)
            {
                // segment.p[1] is the closest point to the circle.
                Update(segment.p[1], one, circle, lcOutput);
            }
            else if (t1 < zero)
            {
                // segment.p[0] is the closest point to the circle.
                Update(segment.p[0], zero, circle, lcOutput);
            }
            else if (t0 < zero && t1 < one)
            {
                // The segment overlaps the t1-point. Remove the t0-point.
                lcOutput.numClosestPairs = 1;
                lcOutput.parameter[0] = lcOutput.parameter[1];
                lcOutput.parameter[1] = zero;
                lcOutput.closest[0][0] = lcOutput.closest[1][0];
                lcOutput.closest[0][1] = lcOutput.closest[1][1];
                lcOutput.closest[1][0] = { zero, zero };
                lcOutput.closest[1][1] = { zero, zero };
            }
            else if (t0 > zero && t1 > one)
            {
                // The segment overlaps the t0-point. Remove the t1-point.
                lcOutput.numClosestPairs = 1;
                lcOutput.parameter[1] = zero;
                lcOutput.closest[1][0] = { zero, zero };
                lcOutput.closest[1][1] = { zero, zero };
            }
            else if (t0 < zero && t1 > one)
            {
                // The segment is strictly inside the circle. Remove both
                // the t0-point and the t1-point. The closest segment endpoint
                // to the circle is the closest endpoint. Possibly both
                // segment endpoints are closest.
                auto pcResult0 = DCPQuery<T, Vector2<T>, Circle2<T>>{}(segment.p[0], circle);
                auto pcResult1 = DCPQuery<T, Vector2<T>, Circle2<T>>{}(segment.p[1], circle);
                if (pcResult0.distance < pcResult1.distance)
                {
                    // The endpoint segment.p[0] is closer to the circle than
                    // the endpoint segment.p[1].
                    lcOutput.distance = pcResult0.distance;
                    lcOutput.sqrDistance = pcResult0.sqrDistance;
                    lcOutput.numClosestPairs = 1;
                    lcOutput.parameter[0] = zero;
                    lcOutput.parameter[1] = zero;
                    lcOutput.closest[0][0] = pcResult0.closest[0];
                    lcOutput.closest[0][1] = pcResult0.closest[1];
                    lcOutput.closest[1][0] = { zero, zero };
                    lcOutput.closest[1][1] = { zero, zero };
                }
                else if (pcResult0.distance > pcResult1.distance)
                {
                    // The endpoint segment.p[1] is closer to the circle than
                    // the endpoint segment.p[0].
                    lcOutput.distance = pcResult1.distance;
                    lcOutput.sqrDistance = pcResult1.sqrDistance;
                    lcOutput.numClosestPairs = 1;
                    lcOutput.parameter[0] = one;
                    lcOutput.parameter[1] = zero;
                    lcOutput.closest[0][0] = pcResult1.closest[0];
                    lcOutput.closest[0][1] = pcResult1.closest[1];
                    lcOutput.closest[1][0] = { zero, zero };
                    lcOutput.closest[1][1] = { zero, zero };
                }
                else
                {
                    // The endpoints segment.p[0] and segment.p[1] are
                    // equidistant from the circle.
                    lcOutput.distance = pcResult0.distance;
                    lcOutput.sqrDistance = pcResult0.sqrDistance;
                    lcOutput.numClosestPairs = 2;
                    lcOutput.parameter[0] = zero;
                    lcOutput.parameter[1] = one;
                    lcOutput.closest[0][0] = pcResult0.closest[0];
                    lcOutput.closest[0][1] = pcResult0.closest[1];
                    lcOutput.closest[1][0] = pcResult1.closest[0];
                    lcOutput.closest[1][1] = pcResult1.closest[1];
                }
            }
            else  // 0 <= t0 <= 1 && 0 <= t1 <= 1
            {
                // The line-circle intersection points are contained by the
                // segment.
                return;
            }
        }

        static void Update(Vector2<T> const& endpoint, T const& parameter,
            Circle2<T> const& circle, Output& lcOutput)
        {
            // Compute the closest circle point to the ray origin.
            T const zero = C_<T>(0);
            auto pcOutput = DCPQuery<T, Vector2<T>, Circle2<T>>{}(endpoint, circle);

            // Update the line-circle output for the segment endpoint. The
            // segment does not intersect the circle even though the line
            // does.
            lcOutput.distance = pcOutput.distance;
            lcOutput.sqrDistance = pcOutput.sqrDistance;
            lcOutput.numClosestPairs = 1;
            lcOutput.parameter[0] = parameter;
            lcOutput.parameter[1] = zero;
            lcOutput.closest[0][0] = pcOutput.closest[0];
            lcOutput.closest[0][1] = pcOutput.closest[1];
            lcOutput.closest[1][0] = { zero, zero };
            lcOutput.closest[1][1] = { zero, zero };
        }
    };
}
