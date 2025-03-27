// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// The algorithm for least-squares fitting of a point set by a parabola is
// described in
//   https://www.geometrictools.com/Documentation/LeastSquaresFitting.pdf

#include <GTL/Mathematics/MatrixAnalysis/LinearSystem.h>
#include <GTL/Mathematics/Algebra/Matrix.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace gtl
{
    template <typename T>
    class ApprParabola2
    {
    public:
        // A successful fit is indicated by return value of 'true'. Set the
        // meanSquareError to a nonnull pointer if you want the least-squares
        // error.

        // Fit with y = u0*x^2 + u1*x + u0
        static bool Fit(std::vector<Vector2<T>> const& points,
            std::array<T, 3>& u, T* meanSquareError = nullptr)
        {
            return Fit(points.size(), points.data(), u, meanSquareError);
        }

        static bool Fit(std::size_t numPoints, Vector2<T> const* points,
            std::array<T, 3>& u, T* meanSquareError = nullptr)
        {
            GTL_ARGUMENT_ASSERT(
                numPoints >= 3,
                "Insufficient points to fit with a parabola.");

            Matrix3x3<T> A{};
            Vector3<T> B{};

            for (std::size_t i = 0; i < numPoints; i++)
            {
                T x2 = points[i][0] * points[i][0];
                T x3 = points[i][0] * x2;
                T x4 = x2 * x2;
                T x2y = x2 * points[i][1];
                T xy = points[i][0] * points[i][1];

                A(0, 0) += x4;
                A(0, 1) += x3;
                A(0, 2) += x2;
                A(1, 2) += points[i][0];

                B[0] += x2y;
                B[1] += xy;
                B[2] += points[i][1];
            }

            A(1, 0) = A(0, 1);
            A(1, 1) = A(0, 2);
            A(2, 0) = A(0, 2);
            A(2, 1) = A(1, 2);
            A(2, 2) = C_<T>(1);

            // A(2, 2) is already normalized, so no need to divide it by
            // tNumPoints.
            T tNumPoints = static_cast<T>(numPoints);
            for (std::size_t i = 0; i < 8; ++i)
            {
                A[i] /= tNumPoints;
            }

            for (std::size_t i = 0; i < 3; ++i)
            {
                B[i] /= tNumPoints;
            }

            Vector3<T> vecU{};
            bool success = LinearSystem<T>().Solve(A, B, vecU);
            if (success && meanSquareError != nullptr)
            {
                u = { vecU[0], vecU[1], vecU[2] };

                T totalError = C_<T>(0);
                for (std::size_t i = 0; i < numPoints; ++i)
                {
                    Vector2<T> const& point = points[i];
                    T error = std::fabs(
                        u[0] * point[0] * point[0] +
                        u[1] * point[0] +
                        u[2] - point[1]);
                    totalError += error * error;
                }
                totalError = std::sqrt(totalError * totalError) / tNumPoints;
                *meanSquareError = totalError;
            }
            return success;
        }

        // Fit with y-b = v0*(x-a)^2 + v1*(x-a) + v2
        static bool FitRobust(std::vector<Vector2<T>> const& points,
            Vector2<T>& average, std::array<T, 3>& v, T* meanSquareError = nullptr)
        {
            return FitRobust(points.size(), points.data(), average, v, meanSquareError);
        }

        static bool FitRobust(std::size_t numPoints, Vector2<T> const* points,
            Vector2<T>& average, std::array<T, 3>& v, T* meanSquareError = nullptr)
        {
            GTL_ARGUMENT_ASSERT(
                numPoints >= 3,
                "Insufficient points to fit with a parabola.");

            Matrix3x3<T> A{};
            Vector3<T> B{};

            // Compute the mean of the points.
            T tNumPoints = static_cast<T>(numPoints);
            MakeZero(average);
            for (std::size_t i = 0; i < numPoints; ++i)
            {
                average += points[i];
            }
            average /= tNumPoints;

            for (std::size_t i = 0; i < numPoints; i++)
            {
                Vector2<T> diff = points[i] - average;
                T x2 = diff[0] * diff[0];
                T x3 = diff[0] * x2;
                T x4 = x2 * x2;
                T x2y = x2 * diff[1];
                T xy = diff[0] * diff[1];

                A(0, 0) += x4;
                A(0, 1) += x3;
                A(0, 2) += x2;
                A(1, 2) += diff[0];

                B[0] += x2y;
                B[1] += xy;
                B[2] += diff[1];
            }

            A(1, 0) = A(0, 1);
            A(1, 1) = A(0, 2);
            A(2, 0) = A(0, 2);
            A(2, 1) = A(1, 2);
            A(2, 2) = C_<T>(1);

            // A(2, 2) is already normalized, so no need to divide it by
            // tNumPoints.
            for (std::size_t i = 0; i < 8; ++i)
            {
                A[i] /= tNumPoints;
            }

            for (std::size_t i = 0; i < 3; ++i)
            {
                B[i] /= tNumPoints;
            }

            Vector3<T> vecV{};
            bool success = LinearSystem<T>().Solve(A, B, vecV);
            if (success && meanSquareError != nullptr)
            {
                v = { vecV[0], vecV[1], vecV[2] };

                T totalError = C_<T>(0);
                for (std::size_t i = 0; i < numPoints; ++i)
                {
                    Vector2<T> diff = points[i] - average;
                    T error = std::fabs(
                        v[0] * diff[0] * diff[0] +
                        v[1] * diff[0] +
                        v[2] - diff[1]);
                    totalError += error * error;
                }
                totalError = std::sqrt(totalError * totalError) / tNumPoints;
                *meanSquareError = totalError;
            }
            return success;
        }
        
    private:
        friend class UnitTestApprParabola2;
    };
}
