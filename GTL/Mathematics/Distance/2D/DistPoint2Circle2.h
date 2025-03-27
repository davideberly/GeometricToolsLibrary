// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// The input point is stored in the member closest[0]. If a single point on
// the circle is closest to the input point, the member closest[1] is set to
// that point and the equidistant member is set to false. If the entire circle
// is equidistant to the point, the member closest[1] is set to C+r*(1,0),
// where C is the circle center and r is the circle radius, and the
// equidistant member is set to true.

#include <GTL/Mathematics/Distance/DistanceClosestPointQuery.h>
#include <GTL/Mathematics/Primitives/2D/Circle2.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>
#include <cmath>

namespace gtl
{
    template <typename T>
    class DCPQuery<T, Vector2<T>, Circle2<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                distance(C_<T>(0)),
                sqrDistance(C_<T>(0)),
                closest{},
                equidistant(false)
            {
            }

            T distance, sqrDistance;
            std::array<Vector2<T>, 2> closest;
            bool equidistant;
        };

        Output operator()(Vector2<T> const& point, Circle2<T> const& circle)
        {
            Output output{};

            Vector2<T> diff = point - circle.center;
            T sqrLength = Dot(diff, diff);
            T length = std::sqrt(sqrLength);
            if (length > C_<T>(0))
            {
                diff /= length;
                output.distance = std::fabs(length - circle.radius);
                output.sqrDistance = output.distance * output.distance;
                output.closest[0] = point;
                output.closest[1] = circle.center + circle.radius * diff;
                output.equidistant = false;
            }
            else
            {
                output.distance = circle.radius;
                output.sqrDistance = circle.radius * circle.radius;
                output.closest[0] = point;
                output.closest[1] = circle.center + circle.radius * Vector2<T>{ 1.0, 0.0 };
                output.equidistant = true;
            }

            return output;
        }

    private:
        friend class UnitTestDistPoint2Circle2;
    };
}
