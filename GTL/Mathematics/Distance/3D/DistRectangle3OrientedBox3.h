// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// Compute the distance between a rectangle and a solid oriented box in 3D.
//
// The rectangle has center C, unit-length axis directions W[0] and W[1], and
// extents e[0] and e[1]. A rectangle point is X = C + sum_{i=0}^2 s[i] * W[i]
// where |s[i]| <= e[i] for all i.
//
// The oriented box has center C, unit-length axis directions U[i] and extents
// e[i] for all i. A box point is X = C + sum_i y[i] * U[i], where
// |y[i]| <= e[i] for all i.
// 
// The closest point on the rectangle is stored in closest[0] with
// W-coordinates (s[0],s[1]). The closest point on the box is stored in
// closest[1]. When there are infinitely many choices for the pair of closest
// points, only one of them is returned.
//
// TODO: Modify to support non-unit-length W[].

#include <GTL/Mathematics/Distance/3D/DistRectangle3CanonicalBox3.h>
#include <GTL/Mathematics/Primitives/ND/OrientedBox.h>
#include <array>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class DCPQuery<T, Rectangle3<T>, OrientedBox3<T>>
    {
    public:
        using RBQuery = DCPQuery<T, Rectangle3<T>, CanonicalBox3<T>>;
        using Output = typename RBQuery::Output;

        Output operator()(Rectangle3<T> const& rectangle, OrientedBox3<T> const& box)
        {
            Output output{};

            // Rotate and translate the rectangle and box so that the box is
            // aligned and has center at the origin.
            CanonicalBox3<T> cbox(box.extent);
            Vector3<T> delta = rectangle.center - box.center;
            Vector3<T> xfrmCenter{};
            std::array<Vector3<T>, 2> xfrmAxis{};
            for (std::size_t i = 0; i < 3; ++i)
            {
                xfrmCenter[i] = Dot(box.axis[i], delta);
                for (std::size_t j = 0; j < 2; ++j)
                {
                    xfrmAxis[j][i] = Dot(box.axis[i], rectangle.axis[j]);
                }
            }

            // The query computes 'output' relative to the box with center
            // at the origin.
            Rectangle3<T> xfrmRectangle(xfrmCenter, xfrmAxis, rectangle.extent);
            RBQuery rbQuery{};
            output = rbQuery(xfrmRectangle, cbox);

            // Rotate and translate the closest points to the original
            // coordinates.
            std::array<Vector3<T>, 2> closest{ box.center, box.center };
            for (std::size_t i = 0; i < 2; ++i)
            {
                for (std::size_t j = 0; j < 3; ++j)
                {
                    closest[i] += output.closest[i][j] * box.axis[j];
                }
            }
            output.closest = closest;

            return output;
        }

    private:
        friend class UnitTestDistRectangle3OrientedBox3;
    };
}
