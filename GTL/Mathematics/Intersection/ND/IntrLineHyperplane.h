// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.13

#pragma once

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Distance/ND/DistPointHyperplane.h>
#include <GTL/Mathematics/Primitives/ND/Line.h>
#include <cstddef>
#include <limits>

namespace gtl
{
    template <typename T, std::size_t N>
    class TIQuery<T, Line<T, N>, Hyperplane<T, N>>
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

        // The line is P + t * D, where P is the origin and D is the line
        // direction. It is not necessary that D have unit length, which
        // allows for the segment-hyperplane query when the segment is
        // represented in 2-point format. It is also not necessary that the
        // hyperplane normal be unit length.
        Output operator()(Line<T, N> const& line, Hyperplane<T, N> const& hyperplane)
        {
            Output output{};

            T DdN = Dot(line.direction, hyperplane.normal);
            if (DdN != C_<T>(0))
            {
                // The line is not parallel to the hyperplane, so they must
                // intersect.
                output.intersect = true;
            }
            else
            {
                // The line and hyperplane are parallel.
                DCPQuery<T, Vector<T, N>, Hyperplane<T, N>> vpQuery{};
                auto vpOutput = vpQuery(line.origin, hyperplane);
                output.intersect = (vpOutput.distance == C_<T>(0));
            }

            return output;
        }

    private:
        friend class UnitTestIntrLineHyperplane;
    };

    template <typename T, std::size_t N>
    class FIQuery<T, Line<T, N>, Hyperplane<T, N>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0),
                parameter(C_<T>(0)),
                point{}
            {
            }

            // The number of intersections is
            //   0: The line and hyperplane do not intersect.
            //   1: The line and the hyperplane intersect in a point.
            //   std::numeric_limits<std::size_t>::max(): The line is
            //     contained by the hyperplane. The 'parameter' is
            //     set to zero and the 'point' is the line origin.
            bool intersect;
            std::size_t numIntersections;
            T parameter;
            Vector<T, N> point;
        };

        // The line is P + t * D, where P is the origin and D is the line
        // direction. It is not necessary that D have unit length, which
        // allows for the segment-hyperplane query when the segment is
        // represented in 2-point format. It is also not necessary that the
        // hyperplane normal be unit length.
        Output operator()(Line<T, N> const& line, Hyperplane<T, N> const& hyperplane)
        {
            Output output{};
            DoQuery(line.origin, line.direction, hyperplane, output);
            if (output.intersect)
            {
                output.point = line.origin + output.parameter * line.direction;
            }
            return output;
        }

    protected:
        // The caller must ensure that on entry, 'output' is default
        // constructed as if there is no intersection. If an intersection is
        // found, the 'output' values will be modified accordingly.
        void DoQuery(Vector<T, N> const& lineOrigin,
            Vector<T, N> const& lineDirection, Hyperplane<T, N> const& hyperplane,
            Output& output)
        {
            T DdN = Dot(lineDirection, hyperplane.normal);
            DCPQuery<T, Vector<T, N>, Hyperplane<T, N>> vpQuery{};
            auto vpOutput = vpQuery(lineOrigin, hyperplane);

            if (DdN != C_<T>(0))
            {
                // The line is not parallel to the hyperplane, so they must
                // intersect.
                output.intersect = true;
                output.numIntersections = 1;
                output.parameter = -vpOutput.signedDistance / DdN;
            }
            else
            {
                // The line and hyperplane are parallel. Determine whether the
                // line is on the hyperplane.
                if (vpOutput.distance == C_<T>(0))
                {
                    // The line is coincident with the hyperplane. Choose 0
                    // for the parameter. The 'point' is set to the line
                    // origin by the caller.
                    output.intersect = true;
                    output.numIntersections = std::numeric_limits<std::size_t>::max();
                    output.parameter = C_<T>(0);
                }
                // else: The line is not on the hyperplane.
            }
        }

    private:
        friend class UnitTestIntrLineHyperplane;
    };
}
