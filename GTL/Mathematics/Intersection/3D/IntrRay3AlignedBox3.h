// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// The test-intersection queries use the method of separating axes.
// https://www.geometrictools.com/Documentation/MethodOfSeparatingAxes.pdf
// The find-intersection queries use parametric clipping against the six
// faces of the box. The find-intersection queries use Liang-Barsky
// clipping. The queries consider the box to be a solid. The algorithms
// are described in
// https://www.geometrictools.com/Documentation/IntersectionLineBox.pdf

#include <GTL/Mathematics/Intersection/1D/IntrIntervals.h>
#include <GTL/Mathematics/Intersection/3D/IntrLine3AlignedBox3.h>
#include <GTL/Mathematics/Primitives/ND/Ray.h>
#include <array>
#include <cstddef>
#include <cmath>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Ray3<T>, AlignedBox3<T>>
        :
        public TIQuery<T, Line3<T>, AlignedBox3<T>>
    {
    public:
        struct Output
            :
            public TIQuery<T, Line3<T>, AlignedBox3<T>>::Output
        {
            // No additional information to compute.
            Output() = default;
        };

        Output operator()(Ray3<T> const& ray, AlignedBox3<T> const& box)
        {
            // Get the centered form of the aligned box. The axes are
            // implicitly axis[d] = Vector3<T>::Unit(d).
            Vector3<T> boxCenter{}, boxExtent{};
            box.GetCenteredForm(boxCenter, boxExtent);

            // Transform the ray to the aligned-box coordinate system.
            Vector3<T> rayOrigin = ray.origin - boxCenter;

            Output output{};
            DoQuery(rayOrigin, ray.direction, boxExtent, output);
            return output;
        }

    protected:
        // The caller must ensure that on entry, 'output' is default
        // constructed as if there is no intersection. If an intersection is
        // found, the 'output' values will be modified accordingly.
        void DoQuery(Vector3<T> const& rayOrigin, Vector3<T> const& rayDirection,
            Vector3<T> const& boxExtent, Output& output)
        {
            for (std::size_t i = 0; i < 3; ++i)
            {
                if (std::fabs(rayOrigin[i]) > boxExtent[i] &&
                    rayOrigin[i] * rayDirection[i] >= C_<T>(0))
                {
                    output.intersect = false;
                    return;
                }
            }

            TIQuery<T, Line3<T>, AlignedBox3<T>>::DoQuery(
                rayOrigin, rayDirection, boxExtent, output);
        }

    private:
        friend class UnitTestIntrRay3AlignedBox3;
    };

    template <typename T>
    class FIQuery<T, Ray3<T>, AlignedBox3<T>>
        :
        public FIQuery<T, Line3<T>, AlignedBox3<T>>
    {
    public:
        struct Output
            :
            public FIQuery<T, Line3<T>, AlignedBox3<T>>::Output
        {
            // No additional information to compute.
            Output() = default;
        };

        Output operator()(Ray3<T> const& ray, AlignedBox3<T> const& box)
        {
            // Get the centered form of the aligned box. The axes are
            // implicitly axis[d] = Vector3<T>::Unit(d).
            Vector3<T> boxCenter{}, boxExtent{};
            box.GetCenteredForm(boxCenter, boxExtent);

            // Transform the ray to the aligned-box coordinate system.
            Vector3<T> rayOrigin = ray.origin - boxCenter;

            Output output{};
            DoQuery(rayOrigin, ray.direction, boxExtent, output);
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
        void DoQuery(Vector3<T> const& rayOrigin, Vector3<T> const& rayDirection,
            Vector3<T> const& boxExtent, Output& output)
        {
            FIQuery<T, Line3<T>, AlignedBox3<T>>::DoQuery(
                rayOrigin, rayDirection, boxExtent, output);

            if (output.intersect)
            {
                // The line containing the ray intersects the box; the
                // t-interval is [t0,t1]. The ray intersects the box as long
                // as [t0,t1] overlaps the ray t-interval (0,+infinity).
                FIQuery<T, std::array<T, 2>, std::array<T, 2>> iiQuery{};
                auto iiOutput = iiQuery(output.parameter, C_<T>(0), true);
                if (iiOutput.intersect)
                {
                    output.numIntersections = iiOutput.numIntersections;
                    output.parameter = iiOutput.overlap;
                }
                else
                {
                    // The line containing the ray does not intersect the box.
                    output = Output{};
                }
            }
        }

    private:
        friend class UnitTestIntrRay3AlignedBox3;
    };
}
