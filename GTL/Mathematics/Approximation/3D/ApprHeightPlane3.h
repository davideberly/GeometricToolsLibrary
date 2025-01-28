// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// Least-squares fit of a plane to height data (x,y,f(x,y)). The plane is of
// the form (z - z0) = a * (x - x0) + b * (y - y0), where (x0,y0,z0) is the
// average of the sample points and (a,b,-1) is a normal to the plane. Only
// the (a,b) portion is returned (as 'slopes'). The error for point (x,y,z)
// is [a * (x - x0) + b * (y - y0) - (z - z0)]^2.
//
// For details, see Section 3.2 of
// https://www.geometrictools.com/Documentation/LeastSquaresFitting.pdf

#include <GTL/Mathematics/Algebra/Vector.h>
#include <vector>

namespace gtl
{
    template <typename T>
    class ApprHeightPlane3
    {
    public:
        static bool Fit(std::vector<Vector3<T>> const& points,
            Vector3<T>& average, Vector2<T>& slopes)
        {
            // Compute the average of the points, which is a point on
            // the plane.
            MakeZero(average);
            for (auto const& point : points)
            {
                average += point;
            }
            average /= static_cast<T>(points.size());

            // Compute the covariance matrix of the points.
            T covar00 = C_<T>(0), covar01 = C_<T>(0), covar02 = C_<T>(0);
            T covar11 = C_<T>(0), covar12 = C_<T>(0);
            for (auto const& point : points)
            {
                Vector3<T> diff = point - average;
                covar00 += diff[0] * diff[0];
                covar01 += diff[0] * diff[1];
                covar02 += diff[0] * diff[2];
                covar11 += diff[1] * diff[1];
                covar12 += diff[1] * diff[2];
            }

            // Decompose the covariance matrix. If the matrix is not
            // invertible, zeros are returned instead to avoid the inability
            // of rational numbers to represent
            // std::numeric_limits<T>::infinity().
            T det = covar00 * covar11 - covar01 * covar01;
            if (det != C_<T>(0))
            {
                slopes[0] = (covar11 * covar02 - covar01 * covar12) / det;
                slopes[1] = (covar00 * covar12 - covar01 * covar02) / det;
                return true;
            }
            else
            {
                slopes = { C_<T>(0), C_<T>(0) };
                return false;
            }
        }

    private:
        friend class UnitTestApprHeightPlane3;
    };
}
