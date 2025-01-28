// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

#include <GTL/Mathematics/Intersection/2D/IntrLine2Line2.h>
#include <GTL/Mathematics/Primitives/ND/Ray.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Ray2<T>, Ray2<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0)
            {
            }

            // If the rays do not intersect,
            //   intersect = false
            //   numIntersections = 0
            //
            // If the rays intersect in a single point,
            //   intersect = true
            //   numIntersections = 1
            // This includes the case when the rays are collinear, have
            // opposite directions and the ray origins are the common point.
            //
            // If the rays are collinear, have opposite directions and
            // intersect in a segment,
            //   intersect = true
            //   numIntersections = 2
            //
            // If the rays are collinear and have the same direction,
            //   intersect = true
            //   numIntersections = std::numeric_limits<std::size_t>::max()
            // The intersection is a ray.
            bool intersect;
            std::size_t numIntersections;
        };

        Output operator()(Ray2<T> const& ray0, Ray2<T> const& ray1)
        {
            Output output{};

            std::size_t constexpr smax = std::numeric_limits<std::size_t>::max();

            FIQuery<T, Line2<T>, Line2<T>> llQuery{};
            Line2<T> line0(ray0.origin, ray0.direction);
            Line2<T> line1(ray1.origin, ray1.direction);
            auto llOutput = llQuery(line0, line1);
            if (llOutput.numIntersections == 1)
            {
                // Test whether the line-line intersection is on the rays.
                if (llOutput.line0Parameter[0] >= C_<T>(0) &&
                    llOutput.line1Parameter[0] >= C_<T>(0))
                {
                    output.intersect = true;
                    output.numIntersections = 1;
                }
                else
                {
                    output.intersect = false;
                    output.numIntersections = 0;
                }
            }
            else if (llOutput.numIntersections == smax)
            {
                if (Dot(ray0.direction, ray1.direction) > C_<T>(0))
                {
                    // The rays are collinear and in the same direction, so
                    // they must overlap.
                    output.intersect = true;
                    output.numIntersections = smax;
                }
                else
                {
                    // The rays are collinear but have opposite directions.
                    // Test whether they overlap. Ray0 has interval
                    // [0,+infinity) and ray1 has interval (-infinity,t]
                    // relative to ray0.direction.
                    Vector2<T> diff = ray1.origin - ray0.origin;
                    T t = Dot(ray0.direction, diff);
                    if (t > C_<T>(0))
                    {
                        output.intersect = true;
                        output.numIntersections = 2;
                    }
                    else if (t < C_<T>(0))
                    {
                        output.intersect = false;
                        output.numIntersections = 0;
                    }
                    else  // t == 0
                    {
                        output.intersect = true;
                        output.numIntersections = 1;
                    }
                }
            }
            else
            {
                output.intersect = false;
                output.numIntersections = 0;
            }

            return output;
        }

    private:
        friend class UnitTestIntrRay2Ray2;
    };

    template <typename T>
    class FIQuery<T, Ray2<T>, Ray2<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0),
                ray0Parameter{ C_<T>(0), C_<T>(0) },
                ray1Parameter{ C_<T>(0), C_<T>(0) },
                point{}
            {
            }

            // If the rays do not intersect,
            //   intersect = false
            //   numIntersections = 0
            //   ray0Parameter[] = { 0, 0 }  // invalid
            //   ray1Parameter[] = { 0, 0 }  // invalid
            //   point = {{ 0, 0 }, { 0, 0 }}  // invalid
            //
            // If the rays intersect in a single point, the parameter for
            // ray0 is s0 >= 0 and the parameter for ray1 is s1 >= 0,
            //   intersect = true
            //   numIntersections = 1
            //   ray0Parameter[] = { s0, s0 }
            //   ray1Parameter[] = { s1, s1 }
            //   point[0] = ray0.origin + s0 * ray0.direction
            //   point[1] = ray1.origin + s1 * ray1.direction = point[0]
            // This includes the case when the rays are collinear, have
            // opposite directions and the ray origins are the common point,
            // in which case s0 = s1 = 0.
            //
            // If the rays are collinear, have opposite directions and
            // intersect in a segment,
            //   intersect = true
            //   numIntersections = 2
            //   ray0Parameter[] = { 0, t }
            //   ray1Parameter[] = { 0, t }
            //   point[] = { ray0.origin, ray1.origin }
            // where ray0.origin + t * ray0.direction = ray1.origin and
            // ray1.origin + t * ray1.direction = ray0.origin. The point[]
            // ordering is relative to ray0.direction.
            //
            // If the rays are collinear and have the same direction, the
            // intersection is a ray,
            //   intersect = true
            //   numIntersections = std::numeric_limits<std::size_t>::max()
            // When ray0 contains ray1,
            //   ray0Parameter[] = { t, +tmax }
            //   ray1Parameter[] = { 0, +tmax }
            //   point[] = { ray1.origin, ray1.origin }
            // where ray0.origin + t * ray0.direction = ray1.origin. When
            // ray1 contains ray0,
            //   ray0Parameter[] = { 0, +tmax }
            //   ray1Parameter[] = { t, +tmax }
            //   point[] = { ray0.origin, ray0.origin }
            // where ray1.origin + t * ray1.direction = ray0.origin.

            // The 'type' of the parameter.
            static std::int32_t constexpr negInfinity = -1;
            static std::int32_t constexpr finite = 0;
            static std::int32_t constexpr posInfinity = +1;

            bool intersect;
            std::size_t numIntersections;
            std::array<T, 2> ray0Parameter, ray1Parameter;
            std::array<Vector2<T>, 2> point;
            std::array<std::int32_t, 2> ray0ParameterType, ray1ParameterType;
        };

        Output operator()(Ray2<T> const& ray0, Ray2<T> const& ray1)
        {
            Output output{};

            std::size_t constexpr smax = std::numeric_limits<std::size_t>::max();

            FIQuery<T, Line2<T>, Line2<T>> llQuery{};
            Line2<T> line0(ray0.origin, ray0.direction);
            Line2<T> line1(ray1.origin, ray1.direction);
            auto llOutput = llQuery(line0, line1);
            if (llOutput.numIntersections == 1)
            {
                // The lines are not parallel, so the rays are not parallel.
                // Test whether the line-line intersection is on the rays.
                if (llOutput.line0Parameter[0] >= C_<T>(0) &&
                    llOutput.line1Parameter[0] >= C_<T>(0))
                {
                    output.intersect = true;
                    output.numIntersections = 1;
                    output.ray0Parameter[0] = llOutput.line0Parameter[0];
                    output.ray1Parameter[0] = llOutput.line1Parameter[0];
                    output.point[0] = llOutput.point;
                    output.point[1] = output.point[0];
                }
                else
                {
                    output.intersect = false;
                    output.numIntersections = 0;
                }
            }
            else if (llOutput.numIntersections == smax)
            {
                // The lines are the same, so the rays are collinear. Compute
                // t for which ray1.origin = ray0.origin + t * ray0.direction.
                T const tmax = std::numeric_limits<T>::max();
                Vector2<T> diff = ray1.origin - ray0.origin;
                T t = Dot(ray0.direction, diff);
                if (Dot(ray0.direction, ray1.direction) > C_<T>(0))
                {
                    // The rays are collinear and have the same direction, so
                    // their intersection is itself a ray.
                    output.intersect = true;
                    output.numIntersections = smax;
                    if (t >= C_<T>(0))
                    {
                        output.ray0Parameter[0] = t;
                        output.ray0Parameter[1] = tmax;
                        output.ray1Parameter[0] = C_<T>(0);
                        output.ray1Parameter[1] = tmax;
                        output.point[0] = ray1.origin;
                        output.point[1] = output.point[0];
                    }
                    else
                    {
                        output.ray0Parameter[0] = C_<T>(0);
                        output.ray0Parameter[1] = tmax;
                        output.ray1Parameter[0] = -t;
                        output.ray1Parameter[1] = tmax;
                        output.point[0] = ray0.origin;
                        output.point[1] = output.point[0];
                    }
                }
                else
                {
                    // The rays are collinear and have opposite directions.
                    // Test whether the rays overlap.
                    if (t >= C_<T>(0))
                    {
                        output.intersect = true;
                        output.numIntersections = 2;
                        output.ray0Parameter[0] = C_<T>(0);
                        output.ray0Parameter[1] = t;
                        output.ray1Parameter[0] = C_<T>(0);
                        output.ray1Parameter[1] = t;
                        output.point[0] = ray0.origin;
                        output.point[1] = ray1.origin;
                    }
                    else if (t == C_<T>(0))
                    {
                        output.intersect = true;
                        output.numIntersections = 1;
                        output.ray0Parameter[0] = C_<T>(0);
                        output.ray0Parameter[1] = C_<T>(0);
                        output.ray1Parameter[0] = C_<T>(0);
                        output.ray1Parameter[1] = C_<T>(0);
                        output.point[0] = ray0.origin;
                        output.point[1] = ray1.origin;  // = ray0.origin
                    }
                    else
                    {
                        output.intersect = false;
                        output.numIntersections = 0;
                    }
                }
            }
            else
            {
                // The lines are parallel but not the same. The rays cannot
                // intersect.
                output.intersect = false;
                output.numIntersections = 0;
            }

            return output;
        }

    private:
        friend class UnitTestIntrRay2Ray2;
    };
}
