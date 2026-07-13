// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.12

#pragma once

#include <GTL/Mathematics/Approximation/ND/ApprGaussianDistribution.h>
#include <GTL/Mathematics/Primitives/2D/Ellipse2.h>
#include <GTL/Mathematics/Projection/ProjectHyperellipsoidToLine.h>
#include <array>
#include <cmath>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class ContEllipse2
    {
    public:
        // The input points are fit with a Gaussian distribution. The center
        // C of the ellipse is chosen to be the mean of the distribution. The
        // axes of the ellipse are chosen to be the eigenvectors of the
        // covariance matrix M. The shape of the ellipse is determined by the
        // absolute values of the eigenvalues. NOTE: The construction is
        // ill-conditioned if the points are (nearly) collinear. In this case
        // M has a (nearly) zero eigenvalue, so inverting M can be a problem
        // numerically.
        static void GetContainer(std::vector<Vector2<T>> const& points, Ellipse2<T>& ellipse)
        {
            // Fit the points with a Gaussian distribution. The covariance
            // matrix is M = sum_j D[j]*U[j]*U[j]^T, where D[j] are the
            // eigenvalues and U[j] are corresponding unit-length
            // eigenvectors.
            Vector2<T> mean{};
            std::array<T, 2> eigenvalues{};
            std::array<Vector2<T>, 2> eigenvectors{};
            (void)ApprGaussianDistribution<T, 2>::Fit(points, mean, eigenvalues, eigenvectors);

            // Grow the ellipse, while retaining its shape determined by the
            // covariance matrix, to enclose all the input points. The
            // quadratic form that is used for the ellipse construction is
            //   Q(X) = (X - C)^T * M * (X - C)
            //        = (X - C)^T * (sum_j D[j] * U[j] * U[j]^T) * (X - C)
            //        = sum_j D[j] * Dot(U[j], X - C)^2
            // If the maximum value of Q(X[i]) for all input points is V^2,
            // then a bounding ellipse is Q(X) = V^2, because Q(X[i]) <= V^2
            // for all i.
            T maxValue = C_<T>(0);
            for (std::size_t i = 0; i < points.size(); ++i)
            {
                Vector2<T> diff = points[i] - mean;
                std::array<T, 2> dot
                {
                    Dot(eigenvectors[0], diff),
                    Dot(eigenvectors[1], diff)
                };

                T value =
                    eigenvalues[0] * dot[0] * dot[0] +
                    eigenvalues[1] * dot[1] * dot[1];

                if (value > maxValue)
                {
                    maxValue = value;
                }
            }

            // Arrange for the quadratic to satisfy Q(X) <= 1.
            ellipse.center = mean;
            for (std::size_t j = 0; j < 2; ++j)
            {
                ellipse.axis[j] = eigenvectors[j];
                ellipse.extent[j] = std::sqrt(maxValue / eigenvalues[j]);
            }
        }

        // Test for containment of a point inside an ellipse.
        static bool InContainer(Vector2<T> const& point, Ellipse2<T> const& ellipse)
        {
            Vector2<T> diff = point - ellipse.center;
            Vector2<T> standardized{
                Dot(diff, ellipse.axis[0]) / ellipse.extent[0],
                Dot(diff, ellipse.axis[1]) / ellipse.extent[1] };
            return Length(standardized) <= C_<T>(1);
        }

        // Construct a bounding ellipse for the two input ellipses. The result
        // is not necessarily the minimum-area ellipse containing the two
        // ellipses.
        static void MergeContainers(Ellipse2<T> const& ellipse0,
            Ellipse2<T> const& ellipse1, Ellipse2<T>& merge)
        {
            // Compute the average of the input centers.
            merge.center = C_<T>(1, 2) * (ellipse0.center + ellipse1.center);

            // The bounding ellipse orientation is the average of the input
            // orientations.
            if (Dot(ellipse0.axis[0], ellipse1.axis[0]) >= C_<T>(0))
            {
                merge.axis[0] = C_<T>(1, 2) * (ellipse0.axis[0] + ellipse1.axis[0]);
            }
            else
            {
                merge.axis[0] = C_<T>(1, 2) * (ellipse0.axis[0] - ellipse1.axis[0]);
            }
            Normalize(merge.axis[0]);
            merge.axis[1] = -Perp(merge.axis[0]);

            // Project the input ellipses onto the axes obtained by the
            // average of the orientations and that go through the center
            // obtained by the average of the centers.
            for (std::size_t j = 0; j < 2; ++j)
            {
                // Project ellipsoids onto the axis.
                T min0 = C_<T>(0), max0 = C_<T>(0), min1 = C_<T>(0), max1 = C_<T>(0);
                OrthogonalProject(ellipse0, merge.center, merge.axis[j], min0, max0);
                OrthogonalProject(ellipse1, merge.center, merge.axis[j], min1, max1);

                // Determine the smallest interval containing the projected
                // intervals.
                T maxIntr = (max0 >= max1 ? max0 : max1);
                T minIntr = (min0 <= min1 ? min0 : min1);

                // Update the average center to be the center of the bounding
                // box defined by the projected intervals.
                merge.center += (C_<T>(1, 2) * (minIntr + maxIntr)) * merge.axis[j];

                // Compute the extents of the box based on the new center.
                merge.extent[j] = C_<T>(1, 2) * (maxIntr - minIntr);
            }
        }

        // Compute the minimum-area ellipse, (X-C)^T R D R^T (X-C) = 1, given
        // the center C and the orientation matrix R. The columns of R are the
        // axes of the ellipse. The algorithm computes the diagonal matrix D.
        // The minimum area is pi/sqrt(D[0]*D[1]), where D = diag(D[0],D[1]).
        // The problem is equivalent to maximizing the product D[0]*D[1] for
        // a given C and R, and subject to the constraints
        //   (P[i]-C)^T R D R^T (P[i]-C) <= 1
        // for all input points P[i] with 0 <= i < N. Each constraint has the
        // form
        //   A[0]*D[0] + A[1]*D[1] <= 1
        // where A[0] >= 0 and A[1] >= 0.
        static void GetContainer(std::vector<Vector2<T>> const& points,
            Vector2<T> const& C, Matrix2x2<T> const& R, std::array<T, 2>& D)
        {
            // Compute the constraint coefficients of the form (A[0],A[1])
            // for each i.
            std::vector<Vector2<T>> A(points.size());
            for (std::size_t i = 0; i < points.size(); ++i)
            {
                Vector2<T> diff = points[i] - C;  // P[i] - C
                Vector2<T> prod = diff * R;  // R^T*(P[i] - C) = (u,v)
                A[i] = prod * prod;  // (u^2, v^2)
            }

            // Use a lexicographical sort to eliminate redundant constraints.
            // Remove all but the first entry in blocks with x0 = x1 because
            // the corresponding constraint lines for the first entry hides
            // all the others from the origin.
            std::sort(A.begin(), A.end(),
                [](Vector2<T> const& P0, Vector2<T> const& P1)
                {
                    if (P0[0] > P1[0]) { return true; }
                    if (P0[0] < P1[0]) { return false; }
                    return P0[1] > P1[1];
                }
            );
            auto end = std::unique(A.begin(), A.end(),
                [](Vector2<T> const& P0, Vector2<T> const& P1)
                {
                    return P0[0] == P1[0];
                }
            );
            A.erase(end, A.end());

            // Use a lexicographical sort to eliminate redundant constraints.
            // Remove all but the first entry in blocks/ with y0 = y1 because
            // the corresponding constraint lines for the first entry hides
            // all the others from the origin.
            std::sort(A.begin(), A.end(),
                [](Vector2<T> const& P0, Vector2<T> const& P1)
                {
                    if (P0[1] > P1[1])
                    {
                        return true;
                    }

                    if (P0[1] < P1[1])
                    {
                        return false;
                    }

                    return P0[0] > P1[0];
                }
            );
            end = std::unique(A.begin(), A.end(),
                [](Vector2<T> const& P0, Vector2<T> const& P1)
                {
                    return P0[1] == P1[1];
                }
            );
            A.erase(end, A.end());

            MaxProduct(A, D);
        }

    private:
        static void MaxProduct(std::vector<Vector2<T>>& A, std::array<T, 2>& D)
        {
            std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();

            // Keep track of which constraint lines have already been used in
            // the search.
            std::size_t const numConstraints = A.size();
            std::vector<bool> used(A.size());
            std::fill(used.begin(), used.end(), false);

            // Find the constraint line whose y-intercept (0,ymin) is closest
            // to the origin. This line contributes to the convex hull of the
            // constraints and the search for the maximum starts here. Also
            // find the constraint line whose x-intercept (xmin,0) is closest
            // to the origin. This line contributes to the convex hull of the
            // constraints and the search for the maximum terminates before or
            // at this line.
            std::size_t iXMin = invalid, iYMin = invalid;
            T axMax = C_<T>(0), ayMax = C_<T>(0);  // A[i] >= (0,0) by design
            for (std::size_t i = 0; i < numConstraints; ++i)
            {
                // The minimum x-intercept is 1/A[iXMin][0] for A[iXMin][0]
                // the maximum of the A[i][0].
                if (A[i][0] > axMax)
                {
                    axMax = A[i][0];
                    iXMin = i;
                }

                // The minimum y-intercept is 1/A[iYMin][1] for A[iYMin][1]
                // the maximum of the A[i][1].
                if (A[i][1] > ayMax)
                {
                    ayMax = A[i][1];
                    iYMin = i;
                }
            }

            GTL_RUNTIME_ASSERT(
                iXMin != invalid && iYMin != invalid,
                "Unexpected condition.");

            used[iYMin] = true;

            // The convex hull is searched in a clockwise manner starting with
            // the constraint line constructed above. The next vertex of the
            // hull occurs as the closest point to the first vertex on the
            // current constraint line. The following loop finds each
            // consecutive vertex.
            T x0 = C_<T>(0), xMax = C_<T>(1) / axMax;
            std::size_t j{};
            for (j = 0; j < numConstraints; ++j)
            {
                // Find the line whose intersection with the current line is
                // closest to the last hull vertex. The last vertex is at
                // (x0,y0) on the current line.
                T x1 = xMax;
                std::size_t line = invalid;
                for (std::size_t i = 0; i < numConstraints; ++i)
                {
                    if (!used[i])
                    {
                        // This line not yet visited, process it. Given the
                        // current constraint line a0*x+b0*y = 1 and candidate
                        // line a1*x+b1*y = 1, find the point of intersection.
                        // The determinant of the system is d = a0*b1-a1*b0.
                        // We care only about lines that have more negative
                        // slope than the previous one, that is,
                        // -a1/b1 < -a0/b0, in which case we process only
                        // lines for which d < 0.
                        T det = DotPerp(A[iYMin], A[i]);
                        if (det < C_<T>(0))
                        {
                            // Compute the x-value for the point of
                            // intersection, (x1,y1). There may be floating
                            // point error issues in the comparision
                            // 'D[0] <= x1'. Consider modifying to
                            // 'D[0] <= x1 + epsilon'.
                            D[0] = (A[i][1] - A[iYMin][1]) / det;
                            if (x0 < D[0] && D[0] <= x1)
                            {
                                line = i;
                                x1 = D[0];
                            }
                        }
                    }
                }

                // Next vertex is at (x1,y1) whose x-value was computed above.
                // First check for the maximum of x*y on the current line for
                // x in [x0,x1]. On this interval the function is
                // f(x) = x*(1-a0*x)/b0.  The derivative is
                // f'(x) = (1-2*a0*x)/b0 and f'(r) = 0 when r = 1/(2*a0).
                // The three candidates for the maximum are f(x0), f(r) and
                // f(x1). Comparisons are made between r and the endpoints
                // x0 and x1. Because a0 = 0 is possible (constraint line is
                // horizontal and f is increasing on line), the division in r
                // is not performed and the comparisons are made between
                // 1/2 = a0*r and a0*x0 or a0*x1.

                // Compare r < x0.
                if (C_<T>(1, 2) < A[iYMin][0] * x0)
                {
                    // The maximum is f(x0) since the quadratic f decreases
                    // for x > r. The value D[1] is f(x0).
                    D[0] = x0;
                    D[1] = (C_<T>(1) - A[iYMin][0] * D[0]) / A[iYMin][1];
                    break;
                }

                // Compare r < x1.
                if (C_<T>(1, 2) < A[iYMin][0] * x1)
                {
                    // The maximum is f(r). The search ends here because the
                    // current line is tangent to the level curve of
                    // f(x) = f(r) and x*y can therefore only decrease as we
                    // traverse farther around the hull in the clockwise
                    // direction.  The value D[1] is f(r).
                    D[0] = C_<T>(1, 2) / A[iYMin][0];
                    D[1] = C_<T>(1, 2) / A[iYMin][1];
                    break;
                }

                // The maximum is f(x1). The function x*y is potentially
                // larger on the next line, so continue the search.
                GTL_RUNTIME_ASSERT(
                    line != invalid,
                    "Unexpected condition.");

                x0 = x1;
                x1 = xMax;
                used[line] = true;
                iYMin = line;
            }

            GTL_RUNTIME_ASSERT(
                j < numConstraints,
                "Unexpected condition.");
        }

    private:
        friend class UnitTestContEllipse2;
    };
}
