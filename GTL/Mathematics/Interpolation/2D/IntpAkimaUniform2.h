// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.10

#pragma once

// The interpolator is for uniformly spaced (x,y)-values. The input samples
// F must be stored in row-major order to represent f(x,y); that is,
// F[c + numX*r] corresponds to f(x,y), where c is the index corresponding
// to x and r is the index corresponding to y.

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
    class IntpAkimaUniform2
    {
    public:
        // Create the polynomial storage in row-major order.
        IntpAkimaUniform2(
            std::size_t xBound, T const& xMin, T const& xMax,
            std::size_t yBound, T const& yMin, T const& yMax,
            T const* F)
            :
            mNumF(xBound * yBound),
            mF(F),
            mBound{ xBound, yBound },
            mMin{ xMin, yMin },
            mMax{ xMax, yMax },
            mDelta{
                (xMax - xMin) / static_cast<T>(xBound - 1),
                (yMax - yMin) / static_cast<T>(yBound - 1)
            },
            mPoly{ xBound - 1, yBound - 1 }
        {
            // At least a 3x3 block of data points is needed to construct the
            // estimates of the boundary derivatives.
            GTL_ARGUMENT_ASSERT(
                mF != nullptr &&
                mBound[0] >= 3 && mMin[0] < mMax[0] &&
                mBound[1] >= 3 && mMin[1] < mMax[1],
                "Invalid input.");

            // Create a row-major accessor for the function samples.
            MultiarrayAdapter<T, true> Fmap({ mBound[0], mBound[1] }, const_cast<T*>(F));

            // Construct first-order derivatives.
            Multiarray<T, true> FX{ mBound[0], mBound[1] };
            Multiarray<T, true> FY{ mBound[0], mBound[1] };
            GetFX(Fmap, FX);
            GetFY(Fmap, FY);

            // Construct second-order derivatives.
            Multiarray<T, true> FXY{ mBound[0], mBound[1] };
            GetFXY(Fmap, FXY);

            // Construct polynomials.
            GetPolynomials(Fmap, FX, FY, FXY);
        }

        ~IntpAkimaUniform2() = default;

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
        // inputs to xmin <= x <= xmax and ymin <= y <= ymax. The first
        // operator is for function evaluation. The second operator is for
        // function or derivative evaluations. The xOrder argument is the
        // order of the x-derivative and the yOrder argument is the order of
        // the y-derivative. Both orders are zero to get the function value
        // itself.
        T operator()(T const& x, T const& y) const
        {
            T xClamped = std::min(std::max(x, mMin[0]), mMax[1]);
            T yClamped = std::min(std::max(y, mMin[1]), mMax[1]);
            std::size_t ix{}, iy{};
            T dx{}, dy{};
            Lookup(0, xClamped, ix, dx);
            Lookup(1, yClamped, iy, dy);
            return mPoly(ix, iy)(dx, dy);
        }

        T operator()(std::size_t xOrder, std::size_t yOrder, T const& x, T const& y) const
        {
            T xClamped = std::min(std::max(x, mMin[0]), mMax[1]);
            T yClamped = std::min(std::max(y, mMin[1]), mMax[1]);
            std::size_t ix{}, iy{};
            T dx{}, dy{};
            Lookup(0, xClamped, ix, dx);
            Lookup(1, yClamped, iy, dy);
            return mPoly(ix, iy)(xOrder, yOrder, dx, dy);
        }

    private:
        class Polynomial
        {
        public:
            Polynomial()
                :
                mC{}
            {
                for (std::size_t ix = 0; ix < 4; ++ix)
                {
                    mC[ix].fill(C_<T>(0));
                }
            }

            // P(x,y) = (1,x,x^2,x^3)*A*(1,y,y^2,y^3). The matrix term
            // A(ix, iy) corresponds to the polynomial term x^{ix} y^{iy}.
            T& A(std::size_t ix, std::size_t iy)
            {
                return mC[ix][iy];
            }

            T operator()(T const& x, T const& y) const
            {
                std::array<T, 4> xPow = { C_<T>(1), x, x * x, x * x * x };
                std::array<T, 4> yPow = { C_<T>(1), y, y * y, y * y * y };

                T p = C_<T>(0);
                for (std::size_t iy = 0; iy <= 3; ++iy)
                {
                    for (std::size_t ix = 0; ix <= 3; ++ix)
                    {
                        p += mC[ix][iy] * xPow[ix] * yPow[iy];
                    }
                }
                return p;
            }

            T operator()(std::size_t xOrder, std::size_t yOrder, T const& x, T const& y) const
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

                T p = C_<T>(0);
                for (std::size_t iy = 0; iy < 4; ++iy)
                {
                    for (std::size_t ix = 0; ix < 4; ++ix)
                    {
                        p += mC[ix][iy] * xPow[ix] * yPow[iy];
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

            std::array<std::array<T, 4>, 4> mC;
        };

        // Support for construction.
        void GetFX(MultiarrayAdapter<T, true> const& F, Multiarray<T, true>& FX) const
        {
            // permute = { 0, 1 }
            std::size_t const numX = mBound[0];
            std::size_t const numXm1 = mBound[0] - 1;
            std::size_t const numXp1 = mBound[0] + 1;
            std::size_t const numXp2 = mBound[0] + 2;
            std::size_t const numXp3 = mBound[0] + 3;

            Multiarray<T, true> slope{ numXp3, mBound[1] };
            for (std::size_t y = 0; y < mBound[1]; ++y)
            {
                for (std::size_t x = 0, xp1 = 1, xp2 = 2; xp1 < numX; x = xp1, xp1 = xp2++)
                {
                    slope(xp2, y) = (F(xp1, y) - F(x, y)) / mDelta[0];
                }

                slope(1, y) = C_<T>(2) * slope(2, y) - slope(3, y);
                slope(0, y) = C_<T>(2) * slope(1, y) - slope(2, y);
                slope(numXp1, y) = C_<T>(2) * slope(numX, y) - slope(numXm1, y);
                slope(numXp2, y) = C_<T>(2) * slope(numXp1, y) - slope(numX, y);
            }

            for (std::size_t y = 0; y < mBound[1]; ++y)
            {
                for (std::size_t x = 0; x < mBound[0]; ++x)
                {
                    FX(x, y) = ComputeDerivative(&slope(x, y));
                }
            }
        }

        void GetFY(MultiarrayAdapter<T, true> const& F, Multiarray<T, true>& FY) const
        {
            // permute = { 1, 0 }
            std::size_t const numY = mBound[1];
            std::size_t const numYm1 = mBound[1] - 1;
            std::size_t const numYp1 = mBound[1] + 1;
            std::size_t const numYp2 = mBound[1] + 2;
            std::size_t const numYp3 = mBound[1] + 3;

            Multiarray<T, true> slope{ numYp3, mBound[0] };
            for (std::size_t x = 0; x < mBound[0]; ++x)
            {
                for (std::size_t y = 0, yp1 = 1, yp2 = 2; yp1 < numY; y = yp1, yp1 = yp2++)
                {
                    slope(yp2, x) = (F(x, yp1) - F(x, y)) / mDelta[1];
                }

                slope(1, x) = C_<T>(2) * slope(2, x) - slope(3, x);
                slope(0, x) = C_<T>(2) * slope(1, x) - slope(2, x);
                slope(numYp1, x) = C_<T>(2) * slope(numY, x) - slope(numYm1, x);
                slope(numYp2, x) = C_<T>(2) * slope(numYp1, x) - slope(numY, x);
            }

            for (std::size_t y = 0; y < mBound[1]; ++y)
            {
                for (std::size_t x = 0; x < mBound[0]; ++x)
                {
                    FY(x, y) = ComputeDerivative(&slope(y, x));
                }
            }
        }

        void GetFXY(MultiarrayAdapter<T, true> const& F, Multiarray<T, true>& FXY) const
        {
            T const multiplier = C_<T>(1, 4) / (mDelta[0] * mDelta[1]);
            std::size_t const numXm1 = mBound[0] - 1;
            std::size_t const numXm2 = mBound[0] - 2;
            std::size_t const numXm3 = mBound[0] - 3;
            std::size_t const numYm1 = mBound[1] - 1;
            std::size_t const numYm2 = mBound[1] - 2;
            std::size_t const numYm3 = mBound[1] - 3;

            // corners
            FXY(0, 0) = multiplier * (
                C_<T>(9) * F(0, 0)
                - C_<T>(12) * (F(1, 0) + F(0, 1))
                + C_<T>(3) * (F(2, 0) + F(0, 2))
                + C_<T>(16) * F(1, 1)
                - C_<T>(4) * (F(2, 1) + F(1, 2))
                + F(2, 2));

            FXY(numXm1, 0) = multiplier * (
                C_<T>(9) * F(numXm1, 0)
                - C_<T>(12) * (F(numXm2, 0) + F(numXm1, 1))
                + C_<T>(3) * (F(numXm3, 0) + F(numXm1, 2))
                + C_<T>(16) * F(numXm2, 1)
                - C_<T>(4) * (F(numXm3, 1) + F(numXm2, 2))
                + F(numXm3, 2));

            FXY(0, numYm1) = multiplier * (
                C_<T>(9) * F(0, numYm1)
                - C_<T>(12) * (F(1, numYm1) + F(0, numYm2))
                + C_<T>(3) * (F(2, numYm1) + F(0, numYm3))
                + C_<T>(16) * F(1, numYm2)
                - C_<T>(4) * (F(2, numYm2) + F(1, numYm3))
                + F(2, numYm3));

            FXY(numXm1, numYm1) = multiplier * (
                C_<T>(9) * F(numXm1, numYm1)
                - C_<T>(12) * (F(numXm2, numYm1) + F(numXm1, numYm2))
                + C_<T>(3) * (F(numXm3, numYm1) + F(numXm1, numYm3))
                + C_<T>(16) * F(numXm2, numYm2)
                - C_<T>(4) * (F(numXm3, numYm2) + F(numXm2, numYm3))
                + F(numXm3, numYm3));

            // x-edges
            for (std::size_t xm1 = 0, x = 1, xp1 = 2; x < numXm1; xm1 = x, x = xp1++)
            {
                FXY(x, 0) = multiplier * (
                    C_<T>(3) * (F(xm1, 0) - F(xp1, 0))
                    - C_<T>(4) * (F(xm1, 1) - F(xp1, 1))
                    + (F(xm1, 2) - F(xp1, 2)));

                FXY(x, numYm1) = multiplier * (
                    C_<T>(3) * (F(xm1, numYm1) - F(xp1, numYm1))
                    - C_<T>(4) * (F(xm1, numYm2) - F(xp1, numYm2))
                    + (F(xm1, numYm3) - F(xp1, numYm3)));
            }

            // y-edges
            for (std::size_t ym1 = 0, y = 1, yp1 = 2; y < numYm1; ym1 = y, y = yp1++)
            {
                FXY(0, y) = multiplier * (
                    C_<T>(3) * (F(0, ym1) - F(0, yp1))
                    - C_<T>(4) * (F(1, ym1) - F(1, yp1))
                    + (F(2, ym1) - F(2, yp1)));

                FXY(numXm1, y) = multiplier * (
                    C_<T>(3) * (F(numXm1, ym1) - F(numXm1, yp1))
                    - C_<T>(4) * (F(numXm2, ym1) - F(numXm2, yp1))
                    + (F(numXm3, ym1) - F(numXm3, yp1)));
            }

            // interior
            for (std::size_t ym1 = 0, y = 1, yp1 = 2; y < numYm1; ym1 = y, y = yp1++)
            {
                for (std::size_t xm1 = 0, x = 1, xp1 = 2; x < numXm1; xm1 = x, x = xp1++)
                {
                    FXY(x, y) = multiplier * (
                        F(xm1, ym1) - F(xp1, ym1) - F(xm1, yp1) + F(xp1, yp1));
                }
            }
        }

        static T ComputeDerivative(T const* slope)
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

        void GetPolynomials(
            MultiarrayAdapter<T, true> const& F,
            Multiarray<T, true> const& FX,
            Multiarray<T, true> const& FY,
            Multiarray<T, true> const& FXY)
        {
            // Note the 'transposing' of the 2x2 blocks to match notation used
            // in the polynomial definition.
            std::array<std::array<T, 2>, 2> G{}, GX{}, GY{}, GXY{};

            for (std::size_t y = 0, yp1 = 1; yp1 < mBound[1]; y = yp1++)
            {
                for (std::size_t x = 0, xp1 = 1; xp1 < mBound[0]; x = xp1++)
                {
                    G[0][0] = F(x, y);
                    G[0][1] = F(x, yp1);
                    G[1][0] = F(xp1, y);
                    G[1][1] = F(xp1, yp1);

                    GX[0][0] = FX(x, y);
                    GX[0][1] = FX(x, yp1);
                    GX[1][0] = FX(xp1, y);
                    GX[1][1] = FX(xp1, yp1);

                    GY[0][0] = FY(x, y);
                    GY[0][1] = FY(x, yp1);
                    GY[1][0] = FY(xp1, y);
                    GY[1][1] = FY(xp1, yp1);

                    GXY[0][0] = FXY(x, y);
                    GXY[0][1] = FXY(x, yp1);
                    GXY[1][0] = FXY(xp1, y);
                    GXY[1][1] = FXY(xp1, yp1);

                    Construct(mPoly(x, y), G, GX, GY, GXY);
                }
            }
        }

        void Construct(Polynomial& poly,
            std::array<std::array<T, 2>, 2> const& F,
            std::array<std::array<T, 2>, 2> const& FX,
            std::array<std::array<T, 2>, 2> const& FY,
            std::array<std::array<T, 2>, 2> const& FXY)
        {
            T dx = mDelta[0];
            T dy = mDelta[1];
            T invDX = C_<T>(1) / dx, invDX2 = invDX * invDX;
            T invDY = C_<T>(1) / dy, invDY2 = invDY * invDY;
            T b0{}, b1{}, b2{}, b3{};

            poly.A(0, 0) = F[0][0];
            poly.A(1, 0) = FX[0][0];
            poly.A(0, 1) = FY[0][0];
            poly.A(1, 1) = FXY[0][0];

            b0 = (F[1][0] - poly(0, 0, dx, C_<T>(0))) * invDX2;
            b1 = (FX[1][0] - poly(1, 0, dx, C_<T>(0))) * invDX;
            poly.A(2, 0) = C_<T>(3) * b0 - b1;
            poly.A(3, 0) = (C_<T>(-2) * b0 + b1) * invDX;

            b0 = (F[0][1] - poly(0, 0, C_<T>(0), dy)) * invDY2;
            b1 = (FY[0][1] - poly(0, 1, C_<T>(0), dy)) * invDY;
            poly.A(0, 2) = C_<T>(3) * b0 - b1;
            poly.A(0, 3) = (C_<T>(-2) * b0 + b1) * invDY;

            b0 = (FY[1][0] - poly(0, 1, dx, C_<T>(0))) * invDX2;
            b1 = (FXY[1][0] - poly(1, 1, dx, C_<T>(0))) * invDX;
            poly.A(2, 1) = C_<T>(3) * b0 - b1;
            poly.A(3, 1) = (C_<T>(-2) * b0 + b1) * invDX;

            b0 = (FX[0][1] - poly(1, 0, C_<T>(0), dy)) * invDY2;
            b1 = (FXY[0][1] - poly(1, 1, C_<T>(0), dy)) * invDY;
            poly.A(1, 2) = C_<T>(3) * b0 - b1;
            poly.A(1, 3) = (C_<T>(-2) * b0 + b1) * invDY;

            b0 = (F[1][1] - poly(0, 0, dx, dy)) * invDX2 * invDY2;
            b1 = (FX[1][1] - poly(1, 0, dx, dy)) * invDX * invDY2;
            b2 = (FY[1][1] - poly(0, 1, dx, dy)) * invDX2 * invDY;
            b3 = (FXY[1][1] - poly(1, 1, dx, dy)) * invDX * invDY;
            poly.A(2, 2) = C_<T>(9) * b0 - C_<T>(3) * b1 - C_<T>(3) * b2 + b3;
            poly.A(3, 2) = (C_<T>(-6) * b0 + C_<T>(3) * b1 + C_<T>(2) * b2 - b3) * invDX;
            poly.A(2, 3) = (C_<T>(-6) * b0 + C_<T>(2) * b1 + C_<T>(3) * b2 - b3) * invDY;
            poly.A(3, 3) = (C_<T>(4) * b0 - C_<T>(2) * b1 - C_<T>(2) * b2 + b3) * invDX * invDY;
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
        std::array<std::size_t, 2> mBound;
        std::array<T, 2> mMin, mMax, mDelta;
        Multiarray<Polynomial, true> mPoly;

    private:
        friend class UnitTestIntpAkimaUniform2;
    };
}
