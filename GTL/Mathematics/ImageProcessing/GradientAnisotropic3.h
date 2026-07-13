// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

#pragma once

#include <GTL/Mathematics/ImageProcessing/PDEFilter3.h>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace gtl
{
    template <typename T>
    class GradientAnisotropic3 : public PDEFilter3<T>
    {
    public:
        // If all pixels are to be processed, pass nullptr for 'mask'.
        // Otherwise, 'mask' is the same size as 'data' and has mask[i] != 0
        // for pixels that should be processed or mask[i] = 0 for pixels that
        // should not be processed.
        GradientAnisotropic3(std::size_t xBound, std::size_t yBound, std::size_t zBound, T const& xSpacing,
            T const& ySpacing, T const& zSpacing, T const* data, std::int32_t const* mask,
            T const& borderValue, typename PDEFilter<T>::ScaleType scaleType, T const& K)
            :
            PDEFilter3<T>(xBound, yBound, zBound, xSpacing, ySpacing, zSpacing,
                data, mask, borderValue, scaleType),
            mK(K),
            mParameter(C_<T>(0)),
            mMHalfParameter(C_<T>(0))
        {
            static_assert(std::is_floating_point<T>::value,
                "The template type must be 'float' or 'double'.");

            ComputeParameter();
        }

        virtual ~GradientAnisotropic3() = default;

    protected:
        void ComputeParameter()
        {
            T gradMagSqr = C_<T>(0);
            for (std::size_t z = 1; z <= this->mZBound; ++z)
            {
                for (std::size_t y = 1; y <= this->mYBound; ++y)
                {
                    for (std::size_t x = 1; x <= this->mXBound; ++x)
                    {
                        T ux = this->GetUx(x, y, z);
                        T uy = this->GetUy(x, y, z);
                        T uz = this->GetUz(x, y, z);
                        gradMagSqr += ux * ux + uy * uy + uz * uz;
                    }
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

        virtual void OnUpdateSingle(std::size_t x, std::size_t y, std::size_t z) override
        {
            this->LookUp27(x, y, z);

            // one-sided U-derivative estimates
            T uxFwd = this->mInvDx * (this->mUpzz - this->mUzzz);
            T uxBwd = this->mInvDx * (this->mUzzz - this->mUmzz);
            T uyFwd = this->mInvDy * (this->mUzpz - this->mUzzz);
            T uyBwd = this->mInvDy * (this->mUzzz - this->mUzmz);
            T uzFwd = this->mInvDz * (this->mUzzp - this->mUzzz);
            T uzBwd = this->mInvDz * (this->mUzzz - this->mUzzm);

            // centered U-derivative estimates
            T duvzz = this->mHalfInvDx * (this->mUpzz - this->mUmzz);
            T duvpz = this->mHalfInvDx * (this->mUppz - this->mUmpz);
            T duvmz = this->mHalfInvDx * (this->mUpmz - this->mUmmz);
            T duvzp = this->mHalfInvDx * (this->mUpzp - this->mUmzp);
            T duvzm = this->mHalfInvDx * (this->mUpzm - this->mUmzm);

            T duzvz = this->mHalfInvDy * (this->mUzpz - this->mUzmz);
            T dupvz = this->mHalfInvDy * (this->mUppz - this->mUpmz);
            T dumvz = this->mHalfInvDy * (this->mUmpz - this->mUmmz);
            T duzvp = this->mHalfInvDy * (this->mUzpp - this->mUzmp);
            T duzvm = this->mHalfInvDy * (this->mUzpm - this->mUzmm);

            T duzzv = this->mHalfInvDz * (this->mUzzp - this->mUzzm);
            T dupzv = this->mHalfInvDz * (this->mUpzp - this->mUpzm);
            T dumzv = this->mHalfInvDz * (this->mUmzp - this->mUmzm);
            T duzpv = this->mHalfInvDz * (this->mUzpp - this->mUzpm);
            T duzmv = this->mHalfInvDz * (this->mUzmp - this->mUzmm);

            T uxCenSqr = duvzz * duvzz;
            T uyCenSqr = duzvz * duzvz;
            T uzCenSqr = duzzv * duzzv;
            T uxEst{}, uyEst{}, uzEst{}, gradMagSqr{};

            // estimate for C(x+1,y,z)
            uyEst = C_<T>(1, 2) * (duzvz + dupvz);
            uzEst = C_<T>(1, 2) * (duzzv + dupzv);
            gradMagSqr = uxCenSqr + uyEst * uyEst + uzEst * uzEst;
            T cxp = std::exp(mMHalfParameter * gradMagSqr);

            // estimate for C(x-1,y,z)
            uyEst = C_<T>(1, 2) * (duzvz + dumvz);
            uzEst = C_<T>(1, 2) * (duzzv + dumzv);
            gradMagSqr = uxCenSqr + uyEst * uyEst + uzEst * uzEst;
            T cxm = std::exp(mMHalfParameter * gradMagSqr);

            // estimate for C(x,y+1,z)
            uxEst = C_<T>(1, 2) * (duvzz + duvpz);
            uzEst = C_<T>(1, 2) * (duzzv + duzpv);
            gradMagSqr = uxEst * uxEst + uyCenSqr + uzEst * uzEst;
            T cyp = std::exp(mMHalfParameter * gradMagSqr);

            // estimate for C(x,y-1,z)
            uxEst = C_<T>(1, 2) * (duvzz + duvmz);
            uzEst = C_<T>(1, 2) * (duzzv + duzmv);
            gradMagSqr = uxEst * uxEst + uyCenSqr + uzEst * uzEst;
            T cym = std::exp(mMHalfParameter * gradMagSqr);

            // estimate for C(x,y,z+1)
            uxEst = C_<T>(1, 2) * (duvzz + duvzp);
            uyEst = C_<T>(1, 2) * (duzvz + duzvp);
            gradMagSqr = uxEst * uxEst + uyEst * uyEst + uzCenSqr;
            T czp = std::exp(mMHalfParameter * gradMagSqr);

            // estimate for C(x,y,z-1)
            uxEst = C_<T>(1, 2) * (duvzz + duvzm);
            uyEst = C_<T>(1, 2) * (duzvz + duzvm);
            gradMagSqr = uxEst * uxEst + uyEst * uyEst + uzCenSqr;
            T czm = std::exp(mMHalfParameter * gradMagSqr);

            this->mBuffer[this->mDst](x, y, z) = this->mUzzz + this->mTimeStep * (
                cxp * uxFwd - cxm * uxBwd +
                cyp * uyFwd - cym * uyBwd +
                czp * uzFwd - czm * uzBwd);
        }

        // These are updated on each iteration, since they depend on the
        // current average of the squared length of the gradients at the
        // voxels.
        T mK;               // k
        T mParameter;       // 1/(k^2*average(gradMagSqr))
        T mMHalfParameter;  // -0.5*mParameter

    private:
        friend class UnitTestGradientAnisotropic3;
    };
}