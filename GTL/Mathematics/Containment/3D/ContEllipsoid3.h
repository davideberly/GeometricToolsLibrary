// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.13

#pragma once

#include <GTL/Mathematics/Approximation/ND/ApprGaussianDistribution.h>
#include <GTL/Mathematics/Primitives/3D/Ellipsoid3.h>
#include <GTL/Mathematics/Projection/ProjectHyperellipsoidToLine.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>
#include <random>
#include <vector>

namespace gtl
{
    template <typename T>
    class ContEllipsoid3
    {
    public:
        // The input points are fit with a Gaussian distribution. The center C
        // of the ellipsoid is chosen to be the mean of the distribution. The
        // axes of the ellipsoid are chosen to be the eigenvectors of the
        // covariance matrix M. The shape of the ellipsoid is determined by
        // the absolute values of the eigenvalues. NOTE: The construction is
        // ill-conditioned if the points are (nearly) collinear or (nearly)
        // planar. In this case M has a (nearly) zero eigenvalue, so inverting
        // M is problematic.
        static void GetContainer(std::vector<Vector3<T>> const& points, Ellipsoid3<T>& ellipsoid)
        {
            // Fit the points with a Gaussian distribution. The covariance
            // matrix is M = sum_j D[j]*U[j]*U[j]^T, where D[j] are the
            // eigenvalues and U[j] are corresponding unit-length
            // eigenvectors.
            Vector3<T> mean{};
            std::array<T, 3> eigenvalues{};
            std::array<Vector3<T>, 3> eigenvectors{};
            (void)ApprGaussianDistribution<T, 3>::Fit(points, mean, eigenvalues, eigenvectors);

            // Grow the ellipsoid, while retaining its shape determined by the
            // covariance matrix, to enclose all the input points. The
            // quadratic
            //   Q(X) = (X - C)^T * M * (X - C)
            //        = (X - C)^T * (sum_j D[j] * U[j] * U[j]^T) * (X - C)
            //        = sum_j D[j] * Dot(U[j], X - C)^2
            // If the maximum value of Q(X[i]) for all input points is V^2,
            // then a bounding ellipsoid is Q(X) = V^2, because Q(X[i]) <= V^2
            // for all i.
            T maxValue = C_<T>(0);
            for (std::size_t i = 0; i < points.size(); ++i)
            {
                Vector3<T> diff = points[i] - mean;
                std::array<T, 3> dot
                {
                    Dot(eigenvectors[0], diff),
                    Dot(eigenvectors[1], diff),
                    Dot(eigenvectors[2], diff)
                };

                T value =
                    eigenvalues[0] * dot[0] * dot[0] +
                    eigenvalues[1] * dot[1] * dot[1] +
                    eigenvalues[2] * dot[2] * dot[2];

                if (value > maxValue)
                {
                    maxValue = value;
                }
            }

            // Arrange for the quadratic to satisfy Q(X) <= 1.
            ellipsoid.center = mean;
            for (std::size_t j = 0; j < 3; ++j)
            {
                ellipsoid.axis[j] = eigenvectors[j];
                ellipsoid.extent[j] = std::sqrt(maxValue / eigenvalues[j]);
            }
        }

        // Test for containment of a point by an ellipsoid.
        static bool InContainer(Vector3<T> const& point, Ellipsoid3<T> const& ellipsoid)
        {
            Vector3<T> diff = point - ellipsoid.center;
            Vector3<T> standardized{
                Dot(diff, ellipsoid.axis[0]) / ellipsoid.extent[0],
                Dot(diff, ellipsoid.axis[1]) / ellipsoid.extent[1],
                Dot(diff, ellipsoid.axis[2]) / ellipsoid.extent[2] };
            return Length(standardized) <= C_<T>(1);
        }

        // Construct a bounding ellipsoid for the two input ellipsoids. The
        // result is not necessarily the minimum-volume ellipsoid containing
        // the two ellipsoids.
        static void MergeContainers(Ellipsoid3<T> const& ellipsoid0,
            Ellipsoid3<T> const& ellipsoid1, Ellipsoid3<T>& merge)
        {
            // Compute the average of the input centers
            merge.center = C_<T>(1, 2) * (ellipsoid0.center + ellipsoid1.center);

            // The bounding ellipsoid orientation is the average of the input
            // orientations.
            Matrix3x3<T> rot0{}, rot1{};
            rot0.SetCol(0, ellipsoid0.axis[0]);
            rot0.SetCol(1, ellipsoid0.axis[1]);
            rot0.SetCol(2, ellipsoid0.axis[2]);
            rot1.SetCol(0, ellipsoid1.axis[0]);
            rot1.SetCol(1, ellipsoid1.axis[1]);
            rot1.SetCol(2, ellipsoid1.axis[2]);
            Quaternion<T> q0{}, q1{};
            RigidMotion<T>::Convert(rot0, q0);
            RigidMotion<T>::Convert(rot1, q1);
            if (Dot(q0, q1) < C_<T>(0))
            {
                q1 = -q1;
            }

            Quaternion<T> q = q0 + q1;
            Normalize(q);
            Matrix3x3<T> rot{};
            RigidMotion<T>::Convert(q, rot);
            for (std::size_t j = 0; j < 3; ++j)
            {
                merge.axis[j] = rot.GetCol(j);
            }

            // Project the input ellipsoids onto the axes obtained by the
            // average of the orientations and that go through the center
            // obtained by the average of the centers.
            for (std::size_t i = 0; i < 3; ++i)
            {
                // Project ellipsoids onto the axis.
                T min0 = C_<T>(0), max0 = C_<T>(0), min1 = C_<T>(0), max1 = C_<T>(0);
                OrthogonalProject(ellipsoid0, merge.center, merge.axis[i], min0, max0);
                OrthogonalProject(ellipsoid1, merge.center, merge.axis[i], min1, max1);

                // Determine the smallest interval containing the projected
                // intervals.
                T maxIntr = (max0 >= max1 ? max0 : max1);
                T minIntr = (min0 <= min1 ? min0 : min1);

                // Update the average center to be the center of the bounding
                // box defined by the projected intervals.
                merge.center += (C_<T>(1, 2) * (minIntr + maxIntr)) * merge.axis[i];

                // Compute the extents of the box based on the new center.
                merge.extent[i] = C_<T>(1, 2) * (maxIntr - minIntr);
            }
        }

        static void GetContainer(std::vector<Vector3<T>> const& points,
            Vector3<T> const& C, Matrix3x3<T> const& R, std::array<T, 3>& D)
        {
            // Compute the constraint coefficients, of the form (A[0],A[1])
            // for each i.
            std::vector<Vector3<T>> A(points.size());
            for (std::size_t i = 0; i < points.size(); ++i)
            {
                Vector3<T> diff = points[i] - C;  // P[i] - C
                Vector3<T> prod = diff * R;  // R^T*(P[i] - C) = (u,v,w)
                A[i] = prod * prod;  // (u^2, v^2, w^2)
            }

            // TODO:  Sort the constraints to eliminate redundant ones. It
            // is clear how to do this in ContEllipse2 for specified C and R.
            // How can this be done in 3D?

            MaxProduct(A, D);
        }

    private:
        static void FindEdgeMax(std::vector<Vector3<T>>& A, std::size_t& plane0,
            std::size_t& plane1, std::array<T, 3>& D)
        {
            std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();

            // Compute direction to local maximum point on line of
            // intersection.
            T xDir = A[plane0][1] * A[plane1][2] - A[plane1][1] * A[plane0][2];
            T yDir = A[plane0][2] * A[plane1][0] - A[plane1][2] * A[plane0][0];
            T zDir = A[plane0][0] * A[plane1][1] - A[plane1][0] * A[plane0][1];

            // Build quadratic Q'(t) = (d/dt)(x(t)y(t)z(t)) = a0+a1*t+a2*t^2.
            T a0 = D[0] * D[1] * zDir + D[0] * D[2] * yDir + D[1] * D[2] * xDir;
            T a1 = C_<T>(2) * (D[2] * xDir * yDir + D[1] * xDir * zDir + D[0] * yDir * zDir);
            T a2 = C_<T>(3) * (xDir * yDir * zDir);

            // Find root to Q'(t) = 0 corresponding to maximum.
            T tFinal = C_<T>(0);
            if (a2 != C_<T>(0))
            {
                T invA2 = C_<T>(1) / a2;
                T discr = a1 * a1 - C_<T>(4) * a0 * a2;
                discr = std::sqrt(std::max(discr, C_<T>(0)));
                tFinal = -C_<T>(1, 2) * (a1 + discr) * invA2;
                if (a1 + C_<T>(2) * a2 * tFinal > C_<T>(0))
                {
                    tFinal = C_<T>(1, 2) * (-a1 + discr) * invA2;
                }
            }
            else if (a1 != C_<T>(0))
            {
                tFinal = -a0 / a1;
            }
            else if (a0 != C_<T>(0))
            {
                T fmax = std::numeric_limits<T>::max();
                tFinal = (a0 >= C_<T>(0) ? fmax : -fmax);
            }
            else
            {
                return;
            }

            if (tFinal < C_<T>(0))
            {
                // Make (xDir,yDir,zDir) point in direction of increase of Q.
                tFinal = -tFinal;
                xDir = -xDir;
                yDir = -yDir;
                zDir = -zDir;
            }

            // Sort remaining planes along line from current point to local
            // maximum.
            T tMax = tFinal;
            std::size_t plane2 = invalid;
            for (std::size_t i = 0; i < A.size(); ++i)
            {
                if (i == plane0 || i == plane1)
                {
                    continue;
                }

                T norDotDir = A[i][0] * xDir + A[i][1] * yDir + A[i][2] * zDir;
                if (norDotDir <= C_<T>(0))
                {
                    continue;
                }

                // Theoretically the numerator must be nonnegative since an
                // invariant in the algorithm is that (x0,y0,z0) is on the
                // convex hull of the constraints. However, some numerical
                // error may make this a small negative number. In that case
                // set tmax = 0 (no change in position).
                T numer = C_<T>(1) - A[i][0] * D[0] - A[i][1] * D[1] - A[i][2] * D[2];
                GTL_RUNTIME_ASSERT(
                    numer >= C_<T>(0),
                    "Unexpected condition.");

                T t = numer / norDotDir;
                if (C_<T>(0) <= t && t < tMax)
                {
                    plane2 = i;
                    tMax = t;
                }
            }

            D[0] += tMax * xDir;
            D[1] += tMax * yDir;
            D[2] += tMax * zDir;

            if (tMax == tFinal)
            {
                return;
            }

            if (tMax > C_<T>(0))
            {
                plane0 = plane2;
                FindFacetMax(A, plane0, D);
                return;
            }

            // tmax == 0, so return with D[0], D[1], and D[2] unchanged.
        }

        static void FindFacetMax(std::vector<Vector3<T>>& A, std::size_t& plane0,
            std::array<T, 3>& D)
        {
            std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();

            T tFinal = C_<T>(0), xDir = C_<T>(0), yDir = C_<T>(0), zDir = C_<T>(0);

            if (A[plane0][0] > C_<T>(0) && A[plane0][1] > C_<T>(0) && A[plane0][2] > C_<T>(0))
            {
                // Compute local maximum point on plane.
                T xMax = C_<T>(1, 3) / A[plane0][0];
                T yMax = C_<T>(1, 3) / A[plane0][1];
                T zMax = C_<T>(1, 3) / A[plane0][2];

                // Compute direction to local maximum point on plane.
                tFinal = C_<T>(1);
                xDir = xMax - D[0];
                yDir = yMax - D[1];
                zDir = zMax - D[2];
            }
            else
            {
                tFinal = std::numeric_limits<T>::max();

                if (A[plane0][0] > C_<T>(0))
                {
                    xDir = C_<T>(0);
                }
                else
                {
                    xDir = C_<T>(0);
                }

                if (A[plane0][1] > C_<T>(0))
                {
                    yDir = C_<T>(0);
                }
                else
                {
                    yDir = C_<T>(1);
                }

                if (A[plane0][2] > C_<T>(0))
                {
                    zDir = C_<T>(0);
                }
                else
                {
                    zDir = C_<T>(1);
                }
            }

            // Sort remaining planes along line from current point.
            T tMax = tFinal;
            std::size_t plane1 = invalid;
            for (std::size_t i = 0; i < A.size(); ++i)
            {
                if (i == plane0)
                {
                    continue;
                }

                T norDotDir = A[i][0] * xDir + A[i][1] * yDir + A[i][2] * zDir;
                if (norDotDir <= C_<T>(0))
                {
                    continue;
                }

                // Theoretically the numerator must be nonnegative because an
                // invariant in the algorithm is that (x0,y0,z0) is on the
                // convex hull of the constraints. However, some numerical
                // error may make this a small negative number. In that case,
                // set tmax = 0 (no change in position).
                T numer = C_<T>(1) - A[i][0] * D[0] - A[i][1] * D[1] - A[i][2] * D[2];
                GTL_RUNTIME_ASSERT(
                    numer >= C_<T>(0),
                    "Unexpected condition.");

                T t = numer / norDotDir;
                if (C_<T>(0) <= t && t < tMax)
                {
                    plane1 = i;
                    tMax = t;
                }
            }

            D[0] += tMax * xDir;
            D[1] += tMax * yDir;
            D[2] += tMax * zDir;

            if (tMax == C_<T>(1))
            {
                return;
            }

            if (tMax > C_<T>(0))
            {
                plane0 = plane1;
                FindFacetMax(A, plane0, D);
                return;
            }

            FindEdgeMax(A, plane0, plane1, D);
        }

        static void MaxProduct(std::vector<Vector3<T>>& A, std::array<T, 3>& D)
        {
            // Maximize x*y*z subject to x >= 0, y >= 0, z >= 0, and
            // A[i]*x+B[i]*y+C[i]*z <= 1 for 0 <= i < N where A[i] >= 0,
            // B[i] >= 0, and C[i] >= 0.

            // Jitter the lines to avoid cases where more than three planes
            // intersect at the same point. This should also break parallelism
            // and planes parallel to the coordinate planes. TODO: Revisit this
            // algorithm and try to avoid jittering.
            std::mt19937 mte{};
            std::uniform_real_distribution<double> urd(0.0, 1e-12);
            for (std::size_t i = 0; i < A.size(); ++i)
            {
                A[i][0] += static_cast<T>(urd(mte));
                A[i][1] += static_cast<T>(urd(mte));
                A[i][2] += static_cast<T>(urd(mte));
            }

            // Sort lines along the z-axis (x = 0 and y = 0).
            std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();
            std::size_t plane = invalid;
            T zmax = C_<T>(0);
            for (std::size_t i = 0; i < A.size(); ++i)
            {
                if (A[i][2] > zmax)
                {
                    zmax = A[i][2];
                    plane = i;
                }
            }
            GTL_RUNTIME_ASSERT(
                plane != invalid,
                "Unexpected condition.");

            // Walk along convex hull searching for maximum.
            D[0] = C_<T>(0);
            D[1] = C_<T>(0);
            D[2] = C_<T>(1) / zmax;
            FindFacetMax(A, plane, D);
        }

    private:
        friend class UnitTestContEllipsoid3;
    };
}
