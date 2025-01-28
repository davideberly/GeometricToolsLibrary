// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// The 3D point-circle distance algorithm is described in
// https://www.geometrictools.com/Documentation/DistanceToCircle3.pdf
// The notation used in the code matches that of the document.

#include <GTL/Mathematics/Distance/3D/DistPoint3Circle3.h>
#include <GTL/Mathematics/Primitives/3D/Arc3.h>
#include <array>
#include <cmath>

namespace gtl
{
    template <typename T>
    class DCPQuery<T, Vector3<T>, Arc3<T>>
    {
    public:
        // The input point is stored in the member closest[0]. If a single
        // point on the arc is closest to the input point, the member
        // closest[1] is set to that point and the equidistant member is set
        // to false. If the entire arc is equidistant to the point, the
        // member closest[1] is set to the arc.end[0] endpoint and the
        // equidistant member is set to true.
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
            std::array<Vector3<T>, 2> closest;
            bool equidistant;
        };
        

        Output operator()(Vector3<T> const& point, Arc3<T> const& arc)
        {
            Output output{};

            Circle3<T> circle(arc.center, arc.normal, arc.radius);
            auto pcOutput = DCPQuery<T, Vector3<T>, Circle3<T>>{}(point, circle);

            if (!pcOutput.equidistant)
            {
                // Clamp the closest point to the arc of the circle.
                Vector3<T> arcClosest{};
                if (arc.Contains(pcOutput.closest[1]))
                {
                    output.closest[0] = point;
                    output.closest[1] = pcOutput.closest[1];
                    Vector3<T> diff = pcOutput.closest[1] - point;
                    output.sqrDistance = Dot(diff, diff);
                    output.distance = std::sqrt(output.sqrDistance);
                }
                else
                {
                    std::array<Vector3<T>, 2> diffs =
                    {
                        arc.end[0] - point,
                        arc.end[1] - point
                    };

                    std::array<T, 2> sqrDistances =
                    {
                        Dot(diffs[0], diffs[0]),
                        Dot(diffs[1], diffs[1])
                    };

                    // On equal distances, choose arc.end[0] as the closest
                    // arc point to the input point. TODO: Does it make sense
                    // to chooce the arc midpoint instead? Does it matter?
                    if (sqrDistances[0] <= sqrDistances[1])
                    {
                        output.closest[0] = point;
                        output.closest[1] = arc.end[0];
                        output.sqrDistance = sqrDistances[0];
                        output.distance = std::sqrt(output.sqrDistance);
                        output.equidistant = false;
                    }
                    else
                    {
                        output.closest[0] = point;
                        output.closest[1] = arc.end[1];
                        output.sqrDistance = sqrDistances[1];
                        output.distance = std::sqrt(output.sqrDistance);
                        output.equidistant = false;
                    }
                }
            }
            else
            {
                output.closest[0] = point;
                output.closest[1] = arc.end[0];
                Vector3<T> diff = arc.end[0] - point;
                output.sqrDistance = Dot(diff, diff);
                output.distance = std::sqrt(output.sqrDistance);
                output.equidistant = true;
            }

            return output;
        }

    private:
        friend class UnitTestDistPoint3Circle3;
    };
}
