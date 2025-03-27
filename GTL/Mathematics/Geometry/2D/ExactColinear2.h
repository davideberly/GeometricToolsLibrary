// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// Queries to compute whether 3 points are exactly colinear. The input type
// T must satisfy std::is_floating_point<T>::value = true. The compute type
// is BSNumber<UIntegerFP32<N>>, where N depends on the input type and the
// expression tree. The determination of worst-case N is by the tool
// GeometricTools/GTL/Tools/PrecisionCalculator. These N-values are
// conservative so that the number of bits for the query is sufficient for
// any finite floating-point inputs.
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
#include <functional>
#include <type_traits>
#include <vector>

namespace gtl
{
    template <typename T>
    class ExactColinear2
    {
    public:
        using Rational = BSNumber<UIntegerFP32<2>>;

        ExactColinear2()
            :
            mNode(numNodes)
        {
            static_assert(
                std::is_floating_point<T>::value,
                "The input type must be a floating-point type.");
        }

        // Return true when the points are colinear.
        bool operator()(Vector2<T> const& P0, Vector2<T> const& P1, Vector2<T> const& P2)
        {
            // Use interval arithmetic to determine the non-colinearity,
            // if possible.
            if (IntervalNotColinear(P0, P1, P2))
            {
                return false;
            }

            // The exact status is not known. Compute the status using
            // rational arithmetic.
            return RationalColinear(P0, P1, P2);
        }

        // Return true when the points are colinear.
        bool operator()(Vector2<T> const& P0, Vector2<T> const& P1, Vector2<T> const& P2,
            std::function<std::array<Vector2<Rational> const*, 3>()> const& GetRPoints)
        {
            // Use interval arithmetic to determine the non-colinearity,
            // if possible.
            if (IntervalNotColinear(P0, P1, P2))
            {
                return false;
            }

            // The exact status is not known. Compute the status using
            // rational arithmetic.
            std::array<Vector2<Rational> const*, 3> rPoints = GetRPoints();
            auto const& rP0 = *rPoints[0];
            auto const& rP1 = *rPoints[1];
            auto const& rP2 = *rPoints[2];
            return RationalColinear(rP0, rP1, rP2);
        }

    private:
        // The function returns +1 if the points are not colinear. With the
        // SWInterval arithmetic, it is not possible to detect colinear
        // points. Therefore, the function returns invalidSign when it cannot
        // determine non-colinearity.
        bool IntervalNotColinear(Vector2<T> const& P0, Vector2<T> const& P1, Vector2<T> const& P2)
        {
            Vector2<SWInterval<T>> sP0{ P0[0], P0[1] };
            Vector2<SWInterval<T>> sP1{ P1[0], P1[1] };
            Vector2<SWInterval<T>> sP2{ P2[0], P2[1] };
            Vector2<SWInterval<T>> sU = sP1 - sP0;
            Vector2<SWInterval<T>> sV = sP2 - sP0;
            SWInterval<T> sDotPerp = DotPerp(sU, sV);
            return C_<T>(0) < sDotPerp[0] || sDotPerp[1] < C_<T>(0);
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

        bool RationalColinear()
        {
            std::size_t constexpr x0 = 0, y0 = 1, x1 = 2, y1 = 3, x2 = 4, y2 = 5;
            std::size_t constexpr x1mx0 = 6, y1my0 = 7, x2mx0 = 8, y2my0 = 9;
            std::size_t constexpr product0 = 1, product1 = 2, dotPerp = 0;
            Sub(x1, x0, x1mx0);
            Sub(y1, y0, y1my0);
            Sub(x2, x0, x2mx0);
            Sub(y2, y0, y2my0);
            Mul(x1mx0, y2my0, product0);
            Mul(x2mx0, y1my0, product1);
            Sub(product0, product1, dotPerp);
            return mNode[dotPerp].GetSign() == 0;
        }

        bool RationalColinear(Vector2<T> const& P0, Vector2<T> const& P1, Vector2<T> const& P2)
        {
            mNode[0] = P0[0];
            mNode[1] = P0[1];
            mNode[2] = P1[0];
            mNode[3] = P1[1];
            mNode[4] = P2[0];
            mNode[5] = P2[1];
            return RationalColinear();
        }

        bool RationalColinear(Vector2<Rational> const& rP0, Vector2<Rational> const& rP1,
            Vector2<Rational> const& rP2)
        {
            mNode[0] = rP0[0];
            mNode[1] = rP0[1];
            mNode[2] = rP1[0];
            mNode[3] = rP1[1];
            mNode[4] = rP2[0];
            mNode[5] = rP2[1];
            return RationalColinear();
        }

        static std::size_t constexpr numNodes = 10;
        std::vector<CRational> mNode;

    private:
        friend class UnitTestExactColinear2;
    };
}
