// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

#pragma once

// Support for 1D image processing using finite differences for partial
// differential equations. PDEFilter1 is an abstract base class.

#include <GTL/Mathematics/ImageProcessing/PdeFilter.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T>
    class PDEFilter1 : public PDEFilter<T>
    {
    public:
        // Abstract base class. If all pixels are to be processed, pass
        // nullptr for 'mask'. Otherwise, 'mask' is the same size as 'data'
        // and has mask[i] != 0 for pixels that should be processed or
        // mask[i] = 0 for pixels that should not be processed.
        PDEFilter1(std::size_t xBound, T const& xSpacing, T const* data, std::int32_t const* mask,
            T const& borderValue, typename PDEFilter<T>::ScaleType scaleType)
            :
            PDEFilter<T>(xBound, data, borderValue, scaleType),
            mXBound(xBound),
            mXSpacing(xSpacing),
            mInvDx(C_<T>(1) / xSpacing),
            mHalfInvDx(C_<T>(1, 2) * mInvDx),
            mInvDxDx(mInvDx * mInvDx),
            mUm(C_<T>(0)),
            mUz(C_<T>(0)),
            mUp(C_<T>(0)),
            mBuffer{},
            mSrc(0),
            mDst(1),
            mMask(xBound + 2),
            mHasMask(mask != nullptr)
        {
            // The mBuffer[] are ping-pong buffers for filtering.
            for (std::size_t i = 0; i < 2; ++i)
            {
                mBuffer[i].resize(xBound + 2);
            }

            for (std::size_t x = 0, xp = 1, i = 0; x < mXBound; ++x, ++xp, ++i)
            {
                mBuffer[mSrc][xp] =
                    this->mOffset + (data[i] - this->mMin) * this->mScale;
                mBuffer[mDst][xp] = C_<T>(0);
                mMask[xp] = (mHasMask ? mask[i] : 1);
            }

            // Assign values to the 1-pixel-thick border. TODO: Modify to
            // support rational arithmetic (cannot use std::numeric_limits).
            if (this->mBorderValue != std::numeric_limits<T>::max())
            {
                AssignDirichletImageBorder();
            }
            else
            {
                AssignNeumannImageBorder();
            }

            // To handle masks that do not cover the entire image, assign
            // values to those pixels that are 2-neighbors of the mask pixels.
            if (mHasMask)
            {
                if (this->mBorderValue != std::numeric_limits<T>::max())
                {
                    AssignDirichletMaskBorder();
                }
                else
                {
                    AssignNeumannMaskBorder();
                }
            }
        }

        virtual ~PDEFilter1() = default;

        // Member access. The internal 1D images for "data" and "mask" are
        // copies of the inputs to the constructor but padded with a 1-pixel
        // thick border to support filtering on the image boundary. These
        // images are of size (xbound+2). The correct lookups into the padded
        // arrays are handled internally.
        inline std::size_t GetXBound() const
        {
            return mXBound;
        }

        inline T const& GetXSpacing() const
        {
            return mXSpacing;
        }

        // Pixel access and derivative estimation. The lookups into the
        // padded data are handled correctly. The estimation involves only
        // the 3-tuple neighborhood of (x), where 0 <= x < xbound. TODO: If
        // larger neighborhoods are desired at a later date, the padding and
        // associated code must be adjusted accordingly.
        T GetU(std::size_t x) const
        {
            auto const& F = mBuffer[mSrc];
            return F[x + 1];
        }

        T GetUx(std::size_t x) const
        {
            auto const& F = mBuffer[mSrc];
            return mHalfInvDx * (F[x + 2] - F[x]);
        }

        T GetUxx(std::size_t x) const
        {
            auto const& F = mBuffer[mSrc];
            return mInvDxDx * (F[x + 2] - C_<T>(2) * F[x + 1] + F[x]);
        }

        std::int32_t GetMask(std::size_t x) const
        {
            return mMask[x + 1];
        }

    protected:
        // Assign values to the 1-pixel image border.
        void AssignDirichletImageBorder()
        {
            std::size_t xBp1 = mXBound + 1;

            // vertex (0,0)
            mBuffer[mSrc][0] = this->mBorderValue;
            mBuffer[mDst][0] = this->mBorderValue;
            if (mHasMask)
            {
                mMask[0] = 0;
            }

            // vertex (xmax,0)
            mBuffer[mSrc][xBp1] = this->mBorderValue;
            mBuffer[mDst][xBp1] = this->mBorderValue;
            if (mHasMask)
            {
                mMask[xBp1] = 0;
            }
        }

        void AssignNeumannImageBorder()
        {
            std::size_t xBp1 = mXBound + 1;
            T duplicate{};

            // vertex (0,0)
            duplicate = mBuffer[mSrc][1];
            mBuffer[mSrc][0] = duplicate;
            mBuffer[mDst][0] = duplicate;
            if (mHasMask)
            {
                mMask[0] = 0;
            }

            // vertex (xmax,0)
            duplicate = mBuffer[mSrc][mXBound];
            mBuffer[mSrc][xBp1] = duplicate;
            mBuffer[mDst][xBp1] = duplicate;
            if (mHasMask)
            {
                mMask[xBp1] = 0;
            }
        }

        // Assign values to the 1-pixel mask border.
        void AssignDirichletMaskBorder()
        {
            for (std::size_t x = 1; x <= mXBound; ++x)
            {
                if (mMask[x])
                {
                    continue;
                }

                for (std::size_t i0 = 0, j0 = x - 1; i0 < 3; ++i0, ++j0)
                {
                    if (mMask[j0])
                    {
                        mBuffer[mSrc][x] = this->mBorderValue;
                        mBuffer[mDst][x] = this->mBorderValue;
                        break;
                    }
                }
            }
        }

        void AssignNeumannMaskBorder()
        {
            // Recompute the values just outside the masked region. This
            // guarantees that derivative estimations use the current values
            // around the boundary.
            for (std::size_t x = 1; x <= mXBound; ++x)
            {
                if (mMask[x])
                {
                    continue;
                }

                std::size_t count = 0;
                T average = C_<T>(0);
                for (std::size_t i0 = 0, j0 = x - 1; i0 < 3; ++i0, ++j0)
                {
                    if (mMask[j0])
                    {
                        average += mBuffer[mSrc][j0];
                        ++count;
                    }
                }

                if (count > 0)
                {
                    average /= static_cast<T>(count);
                    mBuffer[mSrc][x] = average;
                    mBuffer[mDst][x] = average;
                }
            }
        }

        // This function recomputes the boundary values when Neumann
        // conditions are used. If a derived class overrides this, it must
        // call the base-class OnPreUpdate first.
        virtual void OnPreUpdate() override
        {
            if (mHasMask && this->mBorderValue == std::numeric_limits<T>::max())
            {
                // Neumann boundary conditions are in use, so recompute the
                // mask border.
                AssignNeumannMaskBorder();
            }
            // else: No mask has been specified or Dirichlet boundary
            // conditions are in use. Nothing to do.
        }

        // Iterate over all the pixels and call OnUpdate(x) for each pixel
        // that is not masked out.
        virtual void OnUpdate() override
        {
            for (std::size_t x = 1; x <= mXBound; ++x)
            {
                if (!mHasMask || mMask[x])
                {
                    OnUpdate(x);
                }
            }
        }

        // If a derived class overrides this, it must call the base-class
        // OnPostUpdate last. The base-class function swaps the buffers for
        // the next pass.
        virtual void OnPostUpdate() override
        {
            std::swap(mSrc, mDst);
        }

        // The per-pixel processing depends on the PDE algorithm. Input x must
        // be in padded coordinates: 1 <= x <= xbound.
        virtual void OnUpdate(std::size_t x) = 0;

        // Copy source data to temporary storage.
        void LookUp3(std::size_t x)
        {
            auto const& F = mBuffer[mSrc];
            mUm = F[x - 1];
            mUz = F[x];
            mUp = F[x + 1];
        }

        // Image parameters.
        std::size_t mXBound;
        T mXSpacing;       // dx
        T mInvDx;          // 1/dx
        T mHalfInvDx;      // 1/(2*dx)
        T mInvDxDx;        // 1/(dx*dx)

        // Temporary storage for 3-tuple neighborhood.  In the notation mUx,
        // the x index is in {m,z,p}, referring to subtract 1 (m), no change
        // (z), or add 1 (p) to the appropriate index.
        T mUm, mUz, mUp;

        // Successive iterations toggle between two buffers.
        std::array<std::vector<T>, 2> mBuffer;
        std::size_t mSrc, mDst;
        std::vector<std::int32_t> mMask;
        bool mHasMask;

    private:
        friend class UnitTestPDEFilter1;
    };
}
