// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// Compute the distance between a ray and a solid aligned box in 2D.
// 
// The ray is P + t * D for t >= 0, where D is not required to be unit length.
// 
// The aligned box has minimum corner A and maximum corner B. A box point is X
// where A <= X <= B; the comparisons are componentwise.
// 
// The closest point on the ray is stored in closest[0] with parameter t. The
// closest point on the box is stored in closest[1]. When there are infinitely
// many choices for the pair of closest points, only one of them is returned.

#include <GTL/Mathematics/Distance/2D/DistLine2AlignedBox2.h>
#include <GTL/Mathematics/Distance/ND/DistPointAlignedBox.h>
#include <GTL/Mathematics/Primitives/ND/Ray.h>

namespace gtl
{
    template <typename T>
    class DCPQuery<T, Ray2<T>, AlignedBox2<T>>
    {
    public:
        using AlignedQuery = DCPQuery<T, Line2<T>, AlignedBox2<T>>;
        using Output = typename AlignedQuery::Output;

        Output operator()(Ray2<T> const& ray, AlignedBox2<T> const& box)
        {
            Output output{};

            Line2<T> line(ray.origin, ray.direction);
            AlignedQuery lbQuery{};
            auto lbOutput = lbQuery(line, box);
            if (lbOutput.parameter >= C_<T>(0))
            {
                output = lbOutput;
            }
            else
            {
                DCPQuery<T, Vector2<T>, AlignedBox2<T>> pbQuery{};
                auto pbResult = pbQuery(ray.origin, box);
                output.distance = pbResult.distance;
                output.sqrDistance = pbResult.sqrDistance;
                output.parameter = C_<T>(0);
                output.closest[0] = ray.origin;
                output.closest[1] = pbResult.closest[1];
            }

            return output;
        }

    private:
        friend class UnitTestDistRay2AlignedBox2;
    };
}
