// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.14

#pragma once

// The interpolator is for uniformly spaced x-values.

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
    class IntpAkimaUniform1
    {
    public:
        IntpAkimaUniform1(std::size_t xBound, T const& xMin, T const& xMax, T const* F)
            :
            mNumF(xBound),
            mF(F),
            mXBound(xBound),
            mXMin(xMin),
            mXMax(xMax),
            mXDelta((xMax - xMin) / static_cast<T>(xBound - 1)),
            mPoly(xBound - 1)
        {
            GTL_ARGUMENT_ASSERT(
                mF != nullptr &&
                mXBound >= 3 && mXMin < mXMax,
                "Invalid input.");

            // Construct first-order derivatives.
            std::vector<T> FX(xBound);
            GetFX(FX);

            // Construct polynomials.
            GetPolynomials(FX);
        }

        ~IntpAkimaUniform1() = default;

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

        T const& GetXMin() const
        {
            return mXMin;
        }

        T const& GetXMax() const
        {
            return mXMax;
        }

        inline T GetDelta() const
        {
            return mXDelta;
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
            for (std::size_t x = 0, xp1 = 1, xp2 = 2; xp1 < numX; x = xp1, xp1 = xp2++)
            {
                slope[xp2] = (mF[xp1] - mF[x]) / mXDelta;
            }
            slope[1] = C_<T>(2) * slope[2] - slope[3];
            slope[0] = C_<T>(2) * slope[1] - slope[2];
            slope[numXp1] = C_<T>(2) * slope[numX] - slope[numXm1];
            slope[numXp2] = C_<T>(2) * slope[numXp1] - slope[numX];

            for (std::size_t x = 0; x < mXBound; ++x)
            {
                FX[x] = this->ComputeDerivative(&slope[x]);
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

        void GetPolynomials(std::vector<T>& FX)
        {
            T const DX2 = mXDelta * mXDelta;
            T const DX3 = DX2 * mXDelta;
            for (std::size_t x = 0, xp1 = 1; xp1 < mXBound; x = xp1++)
            {
                auto& poly = mPoly[x];
                T F0 = mF[x];
                T F1 = mF[xp1];
                T DF = F1 - F0;
                T FX0 = FX[x];
                T FX1 = FX[xp1];
                poly[0] = F0;
                poly[1] = FX0;
                poly[2] = (C_<T>(3) * DF - mXDelta * (FX1 + C_<T>(2) * FX0)) / DX2;
                poly[3] = (mXDelta * (FX0 + FX1) - C_<T>(2) * DF) / DX3;
            }
        }

        void Lookup(T const& x, std::size_t& index, T& dx) const
        {
            // The caller has ensured that mXMin <= x <= mXMax.
            for (index = 0; index + 1 < mXBound; ++index)
            {
                if (x < mXMin + mXDelta * static_cast<T>(index + 1))
                {
                    dx = x - (mXMin + mXDelta * static_cast<T>(index));
                    return;
                }
            }

            --index;
            dx = x - (mXMin + mXDelta * static_cast<T>(index));
        }

        std::size_t mNumF;
        T const* mF;
        std::size_t mXBound;
        T mXMin, mXMax, mXDelta;
        std::vector<Polynomial> mPoly;

    private:
        friend class UnitTestIntpAkimaUniform1;
    };
}
