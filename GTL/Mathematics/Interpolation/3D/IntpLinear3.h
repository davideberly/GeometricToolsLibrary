// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.14

#pragma once

// The interpolator is for uniformly spaced(x,y z)-values. The input samples
// must be stored in lexicographical order to represent f(x,y,z); that is,
// F[c + xBound*(r + yBound*s)] corresponds to f(x,y,z), where c is the index
// corresponding to x, r is the index corresponding to y, and s is the index
// corresponding to z.

#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Utility/Exceptions.h>
#include <array>
#include <cstddef>
#include <type_traits>

namespace gtl
{
    template <typename T>
    class IntpLinear3
    {
    public:
        // Construction.
        IntpLinear3(std::size_t xBound, std::size_t yBound, std::size_t zBound, T const& xMin,
            T const& xSpacing, T const& yMin, T const& ySpacing, T const& zMin,
            T const& zSpacing, T const* F)
            :
            mXBound(xBound),
            mYBound(yBound),
            mZBound(zBound),
            mQuantity(xBound* yBound* zBound),
            mXMin(xMin),
            mXSpacing(xSpacing),
            mYMin(yMin),
            mYSpacing(ySpacing),
            mZMin(zMin),
            mZSpacing(zSpacing),
            mF(F),
            mBlend{}
        {
            // At least a 2x2x2 block of data points are needed to construct
            // the trilinear interpolation.
            GTL_ARGUMENT_ASSERT(
                mXBound >= 2 && mYBound >= 2 && mZBound >= 2 && mF != nullptr &&
                mXSpacing > C_<T>(0) && mYSpacing > C_<T>(0) && mZSpacing > C_<T>(0),
                "Invalid input.");

            mXMax = mXMin + mXSpacing * static_cast<T>(mXBound - 1);
            mInvXSpacing = C_<T>(1) / mXSpacing;
            mYMax = mYMin + mYSpacing * static_cast<T>(mYBound - 1);
            mInvYSpacing = C_<T>(1) / mYSpacing;
            mZMax = mZMin + mZSpacing * static_cast<T>(mZBound - 1);
            mInvZSpacing = C_<T>(1) / mZSpacing;

            mBlend[0][0] = C_<T>(1);
            mBlend[0][1] = -C_<T>(1);
            mBlend[1][0] = C_<T>(0);
            mBlend[1][1] = C_<T>(1);
        }

        // Member access.
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

        inline std::size_t GetQuantity() const
        {
            return mQuantity;
        }

        inline T const* GetF() const
        {
            return mF;
        }

        inline T const& GetXMin() const
        {
            return mXMin;
        }

        inline T const& GetXMax() const
        {
            return mXMax;
        }

        inline T const& GetXSpacing() const
        {
            return mXSpacing;
        }

        inline T const& GetYMin() const
        {
            return mYMin;
        }

        inline T const& GetYMax() const
        {
            return mYMax;
        }

        inline T const& GetYSpacing() const
        {
            return mYSpacing;
        }

        inline T const& GetZMin() const
        {
            return mZMin;
        }

        inline T const& GetZMax() const
        {
            return mZMax;
        }

        inline T const& GetZSpacing() const
        {
            return mZSpacing;
        }

        // Evaluate the function and its derivatives. The functions clamp the
        // inputs to xmin <= x <= xmax, ymin <= y <= ymax and
        // zmin <= z <= zmax. The first operator is for function evaluation.
        // The second operator is for function or derivative evaluations. The
        // xOrder argument is the order of the x-derivative, the yOrder
        // argument is the order of the y-derivative, and the zOrder argument
        // is the order of the z-derivative. All orders are zero to get the
        // function value itself.
        T operator()(T const& x, T const& y, T const& z) const
        {
            return operator()(0, 0, 0, x, y, z);
        }

        T operator()(std::size_t xOrder, std::size_t yOrder, std::size_t zOrder, T const& x,
            T const& y, T const& z) const
        {
            // Compute x-index and clamp to image.
            T xIndex = (x - mXMin) * mInvXSpacing;
            std::size_t ix{};
            if (xIndex >= C_<T>(0))
            {
                ix = static_cast<std::size_t>(xIndex);
                if (ix >= mXBound)
                {
                    ix = mXBound - 1;
                }
            }
            else
            {
                ix = 0;
            }

            // Compute y-index and clamp to image.
            T yIndex = (y - mYMin) * mInvYSpacing;
            std::size_t iy{};
            if (yIndex >= C_<T>(0))
            {
                iy = static_cast<std::size_t>(yIndex);
                if (iy >= mYBound)
                {
                    iy = mYBound - 1;
                }
            }
            else
            {
                iy = 0;
            }

            // Compute z-index and clamp to image.
            T zIndex = (z - mZMin) * mInvZSpacing;
            std::size_t iz{};
            if (zIndex >= C_<T>(0))
            {
                iz = static_cast<std::size_t>(zIndex);
                if (iz >= mZBound)
                {
                    iz = mZBound - 1;
                }
            }
            else
            {
                iz = 0;
            }

            // Compute U.
            std::array<T, 2> U{};
            T xMultiplier{};
            switch (xOrder)
            {
            case 0:
                U[0] = C_<T>(1);
                U[1] = xIndex - static_cast<T>(ix);
                xMultiplier = C_<T>(1);
                break;
            case 1:
                U[0] = C_<T>(0);
                U[1] = C_<T>(1);
                xMultiplier = mInvXSpacing;
                break;
            default:
                return C_<T>(0);
            }

            // Compute V.
            std::array<T, 2> V{};
            T yMultiplier{};
            switch (yOrder)
            {
            case 0:
                V[0] = C_<T>(1);
                V[1] = yIndex - static_cast<T>(iy);
                yMultiplier = C_<T>(1);
                break;
            case 1:
                V[0] = C_<T>(0);
                V[1] = C_<T>(1);
                yMultiplier = mInvYSpacing;
                break;
            default:
                return C_<T>(0);
            }

            // Compute W.
            std::array<T, 2> W{};
            T zMultiplier{};
            switch (zOrder)
            {
            case 0:
                W[0] = C_<T>(1);
                W[1] = zIndex - static_cast<T>(iz);
                zMultiplier = C_<T>(1);
                break;
            case 1:
                W[0] = C_<T>(0);
                W[1] = C_<T>(1);
                zMultiplier = mInvZSpacing;
                break;
            default:
                return C_<T>(0);
            }

            // Compute P = M*U, Q = M*V, and R = M*W.
            std::array<T, 2> P{}, Q{}, R{};
            for (std::size_t row = 0; row < 2; ++row)
            {
                P[row] = C_<T>(0);
                Q[row] = C_<T>(0);
                R[row] = C_<T>(0);
                for (std::size_t col = 0; col < 2; ++col)
                {
                    P[row] += mBlend[row][col] * U[col];
                    Q[row] += mBlend[row][col] * V[col];
                    R[row] += mBlend[row][col] * W[col];
                }
            }

            // Compute the tensor product (M*U)(M*V)(M*W)*D where D is the 2x2x2
            // subimage containing (x,y,z).
            T result = C_<T>(0);
            for (std::size_t slice = 0; slice < 2; ++slice)
            {
                std::size_t zClamp = iz + slice;
                if (zClamp >= mZBound)
                {
                    zClamp = mZBound - 1;
                }

                for (std::size_t row = 0; row < 2; ++row)
                {
                    std::size_t yClamp = iy + row;
                    if (yClamp >= mYBound)
                    {
                        yClamp = mYBound - 1;
                    }

                    for (std::size_t col = 0; col < 2; ++col)
                    {
                        std::size_t xClamp = ix + col;
                        if (xClamp >= mXBound)
                        {
                            xClamp = mXBound - 1;
                        }

                        result += P[col] * Q[row] * R[slice] *
                            mF[xClamp + mXBound * (yClamp + mYBound * zClamp)];
                    }
                }
            }
            result *= xMultiplier * yMultiplier * zMultiplier;

            return result;
        }

    private:
        std::size_t mXBound, mYBound, mZBound, mQuantity;
        T mXMin, mXMax, mXSpacing, mInvXSpacing;
        T mYMin, mYMax, mYSpacing, mInvYSpacing;
        T mZMin, mZMax, mZSpacing, mInvZSpacing;
        T const* mF;
        std::array<std::array<T, 2>, 2> mBlend;

    private:
        friend class UnitTestIntpTrilinear3;
    };
}
