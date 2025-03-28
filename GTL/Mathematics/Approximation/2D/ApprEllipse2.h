// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// An ellipse is defined implicitly by (X-C)^T * M * (X-C) = 1, where C is
// the center, M is a positive definite matrix and X is any point on the
// ellipse. The code implements a nonlinear least-squares fitting algorithm
// for the error function
//   F(C,M) = sum_{i=0}^{n-1} ((X[i] - C)^T * M * (X[i] - C) - 1)^2
// for n data points X[0] through X[n-1]. An Ellipse2<T> object has
// member 'center' that corresponds to C. It also has axes with unit-length
// directions 'axis[]' and corresponding axis half-lengths 'extent[]'. The
// matrix is M = sum_{i=0}^1 axis[i] * axis[i]^T / extent[i]^2, where axis[i]
// is a 2x1 vector and axis[i]^T is a 1x2 vector.
//
// The minimizer uses a 2-step gradient descent algorithm.
//
// Given the current (C,M), locate a minimum of
//   G(t) = F(C - t * dF(C,M)/dC, M)
// for t > 0. The function G(t) >= 0 is a polynomial of degree 4 with
// derivative G'(t) that is a polynomial of degree 3. G'(t) must have a
// positive root because G(0) > 0 and G'(0) < 0 and the G-coefficient of t^4
// is positive. The positive root that T produces the smallest G-value is used
// to update the center C' = C - T * dF/dC(C,M).
//
// Given the current (C,M), locate a minimum of
//   H(t) = F(C, M - t * dF(C,M)/dM)
// for t > 0. The function H(t) >= 0 is a polynomial of degree 2 with
// derivative H'(t) that is a polynomial of degree 1. H'(t) must have a
// positive root because H(0) > 0 and H'(0) < 0 and the H-coefficient of t^2
// is positive. The positive root T that produces the smallest G-value is used
// to update the matrix M' = M - T * dF/dC(C,M) as long as M' is positive
// definite. If M' is not positive definite, the root is halved for a finite
// number of steps until M' is positive definite.

#include <GTL/Mathematics/Containment/2D/ContOrientedBox2.h>
#include <GTL/Mathematics/Primitives/2D/Ellipse2.h>
#include <GTL/Mathematics/RootFinders/RootsCubic.h>
#include <array>
#include <cmath>
#include <cstddef>
#include <map>
#include <vector>

namespace gtl
{
    template <typename T>
    class ApprEllipse2
    {
    public:
        // The number of points should be at least 5 because the number of
        // ellipse parameters is 5 (center_x, center_y, orientation_angle,
        // major_axis extent, minor_axis extent).
        // 
        // The numIterations is the number of steps used in updating the
        // estimates of the ellipse matrix and the ellipse center. The value
        // depends on how close the input points are to a purported ellipse.
        // 
        // The numUpdateMatrixIterations is used during the ellipse matrix
        // updates. Sylvester's criterion is used for testing whether the
        // matrix is positive definite, which is a requirement for the
        // ellipse matrix. The criterior uses a loop of iterations at most
        // numUpdateMatrixIterations. If T is a floating-point type, the
        // limit on iterations should be sufficiently large (2048 for 'float',
        // 4096 for 'double'). For a rational type T, the loop will not
        // terminate unless a maximum number of iterations is specified. The
        // maximum in GTE was hard-coded to 2048.
        // 
        // If you want this function to compute the initial guess for the
        // ellipse, set 'useEllipseForInitialGuess' to false. An oriented
        // bounding box containing the points is used to start the minimizer.
        // Set 'useEllipseForInitialGuess' to true if you want the initial
        // guess to be the input ellipse. This is useful if you want to
        // repeat the query. The returned 'T' value is the error function
        // value for the output 'ellipse'.
        static T Fit(std::vector<Vector2<T>> const& points,
            std::size_t numIterations, std::size_t numUpdateMatrixIterations,
            bool useEllipseForInitialGuess, Ellipse2<T>& ellipse)
        {
            Vector2<T> C{};  // the zero vector
            Matrix2x2<T> M{};  // the zero matrix
            if (useEllipseForInitialGuess)
            {
                C = ellipse.center;
                for (std::size_t i = 0; i < 2; ++i)
                {
                    auto product = OuterProduct(ellipse.axis[i], ellipse.axis[i]);
                    M += product / (ellipse.extent[i] * ellipse.extent[i]);
                }
            }
            else
            {
                OrientedBox2<T> box{};
                GetContainer(points, box);
                C = box.center;
                for (std::size_t i = 0; i < 2; ++i)
                {
                    auto product = OuterProduct(box.axis[i], box.axis[i]);
                    M += product / (box.extent[i] * box.extent[i]);
                }
            }

            T error = ErrorFunction(points, C, M);
            for (std::size_t i = 0; i < numIterations; ++i)
            {
                error = UpdateMatrix(points, numUpdateMatrixIterations, C, M);
                error = UpdateCenter(points, M, C);
            }

            // Extract the ellipse axes and extents.
            SymmetricEigensolver<T, 2> solver;
            solver(M(0, 0), M(0, 1), M(1, 1));
            ellipse.center = C;
            for (std::size_t i = 0; i < 2; ++i)
            {
                ellipse.axis[i] = solver.GetEigenvector(i);
                ellipse.extent[i] = C_<T>(1) / std::sqrt(solver.GetEigenvalue(i));
            }

            return error;
        }

    private:
        static T UpdateCenter(std::vector<Vector2<T>> const& points,
            Matrix2x2<T> const& M, Vector2<T>& C)
        {
            T const epsilon = static_cast<T>(1e-06);

            T const tNumPoints = static_cast<T>(points.size());
            std::vector<Vector2<T>> MDelta(points.size());
            std::vector<T> a(points.size());
            Vector2<T> negDFDC{};  // zero vector
            T aMean = C_<T>(0), aaMean = C_<T>(0);
            for (std::size_t i = 0; i < points.size(); ++i)
            {
                Vector2<T> Delta = points[i] - C;
                MDelta[i] = M * Delta;
                a[i] = Dot(Delta, MDelta[i]) - C_<T>(1);
                aMean += a[i];
                aaMean += a[i] * a[i];
                negDFDC += a[i] * MDelta[i];
            }
            aMean /= tNumPoints;
            aaMean /= tNumPoints;
            if (Normalize(negDFDC) < epsilon)
            {
                return aaMean;
            }

            T bMean = C_<T>(0), abMean = C_<T>(0), bbMean = C_<T>(0);
            T c = Dot(negDFDC, M * negDFDC);
            for (std::size_t i = 0; i < points.size(); ++i)
            {
                T b = Dot(negDFDC, MDelta[i]);
                bMean += b;
                abMean += a[i] * b;
                bbMean += b * b;
            }
            bMean /= tNumPoints;
            abMean /= tNumPoints;
            bbMean /= tNumPoints;

            // Compute the coefficients of the quartic polynomial q(t) that
            // represents the error function on the given line in the gradient
            // descent minimization.
            std::array<T, 5> q;
            q[0] = aaMean;
            q[1] = -C_<T>(4) * abMean;
            q[2] = C_<T>(4) * bbMean + C_<T>(2) * c * aMean;
            q[3] = -C_<T>(4) * c * bMean;
            q[4] = c * c;

            // Compute the coefficients of q'(t).
            std::array<T, 4> dq;
            dq[0] = q[1];
            dq[1] = C_<T>(2) * q[2];
            dq[2] = C_<T>(3) * q[3];
            dq[3] = C_<T>(4) * q[4];

            // Compute the roots of q'(t).
            std::array<PolynomialRoot<T>, 3> roots{};
            std::size_t numRoots = RootsCubic<T>::Solve(false, dq[0], dq[1], dq[2], dq[3], roots.data());

            // Choose the root that leads to the minimum along the gradient
            // descent line and update the center to that point.
            T minError = aaMean;
            T minRoot = C_<T>(0);
            for (std::size_t i = 0; i < numRoots; ++i)
            {
                T root = roots[i].x;
                if (root > C_<T>(0))
                {
                    T error = q[0] + root * (q[1] + root * (q[2] + root * (q[3] + root * q[4])));
                    if (error < minError)
                    {
                        minError = error;
                        minRoot = root;
                    }
                }
            }

            if (minRoot > C_<T>(0))
            {
                C += minRoot * negDFDC;
                return minError;
            }
            return aaMean;
        }

        static T UpdateMatrix(std::vector<Vector2<T>> const& points,
            std::size_t numUpdateMatrixIterations, Vector2<T> const& C, Matrix2x2<T>& M)
        {
            T const epsilon = static_cast<T>(1e-06);

            T const tNumPoints = static_cast<T>(points.size());
            std::vector<Vector2<T>> Delta(points.size());
            std::vector<T> a(points.size());
            Matrix2x2<T> negDFDM;  // zero matrix, symmetric

            T aaMean = C_<T>(0);
            for (std::size_t i = 0; i < points.size(); ++i)
            {
                Delta[i] = points[i] - C;
                a[i] = Dot(Delta[i], M * Delta[i]) - C_<T>(1);
                T twoA = C_<T>(2) * a[i];
                negDFDM(0, 0) -= a[i] * Delta[i][0] * Delta[i][0];
                negDFDM(0, 1) -= twoA * Delta[i][0] * Delta[i][1];
                negDFDM(1, 1) -= a[i] * Delta[i][1] * Delta[i][1];
                aaMean += a[i] * a[i];
            }
            negDFDM(1, 0) = negDFDM(0, 1);
            aaMean /= tNumPoints;

            // Normalize the matrix as if it were a vector of numbers.
            T length = L2Norm(negDFDM);
            if (length < epsilon)
            {
                return aaMean;
            }
            negDFDM /= length;

            T abMean = C_<T>(0), bbMean = C_<T>(0);
            for (std::size_t i = 0; i < points.size(); ++i)
            {
                T b = Dot(Delta[i], negDFDM * Delta[i]);
                abMean += a[i] * b;
                bbMean += b * b;
            }
            abMean /= tNumPoints;
            bbMean /= tNumPoints;

            // Compute the coefficients of the quadratic polynomial q(t) that
            // represents the error function on the given line in the gradient
            // descent minimization.
            std::array<T, 3> q;
            q[0] = aaMean;
            q[1] = C_<T>(2) * abMean;
            q[2] = bbMean;

            // Compute the coefficients of q'(t).
            std::array<T, 2> dq;
            dq[0] = q[1];
            dq[1] = C_<T>(2) * q[2];

            // Compute the root as long as it is positive and
            // M + root * negGradM is a positive definite matrix.
            T root = -dq[0] / dq[1];
            if (root > C_<T>(0))
            {
                // Use Sylvester's criterion for testing positive definitess.
                // A for(;;) loop terminates for floating-point arithmetic but
                // not for rational (BSRational<UInteger>) arithmetic. Limit
                // the number of iterations so that the loop terminates for
                // rational arithmetic.
                for (std::size_t k = 0; k < numUpdateMatrixIterations; ++k)
                {
                    Matrix2x2<T> nextM = M + root * negDFDM;
                    if (nextM(0, 0) > C_<T>(0))
                    {
                        T det = Determinant(nextM);
                        if (det > C_<T>(0))
                        {
                            M = nextM;
                            T minError = q[0] + root * (q[1] + root * q[2]);
                            return minError;
                        }
                    }
                    root *= C_<T>(1, 2);
                }
            }
            return aaMean;
        }

        static T ErrorFunction(std::vector<Vector2<T>> const& points,
            Vector2<T> const& C, Matrix2x2<T> const& M)
        {
            T error = C_<T>(0);
            for (auto const& P : points)
            {
                Vector2<T> Delta = P - C;
                T a = Dot(Delta, M * Delta) - C_<T>(1);
                error += a * a;
            }
            error /= static_cast<T>(points.size());
            return error;
        }

    private:
        friend class UnitTestApprEllipse2;
    };
}
