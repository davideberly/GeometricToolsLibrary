// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.09.12

#pragma once

// The algorithm implemented here is in
//   3D Game Engine Design:
//   A Practical Approach to T-Time Computer Graphics,
//   2nd Edition by David Eberly,
//   Morgan Kaufmann Publishers, San Francisco, December 2005
//   Section 13.4.2 Lozenge Containing Points
//
// Compute the plane of the lozenge rectangle using least-squares fit.
// Parallel planes are chosen close enough together so that all the data
// points lie between them. The radius is half the distance between the
// two planes. The half-cylinder and quarter-cylinder side pieces are
// chosen using a method similar to that used for fitting by capsules.

#include <GTL/Utility/Exceptions.h>
#include <GTL/Mathematics/Algebra/Matrix.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <GTL/Mathematics/Approximation/ND/ApprGaussianDistribution.h>
#include <GTL/Mathematics/Distance/ND/DistPointRectangle.h>
#include <GTL/Mathematics/Primitives/ND/Cylinder.h>
#include <GTL/Mathematics/Primitives/ND/Lozenge.h>
#include <GTL/Mathematics/Primitives/ND/OrientedBox.h>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>

namespace gtl
{
    template <typename T>
    class ContLozenge3
    {
    public:
        static void GetContainer(std::vector<Vector3<T>> const& points, Lozenge3<T>& lozenge)
        {
            GTL_ARGUMENT_ASSERT(
                points.size() >= 6,
                "Insufficient number of points for containment.");

            T const zero = C_<T>(0);
            T const half = C_<T>(1, 2);
            T const one = C_<T>(1);

            // Fit the input points with a Gaussian distribution. The returned box
            // center is the mean of the input points. The returned box axes are
            // unit-length eigenvectors of the covariance matrix of the input
            // points. The eigenvalues are return in increasing order.
            OrientedBox3<T> box{};
            std::array<T, 3> eigenvalues{};
            ApprGaussianDistribution<T, 3>::Fit(points, box.center, eigenvalues, box.axis);

            // Transform the input points to box coordinates. The box coordinates
            // are P = C + y[0] * U[0] + y[1] * U[1] + y[2] * U[2], where C is the
            // box center and U[j] are the box axis directions. Note that y[j] =
            // Dot(U[j], P-C) for 0 <= j < 3. The transformed point is
            // P = (y[0],y[1],y[2]), living in a coordinate system with origin
            // (0,0,0) and having basis vectors (1,0,0), (0,1,0), and (0,0,1).
            Matrix3x3<T> rotate{};
            rotate.SetCol(0, box.axis[0]);
            rotate.SetCol(1, box.axis[1]);
            rotate.SetCol(2, box.axis[2]);
            std::vector<Vector3<T>> P(points.size());
            for (std::size_t i = 0; i < P.size(); ++i)
            {
                P[i] = (points[i] - box.center) * rotate;
            }

            // Compute the box extents so that the box tightly fits the
            // projections of the input points onto lines containing the box
            // center and having direction vectors that are the eigenvectors.
            Vector3<T> min{ zero, zero, zero };
            Vector3<T> max{ zero, zero, zero };
            for (auto const& point : P)
            {
                for (std::size_t j = 0; j < 3; ++j)
                {
                    min[j] = std::min(point[j], min[j]);
                    max[j] = std::max(point[j], max[j]);
                }
            }

            // Translate the box center to be the average of the extremes.
            for (std::size_t j = 0; j < 3; ++j)
            {
                box.center += half * (max[j] + min[j]) * box.axis[j];
                box.extent[j] = half * (max[j] - min[j]);
            }

            // Translate the input points based on the new box center.
            for (std::size_t i = 0; i < P.size(); ++i)
            {
                P[i] = (points[i] - box.center) * rotate;
            }

            // Compute the axis-aligned lozenge containing the points. The radius
            // of the lozenge is chosen to be the extent of the box in the
            // box.axis[0] direction, which is the eigenvector of the covariance
            // matrix corresponding to the minimum eigenvalue.
            T radius = box.extent[0];
            T radiusSqr = radius * radius;

            T yCenter0 = radius - box.extent[1];  // yCenter0 < 0
            T yCenter1 = box.extent[1] - radius;  // yCenter1 > 0
            T zCenter0 = radius - box.extent[2];  // zCenter0 < 0
            T zCenter1 = box.extent[2] - radius;  // zCenter1 > 0
            for (std::size_t i = 0; i < P.size(); ++i)
            {
                auto const& source = P[i];
                Vector2<T> diff{};
                T lengthSqr{};

                if (source[1] < yCenter0)
                {
                    diff = { source[0], source[1] - yCenter0 };
                    lengthSqr = Dot(diff, diff);
                    if (lengthSqr > radiusSqr)
                    {
                        yCenter0 = source[1] - std::sqrt(std::max(radiusSqr - source[0] * source[0] - source[2] * source[2], zero));
                    }
                }

                if (source[1] > yCenter1)
                {
                    diff = { source[0], source[1] - yCenter1 };
                    lengthSqr = Dot(diff, diff);
                    if (lengthSqr > radiusSqr)
                    {
                        yCenter1 = source[1] + std::sqrt(std::max(radiusSqr - source[0] * source[0] - source[2] * source[2], zero));
                    }
                }

                if (source[2] < zCenter0)
                {
                    diff = { source[0], source[2] - zCenter0 };
                    lengthSqr = Dot(diff, diff);
                    if (lengthSqr > radiusSqr)
                    {
                        zCenter0 = source[2] - std::sqrt(std::max(radiusSqr - source[0] * source[0] - source[1] * source[1], zero));
                    }
                }

                if (source[2] > zCenter1)
                {
                    diff = { source[0], source[2] - zCenter1 };
                    lengthSqr = Dot(diff, diff);
                    if (lengthSqr > radiusSqr)
                    {
                        zCenter1 = source[2] + std::sqrt(std::max(radiusSqr - source[0] * source[0] - source[1] * source[1], zero));
                    }
                }
            }

            // Set the axis-aligned lozenge parameters.
            lozenge.rectangle.center[0] = zero;
            lozenge.rectangle.center[1] = half * (yCenter1 + yCenter0);
            lozenge.rectangle.center[2] = half * (zCenter1 + zCenter0);
            lozenge.rectangle.axis[0] = { zero, one, zero };
            lozenge.rectangle.axis[1] = { zero, zero, one };
            lozenge.rectangle.extent[0] = half * (yCenter1 - yCenter0);
            lozenge.rectangle.extent[1] = half * (zCenter1 - zCenter0);
            lozenge.radius = radius;

            // Transform the lozenge to the original space of the input
            // points.
            lozenge.rectangle.center = box.center + rotate * lozenge.rectangle.center;
            lozenge.rectangle.axis[0] = rotate * lozenge.rectangle.axis[0];
            lozenge.rectangle.axis[1] = rotate * lozenge.rectangle.axis[1];
        }

        // Test for containment of a point by a lozenge.
        template <typename T>
        static bool InContainer(Vector3<T> const& point, Lozenge3<T> const& lozenge)
        {
            DCPQuery<T, Vector3<T>, Rectangle3<T>> prQuery;
            auto result = prQuery(point, lozenge.rectangle);
            return result.distance <= lozenge.radius;
        }

    private:
        friend class UnitTestContLozenge3;
    };
}
