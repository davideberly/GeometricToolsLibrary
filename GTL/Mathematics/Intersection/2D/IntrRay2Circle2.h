// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

// The queries consider the circle to be a solid disk.

#include <GTL/Mathematics/Intersection/1D/IntrIntervals.h>
#include <GTL/Mathematics/Intersection/2D/IntrLine2Circle2.h>
#include <GTL/Mathematics/Primitives/ND/Ray.h>
#include <array>
#include <cstddef>
#include <limits>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Ray2<T>, Circle2<T>>
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

        Output operator()(Ray2<T> const& ray, Circle2<T> const& circle)
        {
            Output output{};
            FIQuery<T, Ray2<T>, Circle2<T>> rcQuery{};
            output.intersect = rcQuery(ray, circle).intersect;
            return output;
        }

    private:
        friend class UnitTestIntrRay2Circle2;
    };

    template <typename T>
    class FIQuery<T, Ray2<T>, Circle2<T>>
        :
        public FIQuery<T, Line2<T>, Circle2<T>>
    {
    public:
        struct Output
            :
            public FIQuery<T, Line2<T>, Circle2<T>>::Output
        {
            Output()
                :
                FIQuery<T, Line2<T>, Circle2<T>>::Output{}
            {
            }

            // No additional information to compute.
        };

        Output operator()(Ray2<T> const& ray, Circle2<T> const& circle)
        {
            Output output{};
            DoQuery(ray.origin, ray.direction, circle, output);
            for (std::size_t i = 0; i < output.numIntersections; ++i)
            {
                output.point[i] = ray.origin + output.parameter[i] * ray.direction;
            }
            return output;
        }

    protected:
        void DoQuery(Vector2<T> const& rayOrigin,
            Vector2<T> const& rayDirection, Circle2<T> const& circle,
            Output& output)
        {
            FIQuery<T, Line2<T>, Circle2<T>>::DoQuery(rayOrigin,
                rayDirection, circle, output);

            if (output.intersect)
            {
                // The line containing the ray intersects the disk; the
                // t-interval is [t0,t1].  The ray intersects the disk as long
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
        friend class UnitTestIntrRay2Circle2;
    };
}
