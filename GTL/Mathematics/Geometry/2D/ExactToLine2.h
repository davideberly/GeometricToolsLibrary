// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// For a line with origin V0 and direction V1-V0, operator() returns
//   +1, P on right of line
//   -1, P on left of line
//    0, P on the line
// The input type T must satisfy std::is_floating_point<T>::value = true. The
// compute type is BSNumber<UIntegerFP32<N>>, where N depends on the input
// type and the expression tree of the query. The determination of worst-case
// N is by the GeometricTools/GTL/Tools/PrecisionCalculator tool. The
// N-values are conservative so that the number of bits for the query is
// sufficient for any finite floating-point inputs.
//
// expression tree number of nodes = 10
//
// compute type = float
//   N = 18
//   sizeof(BSNumber<UIntegerFP32<18>>) = 88
//   number of heap bytes for rational computing = 880 = 10 * 88
//
// compute type = double
//   N = 132
//   sizeof(BSNumber<UIntegerFP32<132>>) = 544
//   number of heap bytes for rational computing = 5440 = 10 * 544
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
    class ExactToLine2
    {
    public:
        using Rational = BSNumber<UIntegerFP32<2>>;

        ExactToLine2()
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

        std::int32_t operator()(Vector2<T> const& P, Vector2<T> const& V0, Vector2<T> const& V1)
        {
            mISign = invalidSign;
            mRSign = invalidSign;

            // Use interval arithmetic to determine the relative location of
            // P, if possible.
            ComputeInterval(P, V0, V1);
            if (mISign != invalidSign)
            {
                return mISign;
            }

            // The exact relative location of P is not known. Compute the
            // relative location using rational arithmetic.
            ComputeRational(P, V0, V1);
            return mRSign;
        }

        std::int32_t operator()(Vector2<T> const& P, Vector2<T> const& V0, Vector2<T> const& V1,
            std::function<std::array<Vector2<Rational> const*, 3>()> const& GetRPoints)
        {
            mISign = invalidSign;
            mRSign = invalidSign;

            // Use interval arithmetic to determine the relative location of
            // P, if possible.
            ComputeInterval(P, V0, V1);
            if (mISign != invalidSign)
            {
                return mISign;
            }

            // The exact relative location of P is not known. Compute the
            // relative location using rational arithmetic.
            std::array<Vector2<Rational> const*, 3> rPoints = GetRPoints();
            auto const& rP = *rPoints[0];
            auto const& rV0 = *rPoints[1];
            auto const& rV1 = *rPoints[2];
            ComputeRational(rP, rV0, rV1);
            return mRSign;
        }

    private:
        void ComputeInterval(Vector2<T> const& P, Vector2<T> const& V0,
            Vector2<T> const& V1)
        {
            auto x0 = SWInterval<T>::Sub(P[0], V0[0]);
            auto y0 = SWInterval<T>::Sub(P[1], V0[1]);
            auto x1 = SWInterval<T>::Sub(V1[0], V0[0]);
            auto y1 = SWInterval<T>::Sub(V1[1], V0[1]);
            auto x0y1 = x0 * y1;
            auto x1y0 = x1 * y0;
            mIDet = x0y1 - x1y0;

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
            std::size_t constexpr p0 = 0, p1 = 1, v00 = 2, v01 = 3, v10 = 4, v11 = 5;
            std::size_t constexpr x0 = 6, y0 = 7, x1 = 8, y1 = 9;
            std::size_t constexpr x0y1 = 1, x1y0 = 2;

            Sub(p0, v00, x0);
            Sub(p1, v01, y0);
            Sub(v10, v00, x1);
            Sub(v11, v01, y1);
            Mul(x0, y1, x0y1);
            Mul(x1, y0, x1y0);
            Sub(x0y1, x1y0, detNode);

            mRSign = mNode[detNode].GetSign();
        }

        void ComputeRational(Vector2<T> const& P, Vector2<T> const& V0,
            Vector2<T> const& V1)
        {
            mNode[0] = P[0];
            mNode[1] = P[1];
            mNode[2] = V0[0];
            mNode[3] = V0[1];
            mNode[4] = V1[0];
            mNode[5] = V1[1];
            ComputeRational();
        }

        void ComputeRational(Vector2<Rational> const& rP,
            Vector2<Rational> const& rV0, Vector2<Rational> const& rV1)
        {
            mNode[0] = rP[0];
            mNode[1] = rP[1];
            mNode[2] = rV0[0];
            mNode[3] = rV0[1];
            mNode[4] = rV1[0];
            mNode[5] = rV1[1];
            ComputeRational();
        }

        static std::int32_t constexpr invalidSign = std::numeric_limits<std::int32_t>::max();
        static std::size_t constexpr numNodes = 10;
        static std::size_t constexpr detNode = 0;
        std::int32_t mISign;
        std::int32_t mRSign;
        SWInterval<T> mIDet;
        std::vector<CRational> mNode;

    private:
        friend class UnitTestExactToLine2;
    };
}
