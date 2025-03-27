// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

#include <GTL/Mathematics/Intersection/2D/IntrLine2Line2.h>
#include <GTL/Mathematics/Primitives/ND/Ray.h>
#include <array>
#include <cstddef>
#include <limits>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Line2<T>, Ray2<T>>
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

            // If the line and ray do not intersect,
            //   intersect = false
            //   numIntersections = 0
            //
            // If the line and ray intersect in a single point,
            //   intersect = true
            //   numIntersections = 1
            //
            // If the line and ray are collinear,
            //   intersect = true
            //   numIntersections = std::numeric_limits<std::size_t>::max()
            bool intersect;
            std::size_t numIntersections;
        };

        Output operator()(Line2<T> const& line, Ray2<T> const& ray)
        {
            Output output{};

            FIQuery<T, Line2<T>, Line2<T>> llQuery{};
            auto llOutput = llQuery(line, Line2<T>(ray.origin, ray.direction));
            if (llOutput.numIntersections == 1)
            {
                // Test whether the line-line intersection is on the ray.
                if (llOutput.line1Parameter[0] >= C_<T>(0))
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
            else
            {
                output.intersect = llOutput.intersect;
                output.numIntersections = llOutput.numIntersections;
            }

            return output;
        }

    private:
        friend class UnitTestIntrLine2Ray2;
    };

    template <typename T>
    class FIQuery<T, Line2<T>, Ray2<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0),
                lineParameter{ C_<T>(0), C_<T>(0) },
                rayParameter{ C_<T>(0), C_<T>(0) },
                point{}
            {
            }

            // If the line and ray do not intersect,
            //   intersect = false
            //   numIntersections = 0
            //   lineParameter[] = { 0, 0 }  // invalid
            //   rayParameter[] = { 0, 0 }  // invalid
            //   point = { 0, 0 }  // invalid
            //
            // If the line and ray intersect in a single point, the parameter
            // for line is s0 and the parameter for ray is s1 >= 0,
            //   intersect = true
            //   numIntersections = 1
            //   lineParameter = { s0, s0 }
            //   rayParameter = { s1, s1 }
            //   point = line.origin + s0 * line.direction
            //         = ray.origin + s1 * ray.direction
            //
            // If the line and ray are collinear,
            //   intersect = true
            //   numIntersections = std::numeric_limits<std::size_t>::max()
            //   lineParameter[] = { -tmax, +tmax }
            //   rayParameter[] = { 0, +tmax }
            //   point = { 0, 0 }  // invalid
            // where tmax = std::numeric_limits<T>::max().
            bool intersect;
            std::size_t numIntersections;
            std::array<T, 2> lineParameter;
            std::array<T, 2> rayParameter;
            Vector2<T> point;
        };

        Output operator()(Line2<T> const& line, Ray2<T> const& ray)
        {
            Output output{};

            std::size_t constexpr smax = std::numeric_limits<std::size_t>::max();

            FIQuery<T, Line2<T>, Line2<T>> llQuery{};
            auto llOutput = llQuery(line, Line2<T>(ray.origin, ray.direction));
            if (llOutput.numIntersections == 1)
            {
                // The lines are not parallel, so the rays are not parallel.
                // Test whether the line-line intersection is on the ray.
                if (llOutput.line1Parameter[0] >= C_<T>(0))
                {
                    output.intersect = true;
                    output.numIntersections = 1;
                    output.lineParameter[0] = llOutput.line0Parameter[0];
                    output.lineParameter[1] = output.lineParameter[0];
                    output.rayParameter[0] = llOutput.line1Parameter[0];
                    output.rayParameter[1] = output.rayParameter[0];
                    output.point = llOutput.point;
                }
                else
                {
                    output.intersect = false;
                    output.numIntersections = 0;
                }
            }
            else if (llOutput.numIntersections == smax)
            {
                // The lines are the same, so the line and ray are collinear.
                output.intersect = true;
                output.numIntersections = smax;
                T const tmax = std::numeric_limits<T>::max();
                output.lineParameter[0] = -tmax;
                output.lineParameter[1] = +tmax;
                output.rayParameter[0] = C_<T>(0);
                output.rayParameter[1] = +tmax;
            }
            else
            {
                output.intersect = false;
                output.numIntersections = 0;
            }

            return output;
        }

    private:
        friend class UnitTestIntrLine2Ray2;
    };
}
