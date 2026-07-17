// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

// The queries consider the box to be a solid. The test-intersection queries
// use the method of separating axes.
// https://www.geometrictools.com/Documentation/MethodOfSeparatingAxes.pdf
// The find-intersection queries use parametric clipping against the four
// edges of the box.

#include <GTL/Mathematics/Intersection/1D/IntrIntervals.h>
#include <GTL/Mathematics/Intersection/2D/IntrLine2AlignedBox2.h>
#include <GTL/Mathematics/Primitives/ND/Ray.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Ray2<T>, AlignedBox2<T>>
        :
        public TIQuery<T, Line2<T>, AlignedBox2<T>>
    {
    public:
        struct Output
            :
            public TIQuery<T, Line2<T>, AlignedBox2<T>>::Output
        {
            Output()
                :
                TIQuery<T, Line2<T>, AlignedBox2<T>>::Output{}
            {
            }

            // No additional information to compute.
        };

        Output operator()(Ray2<T> const& ray, AlignedBox2<T> const& box)
        {
            // Get the centered form of the aligned box. The axes are
            // implicitly Axis[d] = Vector2<T>::Unit(d).
            Vector2<T> boxCenter{}, boxExtent{};
            box.GetCenteredForm(boxCenter, boxExtent);

            // Transform the ray to the aligned-box coordinate system.
            Vector2<T> rayOrigin = ray.origin - boxCenter;

            Output output{};
            DoQuery(rayOrigin, ray.direction, boxExtent, output);
            return output;
        }

    protected:
        void DoQuery(Vector2<T> const& rayOrigin,
            Vector2<T> const& rayDirection, Vector2<T> const& boxExtent,
            Output& output)
        {
            for (std::size_t i = 0; i < 2; ++i)
            {
                if (std::fabs(rayOrigin[i]) > boxExtent[i] &&
                    rayOrigin[i] * rayDirection[i] >= C_<T>(0))
                {
                    output.intersect = false;
                    return;
                }
            }

            TIQuery<T, Line2<T>, AlignedBox2<T>>::DoQuery(rayOrigin,
                rayDirection, boxExtent, output);
        }

    private:
        friend class UnitTestIntrRay2AlignedBox2;
    };

    template <typename T>
    class FIQuery<T, Ray2<T>, AlignedBox2<T>>
        :
        public FIQuery<T, Line2<T>, AlignedBox2<T>>
    {
    public:
        struct Output
            :
            public FIQuery<T, Line2<T>, AlignedBox2<T>>::Output
        {
            Output()
                :
                FIQuery<T, Line2<T>, AlignedBox2<T>>::Output{}
            {
            }

            // No additional information to compute.
        };

        Output operator()(Ray2<T> const& ray, AlignedBox2<T> const& box)
        {
            // Get the centered form of the aligned box. The axes are
            // implicitly Axis[d] = Vector2<T>::Unit(d).
            Vector2<T> boxCenter{}, boxExtent{};
            box.GetCenteredForm(boxCenter, boxExtent);

            // Transform the ray to the aligned-box coordinate system.
            Vector2<T> rayOrigin = ray.origin - boxCenter;

            Output output{};
            DoQuery(rayOrigin, ray.direction, boxExtent, output);
            for (std::size_t i = 0; i < output.numIntersections; ++i)
            {
                output.point[i] = ray.origin + output.parameter[i] * ray.direction;
            }
            return output;
        }

    protected:
        void DoQuery(Vector2<T> const& rayOrigin,
            Vector2<T> const& rayDirection, Vector2<T> const& boxExtent,
            Output& output)
        {
            FIQuery<T, Line2<T>, AlignedBox2<T>>::DoQuery(rayOrigin,
                rayDirection, boxExtent, output);

            if (output.intersect)
            {
                // The line containing the ray intersects the box; the
                // t-interval is [t0,t1]. The ray intersects the box as long
                // as [t0,t1] overlaps the ray t-interval [0,+infinity).
                std::array<T, 2> rayInterval =
                    { C_<T>(0), std::numeric_limits<T>::max()};
                FIQuery<T, std::array<T, 2>, std::array<T, 2>> iiQuery{};
                auto iiOutput = iiQuery(output.parameter, rayInterval);
                output.intersect = iiOutput.intersect;
                output.numIntersections = iiOutput.numIntersections;
                output.parameter = iiOutput.overlap;
            }
        }

    private:
        friend class UnitTestIntrRay2AlignedBox2;
    };
}
