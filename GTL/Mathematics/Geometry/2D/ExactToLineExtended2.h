// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// An extended classification of the relationship of a point to a line
// segment. The input type T must satisfy std::is_floating_point<T>::value =
// true. The compute type is BSNumber<UIntegerFP32<N>>, where N depends on
// the input type and the expression tree of the query. The determination of
// worst-case N is by the GeometricTools/GTL/Tools/PrecisionCalculator
// tool. The N-values are conservative so that the number of bits for the
// query is sufficient for any finite floating-point inputs.
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
    class ExactToLineExtended2
    {
    public:
        using Rational = BSNumber<UIntegerFP32<2>>;

        ExactToLineExtended2()
            :
            mIOrder(OrderType::UNKNOWN),
            mROrder(OrderType::UNKNOWN),
            mNode(numNodes)
        {
            static_assert(
                std::is_floating_point<T>::value,
                "The input type must be a floating-point type.");
        }

        enum class OrderType
        {
            UNKNOWN,            // initial value
            V0_EQUALS_V1,       // V0 = V1, degenerate segment (a point)
            P_EQUALS_V0,        // P = V0, P is endpoint of segment
            P_EQUALS_V1,        // P = V1, P is endpoint of segment
            P_RIGHT_OF_V0_V1,   // P is right-of line <V0,V1>
            P_LEFT_OF_V0_V1,    // P is left-of line <V0,V1>
            COLLINEAR_LEFT,     // point ordering on line is <P,V0,V1>
            COLLINEAR_RIGHT,    // point ordering on line is <V0,V1,P>
            COLLINEAR_CONTAIN   // point ordering on line is <V0,P,V1>
        };

        OrderType operator()(
            Vector2<T> const& P, Vector2<T> const& V0, Vector2<T> const& V1)
        {
            mIOrder = OrderType::UNKNOWN;
            mROrder = OrderType::UNKNOWN;

            // Use interval arithmetic to determine the relative location of
            // P, if possible.
            if (ComputeInterval(P, V0, V1, mIOrder))
            {
                return mIOrder;
            }

            // The exact relative location of P is not known. Compute the
            // relative location using rational arithmetic.
            ComputeRational(P, V0, V1);
            return mROrder;
        }

        OrderType operator()(
            Vector2<T> const& P, Vector2<T> const& V0, Vector2<T> const& V1,
            std::function<std::array<Vector2<Rational> const*, 3>()> const& GetRPoints)
        {
            mIOrder = OrderType::UNKNOWN;
            mROrder = OrderType::UNKNOWN;

            // Use interval arithmetic to determine the relative location of
            // P, if possible.
            if (ComputeInterval(P, V0, V1, mIOrder))
            {
                return mIOrder;
            }

            // The exact relative location of P is not known. Compute the
            // relative location using rational arithmetic.
            std::array<Vector2<Rational> const*, 3> rPoints = GetRPoints();
            auto const& rP = *rPoints[0];
            auto const& rV0 = *rPoints[1];
            auto const& rV1 = *rPoints[2];
            ComputeRational(rP, rV0, rV1);
            return mROrder;
        }

    private:
        bool ComputeInterval(Vector2<T> const& P, Vector2<T> const& V0,
            Vector2<T> const& V1, OrderType& order)
        {
            // The exact equality tests can be performed using floating-point
            // arithmetic.
            if (V0 == V1)
            {
                order = OrderType::V0_EQUALS_V1;
                return true;
            }

            if (P == V0)
            {
                order = OrderType::P_EQUALS_V0;
                return true;
            }

            if (P == V1)
            {
                order = OrderType::P_EQUALS_V1;
                return true;
            }

            // (x0,y0) = V1 - V0, (x1,y1) = P - V0
            auto x0 = SWInterval<T>::Sub(V1[0], V0[0]);
            auto y0 = SWInterval<T>::Sub(V1[1], V0[1]);
            auto x1 = SWInterval<T>::Sub(P[0], V0[0]);
            auto y1 = SWInterval<T>::Sub(P[1], V0[1]);
            auto x0y1 = x0 * y1;
            auto x1y0 = x1 * y0;
            auto det = x0y1 - x1y0;

            if (det[0] > C_<T>(0))
            {
                order = OrderType::P_LEFT_OF_V0_V1;
                return true;
            }
            else if (det[1] < C_<T>(0))
            {
                order = OrderType::P_RIGHT_OF_V0_V1;
                return true;
            }
            else
            {
                // Although it is possible to detect the case when det[i] = 0
                // for both i, the number of FPU rounding-mode changes are
                // expensive. It is better just to fallback to rational
                // arithmetic, which is not expensive for this query.
                order = OrderType::UNKNOWN;
                return false;
            }
        }

        static std::size_t constexpr numWords = std::is_same<T, float>::value ? 18 : 132;
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
            CRational const rzero(0);
            std::size_t constexpr p0 = 0, p1 = 1, v00 = 2, v01 = 3, v10 = 4, v11 = 5;
            std::size_t constexpr x0 = 6, y0 = 7, x1 = 8, y1 = 9;
            std::size_t constexpr x0y1 = 1, x1y0 = 2;

            Sub(v10, v00, x0);
            Sub(v11, v01, y0);
            Sub(p0, v00, x1);
            Sub(p1, v01, y1);
            Mul(x0, y1, x0y1);
            Mul(x1, y0, x1y0);
            Sub(x0y1, x1y0, detNode);
            if (mNode[detNode].GetSign() > 0)
            {
                // The points form a counterclockwise triangle <P,V0,V1>.
                mROrder = OrderType::P_LEFT_OF_V0_V1;
                mNode[dotNode] = rzero;
                mNode[sqrLengthNode] = rzero;
                return;
            }
            if (mNode[detNode].GetSign() < 0)
            {
                // The points form a clockwise triangle <P,V1,V0>.
                mROrder = OrderType::P_RIGHT_OF_V0_V1;
                mNode[dotNode] = rzero;
                mNode[sqrLengthNode] = rzero;
                return;
            }

            // The points are collinear. Determine their ordering along the
            // containing line.
            std::size_t constexpr x0x1 = 0, y0y1 = 1;

            Mul(x0, x1, x0x1);
            Mul(y0, y1, y0y1);
            Add(x0x1, y0y1, dotNode);
            if (mNode[dotNode].GetSign() < 0)
            {
                // The line ordering is <P,V0,V1>.
                mROrder = OrderType::COLLINEAR_LEFT;
                mNode[sqrLengthNode] = rzero;
                return;
            }

            std::size_t constexpr x0x0 = 0, y0y0 = 1;

            Mul(x0, x0, x0x0);
            Mul(y0, y0, y0y0);
            Add(x0x0, y0y0, sqrLengthNode);
            if (mNode[dotNode] > mNode[sqrLengthNode])
            {
                // The line ordering is <V0,V1,P>.
                mROrder = OrderType::COLLINEAR_RIGHT;
                return;
            }

            // The line ordering is <V0,P,V1> with P strictly between
            // V0 and V1.
            mROrder = OrderType::COLLINEAR_CONTAIN;
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
        static std::size_t constexpr detNode = 0, dotNode = 2, sqrLengthNode = 3;
        OrderType mIOrder;
        OrderType mROrder;
        std::vector<CRational> mNode;

    private:
        friend class UnitTestExactToLineExtended2;
    };
}
