// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// Compute the distance between a line and a circle in 2D. The circle is
// considered to be a curve, not a solid disk.
//
// The line is P + t * D, where P is a point on the line and D is not required
// to be unit length.
//
// The circle is C + r * U(s), where C is the center, r > 0 is the radius,
// and U(s) = (cos(s), sin(s)) for s in [0,2*pi).
//
// The closest point on the line is stored in closest[0] with parameter t. The
// closest point on the circle is stored in closest[1]. If the line intersects
// the circle in 1 or 2 points, Output.numIntersections is that number of
// points. The Output.intersections array has that number of valid points.
// When Output.numIntersections is 2, Output.parameter and Output.closest[0,1]
// store Output.intersectionParameters[0] and Output.intersections[0].

#include <GTL/Mathematics/Distance/DistanceClosestPointQuery.h>
#include <GTL/Mathematics/Primitives/2D/Circle2.h>
#include <GTL/Mathematics/Primitives/ND/Line.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class DCPQuery<T, Line2<T>, Circle2<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                distance(C_<T>(0)),
                sqrDistance(C_<T>(0)),
                numClosestPairs(0),
                parameter{ C_<T>(0), C_<T>(0) },
                closest{}
            {
            }

            // These members are relevant for any line and circle. If the line
            // is tangent to the circle, the members output.parameter[0] and
            // output.closest[] are valid and output.numClosestPairs is set
            // to 1.
            T distance, sqrDistance;
            std::size_t numClosestPairs;
            std::array<T, 2> parameter;
            std::array<std::array<Vector2<T>, 2>, 2> closest;
        };

        Output operator()(Line2<T> const& line, Circle2<T> const& circle)
        {
            Output output{};

            // Translate the line and circle so that the circle has center at
            // the origin.
            Vector2<T> delta = line.origin - circle.center;

            // The query computes 'output' relative to the circle with center
            // at the origin.
            DoQuery(delta, line.direction, circle.radius, output);

            // Translate the closest points to the original coordinates and
            // the compute the distance and squared distance.
            for (std::size_t j = 0; j < output.numClosestPairs; ++j)
            {
                for (std::size_t i = 0; i < 2; ++i)
                {
                    output.closest[j][i] += circle.center;
                }
            }

            Vector2<T> diff = output.closest[0][0] - output.closest[0][1];
            output.sqrDistance = Dot(diff, diff);
            output.distance = std::sqrt(output.sqrDistance);
            return output;
        }

    protected:
        // Compute the distance and closest point between a line and an
        // aligned box whose center is the origin.
        static void DoQuery(Vector2<T> const& delta, Vector2<T> const& direction,
            T const& radius, Output& output)
        {
            // Compute the distance from the line to the origin. The distance
            // is d = |Dot(D,Perp(D))|/|D|. The line does not intersect the
            // circle when d > r. The line is tangent to the circle when
            // d = r. The line intersects the circle in 2 points when d < r.
            // Rather than normalize D at this time, replace the comparisons
            // by sign tests for |Dot(D,Perp(D))|^2 - r^2 * |D|^2. This allows
            // for theoretically correct classification of line-circle
            // tangency when using rational arithmetic.
            T const zero = C_<T>(0);
            T dotDirDir = Dot(direction, direction);
            T dotDirDel = Dot(direction, delta);
            T dotPerpDirDel = DotPerp(direction, delta);
            T rSqr = radius * radius;
            T test = dotPerpDirDel * dotPerpDirDel - rSqr * dotDirDir;
            if (test >= zero)
            {
                // When the line-origin distance equals the radius, the line
                // is tangent to the circle; there is 1 point of intersection
                // and the line-circle distance is 0. When the line-origin
                // distance is larger than the radius, the line and circle do
                // not intersect. The closest circle point is the tangent
                // point if the line were to be translated in its normal
                // direction to just touch the circle. In this case, the
                // distance between the circle and line is the difference
                // between the line-origin distance and the radius.

                // Compute the line point closest to the circle.
                output.numClosestPairs = 1;
                output.parameter[0] = -dotDirDel / dotDirDir;
                output.closest[0][0] = delta + output.parameter[0] * direction;
                output.closest[0][1] = output.closest[0][0];

                // Compute the circle point closest to the line.
                if (test > zero)
                {
                    Normalize(output.closest[0][1]);
                    output.closest[0][1] *= radius;
                }
            }
            else // lineOriginDistance < radius
            {
                // The line and circle intersect in 2 points. Solve the
                // quadratic equation a2*t^2 + 2*a1*t + a0 = 0. The solutions
                // are (-a1 +/- sqrt(a1 * a1 - a0 * a2)) / a2. Theoretically,
                // discr > 0. Guard against a negative floating-point output.
                T a0 = Dot(delta, delta) - radius * radius;
                T a1 = dotDirDel;
                T a2 = dotDirDir;
                T discr = std::max(a1 * a1 - a0 * a2, zero);
                T sqrtDiscr = std::sqrt(discr);

                // Evaluate the line parameters but do so to avoid subtractive
                // cancellation.
                T temp = -dotDirDel + (dotDirDel > zero ? -sqrtDiscr : sqrtDiscr);
                output.numClosestPairs = 2;
                output.parameter[0] = temp / dotDirDir;
                output.parameter[1] = a0 / temp;
                if (output.parameter[0] > output.parameter[1])
                {
                    std::swap(output.parameter[0], output.parameter[1]);
                }

                // Compute the intersection points.
                output.closest[0][0] = delta + output.parameter[0] * direction;
                output.closest[0][1] = output.closest[0][0];
                output.closest[1][0] = delta + output.parameter[1] * direction;
                output.closest[1][1] = output.closest[1][0];
            }
        }
    };
}
