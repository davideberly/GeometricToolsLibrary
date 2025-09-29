// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.09.29

#pragma once

// WARNING. The implementation allows you to transform the inputs (x,y) to
// the unit square and perform the interpolation in that space. The idea is
// to keep the floating-point numbers to order 1 for numerical stability of
// the algorithm. The classical thin-plate spline algorithm does not include
// this transformation. The interpolation is invariant to translations and
// rotations of (x,y) but not to scaling unless you scale x, y and f(x,y)
// by the same value. The following document is about thin plate splines.
// https://www.geometrictools.com/Documentation/ThinPlateSplines.pdf

#include <GTL/Mathematics/MatrixAnalysis/GaussianElimination.h>
#include <array>
#include <cmath>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class IntpThinPlateSpline2
    {
    public:
        // Data points are (x,y,f(x,y)). The smoothing parameter must be
        // nonnegative.
        IntpThinPlateSpline2(std::vector<Vector3<T>> const& points, T const& smooth,
            bool transformToUnitSquare)
            :
            mPoints(points.size()),
            mSmooth(smooth),
            mTransformToUnitSquare(transformToUnitSquare),
            mA(points.size()),
            mB{ C_<T>(0), C_<T>(0), C_<T>(0) },
            mXMin{},
            mXMax{},
            mXInvRange{},
            mYMin{},
            mYMax{},
            mYInvRange{}
        {
            GTL_ARGUMENT_ASSERT(
                mPoints.size() >= 3 && mSmooth >= C_<T>(0),
                "Invalid input.");

            if (transformToUnitSquare)
            {
                // Map input (x,y) to unit square. This is not part of the
                // classical thin-plate spline algorithm because the
                // interpolation is not invariant to scalings in (x,y).
                auto extreme = ComputeExtremes(points);
                mXMin = extreme.first[0];
                mXMax = extreme.second[0];
                mYMin = extreme.first[1];
                mYMax = extreme.second[1];
                mXInvRange = C_<T>(1) / (mXMax - mXMin);
                mYInvRange = C_<T>(1) / (mYMax - mYMin);
                for (std::size_t i = 0; i < mPoints.size(); ++i)
                {
                    mPoints[i][0] = (points[i][0] - mXMin) * mXInvRange;
                    mPoints[i][1] = (points[i][1] - mYMin) * mYInvRange;
                }
            }
            else
            {
                // The classical thin-plate spline uses the data as is. The
                // extremes are unused by the interpolator.
                mXMin = C_<T>(0);
                mXMax = C_<T>(1);
                mXInvRange = C_<T>(1);
                mYMin = C_<T>(0);
                mYMax = C_<T>(1);
                mYInvRange = C_<T>(1);
                mPoints = points;
            }

            // Compute matrix A = M + lambda*I [NxN matrix].
            Matrix<T> AMat(mPoints.size(), mPoints.size());
            for (std::size_t row = 0; row < mPoints.size(); ++row)
            {
                for (std::size_t col = 0; col < mPoints.size(); ++col)
                {
                    if (row == col)
                    {
                        AMat(row, col) = mSmooth;
                    }
                    else
                    {
                        T dx = mPoints[row][0] - mPoints[col][0];
                        T dy = mPoints[row][1] - mPoints[col][1];
                        T t = std::sqrt(dx * dx + dy * dy);
                        AMat(row, col) = Kernel(t);
                    }
                }
            }

            // Compute matrix B [Nx3 matrix].
            Matrix<T> BMat(mPoints.size(), 3);
            for (std::size_t row = 0; row < mPoints.size(); ++row)
            {
                BMat(row, 0) = C_<T>(1);
                BMat(row, 1) = mPoints[row][0];
                BMat(row, 2) = mPoints[row][1];
            }

            // Compute A^{-1}.
            T determinant{};
            Matrix<T> invAMat = Inverse(AMat, &determinant);
            GTL_RUNTIME_ASSERT(
                determinant != C_<T>(0),
                "Failed to invert matrix A.");

            // Compute P = B^T A^{-1}  [3xN matrix].
            Matrix<T> PMat = MultiplyATB(BMat, invAMat);

            // Compute Q = P B = B^T A^{-1} B  [3x3 matrix].
            Matrix<T> QMat = PMat * BMat;

            // Compute Q^{-1}.
            Matrix<T> invQMat = Inverse(QMat, &determinant);
            GTL_RUNTIME_ASSERT(
                determinant != C_<T>(0),
                "Failed to invert matrix Q.");

            // Compute P * z.
            std::array<T, 3> prod{};
            for (std::size_t row = 0; row < 3; ++row)
            {
                prod[row] = C_<T>(0);
                for (std::size_t i = 0; i < mPoints.size(); ++i)
                {
                    prod[row] += PMat(row, i) * mPoints[i][2];
                }
            }

            // Compute 'b' vector for smooth thin plate spline.
            for (std::size_t row = 0; row < 3; ++row)
            {
                mB[row] = C_<T>(0);
                for (std::size_t i = 0; i < 3; ++i)
                {
                    mB[row] += invQMat(row, i) * prod[i];
                }
            }

            // Compute z - B * b.
            std::vector<T> tmp(mPoints.size());
            for (std::size_t row = 0; row < mPoints.size(); ++row)
            {
                tmp[row] = mPoints[row][2];
                for (std::size_t i = 0; i < 3; ++i)
                {
                    tmp[row] -= BMat(row, i) * mB[i];
                }
            }

            // Compute 'a' vector for smooth thin plate spline.
            for (std::size_t row = 0; row < mPoints.size(); ++row)
            {
                mA[row] = C_<T>(0);
                for (std::size_t i = 0; i < mPoints.size(); ++i)
                {
                    mA[row] += invAMat(row, i) * tmp[i];
                }
            }
        }

        // Evaluate the interpolator.
        T operator()(T x, T y) const
        {
            if (mTransformToUnitSquare)
            {
                // Map (x,y) to the unit square.
                x = (x - mXMin) * mXInvRange;
                y = (y - mYMin) * mYInvRange;
            }

            T result = mB[0] + mB[1] * x + mB[2] * y;
            for (std::size_t i = 0; i < mPoints.size(); ++i)
            {
                T dx = x - mPoints[i][0];
                T dy = y - mPoints[i][1];
                T t = std::sqrt(dx * dx + dy * dy);
                result += mA[i] * Kernel(t);
            }
            return result;
        }

        // Compute the functional value a^T*M*a when lambda is zero or
        // lambda*w^T*(M+lambda*I)*w when lambda is positive. See the thin
        // plate splines PDF for a description of these quantities.
        T ComputeFunctional() const
        {
            T functional = C_<T>(0);
            for (std::size_t row = 0; row < mPoints.size(); ++row)
            {
                for (std::size_t col = 0; col < mPoints.size(); ++col)
                {
                    if (row == col)
                    {
                        functional += mSmooth * mA[row] * mA[col];
                    }
                    else
                    {
                        T dx = mPoints[row][0] - mPoints[col][1];
                        T dy = mPoints[row][1] - mPoints[col][1];
                        T t = std::sqrt(dx * dx + dy * dy);
                        functional += Kernel(t) * mA[row] * mA[col];
                    }
                }
            }

            if (mSmooth > C_<T>(0))
            {
                functional *= mSmooth;
            }

            return functional;
        }

    private:
        // Kernel(t) = t^2 * log(t^2)
        static T Kernel(T const& t)
        {
            if (t > C_<T>(0))
            {
                T t2 = t * t;
                return t2 * std::log(t2);
            }
            return C_<T>(0);
        }

        // Input data.
        std::vector<Vector3<T>> mPoints;
        T mSmooth;
        bool mTransformToUnitSquare;

        // Thin plate spline coefficients. The A[] coefficients are associated
        // with the Green's functions G(x,y,*) and the B[] coefficients are
        // associated with the affine term B[0] + B[1]*x + B[2]*y.
        std::vector<T> mA;  // mPoints.size() elements
        std::array<T, 3> mB;

        // Extent of input data.
        T mXMin, mXMax, mXInvRange;
        T mYMin, mYMax, mYInvRange;

    private:
        friend class UnitTestIntpThinPlateSpline2;
    };
}
