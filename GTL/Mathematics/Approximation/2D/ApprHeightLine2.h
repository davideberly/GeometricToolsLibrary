// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// Least-squares fit of a line to height data (x,f(x)). The line is of the
// form (y - y0) = s * (x - x0), where (x0,y0) is the average of the sample
// points and s is the slope of the line. The error for point (x,y) is
// [a * (x - x0) - (y - y0)]^2.
//
// For details, see Section 3.1 of
// https://www.geometrictools.com/Documentation/LeastSquaresFitting.pdf

#include <GTL/Mathematics/Algebra/Vector.h>
#include <vector>

namespace gtl
{
    template <typename T>
    class ApprHeightLine2
    {
    public:
        static bool Fit(std::vector<Vector2<T>> const& points,
            Vector2<T>& average, T& slope)
        {
            GTL_ARGUMENT_ASSERT(
                points.size() >= 2,
                "Invalid number of points.");

            // Compute the average of the points, which is a point on
            // the line.
            MakeZero(average);
            for (auto const& point : points)
            {
                average += point;
            }
            average /= static_cast<T>(points.size());

            // Compute the covariance matrix of the points.
            T covar00 = C_<T>(0), covar01 = C_<T>(0);
            for (auto const& point : points)
            {
                Vector2<T> diff = point - average;
                covar00 += diff[0] * diff[0];
                covar01 += diff[0] * diff[1];
            }

            // Compute the slope of the line. If the slope is infinite, zero
            // is returned instead to avoid the inability of rational numbers
            // to represent std::numeric_limits<T>::infinity().
            if (covar01 != C_<T>(0))
            {
                slope = covar01 / covar00;
                return true;
            }
            else
            {
                slope = C_<T>(0);
                return false;
            }
        }

    private:
        friend class UnitTestApprHeightLine2;
    };
}
