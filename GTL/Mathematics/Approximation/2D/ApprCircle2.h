// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// Least-squares fit of a circle to a set of points. The algorithms are
// described in Section 5 of
//   https://www.geometrictools.com/Documentation/LeastSquaresFitting.pdf
// FitUsingLengths uses the algorithm of Section 5.1.
// FitUsingSquaredLengths uses the algorithm of Section 5.2.

#include <GTL/Mathematics/Primitives/2D/Circle2.h>
#include <cmath>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class ApprCircle2
    {
    public:
        // The return value is 'true' when the linear system of the algorithm
        // is solvable, 'false' otherwise. If 'false' is returned, the circle
        // center and radius are set to zero values.
        bool FitUsingSquaredLengths(std::vector<Vector2<T>> const& points,
            Circle2<T>& circle)
        {
            // Compute the average of the data points.
            Vector2<T> average{ C_<T>(0), C_<T>(0) };
            for (auto const& point : points)
            {
                average += point;
            }
            T tNumPoints = static_cast<T>(points.size());
            average /= tNumPoints;

            // Compute the covariance matrix M of the Y[i] = X[i]-A and the
            // right-hand side R of the linear system M*(C-A) = R.
            T M00 = C_<T>(0), M01 = C_<T>(0), M11 = C_<T>(0);
            Vector2<T> R{ C_<T>(0), C_<T>(0) };
            for (auto const& point : points)
            {
                Vector2<T> Y = point - average;
                T Y0Y0 = Y[0] * Y[0];
                T Y0Y1 = Y[0] * Y[1];
                T Y1Y1 = Y[1] * Y[1];
                M00 += Y0Y0;
                M01 += Y0Y1;
                M11 += Y1Y1;
                R += (Y0Y0 + Y1Y1) * Y;
            }
            R *= C_<T>(1, 2);

            // Solve the linear system M*(C-A) = R for the center C.
            T det = M00 * M11 - M01 * M01;
            if (det != C_<T>(0))
            {
                circle.center[0] = average[0] + (M11 * R[0] - M01 * R[1]) / det;
                circle.center[1] = average[1] + (M00 * R[1] - M01 * R[0]) / det;
                T rsqr = C_<T>(0);
                for (auto const& point : points)
                {
                    Vector2<T> delta = point - circle.center;
                    rsqr += Dot(delta, delta);
                }
                rsqr /= tNumPoints;
                circle.radius = std::sqrt(rsqr);
                return true;
            }
            else
            {
                circle.center = { C_<T>(0), C_<T>(0) };
                circle.radius = C_<T>(0);
                return false;
            }
        }

        // Fit the points using lengths to drive the least-squares algorithm.
        // If initialCenterIsAverage is set to 'false', the initial guess for
        // the initial circle center is computed as the average of the data
        // points. If the data points are clustered along a small arc, the
        // algorithm is slow to converge. If initialCenterIsAverage is set to
        // 'true', the incoming circle center is used as-is to start the
        // iterative algorithm. This approach tends to converge more rapidly
        // than when using the average of points but can be much slower than
        // FitUsingSquaredLengths.
        //
        // The value epsilon may be chosen as a positive number for the
        // comparison of consecutive estimated circle centers, terminating the
        // iterations when the center difference has length less than or equal
        // to epsilon.
        //
        // The return value is the number of iterations used. If is is the
        // input maxIterations, you can either accept the result or polish the
        // result by calling the function again with initialCenterIsAverage
        // set to 'true'.
        std::size_t FitUsingLengths(std::vector<Vector2<T>> const& points,
            std::size_t maxIterations, bool initialCenterIsAverage,
            Circle2<T>& circle, T epsilon = C_<T>(0))
        {
            // Compute the average of the data points.
            Vector2<T> average{ C_<T>(0), C_<T>(0) };
            for (auto const& point : points)
            {
                average += point;
            }
            T tNumPoints = static_cast<T>(points.size());
            average /= tNumPoints;

            // The initial guess for the center.
            if (initialCenterIsAverage)
            {
                circle.center = average;
            }

            T epsilonSqr = epsilon * epsilon;
            std::size_t iteration;
            for (iteration = 0; iteration < maxIterations; ++iteration)
            {
                // Update the iterates.
                Vector2<T> current = circle.center;

                // Compute average L, dL/da, dL/db.
                T lenAverage = C_<T>(0);
                Vector2<T> derLenAverage{};  // zero vector
                for (auto const& point : points)
                {
                    Vector2<T> diff = point - circle.center;
                    T length = Length(diff);
                    if (length > C_<T>(0))
                    {
                        lenAverage += length;
                        derLenAverage -= diff / length;
                    }
                }
                lenAverage /= tNumPoints;
                derLenAverage /= tNumPoints;

                circle.center = average + lenAverage * derLenAverage;
                circle.radius = lenAverage;

                Vector2<T> diff = circle.center - current;
                T diffSqrLen = Dot(diff, diff);
                if (diffSqrLen <= epsilonSqr)
                {
                    break;
                }
            }

            return ++iteration;
        }

    private:
        friend class UnitTestApprCircle2;
    };
}
