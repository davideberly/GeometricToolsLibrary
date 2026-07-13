// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

#pragma once

#include <GTL/Mathematics/ImageProcessing/PDEFilter2.h>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace gtl
{
    template <typename T>
    class GradientAnisotropic2 : public PDEFilter2<T>
    {
    public:
        // If all pixels are to be processed, pass nullptr for 'mask'.
        // Otherwise, 'mask' is the same size as 'data' and has mask[i] != 0
        // for pixels that should be processed or mask[i] = 0 for pixels that
        // should not be processed.
        GradientAnisotropic2(std::size_t xBound, std::size_t yBound, T const& xSpacing,
            T const& ySpacing, T const* data, std::int32_t const* mask, T borderValue,
            typename PDEFilter<T>::ScaleType scaleType, T const& K)
            :
            PDEFilter2<T>(xBound, yBound, xSpacing, ySpacing, data, mask,
                borderValue, scaleType),
            mK(K),
            mParameter(C_<T>(0)),
            mMHalfParameter(C_<T>(0))
        {
            static_assert(std::is_floating_point<T>::value,
                "The template type must be 'float' or 'double'.");

            ComputeParameter();
        }

        virtual ~GradientAnisotropic2() = default;

    protected:
        void ComputeParameter()
        {
            T gradMagSqr = C_<T>(0);
            for (std::size_t y = 1; y <= this->mYBound; ++y)
            {
                for (std::size_t x = 1; x <= this->mXBound; ++x)
                {
                    T ux = this->GetUx(x, y);
                    T uy = this->GetUy(x, y);
                    gradMagSqr += ux * ux + uy * uy;
                }
            }
            gradMagSqr /= static_cast<T>(this->mQuantity);

            mParameter = C_<T>(1) / (mK * mK * gradMagSqr);
            mMHalfParameter = -C_<T>(1, 2) * mParameter;
        }

        virtual void OnPreUpdate() override
        {
            ComputeParameter();
        }

        virtual void OnUpdateSingle(std::size_t x, std::size_t y) override
        {
            this->LookUp9(x, y);

            // one-sided U-derivative estimates
            T uxFwd = this->mInvDx * (this->mUpz - this->mUzz);
            T uxBwd = this->mInvDx * (this->mUzz - this->mUmz);
            T uyFwd = this->mInvDy * (this->mUzp - this->mUzz);
            T uyBwd = this->mInvDy * (this->mUzz - this->mUzm);

            // centered U-derivative estimates
            T uxCenM = this->mHalfInvDx * (this->mUpm - this->mUmm);
            T uxCenZ = this->mHalfInvDx * (this->mUpz - this->mUmz);
            T uxCenP = this->mHalfInvDx * (this->mUpp - this->mUmp);
            T uyCenM = this->mHalfInvDy * (this->mUmp - this->mUmm);
            T uyCenZ = this->mHalfInvDy * (this->mUzp - this->mUzm);
            T uyCenP = this->mHalfInvDy * (this->mUpp - this->mUpm);

            T uxCenZSqr = uxCenZ * uxCenZ;
            T uyCenZSqr = uyCenZ * uyCenZ;
            T gradMagSqr{};

            // estimate for C(x+1,y)
            T uyEstP = C_<T>(1, 2) * (uyCenZ + uyCenP);
            gradMagSqr = uxCenZSqr + uyEstP * uyEstP;
            T cxp = std::exp(mMHalfParameter * gradMagSqr);

            // estimate for C(x-1,y)
            T uyEstM = C_<T>(1, 2) * (uyCenZ + uyCenM);
            gradMagSqr = uxCenZSqr + uyEstM * uyEstM;
            T cxm = std::exp(mMHalfParameter * gradMagSqr);

            // estimate for C(x,y+1)
            T uxEstP = C_<T>(1, 2) * (uxCenZ + uxCenP);
            gradMagSqr = uyCenZSqr + uxEstP * uxEstP;
            T cyp = std::exp(mMHalfParameter * gradMagSqr);

            // estimate for C(x,y-1)
            T uxEstM = C_<T>(1, 2) * (uxCenZ + uxCenM);
            gradMagSqr = uyCenZSqr + uxEstM * uxEstM;
            T cym = std::exp(mMHalfParameter * gradMagSqr);

            this->mBuffer[this->mDst](x, y) = this->mUzz + this->mTimeStep * (
                cxp * uxFwd - cxm * uxBwd +
                cyp * uyFwd - cym * uyBwd);
        }

        // These are updated on each iteration, since they depend on the
        // current average of the squared length of the gradients at the
        // pixels.
        T mK;                // k
        T mParameter;        // 1/(k^2*average(gradMagSqr))
        T mMHalfParameter;   // -0.5*mParameter

    private:
        friend class UnitTestGradientAnisotropic2;
    };
}
