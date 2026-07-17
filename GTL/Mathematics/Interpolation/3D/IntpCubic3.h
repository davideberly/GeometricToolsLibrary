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
// corresponding to z. Exact interpolation is achieved by setting catmullRom
// to 'true', giving you the Catmull-Rom blending matrix. If a smooth
// interpolation is desired, set catmullRom to 'false' to obtain B-spline
// blending.

#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Utility/Exceptions.h>
#include <array>
#include <cstddef>
#include <type_traits>

namespace gtl
{
    template <typename T>
    class IntpCubic3
    {
    public:
        // Construction.
        IntpCubic3(std::size_t xBound, std::size_t yBound, std::size_t zBound, T const& xMin,
            T const& xSpacing, T const& yMin, T const& ySpacing, T const& zMin,
            T const& zSpacing, T const* F, bool catmullRom)
            :
            mXBound(xBound),
            mYBound(yBound),
            mZBound(zBound),
            mQuantity(xBound * yBound * zBound),
            mXMin(xMin),
            mXSpacing(xSpacing),
            mYMin(yMin),
            mYSpacing(ySpacing),
            mZMin(zMin),
            mZSpacing(zSpacing),
            mF(F),
            mBlend{}
        {
            // At least a 4x4x4 block of data points are needed to construct
            // the tricubic interpolation.
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

            if (catmullRom)
            {
                mBlend[0][0] = C_<T>(0);
                mBlend[0][1] = -C_<T>(1, 2);
                mBlend[0][2] = C_<T>(1);
                mBlend[0][3] = -C_<T>(1, 2);
                mBlend[1][0] = C_<T>(1);
                mBlend[1][1] = C_<T>(0);
                mBlend[1][2] = -C_<T>(5, 2);
                mBlend[1][3] = C_<T>(3, 2);
                mBlend[2][0] = C_<T>(0);
                mBlend[2][1] = C_<T>(1, 2);
                mBlend[2][2] = C_<T>(2);
                mBlend[2][3] = -C_<T>(3, 2);
                mBlend[3][0] = C_<T>(0);
                mBlend[3][1] = C_<T>(0);
                mBlend[3][2] = -C_<T>(1, 2);
                mBlend[3][3] = C_<T>(1, 2);
            }
            else
            {
                mBlend[0][0] = C_<T>(1, 6);
                mBlend[0][1] = -C_<T>(1, 2);
                mBlend[0][2] = C_<T>(1, 2);
                mBlend[0][3] = -C_<T>(1, 6);
                mBlend[1][0] = C_<T>(2, 3);
                mBlend[1][1] = C_<T>(0);
                mBlend[1][2] = -C_<T>(1);
                mBlend[1][3] = C_<T>(1, 2);
                mBlend[2][0] = C_<T>(1, 6);
                mBlend[2][1] = C_<T>(1, 2);
                mBlend[2][2] = C_<T>(1, 2);
                mBlend[2][3] = -C_<T>(1, 2);
                mBlend[3][0] = C_<T>(0);
                mBlend[3][1] = C_<T>(0);
                mBlend[3][2] = C_<T>(0);
                mBlend[3][3] = C_<T>(1, 6);
            }
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
        // argument is the order of the y-derivative and the zOrder argument
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
            std::array<T, 4> U{};
            T dx{}, xMultiplier{};
            switch (xOrder)
            {
            case 0:
                dx = xIndex - static_cast<T>(ix);
                U[0] = C_<T>(1);
                U[1] = dx;
                U[2] = dx * U[1];
                U[3] = dx * U[2];
                xMultiplier = C_<T>(1);
                break;
            case 1:
                dx = xIndex - static_cast<T>(ix);
                U[0] = C_<T>(0);
                U[1] = C_<T>(1);
                U[2] = C_<T>(2) * dx;
                U[3] = C_<T>(3) * dx * dx;
                xMultiplier = mInvXSpacing;
                break;
            case 2:
                dx = xIndex - static_cast<T>(ix);
                U[0] = C_<T>(0);
                U[1] = C_<T>(0);
                U[2] = C_<T>(2);
                U[3] = C_<T>(6) * dx;
                xMultiplier = mInvXSpacing * mInvXSpacing;
                break;
            case 3:
                U[0] = C_<T>(0);
                U[1] = C_<T>(0);
                U[2] = C_<T>(0);
                U[3] = C_<T>(6);
                xMultiplier = mInvXSpacing * mInvXSpacing * mInvXSpacing;
                break;
            default:
                return C_<T>(0);
            }

            // Compute V.
            std::array<T, 4> V{};
            T dy{}, yMultiplier{};
            switch (yOrder)
            {
            case 0:
                dy = yIndex - static_cast<T>(iy);
                V[0] = C_<T>(1);
                V[1] = dy;
                V[2] = dy * V[1];
                V[3] = dy * V[2];
                yMultiplier = C_<T>(1);
                break;
            case 1:
                dy = yIndex - static_cast<T>(iy);
                V[0] = C_<T>(0);
                V[1] = C_<T>(1);
                V[2] = C_<T>(2) * dy;
                V[3] = C_<T>(3) * dy * dy;
                yMultiplier = mInvYSpacing;
                break;
            case 2:
                dy = yIndex - static_cast<T>(iy);
                V[0] = C_<T>(0);
                V[1] = C_<T>(0);
                V[2] = C_<T>(2);
                V[3] = C_<T>(6) * dy;
                yMultiplier = mInvYSpacing * mInvYSpacing;
                break;
            case 3:
                V[0] = C_<T>(0);
                V[1] = C_<T>(0);
                V[2] = C_<T>(0);
                V[3] = C_<T>(6);
                yMultiplier = mInvYSpacing * mInvYSpacing * mInvYSpacing;
                break;
            default:
                return C_<T>(0);
            }

            // Compute W.
            std::array<T, 4> W{};
            T dz{}, zMultiplier{};
            switch (zOrder)
            {
            case 0:
                dz = zIndex - static_cast<T>(iz);
                W[0] = C_<T>(1);
                W[1] = dz;
                W[2] = dz * W[1];
                W[3] = dz * W[2];
                zMultiplier = C_<T>(1);
                break;
            case 1:
                dz = zIndex - static_cast<T>(iz);
                W[0] = C_<T>(0);
                W[1] = C_<T>(1);
                W[2] = C_<T>(2) * dz;
                W[3] = C_<T>(3) * dz * dz;
                zMultiplier = mInvZSpacing;
                break;
            case 2:
                dz = zIndex - static_cast<T>(iz);
                W[0] = C_<T>(0);
                W[1] = C_<T>(0);
                W[2] = C_<T>(2);
                W[3] = C_<T>(6) * dz;
                zMultiplier = mInvZSpacing * mInvZSpacing;
                break;
            case 3:
                W[0] = C_<T>(0);
                W[1] = C_<T>(0);
                W[2] = C_<T>(0);
                W[3] = C_<T>(6);
                zMultiplier = mInvZSpacing * mInvZSpacing * mInvZSpacing;
                break;
            default:
                return C_<T>(0);
            }

            // Compute P = M*U, Q = M*V, and R = M*W.
            std::array<T, 4> P{}, Q{}, R{};
            for (std::size_t row = 0; row < 4; ++row)
            {
                P[row] = C_<T>(0);
                Q[row] = C_<T>(0);
                R[row] = C_<T>(0);
                for (std::size_t col = 0; col < 4; ++col)
                {
                    P[row] += mBlend[row][col] * U[col];
                    Q[row] += mBlend[row][col] * V[col];
                    R[row] += mBlend[row][col] * W[col];
                }
            }

            // Compute the tensor product (M*U)(M*V)(M*W)*D where D is the
            // 4x4x4 subimage containing (x,y,z). The handling of the signs of
            // ix, iy and iz (all std::size_t) is correct because of the rules for
            // incrementing and decrementing unsigned-integer types.
            --ix;  // -1 <= ix <= mXBound - 2
            --iy;  // -1 <= iy <= mYBound - 2
            --iz;  // -1 <= iz <= mZBound - 2
            T result = C_<T>(0);
            for (std::size_t slice = 0; slice < 4; ++slice)
            {
                std::size_t zClamp{};
                if (slice > 0)
                {
                    zClamp = iz + slice;
                    if (zClamp >= mZBound)
                    {
                        zClamp = mZBound - 1;
                    }
                }
                else
                {
                    zClamp = 0;
                }

                for (std::size_t row = 0; row < 4; ++row)
                {
                    std::size_t yClamp{};
                    if (row > 0)
                    {
                        yClamp = iy + row;
                        if (yClamp >= mYBound)
                        {
                            yClamp = mYBound - 1;
                        }
                    }
                    else
                    {
                        yClamp = 0;
                    }

                    for (std::size_t col = 0; col < 4; ++col)
                    {
                        std::size_t xClamp{};
                        if (col > 0)
                        {
                            xClamp = ix + col;
                            if (xClamp >= mXBound)
                            {
                                xClamp = mXBound - 1;
                            }
                        }
                        else
                        {
                            xClamp = 0;
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
        std::array<std::array<T, 4>, 4> mBlend;

    private:
        friend class UnitTestIntpTricubic3;
    };
}
