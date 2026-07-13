// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

#pragma once

#include <GTL/Mathematics/ImageProcessing/PDEFilter2.h>
#include <cstddef>
#include <cstdint>

namespace gtl
{
    template <typename T>
    class CurvatureFlow2 : public PDEFilter2<T>
    {
    public:
        CurvatureFlow2(std::size_t xBound, std::size_t yBound, T const& xSpacing,
            T const& ySpacing, T const* data, std::int32_t const* mask,
            T const& borderValue, typename PDEFilter<T>::ScaleType scaleType)
            :
            PDEFilter2<T>(xBound, yBound, xSpacing, ySpacing, data, mask,
                borderValue, scaleType)
        {
        }

        virtual ~CurvatureFlow2() = default;

    protected:
        virtual void OnUpdateSingle(std::size_t x, std::size_t y) override
        {
            this->LookUp9(x, y);

            T ux = this->mHalfInvDx * (this->mUpz - this->mUmz);
            T uy = this->mHalfInvDy * (this->mUzp - this->mUzm);
            T uxx = this->mInvDxDx * (this->mUpz - C_<T>(2) * this->mUzz + this->mUmz);
            T uxy = this->mFourthInvDxDy * (this->mUmm + this->mUpp - this->mUmp - this->mUpm);
            T uyy = this->mInvDyDy * (this->mUzp - C_<T>(2) * this->mUzz + this->mUzm);

            T sqrUx = ux * ux;
            T sqrUy = uy * uy;
            T denom = sqrUx + sqrUy;
            if (denom > C_<T>(0))
            {
                T numer = uxx * sqrUy + uyy * sqrUx - C_<T>(1, 2) * uxy * ux * uy;
                this->mBuffer[this->mDst](x, y) = this->mUzz + this->mTimeStep * numer / denom;
            }
            else
            {
                this->mBuffer[this->mDst](x, y) = this->mUzz;
            }
        }

    private:
        friend class UnitTestCurvatureFlow2;
    };
}
