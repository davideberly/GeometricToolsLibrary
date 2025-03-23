// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.03.23

#pragma once

// IntpBSplineUniform is the class for B-spline interpolation of uniformly
// spaced N-dimensional data. The algorithm is described in
//   https://www.geometrictools.com/Documentation/BSplineInterpolation.pdf
//
// The Controls adapter allows access to your control points without regard to
// how you organize your data. You can even defer the computation of a control
// point until it is needed via the operator()(...) calls that Controls must
// provide, and you can cache the points according to your own needs. The
// minimal interface for a Controls adapter is
//
// struct Controls
// {
//   // The control_point_type is of your choosing. It must support
//   // assignment, scalar multiplication and addition. Specifically, if
//   // C0, C1, and C2 are control points and s is a scalar, the interpolator
//   // needs to perform operations
//   //   C1 = C0;
//   //   C1 = C0 * s;
//   //   C2 = C0 + C1;
//   using Type = control_point_type;
//
//   // The number of elements in the specified dimension.
//   std::size_t GetSize(std::size_t dimension) const;
//
//   // Get a control point based on an N-tuple lookup. The interpolator does
//   // not need to know your organization; all it needs is the desired
//   // control point. The 'tuple' input must have N elements.
//   Type operator()(std::size_t const* tuple) const;
//
//   // If you choose to use the specialized interpolators for dimensions
//   // 1, 2, or 3, you must also provide the following accessor, where the
//   // input is an N-tuple listed component-by-component (1, 2, or 3
//   // components).
//   Type operator()(std::size_t i0, std::size_t i1, ..., std::size_t iNm1)
//       const;
// }

#include <GTL/Mathematics/Interpolation/ND/IntpBSplineUniformShared.h>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace gtl
{
    template <typename T, typename Controls>
    class IntpBSplineUniform : public IntpBSplineUniformShared<T>
    {
    public:
        // The caller is responsible for ensuring that the 'controls' exist as
        // long as the IntpBSplineUniform exists.
        IntpBSplineUniform(std::vector<std::size_t> const& degree, Controls const& controls,
            typename Controls::Type const& ctZero, std::uint32_t cacheMode)
            :
            N(degree.size()),
            mDegree(degree),
            mControls(&controls),
            mCTZero(ctZero),
            mCacheMode(cacheMode),
            mNumLocalControls(0),
            mDegreeP1(N),
            mNumControls(N),
            mTMin(N),
            mTMax(N),
            mBlender(N),
            mDCoefficient(N),
            mLMax(N),
            mPowerDSDT(N),
            mITuple(N),
            mJTuple(N),
            mKTuple(N),
            mLTuple(N),
            mSumIJTuple(N),
            mUTuple(N),
            mPTuple(N),
            mPhi{},
            mTBound{},
            mComputeJTuple{},
            mComputeSumIJTuple{},
            mDegreeMinusOrder{},
            mTerm{},
            mTensor{},
            mCached{}
        {
            GTL_ARGUMENT_ASSERT(
                N > 0,
                "The dimension must be positive.");

            // The condition c+1 > d+1 is required so that when s = c+1-d, its
            // maximum value, we have at least two s-knots (d and d + 1).
            for (std::size_t d = 0; d < N; ++d)
            {
                GTL_ARGUMENT_ASSERT(
                    mDegree[d] > 0 &&
                    mControls->GetSize(d) > mDegree[d] + 1,
                    "Incompatible degree and number of controls.");
            }

            GTL_ARGUMENT_ASSERT(
                mCacheMode < IntpBSplineUniformShared<T>::NUM_CACHING_MODES,
                "Invalid caching mode.");

            mNumLocalControls = 1;
            for (std::size_t d = 0; d < N; ++d)
            {
                mDegree[d] = degree[d];
                mDegreeP1[d] = degree[d] + 1;
                mNumLocalControls *= mDegreeP1[d];
                mNumControls[d] = controls.GetSize(d);
                mTMin[d] = static_cast<T>(-0.5);
                mTMax[d] = static_cast<T>(mNumControls[d]) - static_cast<T>(0.5);
                IntpBSplineUniformShared<T>::ComputeBlendingMatrix(mDegree[d], mBlender[d]);
                IntpBSplineUniformShared<T>::ComputeDCoefficients(mDegree[d], mDCoefficient[d], mLMax[d]);
                IntpBSplineUniformShared<T>::ComputePowers(mDegree[d], mNumControls[d], mTMin[d], mTMax[d], mPowerDSDT[d]);
            }

            if (mCacheMode == IntpBSplineUniformShared<T>::NO_CACHING)
            {
                mPhi.resize(N);
                for (std::size_t d = 0; d < N; ++d)
                {
                    mPhi[d].resize(mDegreeP1[d]);
                }
            }
            else
            {
                InitializeTensors();
            }
        }

        // Disallow copying and moving.
        IntpBSplineUniform(IntpBSplineUniform const&) = delete;
        IntpBSplineUniform& operator=(IntpBSplineUniform const&) = delete;
        IntpBSplineUniform(IntpBSplineUniform&&) = delete;
        IntpBSplineUniform& operator=(IntpBSplineUniform&&) = delete;

        // Member access. The input d specifies the dimension, 0 <= d < N.
        inline std::size_t GetDegree(std::size_t d) const
        {
            return mDegree[d];
        }

        inline std::size_t GetNumControls(std::size_t d) const
        {
            return mNumControls[d];
        }

        inline T GetTMin(std::size_t d) const
        {
            return mTMin[d];
        }

        inline T GetTMax(std::size_t d) const
        {
            return mTMax[d];
        }

        inline std::uint32_t GetCacheMode() const
        {
            return mCacheMode;
        }

        // Evaluate the interpolator. Each element of 'order' indicates the
        // order of the derivative you want to compute. For the function value
        // itself, pass in 'order' that has all 0 elements.
        typename Controls::Type Evaluate(std::vector<std::size_t> const& order,
            std::vector<T> const& t)
        {
            if (order.size() >= N && t.size() >= N)
            {
                if (mCacheMode == IntpBSplineUniformShared<T>::NO_CACHING)
                {
                    return EvaluateNoCaching(order.data(), t.data());
                }
                else
                {
                    return EvaluateCaching(order.data(), t.data());
                }
            }
            else
            {
                return mCTZero;
            }
        }

    protected:

        // For the multidimensional tensor Phi{iTuple, kTuple), compute the
        // portion of the 1-dimensional index that corresponds to iTuple.
        std::size_t GetRowIndex(std::vector<std::size_t> const& i) const
        {
            std::size_t rowIndex = i[N - 1];
            std::size_t j1 = 2 * N - 2;
            for (std::size_t c = 2, j0 = N - 2; c <= N; ++c, --j0, --j1)
            {
                rowIndex = mTBound[j1] * rowIndex + i[j0];
            }
            rowIndex = mTBound[j1] * rowIndex;
            return rowIndex;
        }

        // For the multidimensional tensor Phi{iTuple, kTuple), combine the
        // GetRowIndex(...) output with kTuple to produce the full
        // 1-dimensional index.
        std::size_t GetIndex(std::size_t rowIndex, std::vector<std::size_t> const& k) const
        {
            std::size_t index = rowIndex + k[N - 1];
            for (std::size_t c = 2, j = N - 2; c <= N; ++c, --j)
            {
                index = mTBound[j] * index + k[j];
            }
            return index;
        }

        // Compute Phi(iTuple, kTuple).  The 'index' value is an already
        // computed 1-dimensional index for the tensor.
        void ComputeTensor(std::size_t const* i, std::size_t const* k, std::size_t index)
        {
            typename Controls::Type element = mCTZero;
            for (std::size_t d = 0; d < N; ++d)
            {
                mComputeJTuple[d] = 0;
            }
            for (std::size_t iterate = 0; iterate < mNumLocalControls; ++iterate)
            {
                T blend(1);
                for (std::size_t d = 0; d < N; ++d)
                {
                    blend *= mBlender[d][k[d] + static_cast<size_t>(mDegreeP1[d]) * mComputeJTuple[d]];
                    mComputeSumIJTuple[d] = i[d] + mComputeJTuple[d];
                }
                element = element + (*mControls)(mComputeSumIJTuple.data()) * blend;

                for (std::size_t d = 0; d < N; ++d)
                {
                    if (++mComputeJTuple[d] < mDegreeP1[d])
                    {
                        break;
                    }
                    mComputeJTuple[d] = 0;
                }
            }
            mTensor[index] = element;
        }

        // Allocate the containers used for caching and fill in the tensor
        // for precaching when that mode is selected.
        void InitializeTensors()
        {
            mTBound.resize(2 * static_cast<size_t>(N));
            mComputeJTuple.resize(N);
            mComputeSumIJTuple.resize(N);
            mDegreeMinusOrder.resize(N);
            mTerm.resize(N);

            std::size_t current = 0;
            std::size_t numCached = 1;
            for (std::size_t d = 0; d < N; ++d, ++current)
            {
                mTBound[current] = mDegreeP1[d];
                numCached *= mTBound[current];
            }
            for (std::size_t d = 0; d < N; ++d, ++current)
            {
                mTBound[current] = mNumControls[d] - mDegree[d];
                numCached *= mTBound[current];
            }
            mTensor.resize(numCached);
            mCached.resize(numCached);
            if (mCacheMode == IntpBSplineUniformShared<T>::PRE_CACHING)
            {
                std::vector<std::size_t> tuple(2 * static_cast<size_t>(N), 0);
                for (std::size_t index = 0; index < numCached; ++index)
                {
                    ComputeTensor(&tuple[N], &tuple[0], index);
                    for (std::size_t i = 0; i < 2 * N; ++i)
                    {
                        if (++tuple[i] < mTBound[i])
                        {
                            break;
                        }
                        tuple[i] = 0;
                    }
                }
                std::fill(mCached.begin(), mCached.end(), true);
            }
            else
            {
                std::fill(mCached.begin(), mCached.end(), false);
            }
        }

        // Evaluate the interpolator.  Each element of 'order' indicates the
        // order of the derivative you want to compute.  For the function
        // value itself, pass in 'order' that has all 0 elements.
        typename Controls::Type EvaluateNoCaching(std::size_t const* order, T const* t)
        {
            typename Controls::Type result = mCTZero;
            for (std::size_t d = 0; d < N; ++d)
            {
                if (order[d] < 0 || order[d] > mDegree[d])
                {
                    return result;
                }
            }

            for (std::size_t d = 0; d < N; ++d)
            {
                IntpBSplineUniformShared<T>::GetKey(t[d], mTMin[d], mTMax[d], mPowerDSDT[d][1],
                    mNumControls[d], mDegree[d], mITuple[d], mUTuple[d]);
            }

            for (std::size_t d = 0; d < N; ++d)
            {
                std::size_t jIndex = 0;
                for (std::size_t j = 0; j <= mDegree[d]; ++j)
                {
                    std::size_t kjIndex = mDegree[d] + jIndex;
                    std::size_t ell = mLMax[d][order[d]];
                    mPhi[d][j] = C_<T>(0);
                    for (std::size_t c = order[d], k = mDegree[d]; c <= mDegree[d]; ++c, --k)
                    {
                        mPhi[d][j] = mPhi[d][j] * mUTuple[d] +
                            mBlender[d][kjIndex--] * mDCoefficient[d][ell--];
                    }
                    jIndex += mDegreeP1[d];
                }
            }

            for (std::size_t d = 0; d < N; ++d)
            {
                mJTuple[d] = 0;
                mSumIJTuple[d] = mITuple[d];
                mPTuple[d] = mPhi[d][0];
            }
            for (std::size_t iterate = 0; iterate < mNumLocalControls; ++iterate)
            {
                T product(1);
                for (std::size_t d = 0; d < N; ++d)
                {
                    product *= mPTuple[d];
                }

                result = result + (*mControls)(mSumIJTuple.data()) * product;

                for (std::size_t d = 0; d < N; ++d)
                {
                    if (++mJTuple[d] <= mDegree[d])
                    {
                        mSumIJTuple[d] = mITuple[d] + mJTuple[d];
                        mPTuple[d] = mPhi[d][mJTuple[d]];
                        break;
                    }
                    mJTuple[d] = 0;
                    mSumIJTuple[d] = mITuple[d];
                    mPTuple[d] = mPhi[d][0];
                }
            }

            T adjust(1);
            for (std::size_t d = 0; d < N; ++d)
            {
                adjust *= mPowerDSDT[d][order[d]];
            }
            result = result * adjust;
            return result;
        }

        typename Controls::Type EvaluateCaching(std::size_t const* order, T const* t)
        {
            std::size_t numIterates = 1;
            for (std::size_t d = 0; d < N; ++d)
            {
                mDegreeMinusOrder[d] = mDegree[d] - order[d];
                if (mDegreeMinusOrder[d] < 0 || mDegreeMinusOrder[d] > mDegree[d])
                {
                    return mCTZero;
                }
                numIterates *= mDegreeMinusOrder[d] + 1;
            }

            for (std::size_t d = 0; d < N; ++d)
            {
                IntpBSplineUniformShared<T>::GetKey(t[d], mTMin[d], mTMax[d], mPowerDSDT[d][1],
                    mNumControls[d], mDegree[d], mITuple[d], mUTuple[d]);
            }

            std::size_t rowIndex = GetRowIndex(mITuple);
            for (std::size_t d = 0; d < N; ++d)
            {
                mJTuple[d] = 0;
                mKTuple[d] = mDegree[d];
                mLTuple[d] = mLMax[d][order[d]];
                mTerm[d] = mCTZero;
            }
            for (std::size_t iterate = 0; iterate < numIterates; ++iterate)
            {
                std::size_t index = GetIndex(rowIndex, mKTuple);
                if (mCacheMode == IntpBSplineUniformShared<T>::ON_DEMAND_CACHING &&
                    !mCached[index])
                {
                    ComputeTensor(mITuple.data(), mKTuple.data(), index);
                    mCached[index] = true;
                }
                mTerm[0] = mTerm[0] * mUTuple[0] + mTensor[index] * mDCoefficient[0][mLTuple[0]];
                for (std::size_t d = 0; d < N; ++d)
                {
                    if (++mJTuple[d] <= mDegreeMinusOrder[d])
                    {
                        --mKTuple[d];
                        --mLTuple[d];
                        break;
                    }
                    std::size_t dimp1 = d + 1;
                    if (dimp1 < N)
                    {
                        mTerm[dimp1] = mTerm[dimp1] * mUTuple[dimp1] + mTerm[d] * mDCoefficient[dimp1][mLTuple[dimp1]];
                        mTerm[d] = mCTZero;
                        mJTuple[d] = 0;
                        mKTuple[d] = mDegree[d];
                        mLTuple[d] = mLMax[d][order[d]];
                    }
                }
            }

            typename Controls::Type result = mTerm[static_cast<size_t>(N) - 1];
            T adjust = C_<T>(1);
            for (std::size_t d = 0; d < N; ++d)
            {
                adjust *= mPowerDSDT[d][order[d]];
            }
            result = result * adjust;
            return result;
        }

        // Constructor inputs.
        std::size_t N;
        std::vector<std::size_t> mDegree;  // degree[N]
        Controls const* mControls;
        typename Controls::Type const mCTZero;
        std::uint32_t const mCacheMode;

        // Parameters for B-spline evaluation. All outer std::vector
        // containers have N elements.
        std::size_t mNumLocalControls;  // product of (degree[]+1)
        std::vector<std::size_t> mDegreeP1;
        std::vector<std::size_t> mNumControls;
        std::vector<T> mTMin, mTMax;
        std::vector<std::vector<T>> mBlender;
        std::vector<std::vector<T>> mDCoefficient;
        std::vector<std::vector<std::size_t>> mLMax;
        std::vector<std::vector<T>> mPowerDSDT;
        std::vector<std::size_t> mITuple;
        std::vector<std::size_t> mJTuple;
        std::vector<std::size_t> mKTuple;
        std::vector<std::size_t> mLTuple;
        std::vector<std::size_t> mSumIJTuple;
        std::vector<T> mUTuple;
        std::vector<T> mPTuple;

        // Support for no-cached B-spline evaluation. The outer std::vector 
        // container has N elements.
        std::vector<std::vector<T>> mPhi;

        // Support for cached B-spline evaluation.
        std::vector<std::size_t> mTBound;  // tbound[2*N]
        std::vector<std::size_t> mComputeJTuple;  // computejtuple[N]
        std::vector<std::size_t> mComputeSumIJTuple;  // computesumijtuple[N]
        std::vector<std::size_t> mDegreeMinusOrder;  // degreeminusorder[N]
        std::vector<typename Controls::Type> mTerm;  // mTerm[N]
        std::vector<typename Controls::Type> mTensor;  // depends on numcontrols
        std::vector<bool> mCached;  // same size as mTensor

    private:
        friend class UnitTestBSplineUniform;
    };
}
