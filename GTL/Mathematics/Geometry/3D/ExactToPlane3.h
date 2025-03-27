// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// For a plane with origin V0 and normal N = Cross(V1-V0,V2-V0), operator()
// returns
//   +1, P on positive side of plane (side to which N points)
//   -1, P on negative side of plane (side to which -N points)
//    0, P on the plane
// The input type T must satisfy std::is_floating_point<T>::value = true. The
// compute type is BSNumber<UIntegerFP32<N>>, where N depends on the input
// type and the expression tree of the query. The determination of worst-case
// N is by the GeometricTools/GTL/Tools/PrecisionCalculator tool. The
// N-values are conservative so that the number of bits for the query is
// sufficient for any finite floating-point inputs.
//
// expression tree number of nodes = 23
//
// compute type = float
//   N = 28
//   sizeof(BSNumber<UIntegerFP32<18>>) = 128
//   number of heap bytes for rational computing = 2944 = 23 * 128
//
// compute type = double
//   N = 198
//   sizeof(BSNumber<UIntegerFP32<198>>) = 808
//   number of heap bytes for rational computing = 18584 = 23 * 808
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
    class ExactToPlane3
    {
    public:
        using Rational = BSNumber<UIntegerFP32<2>>;

        ExactToPlane3()
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

        std::int32_t operator()(Vector3<T> const& P, Vector3<T> const& V0,
            Vector3<T> const& V1, Vector3<T> const& V2)
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

        std::int32_t operator()(Vector3<T> const& P, Vector3<T> const& V0,
            Vector3<T> const& V1, Vector3<T> const& V2,
            std::function<std::array<Vector3<Rational> const*, 4>()> const& GetRPoints)
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
            std::array<Vector3<Rational> const*, 4> rPoints = GetRPoints();
            auto const& rP = *rPoints[0];
            auto const& rV0 = *rPoints[1];
            auto const& rV1 = *rPoints[2];
            auto const& rV2 = *rPoints[3];
            ComputeRational(rP, rV0, rV1, rV2);
            return mRSign;
        }

    private:
        void ComputeInterval(Vector3<T> const& P, Vector3<T> const& V0,
            Vector3<T> const& V1, Vector3<T> const& V2)
        {
            auto x0 = SWInterval<T>::Sub(P[0], V0[0]);
            auto y0 = SWInterval<T>::Sub(P[1], V0[1]);
            auto z0 = SWInterval<T>::Sub(P[2], V0[2]);
            auto x1 = SWInterval<T>::Sub(V1[0], V0[0]);
            auto y1 = SWInterval<T>::Sub(V1[1], V0[1]);
            auto z1 = SWInterval<T>::Sub(V1[2], V0[2]);
            auto x2 = SWInterval<T>::Sub(V2[0], V0[0]);
            auto y2 = SWInterval<T>::Sub(V2[1], V0[1]);
            auto z2 = SWInterval<T>::Sub(V2[2], V0[2]);
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
            std::size_t constexpr p0 = 0, p1 = 1, p2 = 2;
            std::size_t constexpr v00 = 3, v01 = 4, v02 = 5;
            std::size_t constexpr v10 = 6, v11 = 7, v12 = 8;
            std::size_t constexpr v20 = 9, v21 = 10, v22 = 11;

            std::size_t constexpr y0 = 12, z0 = 13, y1 = 14, z1 = 15, y2 = 16, z2 = 17;
            Sub(p1, v01, y0);
            Sub(p2, v02, z0);
            Sub(p1, v11, y1);
            Sub(p2, v12, z1);
            Sub(p1, v21, y2);
            Sub(p2, v22, z2);

            std::size_t constexpr y1z2 = 18, y2z1 = 19, c0 = 20;
            std::size_t constexpr y2z0 = 18, y0z2 = 19, c1 = 21;
            std::size_t constexpr y0z1 = 18, y1z0 = 19, c2 = 22;
            Mul(y1, z2, y1z2);
            Mul(y2, z1, y2z1);
            Sub(y1z2, y2z1, c0);
            Mul(y2, z0, y2z0);
            Mul(y0, z2, y0z2);
            Sub(y2z0, y0z2, c1);
            Mul(y0, z1, y0z1);
            Mul(y1, z0, y1z0);
            Sub(y0z1, y1z0, c2);

            std::size_t constexpr x0 = 1, x1 = 2, x2 = 4, x0c0 = 0, x1c1 = 1, x2c2 = 2;
            std::size_t constexpr sum = 3;
            Sub(p0, v00, x0);
            Sub(p0, v10, x1);
            Sub(p0, v20, x2);
            Mul(x0, c0, x0c0);
            Mul(x1, c1, x1c1);
            Mul(x2, c2, x2c2);
            Add(x0c0, x1c1, sum);
            Add(sum, x2c2, detNode);

            mRSign = mNode[detNode].GetSign();
        }

        void ComputeRational(Vector3<T> const& P, Vector3<T> const& V0,
            Vector3<T> const& V1, Vector3<T> const& V2)
        {
            mNode[0] = P[0];
            mNode[1] = P[1];
            mNode[2] = P[2];
            mNode[3] = V0[0];
            mNode[4] = V0[1];
            mNode[5] = V0[2];
            mNode[6] = V1[0];
            mNode[7] = V1[1];
            mNode[8] = V1[2];
            mNode[9] = V2[0];
            mNode[10] = V2[1];
            mNode[11] = V2[2];
            ComputeRational();
        }

        void ComputeRational(Vector3<Rational> const& rP,
            Vector3<Rational> const& rV0, Vector3<Rational> const& rV1,
            Vector3<Rational> const& rV2)
        {
            mNode[0] = rP[0];
            mNode[1] = rP[1];
            mNode[2] = rP[2];
            mNode[3] = rV0[0];
            mNode[4] = rV0[1];
            mNode[5] = rV0[2];
            mNode[6] = rV1[0];
            mNode[7] = rV1[1];
            mNode[8] = rV1[2];
            mNode[9] = rV2[0];
            mNode[10] = rV2[1];
            mNode[11] = rV2[2];
            ComputeRational();
        }

        static std::int32_t constexpr invalidSign = std::numeric_limits<std::int32_t>::max();
        static std::size_t constexpr numNodes = 23;
        static std::size_t constexpr detNode = 0;
        std::int32_t mISign;
        std::int32_t mRSign;
        SWInterval<T> mIDet;
        std::vector<CRational> mNode;

    private:
        friend class UnitTestExactToPlane3;
    };
}
