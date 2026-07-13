// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

#pragma once

// Support for 2D image processing using finite differences for partial
// differential equations. PDEFilter2 is an abstract base class.

#include <GTL/Mathematics/ImageProcessing/PDEFilter.h>
#include <GTL/Utility/Multiarray.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>

namespace gtl
{
    template <typename T>
    class PDEFilter2 : public PDEFilter<T>
    {
    public:
        // Abstract base class. If all pixels are to be processed, pass
        // nullptr for 'mask'. Otherwise, 'mask' is the same size as 'data'
        // and has mask[i] != 0 for pixels that should be processed or
        // mask[i] = 0 for pixels that should not be processed.
        PDEFilter2(std::size_t xBound, std::size_t yBound, T const& xSpacing, T const& ySpacing,
            T const* data, std::int32_t const* mask, T const& borderValue,
            typename PDEFilter<T>::ScaleType scaleType)
            :
            PDEFilter<T>(xBound * yBound, data, borderValue, scaleType),
            mXBound(xBound),
            mYBound(yBound),
            mXSpacing(xSpacing),
            mYSpacing(ySpacing),
            mInvDx(C_<T>(1) / xSpacing),
            mInvDy(C_<T>(1) / ySpacing),
            mHalfInvDx(C_<T>(1, 2) * mInvDx),
            mHalfInvDy(C_<T>(1, 2) * mInvDy),
            mInvDxDx(mInvDx * mInvDx),
            mFourthInvDxDy(mHalfInvDx * mHalfInvDy),
            mInvDyDy(mInvDy * mInvDy),
            mUmm(C_<T>(0)), mUzm(C_<T>(0)), mUpm(C_<T>(0)),
            mUmz(C_<T>(0)), mUzz(C_<T>(0)), mUpz(C_<T>(0)),
            mUmp(C_<T>(0)), mUzp(C_<T>(0)), mUpp(C_<T>(0)),
            mSrc(0),
            mDst(1),
            mMask{ xBound + 2, yBound + 2 },
            mHasMask(mask != nullptr)
        {
            for (std::size_t i = 0; i < 2; ++i)
            {
                mBuffer[i].resize({ xBound + 2, yBound + 2 });
            }
                    
            // The mBuffer[] are ping-pong buffers for filtering.
            for (std::size_t y = 0, yp = 1, i = 0; y < mYBound; ++y, ++yp)
            {
                for (std::size_t x = 0, xp = 1; x < mXBound; ++x, ++xp, ++i)
                {
                    mBuffer[mSrc](xp, yp) =
                        this->mOffset + (data[i] - this->mMin) * this->mScale;
                    mBuffer[mDst](xp, yp) = C_<T>(0);
                    mMask(xp, yp) = (mHasMask ? mask[i] : 1);
                }
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
            // values to those pixels that are 8-neighbors of the mask pixels.
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

        virtual ~PDEFilter2() = default;

        // Member access. The internal 2D images for "data" and "mask" are
        // copies of the inputs to the constructor but padded with a 1-pixel
        // thick border to support filtering on the image boundary. These
        // images are of size (xbound+2)-by-(ybound+2). The correct lookups
        // into the padded arrays are handled internally.
        inline std::size_t GetXBound() const
        {
            return mXBound;
        }

        inline std::size_t GetYBound() const
        {
            return mYBound;
        }

        inline T const& GetXSpacing() const
        {
            return mXSpacing;
        }

        inline T const& GetYSpacing() const
        {
            return mYSpacing;
        }

        // Pixel access and derivative estimation. The lookups into the
        // padded data are handled correctly. The estimation involves only
        // the 3-by-3 neighborhood of (x,y), where 0 <= x < xbound and
        // 0 <= y < ybound. TODO: If larger neighborhoods are desired at a
        // later date, the padding and associated code must be adjusted
        // accordingly.
        T GetU(std::size_t x, std::size_t y) const
        {
            auto const& F = mBuffer[mSrc];
            std::size_t xp1 = x + 1, yp1 = y + 1;
            return F(xp1, yp1);
        }

        T GetUx(std::size_t x, std::size_t y) const
        {
            auto const& F = mBuffer[mSrc];
            std::size_t xp2 = x + 2, yp1 = y + 1;
            return mHalfInvDx * (F(xp2, yp1) - F(x, yp1));
        }

        T GetUy(std::size_t x, std::size_t y) const
        {
            auto const& F = mBuffer[mSrc];
            std::size_t xp1 = x + 1, yp2 = y + 2;
            return mHalfInvDy * (F(xp1, yp2) - F(xp1, y));
        }

        T GetUxx(std::size_t x, std::size_t y) const
        {
            auto const& F = mBuffer[mSrc];
            std::size_t xp1 = x + 1, xp2 = x + 2, yp1 = y + 1;
            return mInvDxDx * (F(xp2, yp1) - C_<T>(2) * F(xp1, yp1) + F(x, yp1));
        }

        T GetUxy(std::size_t x, std::size_t y) const
        {
            auto const& F = mBuffer[mSrc];
            std::size_t xp2 = x + 2, yp2 = y + 2;
            return mFourthInvDxDy * (F(x, y) - F(xp2, y) + F(xp2, yp2) - F(x, yp2));
        }

        T GetUyy(std::size_t x, std::size_t y) const
        {
            auto const& F = mBuffer[mSrc];
            std::size_t xp1 = x + 1, yp1 = y + 1, yp2 = y + 2;
            return mInvDyDy * (F(xp1, yp2) - C_<T>(2) * F(xp1, yp1) + F(xp1, y));
        }

        std::int32_t GetMask(std::size_t x, std::size_t y) const
        {
            std::size_t xp1 = x + 1, yp1 = y + 1;
            return mMask(xp1, yp1);
        }

    protected:
        // Assign values to the 1-pixel image border.
        void AssignDirichletImageBorder()
        {
            std::size_t xBp1 = mXBound + 1, yBp1 = mYBound + 1;
            std::size_t x{}, y{};

            // vertex (0,0)
            mBuffer[mSrc](0, 0) = this->mBorderValue;
            mBuffer[mDst](0, 0) = this->mBorderValue;
            if (mHasMask)
            {
                mMask(0, 0) = 0;
            }

            // vertex (xmax,0)
            mBuffer[mSrc](xBp1, 0) = this->mBorderValue;
            mBuffer[mDst](xBp1, 0) = this->mBorderValue;
            if (mHasMask)
            {
                mMask(xBp1, 0) = 0;
            }

            // vertex (0,ymax)
            mBuffer[mSrc](0, yBp1) = this->mBorderValue;
            mBuffer[mDst](0, yBp1) = this->mBorderValue;
            if (mHasMask)
            {
                mMask(0, yBp1) = 0;
            }

            // vertex (xmax,ymax)
            mBuffer[mSrc](xBp1, yBp1) = this->mBorderValue;
            mBuffer[mDst](xBp1, yBp1) = this->mBorderValue;
            if (mHasMask)
            {
                mMask(xBp1, yBp1) = 0;
            }

            // edges (x,0) and (x,ymax)
            for (x = 1; x <= mXBound; ++x)
            {
                mBuffer[mSrc](x, 0) = this->mBorderValue;
                mBuffer[mDst](x, 0) = this->mBorderValue;
                if (mHasMask)
                {
                    mMask(x, 0) = 0;
                }

                mBuffer[mSrc](x, yBp1) = this->mBorderValue;
                mBuffer[mDst](x, yBp1) = this->mBorderValue;
                if (mHasMask)
                {
                    mMask(x, yBp1) = 0;
                }
            }

            // edges (0,y) and (xmax,y)
            for (y = 1; y <= mYBound; ++y)
            {
                mBuffer[mSrc](0, y) = this->mBorderValue;
                mBuffer[mDst](0, y) = this->mBorderValue;
                if (mHasMask)
                {
                    mMask(0, y) = 0;
                }

                mBuffer[mSrc](xBp1, y) = this->mBorderValue;
                mBuffer[mDst](xBp1, y) = this->mBorderValue;
                if (mHasMask)
                {
                    mMask(xBp1, y) = 0;
                }
            }
        }

        void AssignNeumannImageBorder()
        {
            std::size_t xBp1 = mXBound + 1, yBp1 = mYBound + 1;
            std::size_t x{}, y{};
            T duplicate{};

            // vertex (0,0)
            duplicate = mBuffer[mSrc](1, 1);
            mBuffer[mSrc](0, 0) = duplicate;
            mBuffer[mDst](0, 0) = duplicate;
            if (mHasMask)
            {
                mMask(0, 0) = 0;
            }

            // vertex (xmax,0)
            duplicate = mBuffer[mSrc](mXBound, 1);
            mBuffer[mSrc](xBp1, 0) = duplicate;
            mBuffer[mDst](xBp1, 0) = duplicate;
            if (mHasMask)
            {
                mMask(xBp1, 0) = 0;
            }

            // vertex (0,ymax)
            duplicate = mBuffer[mSrc](1, mYBound);
            mBuffer[mSrc](0, yBp1) = duplicate;
            mBuffer[mDst](0, yBp1) = duplicate;
            if (mHasMask)
            {
                mMask(0, yBp1) = 0;
            }

            // vertex (xmax,ymax)
            duplicate = mBuffer[mSrc](mXBound, mYBound);
            mBuffer[mSrc](xBp1, yBp1) = duplicate;
            mBuffer[mDst](xBp1, yBp1) = duplicate;
            if (mHasMask)
            {
                mMask(xBp1, yBp1) = 0;
            }

            // edges (x,0) and (x,ymax)
            for (x = 1; x <= mXBound; ++x)
            {
                duplicate = mBuffer[mSrc](x, 1);
                mBuffer[mSrc](x, 0) = duplicate;
                mBuffer[mDst](x, 0) = duplicate;
                if (mHasMask)
                {
                    mMask(x, 0) = 0;
                }

                duplicate = mBuffer[mSrc](x, mYBound);
                mBuffer[mSrc](x, yBp1) = duplicate;
                mBuffer[mDst](x, yBp1) = duplicate;
                if (mHasMask)
                {
                    mMask(x, yBp1) = 0;
                }
            }

            // edges (0,y) and (xmax,y)
            for (y = 1; y <= mYBound; ++y)
            {
                duplicate = mBuffer[mSrc](1, y);
                mBuffer[mSrc](0, y) = duplicate;
                mBuffer[mDst](0, y) = duplicate;
                if (mHasMask)
                {
                    mMask(0, y) = 0;
                }

                duplicate = mBuffer[mSrc](mXBound, y);
                mBuffer[mSrc](xBp1, y) = duplicate;
                mBuffer[mDst](xBp1, y) = duplicate;
                if (mHasMask)
                {
                    mMask(xBp1, y) = 0;
                }
            }
        }

        // Assign values to the 1-pixel mask border.
        void AssignDirichletMaskBorder()
        {
            for (std::size_t y = 1; y <= mYBound; ++y)
            {
                for (std::size_t x = 1; x <= mXBound; ++x)
                {
                    if (mMask(x, y))
                    {
                        continue;
                    }

                    bool found = false;
                    for (std::size_t i1 = 0, j1 = y - 1; i1 < 3 && !found; ++i1, ++j1)
                    {
                        for (std::size_t i0 = 0, j0 = x - 1; i0 < 3; ++i0, ++j0)
                        {
                            if (mMask(j0, j1))
                            {
                                mBuffer[mSrc](x, y) = this->mBorderValue;
                                mBuffer[mDst](x, y) = this->mBorderValue;
                                found = true;
                                break;
                            }
                        }
                    }
                }
            }
        }

        void AssignNeumannMaskBorder()
        {
            // Recompute the values just outside the masked region. This
            // guarantees that derivative estimations use the current values
            // around the boundary.
            for (std::size_t y = 1; y <= mYBound; ++y)
            {
                for (std::size_t x = 1; x <= mXBound; ++x)
                {
                    if (mMask(x, y))
                    {
                        continue;
                    }

                    std::size_t count = 0;
                    T average = C_<T>(0);
                    for (std::size_t i1 = 0, j1 = y - 1; i1 < 3; ++i1, ++j1)
                    {
                        for (std::size_t i0 = 0, j0 = x - 1; i0 < 3; ++i0, ++j0)
                        {
                            if (mMask(j0, j1))
                            {
                                average += mBuffer[mSrc](j0, j1);
                                ++count;
                            }
                        }
                    }

                    if (count > 0)
                    {
                        average /= static_cast<T>(count);
                        mBuffer[mSrc](x, y) = average;
                        mBuffer[mDst](x, y) = average;
                    }
                }
            }
        }

        // This function recomputes the boundary values when Neumann
        // conditions are used.  If a derived class overrides this, it must
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

        // Iterate over all the pixels and call OnUpdate(x,y) for each pixel
        // that is not masked out.
        virtual void OnUpdate() override
        {
            for (std::size_t y = 1; y <= mYBound; ++y)
            {
                for (std::size_t x = 1; x <= mXBound; ++x)
                {
                    if (!mHasMask || mMask(x, y))
                    {
                        OnUpdateSingle(x, y);
                    }
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

        // The per-pixel processing depends on the PDE algorithm. The input
        // (x,y) must be in padded coordinates: 1 <= x <= xbound and
        // 1 <= y <= ybound.
        virtual void OnUpdateSingle(std::size_t x, std::size_t y) = 0;

        // Copy source data to temporary storage.
        void LookUp5(std::size_t x, std::size_t y)
        {
            auto const& F = mBuffer[mSrc];
            std::size_t xm = x - 1, xp = x + 1;
            std::size_t ym = y - 1, yp = y + 1;
            mUzm = F(x, ym);
            mUmz = F(xm, y);
            mUzz = F(x, y);
            mUpz = F(xp, y);
            mUzp = F(x, yp);
        }

        void LookUp9(std::size_t x, std::size_t y)
        {
            auto const& F = mBuffer[mSrc];
            std::size_t xm = x - 1, xp = x + 1;
            std::size_t ym = y - 1, yp = y + 1;
            mUmm = F(xm, ym);
            mUzm = F(x, ym);
            mUpm = F(xp, ym);
            mUmz = F(xm, y);
            mUzz = F(x, y);
            mUpz = F(xp, y);
            mUmp = F(xm, yp);
            mUzp = F(x, yp);
            mUpp = F(xp, yp);
        }

        // Image parameters.
        std::size_t mXBound, mYBound;
        T mXSpacing;       // dx
        T mYSpacing;       // dy
        T mInvDx;          // 1/dx
        T mInvDy;          // 1/dy
        T mHalfInvDx;      // 1/(2*dx)
        T mHalfInvDy;      // 1/(2*dy)
        T mInvDxDx;        // 1/(dx*dx)
        T mFourthInvDxDy;  // 1/(4*dx*dy)
        T mInvDyDy;        // 1/(dy*dy)

        // Temporary storage for 3x3 neighborhood.  In the notation mUxy, the
        // x and y indices are in {m,z,p}, referring to subtract 1 (m), no
        // change (z), or add 1 (p) to the appropriate index.
        T mUmm, mUzm, mUpm;
        T mUmz, mUzz, mUpz;
        T mUmp, mUzp, mUpp;

        // Successive iterations toggle between two buffers.
        std::array<Multiarray<T, true>, 2> mBuffer;
        std::size_t mSrc, mDst;
        Multiarray<std::int32_t, true> mMask;
        bool mHasMask;

    private:
        friend class UnitTestPDEFilter2;
    };
}
