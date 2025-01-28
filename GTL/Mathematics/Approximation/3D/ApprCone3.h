// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// The cone vertex is V, the unit-length axis direction is U and the
// cone angle is A in (0,pi/2). The cone is defined algebraically by
// those points X for which
//     Dot(U,X-V)/Length(X-V) = cos(A)
// This can be written as a quadratic equation
//     (V-X)^T * (cos(A)^2 - U * U^T) * (V-X) = 0
// with the implicit constraint that Dot(U, X-V) > 0 (X is on the
// "positive" cone). Define W = U/cos(A), so Length(W) > 1 and
//     F(X;V,W) = (V-X)^T * (I - W * W^T) * (V-X) = 0
// The nonlinear least squares fitting of points {X[i]}_{i=0}^{n-1}
// computes V and W to minimize the error function
//     E(V,W) = sum_{i=0}^{n-1} F(X[i];V,W)^2
// I recommend using the Gauss-Newton minimizer when your cone points
// are truly nearly a cone; otherwise, try the Levenberg-Marquardt
// minimizer.
//
// The mathematics used in this implementation are found in
//   https://www.geometrictools.com/Documentation/LeastSquaresFitting.pdf

#include <GTL/Mathematics/Approximation/2D/ApprHeightLine2.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <GTL/Mathematics/Minimizers/GaussNewtonMinimizer.h>
#include <GTL/Mathematics/Minimizers/LevenbergMarquardtMinimizer.h>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <limits>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T>
    class ApprCone3
    {
    public:
        // If you want to specify that coneVertex, coneAxis and coneAngle
        // are the initial guesses for the minimizer, set the parameter
        // useConeInputAsInitialGuess to 'true'. If you want the function
        // to compute initial guesses, set that parameter to 'false'.
        // A Gauss-Newton minimizer is used to fit a cone using nonlinear
        // least-squares. The fitted cone is returned in coneVertex,
        // coneAxis and coneAngle. See GaussNewtonMinimizer.h for a
        // description of the least-squares algorithm and the parameters
        // that it requires.
        using GNOutput = typename GaussNewtonMinimizer<T>::Output;

        static GNOutput Fit(std::vector<Vector3<T>> const& points,
            std::size_t maxIterations, T const& updateLengthTolerance,
            T const& errorDifferenceTolerance, bool useConeInputAsInitialGuess,
            Vector3<T>& coneVertex, Vector3<T>& coneAxis, T& coneAngle)
        {
            std::function<void(Vector<T> const&, Vector<T>&)> FFunction{};
            std::function<void(Vector<T> const&, Matrix<T>&)> JFunction{};
            CreateFunctionObjects(points, FFunction, JFunction);

            GaussNewtonMinimizer<T> minimizer(6, points.size(), FFunction, JFunction);

            auto initial = InitialGuess(points, useConeInputAsInitialGuess,
                coneVertex, coneAxis, coneAngle);

            auto output = minimizer(initial, maxIterations, updateLengthTolerance,
                errorDifferenceTolerance);

            // No test is made for output.converged so that we return some
            // estimates of the cone. The caller can decide how to respond
            // when output.converged is false.
            Finalize(output.minLocation, coneVertex, coneAxis, coneAngle);
            return output;
        }

        // The parameters coneVertex, coneAxis and coneAngle are in/out
        // variables. The caller must provide initial guesses for these.
        // The function estimates the cone parameters and returns them. See
        // GaussNewtonMinimizer.h for a description of the least-squares
        // algorithm and the parameters that it requires. (The file
        // LevenbergMarquardtMinimizer.h directs you to the Gauss-Newton
        // file to read about the parameters.)
        using LMOutput = typename LevenbergMarquardtMinimizer<T>::Output;

        static LMOutput Fit(std::vector<Vector3<T>> const& points,
            std::size_t maxIterations, T const& updateLengthTolerance,
            T const& errorDifferenceTolerance, T const& lambdaFactor,
            T const& lambdaAdjust, std::size_t maxAdjustments, bool useConeInputAsInitialGuess,
            Vector3<T>& coneVertex, Vector3<T>& coneAxis, T& coneAngle)
        {
            std::function<void(Vector<T> const&, Vector<T>&)> FFunction{};
            std::function<void(Vector<T> const&, Matrix<T>&)> JFunction{};
            CreateFunctionObjects(points, FFunction, JFunction);

            LevenbergMarquardtMinimizer<T> minimizer(6, points.size(), FFunction, JFunction);

            auto initial = InitialGuess(points, useConeInputAsInitialGuess,
                coneVertex, coneAxis, coneAngle);

            auto output = minimizer(initial, maxIterations, updateLengthTolerance,
                errorDifferenceTolerance, lambdaFactor, lambdaAdjust, maxAdjustments);

            // No test is made for output.converged so that we return some
            // estimates of the cone. The caller can decide how to respond
            // when output.converged is false.
            Finalize(output.minLocation, coneVertex, coneAxis, coneAngle);
            return output;
        }

    private:
        static void CreateFunctionObjects(std::vector<Vector3<T>> const& points,
            std::function<void(Vector<T> const&, Vector<T>&)>& FFunction,
            std::function<void(Vector<T> const&, Matrix<T>&)>& JFunction)
        {
            // F[i](V,W) = D^T * (I - W * W^T) * D, D = V - X[i], P = (V,W)
            FFunction = [&points](Vector<T> const& P, Vector<T>& F)
            {
                Vector3<T> V = { P[0], P[1], P[2] };
                Vector3<T> W = { P[3], P[4], P[5] };
                for (std::size_t i = 0; i < points.size(); ++i)
                {
                    Vector3<T> delta = V - points[i];
                    T deltaDotW = Dot(delta, W);
                    F[i] = Dot(delta, delta) - deltaDotW * deltaDotW;
                }
            };

            // dF[i]/dV = 2 * (D - Dot(W, D) * W)
            // dF[i]/dW = -2 * Dot(W, D) * D
            JFunction = [&points](Vector<T> const& P, Matrix<T>& J)
            {
                Vector3<T> V{ P[0], P[1], P[2] };
                Vector3<T> W{ P[3], P[4], P[5] };
                for (std::size_t row = 0; row < points.size(); ++row)
                {
                    Vector3<T> delta = V - points[row];
                    T deltaDotW = Dot(delta, W);
                    Vector3<T> temp0 = delta - deltaDotW * W;
                    Vector3<T> temp1 = deltaDotW * delta;
                    for (std::size_t col = 0; col < 3; ++col)
                    {
                        J(row, col) = C_<T>(2) * temp0[col];
                        J(row, col + 3) = -C_<T>(2) * temp1[col];
                    }
                }
            };
        }

        static void ComputeInitialCone(std::vector<Vector3<T>> const& points,
            Vector3<T>& coneVertex, Vector3<T>& coneAxis, T& coneAngle)
        {
            // Compute the average of the sample points.
            T const zero = C_<T>(0);
            Vector3<T> center{ zero, zero, zero };
            T const tNumPoints = static_cast<T>(points.size());
            for (std::size_t i = 0; i < points.size(); ++i)
            {
                center += points[i];
            }
            center /= tNumPoints;

            // The cone axis is estimated from ZZTZ (see the PDF).
            coneAxis = { zero, zero, zero };
            for (std::size_t i = 0; i < points.size(); ++i)
            {
                Vector3<T> delta = points[i] - center;
                coneAxis += delta* Dot(delta, delta);
            }
            Normalize(coneAxis);

            // Compute the signed heights of the points along the cone axis
            // relative to C. These are the projections of the points onto the
            // line C+t*U. Also compute the radial distances of the points
            // from the line C+t*U.
            std::vector<Vector2<T>> hrPairs(points.size());
            T hMin = std::numeric_limits<T>::max(), hMax = -hMin;
            for (std::size_t i = 0; i < points.size(); ++i)
            {
                Vector3<T> delta = points[i] - center;
                T h = Dot(coneAxis, delta);
                hMin = std::min(hMin, h);
                hMax = std::max(hMax, h);
                Vector3<T> projection = delta - Dot(coneAxis, delta) * coneAxis;
                T r = Length(projection);
                hrPairs[i] = { h, r };
            }

            // The radial distance is considered to be a function of height.
            // Fit the (h,r) pairs with a line:
            //   r - rAverage = hrSlope * (h - hAverage)
            ApprHeightLine2<T> fitter{};
            Vector2<T> average{};
            T slope{};
            fitter.Fit(hrPairs, average, slope);
            T hAverage = average[0];
            T rAverage = average[1];
            T hrSlope = slope;

            // If U is directed so that r increases as h increases, U is the
            // correct cone axis estimate. However, if r decreases as h
            // increases, -U is the correct cone axis estimate.
            if (hrSlope < C_<T>(0))
            {
                coneAxis = -coneAxis;
                hrSlope = -hrSlope;
                std::swap(hMin, hMax);
                hMin = -hMin;
                hMax = -hMax;
            }

            // Compute the extreme radial distance values for the points.
            T rMin = rAverage + hrSlope * (hMin - hAverage);
            T rMax = rAverage + hrSlope * (hMax - hAverage);
            T hRange = hMax - hMin;
            T rRange = rMax - rMin;

            // Using trigonometry and right triangles, compute the tangent
            // function of the cone angle.
            T tanAngle = rRange / hRange;
            coneAngle = std::atan2(rRange, hRange);

            // Compute the cone vertex.
            T offset = rMax / tanAngle - hMax;
            coneVertex = center - offset * coneAxis;
        }

        static Vector<T> InitialGuess(std::vector<Vector3<T>> const& points,
            bool useConeInputAsInitialGuess, Vector3<T>& coneVertex,
            Vector3<T>& coneAxis, T& coneAngle)
        {
            T coneCosAngle = C_<T>(0);
            if (useConeInputAsInitialGuess)
            {
                Normalize(coneAxis);
                coneCosAngle = std::cos(coneAngle);
            }
            else
            {
                ComputeInitialCone(points, coneVertex, coneAxis, coneCosAngle);
            }

            // The initial guess for the cone vertex.
            Vector<T> initial(6);
            initial[0] = coneVertex[0];
            initial[1] = coneVertex[1];
            initial[2] = coneVertex[2];

            // The initial guess for the weighted cone axis.
            initial[3] = coneAxis[0] / coneCosAngle;
            initial[4] = coneAxis[1] / coneCosAngle;
            initial[5] = coneAxis[2] / coneCosAngle;
            return initial;
        }

        static void Finalize(Vector<T> const& minLocation, Vector3<T>& coneVertex,
            Vector3<T>& coneAxis, T& coneAngle)
        {
            for (std::size_t i = 0; i < 3; ++i)
            {
                coneVertex[i] = minLocation[i];
                coneAxis[i] = minLocation[i + 3];
            }

            // We know that coneCosAngle will be nonnegative. The std::min
            // call guards against rounding errors producing a number larger
            // than 1. The clamping ensures std::acos will not return a NaN.
            T length = Normalize(coneAxis);
            if (length > C_<T>(1))
            {
                coneAngle = std::acos(std::min(C_<T>(1) / length, C_<T>(1)));
            }
            else
            {
                coneAngle = C_<T>(0);
            }
        }

    private:
        friend class UnitTestApprCone3;
    };
}
