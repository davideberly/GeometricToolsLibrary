// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.20

#pragma once

#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>

namespace gtl
{
    template <typename T, std::size_t N>
    class ParametricVolume
    {
    protected:
        // Abstract base class for a parameterized volume X(u,v,w). For now
        // the parametric domain is a solid box. Valid (u,v,w) values for a
        // box domain satisfy
        //   umin <= u <= umax, vmin <= v <= vmax, wmin <= w <= wmax
        ParametricVolume(T const& umin, T const& umax, T const& vmin,
            T const& vmax, T const& wmin, T const& wmax)
            :
            mUMin(umin),
            mUMax(umax),
            mVMin(vmin),
            mVMax(vmax),
            mWMin(wmin),
            mWMax(wmax)
        {
        }

    public:
        virtual ~ParametricVolume() = default;

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

        inline T const& GetWMin() const
        {
            return mWMin;
        }

        inline T const& GetWMax() const
        {
            return mWMax;
        }

        // Evaluation of the volume. If you want only the position, pass in
        // order of 0. If you want the position and first derivatives, pass in
        // order of 1, and so on. The output array 'jet' must have enough
        // storage to support the specified order. The values
        // are ordered as:
        //   jet[0] contains position X
        //   jet[1] contains first-order derivative dX/du
        //   jet[2] contains first-order derivative dX/dv
        //   jet[3] contains first-order derivative dX/dw
        //   jet[4] contains second-order derivative d2X/du2
        //   jet[5] contains second-order derivative d2X/dv2
        //   jet[6] contains second-order derivative d2X/dw2
        //   jet[7] contains second-order derivative d2X/dudv
        //   jet[8] contains second-order derivative d2X/dudw
        //   jet[9] contains second-order derivative d2X/dvdw
        // and so on.
        virtual void Evaluate(T const& u, T const& v, T const& w,
            std::size_t order, Vector<T, N>* jet) const = 0;

        // Differential geometric quantities.
        Vector<T, N> GetPosition(T const& u, T const& v, T const& w) const
        {
            Vector<T, N> position{};
            Evaluate(u, v, w, 0, &position);
            return position;
        }

        Vector<T, N> GetUTangent(T const& u, T const& v, T const& w) const
        {
            std::array<Vector<T, N>, 3> jet{};
            Evaluate(u, v, w, 1, jet.data());
            Normalize(jet[1]);
            return jet[1];
        }

        Vector<T, N> GetVTangent(T const& u, T const& v, T const& w) const
        {
            std::array<Vector<T, N>, 3> jet{};
            Evaluate(u, v, w, 1, jet.data());
            Normalize(jet[2]);
            return jet[2];
        }

        Vector<T, N> GetWTangent(T const& u, T const& v, T const& w) const
        {
            std::array<Vector<T, N>, 3> jet{};
            Evaluate(u, v, w, 1, jet.data());
            Normalize(jet[3]);
            return jet[3];
        }

    protected:
        T mUMin, mUMax, mVMin, mVMax, mWMin, mWMax;

    private:
        friend class UnitTestParametricVolume;
    };
}
