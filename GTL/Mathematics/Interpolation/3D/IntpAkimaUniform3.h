// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.14

#pragma once

// The interpolator is for uniformly spaced(x,y,z)-values. The input samples
// must be stored in lexicographical order to represent f(x,y,z); that is,
// F(c + xBound*(r + yBound*s)] corresponds to f(x,y,z), where c is the index
// corresponding to x, r is the index corresponding to y, and s is the index
// corresponding to z.

#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Utility/Exceptions.h>
#include <GTL/Utility/Multiarray.h>
#include <GTL/Utility/MultiarrayAdapter.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class IntpAkimaUniform3
    {
    public:
        // Create the polynomial storage in lexicographical order.
        IntpAkimaUniform3(
            std::size_t xBound, T const& xMin, T const& xMax,
            std::size_t yBound, T const& yMin, T const& yMax,
            std::size_t zBound, T const& zMin, T const& zMax,
            T const* F)
            :
            mNumF(xBound * yBound * zBound),
            mF(F),
            mBound{ xBound, yBound, zBound },
            mMin{ xMin, yMin, zMin },
            mMax{ xMax, yMax, zMax },
            mDelta{
                (xMax - xMin) / static_cast<T>(xBound - 1),
                (yMax - yMin) / static_cast<T>(yBound - 1),
                (zMax - zMin) / static_cast<T>(zBound - 1),
            },
            mPoly{ xBound - 1, yBound - 1, zBound - 1}
        {
            // At least a 3x3x3 block of data points is needed to construct
            // the estimates of the boundary derivatives.
            GTL_ARGUMENT_ASSERT(
                mF != nullptr &&
                mBound[0] >= 3 && mMin[0] < mMax[0] &&
                mBound[1] >= 3 && mMin[1] < mMax[1] &&
                mBound[2] >= 3 && mMin[2] < mMax[2],
                "Invalid input.");

            // Create a lexicographical-order accessor for the function
            // samples.
            MultiarrayAdapter<T, true> Fmap({ mBound[0], mBound[1], mBound[2] }, const_cast<T*>(mF));

            // Construct first-order derivatives.
            Multiarray<T, true> FX{ mBound[0], mBound[1], mBound[2] };
            Multiarray<T, true> FY{ mBound[0], mBound[1], mBound[2] };
            Multiarray<T, true> FZ{ mBound[0], mBound[1], mBound[2] };
            GetFX(Fmap, FX);
            GetFX(Fmap, FY);
            GetFX(Fmap, FZ);

            // Construct second-order derivatives.
            Multiarray<T, true> FXY{ mBound[0], mBound[1], mBound[2] };
            Multiarray<T, true> FXZ{ mBound[0], mBound[1], mBound[2] };
            Multiarray<T, true> FYZ{ mBound[0], mBound[1], mBound[2] };
            GetFX(Fmap, FXY);
            GetFX(Fmap, FXZ);
            GetFX(Fmap, FYZ);

            // Construct third-order derivatives.
            Multiarray<T, true> FXYZ{ mBound[0], mBound[1], mBound[2] };
            GetFXYZ(Fmap, FXYZ);

            // Construct polynomials.
            GetPolynomials(Fmap, FX, FY, FZ, FXY, FXZ, FYZ, FXYZ);
        }

        ~IntpAkimaUniform3() = default;

        // Member access.
        inline std::size_t GetNumF() const
        {
            return mNumF;
        }

        inline T const* GetF() const
        {
            return mF;
        }

        inline std::size_t GetBound(std::size_t i) const
        {
            return mBound[i];
        }

        inline T const& GetMin(std::size_t i) const
        {
            return mMin[i];
        }

        inline T const& GetMax(std::size_t i) const
        {
            return mMax[i];
        }

        inline T const& GetDelta(std::size_t i) const
        {
            return mDelta[i];
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
            T xClamped = std::min(std::max(x, mMin[0]), mMax[0]);
            T yClamped = std::min(std::max(y, mMin[1]), mMax[1]);
            T zClamped = std::min(std::max(z, mMin[2]), mMax[2]);
            std::size_t ix{}, iy{}, iz{};
            T dx{}, dy{}, dz{};
            Lookup(0, xClamped, ix, dx);
            Lookup(1, yClamped, iy, dy);
            Lookup(2, zClamped, iz, dz);
            return mPoly(ix, iy, iz)(dx, dy, dz);
        }

        T operator()(std::size_t xOrder, std::size_t yOrder, std::size_t zOrder, T const& x, T const& y, T const& z) const
        {
            T xClamped = std::min(std::max(x, mMin[0]), mMax[0]);
            T yClamped = std::min(std::max(y, mMin[1]), mMax[1]);
            T zClamped = std::min(std::max(z, mMin[2]), mMax[2]);
            std::size_t ix{}, iy{}, iz{};
            T dx{}, dy{}, dz{};
            Lookup(0, xClamped, ix, dx);
            Lookup(1, yClamped, iy, dy);
            Lookup(2, zClamped, iz, dz);
            return mPoly(ix, iy, iz)(xOrder, yOrder, zOrder, dx, dy, dz);
        }

    private:
        class Polynomial
        {
        public:
            Polynomial()
                :
                mC{}
            {
                for (std::size_t x = 0; x < 4; ++x)
                {
                    for (std::size_t y = 0; y < 4; ++y)
                    {
                        mC[x][y].fill(C_<T>(0));
                    }
                }
            }

            // P(x,y,z) =
            //   sum_{i=0}^3 sum_{j=0}^3 sum_{k=0}^3 a_{ijk} x^i y^j z^k.
            // The multiindexed term A[x][y][iz] corresponds to the polynomial
            // term x^{x} y^{y} z^{iz}.
            T& A(std::size_t x, std::size_t y, std::size_t iz)
            {
                return mC[x][y][iz];
            }

            T operator()(T const& x, T const& y, T const& z) const
            {
                std::array<T, 4> xPow = { C_<T>(1), x, x * x, x * x * x };
                std::array<T, 4> yPow = { C_<T>(1), y, y * y, y * y * y };
                std::array<T, 4> zPow = { C_<T>(1), z, z * z, z * z * z };

                T p = C_<T>(0);
                for (std::size_t iz = 0; iz <= 3; ++iz)
                {
                    for (std::size_t iy = 0; iy <= 3; ++iy)
                    {
                        for (std::size_t ix = 0; ix <= 3; ++ix)
                        {
                            p += mC[ix][iy][iz] * xPow[ix] * yPow[iy] * zPow[iz];
                        }
                    }
                }
                return p;
            }

            T operator()(std::size_t xOrder, std::size_t yOrder, std::size_t zOrder, T const& x, T const& y, T const& z) const
            {
                std::array<T, 4> xPow{};
                if (!ComputePowers(xOrder, xPow, x))
                {
                    return C_<T>(0);
                }

                std::array<T, 4> yPow{};
                if (!ComputePowers(yOrder, yPow, y))
                {
                    return C_<T>(0);
                }

                std::array<T, 4> zPow{};
                if (!ComputePowers(zOrder, zPow, z))
                {
                    return C_<T>(0);
                }

                T p = C_<T>(0);
                for (std::size_t iz = 0; iz <= 3; ++iz)
                {
                    for (std::size_t iy = 0; iy <= 3; ++iy)
                    {
                        for (std::size_t ix = 0; ix <= 3; ++ix)
                        {
                            p += mC[ix][iy][iz] * xPow[ix] * yPow[iy] * zPow[iz];
                        }
                    }
                }

                return p;
            }

        private:
            // The return value is 'true' if power[] is computed; otherwise,
            // the caller should return 0 as the polynomial evaluation.
            bool ComputePowers(std::size_t order, std::array<T, 4>& power, T const& input) const
            {
                switch (order)
                {
                case 0:
                    power[0] = C_<T>(1);
                    power[1] = input;
                    power[2] = input * input;
                    power[3] = input * input * input;
                    break;
                case 1:
                    power[0] = C_<T>(0);
                    power[1] = C_<T>(1);
                    power[2] = C_<T>(2) * input;
                    power[3] = C_<T>(3) * input * input;
                    break;
                case 2:
                    power[0] = C_<T>(0);
                    power[1] = C_<T>(0);
                    power[2] = C_<T>(2);
                    power[3] = C_<T>(6) * input;
                    break;
                case 3:
                    power[0] = C_<T>(0);
                    power[1] = C_<T>(0);
                    power[2] = C_<T>(0);
                    power[3] = C_<T>(6);
                    break;
                default:
                    return false;
                }
                return true;
            }

            std::array<std::array<std::array<T, 4>, 4>, 4> mC;
        };

        // Support for construction.
        void GetFX(MultiarrayAdapter<T, true> const& F, Multiarray<T, true>& FX)
        {
            std::size_t const numXm1 = mBound[0] - 1;
            std::size_t const numXp1 = mBound[0] + 1;
            std::size_t const numXp2 = mBound[0] + 2;
            std::size_t const numXp3 = mBound[0] + 3;

            Multiarray<T, true> slope{ numXp3, mBound[1], mBound[2] };
            for (std::size_t z = 0; z < mBound[2]; ++z)
            {
                for (std::size_t y = 0; y < mBound[1]; ++y)
                {
                    for (std::size_t x = 0, xp1 = 1, xp2 = 2; xp1 < mBound[0]; x = xp1, xp1 = xp2++)
                    {
                        slope(xp2, y, z) = (F(xp1, y, z) - F(x, y, z)) / mDelta[0];
                    }

                    slope(1, y, z) = C_<T>(2) * slope(2, y, z) - slope(3, y, z);
                    slope(0, y, z) = C_<T>(2) * slope(1, y, z) - slope(2, y, z);
                    slope(numXp1, y, z) = C_<T>(2) * slope(mBound[0], y, z) - slope(numXm1, y, z);
                    slope(numXp2, y, z) = C_<T>(2) * slope(numXp1, y, z) - slope(mBound[0], y, z);
                }
            }

            for (std::size_t z = 0; z < mBound[2]; ++z)
            {
                for (std::size_t y = 0; y < mBound[1]; ++y)
                {
                    for (std::size_t x = 0; x < mBound[0]; ++x)
                    {
                        FX(x, y, z) = ComputeDerivative(&slope(x, y, z));
                    }
                }
            }
        }

        void GetFY(MultiarrayAdapter<T, true> const& F, Multiarray<T, true>& FY)
        {
            std::size_t const numYm1 = mBound[1] - 1;
            std::size_t const numYp1 = mBound[1] + 1;
            std::size_t const numYp2 = mBound[1] + 2;
            std::size_t const numYp3 = mBound[1] + 3;

            Multiarray<T, true> slope{ numYp3, mBound[2], mBound[0] };
            for (std::size_t x = 0; x < mBound[0]; ++x)
            {
                for (std::size_t z = 0; z < mBound[2]; ++z)
                {
                    for (std::size_t y = 0, yp1 = 1, yp2 = 2; yp1 < mBound[1]; y = yp1, yp1 = yp2++)
                    {
                        slope(yp2, z, x) = (F(x, yp1, z) - F(x, y, z)) / mDelta[1];
                    }

                    slope(1, z, x) = C_<T>(2) * slope(2, z, x) - slope(3, z, x);
                    slope(0, z, x) = C_<T>(2) * slope(1, z, x) - slope(2, z, x);
                    slope(numYp1, z, x) = C_<T>(2) * slope(mBound[1], z, x) - slope(numYm1, z, x);
                    slope(numYp2, z, x) = C_<T>(2) * slope(numYp1, z, x) - slope(mBound[1], z, x);
                }
            }

            for (std::size_t z = 0; z < mBound[2]; ++z)
            {
                for (std::size_t y = 0; y < mBound[1]; ++y)
                {
                    for (std::size_t x = 0; x < mBound[0]; ++x)
                    {
                        FY(x, y, z) = ComputeDerivative(&slope(y, z, x));
                    }
                }
            }
        }

        void GetFZ(MultiarrayAdapter<T, true> const& F, Multiarray<T, true>& FZ)
        {
            std::size_t const numZm1 = mBound[2] - 1;
            std::size_t const numZp1 = mBound[2] + 1;
            std::size_t const numZp2 = mBound[2] + 2;
            std::size_t const numZp3 = mBound[2] + 3;

            Multiarray<T, true> slope{ numZp3, mBound[0], mBound[1] };
            for (std::size_t y = 0; y < mBound[1]; ++y)
            {
                for (std::size_t x = 0; x < mBound[0]; ++x)
                {
                    for (std::size_t z = 0, zp1 = 1, zp2 = 2; zp1 < mBound[2]; z = zp1, zp1 = zp2++)
                    {
                        slope(zp2, x, y) = (F(x, y, zp1) - F(x, y, z)) / mDelta[2];
                    }

                    slope(1, x, y) = C_<T>(2) * slope(2, x, y) - slope(3, x, y);
                    slope(0, x, y) = C_<T>(2) * slope(1, x, y) - slope(2, x, y);
                    slope(numZp1, x, y) = C_<T>(2) * slope(mBound[2], x, y) - slope(numZm1, x, y);
                    slope(numZp2, x, y) = C_<T>(2) * slope(numZp1, x, y) - slope(mBound[2], x, y);
                }
            }

            for (std::size_t z = 0; z < mBound[2]; ++z)
            {
                for (std::size_t y = 0; y < mBound[1]; ++y)
                {
                    for (std::size_t x = 0; x < mBound[0]; ++x)
                    {
                        FZ(x, y, z) = ComputeDerivative(&slope(z, x, y));
                    }
                }
            }
        }

        void GetFXY(MultiarrayAdapter<T, true> const& F, Multiarray<T, true>& FXY)
        {
            T const multiplier = C_<T>(1, 4) / (mDelta[0] * mDelta[1]);
            std::size_t const numXm1 = mBound[0] - 1;
            std::size_t const numXm2 = mBound[0] - 2;
            std::size_t const numXm3 = mBound[0] - 3;
            std::size_t const numYm1 = mBound[1] - 1;
            std::size_t const numYm2 = mBound[1] - 2;
            std::size_t const numYm3 = mBound[1] - 3;

            for (std::size_t z = 0; z < mBound[2]; ++z)
            {
                // corners of z-slice
                FXY(0, 0, z) = multiplier * (
                    C_<T>(9) * F(0, 0, z)
                    - C_<T>(12) * (F(1, 0, z) + F(0, 1, z))
                    + C_<T>(3) * (F(2, 0, z) + F(0, 2, z))
                    + C_<T>(16) * F(1, 1, z)
                    - C_<T>(4) * (F(2, 1, z) + F(1, 2, z))
                    + F(2, 2, z));

                FXY(numXm1, 0, z) = multiplier * (
                    C_<T>(9) * F(numXm1, 0, z)
                    - C_<T>(12) * (F(numXm2, 0, z) + F(numXm1, 1, z))
                    + C_<T>(3) * (F(numXm3, 0, z) + F(numXm1, 2, z))
                    + C_<T>(16) * F(numXm2, 1, z)
                    - C_<T>(4) * (F(numXm3, 1, z) + F(numXm2, 2, z))
                    + F(numXm3, 2, z));

                FXY(0, numYm1, z) = multiplier * (
                    C_<T>(9) * F(0, numYm1, z)
                    - C_<T>(12) * (F(1, numYm1, z) + F(0, numYm2, z))
                    + C_<T>(3) * (F(2, numYm1, z) + F(0, numYm3, z))
                    + C_<T>(16) * F(1, numYm2, z)
                    - C_<T>(4) * (F(2, numYm2, z) + F(1, numYm3, z))
                    + F(2, numYm3, z));

                FXY(numXm1, numYm1, z) = multiplier * (
                    C_<T>(9) * F(numXm1, numYm1, z)
                    - C_<T>(12) * (F(numXm2, numYm1, z) + F(numXm1, numYm2, z))
                    + C_<T>(3) * (F(numXm3, numYm1, z) + F(numXm1, numYm3, z))
                    + C_<T>(16) * F(numXm2, numYm2, z)
                    - C_<T>(4) * (F(numXm3, numYm2, z) + F(numXm2, numYm3, z))
                    + F(numXm3, numYm3, z));

                // x-edges of z-slice
                for (std::size_t xm1 = 0, x = 1, xp1 = 2; x < numXm1; xm1 = x, x = xp1++)
                {
                    FXY(x, 0, z) = multiplier * (
                        C_<T>(3) * (F(xm1, 0, z) - F(xp1, 0, z))
                        - C_<T>(4) * (F(xm1, 1, z) - F(xp1, 1, z))
                        + (F(xm1, 2, z) - F(xp1, 2, z)));

                    FXY(x, numYm1, z) = multiplier * (
                        C_<T>(3) * (F(xm1, numYm1, z) - F(xp1, numYm1, z))
                        - C_<T>(4) * (F(xm1, numYm2, z) - F(xp1, numYm2, z))
                        + (F(xm1, numYm3, z) - F(xp1, numYm3, z)));
                }

                // y-edges of z-slice
                for (std::size_t ym1 = 0, y = 1, yp1 = 2; y < numYm1; ym1 = y, y = yp1++)
                {
                    FXY(0, y, z) = multiplier * (
                        C_<T>(3) * (F(0, ym1, z) - F(0, yp1, z))
                        - C_<T>(4) * (F(1, ym1, z) - F(1, yp1, z))
                        + (F(2, ym1, z) - F(2, yp1, z)));

                    FXY(numXm1, y, z) = multiplier * (
                        C_<T>(3) * (F(numXm1, ym1, z) - F(numXm1, yp1, z))
                        - C_<T>(4) * (F(numXm2, ym1, z) - F(numXm2, yp1, z))
                        + (F(numXm3, ym1, z) - F(numXm3, yp1, z)));
                }

                // interior of z-slice
                for (std::size_t ym1 = 0, y = 1, yp1 = 2; y < numYm1; ym1 = y, y = yp1++)
                {
                    for (std::size_t xm1 = 0, x = 1, xp1 = 2; x < numXm1; xm1 = x, x = xp1++)
                    {
                        FXY(x, y, z) = multiplier * (
                            F(xm1, ym1, z) - F(xp1, ym1, z) - F(xm1, yp1, z) + F(xp1, yp1, z));
                    }
                }
            }
        }

        void GetFXZ(MultiarrayAdapter<T, true> const& F, Multiarray<T, true>& FXZ)
        {
            T const multiplier = C_<T>(1, 4) / (mDelta[0] * mDelta[2]);
            std::size_t const numXm1 = mBound[0] - 1;
            std::size_t const numXm2 = mBound[0] - 2;
            std::size_t const numXm3 = mBound[0] - 3;
            std::size_t const numZm1 = mBound[2] - 1;
            std::size_t const numZm2 = mBound[2] - 2;
            std::size_t const numZm3 = mBound[2] - 3;

            for (std::size_t y = 0; y < mBound[1]; ++y)
            {
                // corners of z-slice
                FXZ(0, y, 0) = multiplier * (
                    C_<T>(9) * F(0, y, 0)
                    - C_<T>(12) * (F(1, y, 0) + F(0, y, 1))
                    + C_<T>(3) * (F(2, y, 0) + F(0, y, 2))
                    + C_<T>(16) * F(1, y, 1)
                    - C_<T>(4) * (F(2, y, 1) + F(1, y, 2))
                    + F(2, y, 2));

                FXZ(numXm1, y, 0) = multiplier * (
                    C_<T>(9) * F(numXm1, y, 0)
                    - C_<T>(12) * (F(numXm2, y, 0) + F(numXm1, y, 1))
                    + C_<T>(3) * (F(numXm3, y, 0) + F(numXm1, y, 2))
                    + C_<T>(16) * F(numXm2, y, 1)
                    - C_<T>(4) * (F(numXm3, y, 1) + F(numXm2, y, 2))
                    + F(numXm3, y, 2));

                FXZ(0, y, numZm1) = multiplier * (
                    C_<T>(9) * F(0, y, numZm1)
                    - C_<T>(12) * (F(1, y, numZm1) + F(0, y, numZm2))
                    + C_<T>(3) * (F(2, y, numZm1) + F(0, y, numZm3))
                    + C_<T>(16) * F(1, y, numZm2)
                    - C_<T>(4) * (F(2, y, numZm2) + F(1, y, numZm3))
                    + F(2, y, numZm3));

                FXZ(numXm1, y, numZm1) = multiplier * (
                    C_<T>(9) * F(numXm1, y, numZm1)
                    - C_<T>(12) * (F(numXm2, y, numZm1) + F(numXm1, y, numZm2))
                    + C_<T>(3) * (F(numXm3, y, numZm1) + F(numXm1, y, numZm3))
                    + C_<T>(16) * F(numXm2, y, numZm2)
                    - C_<T>(4) * (F(numXm3, y, numZm2) + F(numXm2, y, numZm3))
                    + F(numXm3, y, numZm3));

                // x-edges of y-slice
                for (std::size_t xm1 = 0, x = 1, xp1 = 2; x < numXm1; xm1 = x, x = xp1++)
                {
                    FXZ(x, y, 0) = multiplier * (
                        C_<T>(3) * (F(xm1, y, 0) - F(xp1, y, 0))
                        - C_<T>(4) * (F(xm1, y, 1) - F(xp1, y, 1))
                        + (F(xm1, y, 2) - F(xp1, y, 2)));

                    FXZ(x, y, numZm1) = multiplier * (
                        C_<T>(3) * (F(xm1, y, numZm1) - F(xp1, y, numZm1))
                        - C_<T>(4) * (F(xm1, y, numZm2) - F(xp1, y, numZm2))
                        + (F(xm1, y, numZm3) - F(xp1, y, numZm3)));
                }

                // z-edges of y-slice
                for (std::size_t zm1 = 0, z = 1, zp1 = 2; z < numZm1; zm1 = z, z = zp1++)
                {
                    FXZ(0, y, z) = multiplier * (
                        C_<T>(3) * (F(0, y, zm1) - F(0, y, zp1))
                        - C_<T>(4) * (F(1, y, zm1) - F(1, y, zp1))
                        + (F(2, y, zm1) - F(2, y, zp1)));

                    FXZ(numXm1, y, numZm1) = multiplier * (
                        C_<T>(3) * (F(numXm1, y, zm1) - F(numXm1, y, zp1))
                        - C_<T>(4) * (F(numXm2, y, zm1) - F(numXm1, y, zp1))
                        + (F(numXm3, y, zm1) - F(numXm3, y, zp1)));
                }

                // interior of y-slice
                for (std::size_t zm1 = 0, z = 1, zp1 = 2; z < numZm1; zm1 = z, z = zp1++)
                {
                    for (std::size_t xm1 = 0, x = 1, xp1 = 2; x < numXm1; xm1 = x, x = xp1++)
                    {
                        FXZ(x, y, z) = multiplier * (
                            F(xm1, y, zm1) - F(xp1, y, zm1) - F(xm1, y, zp1) + F(xp1, y, zp1));
                    }
                }
            }
        }

        void GetFYZ(MultiarrayAdapter<T, true> const& F, Multiarray<T, true>& FYZ)
        {
            T const multiplier = C_<T>(1, 4) / (mDelta[1] * mDelta[2]);
            std::size_t const numYm1 = mBound[1] - 1;
            std::size_t const numYm2 = mBound[1] - 2;
            std::size_t const numYm3 = mBound[1] - 3;
            std::size_t const numZm1 = mBound[2] - 1;
            std::size_t const numZm2 = mBound[2] - 2;
            std::size_t const numZm3 = mBound[2] - 3;

            for (std::size_t x = 0; x < mBound[0]; ++x)
            {
                // corners of x-slice
                FYZ(x, 0, 0) = multiplier * (
                    C_<T>(9) * F(x, 0, 0)
                    - C_<T>(12) * (F(x, 1, 0) + F(x, 0, 1))
                    + C_<T>(3) * (F(x, 2, 0) + F(x, 0, 2))
                    + C_<T>(16) * F(x, 1, 1)
                    - C_<T>(4) * (F(x, 2, 1) + F(x, 1, 2))
                    + F(x, 2, 2));

                FYZ(x, numYm1, 0) = multiplier * (
                    C_<T>(9) * F(x, numYm1, 0)
                    - C_<T>(12) * (F(x, numYm2, 0) + F(x, numYm1, 1))
                    + C_<T>(3) * (F(x, numYm3, 0) + F(x, numYm1, 2))
                    + C_<T>(16) * F(x, numYm2, 1)
                    - C_<T>(4) * (F(x, numYm3, 1) + F(x, numYm2, 2))
                    + F(x, numYm3, 2));

                FYZ(x, 0, numZm1) = multiplier * (
                    C_<T>(9) * F(x, 0, numZm1)
                    - C_<T>(12) * (F(x, 1, numZm1) + F(x, 0, numZm2))
                    + C_<T>(3) * (F(x, 2, numZm1) + F(x, 0, numZm3))
                    + C_<T>(16) * F(x, 1, numZm2)
                    - C_<T>(4) * (F(x, 2, numZm2) + F(x, 1, numZm3))
                    + F(x, 2, numZm3));

                FYZ(x, numYm1, numZm1) = multiplier * (
                    C_<T>(9) * F(x, numYm1, numZm1)
                    - C_<T>(12) * (F(x, numYm2, numZm1) + F(x, numYm1, numZm2))
                    + C_<T>(3) * (F(x, numYm3, numZm1) + F(x, numYm1, numZm3))
                    + C_<T>(16) * F(x, numYm2, numZm2)
                    - C_<T>(4) * (F(x, numYm3, numZm2) + F(x, numYm2, numZm3))
                    + F(x, numYm3, numZm3));

                // y-edges of x-slice
                for (std::size_t ym1 = 0, y = 1, yp1 = 2; y < numYm1; ym1 = y, y = yp1++)
                {
                    FYZ(x, y, 0) = multiplier * (
                        C_<T>(3) * (F(x, ym1, 0) - F(x, yp1, 0))
                        - C_<T>(4) * (F(x, ym1, 1) - F(x, yp1, 1))
                        + (F(x, ym1, 2) - F(x, yp1, 2)));

                    FYZ(x, y, numZm1) = multiplier * (
                        C_<T>(3) * (F(x, ym1, numZm1) - F(x, yp1, numZm1))
                        - C_<T>(4) * (F(x, ym1, numZm2) - F(x, yp1, numZm2))
                        + (F(x, ym1, numZm3) - F(x, yp1, numZm3)));
                }

                // z-edges of x-slice
                for (std::size_t zm1 = 0, z = 1, zp1 = 2; z < numZm1; zm1 = z, z = zp1++)
                {
                    FYZ(x, 0, z) = multiplier * (
                        C_<T>(3) * (F(x, 0, zm1) - F(x, 0, zp1))
                        - C_<T>(4) * (F(x, 1, zm1) - F(x, 1, zp1))
                        + (F(x, 2, zm1) - F(x, 2, zp1)));

                    FYZ(x, numYm1, z) = multiplier * (
                        C_<T>(3) * (F(x, numYm1, zm1) - F(x, numYm1, zp1))
                        - C_<T>(4) * (F(x, numYm2, zm1) - F(x, numYm2, zp1))
                        + (F(x, numYm3, zm1) - F(x, numYm3, zp1)));
                }

                // interior of x-slice
                for (std::size_t zm1 = 0, z = 1, zp1 = 2; z < numZm1; zm1 = z, z = zp1++)
                {
                    for (std::size_t ym1 = 0, y = 1, yp1 = 2; y < numYm1; ym1 = y, y = yp1++)
                    {
                        FYZ(x, y, z) = multiplier * (
                            F(x, ym1, zm1) - F(x, yp1, zm1) - F(x, ym1, zp1) + F(x, yp1, zp1));
                    }
                }
            }
        }

        void GetFXYZ(MultiarrayAdapter<T, true> const& F, Multiarray<T, true>& FXYZ)
        {
            T multiplier = C_<T>(1) / (mDelta[0] * mDelta[1] * mDelta[2]);
            std::size_t numXm1 = mBound[0] - 1;
            std::size_t numYm1 = mBound[1] - 1;
            std::size_t numZm1 = mBound[2] - 1;

            // convolution masks
            //   centered difference, O(h^2)
            std::array<T, 3> CDer = { C_<T>(-1, 2), C_<T>(0), C_<T>(1, 2) };
            //   one-sided difference, O(h^2)
            std::array<T, 3> ODer = { C_<T>(-3, 2), C_<T>(2), C_<T>(-1, 2) };
            T mask{};

            // corners
            FXYZ(0, 0, 0) = C_<T>(0);
            FXYZ(numXm1, 0, 0) = C_<T>(0);
            FXYZ(0, numYm1, 0) = C_<T>(0);
            FXYZ(numXm1, numYm1, 0) = C_<T>(0);
            FXYZ(0, numZm1, 0) = C_<T>(0);
            FXYZ(numXm1, 0, numZm1) = C_<T>(0);
            FXYZ(0, numYm1, numZm1) = C_<T>(0);
            FXYZ(numXm1, numYm1, numZm1) = C_<T>(0);
            for (std::size_t dz = 0; dz <= 2; ++dz)
            {
                for (std::size_t dy = 0; dy <= 2; ++dy)
                {
                    for (std::size_t dx = 0; dx <= 2; ++dx)
                    {
                        mask = multiplier * ODer[dx] * ODer[dy] * ODer[dz];
                        FXYZ(0, 0, 0) += mask * F(dx, dy, dz);
                        FXYZ(numXm1, 0, 0) += mask * F(numXm1 - dx, dy, dz);
                        FXYZ(0, numYm1, 0) += mask * F(dx, numYm1 - dy, dz);
                        FXYZ(numXm1, numYm1, 0) += mask * F(numXm1 - dx, numYm1 - dy, dz);
                        FXYZ(0, 0, numZm1) += mask * F(dx, dy, numZm1 - dz);
                        FXYZ(numXm1, 0, numZm1) += mask * F(numXm1 - dx, dy, numZm1 - dz);
                        FXYZ(0, numYm1, numZm1) += mask * F(dx, numYm1 - dy, numZm1 - dz);
                        FXYZ(numXm1, numYm1, numZm1) += mask * F(numXm1 - dx, numYm1 - dy, numZm1 - dz);
                    }
                }
            }

            // x-edges
            for (std::size_t x = 1; x < numXm1; ++x)
            {
                FXYZ(x, 0, 0) = C_<T>(0);
                FXYZ(x, numYm1, 0) = C_<T>(0);
                FXYZ(x, 0, numZm1) = C_<T>(0);
                FXYZ(x, numYm1, numZm1) = C_<T>(0);
                for (std::size_t dz = 0; dz <= 2; ++dz)
                {
                    for (std::size_t dy = 0; dy <= 2; ++dy)
                    {
                        for (std::size_t dx = 0; dx <= 2; ++dx)
                        {
                            mask = multiplier * CDer[dx] * ODer[dy] * ODer[dz];
                            FXYZ(x, 0, 0) += mask * F(x + dx - 1, dy, dz);
                            FXYZ(x, numYm1, 0) += mask * F(x + dx - 1, numYm1 - dy, dz);
                            FXYZ(x, 0, numZm1) += mask * F(x + dx - 1, dy, numZm1 - dz);
                            FXYZ(x, numYm1, numZm1) += mask * F(x + dx - 1, numYm1 - dy, numZm1 - dz);
                        }
                    }
                }
            }

            // y-edges
            for (std::size_t y = 1; y < numYm1; ++y)
            {
                FXYZ(0, y, 0) = C_<T>(0);
                FXYZ(numXm1, y, 0) = C_<T>(0);
                FXYZ(0, y, numZm1) = C_<T>(0);
                FXYZ(numXm1, y, numZm1) = C_<T>(0);
                for (std::size_t dz = 0; dz <= 2; ++dz)
                {
                    for (std::size_t dy = 0; dy <= 2; ++dy)
                    {
                        for (std::size_t dx = 0; dx <= 2; ++dx)
                        {
                            mask = multiplier * ODer[dx] * CDer[dy] * ODer[dz];
                            FXYZ(0, y, 0) += mask * F(dx, y + dy - 1, dz);
                            FXYZ(numXm1, y, 0) += mask * F(numXm1 - dx, y + dy - 1, dz);
                            FXYZ(0, y, numZm1) += mask * F(dx, y + dy - 1, numZm1 - dz);
                            FXYZ(numXm1, y, numZm1) += mask * F(numXm1 - dx, y + dy - 1, numZm1 - dz);
                        }
                    }
                }
            }

            // z-edges
            for (std::size_t z = 1; z < numZm1; ++z)
            {
                FXYZ(0, 0, z) = C_<T>(0);
                FXYZ(numXm1, 0, z) = C_<T>(0);
                FXYZ(0, numYm1, z) = C_<T>(0);
                FXYZ(numXm1, numYm1, z) = C_<T>(0);
                for (std::size_t dz = 0; dz <= 2; ++dz)
                {
                    for (std::size_t dy = 0; dy <= 2; ++dy)
                    {
                        for (std::size_t dx = 0; dx <= 2; ++dx)
                        {
                            mask = multiplier * ODer[dx] * ODer[dy] * CDer[dz];
                            FXYZ(0, 0, z) += mask * F(dx, dy, z + dz - 1);
                            FXYZ(numXm1, 0, z) += mask * F(numXm1 - dx, dy, z + dz - 1);
                            FXYZ(0, numYm1, z) += mask * F(dx, numYm1 - dy, z + dz - 1);
                            FXYZ(numXm1, numYm1, z) += mask * F(numXm1 - dx, numYm1 - dy, z + dz - 1);
                        }
                    }
                }
            }

            // xy-faces
            for (std::size_t y = 1; y < numYm1; ++y)
            {
                for (std::size_t x = 1; x < numXm1; ++x)
                {
                    FXYZ(x, y, 0) = C_<T>(0);
                    FXYZ(x, y, numZm1) = C_<T>(0);
                    for (std::size_t dz = 0; dz <= 2; ++dz)
                    {
                        for (std::size_t dy = 0; dy <= 2; ++dy)
                        {
                            for (std::size_t dx = 0; dx <= 2; ++dx)
                            {
                                mask = multiplier * CDer[dx] * CDer[dy] * ODer[dz];
                                FXYZ(x, y, 0) += mask * F(x + dx - 1, y + dy - 1, dz);
                                FXYZ(x, y, numZm1) += mask * F(x + dx - 1, y + dy - 1, numZm1 - dz);
                            }
                        }
                    }
                }
            }

            // xz-faces
            for (std::size_t z = 1; z < numZm1; ++z)
            {
                for (std::size_t x = 1; x < numXm1; ++x)
                {
                    FXYZ(x, 0, z) = C_<T>(0);
                    FXYZ(x, numYm1, z) = C_<T>(0);
                    for (std::size_t dz = 0; dz <= 2; ++dz)
                    {
                        for (std::size_t dy = 0; dy <= 2; ++dy)
                        {
                            for (std::size_t dx = 0; dx <= 2; ++dx)
                            {
                                mask = multiplier * CDer[dx] * ODer[dy] * CDer[dz];
                                FXYZ(x, 0, z) += mask * F(x + dx - 1, dy, z + dz - 1);
                                FXYZ(x, numYm1, z) += mask * F(x + dx - 1, numYm1 - dy, z + dz - 1);
                            }
                        }
                    }
                }
            }

            // yz-faces
            for (std::size_t z = 1; z < numZm1; ++z)
            {
                for (std::size_t y = 1; y < numYm1; ++y)
                {
                    FXYZ(0, y, z) = C_<T>(0);
                    FXYZ(numXm1, y, z) = C_<T>(0);
                    for (std::size_t dz = 0; dz <= 2; ++dz)
                    {
                        for (std::size_t dy = 0; dy <= 2; ++dy)
                        {
                            for (std::size_t dx = 0; dx <= 2; ++dx)
                            {
                                mask = multiplier * ODer[dx] * CDer[dy] * CDer[dz];
                                FXYZ(0, y, z) += mask * F(dx, y + dy - 1, z + dz - 1);
                                FXYZ(numXm1, y, z) += mask * F(numXm1 - dx, y + dy - 1, z + dz - 1);
                            }
                        }
                    }
                }
            }

            // interiors
            for (std::size_t z = 1; z < numZm1; ++z)
            {
                for (std::size_t y = 1; y < numYm1; ++y)
                {
                    for (std::size_t x = 1; x < numXm1; ++x)
                    {
                        FXYZ(x, y, z) = C_<T>(0);

                        for (std::size_t dz = 0; dz <= 2; ++dz)
                        {
                            for (std::size_t dy = 0; dy <= 2; ++dy)
                            {
                                for (std::size_t dx = 0; dx <= 2; ++dx)
                                {
                                    mask = multiplier * CDer[dx] * CDer[dy] * CDer[dz];
                                    FXYZ(x, y, z) += mask * F(x + dx - 1, y + dy - 1, z + dz - 1);
                                }
                            }
                        }
                    }
                }
            }
        }

        void GetPolynomials(
            MultiarrayAdapter<T, true> const& F,
            Multiarray<T, true> const& FX,
            Multiarray<T, true> const& FY,
            Multiarray<T, true> const& FZ,
            Multiarray<T, true> const& FXY,
            Multiarray<T, true> const& FXZ,
            Multiarray<T, true> const& FYZ,
            Multiarray<T, true> const& FXYZ)
        {
            // Note the 'transposing' of the 2x2x2 blocks (to match notation
            // used in the polynomial definition).
            std::array<std::array<std::array<T, 2>, 2>, 2> G{}, GX{}, GY{}, GZ{};
            std::array<std::array<std::array<T, 2>, 2>, 2> GXY{}, GXZ{}, GYZ{}, GXYZ{};

            for (std::size_t z = 0, zp1 = 1; zp1 < mBound[2]; z = zp1++)
            {
                for (std::size_t y = 0, yp1 = 1; yp1 < mBound[1]; y = yp1++)
                {
                    for (std::size_t x = 0, xp1 = 1; xp1 < mBound[0]; x = xp1++)
                    {
                        G[0][0][0] = F(x, y, z);
                        G[0][0][1] = F(x, y, zp1);
                        G[0][1][0] = F(x, yp1, z);
                        G[0][1][1] = F(x, yp1, zp1);
                        G[1][0][0] = F(xp1, y, z);
                        G[1][0][1] = F(xp1, y, zp1);
                        G[1][1][0] = F(xp1, yp1, z);
                        G[1][1][1] = F(xp1, yp1, zp1);

                        GX[0][0][0] = FX(x, y, z);
                        GX[0][0][1] = FX(x, y, zp1);
                        GX[0][1][0] = FX(x, yp1, z);
                        GX[0][1][1] = FX(x, yp1, zp1);
                        GX[1][0][0] = FX(xp1, y, z);
                        GX[1][0][1] = FX(xp1, y, zp1);
                        GX[1][1][0] = FX(xp1, yp1, z);
                        GX[1][1][1] = FX(xp1, yp1, zp1);

                        GY[0][0][0] = FY(x, y, z);
                        GY[0][0][1] = FY(x, y, zp1);
                        GY[0][1][0] = FY(x, yp1, z);
                        GY[0][1][1] = FY(x, yp1, zp1);
                        GY[1][0][0] = FY(xp1, y, z);
                        GY[1][0][1] = FY(xp1, y, zp1);
                        GY[1][1][0] = FY(xp1, yp1, z);
                        GY[1][1][1] = FY(xp1, yp1, zp1);

                        GZ[0][0][0] = FZ(x, y, z);
                        GZ[0][0][1] = FZ(x, y, zp1);
                        GZ[0][1][0] = FZ(x, yp1, z);
                        GZ[0][1][1] = FZ(x, yp1, zp1);
                        GZ[1][0][0] = FZ(xp1, y, z);
                        GZ[1][0][1] = FZ(xp1, y, zp1);
                        GZ[1][1][0] = FZ(xp1, yp1, z);
                        GZ[1][1][1] = FZ(xp1, yp1, zp1);

                        GXY[0][0][0] = FXY(x, y, z);
                        GXY[0][0][1] = FXY(x, y, zp1);
                        GXY[0][1][0] = FXY(x, yp1, z);
                        GXY[0][1][1] = FXY(x, yp1, zp1);
                        GXY[1][0][0] = FXY(xp1, y, z);
                        GXY[1][0][1] = FXY(xp1, y, zp1);
                        GXY[1][1][0] = FXY(xp1, yp1, z);
                        GXY[1][1][1] = FXY(xp1, yp1, zp1);

                        GXZ[0][0][0] = FXZ(x, y, z);
                        GXZ[0][0][1] = FXZ(x, y, zp1);
                        GXZ[0][1][0] = FXZ(x, yp1, z);
                        GXZ[0][1][1] = FXZ(x, yp1, zp1);
                        GXZ[1][0][0] = FXZ(xp1, y, z);
                        GXZ[1][0][1] = FXZ(xp1, y, zp1);
                        GXZ[1][1][0] = FXZ(xp1, yp1, z);
                        GXZ[1][1][1] = FXZ(xp1, yp1, zp1);

                        GYZ[0][0][0] = FYZ(x, y, z);
                        GYZ[0][0][1] = FYZ(x, y, zp1);
                        GYZ[0][1][0] = FYZ(x, yp1, z);
                        GYZ[0][1][1] = FYZ(x, yp1, zp1);
                        GYZ[1][0][0] = FYZ(xp1, y, z);
                        GYZ[1][0][1] = FYZ(xp1, y, zp1);
                        GYZ[1][1][0] = FYZ(xp1, yp1, z);
                        GYZ[1][1][1] = FYZ(xp1, yp1, zp1);

                        GXYZ[0][0][0] = FXYZ(x, y, z);
                        GXYZ[0][0][1] = FXYZ(x, y, zp1);
                        GXYZ[0][1][0] = FXYZ(x, yp1, z);
                        GXYZ[0][1][1] = FXYZ(x, yp1, zp1);
                        GXYZ[1][0][0] = FXYZ(xp1, y, z);
                        GXYZ[1][0][1] = FXYZ(xp1, y, zp1);
                        GXYZ[1][1][0] = FXYZ(xp1, yp1, z);
                        GXYZ[1][1][1] = FXYZ(xp1, yp1, zp1);

                        Construct(mPoly(x, y, z), G, GX, GY, GZ, GXY, GXZ, GYZ, GXYZ);
                    }
                }
            }
        }

        T ComputeDerivative(T const* slope) const
        {
            if (slope[1] != slope[2])
            {
                if (slope[0] != slope[1])
                {
                    if (slope[2] != slope[3])
                    {
                        T ad0 = std::fabs(slope[3] - slope[2]);
                        T ad1 = std::fabs(slope[0] - slope[1]);
                        return (ad0 * slope[1] + ad1 * slope[2]) / (ad0 + ad1);
                    }
                    else
                    {
                        return slope[2];
                    }
                }
                else
                {
                    if (slope[2] != slope[3])
                    {
                        return slope[1];
                    }
                    else
                    {
                        return C_<T>(1, 2) * (slope[1] + slope[2]);
                    }
                }
            }
            else
            {
                return slope[1];
            }
        }

        void Construct(Polynomial& poly,
            std::array<std::array<std::array<T, 2>, 2>, 2> const& F,
            std::array<std::array<std::array<T, 2>, 2>, 2> const& FX,
            std::array<std::array<std::array<T, 2>, 2>, 2> const& FY,
            std::array<std::array<std::array<T, 2>, 2>, 2> const& FZ,
            std::array<std::array<std::array<T, 2>, 2>, 2> const& FXY,
            std::array<std::array<std::array<T, 2>, 2>, 2> const& FXZ,
            std::array<std::array<std::array<T, 2>, 2>, 2> const& FYZ,
            std::array<std::array<std::array<T, 2>, 2>, 2> const& FXYZ)
        {
            T dx = mDelta[0];
            T dy = mDelta[1];
            T dz = mDelta[2];
            T invDX = C_<T>(1) / dx, invDX2 = invDX * invDX;
            T invDY = C_<T>(1) / dy, invDY2 = invDY * invDY;
            T invDZ = C_<T>(1) / dz, invDZ2 = invDZ * invDZ;
            T b0{}, b1{}, b2{}, b3{}, b4{}, b5{}, b6{}, b7{};

            poly.A(0, 0, 0) = F[0][0][0];
            poly.A(1, 0, 0) = FX[0][0][0];
            poly.A(0, 1, 0) = FY[0][0][0];
            poly.A(0, 0, 1) = FZ[0][0][0];
            poly.A(1, 1, 0) = FXY[0][0][0];
            poly.A(1, 0, 1) = FXZ[0][0][0];
            poly.A(0, 1, 1) = FYZ[0][0][0];
            poly.A(1, 1, 1) = FXYZ[0][0][0];

            // solve for Aij0
            b0 = (F[1][0][0] - poly(0, 0, 0, dx, C_<T>(0), C_<T>(0))) * invDX2;
            b1 = (FX[1][0][0] - poly(1, 0, 0, dx, C_<T>(0), C_<T>(0))) * invDX;
            poly.A(2, 0, 0) = C_<T>(3) * b0 - b1;
            poly.A(3, 0, 0) = (C_<T>(-2) * b0 + b1) * invDX;

            b0 = (F[0][1][0] - poly(0, 0, 0, C_<T>(0), dy, C_<T>(0))) * invDY2;
            b1 = (FY[0][1][0] - poly(0, 1, 0, C_<T>(0), dy, C_<T>(0))) * invDY;
            poly.A(0, 2, 0) = C_<T>(3) * b0 - b1;
            poly.A(0, 3, 0) = (C_<T>(-2) * b0 + b1) * invDY;

            b0 = (FY[1][0][0] - poly(0, 1, 0, dx, C_<T>(0), C_<T>(0))) * invDX2;
            b1 = (FXY[1][0][0] - poly(1, 1, 0, dx, C_<T>(0), C_<T>(0))) * invDX;
            poly.A(2, 1, 0) = C_<T>(3) * b0 - b1;
            poly.A(3, 1, 0) = (C_<T>(-2) * b0 + b1) * invDX;

            b0 = (FX[0][1][0] - poly(1, 0, 0, C_<T>(0), dy, C_<T>(0))) * invDY2;
            b1 = (FXY[0][1][0] - poly(1, 1, 0, C_<T>(0), dy, C_<T>(0))) * invDY;
            poly.A(1, 2, 0) = C_<T>(3) * b0 - b1;
            poly.A(1, 3, 0) = (C_<T>(-2) * b0 + b1) * invDY;

            b0 = (F[1][1][0] - poly(0, 0, 0, dx, dy, C_<T>(0))) * invDX2 * invDY2;
            b1 = (FX[1][1][0] - poly(1, 0, 0, dx, dy, C_<T>(0))) * invDX * invDY2;
            b2 = (FY[1][1][0] - poly(0, 1, 0, dx, dy, C_<T>(0))) * invDX2 * invDY;
            b3 = (FXY[1][1][0] - poly(1, 1, 0, dx, dy, C_<T>(0))) * invDX * invDY;
            poly.A(2, 2, 0) = C_<T>(9) * b0 - C_<T>(3) * b1 - C_<T>(3) * b2 + b3;
            poly.A(3, 2, 0) = (C_<T>(-6) * b0 + C_<T>(3) * b1 + C_<T>(2) * b2 - b3) * invDX;
            poly.A(2, 3, 0) = (C_<T>(-6) * b0 + C_<T>(2) * b1 + C_<T>(3) * b2 - b3) * invDY;
            poly.A(3, 3, 0) = (C_<T>(4) * b0 - C_<T>(2) * b1 - C_<T>(2) * b2 + b3) * invDX * invDY;

            // solve for Ai0k
            b0 = (F[0][0][1] - poly(0, 0, 0, C_<T>(0), C_<T>(0), dz)) * invDZ2;
            b1 = (FZ[0][0][1] - poly(0, 0, 1, C_<T>(0), C_<T>(0), dz)) * invDZ;
            poly.A(0, 0, 2) = C_<T>(3) * b0 - b1;
            poly.A(0, 0, 3) = (C_<T>(-2) * b0 + b1) * invDZ;

            b0 = (FZ[1][0][0] - poly(0, 0, 1, dx, C_<T>(0), C_<T>(0))) * invDX2;
            b1 = (FXZ[1][0][0] - poly(1, 0, 1, dx, C_<T>(0), C_<T>(0))) * invDX;
            poly.A(2, 0, 1) = C_<T>(3) * b0 - b1;
            poly.A(3, 0, 1) = (C_<T>(-2) * b0 + b1) * invDX;

            b0 = (FX[0][0][1] - poly(1, 0, 0, C_<T>(0), C_<T>(0), dz)) * invDZ2;
            b1 = (FXZ[0][0][1] - poly(1, 0, 1, C_<T>(0), C_<T>(0), dz)) * invDZ;
            poly.A(1, 0, 2) = C_<T>(3) * b0 - b1;
            poly.A(1, 0, 3) = (C_<T>(-2) * b0 + b1) * invDZ;

            b0 = (F[1][0][1] - poly(0, 0, 0, dx, C_<T>(0), dz)) * invDX2 * invDZ2;
            b1 = (FX[1][0][1] - poly(1, 0, 0, dx, C_<T>(0), dz)) * invDX * invDZ2;
            b2 = (FZ[1][0][1] - poly(0, 0, 1, dx, C_<T>(0), dz)) * invDX2 * invDZ;
            b3 = (FXZ[1][0][1] - poly(1, 0, 1, dx, C_<T>(0), dz)) * invDX * invDZ;
            poly.A(2, 0, 2) = C_<T>(9) * b0 - C_<T>(3) * b1 - C_<T>(3) * b2 + b3;
            poly.A(3, 0, 2) = (C_<T>(-6) * b0 + C_<T>(3) * b1 + C_<T>(2) * b2 - b3) * invDX;
            poly.A(2, 0, 3) = (C_<T>(-6) * b0 + C_<T>(2) * b1 + C_<T>(3) * b2 - b3) * invDZ;
            poly.A(3, 0, 3) = (C_<T>(4) * b0 - C_<T>(2) * b1 - C_<T>(2) * b2 + b3) * invDX * invDZ;

            // solve for A0jk
            b0 = (FZ[0][1][0] - poly(0, 0, 1, C_<T>(0), dy, C_<T>(0))) * invDY2;
            b1 = (FYZ[0][1][0] - poly(0, 1, 1, C_<T>(0), dy, C_<T>(0))) * invDY;
            poly.A(0, 2, 1) = C_<T>(3) * b0 - b1;
            poly.A(0, 3, 1) = (C_<T>(-2) * b0 + b1) * invDY;

            b0 = (FY[0][0][1] - poly(0, 1, 0, C_<T>(0), C_<T>(0), dz)) * invDZ2;
            b1 = (FYZ[0][0][1] - poly(0, 1, 1, C_<T>(0), C_<T>(0), dz)) * invDZ;
            poly.A(0, 1, 2) = C_<T>(3) * b0 - b1;
            poly.A(0, 1, 3) = (C_<T>(-2) * b0 + b1) * invDZ;

            b0 = (F[0][1][1] - poly(0, 0, 0, C_<T>(0), dy, dz)) * invDY2 * invDZ2;
            b1 = (FY[0][1][1] - poly(0, 1, 0, C_<T>(0), dy, dz)) * invDY * invDZ2;
            b2 = (FZ[0][1][1] - poly(0, 0, 1, C_<T>(0), dy, dz)) * invDY2 * invDZ;
            b3 = (FYZ[0][1][1] - poly(0, 1, 1, C_<T>(0), dy, dz)) * invDY * invDZ;
            poly.A(0, 2, 2) = C_<T>(9) * b0 - C_<T>(3) * b1 - C_<T>(3) * b2 + b3;
            poly.A(0, 3, 2) = (C_<T>(-6) * b0 + C_<T>(3) * b1 + C_<T>(2) * b2 - b3) * invDY;
            poly.A(0, 2, 3) = (C_<T>(-6) * b0 + C_<T>(2) * b1 + C_<T>(3) * b2 - b3) * invDZ;
            poly.A(0, 3, 3) = (C_<T>(4) * b0 - C_<T>(2) * b1 - C_<T>(2) * b2 + b3) * invDY * invDZ;

            // solve for Aij1
            b0 = (FYZ[1][0][0] - poly(0, 1, 1, dx, C_<T>(0), C_<T>(0))) * invDX2;
            b1 = (FXYZ[1][0][0] - poly(1, 1, 1, dx, C_<T>(0), C_<T>(0))) * invDX;
            poly.A(2, 1, 1) = C_<T>(3) * b0 - b1;
            poly.A(3, 1, 1) = (C_<T>(-2) * b0 + b1) * invDX;

            b0 = (FXZ[0][1][0] - poly(1, 0, 1, C_<T>(0), dy, C_<T>(0))) * invDY2;
            b1 = (FXYZ[0][1][0] - poly(1, 1, 1, C_<T>(0), dy, C_<T>(0))) * invDY;
            poly.A(1, 2, 1) = C_<T>(3) * b0 - b1;
            poly.A(1, 3, 1) = (C_<T>(-2) * b0 + b1) * invDY;

            b0 = (FZ[1][1][0] - poly(0, 0, 1, dx, dy, C_<T>(0))) * invDX2 * invDY2;
            b1 = (FXZ[1][1][0] - poly(1, 0, 1, dx, dy, C_<T>(0))) * invDX * invDY2;
            b2 = (FYZ[1][1][0] - poly(0, 1, 1, dx, dy, C_<T>(0))) * invDX2 * invDY;
            b3 = (FXYZ[1][1][0] - poly(1, 1, 1, dx, dy, C_<T>(0))) * invDX * invDY;
            poly.A(2, 2, 1) = C_<T>(9) * b0 - C_<T>(3) * b1 - C_<T>(3) * b2 + b3;
            poly.A(3, 2, 1) = (C_<T>(-6) * b0 + C_<T>(3) * b1 + C_<T>(2) * b2 - b3) * invDX;
            poly.A(2, 3, 1) = (C_<T>(-6) * b0 + C_<T>(2) * b1 + C_<T>(3) * b2 - b3) * invDY;
            poly.A(3, 3, 1) = (C_<T>(4) * b0 - C_<T>(2) * b1 - C_<T>(2) * b2 + b3) * invDX * invDY;

            // solve for Ai1k
            b0 = (FXY[0][0][1] - poly(1, 1, 0, C_<T>(0), C_<T>(0), dz)) * invDZ2;
            b1 = (FXYZ[0][0][1] - poly(1, 1, 1, C_<T>(0), C_<T>(0), dz)) * invDZ;
            poly.A(1, 1, 2) = C_<T>(3) * b0 - b1;
            poly.A(1, 1, 3) = (C_<T>(-2) * b0 + b1) * invDZ;

            b0 = (FY[1][0][1] - poly(0, 1, 0, dx, C_<T>(0), dz)) * invDX2 * invDZ2;
            b1 = (FXY[1][0][1] - poly(1, 1, 0, dx, C_<T>(0), dz)) * invDX * invDZ2;
            b2 = (FYZ[1][0][1] - poly(0, 1, 1, dx, C_<T>(0), dz)) * invDX2 * invDZ;
            b3 = (FXYZ[1][0][1] - poly(1, 1, 1, dx, C_<T>(0), dz)) * invDX * invDZ;
            poly.A(2, 1, 2) = C_<T>(9) * b0 - C_<T>(3) * b1 - C_<T>(3) * b2 + b3;
            poly.A(3, 1, 2) = (C_<T>(-6) * b0 + C_<T>(3) * b1 + C_<T>(2) * b2 - b3) * invDX;
            poly.A(2, 1, 3) = (C_<T>(-6) * b0 + C_<T>(2) * b1 + C_<T>(3) * b2 - b3) * invDZ;
            poly.A(3, 1, 3) = (C_<T>(4) * b0 - C_<T>(2) * b1 - C_<T>(2) * b2 + b3) * invDX * invDZ;

            // solve for A1jk
            b0 = (FX[0][1][1] - poly(1, 0, 0, C_<T>(0), dy, dz)) * invDY2 * invDZ2;
            b1 = (FXY[0][1][1] - poly(1, 1, 0, C_<T>(0), dy, dz)) * invDY * invDZ2;
            b2 = (FXZ[0][1][1] - poly(1, 0, 1, C_<T>(0), dy, dz)) * invDY2 * invDZ;
            b3 = (FXYZ[0][1][1] - poly(1, 1, 1, C_<T>(0), dy, dz)) * invDY * invDZ;
            poly.A(1, 2, 2) = C_<T>(9) * b0 - C_<T>(3) * b1 - C_<T>(3) * b2 + b3;
            poly.A(1, 3, 2) = (C_<T>(-6) * b0 + C_<T>(3) * b1 + C_<T>(2) * b2 - b3) * invDY;
            poly.A(1, 2, 3) = (C_<T>(-6) * b0 + C_<T>(2) * b1 + C_<T>(3) * b2 - b3) * invDZ;
            poly.A(1, 3, 3) = (C_<T>(4) * b0 - C_<T>(2) * b1 - C_<T>(2) * b2 + b3) * invDY * invDZ;

            // solve for remaining Aijk with i >= 2, j >= 2, k >= 2
            b0 = (F[1][1][1] - poly(0, 0, 0, dx, dy, dz)) * invDX2 * invDY2 * invDZ2;
            b1 = (FX[1][1][1] - poly(1, 0, 0, dx, dy, dz)) * invDX * invDY2 * invDZ2;
            b2 = (FY[1][1][1] - poly(0, 1, 0, dx, dy, dz)) * invDX2 * invDY * invDZ2;
            b3 = (FZ[1][1][1] - poly(0, 0, 1, dx, dy, dz)) * invDX2 * invDY2 * invDZ;
            b4 = (FXY[1][1][1] - poly(1, 1, 0, dx, dy, dz)) * invDX * invDY * invDZ2;
            b5 = (FXZ[1][1][1] - poly(1, 0, 1, dx, dy, dz)) * invDX * invDY2 * invDZ;
            b6 = (FYZ[1][1][1] - poly(0, 1, 1, dx, dy, dz)) * invDX2 * invDY * invDZ;
            b7 = (FXYZ[1][1][1] - poly(1, 1, 1, dx, dy, dz)) * invDX * invDY * invDZ;
            poly.A(2, 2, 2) = C_<T>(27) * b0 - C_<T>(9) * b1 - C_<T>(9) * b2 -
                C_<T>(9) * b3 + C_<T>(3) * b4 + C_<T>(3) * b5 + C_<T>(3) * b6 - b7;
            poly.A(3, 2, 2) = (C_<T>(-18) * b0 + C_<T>(9) * b1 + C_<T>(6) * b2 +
                C_<T>(6) * b3 - C_<T>(3) * b4 - C_<T>(3) * b5 - C_<T>(2) * b6 + b7) * invDX;
            poly.A(2, 3, 2) = (C_<T>(-18) * b0 + C_<T>(6) * b1 + C_<T>(9) * b2 +
                C_<T>(6) * b3 - C_<T>(3) * b4 - C_<T>(2) * b5 - C_<T>(3) * b6 + b7) * invDY;
            poly.A(2, 2, 3) = (C_<T>(-18) * b0 + C_<T>(6) * b1 + C_<T>(6) * b2 +
                C_<T>(9) * b3 - C_<T>(2) * b4 - C_<T>(3) * b5 - C_<T>(3) * b6 + b7) * invDZ;
            poly.A(3, 3, 2) = (C_<T>(12) * b0 - C_<T>(6) * b1 - C_<T>(6) * b2 -
                C_<T>(4) * b3 + C_<T>(3) * b4 + C_<T>(2) * b5 + C_<T>(2) * b6 - b7) *
                invDX * invDY;
            poly.A(3, 2, 3) = (C_<T>(12) * b0 - C_<T>(6) * b1 - C_<T>(4) * b2 -
                C_<T>(6) * b3 + C_<T>(2) * b4 + C_<T>(3) * b5 + C_<T>(2) * b6 - b7) *
                invDX * invDZ;
            poly.A(2, 3, 3) = (C_<T>(12) * b0 - C_<T>(4) * b1 - C_<T>(6) * b2 -
                C_<T>(6) * b3 + C_<T>(2) * b4 + C_<T>(2) * b5 + C_<T>(3) * b6 - b7) *
                invDY * invDZ;
            poly.A(3, 3, 3) = (C_<T>(-8) * b0 + C_<T>(4) * b1 + C_<T>(4) * b2 +
                C_<T>(4) * b3 - C_<T>(2) * b4 - C_<T>(2) * b5 - C_<T>(2) * b6 + b7) *
                invDX * invDY * invDZ;
        }

        // Support for evaluation.
        void Lookup(std::size_t coordinate, T const& v, std::size_t& vIndex, T& dv) const
        {
            T const& vDelta = mDelta[coordinate];
            T const& vMin = mMin[coordinate];
            for (vIndex = 0; vIndex + 1 < mBound[coordinate]; ++vIndex)
            {
                if (v < vMin + vDelta * static_cast<T>(vIndex + 1))
                {
                    dv = v - (vMin + vDelta * static_cast<T>(vIndex));
                    return;
                }
            }

            --vIndex;
            dv = v - (vMin + vDelta * static_cast<T>(vIndex));
        }

        std::size_t mNumF;
        T const* mF;
        std::array<std::size_t, 3> mBound;
        std::array<T, 3> mMin, mMax, mDelta;
        Multiarray<Polynomial, true> mPoly;

    private:
        friend class UnitTestIntpAkimaUniform3;
    };
}
