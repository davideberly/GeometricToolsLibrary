// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// Compute the distance between a line and a solid oriented box in 3D.
// 
// The line is P + t * D, where D is not required to be unit length.
// 
// The oriented box has center C, unit-length axis directions U[i] and extents
// e[i] for all i. A box point is X = C + sum_i y[i] * U[i], where
// |y[i]| <= e[i] for all i.
// 
// The closest point on the line is stored in closest[0] with parameter t. The
// closest point on the box is stored in closest[1]. When there are infinitely
// many choices for the pair of closest points, only one of them is returned.

#include <GTL/Mathematics/Distance/3D/DistLine3CanonicalBox3.h>
#include <GTL/Mathematics/Primitives/ND/OrientedBox.h>
#include <array>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class DCPQuery<T, Line3<T>, OrientedBox3<T>>
    {
    public:
        using LBQuery = DCPQuery<T, Line3<T>, CanonicalBox3<T>>;
        using Output = typename LBQuery::Output;

        Output operator()(Line3<T> const& line, OrientedBox3<T> const& box)
        {
            Output output{};

            // Rotate and translate the line and box so that the box is
            // aligned and has center at the origin.
            CanonicalBox3<T> cbox(box.extent);
            Vector3<T> delta = line.origin - box.center;
            Vector3<T> xfrmOrigin{}, xfrmDirection{};
            for (std::size_t i = 0; i < 3; ++i)
            {
                xfrmOrigin[i] = Dot(box.axis[i], delta);
                xfrmDirection[i] = Dot(box.axis[i], line.direction);
            }

            // The query computes 'output' relative to the box with center
            // at the origin.
            Line3<T> xfrmLine(xfrmOrigin, xfrmDirection);
            LBQuery lbQuery{};
            Output lbOutput = lbQuery(xfrmLine, cbox);

            output.distance = lbOutput.distance;
            output.sqrDistance = lbOutput.sqrDistance;
            output.parameter = lbOutput.parameter;

            // Compute the closest point on the line in the original
            // coordinate system.
            output.closest[0] = line.origin + lbOutput.parameter * line.direction;

            // Compute the closest point on the box in the original coordinate
            // system.
            output.closest[1] = box.center;
            for (int32_t j = 0; j < 3; ++j)
            {
                output.closest[1] += lbOutput.closest[1][j] * box.axis[j];
            }

            return output;
        }

    private:
        friend class UnitTestDistLine3OrientedBox3;
    };
}
