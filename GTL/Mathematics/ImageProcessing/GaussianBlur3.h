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
    class GaussianBlur3 : public PDEFilter3<T>
    {
    public:
        // If all pixels are to be processed, pass nullptr for 'mask'.
        // Otherwise, 'mask' is the same size as 'data' and has mask[i] != 0
        // for pixels that should be processed or mask[i] = 0 for pixels that
        // should not be processed.
        GaussianBlur3(std::size_t xBound, std::size_t yBound, std::size_t zBound, T const& xSpacing,
            T const& ySpacing, T const& zSpacing, T const* data, std::int32_t const* mask,
            T const& borderValue, typename PDEFilter<T>::ScaleType scaleType)
            :
            PDEFilter3<T>(xBound, yBound, zBound, xSpacing, ySpacing, zSpacing,
                data, mask, borderValue, scaleType),
            mMaximumTimeStep(C_<T>(1, 2) / (this->mInvDxDx + this->mInvDyDy + this->mInvDzDz))
        {
        }

        virtual ~GaussianBlur3() = default;

        inline T const& GetMaximumTimeStep() const
        {
            return mMaximumTimeStep;
        }

    protected:
        virtual void OnUpdateSingle(std::size_t x, std::size_t y, std::size_t z) override
        {
            this->LookUp7(x, y, z);

            T uxx = this->mInvDxDx * (this->mUpzz - C_<T>(2) * this->mUzzz + this->mUmzz);
            T uyy = this->mInvDyDy * (this->mUzpz - C_<T>(2) * this->mUzzz + this->mUzmz);
            T uzz = this->mInvDzDz * (this->mUzzp - C_<T>(2) * this->mUzzz + this->mUzzm);

            this->mBuffer[this->mDst](x, y, z) = this->mUzzz + this->mTimeStep * (uxx + uyy + uzz);
        }

        T mMaximumTimeStep;

    private:
        friend class UnitTestGaussianBlur3;
    };
}
