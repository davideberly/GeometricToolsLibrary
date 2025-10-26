// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.26

#pragma once

// In addition to the least-squares fit of a great circle, the input vectors
// are projected onto that circle. The sector of smallest angle (possibly
// obtuse) that contains the points is computed. The endpoints of the arc of
// the sector are returned. The returned endpoints A0 and A1 are perpendicular
// to the returned normal N. Moreover, when you view the arc by looking at the
// plane of the great circle with a view direction of -N, the arc is traversed
// counterclockwise starting at A0 and ending at A1.

#include <GTL/Mathematics/Approximation/3D/ApprGreatCircle3.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class ApprGreatArc3
    {
    public:
        static void Fit(std::vector<Vector3<T>> const& points,
            Vector3<T>& normal, Vector3<T>& arcEnd0, Vector3<T>& arcEnd1)
        {
            // Get the least-squares great circle for the vectors. The circle
            // is on the plane Dot(N,X) = 0. Generate a basis from the great
            // circle normal.
            Vector3<T> U{}, V{};  // { normal, U, V }
            ApprGreatCircle3<T>::Fit(points, normal);
            ComputeOrthonormalBasis(1, normal, U, V);

            // The vectors are X[i] = u[i]*U + v[i]*V + w[i]*normal. The
            // projections are
            //   P[i] = (u[i]*U + v[i]*V) / sqrt(u[i]*u[i] + v[i]*v[i])
            // The great circle is parameterized by
            //   C(t) = cos(t)*U + sin(t)*V
            // Compute the angles t in [-pi,pi] for the projections onto the
            // great circle. It is not necesarily to normalize (u[i],v[i]),
            // instead computing t = atan2(v[i],u[i]). The items[] represents
            // (u, v, angle).
            std::vector<std::array<T, 3>> items(points.size());
            for (std::size_t i = 0; i < points.size(); ++i)
            {
                items[i][0] = Dot(U, points[i]);
                items[i][1] = Dot(V, points[i]);
                items[i][2] = std::atan2(items[i][1], items[i][0]);
            }
            std::sort(items.begin(), items.end(),
                [](std::array<T, 3> const& item0, std::array<T, 3> const& item1)
                {
                    return item0[2] < item1[2];
                }
            );

            // Locate the pair of consecutive angles whose difference is a
            // maximum. Effectively, we are constructing a cone of minimum
            // angle that contains the unit-length vectors.
            std::size_t numPointsM1 = points.size() - 1;
            T maxDiff = C_TWO_PI<T> + items[0][2] - items[numPointsM1][2];
            std::size_t end0 = 0, end1 = numPointsM1;
            for (std::size_t i0 = 0, i1 = 1; i0 < numPointsM1; i0 = i1++)
            {
                T diff = items[i1][2] - items[i0][2];
                if (diff > maxDiff)
                {
                    maxDiff = diff;
                    end0 = i1;
                    end1 = i0;
                }
            }

            arcEnd0 = items[end0][0] * U + items[end0][1] * V;
            arcEnd1 = items[end1][0] * U + items[end1][1] * V;
            Normalize(arcEnd0);
            Normalize(arcEnd1);
        }

    private:
        friend class UnitTestApprGreatArc3;
    };
}
