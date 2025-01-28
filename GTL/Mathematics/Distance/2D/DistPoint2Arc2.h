// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// The input point is stored in the member closest[0]. If a single point on
// the arc is closest to the input point, the member closest[1] is set to
// that point and the equidistant member is set to false. If the entire arc
// is equidistant to the point, the member closest[1] is set to the endpoint
// E0 of the arc and the equidistant member is set to true.

#include <GTL/Mathematics/Distance/2D/DistPoint2Circle2.h>
#include <GTL/Mathematics/Primitives/2D/Arc2.h>
#include <array>
#include <cmath>

namespace gtl
{
    template <typename T>
    class DCPQuery<T, Vector2<T>, Arc2<T>>
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

        Output operator()(Vector2<T> const& point, Arc2<T> const& arc)
        {
            Output output{};

            Circle2<T> circle(arc.center, arc.radius);
            DCPQuery<T, Vector2<T>, Circle2<T>> pcQuery{};
            auto pcResult = pcQuery(point, circle);
            if (!pcResult.equidistant)
            {
                // Test whether the closest circle point is on the arc. If it
                // is, that point is the closest arc point. If it is not, the
                // closest arc point is an arc endpoint. Determine which
                // endpoint that is.
                if (arc.Contains(pcResult.closest[1]))
                {
                    output.distance = pcResult.distance;
                    output.sqrDistance = pcResult.sqrDistance;
                    output.closest = pcResult.closest;
                    output.equidistant = pcResult.equidistant;
                }
                else
                {
                    Vector2<T> diff0 = arc.end[0] - point;
                    Vector2<T> diff1 = arc.end[1] - point;
                    T sqrLength0 = Dot(diff0, diff0);
                    T sqrLength1 = Dot(diff1, diff1);
                    if (sqrLength0 <= sqrLength1)
                    {
                        output.distance = std::sqrt(sqrLength0);
                        output.sqrDistance = sqrLength0;
                        output.closest[0] = point;
                        output.closest[1] = arc.end[0];
                        output.equidistant = false;
                    }
                    else
                    {
                        output.distance = std::sqrt(sqrLength1);
                        output.sqrDistance = sqrLength1;
                        output.closest[0] = point;
                        output.closest[1] = arc.end[1];
                        output.equidistant = false;
                    }
                }
            }
            else
            {
                // The point is the center of the circle containing the arc.
                output.distance = arc.radius;
                output.sqrDistance = arc.radius * arc.radius;
                output.closest[0] = point;
                output.closest[1] = arc.end[0];
                output.equidistant = true;
            }

            return output;
        }

    private:
        friend class UnitTestDistPoint2Arc2;
    };
}
