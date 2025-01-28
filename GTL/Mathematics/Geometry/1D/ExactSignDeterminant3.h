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
// expression tree number of nodes = 14
//
// compute type = float
//   N = 28
//   sizeof(BSNumber<UIntegerFP32<28>>) = 128
//   number of heap bytes for rational computing = 1792 = 14 * 128
//
// compute type = double
//   N = 198
//   sizeof(BSNumber<UIntegerFP32<198>>) = 808
//   number of heap bytes for rational computing = 11312 = 14 * 808
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
#include <limits>
#include <type_traits>
#include <vector>

namespace gtl
{
    template <typename T>
    class ExactSignDeterminant3
    {
    public:
        using Rational = BSNumber<UIntegerFP32<2>>;

        ExactSignDeterminant3()
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
            T const& a00, T const& a01, T const& a02,
            T const& a10, T const& a11, T const& a12,
            T const& a20, T const& a21, T const& a22)
        {
            mISign = invalidSign;
            mRSign = invalidSign;

            // Use interval arithmetic to determine the sign of the
            // determinant, if possible.
            ComputeInterval(a00, a01, a02, a10, a11, a12, a20, a21, a22);
            if (mISign != invalidSign)
            {
                return mISign;
            }

            // The exact sign of the determinant is not known. Compute the
            // determinant using rational arithmetic.
            ComputeRational(a00, a01, a02, a10, a11, a12, a20, a21, a22);
            return mRSign;
        }

        std::int32_t operator()(
            T const& a00, T const& a01, T const& a02,
            T const& a10, T const& a11, T const& a12,
            T const& a20, T const& a21, T const& a22,
            std::function<std::array<Rational const*, 9>()> const& GetRValues)
        {
            mISign = invalidSign;
            mRSign = invalidSign;

            // Use interval arithmetic to determine the sign of the
            // determinant, if possible.
            ComputeInterval(a00, a01, a02, a10, a11, a12, a20, a21, a22);
            if (mISign != invalidSign)
            {
                return mISign;
            }

            // The exact sign of the determinant is not known. Compute the
            // determinant using rational arithmetic.
            std::array<Rational const*, 9> rValues = GetRValues();
            auto const& ra00 = *rValues[0];
            auto const& ra01 = *rValues[1];
            auto const& ra02 = *rValues[2];
            auto const& ra10 = *rValues[3];
            auto const& ra11 = *rValues[4];
            auto const& ra12 = *rValues[5];
            auto const& ra20 = *rValues[6];
            auto const& ra21 = *rValues[7];
            auto const& ra22 = *rValues[8];
            ComputeRational(ra00, ra01, ra02, ra10, ra11, ra12, ra20, ra21, ra22);
            return mRSign;
        }

    private:
        void ComputeInterval(
            T const& a00, T const& a01, T const& a02,
            T const& a10, T const& a11, T const& a12,
            T const& a20, T const& a21, T const& a22)
        {
            auto a10a21 = SWInterval<T>::Mul(a10, a21);
            auto a10a22 = SWInterval<T>::Mul(a10, a22);
            auto a11a20 = SWInterval<T>::Mul(a11, a20);
            auto a11a22 = SWInterval<T>::Mul(a11, a22);
            auto a12a20 = SWInterval<T>::Mul(a12, a20);
            auto a12a21 = SWInterval<T>::Mul(a12, a21);
            auto b0 = a11a22 - a12a21;
            auto b1 = a12a20 - a10a22;
            auto b2 = a10a21 - a11a20;
            auto c0 = a00 * b0;
            auto c1 = a01 * b1;
            auto c2 = a02 * b2;
            mIDet = c0 + c1 + c2;

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

        static std::size_t constexpr numWords = std::is_same<T, float>::value ? 28 : 198;
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
            std::size_t constexpr a00 = 0, a01 = 1, a02 = 2;
            std::size_t constexpr a10 = 3, a11 = 4, a12 = 5;
            std::size_t constexpr a20 = 6, a21 = 7, a22 = 8;
            std::size_t constexpr a11a22 = 9, a12a21 = 10, b0 = 11;
            std::size_t constexpr a12a20 = 9, a10a22 = 10, b1 = 12;
            std::size_t constexpr a10a21 = 9, a11a20 = 10, b2 = 13;
            std::size_t constexpr c0 = 3, c1 = 4, c2 = 5, sum = 1;

            Mul(a11, a22, a11a22);
            Mul(a12, a21, a12a21);
            Sub(a11a22, a12a21, b0);
            Mul(a12, a20, a12a20);
            Mul(a10, a22, a10a22);
            Sub(a12a20, a10a22, b1);
            Mul(a10, a21, a10a21);
            Mul(a11, a20, a11a20);
            Sub(a10a21, a11a20, b2);
            Mul(a00, b0, c0);
            Mul(a01, b1, c1);
            Mul(a02, b2, c2);
            Add(c0, c1, sum);
            Add(sum, c2, detNode);

            mRSign =  mNode[detNode].GetSign();
        }

        void ComputeRational(
            T const& a00, T const& a01, T const& a02,
            T const& a10, T const& a11, T const& a12,
            T const& a20, T const& a21, T const& a22)
        {
            mNode[0] = a00;
            mNode[1] = a01;
            mNode[2] = a02;
            mNode[3] = a10;
            mNode[4] = a11;
            mNode[5] = a12;
            mNode[6] = a20;
            mNode[7] = a21;
            mNode[8] = a22;
            ComputeRational();
        }

        void ComputeRational(
            Rational const& ra00, Rational const& ra01, Rational const& ra02,
            Rational const& ra10, Rational const& ra11, Rational const& ra12,
            Rational const& ra20, Rational const& ra21, Rational const& ra22)
        {
            mNode[0] = ra00;
            mNode[1] = ra01;
            mNode[2] = ra02;
            mNode[3] = ra10;
            mNode[4] = ra11;
            mNode[5] = ra12;
            mNode[6] = ra20;
            mNode[7] = ra21;
            mNode[8] = ra22;
            ComputeRational();
        }

        static std::int32_t constexpr invalidSign = std::numeric_limits<std::int32_t>::max();
        static std::size_t constexpr numNodes = 14;
        static std::size_t constexpr detNode = 0;
        std::int32_t mISign;
        std::int32_t mRSign;
        SWInterval<T> mIDet;
        std::vector<CRational> mNode;

    private:
        friend class UnitTestExactSignDeterminant3;
    };
}
