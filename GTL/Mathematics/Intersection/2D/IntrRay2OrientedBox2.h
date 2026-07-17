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

#include <GTL/Mathematics/Intersection/2D/IntrRay2AlignedBox2.h>
#include <GTL/Mathematics/Primitives/ND/OrientedBox.h>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Ray2<T>, OrientedBox2<T>>
        :
        public TIQuery<T, Ray2<T>, AlignedBox2<T>>
    {
    public:
        struct Output
            :
            public TIQuery<T, Ray2<T>, AlignedBox2<T>>::Output
        {
            Output()
                :
                TIQuery<T, Ray2<T>, AlignedBox2<T>>::Output{}
            {
            }

            // No additional information to compute.
        };

        Output operator()(Ray2<T> const& ray, OrientedBox2<T> const& box)
        {
            // Transform the ray to the oriented-box coordinate system.
            Vector2<T> diff = ray.origin - box.center;
            Vector2<T> rayOrigin
            {
                Dot(diff, box.axis[0]),
                Dot(diff, box.axis[1])
            };
            Vector2<T> rayDirection = Vector2<T>
            {
                Dot(ray.direction, box.axis[0]),
                Dot(ray.direction, box.axis[1])
            };

            Output output{};
            this->DoQuery(rayOrigin, rayDirection, box.extent, output);
            return output;
        }

    private:
        friend class UnitTestIntrRay2OrientedBox2;
    };

    template <typename T>
    class FIQuery<T, Ray2<T>, OrientedBox2<T>>
        :
        public FIQuery<T, Ray2<T>, AlignedBox2<T>>
    {
    public:
        struct Output
            :
            public FIQuery<T, Ray2<T>, AlignedBox2<T>>::Output
        {
            Output()
                :
                FIQuery<T, Ray2<T>, AlignedBox2<T>>::Output{}
            {
            }

            // No additional information to compute.
        };

        Output operator()(Ray2<T> const& ray, OrientedBox2<T> const& box)
        {
            // Transform the ray to the oriented-box coordinate system.
            Vector2<T> diff = ray.origin - box.center;
            Vector2<T> rayOrigin
            {
                Dot(diff, box.axis[0]),
                Dot(diff, box.axis[1])
            };
            Vector2<T> rayDirection = Vector2<T>
            {
                Dot(ray.direction, box.axis[0]),
                Dot(ray.direction, box.axis[1])
            };

            Output output{};
            this->DoQuery(rayOrigin, rayDirection, box.extent, output);
            for (std::size_t i = 0; i < output.numIntersections; ++i)
            {
                output.point[i] = ray.origin + output.parameter[i] * ray.direction;
            }
            return output;
        }

    private:
        friend class UnitTestIntrRay2OrientedBox2;
    };
}
