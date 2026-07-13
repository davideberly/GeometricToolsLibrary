// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.13

#pragma once

#include <GTL/Mathematics/Intersection/ND/IntrLineHyperplane.h>
#include <GTL/Mathematics/Primitives/ND/Segment.h>
#include <cstddef>

namespace gtl
{
    template <typename T, std::size_t N>
    class TIQuery<T, Segment<T, N>, Hyperplane<T, N>>
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

        // The segment is P0 + t * (P1 - P0) for 0 <= t <= 1, where P0 and
        // P1 are the endpoints of the segment. The segment direction is
        // D = P1 - P0, which is generally not unit length, but the query
        // allows for this. It is also not necessary that the hyperplane
        // normal be unit length.
        Output operator()(Segment<T, N> const& segment, Hyperplane<T, N> const& hyperplane)
        {
            Output output{};

            // Compute the signed distance from the segment endpoints to the
            // hyperplane.
            DCPQuery<T, Vector<T, N>, Hyperplane<T, N>> vpQuery{};
            auto vpOutput0 = vpQuery(segment.p[0], hyperplane);
            if (vpOutput0.signedDistance == C_<T>(0))
            {
                // Endpoint p[0] is on the hyperplane.
                output.intersect = true;
                return output;
            }

            auto vpOutput1 = vpQuery(segment.p[1], hyperplane);
            if (vpOutput1.signedDistance == C_<T>(0))
            {
                // Endpoint p[1] is on the hyperplane.
                output.intersect = true;
                return output;
            }

            // Test whether the segment transversely intersects the
            // hyperplane.
            output.intersect =
                (vpOutput0.signedDistance > C_<T>(0) && vpOutput1.signedDistance < C_<T>(0)) ||
                (vpOutput0.signedDistance < C_<T>(0) && vpOutput1.signedDistance > C_<T>(0));

            return output;
        }

    private:
        friend class UnitTestIntrSegmentHyperplane;
    };

    template <typename T, std::size_t N>
    class FIQuery<T, Segment<T, N>, Hyperplane<T, N>>
        :
        public FIQuery<T, Line<T, N>, Hyperplane<T, N>>
    {
    public:
        struct Output
            :
            public FIQuery<T, Line<T, N>, Hyperplane<T, N>>::Output
        {
            // No additional information to compute.
        };

        // The segment is P0 + t * (P1 - P0) for 0 <= t <= 1, where P0 and
        // P1 are the endpoints of the segment. The segment direction is
        // D = P1 - P0, which is generally not unit length, but the query
        // allows for this. It is also not necessary that the hyperplane
        // normal be unit length.
        Output operator()(Segment<T, N> const& segment, Hyperplane<T, N> const& hyperplane)
        {
            Output output{};
            Vector<T, N> const& segOrigin = segment.p[0];
            Vector<T, N> segDirection = segment.p[1] - segment.p[0];
            DoQuery(segOrigin, segDirection, hyperplane, output);
            if (output.intersect)
            {
                output.point = segOrigin + output.parameter * segDirection;
            }
            return output;
        }

    protected:
        // The caller must ensure that on entry, 'output' is default
        // constructed as if there is no intersection. If an intersection is
        // found, the 'output' values will be modified accordingly.
        void DoQuery(Vector<T, N> const& segOrigin, Vector<T, N> const& segDirection,
            Hyperplane<T, N> const& hyperplane, Output& output)
        {
            FIQuery<T, Line<T, N>, Hyperplane<T, N>>::DoQuery(
                segOrigin, segDirection, hyperplane, output);

            if (output.intersect)
            {
                // The line intersects the plane in a point that might not be
                // on the segment.
                if (output.parameter < C_<T>(0) ||
                    output.parameter > C_<T>(1))
                {
                    output = Output{};
                }
            }
        }

    private:
        friend class UnitTestIntrSegmentHyperplane;
    };
}
