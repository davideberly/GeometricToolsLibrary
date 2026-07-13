// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

#pragma once

// Support for 3D image processing using finite differences for partial
// differential equations. PDEFilter3 is an abstract base class.

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
    class PDEFilter3 : public PDEFilter<T>
    {
    public:
        // Abstract base class. If all pixels are to be processed, pass
        // nullptr for 'mask'. Otherwise, 'mask' is the same size as 'data'
        // and has mask[i] != 0 for pixels that should be processed or
        // mask[i] = 0 for pixels that should not be processed.
        PDEFilter3(std::size_t xBound, std::size_t yBound, std::size_t zBound, T const& xSpacing,
            T const& ySpacing, T const& zSpacing, T const* data, std::int32_t const* mask,
            T const& borderValue, typename PDEFilter<T>::ScaleType scaleType)
            :
            PDEFilter<T>(xBound * yBound * zBound, data, borderValue, scaleType),
            mXBound(xBound),
            mYBound(yBound),
            mZBound(zBound),
            mXSpacing(xSpacing),
            mYSpacing(ySpacing),
            mZSpacing(zSpacing),
            mInvDx(C_<T>(1) / xSpacing),
            mInvDy(C_<T>(1) / ySpacing),
            mInvDz(C_<T>(1) / zSpacing),
            mHalfInvDx(C_<T>(1, 2) * mInvDx),
            mHalfInvDy(C_<T>(1, 2) * mInvDy),
            mHalfInvDz(C_<T>(1, 2) * mInvDz),
            mInvDxDx(mInvDx * mInvDx),
            mFourthInvDxDy(mHalfInvDx * mHalfInvDy),
            mFourthInvDxDz(mHalfInvDx * mHalfInvDz),
            mInvDyDy(mInvDy * mInvDy),
            mFourthInvDyDz(mHalfInvDy * mHalfInvDz),
            mInvDzDz(mInvDz * mInvDz),
            mUmmm(C_<T>(0)), mUzmm(C_<T>(0)), mUpmm(C_<T>(0)),
            mUmzm(C_<T>(0)), mUzzm(C_<T>(0)), mUpzm(C_<T>(0)),
            mUmpm(C_<T>(0)), mUzpm(C_<T>(0)), mUppm(C_<T>(0)),
            mUmmz(C_<T>(0)), mUzmz(C_<T>(0)), mUpmz(C_<T>(0)),
            mUmzz(C_<T>(0)), mUzzz(C_<T>(0)), mUpzz(C_<T>(0)),
            mUmpz(C_<T>(0)), mUzpz(C_<T>(0)), mUppz(C_<T>(0)),
            mUmmp(C_<T>(0)), mUzmp(C_<T>(0)), mUpmp(C_<T>(0)),
            mUmzp(C_<T>(0)), mUzzp(C_<T>(0)), mUpzp(C_<T>(0)),
            mUmpp(C_<T>(0)), mUzpp(C_<T>(0)), mUppp(C_<T>(0)),
            mSrc(0),
            mDst(1),
            mMask{ xBound + 2, yBound + 2, zBound + 2 },
            mHasMask(mask != nullptr)
        {
            for (std::size_t i = 0; i < 2; ++i)
            {
                mBuffer[i].resize({ xBound + 2, yBound + 2, zBound + 2 });
            }

            // The mBuffer[] are ping-pong buffers for filtering.
            for (std::size_t z = 0, zp = 1, i = 0; z < mZBound; ++z, ++zp)
            {
                for (std::size_t y = 0, yp = 1; y < mYBound; ++y, ++yp)
                {
                    for (std::size_t x = 0, xp = 1; x < mXBound; ++x, ++xp, ++i)
                    {
                        mBuffer[mSrc](xp, yp, zp) =
                            this->mOffset + (data[i] - this->mMin) * this->mScale;
                        mBuffer[mDst](xp, yp, zp) = C_<T>(0);
                        mMask(xp, yp, zp) = (mHasMask ? mask[i] : 1);
                    }
                }
            }

            // Assign values to the 1-voxel-thick border. TODO: Modify to
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
            // values to those voxels that are 26-neighbors of the mask
            // voxels.
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

        virtual ~PDEFilter3() = default;

        // Member access. The internal 3D images for "data" and "mask" are
        // copies of the inputs to the constructor but padded with a 1-voxel
        // thick border to support filtering on the image boundary. These
        // images are of size (xbound+2)-by-(ybound+2)-by-(zbound+2). The
        // correct lookups into the padded arrays are handled internally.
        inline std::size_t GetXBound() const
        {
            return mXBound;
        }

        inline std::size_t GetYBound() const
        {
            return mYBound;
        }

        inline std::size_t GetZBound() const
        {
            return mZBound;
        }

        inline T const& GetXSpacing() const
        {
            return mXSpacing;
        }

        inline T const& GetYSpacing() const
        {
            return mYSpacing;
        }

        inline T const& GetZSpacing() const
        {
            return mZSpacing;
        }

        // Voxel access and derivative estimation. The lookups into the
        // padded data are handled correctly. The estimation involves only
        // the 3-by-3-by-3 neighborhood of (x,y,z), where 0 <= x < xbound,
        // 0 <= y < ybound and 0 <= z < zbound. TODO: If larger neighborhoods
        // are desired at a later date, the padding and associated code must
        // be adjusted accordingly.
        T GetU(std::size_t x, std::size_t y, std::size_t z) const
        {
            auto const& F = mBuffer[mSrc];
            std::size_t xp1 = x + 1, yp1 = y + 1, zp1 = z + 1;
            return F(xp1, yp1, zp1);
        }

        T GetUx(std::size_t x, std::size_t y, std::size_t z) const
        {
            auto const& F = mBuffer[mSrc];
            std::size_t xp2 = x + 2, yp1 = y + 1, zp1 = z + 1;
            return mHalfInvDx * (F(xp2, yp1, zp1) - F(x, yp1, zp1));
        }

        T GetUy(std::size_t x, std::size_t y, std::size_t z) const
        {
            auto const& F = mBuffer[mSrc];
            std::size_t xp1 = x + 1, yp2 = y + 2, zp1 = z + 1;
            return mHalfInvDy * (F(xp1, yp2, zp1) - F(xp1, y, zp1));
        }

        T GetUz(std::size_t x, std::size_t y, std::size_t z) const
        {
            auto const& F = mBuffer[mSrc];
            std::size_t xp1 = x + 1, yp1 = y + 1, zp2 = z + 2;
            return mHalfInvDz * (F(xp1, yp1, zp2) - F(xp1, yp1, z));
        }

        T GetUxx(std::size_t x, std::size_t y, std::size_t z) const
        {
            auto const& F = mBuffer[mSrc];
            std::size_t xp1 = x + 1, xp2 = x + 2, yp1 = y + 1, zp1 = z + 1;
            return mInvDxDx * (F(xp2, yp1, zp1) - C_<T>(2) * F(xp1, yp1, zp1)
                + F(x, yp1, zp1));
        }

        T GetUxy(std::size_t x, std::size_t y, std::size_t z) const
        {
            auto const& F = mBuffer[mSrc];
            std::size_t xp2 = x + 2, yp2 = y + 2, zp1 = z + 1;
            return mFourthInvDxDy * (F(x, y, zp1) - F(xp2, y, zp1)
                + F(xp2, yp2, zp1) - F(x, yp2, zp1));
        }

        T GetUxz(std::size_t x, std::size_t y, std::size_t z) const
        {
            auto const& F = mBuffer[mSrc];
            std::size_t xp2 = x + 2, yp1 = y + 1, zp2 = z + 2;
            return mFourthInvDxDz * (F(x, yp1, z) - F(xp2, yp1, z)
                + F(xp2, yp1, zp2) - F(x, yp1, zp2));
        }

        T GetUyy(std::size_t x, std::size_t y, std::size_t z) const
        {
            auto const& F = mBuffer[mSrc];
            std::size_t xp1 = x + 1, yp1 = y + 1, yp2 = y + 2, zp1 = z + 1;
            return mInvDyDy * (F(xp1, yp2, zp1) - C_<T>(2) * F(xp1, yp1, zp1)
                + F(xp1, y, zp1));
        }

        T GetUyz(std::size_t x, std::size_t y, std::size_t z) const
        {
            auto const& F = mBuffer[mSrc];
            std::size_t xp1 = x + 1, yp2 = y + 2, zp2 = z + 2;
            return mFourthInvDyDz * (F(xp1, y, z) - F(xp1, yp2, z) + F(xp1, yp2, zp2)
                - F(xp1, y, zp2));
        }

        T GetUzz(std::size_t x, std::size_t y, std::size_t z) const
        {
            auto const& F = mBuffer[mSrc];
            std::size_t xp1 = x + 1, yp1 = y + 1, zp1 = z + 1, zp2 = z + 2;
            return mInvDzDz * (F(xp1, yp1, zp2) - C_<T>(2) * F(xp1, yp1, zp1)
                + F(xp1, yp1, z));
        }

        std::int32_t GetMask(std::size_t x, std::size_t y, std::size_t z) const
        {
            std::size_t xp1 = x + 1, yp1 = y + 1, zp1 = z + 1;
            return mMask(xp1, yp1, zp1);
        }

    protected:
        // Assign values to the 1-voxel image border.
        void AssignDirichletImageBorder()
        {
            std::size_t xBp1 = mXBound + 1, yBp1 = mYBound + 1, zBp1 = mZBound + 1;
            std::size_t x{}, y{}, z{};

            // vertex (0,0,0)
            mBuffer[mSrc](0, 0, 0) = this->mBorderValue;
            mBuffer[mDst](0, 0, 0) = this->mBorderValue;
            if (mHasMask)
            {
                mMask(0, 0, 0) = 0;
            }

            // vertex (xmax,0,0)
            mBuffer[mSrc](xBp1, 0, 0) = this->mBorderValue;
            mBuffer[mDst](xBp1, 0, 0) = this->mBorderValue;
            if (mHasMask)
            {
                mMask(xBp1, 0, 0) = 0;
            }

            // vertex (0,ymax,0)
            mBuffer[mSrc](0, yBp1, 0) = this->mBorderValue;
            mBuffer[mDst](0, yBp1, 0) = this->mBorderValue;
            if (mHasMask)
            {
                mMask(0, yBp1, 0) = 0;
            }

            // vertex (xmax,ymax,0)
            mBuffer[mSrc](xBp1, yBp1, 0) = this->mBorderValue;
            mBuffer[mDst](xBp1, yBp1, 0) = this->mBorderValue;
            if (mHasMask)
            {
                mMask(xBp1, yBp1, 0) = 0;
            }

            // vertex (0,0,zmax)
            mBuffer[mSrc](0, 0, zBp1) = this->mBorderValue;
            mBuffer[mDst](0, 0, zBp1) = this->mBorderValue;
            if (mHasMask)
            {
                mMask(0, 0, zBp1) = 0;
            }

            // vertex (xmax,0,zmax)
            mBuffer[mSrc](xBp1, 0, zBp1) = this->mBorderValue;
            mBuffer[mDst](xBp1, 0, zBp1) = this->mBorderValue;
            if (mHasMask)
            {
                mMask(xBp1, 0, zBp1) = 0;
            }

            // vertex (0,ymax,zmax)
            mBuffer[mSrc](0, yBp1, zBp1) = this->mBorderValue;
            mBuffer[mDst](0, yBp1, zBp1) = this->mBorderValue;
            if (mHasMask)
            {
                mMask(0, yBp1, zBp1) = 0;
            }

            // vertex (xmax,ymax,zmax)
            mBuffer[mSrc](xBp1, yBp1, zBp1) = this->mBorderValue;
            mBuffer[mDst](xBp1, yBp1, zBp1) = this->mBorderValue;
            if (mHasMask)
            {
                mMask(xBp1, yBp1, zBp1) = 0;
            }

            // edges (x,0,0) and (x,ymax,0)
            for (x = 1; x <= mXBound; ++x)
            {
                mBuffer[mSrc](x, 0, 0) = this->mBorderValue;
                mBuffer[mDst](x, 0, 0) = this->mBorderValue;
                if (mHasMask)
                {
                    mMask(x, 0, 0) = 0;
                }

                mBuffer[mSrc](x, yBp1, 0) = this->mBorderValue;
                mBuffer[mDst](x, yBp1, 0) = this->mBorderValue;
                if (mHasMask)
                {
                    mMask(x, yBp1, 0) = 0;
                }
            }

            // edges (0,y,0) and (xmax,y,0)
            for (y = 1; y <= mYBound; ++y)
            {
                mBuffer[mSrc](0, y, 0) = this->mBorderValue;
                mBuffer[mDst](0, y, 0) = this->mBorderValue;
                if (mHasMask)
                {
                    mMask(0, y, 0) = 0;
                }

                mBuffer[mSrc](xBp1, y, 0) = this->mBorderValue;
                mBuffer[mDst](xBp1, y, 0) = this->mBorderValue;
                if (mHasMask)
                {
                    mMask(xBp1, y, 0) = 0;
                }
            }

            // edges (x,0,zmax) and (x,ymax,zmax)
            for (x = 1; x <= mXBound; ++x)
            {
                mBuffer[mSrc](x, 0, zBp1) = this->mBorderValue;
                mBuffer[mDst](x, 0, zBp1) = this->mBorderValue;
                if (mHasMask)
                {
                    mMask(x, 0, zBp1) = 0;
                }

                mBuffer[mSrc](x, yBp1, zBp1) = this->mBorderValue;
                mBuffer[mDst](x, yBp1, zBp1) = this->mBorderValue;
                if (mHasMask)
                {
                    mMask(x, yBp1, zBp1) = 0;
                }
            }

            // edges (0,y,zmax) and (xmax,y,zmax)
            for (y = 1; y <= mYBound; ++y)
            {
                mBuffer[mSrc](0, y, zBp1) = this->mBorderValue;
                mBuffer[mDst](0, y, zBp1) = this->mBorderValue;
                if (mHasMask)
                {
                    mMask(0, y, zBp1) = 0;
                }

                mBuffer[mSrc](xBp1, y, zBp1) = this->mBorderValue;
                mBuffer[mDst](xBp1, y, zBp1) = this->mBorderValue;
                if (mHasMask)
                {
                    mMask(xBp1, y, zBp1) = 0;
                }
            }

            // edges (0,0,z) and (xmax,0,z)
            for (z = 1; z <= mZBound; ++z)
            {
                mBuffer[mSrc](0, 0, z) = this->mBorderValue;
                mBuffer[mDst](0, 0, z) = this->mBorderValue;
                if (mHasMask)
                {
                    mMask(0, 0, z) = 0;
                }

                mBuffer[mSrc](xBp1, 0, z) = this->mBorderValue;
                mBuffer[mDst](xBp1, 0, z) = this->mBorderValue;
                if (mHasMask)
                {
                    mMask(xBp1, 0, z) = 0;
                }
            }

            // edges (0,ymax,z) and (xmax,ymax,z)
            for (z = 1; z <= mZBound; ++z)
            {
                mBuffer[mSrc](0, yBp1, z) = this->mBorderValue;
                mBuffer[mDst](0, yBp1, z) = this->mBorderValue;
                if (mHasMask)
                {
                    mMask(0, yBp1, z) = 0;
                }

                mBuffer[mSrc](xBp1, yBp1, z) = this->mBorderValue;
                mBuffer[mDst](xBp1, yBp1, z) = this->mBorderValue;
                if (mHasMask)
                {
                    mMask(xBp1, yBp1, z) = 0;
                }
            }

            // faces (x,y,0) and (x,y,zmax)
            for (y = 1; y <= mYBound; ++y)
            {
                for (x = 1; x <= mXBound; ++x)
                {
                    mBuffer[mSrc](x, y, 0) = this->mBorderValue;
                    mBuffer[mDst](x, y, 0) = this->mBorderValue;
                    if (mHasMask)
                    {
                        mMask(x, y, 0) = 0;
                    }

                    mBuffer[mSrc](x, y, zBp1) = this->mBorderValue;
                    mBuffer[mDst](x, y, zBp1) = this->mBorderValue;
                    if (mHasMask)
                    {
                        mMask(x, y, zBp1) = 0;
                    }
                }
            }

            // faces (x,0,z) and (x,ymax,z)
            for (z = 1; z <= mZBound; ++z)
            {
                for (x = 1; x <= mXBound; ++x)
                {
                    mBuffer[mSrc](x, 0, z) = this->mBorderValue;
                    mBuffer[mDst](x, 0, z) = this->mBorderValue;
                    if (mHasMask)
                    {
                        mMask(x, 0, z) = 0;
                    }

                    mBuffer[mSrc](x, yBp1, z) = this->mBorderValue;
                    mBuffer[mDst](x, yBp1, z) = this->mBorderValue;
                    if (mHasMask)
                    {
                        mMask(x, yBp1, z) = 0;
                    }
                }
            }

            // faces (0,y,z) and (xmax,y,z)
            for (z = 1; z <= mZBound; ++z)
            {
                for (y = 1; y <= mYBound; ++y)
                {
                    mBuffer[mSrc](0, y, z) = this->mBorderValue;
                    mBuffer[mDst](0, y, z) = this->mBorderValue;
                    if (mHasMask)
                    {
                        mMask(0, y, z) = 0;
                    }

                    mBuffer[mSrc](xBp1, y, z) = this->mBorderValue;
                    mBuffer[mDst](xBp1, y, z) = this->mBorderValue;
                    if (mHasMask)
                    {
                        mMask(xBp1, y, z) = 0;
                    }
                }
            }
        }

        void AssignNeumannImageBorder()
        {
            std::size_t xBp1 = mXBound + 1, yBp1 = mYBound + 1, zBp1 = mZBound + 1;
            std::size_t x{}, y{}, z{};
            T duplicate{};

            // vertex (0,0,0)
            duplicate = mBuffer[mSrc](1, 1, 1);
            mBuffer[mSrc](0, 0, 0) = duplicate;
            mBuffer[mDst](0, 0, 0) = duplicate;
            if (mHasMask)
            {
                mMask(0, 0, 0) = 0;
            }

            // vertex (xmax,0,0)
            duplicate = mBuffer[mSrc](mXBound, 1, 1);
            mBuffer[mSrc](xBp1, 0, 0) = duplicate;
            mBuffer[mDst](xBp1, 0, 0) = duplicate;
            if (mHasMask)
            {
                mMask(xBp1, 0, 0) = 0;
            }

            // vertex (0,ymax,0)
            duplicate = mBuffer[mSrc](1, mYBound, 1);
            mBuffer[mSrc](0, yBp1, 0) = duplicate;
            mBuffer[mDst](0, yBp1, 0) = duplicate;
            if (mHasMask)
            {
                mMask(0, yBp1, 0) = 0;
            }

            // vertex (xmax,ymax,0)
            duplicate = mBuffer[mSrc](mXBound, mYBound, 1);
            mBuffer[mSrc](xBp1, yBp1, 0) = duplicate;
            mBuffer[mDst](xBp1, yBp1, 0) = duplicate;
            if (mHasMask)
            {
                mMask(xBp1, yBp1, 0) = 0;
            }

            // vertex (0,0,zmax)
            duplicate = mBuffer[mSrc](1, 1, mZBound);
            mBuffer[mSrc](0, 0, zBp1) = duplicate;
            mBuffer[mDst](0, 0, zBp1) = duplicate;
            if (mHasMask)
            {
                mMask(0, 0, zBp1) = 0;
            }

            // vertex (xmax,0,zmax)
            duplicate = mBuffer[mSrc](mXBound, 1, mZBound);
            mBuffer[mSrc](xBp1, 0, zBp1) = duplicate;
            mBuffer[mDst](xBp1, 0, zBp1) = duplicate;
            if (mHasMask)
            {
                mMask(xBp1, 0, zBp1) = 0;
            }

            // vertex (0,ymax,zmax)
            duplicate = mBuffer[mSrc](1, mYBound, mZBound);
            mBuffer[mSrc](0, yBp1, zBp1) = duplicate;
            mBuffer[mDst](0, yBp1, zBp1) = duplicate;
            if (mHasMask)
            {
                mMask(0, yBp1, zBp1) = 0;
            }

            // vertex (xmax,ymax,zmax)
            duplicate = mBuffer[mSrc](mXBound, mYBound, mZBound);
            mBuffer[mSrc](xBp1, yBp1, zBp1) = duplicate;
            mBuffer[mDst](xBp1, yBp1, zBp1) = duplicate;
            if (mHasMask)
            {
                mMask(xBp1, yBp1, zBp1) = 0;
            }

            // edges (x,0,0) and (x,ymax,0)
            for (x = 1; x <= mXBound; ++x)
            {
                duplicate = mBuffer[mSrc](x, 1, 1);
                mBuffer[mSrc](x, 0, 0) = duplicate;
                mBuffer[mDst](x, 0, 0) = duplicate;
                if (mHasMask)
                {
                    mMask(x, 0, 0) = 0;
                }

                duplicate = mBuffer[mSrc](x, mYBound, 1);
                mBuffer[mSrc](x, yBp1, 0) = duplicate;
                mBuffer[mDst](x, yBp1, 0) = duplicate;
                if (mHasMask)
                {
                    mMask(x, yBp1, 0) = 0;
                }
            }

            // edges (0,y,0) and (xmax,y,0)
            for (y = 1; y <= mYBound; ++y)
            {
                duplicate = mBuffer[mSrc](1, y, 1);
                mBuffer[mSrc](0, y, 0) = duplicate;
                mBuffer[mDst](0, y, 0) = duplicate;
                if (mHasMask)
                {
                    mMask(0, y, 0) = 0;
                }

                duplicate = mBuffer[mSrc](mXBound, y, 1);
                mBuffer[mSrc](xBp1, y, 0) = duplicate;
                mBuffer[mDst](xBp1, y, 0) = duplicate;
                if (mHasMask)
                {
                    mMask(xBp1, y, 0) = 0;
                }
            }

            // edges (x,0,zmax) and (x,ymax,zmax)
            for (x = 1; x <= mXBound; ++x)
            {
                duplicate = mBuffer[mSrc](x, 1, mZBound);
                mBuffer[mSrc](x, 0, zBp1) = duplicate;
                mBuffer[mDst](x, 0, zBp1) = duplicate;
                if (mHasMask)
                {
                    mMask(x, 0, zBp1) = 0;
                }

                duplicate = mBuffer[mSrc](x, mYBound, mZBound);
                mBuffer[mSrc](x, mYBound, mZBound) = duplicate;
                mBuffer[mDst](x, mYBound, mZBound) = duplicate;
                if (mHasMask)
                {
                    mMask(x, mYBound, mZBound) = 0;
                }
            }

            // edges (0,y,zmax) and (xmax,y,zmax)
            for (y = 1; y <= mYBound; ++y)
            {
                duplicate = mBuffer[mSrc](1, y, mZBound);
                mBuffer[mSrc](0, y, zBp1) = duplicate;
                mBuffer[mDst](0, y, zBp1) = duplicate;
                if (mHasMask)
                {
                    mMask(0, y, zBp1) = 0;
                }

                duplicate = mBuffer[mSrc](mXBound, y, mZBound);
                mBuffer[mSrc](xBp1, y, zBp1) = duplicate;
                mBuffer[mDst](xBp1, y, zBp1) = duplicate;
                if (mHasMask)
                {
                    mMask(xBp1, y, zBp1) = 0;
                }
            }

            // edges (0,0,z) and (xmax,0,z)
            for (z = 1; z <= mZBound; ++z)
            {
                duplicate = mBuffer[mSrc](1, 1, z);
                mBuffer[mSrc](0, 0, z) = duplicate;
                mBuffer[mDst](0, 0, z) = duplicate;
                if (mHasMask)
                {
                    mMask(0, 0, z) = 0;
                }

                duplicate = mBuffer[mSrc](mXBound, 1, z);
                mBuffer[mSrc](xBp1, 0, z) = duplicate;
                mBuffer[mDst](xBp1, 0, z) = duplicate;
                if (mHasMask)
                {
                    mMask(xBp1, 0, z) = 0;
                }
            }

            // edges (0,ymax,z) and (xmax,ymax,z)
            for (z = 1; z <= mZBound; ++z)
            {
                duplicate = mBuffer[mSrc](1, mYBound, z);
                mBuffer[mSrc](0, yBp1, z) = duplicate;
                mBuffer[mDst](0, yBp1, z) = duplicate;
                if (mHasMask)
                {
                    mMask(0, yBp1, z) = 0;
                }

                duplicate = mBuffer[mSrc](mXBound, mYBound, z);
                mBuffer[mSrc](xBp1, yBp1, z) = duplicate;
                mBuffer[mDst](xBp1, yBp1, z) = duplicate;
                if (mHasMask)
                {
                    mMask(xBp1, yBp1, z) = 0;
                }
            }

            // faces (x,y,0) and (x,y,zmax)
            for (y = 1; y <= mYBound; ++y)
            {
                for (x = 1; x <= mXBound; ++x)
                {
                    duplicate = mBuffer[mSrc](x, y, 1);
                    mBuffer[mSrc](x, y, 0) = duplicate;
                    mBuffer[mDst](x, y, 0) = duplicate;
                    if (mHasMask)
                    {
                        mMask(x, y, 0) = 0;
                    }

                    duplicate = mBuffer[mSrc](x, y, mZBound);
                    mBuffer[mSrc](x, y, zBp1) = duplicate;
                    mBuffer[mDst](x, y, zBp1) = duplicate;
                    if (mHasMask)
                    {
                        mMask(x, y, zBp1) = 0;
                    }
                }
            }

            // faces (x,0,z) and (x,ymax,z)
            for (z = 1; z <= mZBound; ++z)
            {
                for (x = 1; x <= mXBound; ++x)
                {
                    duplicate = mBuffer[mSrc](x, 1, z);
                    mBuffer[mSrc](x, 0, z) = duplicate;
                    mBuffer[mDst](x, 0, z) = duplicate;
                    if (mHasMask)
                    {
                        mMask(x, 0, z) = 0;
                    }

                    duplicate = mBuffer[mSrc](x, mYBound, z);
                    mBuffer[mSrc](x, yBp1, z) = duplicate;
                    mBuffer[mDst](x, yBp1, z) = duplicate;
                    if (mHasMask)
                    {
                        mMask(x, yBp1, z) = 0;
                    }
                }
            }

            // faces (0,y,z) and (xmax,y,z)
            for (z = 1; z <= mZBound; ++z)
            {
                for (y = 1; y <= mYBound; ++y)
                {
                    duplicate = mBuffer[mSrc](1, y, z);
                    mBuffer[mSrc](0, y, z) = duplicate;
                    mBuffer[mDst](0, y, z) = duplicate;
                    if (mHasMask)
                    {
                        mMask(0, y, z) = 0;
                    }

                    duplicate = mBuffer[mSrc](mXBound, y, z);
                    mBuffer[mSrc](xBp1, y, z) = duplicate;
                    mBuffer[mDst](xBp1, y, z) = duplicate;
                    if (mHasMask)
                    {
                        mMask(xBp1, y, z) = 0;
                    }
                }
            }
        }

        // Assign values to the 1-voxel mask border.
        void AssignDirichletMaskBorder()
        {
            for (std::size_t z = 1; z <= mZBound; ++z)
            {
                for (std::size_t y = 1; y <= mYBound; ++y)
                {
                    for (std::size_t x = 1; x <= mXBound; ++x)
                    {
                        if (mMask(x, y, z))
                        {
                            continue;
                        }

                        bool found = false;
                        for (std::size_t i2 = 0, j2 = z - 1; i2 < 3 && !found; ++i2, ++j2)
                        {
                            for (std::size_t i1 = 0, j1 = y - 1; i1 < 3 && !found; ++i1, ++j1)
                            {
                                for (std::size_t i0 = 0, j0 = x - 1; i0 < 3; ++i0, ++j0)
                                {
                                    if (mMask(j0, j1, j2))
                                    {
                                        mBuffer[mSrc](x, y, z) = this->mBorderValue;
                                        mBuffer[mDst](x, y, z) = this->mBorderValue;
                                        found = true;
                                        break;
                                    }
                                }
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
            for (std::size_t z = 1; z <= mZBound; ++z)
            {
                for (std::size_t y = 1; y <= mYBound; ++y)
                {
                    for (std::size_t x = 1; x <= mXBound; ++x)
                    {
                        if (mMask(x, y, z))
                        {
                            continue;
                        }

                        std::size_t count = 0;
                        T average = C_<T>(0);
                        for (std::size_t i2 = 0, j2 = z - 1; i2 < 3; ++i2, ++j2)
                        {
                            for (std::size_t i1 = 0, j1 = y - 1; i1 < 3; ++i1, ++j1)
                            {
                                for (std::size_t i0 = 0, j0 = x - 1; i0 < 3; ++i0, ++j0)
                                {
                                    if (mMask(j0, j1, j2))
                                    {
                                        average += mBuffer[mSrc](j0, j1, j2);
                                        ++count;
                                    }
                                }
                            }
                        }

                        if (count > 0)
                        {
                            average /= static_cast<T>(count);
                            mBuffer[mSrc](x, y, z) = average;
                            mBuffer[mDst](x, y, z) = average;
                        }
                    }
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

        // Iterate over all the pixels and call OnUpdate(x,y,z) for each voxel
        // that is not masked out.
        virtual void OnUpdate() override
        {
            for (std::size_t z = 1; z <= mZBound; ++z)
            {
                for (std::size_t y = 1; y <= mYBound; ++y)
                {
                    for (std::size_t x = 1; x <= mXBound; ++x)
                    {
                        if (!mHasMask || mMask(x, y, z))
                        {
                            OnUpdateSingle(x, y, z);
                        }
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

        // The per-pixel processing depends on the PDE algorithm. The (x,y,z)
        // must be in padded coordinates: 1 <= x <= xbound, 1 <= y <= ybound
        // and 1 <= z <= zbound.
        virtual void OnUpdateSingle(std::size_t x, std::size_t y, std::size_t z) = 0;

        // Copy source data to temporary storage.
        void LookUp7(std::size_t x, std::size_t y, std::size_t z)
        {
            auto const& F = mBuffer[mSrc];
            std::size_t xm = x - 1, xp = x + 1;
            std::size_t ym = y - 1, yp = y + 1;
            std::size_t zm = z - 1, zp = z + 1;
            mUzzm = F(x, y, zm);
            mUzmz = F(x, ym, z);
            mUmzz = F(xm, y, z);
            mUzzz = F(x, y, z);
            mUpzz = F(xp, y, z);
            mUzpz = F(x, yp, z);
            mUzzp = F(x, y, zp);
        }

        void LookUp27(std::size_t x, std::size_t y, std::size_t z)
        {
            auto const& F = mBuffer[mSrc];
            std::size_t xm = x - 1, xp = x + 1;
            std::size_t ym = y - 1, yp = y + 1;
            std::size_t zm = z - 1, zp = z + 1;
            mUmmm = F(xm, ym, zm);
            mUzmm = F(x, ym, zm);
            mUpmm = F(xp, ym, zm);
            mUmzm = F(xm, y, zm);
            mUzzm = F(x, y, zm);
            mUpzm = F(xp, y, zm);
            mUmpm = F(xm, yp, zm);
            mUzpm = F(x, yp, zm);
            mUppm = F(xp, yp, zm);
            mUmmz = F(xm, ym, z);
            mUzmz = F(x, ym, z);
            mUpmz = F(xp, ym, z);
            mUmzz = F(xm, y, z);
            mUzzz = F(x, y, z);
            mUpzz = F(xp, y, z);
            mUmpz = F(xm, yp, z);
            mUzpz = F(x, yp, z);
            mUppz = F(xp, yp, z);
            mUmmp = F(xm, ym, zp);
            mUzmp = F(x, ym, zp);
            mUpmp = F(xp, ym, zp);
            mUmzp = F(xm, y, zp);
            mUzzp = F(x, y, zp);
            mUpzp = F(xp, y, zp);
            mUmpp = F(xm, yp, zp);
            mUzpp = F(x, yp, zp);
            mUppp = F(xp, yp, zp);
        }

        // Image parameters.
        std::size_t mXBound, mYBound, mZBound;
        T mXSpacing;       // dx
        T mYSpacing;       // dy
        T mZSpacing;       // dz
        T mInvDx;          // 1/dx
        T mInvDy;          // 1/dy
        T mInvDz;          // 1/dz
        T mHalfInvDx;      // 1/(2*dx)
        T mHalfInvDy;      // 1/(2*dy)
        T mHalfInvDz;      // 1/(2*dz)
        T mInvDxDx;        // 1/(dx*dx)
        T mFourthInvDxDy;  // 1/(4*dx*dy)
        T mFourthInvDxDz;  // 1/(4*dx*dz)
        T mInvDyDy;        // 1/(dy*dy)
        T mFourthInvDyDz;  // 1/(4*dy*dz)
        T mInvDzDz;        // 1/(dz*dz)

        // Temporary storage for 3x3x3 neighborhood.  In the notation mUxyz,
        // the x, y and z indices are in {m,z,p}, referring to subtract 1 (m),
        // no change (z), or add 1 (p) to the appropriate index.
        T mUmmm, mUzmm, mUpmm;
        T mUmzm, mUzzm, mUpzm;
        T mUmpm, mUzpm, mUppm;
        T mUmmz, mUzmz, mUpmz;
        T mUmzz, mUzzz, mUpzz;
        T mUmpz, mUzpz, mUppz;
        T mUmmp, mUzmp, mUpmp;
        T mUmzp, mUzzp, mUpzp;
        T mUmpp, mUzpp, mUppp;

        // Successive iterations toggle between two buffers.
        std::array<Multiarray<T, true>, 2> mBuffer;
        std::size_t mSrc, mDst;
        Multiarray<std::int32_t, true> mMask;
        bool mHasMask;

    private:
        friend class UnitTestPDEFilter3;
    };
}
