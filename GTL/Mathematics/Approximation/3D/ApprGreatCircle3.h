// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.26

#pragma once

// Least-squares fit of a great circle to unit-length vectors (x,y,z) by using
// distance measurements orthogonal, measured along great circles, to the
// proposed great circle. The inputs points[] are unit length. The returned
// value is unit length, call it N. The fitted great circle is defined by
// Dot(N,X) = 0, where X is a unit-length vector on the great circle.

#include <GTL/Mathematics/Algebra/Vector.h>
#include <GTL/Mathematics/MatrixAnalysis/SymmetricEigensolver.h>
#include <vector>

namespace gtl
{
    template <typename T>
    class ApprGreatCircle3
    {
    public:
        static void Fit(std::vector<Vector3<T>> const& points, Vector3<T>& normal)
        {
            // Compute the covariance matrix of the vectors.
            T covar00 = C_<T>(0), covar01 = C_<T>(0), covar02 = C_<T>(0);
            T covar11 = C_<T>(0), covar12 = C_<T>(0), covar22 = C_<T>(0);
            for (auto const& point : points)
            {
                covar00 += point[0] * point[0];
                covar01 += point[0] * point[1];
                covar02 += point[0] * point[2];
                covar11 += point[1] * point[1];
                covar12 += point[1] * point[2];
                covar22 += point[2] * point[2];
            }

            T tNumPoints = static_cast<T>(points.size());
            covar00 /= tNumPoints;
            covar01 /= tNumPoints;
            covar02 /= tNumPoints;
            covar11 /= tNumPoints;
            covar12 /= tNumPoints;
            covar22 /= tNumPoints;

            // Solve the eigensystem. The normal vector is the eigenvector
            // corresponding to the smallest eigenvalue.
            SymmetricEigensolver<T, 3> solver{};
            solver(covar00, covar01, covar02, covar11, covar12, covar22, false, false);
            normal = solver.GetEigenvector(0);
        }

    private:
        friend class UnitTestApprGreatCircle3;
    };
}
