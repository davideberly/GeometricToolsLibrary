// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// The queries consider the sphere to be a solid.
//
// The sphere is (X-C)^T*(X-C)-r^2 = 0. The segment has endpoints P0 and P1.
// The segment origin (center) is P = (P0+P1)/2, the segment direction is
// D = (P1-P0)/|P1-P0| and the segment extent (half the segment length) is
// e = |P1-P0|/2. The segment is X = P+t*D for t in [-e,e]. Substitute the
// segment equation into the sphere equation to obtain a quadratic equation
// Q(t) = t^2 + 2*a1*t + a0 = 0, where a1 = (P1-P0)^T*(P0-C) and
// a0 = (P0-C)^T*(P0-C)-r^2. The algorithm involves an analysis of the
// real-valued roots of Q(t) for -e <= t <= e.

#include <GTL/Mathematics/Intersection/1D/IntrIntervals.h>
#include <GTL/Mathematics/Intersection/3D/IntrLine3Sphere3.h>
#include <GTL/Mathematics/Primitives/ND/Segment.h>
#include <array>
#include <cmath>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Segment3<T>, Sphere3<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false)
            {
            }

            bool intersect;
        };

        Output operator()(Segment3<T> const& segment, Sphere3<T> const& sphere)
        {
            Output output{};

            Vector3<T> segOrigin{};     // P
            Vector3<T> segDirection{};  // D
            T segExtent{};              // e
            segment.GetCenteredForm(segOrigin, segDirection, segExtent);

            Vector3<T> diff = segOrigin - sphere.center;
            T a0 = Dot(diff, diff) - sphere.radius * sphere.radius;
            T a1 = Dot(segDirection, diff);
            T discr = a1 * a1 - a0;
            if (discr < C_<T>(0))
            {
                // Q(t) has no real-valued roots. The segment does not
                // intersect the sphere.
                output.intersect = false;
                return output;
            }

            // Q(-e) = e^2 - 2*a1*e + a0, Q(e) = e^2 + 2*a1*e + a0
            T tmp0 = segExtent * segExtent + a0;  // e^2 + a0
            T tmp1 = C_<T>(2) * a1 * segExtent;  // 2*a1*e
            T qm = tmp0 - tmp1;  // Q(-e)
            T qp = tmp0 + tmp1;  // Q(e)
            if (qm * qp <= C_<T>(0))
            {
                // Q(t) has a root on the interval [-e,e]. The segment
                // intesects the sphere.
                output.intersect = true;
                return output;
            }

            // Either (Q(-e) > 0 and Q(e) > 0) or (Q(-e) < 0 and Q(e) < 0).
            // When Q at the endpoints is negative, Q(t) < 0 for all t in
            // [-e,e] and the segment does not intersect the sphere.
            // Otherwise, Q(-e) > 0 [and Q(e) > 0]. The minimum of Q(t)
            // occurs at t = -a1. We know that discr >= 0, so Q(t) has a
            // root on (-e,e) when -a1 is in (-e,e). The combined test for
            // intersection is (Q(-e) > 0 and |a1| < e).
            output.intersect = (qm > C_<T>(0) && std::fabs(a1) < segExtent);
            return output;
        }

    private:
        friend class UnitTestIntrSegment3Sphere3;
    };

    template <typename T>
    class FIQuery<T, Segment3<T>, Sphere3<T>>
        :
        public FIQuery<T, Line3<T>, Sphere3<T>>
    {
    public:
        struct Output
            :
            public FIQuery<T, Line3<T>, Sphere3<T>>::Output
        {
            // No additional information to compute.
            Output() = default;
        };

        Output operator()(Segment3<T> const& segment, Sphere3<T> const& sphere)
        {
            Vector3<T> segOrigin{}, segDirection{};
            T segExtent{};
            segment.GetCenteredForm(segOrigin, segDirection, segExtent);

            Output output{};
            DoQuery(segOrigin, segDirection, segExtent, sphere, output);
            if (output.intersect)
            {
                for (std::size_t i = 0; i < 2; ++i)
                {
                    output.point[i] = segOrigin + output.parameter[i] * segDirection;
                }
            }
            return output;
        }

    protected:
        // The caller must ensure that on entry, 'output' is default
        // constructed as if there is no intersection. If an intersection is
        // found, the 'output' values will be modified accordingly.
        void DoQuery(Vector3<T> const& segOrigin,
            Vector3<T> const& segDirection, T segExtent,
            Sphere3<T> const& sphere, Output& output)
        {
            FIQuery<T, Line3<T>, Sphere3<T>>::DoQuery(
                segOrigin, segDirection, sphere, output);

            if (output.intersect)
            {
                // The line containing the segment intersects the sphere; the
                // t-interval is [t0,t1]. The segment intersects the sphere
                // as long as [t0,t1] overlaps the segment t-interval
                // [-segExtent,+segExtent].
                std::array<T, 2> segInterval = { -segExtent, segExtent };
                FIQuery<T, std::array<T, 2>, std::array<T, 2>> iiQuery{};
                auto iiOutput = iiQuery(output.parameter, segInterval);
                if (iiOutput.intersect)
                {
                    output.numIntersections = iiOutput.numIntersections;
                    output.parameter = iiOutput.overlap;
                }
                else
                {
                    // The line containing the segment does not intersect
                    // the sphere.
                    output = Output{};
                }
            }
        }


    private:
        friend class UnitTestIntrSegment3Sphere3;
    };
}
