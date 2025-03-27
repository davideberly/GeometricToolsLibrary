// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// The queries consider the capsule to be a solid. The test-intersection
// queries are based on distance computations.

#include <GTL/Mathematics/Intersection/1D/IntrIntervals.h>
#include <GTL/Mathematics/Intersection/3D/IntrLine3Capsule3.h>
#include <GTL/Mathematics/Distance/ND/DistRaySegment.h>
#include <GTL/Mathematics/Primitives/ND/Ray.h>
#include <array>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Ray3<T>, Capsule3<T>>
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

        Output operator()(Ray3<T> const& ray, Capsule3<T> const& capsule)
        {
            Output output{};
            DCPQuery<T, Ray3<T>, Segment3<T>> rsQuery{};
            auto rsOutput = rsQuery(ray, capsule.segment);
            output.intersect = (rsOutput.distance <= capsule.radius);
            return output;
        }

    private:
        friend class UnitTestIntrRay3Capsule3;
    };

    template <typename T>
    class FIQuery<T, Ray3<T>, Capsule3<T>>
        :
        public FIQuery<T, Line3<T>, Capsule3<T>>
    {
    public:
        struct Output
            :
            public FIQuery<T, Line3<T>, Capsule3<T>>::Output
        {
            // No additional information to compute.
            Output() = default;
        };

        Output operator()(Ray3<T> const& ray, Capsule3<T> const& capsule)
        {
            Output output{};
            DoQuery(ray.origin, ray.direction, capsule, output);
            if (output.intersect)
            {
                for (std::size_t i = 0; i < 2; ++i)
                {
                    output.point[i] = ray.origin + output.parameter[i] * ray.direction;
                }
            }
            return output;
        }

    protected:
        // The caller must ensure that on entry, 'output' is default
        // constructed as if there is no intersection. If an intersection is
        // found, the 'output' values will be modified accordingly.
        void DoQuery(Vector3<T> const& rayOrigin,
            Vector3<T> const& rayDirection, Capsule3<T> const& capsule,
            Output& output)
        {
            FIQuery<T, Line3<T>, Capsule3<T>>::DoQuery(
                rayOrigin, rayDirection, capsule, output);

            if (output.intersect)
            {
                // The line containing the ray intersects the capsule; the
                // t-interval is [t0,t1]. The ray intersects the capsule as
                // long as [t0,t1] overlaps the ray t-interval [0,+infinity).
                FIQuery<T, std::array<T, 2>, std::array<T, 2>> iiQuery{};
                auto iiOutput = iiQuery(output.parameter, C_<T>(0), true);
                if (iiOutput.intersect)
                {
                    output.numIntersections = iiOutput.numIntersections;
                    output.parameter = iiOutput.overlap;
                }
                else
                {
                    // The line containing the ray does not intersect the
                    // capsule.
                    output = Output{};
                }
            }
        }

    private:
        friend class UnitTestIntrRay3Capsule3;
    };
}
