// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// For a tetrahedron with vertices ordered as described in the file
// TetrahedronKey.h, the function returns
//   +1, P outside circumsphere of tetrahedron
//   -1, P inside circumsphere of tetrahedron
//    0, P on circumsphere of tetrahedron
// The input type T must satisfy std::is_floating_point<T>::value = true. The
// compute type is BSNumber<UIntegerFP32<N>>, where N depends on the input
// type and the expression tree of the query. The determination of worst-case
// N is by the GeometricTools/GTL/Tools/PrecisionCalculator tool. The
// N-values are conservative so that the number of bits for the query is
// sufficient for any finite floating-point inputs.
//
// expression tree number of nodes = 35
//
// compute type = float
//   N = 44
//   sizeof(BSNumber<UIntegerFP32<44>>) = 192
//   number of heap bytes for rational computing = 6720 = 35 * 192
//
// compute type = double
//   N = 330
//   sizeof(BSNumber<UIntegerFP32<330>>) = 1336
//   number of heap bytes for rational computing = 46760 = 35 * 1336
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
    class ExactToCircumsphere3
    {
    public:
        using Rational = BSNumber<UIntegerFP32<2>>;

        ExactToCircumsphere3()
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
            Vector3<T> const& V1, Vector3<T> const& V2, Vector3<T> const& V3)
        {
            mISign = invalidSign;
            mRSign = invalidSign;

            // Use interval arithmetic to determine the relative location of P
            // if possible.
            ComputeInterval(P, V0, V1, V2, V3);
            if (mISign != invalidSign)
            {
                return mISign;
            }

            // The exact relative location of P is not known. Compute the
            // relative location using rational arithmetic.
            ComputeRational(P, V0, V1, V2, V3);
            return mRSign;
        }

        std::int32_t operator()(Vector3<T> const& P, Vector3<T> const& V0,
            Vector3<T> const& V1, Vector3<T> const& V2, Vector3<T> const& V3,
            std::function<std::array<Vector3<Rational> const*, 5>()> const& GetRPoints)
        {
            mISign = invalidSign;
            mRSign = invalidSign;

            // Use interval arithmetic to determine the relative location of P
            // if possible.
            ComputeInterval(P, V0, V1, V2, V3);
            if (mISign != invalidSign)
            {
                return mISign;
            }

            // The exact relative location of P is not known. Compute the
            // relative location using rational arithmetic.
            std::array<Vector3<Rational> const*, 5> rPoints = GetRPoints();
            auto const& rP = *rPoints[0];
            auto const& rV0 = *rPoints[1];
            auto const& rV1 = *rPoints[2];
            auto const& rV2 = *rPoints[3];
            auto const& rV3 = *rPoints[4];
            ComputeRational(rP, rV0, rV1, rV2, rV3);
            return mRSign;
        }

    private:
        void ComputeInterval(Vector3<T> const& P, Vector3<T> const& V0,
            Vector3<T> const& V1, Vector3<T> const& V2, Vector3<T> const& V3)
        {
            auto x0 = SWInterval<T>::Sub(V0[0], P[0]);
            auto y0 = SWInterval<T>::Sub(V0[1], P[1]);
            auto z0 = SWInterval<T>::Sub(V0[2], P[2]);
            auto s00 = SWInterval<T>::Add(V0[0], P[0]);
            auto s01 = SWInterval<T>::Add(V0[1], P[1]);
            auto s02 = SWInterval<T>::Add(V0[2], P[2]);
            auto x1 = SWInterval<T>::Sub(V1[0], P[0]);
            auto y1 = SWInterval<T>::Sub(V1[1], P[1]);
            auto z1 = SWInterval<T>::Sub(V1[2], P[2]);
            auto s10 = SWInterval<T>::Add(V1[0], P[0]);
            auto s11 = SWInterval<T>::Add(V1[1], P[1]);
            auto s12 = SWInterval<T>::Add(V1[2], P[2]);
            auto x2 = SWInterval<T>::Sub(V2[0], P[0]);
            auto y2 = SWInterval<T>::Sub(V2[1], P[1]);
            auto z2 = SWInterval<T>::Sub(V2[2], P[2]);
            auto s20 = SWInterval<T>::Add(V2[0], P[0]);
            auto s21 = SWInterval<T>::Add(V2[1], P[1]);
            auto s22 = SWInterval<T>::Add(V2[2], P[2]);
            auto x3 = SWInterval<T>::Sub(V3[0], P[0]);
            auto y3 = SWInterval<T>::Sub(V3[1], P[1]);
            auto z3 = SWInterval<T>::Sub(V3[2], P[2]);
            auto s30 = SWInterval<T>::Add(V3[0], P[0]);
            auto s31 = SWInterval<T>::Add(V3[1], P[1]);
            auto s32 = SWInterval<T>::Add(V3[2], P[2]);
            auto t00 = s00 * x0;
            auto t01 = s01 * y0;
            auto t02 = s02 * z0;
            auto t10 = s10 * x1;
            auto t11 = s11 * y1;
            auto t12 = s12 * z1;
            auto t20 = s20 * x2;
            auto t21 = s21 * y2;
            auto t22 = s22 * z2;
            auto t30 = s30 * x3;
            auto t31 = s31 * y3;
            auto t32 = s32 * z3;
            auto w0 = t00 + t01 + t02;
            auto w1 = t10 + t11 + t12;
            auto w2 = t20 + t21 + t22;
            auto w3 = t30 + t31 + t32;
            auto x0y1 = x0 * y1;
            auto x0y2 = x0 * y2;
            auto x0y3 = x0 * y3;
            auto x1y0 = x1 * y0;
            auto x1y2 = x1 * y2;
            auto x1y3 = x1 * y3;
            auto x2y0 = x2 * y0;
            auto x2y1 = x2 * y1;
            auto x2y3 = x2 * y3;
            auto x3y0 = x3 * y0;
            auto x3y1 = x3 * y1;
            auto x3y2 = x3 * y2;
            auto z0w1 = z0 * w1;
            auto z0w2 = z0 * w2;
            auto z0w3 = z0 * w3;
            auto z1w0 = z1 * w0;
            auto z1w2 = z1 * w2;
            auto z1w3 = z1 * w3;
            auto z2w0 = z2 * w0;
            auto z2w1 = z2 * w1;
            auto z2w3 = z2 * w3;
            auto z3w0 = z3 * w0;
            auto z3w1 = z3 * w1;
            auto z3w2 = z3 * w2;
            auto u0 = x0y1 - x1y0;
            auto u1 = x0y2 - x2y0;
            auto u2 = x0y3 - x3y0;
            auto u3 = x1y2 - x2y1;
            auto u4 = x1y3 - x3y1;
            auto u5 = x2y3 - x3y2;
            auto v0 = z0w1 - z1w0;
            auto v1 = z0w2 - z2w0;
            auto v2 = z0w3 - z3w0;
            auto v3 = z1w2 - z2w1;
            auto v4 = z1w3 - z3w1;
            auto v5 = z2w3 - z3w2;
            auto u0v5 = u0 * v5;
            auto u1v4 = u1 * v4;
            auto u2v3 = u2 * v3;
            auto u3v2 = u3 * v2;
            auto u4v1 = u4 * v1;
            auto u5v0 = u5 * v0;
            mIDet = u0v5 - u1v4 + u2v3 + u3v2 - u4v1 + u5v0;

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

        static std::size_t constexpr numWords = std::is_same<T, float>::value ? 44 : 330;
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
            std::size_t constexpr v30 = 12, v31 = 13, v32 = 14;

            std::size_t constexpr x0 = 15, y0 = 16, z0 = 17, w0 = 18;
            std::size_t constexpr x1 = 19, y1 = 20, z1 = 21, w1 = 22;
            std::size_t constexpr x2 = 23, y2 = 24, z2 = 25, w2 = 26;
            std::size_t constexpr x3 = 27, y3 = 28, z3 = 29, w3 = 30;

            std::size_t constexpr s00 = 31, s01 = 32, s02 = 33;
            std::size_t constexpr t00 = 3, t01 = 4, t02 = 5;
            std::size_t constexpr sum = 34;
            Sub(v00, p0, x0);
            Add(v00, p0, s00);
            Mul(s00, x0, t00);
            Sub(v01, p1, y0);
            Add(v01, p1, s01);
            Mul(s01, y0, t01);
            Sub(v02, p2, z0);
            Add(v02, p2, s02);
            Mul(s02, z0, t02);
            Add(t00, t01, sum);
            Add(sum, t02, w0);

            std::size_t constexpr s10 = 31, s11 = 32, s12 = 33;
            std::size_t constexpr t10 = 3, t11 = 4, t12 = 5;
            Sub(v10, p0, x1);
            Add(v10, p0, s10);
            Mul(s10, x1, t10);
            Sub(v11, p1, y1);
            Add(v11, p1, s11);
            Mul(s11, y1, t11);
            Sub(v12, p2, z1);
            Add(v12, p2, s12);
            Mul(s12, z1, t12);
            Add(t10, t11, sum);
            Add(sum, t12, w1);

            std::size_t constexpr s20 = 31, s21 = 32, s22 = 33;
            std::size_t constexpr t20 = 3, t21 = 4, t22 = 5;
            Sub(v20, p0, x2);
            Add(v20, p0, s20);
            Mul(s20, x2, t20);
            Sub(v21, p1, y2);
            Add(v21, p1, s21);
            Mul(s21, y2, t21);
            Sub(v22, p2, z2);
            Add(v22, p2, s22);
            Mul(s22, z2, t22);
            Add(t20, t21, sum);
            Add(sum, t22, w2);

            std::size_t constexpr s30 = 31, s31 = 32, s32 = 33;
            std::size_t constexpr t30 = 3, t31 = 4, t32 = 5;
            Sub(v30, p0, x3);
            Add(v30, p0, s30);
            Mul(s30, x3, t30);
            Sub(v31, p1, y3);
            Add(v31, p1, s31);
            Mul(s31, y3, t31);
            Sub(v32, p2, z3);
            Add(v32, p2, s32);
            Mul(s32, z3, t32);
            Add(t30, t31, sum);
            Add(sum, t32, w3);

            std::size_t constexpr u0 = 3, u1 = 4, u2 = 5, u3 = 6, u4 = 7, u5 = 8;
            std::size_t const prd0 = 31, prd1 = 32;
            Mul(x0, y1, prd0);
            Mul(x1, y0, prd1);
            Sub(prd0, prd1, u0);
            Mul(x0, y2, prd0);
            Mul(x2, y0, prd1);
            Sub(prd0, prd1, u1);
            Mul(x0, y3, prd0);
            Mul(x3, y0, prd1);
            Sub(prd0, prd1, u2);
            Mul(x1, y2, prd0);
            Mul(x2, y1, prd1);
            Sub(prd0, prd1, u3);
            Mul(x1, y3, prd0);
            Mul(x3, y1, prd1);
            Sub(prd0, prd1, u4);
            Mul(x2, y3, prd0);
            Mul(x3, y2, prd1);
            Sub(prd0, prd1, u5);

            std::size_t constexpr v0 = 9, v1 = 10, v2 = 11, v3 = 12, v4 = 13, v5 = 14;
            Mul(z0, w1, prd0);
            Mul(z1, w0, prd1);
            Sub(prd0, prd1, v0);
            Mul(z0, w2, prd0);
            Mul(z2, w0, prd1);
            Sub(prd0, prd1, v1);
            Mul(z0, w3, prd0);
            Mul(z3, w0, prd1);
            Sub(prd0, prd1, v2);
            Mul(z1, w2, prd0);
            Mul(z2, w1, prd1);
            Sub(prd0, prd1, v3);
            Mul(z1, w3, prd0);
            Mul(z3, w1, prd1);
            Sub(prd0, prd1, v4);
            Mul(z2, w3, prd0);
            Mul(z3, w2, prd1);
            Sub(prd0, prd1, v5);

            std::size_t constexpr u0v5 = 15, u1v4 = 16, u2v3 = 17, u3v2 = 18, u4v1 = 19, u5v0 = 20;
            Mul(u0, v5, u0v5);
            Mul(u1, v4, u1v4);
            Mul(u2, v3, u2v3);
            Mul(u3, v2, u3v2);
            Mul(u4, v1, u4v1);
            Mul(u5, v0, u5v0);

            std::size_t constexpr sum0 = 1, sum1 = 2, sum2 = 3, sum3 = 4;
            Add(u0v5, u5v0, sum0);
            Add(u1v4, u4v1, sum1);
            Add(u2v3, u3v2, sum2);
            Add(sum0, sum2, sum3);
            Sub(sum3, sum1, detNode);

            mRSign = mNode[detNode].GetSign();
        }

        void ComputeRational(Vector3<T> const& P, Vector3<T> const& V0,
            Vector3<T> const& V1, Vector3<T> const& V2, Vector3<T> const& V3)
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
            mNode[12] = V3[0];
            mNode[13] = V3[1];
            mNode[14] = V3[2];
            ComputeRational();
        }

        void ComputeRational(Vector3<Rational> const& rP, Vector3<Rational> const& rV0,
            Vector3<Rational> const& rV1, Vector3<Rational> const& rV2,
            Vector3<Rational> const& rV3)
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
            mNode[12] = rV3[0];
            mNode[13] = rV3[1];
            mNode[14] = rV3[2];
            ComputeRational();
        }

        static std::int32_t constexpr invalidSign = std::numeric_limits<std::int32_t>::max();
        static std::size_t constexpr numNodes = 35;
        static std::size_t constexpr detNode = 0;
        std::int32_t mISign;
        std::int32_t mRSign;
        SWInterval<T> mIDet;
        std::vector<CRational> mNode;

    private:
        friend class UnitTestExactToCircumsphere3;
    };
}

