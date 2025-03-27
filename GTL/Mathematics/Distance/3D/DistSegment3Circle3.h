// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// The 3D segment-circle distance algorithm is described in
// https://www.geometrictools.com/Documentation/DistanceToCircle3.pdf
// The notation used in the code matches that of the document. The circle has
// center C and the plane of the circle has unit-length normal N. The segment
// has endpoints P0 and P1. The parameterization is P(t) = P0 + t * (P1 - P0)
// = P0 + t * M for 0 <= t <= 1, where M is generally not a unit-length
// vector. The type T can be a floating-point type or a rational type.

#include <GTL/Mathematics/Distance/3D/DistLine3Circle3.h>
#include <GTL/Mathematics/Distance/3D/DistPoint3Circle3.h>
#include <GTL/Mathematics/Primitives/ND/Segment.h>

namespace gtl
{
    template <typename T>
    class DCPQuery<T, Segment3<T>, Circle3<T>>
    {
    public:
        using Output = typename DCPLinearCircularSupport3<T>::Output;
        using Critical = typename DCPLinearCircularSupport3<T>::Critical;

        // The 'critical' parameter is an out-parameter. Usually this is not
        // something an application requires, but it is used by the other
        // 3D linear-circular distance queries and by unit tests.
        Output operator()(Segment3<T> const& segment, Circle3<T> const& circle,
            Critical* outCritical = nullptr)
        {
            // Compute the line points closest to the circle.
            Line3<T> line(segment.p[0], segment.p[1] - segment.p[0]);
            Critical critical{};
            Output output = DCPQuery<T, Line3<T>, Circle3<T>>{}(line, circle, &critical);

            // Clamp the query output to the segment domain.
            if (critical.numPoints == 1)
            {
                HasOneCriticalPoint(segment, circle, critical, output);
            }
            else
            {
                HasTwoCriticalPoints(segment, circle, critical, output);
            }

            if (outCritical != nullptr)
            {
                *outCritical = critical;
            }

            return output;
        }

    private:
        using PCQuery = DCPQuery<T, Vector3<T>, Circle3<T>>;
        using PCOutput = typename PCQuery::Output;

        void HasOneCriticalPoint(Segment3<T> const& segment, Circle3<T> const& circle,
            Critical const& critical, Output& output)
        {
            T const& t0 = critical.parameter[0];

            T const one = C_<T>(1);
            if (t0 >= one)
            {
                // The critical point is not on the segment except possibly
                // the first critical point being the right endpoint of the
                // segment. The right endpoint is the segment point closest to
                // circle. See the left red segment of the one-critical-point
                // graph of figure 8 in the PDF.
                return SegmentEndpointClosest(segment.p[1], circle, output);
            }

            T const zero = C_<T>(0);
            if (t0 <= zero)
            {
                // The critical points are not on the segment except possibly
                // the critical point being the left endpoint of the segment.
                // The left endpoint is the segment point closest to the
                // circle. See the right red segment of the one-critical-point
                // graph of figure 8 in the PDF.
                return SegmentEndpointClosest(segment.p[0], circle, output);
            }
            
            // At this time, 0 < t0 < 1. The closest line-circle pair is the
            // closest segment-circle pair. The output does not need to be
            // modified. See the green segment of the one-critical-point graph
            // of figure 8 in the PDF.
        }

        void HasTwoCriticalPoints(Segment3<T> const& segment, Circle3<T> const& circle,
            Critical const& critical, Output& output)
        {
            T const& t0 = critical.parameter[0];
            T const& t1 = critical.parameter[1];

            T const one = C_<T>(1);
            if (t0 >= one)
            {
                // The critical points are not on the segment except possibly
                // the first critical point being the right endpoint of the
                // segment. The right endpoint is the segment point closest to
                // the circle. See the left red segment of the
                // two-point-critical graphs of figure 8 in the PDF.
                return SegmentEndpointClosest(segment.p[1], circle, output);
            }

            T const zero = C_<T>(0);
            if (t1 <= zero)
            {
                // The critical points are not on the segment except possibly
                // the second critical point being the left endpoint of the
                // segment. The left endpoint is the segment point closest to
                // the circle. See the right red segment of the
                // two-point-critical graphs of figure 8 in the PDF.
                return SegmentEndpointClosest(segment.p[0], circle, output);
            }

            // At this time, t0 < 1 and t1 > 0.
            if (zero <= t0 && t1 <= one)
            {
                // At this time, 0 <= t0 < t1 <= 1. The critical points are on
                // the segment, so the closest segment-circle pairs are the
                // closest line-circle pairs. The output does not need to be
                // modified. See the green segment of the two-critical-point
                // graphs of figure 8 in the PDF.
                return;
            }

            // At this time, t0 < 0 or t1 > 1. At most one critical point is
            // on the segment.
            if (t0 < zero)
            {
                if (t1 >= one)
                {
                    // At this time, t0 < 0 < 1 <= t1. The critical points are
                    // not on the segment except possibly the second critical
                    // point is the right endpoint. See the orange segment of
                    // the two-critical-point graphs of figure 8 in the PDF.
                    SelectClosestPoint(segment.p[0], segment.p[1], circle, output);
                }
                else // t1 < 1
                {
                    // At this time, t0 < 0 < t1 < 1. The critical point at t1
                    // is on the segment but is not an endpoint. See the
                    // purple segment of the two-critical-point graphs of
                    // figure 8 in the PDF.
                    SelectClosestPoint(segment.p[0], critical.linearPoint[1], circle, output);
                }
            }
            else // t1 > 1
            {
                if (t0 <= zero)
                {
                    // At this time, t0 <= 0 < 1 < t1. The critical points are
                    // not on the segment except possibly the first critical
                    // point is the left endpoint. See the orange segment of
                    // the two-critical-point graphs of figure 8 in the PDF.
                    SelectClosestPoint(segment.p[0], segment.p[1], circle, output);
                }
                else // t0 > 0
                {
                    // At this time, 0 < t0 < 1 < t1. The critical point at t0
                    // is on the segment but is not an endpoint. See the gold
                    // segment of the two-critical-point graphs of figure 8 in
                    // the PDF.
                    SelectClosestPoint(segment.p[1], critical.linearPoint[0], circle, output);
                }
            }
        }

        void SegmentEndpointClosest(Vector3<T> const& segmentEndpoint,
            Circle3<T> const& circle, Output& output)
        {
            PCOutput pcOutput = PCQuery{}(segmentEndpoint, circle);
            output.numClosestPairs = 1;
            output.linearClosest[0] = segmentEndpoint;
            output.linearClosest[1] = { C_<T>(0), C_<T>(0), C_<T>(0) };
            output.circularClosest[0] = pcOutput.closest[1];
            output.circularClosest[1] = { C_<T>(0), C_<T>(0), C_<T>(0) };
            output.distance = pcOutput.distance;
            output.sqrDistance = output.distance * output.distance;
        }

        void SelectClosestPoint(Vector3<T> const& point0, Vector3<T> const& point1,
            Circle3<T> const& circle, Output& output)
        {
            PCOutput pcOutput0 = PCQuery{}(point0, circle);
            PCOutput pcOutput1 = PCQuery{}(point1, circle);
            if (pcOutput0.distance < pcOutput1.distance)
            {
                output.numClosestPairs = 1;
                output.linearClosest[0] = point0;
                output.linearClosest[1] = { C_<T>(0), C_<T>(0), C_<T>(0) };
                output.circularClosest[0] = pcOutput0.closest[1];
                output.circularClosest[1] = { C_<T>(0), C_<T>(0), C_<T>(0) };
                output.distance = pcOutput0.distance;
                output.sqrDistance = output.distance * output.distance;
            }
            else if (pcOutput0.distance > pcOutput1.distance)
            {
                output.numClosestPairs = 1;
                output.linearClosest[0] = point1;
                output.linearClosest[1] = { C_<T>(0), C_<T>(0), C_<T>(0) };
                output.circularClosest[0] = pcOutput1.closest[1];
                output.circularClosest[1] = { C_<T>(0), C_<T>(0), C_<T>(0) };
                output.distance = pcOutput1.distance;
                output.sqrDistance = output.distance * output.distance;
            }
            else // pcOutput0.distance = pcOutput1.distance
            {
                output.numClosestPairs = 2;
                output.linearClosest[0] = point0;
                output.linearClosest[1] = point1;
                output.circularClosest[0] = pcOutput0.closest[1];
                output.circularClosest[1] = pcOutput1.closest[1];
                output.distance = pcOutput0.distance;
                output.sqrDistance = output.distance * output.distance;
            }
        }

    private:
        friend class UnitTestDistSegment3Circle3;
    };
}
