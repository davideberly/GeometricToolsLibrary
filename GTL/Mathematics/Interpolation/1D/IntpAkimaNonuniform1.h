// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.14

#pragma once

// The interpolator is for arbitrarily spaced x-values.

#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Utility/Exceptions.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class IntpAkimaNonuniform1
    {
    public:
        IntpAkimaNonuniform1(std::size_t xBound, T const* X, T const* F)
            :
            mNumF(xBound),
            mF(F),
            mXBound(xBound),
            mX(X),
            mPoly(xBound - 1)
        {
            GTL_ARGUMENT_ASSERT(
                mF != nullptr &&
                mXBound >= 3 && mX != nullptr,
                "Invalid input.");

            for (std::size_t ix = 0, ixp1 = 1; ixp1 < mXBound; ix = ixp1++)
            {
                GTL_ARGUMENT_ASSERT(
                    mX[ixp1] > mX[ix],
                    "Invalid ordering of domain values.");
            }

            // Construct first-order derivatives.
            std::vector<T> FX(xBound);
            GetFX(FX);

            // Construct polynomials.
            for (std::size_t i0 = 0, i1 = 1; i1 < mXBound; i0 = i1++)
            {
                auto& poly = this->mPoly[i0];
                T F0 = F[i0];
                T F1 = F[i1];
                T DF = F1 - F0;
                T FX0 = FX[i0];
                T FX1 = FX[i1];
                T DX = X[i1] - X[i0];
                T DX2 = DX * DX;
                T DX3 = DX2 * DX;
                poly[0] = F0;
                poly[1] = FX0;
                poly[2] = (C_<T>(3) * DF - DX * (FX1 + C_<T>(2) * FX0)) / DX2;
                poly[3] = (DX * (FX0 + FX1) - C_<T>(2) * DF) / DX3;
            }

            // Construct polynomials.
            GetPolynomials(F, FX);
        }

        ~IntpAkimaNonuniform1() = default;

        // Member access.
        inline std::size_t GetNumF() const
        {
            return mNumF;
        }

        inline T const* GetF() const
        {
            return mF;
        }

        inline std::size_t GetXBound() const
        {
            return mXBound;
        }

        T const* GetX() const
        {
            return mX;
        }

        T const& GetXMin() const
        {
            return mX[0];
        }

        T const& GetXMax() const
        {
            return mX[mXBound - 1];
        }

        // delta = x[i + 1] - x[i] for 0 <= i <= bound-2
        inline T GetDelta(std::size_t i) const
        {
            return mX[i + 1] - mX[i];
        }

        // Evaluate the function and its derivatives. The functions clamp the
        // inputs to xmin <= x <= xmax. The first operator is for function
        // evaluation. The second operator is for function or derivative
        // evaluations. The order argument is the order of the x-derivative.
        // Set order to 0 to get the function value itself.
        T operator()(T const& x) const
        {
            T xClamped = std::min(std::max(x, GetXMin()), GetXMax());
            std::size_t ix{};
            T dx{};
            Lookup(xClamped, ix, dx);
            return mPoly[ix](dx);
        }

        T operator()(std::size_t order, T const& x) const
        {
            T xClamped = std::min(std::max(x, GetXMin()), GetXMax());
            std::size_t ix{};
            T dx{};
            Lookup(xClamped, ix, dx);
            return mPoly[ix](order, dx);
        }

    protected:
        class Polynomial
        {
        public:
            Polynomial()
                :
                mC{ C_<T>(0), C_<T>(0), C_<T>(0), C_<T>(0) }
            {
            }

            // P(x) = c[0] + c[1]*x + c[2]*x^2 + c[3]*x^3
            inline T& operator[](std::size_t i)
            {
                return mC[i];
            }

            T operator()(T const& x) const
            {
                return mC[0] + x * (mC[1] + x * (mC[2] + x * mC[3]));
            }

            T operator()(std::size_t order, T const& x) const
            {
                switch (order)
                {
                case 0:
                    return mC[0] + x * (mC[1] + x * (mC[2] + x * mC[3]));
                case 1:
                    return mC[1] + x * (C_<T>(2) * mC[2] + x * C_<T>(3) * mC[3]);
                case 2:
                    return C_<T>(2) * mC[2] + x * C_<T>(6) * mC[3];
                case 3:
                    return C_<T>(6) * mC[3];
                default:
                    return C_<T>(0);
                }
            }

        private:
            std::array<T, 4> mC;
        };

        void GetFX(std::vector<T>& FX) const
        {
            std::size_t const numX = mXBound;
            std::size_t const numXm1 = mXBound - 1;
            std::size_t const numXp1 = mXBound + 1;
            std::size_t const numXp2 = mXBound + 2;
            std::size_t const numXp3 = mXBound + 3;

            std::vector<T> slope(numXp3);
            for (std::size_t ix = 0, ixp1 = 1, ixp2 = 2; ixp1 < numX; ix = ixp1, ixp1 = ixp2++)
            {
                T DX = mX[ixp1] - mX[ix];
                T DF = mF[ixp1] - mF[ix];
                slope[ixp2] = DF / DX;
            }
            slope[1] = C_<T>(2) * slope[2] - slope[3];
            slope[0] = C_<T>(2) * slope[1] - slope[2];
            slope[numXp1] = C_<T>(2) * slope[numX] - slope[numXm1];
            slope[numXp2] = C_<T>(2) * slope[numXp1] - slope[numX];

            for (std::size_t ix = 0; ix < mXBound; ++ix)
            {
                FX[ix] = this->ComputeDerivative(&slope[ix]);
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

        void GetPolynomials(T const* F, std::vector<T>& FX)
        {
            for (std::size_t ix = 0, ixp1 = 1; ixp1 < mXBound; ix = ixp1++)
            {
                auto& poly = mPoly[ix];
                T F0 = F[ix];
                T F1 = F[ixp1];
                T DF = F1 - F0;
                T FX0 = FX[ix];
                T FX1 = FX[ixp1];
                T DX = mX[ixp1] - mX[ix];
                T DX2 = DX * DX;
                T DX3 = DX2 * DX;
                poly[0] = F0;
                poly[1] = FX0;
                poly[2] = (C_<T>(3) * DF - DX * (FX1 + C_<T>(2) * FX0)) / DX2;
                poly[3] = (DX * (FX0 + FX1) - C_<T>(2) * DF) / DX3;
            }
        }

        void Lookup(T const& x, std::size_t& index, T& dx) const
        {
            // The caller has ensured that mXMin <= x <= mXMax.
            for (index = 0; index + 1 < mXBound; ++index)
            {
                if (x < mX[index + 1])
                {
                    dx = x - mX[index];
                    return;
                }
            }

            --index;
            dx = x - mX[index];
        }

        std::size_t mNumF;
        T const* mF;
        std::size_t mXBound;
        T const* mX;
        std::vector<Polynomial> mPoly;

    private:
        friend class UnitTestIntpAkimaNonuniform1;
    };
}
