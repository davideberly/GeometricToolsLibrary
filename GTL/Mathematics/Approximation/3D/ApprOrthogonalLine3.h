// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// Least-squares fit of a line to (x,y,z) data by using distance measurements
// orthogonal to the proposed line. The return value is 'true' if and only if
// the fit is unique; that is, when the maximum eigenvalue is unique. The
// returned line is (P,D), where P is the origin and D is the unit-length
// direction. The squared error for S = (x,y,z) is (S-P)^T*(I-D*D^T)*(S-P).
// The error is |Cross(D, S - P)|.
//
// For details, see Section 4.1 of
// https://www.geometrictools.com/Documentation/LeastSquaresFitting.pdf

#include <GTL/Mathematics/MatrixAnalysis/SymmetricEigensolver.h>
#include <GTL/Mathematics/Primitives/ND/Line.h>
#include <vector>

namespace gtl
{
    template <typename T>
    class ApprOrthogonalLine3
    {
    public:
        static bool Fit(std::vector<Vector3<T>> const& points, Line3<T>& line)
        {
            // Compute the average of the points, which is a point on
            // the line.
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
            covar01 /= tNumPoints;
            covar11 /= tNumPoints;
            covar12 /= tNumPoints;
            covar22 /= tNumPoints;

            // Solve the eigensystem for the covariance matrix.
            SymmetricEigensolver<T, 3> solver;
            solver(covar00, covar01, covar02, covar11, covar12, covar22, false, false);

            // The line direction is the eigenvector in the direction
            // of largest variance of the points.
            line.origin = average;
            line.direction = solver.GetEigenvector(2);

            // The fitted line is unique when the maximum eigenvalue
            // has multiplicity 1.
            return solver.GetEigenvalue(1) < solver.GetEigenvalue(2);
        }

    private:
        friend class UnitTestApprOrthogonalLine3;
    };
}
