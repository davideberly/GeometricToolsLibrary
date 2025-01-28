// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// Queries to compute the exact sign of determinants. The input type T must
// satisfy std::is_floating_point<T>::value = true. The compute type is
// BSNumber<UIntegerFP32<N>>, where N depends on the input type and the
// expression tree. The determination of worst-case N is by the tool
// GeometricTools/GTL/Tools/PrecisionCalculator. These N-values are
// conservative so that the number of bits for the query is sufficient for
// any finite floating-point inputs.
//
// expression tree number of nodes = 30
//
// compute type = float
//   N = 36
//   sizeof(BSNumber<UIntegerFP32<36>>) = 160
//   number of heap bytes for rational computing = 4800 = 30 * 160
//
// compute type = double
//   N = 264
//   sizeof(BSNumber<UIntegerFP32<198>>) = 1072
//   number of heap bytes for rational computing = 32160 = 30 * 1072
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
#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <type_traits>
#include <vector>

namespace gtl
{
    template <typename T>
    class ExactSignDeterminant4
    {
    public:
        using Rational = BSNumber<UIntegerFP32<2>>;

        ExactSignDeterminant4()
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

        std::int32_t operator()(
            T const& a00, T const& a01, T const& a02, T const& a03,
            T const& a10, T const& a11, T const& a12, T const& a13,
            T const& a20, T const& a21, T const& a22, T const& a23,
            T const& a30, T const& a31, T const& a32, T const& a33)
        {
            mISign = invalidSign;
            mRSign = invalidSign;

            // Use interval arithmetic to determine the sign of the
            // determinant, if possible.
            ComputeInterval(a00, a01, a02, a03, a10, a11, a12, a13,
                a20, a21, a22, a23, a30, a31, a32, a33);
            if (mISign != invalidSign)
            {
                return mISign;
            }

            // The exact sign of the determinant is not known, so compute
            // the determinant using rational arithmetic.
            ComputeRational(a00, a01, a02, a03, a10, a11, a12, a13,
                a20, a21, a22, a23, a30, a31, a32, a33);
            return mRSign;
        }

        std::int32_t operator()(
            T const& a00, T const& a01, T const& a02, T const& a03,
            T const& a10, T const& a11, T const& a12, T const& a13,
            T const& a20, T const& a21, T const& a22, T const& a23,
            T const& a30, T const& a31, T const& a32, T const& a33,
            std::function<std::array<Rational const*, 16>()> const& GetRValues)
        {
            mISign = invalidSign;
            mRSign = invalidSign;

            // Use interval arithmetic to determine the sign of the
            // determinant, if possible.
            ComputeInterval(a00, a01, a02, a03, a10, a11, a12, a13,
                a20, a21, a22, a23, a30, a31, a32, a33);
            if (mISign != invalidSign)
            {
                return mISign;
            }

            // The exact sign of the determinant is not known, so compute
            // the determinant using rational arithmetic.
            std::array<Rational const*, 16> rValues = GetRValues();
            auto const& ra00 = *rValues[0];
            auto const& ra01 = *rValues[1];
            auto const& ra02 = *rValues[2];
            auto const& ra03 = *rValues[3];
            auto const& ra10 = *rValues[4];
            auto const& ra11 = *rValues[5];
            auto const& ra12 = *rValues[6];
            auto const& ra13 = *rValues[7];
            auto const& ra20 = *rValues[8];
            auto const& ra21 = *rValues[9];
            auto const& ra22 = *rValues[10];
            auto const& ra23 = *rValues[11];
            auto const& ra30 = *rValues[12];
            auto const& ra31 = *rValues[13];
            auto const& ra32 = *rValues[14];
            auto const& ra33 = *rValues[15];
            ComputeRational(ra00, ra01, ra02, ra03, ra10, ra11, ra12, ra13,
                ra20, ra21, ra22, ra23, ra30, ra31, ra32, ra33);
            return mRSign;
        }

    private:
        void ComputeInterval(
            T const& a00, T const& a01, T const& a02, T const& a03,
            T const& a10, T const& a11, T const& a12, T const& a13,
            T const& a20, T const& a21, T const& a22, T const& a23,
            T const& a30, T const& a31, T const& a32, T const& a33)
        {
            auto a00a11 = SWInterval<T>::Mul(a00, a11);
            auto a00a12 = SWInterval<T>::Mul(a00, a12);
            auto a00a13 = SWInterval<T>::Mul(a00, a13);
            auto a01a12 = SWInterval<T>::Mul(a01, a12);
            auto a01a13 = SWInterval<T>::Mul(a01, a13);
            auto a02a13 = SWInterval<T>::Mul(a02, a13);
            auto a01a10 = SWInterval<T>::Mul(a01, a10);
            auto a02a10 = SWInterval<T>::Mul(a02, a10);
            auto a03a10 = SWInterval<T>::Mul(a03, a10);
            auto a02a11 = SWInterval<T>::Mul(a02, a11);
            auto a03a11 = SWInterval<T>::Mul(a03, a11);
            auto a03a12 = SWInterval<T>::Mul(a03, a12);
            auto a20a31 = SWInterval<T>::Mul(a20, a31);
            auto a20a32 = SWInterval<T>::Mul(a20, a32);
            auto a20a33 = SWInterval<T>::Mul(a20, a33);
            auto a21a32 = SWInterval<T>::Mul(a21, a32);
            auto a21a33 = SWInterval<T>::Mul(a21, a33);
            auto a22a33 = SWInterval<T>::Mul(a22, a33);
            auto a21a30 = SWInterval<T>::Mul(a21, a30);
            auto a22a30 = SWInterval<T>::Mul(a22, a30);
            auto a23a30 = SWInterval<T>::Mul(a23, a30);
            auto a22a31 = SWInterval<T>::Mul(a22, a31);
            auto a23a31 = SWInterval<T>::Mul(a23, a31);
            auto a23a32 = SWInterval<T>::Mul(a23, a32);
            auto u0 = a00a11 - a01a10;
            auto u1 = a00a12 - a02a10;
            auto u2 = a00a13 - a03a10;
            auto u3 = a01a12 - a02a11;
            auto u4 = a01a13 - a03a11;
            auto u5 = a02a13 - a03a12;
            auto v0 = a20a31 - a21a30;
            auto v1 = a20a32 - a22a30;
            auto v2 = a20a33 - a23a30;
            auto v3 = a21a32 - a22a31;
            auto v4 = a21a33 - a23a31;
            auto v5 = a22a33 - a23a32;
            auto u0v5 = u0 * v5;
            auto u1v4 = u1 * v4;
            auto u2v3 = u2 * v3;
            auto u3v2 = u3 * v2;
            auto u4v1 = u4 * v1;
            auto u5v0 = u5 * v0;
            mIDet = u0v5 - u1v4[1] + u2v3 + u3v2 - u4v1 + u5v0;

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
            std::size_t constexpr a00 = 0, a01 = 1, a02 = 2, a03 = 3;
            std::size_t constexpr a10 = 4, a11 = 5, a12 = 6, a13 = 7;
            std::size_t constexpr a20 = 8, a21 = 9, a22 = 10, a23 = 11;
            std::size_t constexpr a30 = 12, a31 = 13, a32 = 14, a33 = 15;

            std::size_t constexpr a00a11 = 16, a01a10 = 17, u0 = 18;
            std::size_t constexpr a00a12 = 16, a02a10 = 17, u1 = 19;
            std::size_t constexpr a00a13 = 16, a03a10 = 17, u2 = 20;
            std::size_t constexpr a01a12 = 16, a02a11 = 17, u3 = 21;
            std::size_t constexpr a01a13 = 16, a03a11 = 17, u4 = 22;
            std::size_t constexpr a02a13 = 16, a03a12 = 17, u5 = 23;

            Mul(a00, a11, a00a11);
            Mul(a01, a10, a01a10);
            Sub(a00a11, a01a10, u0);
            Mul(a00, a12, a00a12);
            Mul(a02, a10, a02a10);
            Sub(a00a12, a02a10, u1);
            Mul(a00, a13, a00a13);
            Mul(a03, a10, a03a10);
            Sub(a00a13, a03a10, u2);
            Mul(a01, a12, a01a12);
            Mul(a02, a11, a02a11);
            Sub(a01a12, a02a11, u3);
            Mul(a01, a13, a01a13);
            Mul(a03, a11, a03a11);
            Sub(a01a13, a03a11, u4);
            Mul(a02, a13, a02a13);
            Mul(a03, a12, a03a12);
            Sub(a02a13, a03a12, u5);

            std::size_t constexpr a20a31 = 16, a21a30 = 17, v0 = 24;
            std::size_t constexpr a20a32 = 16, a22a30 = 17, v1 = 25;
            std::size_t constexpr a20a33 = 16, a23a30 = 17, v2 = 26;
            std::size_t constexpr a21a32 = 16, a22a31 = 17, v3 = 27;
            std::size_t constexpr a21a33 = 16, a23a31 = 17, v4 = 28;
            std::size_t constexpr a22a33 = 16, a23a32 = 17, v5 = 29;

            Mul(a20, a31, a20a31);
            Mul(a21, a30, a21a30);
            Sub(a20a31, a21a30, v0);
            Mul(a20, a32, a20a32);
            Mul(a22, a30, a22a30);
            Sub(a20a32, a22a30, v1);
            Mul(a20, a33, a20a33);
            Mul(a23, a30, a23a30);
            Sub(a20a33, a23a30, v2);
            Mul(a21, a32, a21a32);
            Mul(a22, a31, a22a31);
            Sub(a21a32, a22a31, v3);
            Mul(a21, a33, a21a33);
            Mul(a23, a31, a23a31);
            Sub(a21a33, a23a31, v4);
            Mul(a22, a33, a22a33);
            Mul(a23, a32, a23a32);
            Sub(a22a33, a23a32, v5);

            std::size_t constexpr u0v5 = 0, u1v4 = 1, u2v3 = 2;
            std::size_t constexpr u3v2 = 3, u4v1 = 4, u5v0 = 5;
            std::size_t constexpr b0 = 6, b1 = 7, b2 = 8, sum = 1;

            Mul(u0, v5, u0v5);
            Mul(u1, v4, u1v4);
            Mul(u2, v3, u2v3);
            Mul(u3, v2, u3v2);
            Mul(u4, v1, u4v1);
            Mul(u5, v0, u5v0);

            Add(u0v5, u5v0, b0);
            Add(u1v4, u4v1, b1);
            Add(u2v3, u3v2, b2);
            Add(b0, b2, sum);
            Sub(sum, b1, detNode);

            mRSign = mNode[detNode].GetSign();
        }

        void ComputeRational(
            T const& a00, T const& a01, T const& a02, T const& a03,
            T const& a10, T const& a11, T const& a12, T const& a13,
            T const& a20, T const& a21, T const& a22, T const& a23,
            T const& a30, T const& a31, T const& a32, T const& a33)
        {
            mNode[0] = a00;
            mNode[1] = a01;
            mNode[2] = a02;
            mNode[3] = a03;
            mNode[4] = a10;
            mNode[5] = a11;
            mNode[6] = a12;
            mNode[7] = a13;
            mNode[8] = a20;
            mNode[9] = a21;
            mNode[10] = a22;
            mNode[11] = a23;
            mNode[12] = a30;
            mNode[13] = a31;
            mNode[14] = a32;
            mNode[15] = a33;
            ComputeRational();
        }

        void ComputeRational(
            Rational const& ra00, Rational const& ra01, Rational const& ra02, Rational const& ra03,
            Rational const& ra10, Rational const& ra11, Rational const& ra12, Rational const& ra13,
            Rational const& ra20, Rational const& ra21, Rational const& ra22, Rational const& ra23,
            Rational const& ra30, Rational const& ra31, Rational const& ra32, Rational const& ra33)
        {
            mNode[0] = ra00;
            mNode[1] = ra01;
            mNode[2] = ra02;
            mNode[3] = ra03;
            mNode[4] = ra10;
            mNode[5] = ra11;
            mNode[6] = ra12;
            mNode[7] = ra13;
            mNode[8] = ra20;
            mNode[9] = ra21;
            mNode[10] = ra22;
            mNode[11] = ra23;
            mNode[12] = ra30;
            mNode[13] = ra31;
            mNode[14] = ra32;
            mNode[15] = ra33;
            ComputeRational();
        }

        static std::int32_t constexpr invalidSign = std::numeric_limits<std::int32_t>::max();
        static std::size_t constexpr numNodes = 30;
        static std::size_t constexpr detNode = 0;
        std::int32_t mISign;
        std::int32_t mRSign;
        SWInterval<T> mIDet;
        std::vector<CRational> mNode;

    private:
        friend class UnitTestExactSignDeterminant4;
    };
}
