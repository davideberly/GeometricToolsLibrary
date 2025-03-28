// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// Compute the distance between a solid triangle and a solid aligned box
// in 3D.
// 
// The triangle has vertices <V[0],V[1],V[2]>. A triangle point is
// X = sum_{i=0}^2 b[i] * V[i], where 0 <= b[i] <= 1 for all i and
// sum_{i=0}^2 b[i] = 1.
// 
// The aligned box has minimum corner A and maximum corner B. A box point is X
// where A <= X <= B; the comparisons are componentwise.
// 
// The closest point on the triangle closest is stored in closest[0] with
// barycentric coordinates (b[0],b[1],b[2). The closest point on the box is
// stored in closest[1]. When there are infinitely many choices for the pair
// of closest points, only one of them is returned.

#include <GTL/Mathematics/Distance/3D/DistTriangle3CanonicalBox3.h>
#include <GTL/Mathematics/Distance/3D/DistSegment3CanonicalBox3.h>
#include <GTL/Mathematics/Primitives/ND/AlignedBox.h>

namespace gtl
{
    template <typename T>
    class DCPQuery<T, Triangle3<T>, AlignedBox3<T>>
    {
    public:
        using TBQuery = DCPQuery<T, Triangle3<T>, CanonicalBox3<T>>;
        using Output = typename TBQuery::Output;

        Output operator()(Triangle3<T> const& triangle, AlignedBox3<T> const& box)
        {
            Output output{};

            // Translate the triangle and box so that the box has center at
            // the origin.
            Vector3<T> boxCenter{};
            CanonicalBox3<T> cbox{};
            box.GetCenteredForm(boxCenter, cbox.extent);
            Triangle3<T> xfrmTriangle(
                triangle.v[0] - boxCenter,
                triangle.v[1] - boxCenter,
                triangle.v[2] - boxCenter);

            // The query computes 'output' relative to the box with center
            // at the origin.
            TBQuery tbQuery{};
            output = tbQuery(xfrmTriangle, cbox);

            // Translate the closest points to the original coordinates.
            output.closest[0] += boxCenter;
            output.closest[1] += boxCenter;

            return output;
        }
    private:
        friend class UnitTestDistTriangle3AlignedBox3;
    };
}
