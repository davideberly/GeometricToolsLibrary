// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// Least-squares fit of a sphere to a set of points. The algorithms are
// described in Section 5 of
//   https://www.geometrictools.com/Documentation/LeastSquaresFitting.pdf
// FitUsingLengths uses the algorithm of Section 5.1.
// FitUsingSquaredLengths uses the algorithm of Section 5.2.

#include <GTL/Mathematics/Primitives/3D/Sphere3.h>
#include <cmath>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class ApprSphere3
    {
    public:
        // The return value is 'true' when the linear system of the algorithm
        // is solvable, 'false' otherwise. If 'false' is returned, the sphere
        // center and radius are set to zero values.
        bool FitUsingSquaredLengths(std::vector<Vector3<T>> const& points,
            Sphere3<T>& sphere)
        {
            // Compute the average of the data points.
            Vector3<T> average = { C_<T>(0), C_<T>(0), C_<T>(0) };
            for (auto const& point : points)
            {
                average += point;
            }
            T tNumPoints = static_cast<T>(points.size());
            average /= tNumPoints;

            // Compute the covariance matrix M of the Y[i] = X[i]-A and the
            // right-hand side R of the linear system M*(C-A) = R.
            T M00 = C_<T>(0), M01 = C_<T>(0), M02 = C_<T>(0);
            T M11 = C_<T>(0), M12 = C_<T>(0), M22 = C_<T>(0);
            Vector3<T> R = { C_<T>(0), C_<T>(0), C_<T>(0) };
            for (auto const& point : points)
            {
                Vector3<T> Y = point - average;
                T Y0Y0 = Y[0] * Y[0];
                T Y0Y1 = Y[0] * Y[1];
                T Y0Y2 = Y[0] * Y[2];
                T Y1Y1 = Y[1] * Y[1];
                T Y1Y2 = Y[1] * Y[2];
                T Y2Y2 = Y[2] * Y[2];
                M00 += Y0Y0;
                M01 += Y0Y1;
                M02 += Y0Y2;
                M11 += Y1Y1;
                M12 += Y1Y2;
                M22 += Y2Y2;
                R += (Y0Y0 + Y1Y1 + Y2Y2) * Y;
            }
            R *= C_<T>(1, 2);

            // Solve the linear system M*(C-A) = R for the center C.
            T cof00 = M11 * M22 - M12 * M12;
            T cof01 = M02 * M12 - M01 * M22;
            T cof02 = M01 * M12 - M02 * M11;
            T det = M00 * cof00 + M01 * cof01 + M02 * cof02;
            if (det != C_<T>(0))
            {
                T cof11 = M00 * M22 - M02 * M02;
                T cof12 = M01 * M02 - M00 * M12;
                T cof22 = M00 * M11 - M01 * M01;
                sphere.center[0] = average[0] + (cof00 * R[0] + cof01 * R[1] + cof02 * R[2]) / det;
                sphere.center[1] = average[1] + (cof01 * R[0] + cof11 * R[1] + cof12 * R[2]) / det;
                sphere.center[2] = average[2] + (cof02 * R[0] + cof12 * R[1] + cof22 * R[2]) / det;
                T rsqr = C_<T>(0);
                for (auto const& point : points)
                {
                    Vector3<T> delta = point - sphere.center;
                    rsqr += Dot(delta, delta);
                }
                rsqr /= tNumPoints;
                sphere.radius = std::sqrt(rsqr);
                return true;
            }
            else
            {
                sphere.center = { C_<T>(0), C_<T>(0), C_<T>(0) };
                sphere.radius = C_<T>(0);
                return false;
            }
        }

        // Fit the points using lengths to drive the least-squares algorithm.
        // If initialCenterIsAverage is set to 'false', the initial guess for
        // the initial sphere center is computed as the average of the data
        // points. If the data points are clustered along a small solid angle,
        // the algorithm is slow to converge. If initialCenterIsAverage is set
        // to 'true', the incoming sphere center is used as-is to start the
        // iterative algorithm. This approach tends to converge more rapidly
        // than when using the average of points but can be much slower than
        // FitUsingSquaredLengths.
        //
        // The value epsilon may be chosen as a positive number for the
        // comparison of consecutive estimated sphere centers, terminating the
        // iterations when the center difference has length less than or equal
        // to epsilon.
        //
        // The return value is the number of iterations used. If is is the
        // input maxIterations, you can either accept the result or polish the
        // result by calling the function again with initialCenterIsAverage
        // set to 'true'.
        std::size_t FitUsingLengths(std::vector<Vector3<T>> const& points,
            std::size_t maxIterations, bool initialCenterIsAverage,
            Sphere3<T>& sphere, T epsilon = C_<T>(0))
        {
            // Compute the average of the data points.
            Vector3<T> average{ C_<T>(0), C_<T>(0), C_<T>(0) };
            for (auto const& point : points)
            {
                average += point;
            }
            T tNumPoints = static_cast<T>(points.size());
            average /= tNumPoints;

            // The initial guess for the center.
            if (initialCenterIsAverage)
            {
                sphere.center = average;
            }

            T epsilonSqr = epsilon * epsilon;
            std::size_t iteration;
            for (iteration = 0; iteration < maxIterations; ++iteration)
            {
                // Update the iterates.
                Vector3<T> current = sphere.center;

                // Compute average L, dL/da, dL/db, dL/dc.
                T lenAverage = C_<T>(0);
                Vector3<T> derLenAverage{};  // zero vector
                for (auto const& point : points)
                {
                    Vector3<T> diff = point - sphere.center;
                    T length = Length(diff);
                    if (length > C_<T>(0))
                    {
                        lenAverage += length;
                        derLenAverage -= diff / length;
                    }
                }
                lenAverage /= tNumPoints;
                derLenAverage /= tNumPoints;

                sphere.center = average + lenAverage * derLenAverage;
                sphere.radius = lenAverage;

                Vector3<T> diff = sphere.center - current;
                T diffSqrLen = Dot(diff, diff);
                if (diffSqrLen <= epsilonSqr)
                {
                    break;
                }
            }

            return ++iteration;
        }

    private:
        friend class UnitTestApprSphere3;
    };
}
