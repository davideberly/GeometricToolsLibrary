// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// For a triangle with counterclockwise vertices V0, V1 and V2, operator()
// returns
//   +1, P outside circumcircle of triangle
//   -1, P inside circumcircle of triangle
//    0, P on circumcircle of triangle
// The input type T must satisfy std::is_floating_point<T>::value = true. The
// compute type is BSNumber<UIntegerFP32<N>>, where N depends on the input
// type and the expression tree of the query. The determination of worst-case
// N is by the GeometricTools/GTL/Tools/PrecisionCalculator tool. The
// N-values are conservative so that the number of bits for the query is
// sufficient for any finite floating-point inputs.
//
// expression tree number of nodes = 16
//
// compute type = float
//   N = 36
//   sizeof(BSNumber<UIntegerFP32<36>>) = 160
//   number of heap bytes for rational computing = 2560 = 16 * 160
//
// compute type = double
//   N = 264
//   sizeof(BSNumber<UIntegerFP32<132>>) = 1072
//   number of heap bytes for rational computing = 17152 = 16 * 1072
//
// The expression-tree nodes are allocated on the heap. The N-values from the
// precision calculator are rounded up to an even number so that the bit
// storage of UIntegerFP32<N> bit storage is a block of memory whose number of
// bytes is a multiple of 8. The 'bytes' numbers are for the heap memory of
// std::vector<BSNumber<UIntegerFP32<N>> but does not include the stack bytes
// for the members of std::vector.
//
// The member functions with only T-valued arguments are for floating-point
// inputs. The member functions with T-valued and Rational-valued arguments
// are intended for applications where the Rational inputs are cached and
// re-used to avoid re-converting floating-point numbers to rational numbers.

#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#include <GTL/Mathematics/Arithmetic/SWInterval.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <type_traits>
#include <vector>

namespace gtl
{
    template <typename T>
    class ExactToCircumcircle2
    {
    public:
        using Rational = BSNumber<UIntegerFP32<2>>;

        ExactToCircumcircle2()
            :
            mISign(invalidSign),
            mRSign(invalidSign),
            mIDet{},
            mNode(numNodes)
        {
            static_assert(
                std::is_floating_point<T>::value,
                "The input type must be a floating-point type.");
        }

        std::int32_t operator()(Vector2<T> const& P, Vector2<T> const& V0,
            Vector2<T> const& V1, Vector2<T> const& V2)
        {
            mISign = invalidSign;
            mRSign = invalidSign;

            // Use interval arithmetic to determine the relative location of
            // P, if possible.
            ComputeInterval(P, V0, V1, V2);
            if (mISign != invalidSign)
            {
                return mISign;
            }

            // The exact relative location of P is not known. Compute the
            // relative location using rational arithmetic.
            ComputeRational(P, V0, V1, V2);
            return mRSign;
        }

        std::int32_t operator()(Vector2<T> const& P, Vector2<T> const& V0,
            Vector2<T> const& V1, Vector2<T> const& V2,
            std::function<std::array<Vector2<Rational> const*, 4>()> const& GetRPoints)
        {
            mISign = invalidSign;
            mRSign = invalidSign;

            // Use interval arithmetic to determine the relative location of
            // P, if possible.
            ComputeInterval(P, V0, V1, V2);
            if (mISign != invalidSign)
            {
                return mISign;
            }

            // The exact relative location of P is not known. Compute the
            // relative location using rational arithmetic.
            std::array<Vector2<Rational> const*, 4> rPoints = GetRPoints();
            auto const& rP = *rPoints[0];
            auto const& rV0 = *rPoints[1];
            auto const& rV1 = *rPoints[2];
            auto const& rV2 = *rPoints[3];
            ComputeRational(rP, rV0, rV1, rV2);
            return mRSign;
        }

    private:
        void ComputeInterval(Vector2<T> const& P, Vector2<T> const& V0,
            Vector2<T> const& V1, Vector2<T> const& V2)
        {
            auto x0 = SWInterval<T>::Sub(V0[0], P[0]);
            auto y0 = SWInterval<T>::Sub(V0[1], P[1]);
            auto s00 = SWInterval<T>::Add(V0[0], P[0]);
            auto s01 = SWInterval<T>::Add(V0[1], P[1]);
            auto x1 = SWInterval<T>::Sub(V1[0], P[0]);
            auto y1 = SWInterval<T>::Sub(V1[1], P[1]);
            auto s10 = SWInterval<T>::Add(V1[0], P[0]);
            auto s11 = SWInterval<T>::Add(V1[1], P[1]);
            auto x2 = SWInterval<T>::Sub(V2[0], P[0]);
            auto y2 = SWInterval<T>::Sub(V2[1], P[1]);
            auto s20 = SWInterval<T>::Add(V2[0], P[0]);
            auto s21 = SWInterval<T>::Add(V2[1], P[1]);
            auto t00 = s00 * x0;
            auto t01 = s01 * y0;
            auto t10 = s10 * x1;
            auto t11 = s11 * y1;
            auto t20 = s20 * x2;
            auto t21 = s21 * y2;
            auto z0 = t00 + t01;
            auto z1 = t10 + t11;
            auto z2 = t20 + t21;
            auto y0z1 = y0 * z1;
            auto y0z2 = y0 * z2;
            auto y1z0 = y1 * z0;
            auto y1z2 = y1 * z2;
            auto y2z0 = y2 * z0;
            auto y2z1 = y2 * z1;
            auto c0 = y1z2 - y2z1;
            auto c1 = y2z0 - y0z2;
            auto c2 = y0z1 - y1z0;
            auto x0c0 = x0 * c0;
            auto x1c1 = x1 * c1;
            auto x2c2 = x2 * c2;
            mIDet = x0c0 + x1c1 + x2c2;
            mIDet = -mIDet;

            if (mIDet[0] > C_<T>(0))
            {
                mISign = +1;
            }
            else if (mIDet[1] < C_<T>(0))
            {
                mISign = -1;
            }
            else
            {
                // The interval contains 0, so the sign is unknown.
                mISign = invalidSign;
            }
        }

        static std::size_t constexpr numWords = std::is_same<T, float>::value ? 36 : 264;
        using CRational = BSNumber<UIntegerFP32<numWords>>;

        inline void Add(std::size_t arg0, std::size_t arg1, std::size_t result)
        {
            CRational::Add(mNode[arg0], mNode[arg1], mNode[result]);
        }

        inline void Sub(std::size_t arg0, std::size_t arg1, std::size_t result)
        {
            CRational::Sub(mNode[arg0], mNode[arg1], mNode[result]);
        }

        inline void Mul(std::size_t arg0, std::size_t arg1, std::size_t result)
        {
            CRational::Mul(mNode[arg0], mNode[arg1], mNode[result]);
        }

        void ComputeRational()
        {
            std::size_t constexpr p0 = 0, p1 = 1, v00 = 2, v01 = 3;
            std::size_t constexpr v10 = 4, v11 = 5, v20 = 6, v21 = 7;
            std::size_t constexpr x0 = 8, y0 = 9, s00 = 10, s01 = 11;
            std::size_t constexpr x1 = 2, y1 = 3, s10 = 12, s11 = 13;
            std::size_t constexpr x2 = 4, y2 = 5, s20 = 14, s21 = 15;
            Sub(v00, p0, x0);
            Sub(v01, p1, y0);
            Add(v00, p0, s00);
            Add(v01, p1, s01);
            Sub(v10, p0, x1);
            Sub(v11, p1, y1);
            Add(v10, p0, s10);
            Add(v11, p1, s11);
            Sub(v20, p0, x2);
            Sub(v21, p1, y2);
            Add(v20, p0, s20);
            Add(v21, p1, s21);
            std::size_t constexpr t00 = 0, t01 = 1, z0 = 6;
            std::size_t constexpr t10 = 0, t11 = 1, z1 = 7;
            std::size_t constexpr t20 = 0, t21 = 1, z2 = 10;
            Mul(s00, x0, t00);
            Mul(s01, y0, t01);
            Add(t00, t01, z0);
            Mul(s10, x1, t10);
            Mul(s11, y1, t11);
            Add(t10, t11, z1);
            Mul(s20, x2, t20);
            Mul(s21, y2, t21);
            Add(t20, t21, z2);

            std::size_t constexpr y0z1 = 0, y0z2 = 1;
            std::size_t constexpr y1z0 = 9, y1z2 = 11;
            std::size_t constexpr y2z0 = 3, y2z1 = 6;
            Mul(y0, z1, y0z1);
            Mul(y0, z2, y0z2);
            Mul(y1, z0, y1z0);
            Mul(y1, z2, y1z2);
            Mul(y2, z0, y2z0);
            Mul(y2, z1, y2z1);

            std::size_t constexpr c0 = 5, c1 = 6, c2 = 1;
            std::size_t constexpr x0c0 = 0, x1c1 = 3, x2c2 = 2;
            std::size_t constexpr sum = 4;
            Sub(y1z2, y2z1, c0);
            Sub(y2z0, y0z2, c1);
            Sub(y0z1, y1z0, c2);
            Mul(x0, c0, x0c0);
            Mul(x1, c1, x1c1);
            Mul(x2, c2, x2c2);
            Add(x0c0, x1c1, sum);
            Add(sum, x2c2, detNode);

            // NOTE: Change sign to be consistent with ComputeInterval.
            mNode[detNode].Negate();
            mRSign = mNode[detNode].GetSign();
        }

        void ComputeRational(Vector2<T> const& P, Vector2<T> const& V0,
            Vector2<T> const& V1, Vector2<T> const& V2)
        {
            mNode[0] = P[0];
            mNode[1] = P[1];
            mNode[2] = V0[0];
            mNode[3] = V0[1];
            mNode[4] = V1[0];
            mNode[5] = V1[1];
            mNode[6] = V2[0];
            mNode[7] = V2[1];
            ComputeRational();
        }

        void ComputeRational(Vector2<Rational> const& rP,
            Vector2<Rational> const& rV0, Vector2<Rational> const& rV1,
            Vector2<Rational> const& rV2)
        {
            mNode[0] = rP[0];
            mNode[1] = rP[1];
            mNode[2] = rV0[0];
            mNode[3] = rV0[1];
            mNode[4] = rV1[0];
            mNode[5] = rV1[1];
            mNode[6] = rV2[0];
            mNode[7] = rV2[1];
            ComputeRational();
        }

        static std::int32_t constexpr invalidSign = std::numeric_limits<std::int32_t>::max();
        static std::size_t constexpr numNodes = 16;
        static std::size_t constexpr detNode = 0;
        std::int32_t mISign;
        std::int32_t mRSign;
        SWInterval<T> mIDet;
        std::vector<CRational> mNode;

    private:
        friend class UnitTestExactToCircumcircle2;
    };
}
