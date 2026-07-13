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
    class GaussianBlur2 : public PDEFilter2<T>
    {
    public:
        // If all pixels are to be processed, pass nullptr for 'mask'.
        // Otherwise, 'mask' is the same size as 'data' and has mask[i] != 0
        // for pixels that should be processed or mask[i] = 0 for pixels that
        // should not be processed.
        GaussianBlur2(std::size_t xBound, std::size_t yBound, T const& xSpacing, T const& ySpacing,
            T const* data, std::int32_t const* mask, T borderValue,
            typename PDEFilter<T>::ScaleType scaleType)
            :
            PDEFilter2<T>(xBound, yBound, xSpacing, ySpacing, data, mask,
                borderValue, scaleType),
            mMaximumTimeStep(C_<T>(1, 2) / (this->mInvDxDx + this->mInvDyDy))
        {
        }

        virtual ~GaussianBlur2() = default;

        inline T const& GetMaximumTimeStep() const
        {
            return mMaximumTimeStep;
        }

    protected:
        virtual void OnUpdateSingle(std::size_t x, std::size_t y) override
        {
            this->LookUp5(x, y);

            T uxx = this->mInvDxDx * (this->mUpz - C_<T>(2) * this->mUzz + this->mUmz);
            T uyy = this->mInvDyDy * (this->mUzp - C_<T>(2) * this->mUzz + this->mUzm);

            this->mBuffer[this->mDst](x, y) = this->mUzz + this->mTimeStep * (uxx + uyy);
        }

        T mMaximumTimeStep;

    private:
        friend class UnitTestGaussianBlur2;
    };
}
