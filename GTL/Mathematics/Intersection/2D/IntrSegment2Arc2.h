// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

// The queries consider the arc to be a 1-dimensional object.

#include <GTL/Mathematics/Intersection/2D/IntrSegment2Circle2.h>
#include <GTL/Mathematics/Primitives/2D/Arc2.h>
#include <array>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Segment2<T>, Arc2<T>>
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
        Output operator()(Segment2<T> const& segment, Arc2<T> const& arc, T const& epsilon)
        {
            Output output{};
            FIQuery<T, Segment2<T>, Arc2<T>> saQuery{};
            auto saOutput = saQuery(segment, arc, epsilon);
            output.intersect = saOutput.intersect;
            return output;
        }

    private:
        friend class UnitTestIntrSegment2Arc2;
    };

    template <typename T>
    class FIQuery<T, Segment2<T>, Arc2<T>>
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
        Output operator()(Segment2<T> const& segment, Arc2<T> const& arc, T const& epsilon)
        {
            Output output{};

            FIQuery<T, Segment2<T>, Circle2<T>> scQuery{};
            Circle2<T> circle(arc.center, arc.radius);
            auto scOutput = scQuery(segment, circle);
            if (scOutput.intersect)
            {
                // Test whether line-circle intersections are on the arc.
                for (std::size_t i = 0; i < scOutput.numIntersections; ++i)
                {
                    if (arc.Contains(scOutput.point[i], epsilon))
                    {
                        output.intersect = true;
                        output.parameter[output.numIntersections] = scOutput.parameter[i];
                        output.point[output.numIntersections] = scOutput.point[i];
                        ++output.numIntersections;
                    }
                }
            }

            return output;
        }

    private:
        friend class UnitTestIntrSegment2Arc2;
    };
}
