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
// expression tree number of nodes = 6
//
// compute type = float
//   N = 18
//   sizeof(BSNumber<UIntegerFP32<18>>) = 88
//   number of heap bytes for rational computing = 528 = 6 * 88
//
// compute type = double
//   N = 132
//   sizeof(BSNumber<UIntegerFP32<132>>) = 544
//   number of heap bytes for rational computing = 3264 = 6 * 544
//
// The expression-tree nodes are allocated on the heap. The N-values from the
// precision calculator are rounded up to an even number so that the bit
// storage of UIntegerFP32<N> bit storage is a block of memory whose number of
// bytes is a multiple of 8. The 'bytes' numbers are for the heap memory of
// std::vector<BSNumber<UIntegerFP32<N>> but does not include the stack bytes
// for the members of std::vector. The mNode[] std::vector object is resized
// on demand the first time it is needed by queries.
//
// The member functions with only T-valued arguments are for floating-point
// inputs. The member functions with T-valued and Rational-valued arguments
// are intended for applications where the Rational inputs are cached and
// re-used to avoid re-converting floating-point numbers to rational numbers.

#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#include <GTL/Mathematics/Arithmetic/SWInterval.h>
#include <GTL/Mathematics/Algebra/Matrix.h>
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
    class ExactSignDeterminant2
    {
    public:
        using Rational = BSNumber<UIntegerFP32<2>>;

        ExactSignDeterminant2()
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
            T const& a00, T const& a01,
            T const& a10, T const& a11)
        {
            mISign = invalidSign;
            mRSign = invalidSign;

            // Use interval arithmetic to determine the sign of the
            // determinant, if possible.
            ComputeInterval(a00, a01, a10, a11);
            if (mISign != invalidSign)
            {
                return mISign;
            }

            // The exact sign of the determinant is not known. Compute the
            // determinant using rational arithmetic.
            ComputeRational(a00, a01, a10, a11);
            return mRSign;
        }

        std::int32_t operator()(
            T const& a00, T const& a01,
            T const& a10, T const& a11,
            std::function<std::array<Rational const*, 4>()> const& GetRValues)
        {
            mISign = invalidSign;
            mRSign = invalidSign;

            // Use interval arithmetic to determine the sign of the
            // determinant, if possible.
            ComputeInterval(a00, a01, a10, a11);
            if (mISign != invalidSign)
            {
                return mISign;
            }

            // The exact sign of the determinant is not known. Compute the
            // determinant using rational arithmetic.
            std::array<Rational const*, 4> rValues = GetRValues();
            auto const& ra00 = *rValues[0];
            auto const& ra01 = *rValues[1];
            auto const& ra10 = *rValues[2];
            auto const& ra11 = *rValues[3];
            ComputeRational(ra00, ra01, ra10, ra11);
            return mRSign;
        }

    private:
        void ComputeInterval(
            T const& a00, T const& a01,
            T const& a10, T const& a11)
        {
            auto a00a11 = SWInterval<T>::Mul(a00, a11);
            auto a01a10 = SWInterval<T>::Mul(a01, a10);
            mIDet = a00a11 - a01a10;

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

        static std::size_t constexpr numWords = std::is_same<T, float>::value ? 18 : 132;
        using CRational = BSNumber<UIntegerFP32<numWords>>;

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
            std::size_t constexpr a00 = 0, a01 = 1, a10 = 2, a11 = 3;
            std::size_t constexpr a00a11 = 4, a01a10 = 5;

            Mul(a00, a11, a00a11);
            Mul(a01, a10, a01a10);
            Sub(a00a11, a01a10, detNode);

            mRSign = mNode[detNode].GetSign();
        }

        void ComputeRational(
            T const& a00, T const& a01,
            T const& a10, T const& a11)
        {
            mNode[0] = a00;
            mNode[1] = a01;
            mNode[2] = a10;
            mNode[3] = a11;
            ComputeRational();
        }

        void ComputeRational(
            Rational const& ra00, Rational const& ra01,
            Rational const& ra10, Rational const& ra11)
        {
            mNode[0] = ra00;
            mNode[1] = ra01;
            mNode[2] = ra10;
            mNode[3] = ra11;
            ComputeRational();
        }

        static std::int32_t constexpr invalidSign = std::numeric_limits<std::int32_t>::max();
        static std::size_t constexpr numNodes = 6;
        static std::size_t constexpr detNode = 0;
        std::int32_t mISign;
        std::int32_t mRSign;
        SWInterval<T> mIDet;
        std::vector<CRational> mNode;

    private:
        friend class UnitTestExactSignDeterminant2;
    };
}
