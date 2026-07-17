// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

// The queries consider the arc to be a 1-dimensional object.

#include <GTL/Mathematics/Intersection/2D/IntrLine2Circle2.h>
#include <GTL/Mathematics/Primitives/2D/Arc2.h>
#include <array>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Line2<T>, Arc2<T>>
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

        // See Arc2.h for a description of epsilon >= 0.
        Output operator()(Line2<T> const& line, Arc2<T> const& arc, T const& epsilon)
        {
            Output output{};
            FIQuery<T, Line2<T>, Arc2<T>> laQuery{};
            auto laOutput = laQuery(line, arc, epsilon);
            output.intersect = laOutput.intersect;
            return output;
        }

    private:
        friend class UnitTestIntrLine2Arc2;
    };

    template <typename T>
    class FIQuery<T, Line2<T>, Arc2<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0),
                parameter{ C_<T>(0), C_<T>(0) },
                point{}
            {
            }

            bool intersect;
            std::size_t numIntersections;
            std::array<T, 2> parameter;
            std::array<Vector2<T>, 2> point;
        };

        // See Arc2.h for a description of epsilon >= 0.
        Output operator()(Line2<T> const& line, Arc2<T> const& arc, T const& epsilon)
        {
            Output output{};

            FIQuery<T, Line2<T>, Circle2<T>> lcQuery{};
            Circle2<T> circle(arc.center, arc.radius);
            auto lcResult = lcQuery(line, circle);
            if (lcResult.intersect)
            {
                // Test whether line-circle intersections are on the arc.
                output.numIntersections = 0;
                for (std::size_t i = 0; i < lcResult.numIntersections; ++i)
                {
                    if (arc.Contains(lcResult.point[i], epsilon))
                    {
                        output.intersect = true;
                        output.parameter[output.numIntersections] = lcResult.parameter[i];
                        output.point[output.numIntersections] = lcResult.point[i];
                        ++output.numIntersections;
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
        friend class UnitTestIntrLine2Arc2;
    };
}
