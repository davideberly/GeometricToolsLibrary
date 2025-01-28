// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// Compute the distance between a point and a line in nD.
// 
// The line is P + t * D, where D is not required to be unit length.
// 
// The input point is stored in closest[0]. The closest point on the line is
// stored in closest[1].

#include <GTL/Mathematics/Distance/DistanceClosestPointQuery.h>
#include <GTL/Mathematics/Primitives/ND/Line.h>
#include <array>
#include <cmath>
#include <cstddef>

namespace gtl
{
    template <typename T, std::size_t N>
    class DCPQuery<T, Vector<T, N>, Line<T, N>>
    {
    public:
        struct Output
        {
            Output()
                :
                distance(C_<T>(0)),
                sqrDistance(C_<T>(0)),
                parameter(C_<T>(0)),
                closest{}
            {
            }

            T distance, sqrDistance;
            T parameter;
            std::array<Vector<T, N>, 2> closest;
        };

        Output operator()(Vector<T, N> const& point, Line<T, N> const& line)
        {
            Output output{};

            Vector<T, N> diff = point - line.origin;
            output.parameter = Dot(line.direction, diff) / Dot(line.direction, line.direction);
            output.closest[0] = point;
            output.closest[1] = line.origin + output.parameter * line.direction;
            diff = output.closest[0] - output.closest[1];
            output.sqrDistance = Dot(diff, diff);
            output.distance = std::sqrt(output.sqrDistance);

            return output;
        }

    private:
        friend class UnitTestDistPointLine;
    };
}
