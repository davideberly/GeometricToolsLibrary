// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

#pragma once

#include <GTL/Mathematics/ImageProcessing/PDEFilter3.h>
#include <cstddef>
#include <cstdint>

namespace gtl
{
    template <typename T>
    class CurvatureFlow3 : public PDEFilter3<T>
    {
    public:
        CurvatureFlow3(std::size_t xBound, std::size_t yBound, std::size_t zBound, T const& xSpacing,
            T const& ySpacing, T const& zSpacing, T const* data, std::int32_t const* mask,
            T const& borderValue, typename PDEFilter<T>::ScaleType scaleType)
            :
            PDEFilter3<T>(xBound, yBound, zBound, xSpacing, ySpacing,
                zSpacing, data, mask, borderValue, scaleType)
        {
        }

        virtual ~CurvatureFlow3() = default;

    protected:
        virtual void OnUpdateSingle(std::size_t x, std::size_t y, std::size_t z) override
        {
            this->LookUp27(x, y, z);

            T ux = this->mHalfInvDx * (this->mUpzz - this->mUmzz);
            T uy = this->mHalfInvDy * (this->mUzpz - this->mUzmz);
            T uz = this->mHalfInvDz * (this->mUzzp - this->mUzzm);
            T uxx = this->mInvDxDx * (this->mUpzz - C_<T>(2) * this->mUzzz + this->mUmzz);
            T uxy = this->mFourthInvDxDy * (this->mUmmz + this->mUppz - this->mUpmz - this->mUmpz);
            T uxz = this->mFourthInvDxDz * (this->mUmzm + this->mUpzp - this->mUpzm - this->mUmzp);
            T uyy = this->mInvDyDy * (this->mUzpz - C_<T>(2) * this->mUzzz + this->mUzmz);
            T uyz = this->mFourthInvDyDz * (this->mUzmm + this->mUzpp - this->mUzpm - this->mUzmp);
            T uzz = this->mInvDzDz * (this->mUzzp - C_<T>(2) * this->mUzzz + this->mUzzm);

            T denom = ux * ux + uy * uy + uz * uz;
            if (denom > C_<T>(0))
            {
                T numer0 = uy * (uxx * uy - uxy * ux) + ux * (uyy * ux - uxy * uy);
                T numer1 = uz * (uxx * uz - uxz * ux) + ux * (uzz * ux - uxz * uz);
                T numer2 = uz * (uyy * uz - uyz * uy) + uy * (uzz * uy - uyz * uz);
                T numer = numer0 + numer1 + numer2;
                this->mBuffer[this->mDst](x, y, z) = this->mUzzz + this->mTimeStep * numer / denom;
            }
            else
            {
                this->mBuffer[this->mDst](x, y, z) = this->mUzzz;
            }
        }

    private:
        friend class UnitTestCurvatureFlow3;
    };
}
