// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// Compute the distance between a ray and a solid aligned box in 3D.
// 
// The ray is P + t * D for t >= 0, where D is not required to be unit length.
// 
// The aligned box has minimum corner A and maximum corner B. A box point is X
// where A <= X <= B; the comparisons are componentwise.
// 
// The closest point on the ray is stored in closest[0] with parameter t. The
// closest point on the box is stored in closest[1]. When there are infinitely
// many choices for the pair of closest points, only one of them is returned.

#include <GTL/Mathematics/Distance/3D/DistLine3AlignedBox3.h>
#include <GTL/Mathematics/Distance/ND/DistPointAlignedBox.h>
#include <GTL/Mathematics/Primitives/ND/Ray.h>

namespace gtl
{
    template <typename T>
    class DCPQuery<T, Ray3<T>, AlignedBox3<T>>
    {
    public:
        using AlignedQuery = DCPQuery<T, Line3<T>, AlignedBox3<T>>;
        using Output = typename AlignedQuery::Output;

        Output operator()(Ray3<T> const& ray, AlignedBox3<T> const& box)
        {
            Output output{};

            Line3<T> line(ray.origin, ray.direction);
            AlignedQuery lbQuery{};
            auto lbOutput = lbQuery(line, box);
            if (lbOutput.parameter >= C_<T>(0))
            {
                output = lbOutput;
            }
            else
            {
                DCPQuery<T, Vector3<T>, AlignedBox3<T>> pbQuery{};
                auto pbOutput = pbQuery(ray.origin, box);
                output.distance = pbOutput.distance;
                output.sqrDistance = pbOutput.sqrDistance;
                output.parameter = C_<T>(0);
                output.closest[0] = ray.origin;
                output.closest[1] = pbOutput.closest[1];
            }
            return output;
        }

    private:
        friend class UnitTestDistRay3AlignedBox3;
    };
}
