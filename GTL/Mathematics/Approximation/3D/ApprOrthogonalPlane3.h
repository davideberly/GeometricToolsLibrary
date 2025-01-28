// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// Least-squares fit of a plane to (x,y,z) data by using distance measurements
// orthogonal to the proposed plane. The return value is 'true' if and only if
// the fit is unique; that is, when the minimum eigenvalue is unique. The
// returned plane has origin P and unit-length normal N. The error for
// S = (x,y,z) is |Dot(N, S-P)|.
//
// For details, see Section 4.2 of
// https://www.geometrictools.com/Documentation/LeastSquaresFitting.pdf

#include <GTL/Mathematics/Algebra/Vector.h>
#include <GTL/Mathematics/MatrixAnalysis/SymmetricEigensolver.h>
#include <vector>

namespace gtl
{
    template <typename T>
    class ApprOrthogonalPlane3
    {
    public:
        static bool Fit(std::vector<Vector3<T>> const& points,
            Vector3<T>& origin, Vector3<T>& normal)
        {
            // Compute the average of the points, which is a point on the
            // plane.
            Vector3<T> average{};  // zero vector
            for (auto const& point : points)
            {
                average += point;
            }
            T tNumPoints = static_cast<T>(points.size());
            average /= tNumPoints;

            // Compute the covariance matrix of the points.
            T covar00 = C_<T>(0), covar01 = C_<T>(0), covar02 = C_<T>(0);
            T covar11 = C_<T>(0), covar12 = C_<T>(0), covar22 = C_<T>(0);
            for (auto const& point : points)
            {
                Vector3<T> diff = point - average;
                covar00 += diff[0] * diff[0];
                covar01 += diff[0] * diff[1];
                covar02 += diff[0] * diff[2];
                covar11 += diff[1] * diff[1];
                covar12 += diff[1] * diff[2];
                covar22 += diff[2] * diff[2];
            }
            covar00 /= tNumPoints;
            covar01 /= tNumPoints;
            covar02 /= tNumPoints;
            covar11 /= tNumPoints;
            covar12 /= tNumPoints;
            covar22 /= tNumPoints;

            // Solve the eigensystem for the covariance matrix.
            SymmetricEigensolver<T, 3> solver;
            solver(covar00, covar01, covar02, covar11, covar12, covar22, false, false);

            // The plane normal is the eigenvector in the direction of
            // smallest variance of the points.
            origin = average;
            normal = solver.GetEigenvector(0);

            // The fitted plane is unique when the minimum eigenvalue
            // has multiplicity 1.
            return solver.GetEigenvalue(0) < solver.GetEigenvalue(1);
        }

    private:
        friend class UnitTestApprOrthogonalPlane3;
    };
}
