// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// For a triangle with counterclockwise vertices V0, V1 and V2, operator()
// returns
//   +1, P outside triangle
//   -1, P inside triangle
//    0, P on triangle
// The input type T must satisfy std::is_floating_point<T>::value = true. The
// compute type is BSNumber<UIntegerFP32<N>>, where N depends on the input
// type and the expression tree of the query. The determination of worst-case
// N is by the GeometricTools/GTL/Tools/PrecisionCalculator tool. The
// N-values are conservative so that the number of bits for the query is
// sufficient for any finite floating-point inputs.
//
// expression tree number of nodes = 17
//
// compute type = float
//   N = 18
//   sizeof(BSNumber<UIntegerFP32<18>>) = 88
//   number of heap bytes for rational computing = 1232 = 17 * 88
//
// compute type = double
//   N = 132
//   sizeof(BSNumber<UIntegerFP32<132>>) = 544
//   number of heap bytes for rational computing = 7616 = 17 * 544
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
    class ExactToTriangle2
    {
    public:
        using Rational = BSNumber<UIntegerFP32<2>>;

        ExactToTriangle2()
            :
            mISign01(invalidSign),
            mISign12(invalidSign),
            mISign20(invalidSign),
            mRSign01(invalidSign),
            mRSign12(invalidSign),
            mRSign20(invalidSign),
            mIDet01{},
            mIDet12{},
            mIDet20{},
            mNode(numNodes)
        {
            static_assert(
                std::is_floating_point<T>::value,
                "The input type must be a floating-point type.");
        }

        std::int32_t operator()(Vector2<T> const& P, Vector2<T> const& V0,
            Vector2<T> const& V1, Vector2<T> const& V2)
        {
            mISign01 = invalidSign;
            mISign12 = invalidSign;
            mISign20 = invalidSign;
            mRSign01 = invalidSign;
            mRSign12 = invalidSign;
            mRSign20 = invalidSign;

            // Use interval arithmetic to determine the relative location of
            // P, if possible.
            std::int32_t sign = ComputeInterval(P, V0, V1, V2);
            if (sign != invalidSign)
            {
                return sign;
            }

            // The exact relative location of P is not known. Compute the
            // relative location using rational arithmetic.
            return ComputeRational(P, V0, V1, V2);
        }

        std::int32_t operator()(Vector2<T> const& P, Vector2<T> const& V0,
            Vector2<T> const& V1, Vector2<T> const& V2,
            std::function<std::array<Vector2<Rational> const*, 4>()> const& GetRPoints)
        {
            mISign01 = invalidSign;
            mISign12 = invalidSign;
            mISign20 = invalidSign;
            mRSign01 = invalidSign;
            mRSign12 = invalidSign;
            mRSign20 = invalidSign;

            // Use interval arithmetic to determine the relative location of P
            // if possible.
            std::int32_t sign = ComputeInterval(P, V0, V1, V2);
            if (sign != invalidSign)
            {
                return sign;
            }

            // The exact relative location of P is not known. Compute the
            // relative location using rational arithmetic.
            std::array<Vector2<Rational> const*, 4> rPoints = GetRPoints();
            auto const& rP = *rPoints[0];
            auto const& rV0 = *rPoints[1];
            auto const& rV1 = *rPoints[2];
            auto const& rV2 = *rPoints[3];
            return ComputeRational(rP, rV0, rV1, rV2);
        }

    private:
        std::int32_t ComputeInterval(Vector2<T> const& P, Vector2<T> const& V0,
            Vector2<T> const& V1, Vector2<T> const& V2)
        {
            mIDet01 = { C_<T>(0), C_<T>(0) };
            mIDet12 = { C_<T>(0), C_<T>(0) };
            mIDet20 = { C_<T>(0), C_<T>(0) };

            // Test whether P is outside the triangle via edge <V0,V1>.
            auto x0 = SWInterval<T>::Sub(P[0], V0[0]);
            auto y0 = SWInterval<T>::Sub(P[1], V0[1]);
            auto z0 = SWInterval<T>::Sub(V1[0], V0[0]);
            auto w0 = SWInterval<T>::Sub(V1[1], V0[1]);
            auto a0b1 = x0 * w0;
            auto a1b0 = z0 * y0;
            mIDet01 = a0b1 - a1b0;
            if (mIDet01[0] > C_<T>(0))
            {
                mISign01 = +1;
                return +1;
            }
            mISign01 = (mIDet01[1] < C_<T>(0) ? -1 : invalidSign);

            // Test whether P is outside the triangle via edge <V1,V2>.
            auto x1 = SWInterval<T>::Sub(P[0], V1[0]);
            auto y1 = SWInterval<T>::Sub(P[1], V1[1]);
            auto z1 = SWInterval<T>::Sub(V2[0], V1[0]);
            auto w1 = SWInterval<T>::Sub(V2[1], V1[1]);
            a0b1 = x1 * w1;
            a1b0 = z1 * y1;
            mIDet12 = a0b1 - a1b0;
            if (mIDet12[0] > C_<T>(0))
            {
                mISign12 = +1;
                return +1;
            }
            mISign12 = (mIDet12[1] < C_<T>(0) ? -1 : invalidSign);

            // Test whether P is outside the triangle via edge <V2,V0>.
            auto x2 = SWInterval<T>::Sub(P[0], V2[0]);
            auto y2 = SWInterval<T>::Sub(P[1], V2[1]);
            auto z2 = SWInterval<T>::Sub(V0[0], V2[0]);
            auto w2 = SWInterval<T>::Sub(V0[1], V2[1]);
            a0b1 = x2 * w2;
            a1b0 = z2 * y2;
            mIDet20 = a0b1 - a1b0;
            if (mIDet20[0] > C_<T>(0))
            {
                mISign20 = +1;
                return +1;
            }
            mISign20 = (mIDet20[1] < C_<T>(0) ? -1 : invalidSign);

            // If all signs are -1, P is inside the triangle. If at least one
            // sign is invalidSign, it is unknown how P is located relative to
            // the triangle.
            if (mISign01 == -1 && mISign12 == -1 && mISign20 == -1)
            {
                return -1;
            }
            else
            {
                return invalidSign;
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

        std::int32_t ComputeRational()
        {
            std::size_t constexpr p0 = 0, p1 = 1, v00 = 2, v01 = 3;
            std::size_t constexpr v10 = 4, v11 = 5, v20 = 6, v21 = 7;

            // Test whether P is outside the triangle via edge <V0,V1>.
            // (x0,y0) = P - V0, (z0,w0) = V1 - V0
            std::size_t constexpr x0 = 8, y0 = 9, z0 = 10, w0 = 11, x0w0 = 12, z0y0 = 13;
            Sub(p0, v00, x0);
            Sub(p1, v01, y0);
            Sub(v10, v00, z0);
            Sub(v11, v01, w0);
            Mul(x0, w0, x0w0);
            Mul(z0, y0, z0y0);
            Sub(x0w0, z0y0, det01Node);
            mRSign01 = mNode[det01Node].GetSign();
            if (mRSign01 > 0)
            {
                return +1;
            }

            // Test whether P is outside the triangle via edge <V1,V2>.
            // (x1,y1) = P - V1, (z1,w1) = V2 - V1
            std::size_t constexpr x1 = 8, y1 = 9, z1 = 10, w1 = 11, x1w1 = 12, z1y1 = 13;
            Sub(p0, v10, x1);
            Sub(p1, v11, y1);
            Sub(v20, v10, z1);
            Sub(v21, v11, w1);
            Mul(x1, w1, x1w1);
            Mul(z1, y1, z1y1);
            Sub(x1w1, z1y1, det12Node);
            mRSign12 = mNode[det12Node].GetSign();
            if (mRSign12 > 0)
            {
                return +1;
            }

            // Test whether P is outside the triangle via edge <V2,V0>.
            // (x2,y2) = P - V2, (z2,w2) = V0 - V2
            std::size_t constexpr x2 = 8, y2 = 9, z2 = 10, w2 = 11, x2w2 = 12, z2y2 = 13;
            Sub(p0, v20, x2);
            Sub(p1, v21, y2);
            Sub(v00, v20, z2);
            Sub(v01, v21, w2);
            Mul(x2, w2, x2w2);
            Mul(z2, y2, z2y2);
            Sub(x2w2, z2y2, det20Node);
            mRSign20 = mNode[det20Node].GetSign();
            if (mRSign12 > 0)
            {
                return +1;
            }

            // If all signs are -1, P is inside the triangle; otherwise,
            // P is on a triangle edge.
            return ((mRSign01 && mRSign12 && mRSign20) ? -1 : 0);
        }

        std::int32_t ComputeRational(Vector2<T> const& P, Vector2<T> const& V0,
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
            return ComputeRational();
        }

        std::int32_t ComputeRational(Vector2<Rational> const& rP,
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
            return ComputeRational();
        }

        static std::int32_t constexpr invalidSign = std::numeric_limits<std::int32_t>::max();
        static std::size_t constexpr numNodes = 17;
        static std::size_t constexpr det01Node = 14, det12Node = 15, det20Node = 16;
        std::int32_t mISign01, mISign12, mISign20;
        std::int32_t mRSign01, mRSign12, mRSign20;
        SWInterval<T> mIDet01, mIDet12, mIDet20;
        std::vector<CRational> mNode;

    private:
        friend class UnitTestExactToTriangle2;
    };
}
