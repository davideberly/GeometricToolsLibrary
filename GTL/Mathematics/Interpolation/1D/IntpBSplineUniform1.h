// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.03.23

#pragma once

#include <GTL/Utility/Exceptions.h>
#include <GTL/Mathematics/Interpolation/ND/IntpBSplineUniformShared.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace gtl
{
    template <typename T, typename Controls>
    class IntpBSplineUniform1 : public IntpBSplineUniformShared<T>
    {
    public:
        // The caller is responsible for ensuring that the 'controls' exist as
        // long as the IntpBSplineUniform1 exists.
        IntpBSplineUniform1(std::size_t degree, Controls const& controls,
            typename Controls::Type const& ctZero, std::uint32_t cacheMode)
            :
            mDegree(degree),
            mControls(&controls),
            mCTZero(ctZero),
            mCacheMode(cacheMode),
            mDegreeP1(mDegree + 1),
            mNumControls(mControls->GetSize(0)),
            mTMin(static_cast<T>(-0.5)),
            mTMax(static_cast<T>(mNumControls) - static_cast<T>(0.5)),
            mBlender{},
            mDCoefficient{},
            mLMax{},
            mPowerDSDT{},
            mPhi{},
            mNumTRows(0),
            mNumTCols(0),
            mTensor{},
            mCached{}
        {
            // The condition c+1 > d+1 is required so that when s = c+1-d, its
            // maximum value, we have at least two s-knots (d and d + 1).
            GTL_ARGUMENT_ASSERT(
                mControls->GetSize(0) > mDegreeP1,
                "Incompatible degree or number of controls.");

            GTL_ARGUMENT_ASSERT(
                mCacheMode < IntpBSplineUniformShared<T>::NUM_CACHING_MODES,
                "Invalid caching mode.");

            IntpBSplineUniformShared<T>::ComputeBlendingMatrix(mDegree, mBlender);
            IntpBSplineUniformShared<T>::ComputeDCoefficients(mDegree, mDCoefficient, mLMax);
            IntpBSplineUniformShared<T>::ComputePowers(mDegree, mNumControls, mTMin, mTMax, mPowerDSDT);
            if (mCacheMode == IntpBSplineUniformShared<T>::NO_CACHING)
            {
                mPhi.resize(mDegreeP1);
            }
            else
            {
                InitializeTensors();
            }
        }

        // Disallow copying and moving.
        IntpBSplineUniform1(IntpBSplineUniform1 const&) = delete;
        IntpBSplineUniform1& operator=(IntpBSplineUniform1 const&) = delete;
        IntpBSplineUniform1(IntpBSplineUniform1&&) = delete;
        IntpBSplineUniform1& operator=(IntpBSplineUniform1&&) = delete;

        // Member access. The input is abstractly 0 (1-dimensional).
        inline std::size_t GetDegree() const
        {
            return mDegree;
        }

        inline std::size_t GetNumControls() const
        {
            return mNumControls;
        }

        inline T GetTMin() const
        {
            return mTMin;
        }

        inline T GetTMax() const
        {
            return mTMax;
        }

        inline std::uint32_t GetCacheMode() const
        {
            return mCacheMode;
        }

        // Evaluate the interpolator. The order is 0 when you want the B-spline
        // function value itself. The order is 1 for the first derivative of the
        // function, and so on.
        typename Controls::Type Evaluate(std::size_t order, T const& t)
        {
            typename Controls::Type result = mCTZero;
            if (0 <= order && order <= mDegree)
            {
                std::size_t i = 0;
                T u = C_<T>(0);
                IntpBSplineUniformShared<T>::GetKey(t, mTMin, mTMax,
                    mPowerDSDT[1], mNumControls, mDegree, i, u);

                if (mCacheMode == IntpBSplineUniformShared<T>::NO_CACHING)
                {
                    std::size_t jIndex = 0;
                    for (std::size_t j = 0; j <= mDegree; ++j)
                    {
                        std::size_t kjIndex = mDegree + jIndex;
                        std::size_t ell = mLMax[order];
                        mPhi[j] = C_<T>(0);
                        for (std::size_t k = order; k <= mDegree; ++k)
                        {
                            mPhi[j] = mPhi[j] * u + mBlender[kjIndex--] * mDCoefficient[ell--];
                        }
                        jIndex += mDegreeP1;
                    }

                    for (int32_t j = 0; j <= mDegree; ++j)
                    {
                        result = result + (*mControls)(i + j) * mPhi[j];
                    }
                }
                else
                {
                    std::size_t iIndex = mNumTCols * i;
                    std::size_t kiIndex = mDegree + iIndex;
                    std::size_t ell = mLMax[order];

                    for (std::size_t k = order, c = mDegree; k <= mDegree; ++k, --c)
                    {
                        if (mCacheMode == IntpBSplineUniformShared<T>::ON_DEMAND_CACHING &&
                            !mCached[kiIndex])
                        {
                            ComputeTensor(i, c, kiIndex);
                            mCached[kiIndex] = true;
                        }

                        result = result * u + mTensor[kiIndex--] * mDCoefficient[ell--];
                    }
                }

                result = result * mPowerDSDT[order];
            }
            return result;
        }

    protected:
        void ComputeTensor(std::size_t r, std::size_t c, std::size_t index)
        {
            typename Controls::Type element = mCTZero;
            for (std::size_t j = 0; j <= mDegree; ++j)
            {
                element = element + (*mControls)(r + j) * mBlender[c + mDegreeP1 * j];
            }
            mTensor[index] = element;
        }

        void InitializeTensors()
        {
            mNumTRows = mNumControls - mDegree;
            mNumTCols = mDegreeP1;
            std::size_t numCached = mNumTRows * mNumTCols;
            mTensor.resize(numCached);
            mCached.resize(numCached);
            if (mCacheMode == IntpBSplineUniformShared<T>::PRE_CACHING)
            {
                for (std::size_t r = 0, index = 0; r < mNumTRows; ++r)
                {
                    for (std::size_t c = 0; c < mNumTCols; ++c, ++index)
                    {
                        ComputeTensor(r, c, index);
                    }
                }
                std::fill(mCached.begin(), mCached.end(), true);
            }
            else
            {
                std::fill(mCached.begin(), mCached.end(), false);
            }
        }

        // Constructor inputs.
        std::size_t mDegree;
        Controls const* mControls;
        typename Controls::Type mCTZero;
        std::uint32_t mCacheMode;

        // Parameters for B-spline evaluation.
        std::size_t mDegreeP1;
        std::size_t mNumControls;
        T mTMin;
        T mTMax;
        std::vector<T> mBlender;
        std::vector<T> mDCoefficient;
        std::vector<std::size_t> mLMax;
        std::vector<T> mPowerDSDT;

        // Support for no-cached B-spline evaluation.
        std::vector<T> mPhi;

        // Support for cached B-spline evaluation.
        std::size_t mNumTRows;
        std::size_t mNumTCols;
        std::vector<typename Controls::Type> mTensor;
        std::vector<bool> mCached;

    private:
        friend class UnitTestBSplineUniform1;
    };
}
