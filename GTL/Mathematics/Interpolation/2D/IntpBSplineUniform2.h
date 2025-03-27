// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.03.23

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
    class IntpBSplineUniform2 : public IntpBSplineUniformShared<T>
    {
    public:
        // The caller is responsible for ensuring that the 'controls' exist as
        // long as the IntpBSplineUniform2 exists.
        IntpBSplineUniform2(std::array<std::size_t, 2> const& degree, Controls const& controls,
            typename Controls::Type const& ctZero, std::uint32_t cacheMode)
            :
            mDegree(degree),
            mControls(&controls),
            mCTZero(ctZero),
            mCacheMode(cacheMode),
            mDegreeP1
            {
                mDegree[0] + 1,
                mDegree[1] + 1
            },
            mNumControls
            {
                mControls->GetSize(0),
                mControls->GetSize(1)
            },
            mTMin
            {
                static_cast<T>(-0.5),
                static_cast<T>(-0.5)
            },
            mTMax
            {
                static_cast<T>(mNumControls[0]) - static_cast<T>(0.5),
                static_cast<T>(mNumControls[1]) - static_cast<T>(0.5)
            },
            mBlender{},
            mDCoefficient{},
            mLMax{},
            mPowerDSDT{},
            mPhi{},
            mNumTRows{ 0, 0 },
            mNumTCols{ 0, 0 },
            mTensor{},
            mCached{}
        {
            // The condition c+1 > d+1 is required so that when s = c+1-d, its
            // maximum value, we have at least two s-knots (d and d + 1).
            for (std::size_t d = 0; d < 2; ++d)
            {
                GTL_ARGUMENT_ASSERT(
                    mControls->GetSize(d) > mDegreeP1[d],
                    "Incompatible degree or number of controls.");
            }

            GTL_ARGUMENT_ASSERT(
                mCacheMode < IntpBSplineUniformShared<T>::NUM_CACHING_MODES,
                "Invalid caching mode.");

            for (std::size_t d = 0; d < 2; ++d)
            {
                IntpBSplineUniformShared<T>::ComputeBlendingMatrix(mDegree[d], mBlender[d]);
                IntpBSplineUniformShared<T>::ComputeDCoefficients(mDegree[d], mDCoefficient[d], mLMax[d]);
                IntpBSplineUniformShared<T>::ComputePowers(mDegree[d], mNumControls[d], mTMin[d], mTMax[d], mPowerDSDT[d]);
            }

            if (mCacheMode == IntpBSplineUniformShared<T>::NO_CACHING)
            {
                for (std::size_t d = 0; d < 2; ++d)
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
        IntpBSplineUniform2(IntpBSplineUniform2 const&) = delete;
        IntpBSplineUniform2& operator=(IntpBSplineUniform2 const&) = delete;
        IntpBSplineUniform2(IntpBSplineUniform2&&) = delete;
        IntpBSplineUniform2& operator=(IntpBSplineUniform2&&) = delete;

        // Member access. The input d specifies the dimension (0, 1).
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

        // Evaluate the interpolator. The order is (0,0) when you want the
        // B-spline function value itself. The order0 is 1 for the first
        // derivative with respect to t0 and the order1 is 1 for the first
        // derivative with respect to t1. Higher-order derivatives in other
        // t-inputs are computed similarly.
        typename Controls::Type Evaluate(std::array<std::size_t, 2> const& order,
            std::array<T, 2> const& t)
        {
            typename Controls::Type result = mCTZero;
            if (0 <= order[0] && order[0] <= mDegree[0] &&
                0 <= order[1] && order[1] <= mDegree[1])
            {
                std::array<std::size_t, 2> i{ 0, 0 };
                std::array<T, 2> u{ C_<T>(0), C_<T>(0) };
                for (std::size_t d = 0; d < 2; ++d)
                {
                    IntpBSplineUniformShared<T>::GetKey(t[d], mTMin[d], mTMax[d],
                        mPowerDSDT[d][1], mNumControls[d], mDegree[d], i[d], u[d]);
                }

                if (mCacheMode == IntpBSplineUniformShared<T>::NO_CACHING)
                {
                    for (std::size_t d = 0; d < 2; ++d)
                    {
                        std::size_t jIndex = 0;
                        for (std::size_t j = 0; j <= mDegree[d]; ++j)
                        {
                            std::size_t kjIndex = mDegree[d] + jIndex;
                            std::size_t ell = mLMax[d][order[d]];
                            mPhi[d][j] = C_<T>(0);
                            for (std::size_t k = order[d]; k <= mDegree[d]; ++k)
                            {
                                mPhi[d][j] = mPhi[d][j] * u[d] +
                                    mBlender[d][kjIndex--] * mDCoefficient[d][ell--];
                            }
                            jIndex += mDegreeP1[d];
                        }
                    }

                    for (std::size_t j1 = 0; j1 <= mDegree[1]; ++j1)
                    {
                        T phi1 = mPhi[1][j1];
                        for (std::size_t j0 = 0; j0 <= mDegree[0]; ++j0)
                        {
                            T phi0 = mPhi[0][j0];
                            T phi01 = phi0 * phi1;
                            result = result + (*mControls)(i[0] + j0, i[1] + j1) * phi01;
                        }
                    }
                }
                else
                {
                    std::size_t i0i1Index = mNumTCols[1] * (i[0] + mNumTRows[0] * i[1]);
                    std::size_t k1i0i1Index = mDegree[1] + i0i1Index;
                    std::size_t ell1 = mLMax[1][order[1]];

                    for (std::size_t k1 = order[1], c1 = mDegree[1]; k1 <= mDegree[1]; ++k1, --c1)
                    {
                        std::size_t k0k1i0i1Index = mDegree[0] + mNumTCols[0] * k1i0i1Index;
                        std::size_t ell0 = mLMax[0][order[0]];
                        typename Controls::Type term = mCTZero;
                        for (std::size_t k0 = order[0], c0 = mDegree[0]; k0 <= mDegree[0]; ++k0, --c0)
                        {
                            if (mCacheMode == IntpBSplineUniformShared<T>::ON_DEMAND_CACHING &&
                                !mCached[k0k1i0i1Index])
                            {
                                ComputeTensor(i[0], i[1], c0, c1, k0k1i0i1Index);
                                mCached[k0k1i0i1Index] = true;
                            }
                            term = term * u[0] + mTensor[k0k1i0i1Index--] * mDCoefficient[0][ell0--];
                        }
                        result = result * u[1] + term * mDCoefficient[1][ell1--];
                        --k1i0i1Index;
                    }
                }

                T adjust = C_<T>(1);
                for (std::size_t d = 0; d < 2; ++d)
                {
                    adjust *= mPowerDSDT[d][order[d]];
                }
                result = result * adjust;
            }
            return result;
        }

    protected:
        void ComputeTensor(std::size_t r0, std::size_t r1, std::size_t c0, std::size_t c1, std::size_t index)
        {
            typename Controls::Type element = mCTZero;
            for (std::size_t j1 = 0; j1 <= mDegree[1]; ++j1)
            {
                T blend1 = mBlender[1][c1 + mDegreeP1[1] * j1];
                for (std::size_t j0 = 0; j0 <= mDegree[0]; ++j0)
                {
                    T blend0 = mBlender[0][c0 + mDegreeP1[0] * j0];
                    T blend01 = blend0 * blend1;
                    element = element + (*mControls)(r0 + j0, r1 + j1) * blend01;
                }
            }
            mTensor[index] = element;
        }

        void InitializeTensors()
        {
            std::size_t numCached = 1;
            for (std::size_t d = 0; d < 2; ++d)
            {
                mNumTRows[d] = mNumControls[d] - mDegree[d];
                mNumTCols[d] = mDegreeP1[d];
                numCached *= mNumTRows[d] * mNumTCols[d];
            }
            mTensor.resize(numCached);
            mCached.resize(numCached);
            if (mCacheMode == IntpBSplineUniformShared<T>::PRE_CACHING)
            {
                for (std::size_t r1 = 0, index = 0; r1 < mNumTRows[1]; ++r1)
                {
                    for (std::size_t r0 = 0; r0 < mNumTRows[0]; ++r0)
                    {
                        for (std::size_t c1 = 0; c1 < mNumTCols[1]; ++c1)
                        {
                            for (std::size_t c0 = 0; c0 < mNumTCols[0]; ++c0, ++index)
                            {
                                ComputeTensor(r0, r1, c0, c1, index);
                            }
                        }
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
        std::array<std::size_t, 2> mDegree;
        Controls const* mControls;
        typename Controls::Type mCTZero;
        std::uint32_t mCacheMode;

        // Parameters for B-spline evaluation.
        std::array<std::size_t, 2> mDegreeP1;
        std::array<std::size_t, 2> mNumControls;
        std::array<T, 2> mTMin, mTMax;
        std::array<std::vector<T>, 2> mBlender;
        std::array<std::vector<T>, 2> mDCoefficient;
        std::array<std::vector<std::size_t>, 2> mLMax;
        std::array<std::vector<T>, 2> mPowerDSDT;

        // Support for no-cached B-spline evaluation.
        std::array<std::vector<T>, 2> mPhi;

        // Support for cached B-spline evaluation.
        std::array<std::size_t, 2> mNumTRows, mNumTCols;
        std::vector<typename Controls::Type> mTensor;
        std::vector<bool> mCached;

    private:
        friend class UnitTestBSplineUniform2;
    };
}
