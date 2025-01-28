// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>

namespace gtl
{
    template <typename T, std::size_t N>
    class ParametricSurface
    {
    protected:
        // Abstract base class for a parameterized surface X(u,v). The
        // parametric domain is either rectangular or triangular. Valid (u,v)
        // values for a rectangular domain satisfy
        //   umin <= u <= umax, vmin <= v <= vmax
        // and valid (u,v) values for a triangular domain satisfy
        //   umin <= u <= umax, vmin <= v <= vmax,
        //   (vmax-vmin) * (u-umin) + (umax-umin) * (v-vmax) <= 0
        ParametricSurface(T const& umin, T const& umax, T const& vmin,
            T const& vmax, bool rectangular)
            :
            mUMin(umin),
            mUMax(umax),
            mVMin(vmin),
            mVMax(vmax),
            mRectangular(rectangular)
        {
        }

    public:
        virtual ~ParametricSurface() = default;

        // Member access.
        inline T const& GetUMin() const
        {
            return mUMin;
        }

        inline T const& GetUMax() const
        {
            return mUMax;
        }

        inline T const& GetVMin() const
        {
            return mVMin;
        }

        inline T const& GetVMax() const
        {
            return mVMax;
        }

        inline bool IsRectangular() const
        {
            return mRectangular;
        }

        // Evaluation of the surface. If you want only the position, pass in
        // order of 0. If you want the position and first derivatives, pass in
        // order of 1, and so on. The output array 'jet' must have enough
        // storage to support the specified order.  If n is the order, then
        // the number of jet[] values is (n+1)*(n+2)/2. The values are ordered
        // as:
        //   jet[0] contains position X
        //   jet[1] contains first-order derivative dX/du
        //   jet[2] contains first-order derivative dX/dv
        //   jet[3] contains second-order derivative d2X/du2
        //   jet[4] contains second-order derivative d2X/dudv
        //   jet[5] contains second-order derivative d2X/dv2
        // and so on.
        virtual void Evaluate(T const& u, T const& v, std::size_t order, Vector<T, N>* jet) const = 0;

        // Differential geometric quantities.
        Vector<T, N> GetPosition(T const& u, T const& v) const
        {
            // (position, *, *, *, *, *)
            std::array<Vector<T, N>, 6> jet{};
            Evaluate(u, v, 0, jet.data());
            return jet[0];
        }

        Vector<T, N> GetUTangent(T const& u, T const& v) const
        {
            // (position, u-tangent, *, *, *, *)
            std::array<Vector<T, N>, 6> jet{};
            Evaluate(u, v, 1, jet.data());
            Normalize(jet[1]);
            return jet[1];
        }

        Vector<T, N> GetVTangent(T const& u, T const& v) const
        {
            // (position, *, v-tangent, *, *, *)
            std::array<Vector<T, N>, 6> jet{};
            Evaluate(u, v, 1, jet.data());
            Normalize(jet[2]);
            return jet[2];
        }

    protected:
        T mUMin, mUMax, mVMin, mVMax;
        bool mRectangular;

    private:
        friend class UnitTestParametricSurface;
    };
}
