// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

#include <GTL/Mathematics/Intersection/2D/IntrLine2Line2.h>
#include <GTL/Mathematics/Primitives/ND/Segment.h>
#include <array>
#include <cstddef>
#include <limits>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Line2<T>, Segment2<T>>
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

            // If the line and segment do not intersect,
            //   intersect = false
            //   numIntersections = 0
            //
            // If the line and segment intersect in a single point,
            //   intersect = true
            //   numIntersections = 1
            //
            // If the line and segment are collinear,
            //   intersect = true
            //   numIntersections = std::numeric_limits<std::size_t>::max()
            bool intersect;
            std::size_t numIntersections;
        };

        Output operator()(Line2<T> const& line, Segment2<T> const& segment)
        {
            Output output{};

            FIQuery<T, Line2<T>, Line2<T>> llQuery{};
            Line2<T> segLine(segment.p[0], segment.p[1] - segment.p[0]);
            auto llOutput = llQuery(line, segLine);
            if (llOutput.numIntersections == 1)
            {
                // Test whether the line-line intersection is on the segment.
                if (llOutput.line1Parameter[0] >= C_<T>(0) &&
                    llOutput.line1Parameter[1] <= C_<T>(1))
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
        friend class UnitTestIntrLine2Segment2;
    };

    template <typename T>
    class FIQuery<T, Line2<T>, Segment2<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0),
                lineParameter{ C_<T>(0), C_<T>(0) },
                segmentParameter{ C_<T>(0), C_<T>(0) },
                point{}
            {
            }

            // If the line and segment do not intersect,
            //   intersect = false
            //   numIntersections = 0
            //   lineParameter[] = { 0, 0 }  // invalid
            //   segmentParameter[] = { 0, 0 }  // invalid
            //   point = { 0, 0 }  // invalid
            //
            // If the line and segment intersect in a single point, the
            // parameter for line is s0 and the parameter for segment is
            // s1 in [0,1],
            //   intersect = true
            //   numIntersections = 1
            //   lineParameter = { s0, s0 }
            //   segmentParameter = { s1, s1 }
            //   point = line.origin + s0 * line.direction
            //         = segment.p[0] + s1 * (segment.p[1] - segment.p[0]);
            //
            // If the line and segment are collinear, let
            // maxT = std::numeric_limits<T>::max(),
            //   intersect = true
            //   numIntersections = std::numeric_limits<std::size_t>::max()
            //   lineParameter[] = { -maxT, +maxT }
            //   segmentParameter[] = { 0, 1 }
            //   point = { 0, 0 }  // invalid

            bool intersect;
            std::size_t numIntersections;
            std::array<T, 2> lineParameter;
            std::array<T, 2> segmentParameter;
            Vector2<T> point;
        };

        Output operator()(Line2<T> const& line, Segment2<T> const& segment)
        {
            Output output{};

            FIQuery<T, Line2<T>, Line2<T>> llQuery{};
            Line2<T> segLine(segment.p[0], segment.p[1] - segment.p[0]);
            auto llOutput = llQuery(line, segLine);
            if (llOutput.numIntersections == 1)
            {
                // Test whether the line-line intersection is on the segment.
                if (llOutput.line1Parameter[0] >= C_<T>(0) &&
                    llOutput.line1Parameter[1] <= C_<T>(1))
                {
                    output.intersect = true;
                    output.numIntersections = 1;
                    output.lineParameter[0] = llOutput.line0Parameter[0];
                    output.lineParameter[1] = output.lineParameter[0];
                    output.segmentParameter[0] = llOutput.line1Parameter[0];
                    output.segmentParameter[1] = output.segmentParameter[0];
                    output.point = llOutput.point;
                }
                else
                {
                    output.intersect = false;
                    output.numIntersections = 0;
                }
            }
            else if (llOutput.numIntersections == std::numeric_limits<std::size_t>::max())
            {
                output.intersect = true;
                output.numIntersections = std::numeric_limits<std::size_t>::max();
                T maxT = std::numeric_limits<T>::max();
                output.lineParameter[0] = -maxT;
                output.lineParameter[1] = +maxT;
                output.segmentParameter[0] = C_<T>(0);
                output.segmentParameter[1] = C_<T>(1);
            }
            else
            {
                output.intersect = false;
                output.numIntersections = 0;
            }

            return output;
        }

    private:
        friend class UnitTestIntrLine2Segment2;
    };
}
