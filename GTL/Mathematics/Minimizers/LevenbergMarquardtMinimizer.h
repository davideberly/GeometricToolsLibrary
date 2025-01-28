// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// Let F(p) = (F_{0}(p), F_{1}(p), ..., F_{n-1}(p)) be a vector-valued
// function of the parameters p = (p_{0}, p_{1}, ..., p_{m-1}). It is
// required that n >= m. The nonlinear least-squares problem is to minimize
// the real-valued error function E(p) = |F(p)|^2, which is the squared
// length of F(p).
//
// Let J = dF/dp = [dF_{r}/dp_{c}] denote the Jacobian matrix, which is the
// matrix of first-order partial derivatives of F. The matrix has n rows and
// m columns, and the indexing (r,c) refers to row r and column c. A
// first-order approximation is F(p + d) = F(p) + J(p)d, where d is an m-by-1
// vector with small length. Consequently, an approximation to E is E(p + d)
// = |F(p + d)|^2 = |F(p) + J(p)d|^2. The goal is to choose d to minimize
// |F(p) + J(p)d|^2 and, hopefully, with E(p + d) < E(p). Choosing an initial
// p_{0}, the hope is that the algorithm generates a sequence p_{i} for which
// E(p_{i+1}) < E(p_{i}) and, in the limit, E(p_{j}) approaches the global
// minimum of E. The algorithm is referred to as Gauss-Newton iteration. If
// E does not decrease for a step of the algorithm, one can modify the
// algorithm to the Levenberg-Marquardt iteration.
//
// For a single Gauss-Newton iteration, we need to choose d to minimize
// |F(p) + J(p)d|^2 where p is fixed. This is a linear least squares problem
// which can be formulated using the normal equations
// (J^T(p)*J(p))*d = -J^T(p)*F(p). The matrix J^T*J is positive semidefinite.
// If it is invertible, then d = -(J^T(p)*J(p))^{-1}*F(p). If it is not
// invertible, some other algorithm must be used to choose d; one option is
// to use gradient descent for the step. A Cholesky decomposition can be
// used to solve the linear system.
//
// Although an implementation can allow the caller to pass an array of
// functions F_{i}(p) and an array of derivatives dF_{r}/dp_{c}, some
// applications might involve a very large n that precludes storing all
// the computed Jacobian matrix entries because of excessive memory
// requirements. In such an application, it is better to compute instead
// the entries of the m-by-m matrix J^T*J and the m-by-1 vector J^T*F.
// Typically, m is small, so the memory requirements are not excessive. Also,
// there might be additional structure to F for which the caller can take
// advantage; for example, 3-tuples of components of F(p) might correspond to
// vectors that can be manipulated using an already existing mathematics
// library. The implementation here supports both approaches.

#include <GTL/Mathematics/MatrixAnalysis/CholeskyDecomposition.h>
#include <cstddef>
#include <functional>
#include <utility>

namespace gtl
{
    template <typename T>
    class LevenbergMarquardtMinimizer
    {
    public:
        // Convenient types for the domain vectors, the range vectors, the
        // function F and the Jacobian J.
        using DVector = Vector<T>;    // numPDimensions
        using RVector = Vector<T>;    // numFDImensions
        using JMatrix = Matrix<T>;    // numFDimensions-by-numPDimensions
        using JTJMatrix = Matrix<T>;  // numPDimensions-by-numPDimensions
        using JTFVector = Vector<T>;  // numPDimensions
        using FFunction = std::function<void(DVector const&, RVector&)>;
        using JFunction = std::function<void(DVector const&, JMatrix&)>;
        using JPlusFunction = std::function<void(DVector const&, JTJMatrix&, JTFVector&)>;

        // Create the minimizer that computes F(p) and J(p) directly.
        LevenbergMarquardtMinimizer(std::size_t numPDimensions, std::size_t numFDimensions,
            FFunction const& inFFunction, JFunction const& inJFunction)
            :
            mNumPDimensions(numPDimensions),
            mNumFDimensions(numFDimensions),
            mFFunction(inFFunction),
            mJFunction(inJFunction),
            mJPlusFunction{},
            mF(mNumFDimensions),
            mJ(mNumFDimensions, mNumPDimensions),
            mJTJ(mNumPDimensions, mNumPDimensions),
            mNegJTF(mNumPDimensions),
            mDecomposer(mNumPDimensions),
            mUseJFunction(true)
        {
            GTL_ARGUMENT_ASSERT(
                mNumPDimensions > 0 && mNumFDimensions > 0,
                "Invalid dimensions.");
        }

        // Create the minimizer that computes J^T(p)*J(p) and -J(p)*F(p).
        LevenbergMarquardtMinimizer(std::size_t numPDimensions, std::size_t numFDimensions,
            FFunction const& inFFunction, JPlusFunction const& inJPlusFunction)
            :
            mNumPDimensions(numPDimensions),
            mNumFDimensions(numFDimensions),
            mFFunction(inFFunction),
            mJFunction{},
            mJPlusFunction(inJPlusFunction),
            mF(mNumFDimensions),
            mJ(mNumFDimensions, mNumPDimensions),
            mJTJ(mNumPDimensions, mNumPDimensions),
            mNegJTF(mNumPDimensions),
            mDecomposer(mNumPDimensions),
            mUseJFunction(false)
        {
            GTL_ARGUMENT_ASSERT(
                mNumPDimensions > 0 && mNumFDimensions > 0,
                "Invalid dimensions.");
        }

        inline std::size_t GetNumPDimensions() const
        {
            return mNumPDimensions;
        }

        inline std::size_t GetNumFDimensions() const
        {
            return mNumFDimensions;
        }

        // The lambda is positive, the multiplier is positive, and the initial
        // guess for the p-parameter is p0. Typical choices are lambda =
        // 0.001 and multiplier = 10. TODO: Explain lambda in more detail,
        // Multiview Geometry mentions lambda = 0.001*average(diagonal(JTJ)),
        // but let us just expose the factor in front of the average.

        struct Output
        {
            Output()
                :
                minLocation{},
                minUpdateLength(C_<T>(0)),
                minErrorDifference(C_<T>(0)),
                minError(C_<T>(0)),
                numIterations(0),
                numAdjustments(0),
                converged(false)
            {
            }

            Output(DVector const& inMinLocation, T const& inMinError,
                T const& inMinUpdateLength, T const& inMinErrorDifference,
                std::size_t inNumIterations, std::size_t inNumAdjustments, bool inConverged)
                :
                minLocation(inMinLocation),
                minUpdateLength(inMinUpdateLength),
                minErrorDifference(inMinErrorDifference),
                minError(inMinError),
                numIterations(inNumIterations),
                numAdjustments(inNumAdjustments),
                converged(inConverged)
            {
            }

            DVector minLocation;
            T minUpdateLength;
            T minErrorDifference;
            T minError;
            std::size_t numIterations;
            std::size_t numAdjustments;
            bool converged;
        };

        Output operator()(DVector const& p0, std::size_t maxIterations,
            T const& updateLengthTolerance, T const& errorDifferenceTolerance,
            T const& lambdaFactor, T const& lambdaAdjust, std::size_t maxAdjustments)
        {
            GTL_ARGUMENT_ASSERT(
                maxIterations > 0 &&
                updateLengthTolerance >= C_<T>(0) &&
                errorDifferenceTolerance >= C_<T>(0) &&
                lambdaFactor > C_<T>(0) &&
                lambdaAdjust > C_<T>(0) &&
                maxAdjustments >= 1,
                "Tolerances must be nonnegative.");

            Output output(p0, C_<T>(0), C_<T>(0), C_<T>(0), 0, 0, false);

            // Compute the initial error.
            mFFunction(p0, mF);
            output.minError = Dot(mF, mF);

            // Do the Levenberg-Marquart iterations.
            T factor = lambdaFactor;
            DVector pCurrent = p0;
            for (output.numIterations = 1; output.numIterations <= maxIterations; ++output.numIterations)
            {
                std::pair<bool, bool> status{};
                DVector pNext{};
                for (output.numAdjustments = 0; output.numAdjustments < maxAdjustments; ++output.numAdjustments)
                {
                    status = DoIteration(pCurrent, factor, updateLengthTolerance,
                        errorDifferenceTolerance, pNext, output);
                    if (status.first)
                    {
                        // Either the Cholesky decomposition failed or the
                        // iterates converged within tolerance. TODO: See the
                        // note in DoIteration about not failing on Cholesky
                        // decomposition.
                        return output;
                    }

                    if (status.second)
                    {
                        // The error has been reduced but we have not yet
                        // converged within tolerance.
                        break;
                    }

                    factor *= lambdaAdjust;
                }

                if (output.numAdjustments < maxAdjustments)
                {
                    // The current value of lambda led us to an update that
                    // reduced the error, but the error is not yet small
                    // enough to conclude we converged. Reduce lambda for the
                    // next outer-loop iteration.
                    factor /= lambdaAdjust;
                }
                else
                {
                    // All lambdas tried during the inner-loop iteration did
                    // not lead to a reduced error. If we do nothing here,
                    // the next inner-loop iteration will continue to multiply
                    // lambda, risking eventual floating-point overflow. To
                    // avoid this, fall back to a Gauss-Newton iterate.
                    status = DoIteration(pCurrent, factor, updateLengthTolerance,
                        errorDifferenceTolerance, pNext, output);
                    if (status.first)
                    {
                        // Either the Cholesky decomposition failed or the
                        // iterates converged within tolerance.  TODO: See the
                        // note in DoIteration about not failing on Cholesky
                        // decomposition.
                        return output;
                    }
                }

                pCurrent = pNext;
            }

            return output;
        }

    private:
        void ComputeLinearSystemInputs(DVector const& pCurrent, T const& lambda)
        {
            if (mUseJFunction)
            {
                mJFunction(pCurrent, mJ);
                mJTJ = MultiplyATB(mJ, mJ);
                mNegJTF = -(mF * mJ);
            }
            else
            {
                mJPlusFunction(pCurrent, mJTJ, mNegJTF);
            }

            T diagonalSum = C_<T>(0);
            for (std::size_t i = 0; i < mNumPDimensions; ++i)
            {
                diagonalSum += mJTJ(i, i);
            }

            T diagonalAdjust = lambda * diagonalSum / static_cast<T>(mNumPDimensions);
            for (std::size_t i = 0; i < mNumPDimensions; ++i)
            {
                mJTJ(i, i) += diagonalAdjust;
            }
        }

        // The returned 'first' is true when the linear system cannot be
        // solved (output.converged is false in this case) or when the
        // error is reduced to within the tolerances specified by the caller
        // (output.converged is true in this case).  When the 'first' value
        // is true, the 'second' value is true when the error is reduced or
        // false when it is not.
        std::pair<bool, bool> DoIteration(DVector const& pCurrent, T const& lambdaFactor,
            T const& updateLengthTolerance, T const& errorDifferenceTolerance,
            DVector& pNext, Output& output)
        {
            ComputeLinearSystemInputs(pCurrent, lambdaFactor);
            if (!mDecomposer.Factor(mJTJ))
            {
                // TODO: The matrix mJTJ is positive semi-definite, so the
                // failure can occur when mJTJ has a zero eigenvalue in
                // which case mJTJ is not invertible. Generate an iterate
                // anyway, perhaps using gradient descent?
                return std::make_pair(true, false);
            }
            mDecomposer.SolveLower(mJTJ, mNegJTF);
            mDecomposer.SolveUpper(mJTJ, mNegJTF);

            pNext = pCurrent + mNegJTF;
            mFFunction(pNext, mF);
            T error = Dot(mF, mF);
            if (error < output.minError)
            {
                output.minErrorDifference = output.minError - error;
                output.minUpdateLength = Length(mNegJTF);
                output.minLocation = pNext;
                output.minError = error;
                if (output.minErrorDifference <= errorDifferenceTolerance ||
                    output.minUpdateLength <= updateLengthTolerance)
                {
                    output.converged = true;
                    return std::make_pair(true, true);
                }
                else
                {
                    return std::make_pair(false, true);
                }
            }
            else
            {
                return std::make_pair(false, false);
            }
        }

        std::size_t mNumPDimensions, mNumFDimensions;
        FFunction mFFunction;
        JFunction mJFunction;
        JPlusFunction mJPlusFunction;

        // Storage for J^T(p)*J(p) and -J^T(p)*F(p) during the iterations.
        RVector mF;
        JMatrix mJ;
        JTJMatrix mJTJ;
        JTFVector mNegJTF;

        CholeskyDecomposition<T> mDecomposer;

        bool mUseJFunction;
    };
}
