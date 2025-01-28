// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// The queries consider the triangle to be a solid.

#include <GTL/Mathematics/Intersection/1D/IntrIntervals.h>
#include <GTL/Mathematics/Intersection/2D/IntrLine2Triangle2.h>
#include <GTL/Mathematics/Primitives/ND/Ray.h>
#include <array>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Ray2<T>, Triangle2<T>>
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

        // The ray is P + t * D, where P is a point on the line and D is a
        // direction vector that does not have to be unit length. This is
        // useful when using a 2-point representation P0 + t * (P1 - P0). The
        // t-parameter is constrained by t >= 0.
        Output operator()(Ray2<T> const& ray, Triangle2<T> const& triangle)
        {
            Output output{};
            FIQuery<T, Ray2<T>, Triangle2<T>> rtQuery;
            output.intersect = rtQuery(ray, triangle).intersect;
            return output;
        }

    private:
        friend class UnitTestIntrRay2Triangle2;
    };

    template <typename T>
    class FIQuery<T, Ray2<T>, Triangle2<T>>
        :
        public FIQuery<T, Line2<T>, Triangle2<T>>
    {
    public:
        struct Output
            :
            public FIQuery<T, Line2<T>, Triangle2<T>>::Output
        {
            // No additional information to compute.
        };

        // The ray is P + t * D, where P is a point on the line and D is a
        // direction vector that does not have to be unit length. This is
        // useful when using a 2-point representation P0 + t * (P1 - P0). The
        // t-parameter is constrained by t >= 0.
        Output operator()(Ray2<T> const& ray, Triangle2<T> const& triangle)
        {
            Output output{};
            DoQuery(ray.origin, ray.direction, triangle, output);
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
        void DoQuery(Vector2<T> const& origin, Vector2<T> const& direction,
            Triangle2<T> const& triangle, Output& output)
        {
            FIQuery<T, Line2<T>, Triangle2<T>>::DoQuery(
                origin, direction, triangle, output);

            if (output.intersect)
            {
                // The line containing the ray intersects the triangle; the
                // t-interval is [t0,t1]. The ray intersects the triangle as
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
                    // triangle.
                    output = Output{};
                }
            }
        }

    private:
        friend class UnitTestIntrRay2Triangle2;
    };
}
