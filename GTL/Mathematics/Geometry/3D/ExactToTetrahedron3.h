// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// For a tetrahedron with vertices ordered as described in the file
// TetrahedronKey.h, operator() returns
//   +1, P outside tetrahedron
//   -1, P inside tetrahedron
//    0, P on tetrahedron
// The input type T must satisfy std::is_floating_point<T>::value = true. The
// compute type is BSNumber<UIntegerFP32<N>>, where N depends on the input
// type and the expression tree of the query. The determination of worst-case
// N is by the GeometricTools/GTL/Tools/PrecisionCalculator tool. The
// N-values are conservative so that the number of bits for the query is
// sufficient for any finite floating-point inputs.
//
// expression tree number of nodes = 42
//
// compute type = float
//   N = 28
//   sizeof(BSNumber<UIntegerFP32<18>>) = 128
//   number of heap bytes for rational computing = 5376 = 42 * 128
//
// compute type = double
//   N = 198
//   sizeof(BSNumber<UIntegerFP32<198>>) = 808
//   number of heap bytes for rational computing = 33936 = 42 * 808
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
    class ExactToTetrahedron3
    {
    public:
        using Rational = BSNumber<UIntegerFP32<2>>;

        ExactToTetrahedron3()
            :
            mISign021(invalidSign),
            mISign013(invalidSign),
            mISign032(invalidSign),
            mISign123(invalidSign),
            mRSign021(invalidSign),
            mRSign013(invalidSign),
            mRSign032(invalidSign),
            mRSign123(invalidSign),
            mIDet021{},
            mIDet013{},
            mIDet032{},
            mIDet123{},
            mNode(numNodes)
        {
            static_assert(
                std::is_floating_point<T>::value,
                "The input type must be a floating-point type.");
        }

        std::int32_t operator()(Vector3<T> const& P, Vector3<T> const& V0,
            Vector3<T> const& V1, Vector3<T> const& V2, Vector3<T> const& V3)
        {
            mISign021 = invalidSign;
            mISign013 = invalidSign;
            mISign032 = invalidSign;
            mISign123 = invalidSign;
            mRSign021 = invalidSign;
            mRSign013 = invalidSign;
            mRSign032 = invalidSign;
            mRSign123 = invalidSign;

            // Use interval arithmetic to determine the relative location of P
            // if possible.
            std::int32_t sign = ComputeInterval(P, V0, V1, V2, V3);
            if (sign != invalidSign)
            {
                return sign;
            }

            // The exact relative location of P is not known. Compute the
            // relative location using rational arithmetic.
            return ComputeRational(P, V0, V1, V2, V3);
        }

        std::int32_t operator()(Vector3<T> const& P, Vector3<T> const& V0,
            Vector3<T> const& V1, Vector3<T> const& V2, Vector3<T> const& V3,
            std::function<std::array<Vector3<Rational> const*, 5>()> const& GetRPoints)
        {
            mISign021 = invalidSign;
            mISign013 = invalidSign;
            mISign032 = invalidSign;
            mISign123 = invalidSign;
            mRSign021 = invalidSign;
            mRSign013 = invalidSign;
            mRSign032 = invalidSign;
            mRSign123 = invalidSign;

            // Use interval arithmetic to determine the relative location of P
            // if possible.
            std::int32_t sign = ComputeInterval(P, V0, V1, V2, V3);
            if (sign != invalidSign)
            {
                return sign;
            }

            // The exact relative location of P is not known. Compute the
            // relative location using rational arithmetic.
            std::array<Vector3<Rational> const*, 5> rPoints = GetRPoints();
            auto const& rP = *rPoints[0];
            auto const& rV0 = *rPoints[1];
            auto const& rV1 = *rPoints[2];
            auto const& rV2 = *rPoints[3];
            auto const& rV3 = *rPoints[4];
            return ComputeRational(rP, rV0, rV1, rV2, rV3);
        }

    private:
        std::int32_t ComputeInterval(Vector3<T> const& P, Vector3<T> const& V0,
            Vector3<T> const& V1, Vector3<T> const& V2, Vector3<T> const& V3)
        {
            mIDet021 = { C_<T>(0), C_<T>(0) };
            mIDet013 = { C_<T>(0), C_<T>(0) };
            mIDet032 = { C_<T>(0), C_<T>(0) };
            mIDet123 = { C_<T>(0), C_<T>(0) };

            // Test whether P is outside the tetrahedron via face <V0,V2,V1>.
            // (x0,y0,z0) = P-V0, (x2,y2,z2) = V2-V0, (x1,y1,z1) = V1-V0
            // det = (P-V0).(V2-V0)x(V1-V0) = (x0,y0,z0).(x2,y2,z2)x(x1,y1,z1)
            // = x0*(y2*z1-y1*z2) + x2*(y1*z0-y0*z1) + x1*(y0*z2-y2*z0)
            auto x0 = SWInterval<T>::Sub(P[0], V0[0]);
            auto y0 = SWInterval<T>::Sub(P[1], V0[1]);
            auto z0 = SWInterval<T>::Sub(P[2], V0[2]);
            auto x1 = SWInterval<T>::Sub(V1[0], V0[0]);
            auto y1 = SWInterval<T>::Sub(V1[1], V0[1]);
            auto z1 = SWInterval<T>::Sub(V1[2], V0[2]);
            auto x2 = SWInterval<T>::Sub(V2[0], V0[0]);
            auto y2 = SWInterval<T>::Sub(V2[1], V0[1]);
            auto z2 = SWInterval<T>::Sub(V2[2], V0[2]);
            auto y2z1 = y2 * z1;
            auto y1z2 = y1 * z2;
            auto y1z0 = y1 * z0;
            auto y0z1 = y0 * z1;
            auto y0z2 = y0 * z2;
            auto y2z0 = y2 * z0;
            auto c21 = y2z1 - y1z2;
            auto c10 = y1z0 - y0z1;
            auto c02 = y0z2 - y2z0;
            auto x0c21 = x0 * c21;
            auto x2c10 = x2 * c10;
            auto x1c02 = x1 * c02;
            mIDet021 = x0c21 + x2c10 + x1c02;
            if (mIDet021[0] > C_<T>(0))
            {
                mISign021 = +1;
                return +1;
            }
            mISign021 = (mIDet021[1] < C_<T>(0) ? -1 : invalidSign);

            // Test whether P is outside the tetrahedron via face <V0,V1,V3>.
            // (x0,y0,z0) = P-V0, (x1,y1,z1) = V1-V0, (x3,y3,z3) = V3-V0
            // det = (P-V0).(V1-V0)x(V3-V0) = (x0,y0,z0).(x1,y1,z1)x(x3,y3,v3)
            // = x0*(y1*z3-z3*y1) + x1*(y3*z0-y0*z3) + x3*(y0*z1-y1*z0)
            auto x3 = SWInterval<T>::Sub(V3[0], V0[0]);
            auto y3 = SWInterval<T>::Sub(V3[1], V0[1]);
            auto z3 = SWInterval<T>::Sub(V3[2], V0[2]);
            auto y1z3 = y1 * z3;
            auto y3z1 = y3 * z1;
            auto y3z0 = y3 * z0;
            auto y0z3 = y0 * z3;
            auto c13 = y1z3 - y3z1;
            auto c30 = y3z0 - y0z3;
            auto c01 = -c10;
            auto x0c13 = x0 * c13;
            auto x1c30 = x1 * c30;
            auto x3c01 = x3 * c01;
            mIDet013 = x0c13 + x1c30 + x3c01;
            if (mIDet013[0] > C_<T>(0))
            {
                mISign013 = +1;
                return +1;
            }
            mISign013 = (mIDet013[1] < C_<T>(0) ? -1 : invalidSign);

            // Test whether P is outside the tetrahedron via face <V0,V3,V2>.
            // (x0,y0,z0) = P-V0, (x3,y3,z3) = V3-V0, (x2,y2,z2) = V2-V0
            // det = (P-V0).(V3-V0)x(V2-V0) = (x0,y0,z0).(x3,y3,z3)x(x2,y2,z2)
            // = x0*(y3*z2-y2*z3) + x3*(y2*z0-0*z2) + x2*(y0*z3-y3*z0)
            auto y3z2 = y3 * z2;
            auto y2z3 = y2 * z3;
            auto c32 = y3z2 - y2z3;
            auto c20 = -c02;
            auto c03 = -c30;
            auto x0c32 = x0 * c32;
            auto x3c20 = x3 * c20;
            auto x2c03 = x2 * c03;
            mIDet032 = x0c32 + x3c20 + x2c03;
            if (mIDet032[0] > C_<T>(0))
            {
                mISign032 = +1;
                return +1;
            }
            mISign032 = (mIDet032[1] < C_<T>(0) ? -1 : invalidSign);

            // Test whether P is outside the tetrahedron via face <V1,V2,V3>.
            // (x4,y4,z4) = P-V1, (x5,y5,z5) = V2-V1, (x6,y6,z6) = V3-V1
            // det = (P-V1).(V2-V1)x(V3-V1) = (x4,y4,z4).(x5,y5,z5)x(x6,y6,z6)
            auto x4 = SWInterval<T>::Sub(P[0], V1[0]);
            auto y4 = SWInterval<T>::Sub(P[1], V1[1]);
            auto z4 = SWInterval<T>::Sub(P[2], V1[2]);
            auto x5 = SWInterval<T>::Sub(V2[0], V1[0]);
            auto y5 = SWInterval<T>::Sub(V2[1], V1[1]);
            auto z5 = SWInterval<T>::Sub(V2[2], V1[2]);
            auto x6 = SWInterval<T>::Sub(V3[0], V1[0]);
            auto y6 = SWInterval<T>::Sub(V3[1], V1[1]);
            auto z6 = SWInterval<T>::Sub(V3[2], V1[2]);
            auto y5z6 = y5 * z6;
            auto y6z5 = y6 * z5;
            auto y6z4 = y6 * z4;
            auto y4z6 = y4 * z6;
            auto y4z5 = y4 * z5;
            auto y5z4 = y5 * z4;
            auto c56 = y5z6 - y6z5;
            auto c64 = y6z4 - y4z6;
            auto c45 = y4z5 - y5z4;
            auto x4c56 = x4 * c56;
            auto x5c64 = x5 * c64;
            auto x6c45 = x6 * c45;
            mIDet123 = x4c56 + x5c64 + x6c45;
            if (mIDet123[0] > C_<T>(0))
            {
                mISign123 = +1;
                return +1;
            }
            mISign123 = (mIDet123[1] < C_<T>(0) ? -1 : invalidSign);

            // If all signs are -1, P is inside the triangle. If at least one
            // sign is 0, it is unknown how P is located relative to the
            // triangle.
            if (mISign021 == -1 && mISign013 == -1 && mISign032 == -1 && mISign123 == -1)
            {
                return -1;
            }
            else
            {
                return invalidSign;
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

        std::int32_t ComputeRational()
        {
            std::size_t constexpr p0 = 0, p1 = 1, p2 = 2;
            std::size_t constexpr v00 = 3, v01 = 4, v02 = 5;
            std::size_t constexpr v10 = 6, v11 = 7, v12 = 8;
            std::size_t constexpr v20 = 9, v21 = 10, v22 = 11;
            std::size_t constexpr v30 = 12, v31 = 13, v32 = 14;

            // Test whether P is outside the tetrahedron via face <V0,V2,V1>.
            // (x0,y0,z0) = P-V0, (x2,y2,z2) = V2-V0, (x1,y1,z1) = V1-V0
            // det = (P-V0).(V2-V0)x(V1-V0) = (x0,y0,z0).(x2,y2,z2)x(x1,y1,z1)
            // = x0*(y2*z1-y1*z2) + x1*(y1*z0-y0*z1) + x2*(y0*z2-y2*z0)
            // = x0*c21 + x2*c10 + x1*c02
            std::size_t constexpr x0 = 15, y0 = 16, z0 = 17;
            std::size_t constexpr x1 = 18, y1 = 19, z1 = 20;
            std::size_t constexpr x2 = 21, y2 = 22, z2 = 23;
            std::size_t constexpr y2z1 = 24, y1z2 = 25, c21 = 26;
            std::size_t constexpr y1z0 = 24, y0z1 = 25, c10 = 27;
            std::size_t constexpr y0z2 = 24, y2z0 = 25, c02 = 28;
            std::size_t constexpr x0c21 = 29, x2c10 = 30, x1c02 = 31;
            std::size_t constexpr s021 = 32;
            Sub(p0, v00, x0);
            Sub(p1, v01, y0);
            Sub(p2, v02, z0);
            Sub(v10, v00, x1);
            Sub(v11, v01, y1);
            Sub(v12, v02, z1);
            Sub(v20, v00, x2);
            Sub(v21, v01, y2);
            Sub(v22, v02, z2);
            Mul(y2, z1, y2z1);
            Mul(y1, z2, y1z2);
            Sub(y2z1, y1z2, c21);
            Mul(y1, z0, y1z0);
            Mul(y0, z1, y0z1);
            Sub(y1z0, y0z1, c10);
            Mul(y0, z2, y0z2);
            Mul(y2, z0, y2z0);
            Sub(y0z2, y2z0, c02);
            Mul(x0, c21, x0c21);
            Mul(x2, c10, x2c10);
            Mul(x1, c02, x1c02);
            Add(x0c21, x2c10, s021);
            Add(s021, x1c02, det021Node);
            mRSign021 = mNode[det021Node].GetSign();
            if (mRSign021 > 0)
            {
                return +1;
            }

            // Test whether P is outside the tetrahedron via face <V0,V1,V3>.
            // (x0,y0,z0) = P-V0, (x2,y2,z2) = V1-V0, (x3,y3,z3) = V3-V0
            // det = (P-V0).(V1-V0)x(V3-V0) = (x0,y0,z0).(x2,y2,z2)x(x3,y3,z3)
            // = x0*(y1*z3-y3*z1) + x1*(y3*z0-y0*z3) + x3*(y0*z1-y1*z0)
            // = x0*c13 + x1*c30 + x3*c01 = x0*c13 + x1*c30 + x3*(-c10)
            std::size_t constexpr x3 = 24, y3 = 25, z3 = 26;
            std::size_t constexpr y1z3 = 29, y3z1 = 30, c13 = 31;
            std::size_t constexpr y3z0 = 29, y0z3 = 30, c30 = 32;
            std::size_t constexpr c01 = c10;  // node[c01] = -node[c10], see (*)
            std::size_t constexpr x0c13 = 33, x1c30 = 34, x3c01 = 35;
            std::size_t constexpr s013 = 36;
            Sub(v30, v00, x3);
            Sub(v31, v01, y3);
            Sub(v32, v02, z3);
            Mul(y1, z3, y1z3);
            Mul(y3, z1, y3z1);
            Sub(y1z3, y3z1, c13);
            Mul(y3, z0, y3z0);
            Mul(y0, z3, y0z3);
            Sub(y3z0, y0z3, c30);
            mNode[c01].Negate();  // (*)
            Mul(x0, c13, x0c13);
            Mul(x1, c30, x1c30);
            Mul(x3, c01, x3c01);
            Add(x0c13, x1c30, s013);
            Add(s013, x3c01, det013Node);
            mRSign013 = mNode[det013Node].GetSign();
            if (mRSign013 > 0)
            {
                return +1;
            }

            // Test whether P is outside the tetrahedron via face <V0,V3,V2>.
            // (x0,y0,z0) = P-V0, (x3,y3,z3) = V3-V0, (x2,y2,z2) = V2-V0
            // det = (P-V0).(V3-V0)x(V2-V0) = (x0,y0,z0).(x3,y3,z3)x(x2,y2,z2)
            // = x0*(y3*z2-y2*z3) + x3*(y2*z0-y0*z2) + x2*(y0*z3-y3*z0)
            // = x0*c32 + x3*c20 + x2*c03 = x0*c32 + x3*(-c02) + x2*(-c30)
            std::size_t constexpr y3z2 = 3, y2z3 = 4, c32 = 5;
            std::size_t constexpr c20 = c02;  // node[c20] = -node[c02], see (**)
            std::size_t constexpr c03 = c30;  // node[c03] = -node[c30], see (***)
            std::size_t constexpr x0c32 = 16, x3c20 = 17, x2c03 = 18;
            std::size_t constexpr s032 = 19;
            Mul(y3, z2, y3z2);
            Mul(y2, z3, y2z3);
            Sub(y3z2, y2z3, c32);
            mNode[c20].Negate();  // (**)
            mNode[c03].Negate();  // (***)
            Mul(x0, c32, x0c32);
            Mul(x3, c20, x3c20);
            Mul(x2, c03, x2c03);
            Add(x0c32, x3c20, s032);
            Add(s032, x2c03, det032Node);
            mRSign032 = mNode[det032Node].GetSign();
            if (mRSign032 > 0)
            {
                return +1;
            }

            // Test whether P is outside the tetrahedron via face <V1,V2,V3>.
            // (x4,y4,z4) = P-V1, (x5,y5,z5) = V2-V1, (x6,y6,z6) = V3-V1
            std::size_t constexpr x4 = 3, y4 = 4, z4 = 5;
            std::size_t constexpr x5 = 15, y5 = 16, z5 = 17;
            std::size_t constexpr x6 = 18, y6 = 19, z6 = 20;
            std::size_t constexpr y5z6 = 21, y6z5 = 22, c56 = 23;
            std::size_t constexpr y6z4 = 21, y4z6 = 22, c64 = 24;
            std::size_t constexpr y4z5 = 21, y5z4 = 22, c45 = 25;
            std::size_t constexpr x4c56 = 26, x5c64 = 27, x6c45 = 28;
            std::size_t constexpr s123 = 29;
            Sub(p0, v10, x4);
            Sub(p1, v11, y4);
            Sub(p2, v12, z4);
            Sub(v20, v10, x5);
            Sub(v21, v11, y5);
            Sub(v22, v12, z5);
            Sub(v30, v10, x6);
            Sub(v31, v11, y6);
            Sub(v32, v12, z6);
            Mul(y5, z6, y5z6);
            Mul(y6, z5, y6z5);
            Sub(y5z6, y6z5, c56);
            Mul(y6, z4, y6z4);
            Mul(y4, z6, y4z6);
            Sub(y6z4, y4z6, c64);
            Mul(y4, z5, y4z5);
            Mul(y5, z4, y5z4);
            Sub(y4z5, y5z4, c45);
            Mul(x4, c56, x4c56);
            Mul(x5, c64, x5c64);
            Mul(x6, c45, x6c45);
            Add(x4c56, x5c64, s123);
            Add(s123, x6c45, det123Node);
            mRSign123 = mNode[det123Node].GetSign();
            if (mRSign123 > 0)
            {
                return +1;
            }

            // If all signs are -1, P is inside the triangle. If at least one
            // sign is 0, it is unknown how P is located relative to the
            // triangle.
            return (mRSign021 && mRSign013 && mRSign032 && mRSign123 ? -1 : 0);
        }

        std::int32_t ComputeRational(Vector3<T> const& P, Vector3<T> const& V0,
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
            return ComputeRational();
        }

        std::int32_t ComputeRational(Vector3<Rational> const& rP,
            Vector3<Rational> const& rV0, Vector3<Rational> const& rV1,
            Vector3<Rational> const& rV2, Vector3<Rational> const& rV3)
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
            return ComputeRational();
        }

        static std::int32_t constexpr invalidSign = std::numeric_limits<std::int32_t>::max();
        static std::size_t constexpr numNodes = 42;
        static std::size_t constexpr det021Node = 38, det013Node = 39;
        static std::size_t constexpr det032Node = 40, det123Node = 41;
        std::int32_t mISign021, mISign013, mISign032, mISign123;
        std::int32_t mRSign021, mRSign013, mRSign032, mRSign123;
        SWInterval<T> mIDet021, mIDet013, mIDet032, mIDet123;
        std::vector<CRational> mNode;

    private:
        friend class UnitTestExactToTetrahedron3;
    };
}
