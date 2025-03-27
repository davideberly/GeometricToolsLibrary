// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

#include <GTL/Mathematics/Curves/BasisFunction.h>
#include <GTL/Mathematics/Surfaces/ParametricSurface.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T, std::size_t N>
    class BSplineSurface : public ParametricSurface<T, N>
    {
    public:
        BSplineSurface()
            :
            ParametricSurface<T, N>(
                C_<T>(0), C_<T>(1),
                C_<T>(0), C_<T>(1),
                true),
            mBasisFunction{},
            mNumControls{ 0, 0 },
            mControls{}
        {
        }

        // Construction. If the input controls is non-null, a copy is made of
        // the controls. To defer setting the control points, pass a null
        // pointer and later access the control points via GetControls() or
        // SetControl() member functions. The input 'controls' must be stored
        // in row-major order, control[i0 + numControls0*i1]. As a 2D array,
        // this corresponds to control2D[i1][i0].
        BSplineSurface(
            std::array<typename BasisFunction<T>::Input, 2> const& input,
            Vector<T, N> const* controls)
            :
            ParametricSurface<T, N>(
                C_<T>(0), C_<T>(1),
                C_<T>(0), C_<T>(1),
                true),
            mBasisFunction{},
            mNumControls{ 0, 0 },
            mControls{}
        {
            for (std::size_t i = 0; i < 2; ++i)
            {
                mNumControls[i] = input[i].numControls;
                mBasisFunction[i].Create(input[i]);
            }

            // The mBasisFunction stores the domain, but copies are stored in
            // ParametricSurface.
            this->mUMin = mBasisFunction[0].GetMinDomain();
            this->mUMax = mBasisFunction[0].GetMaxDomain();
            this->mVMin = mBasisFunction[1].GetMinDomain();
            this->mVMax = mBasisFunction[1].GetMaxDomain();

            // The replication of control points for periodic splines is
            // avoided by wrapping the i-loop index in Evaluate.
            std::size_t numControls = mNumControls[0] * mNumControls[1];
            mControls.resize(numControls);
            if (controls)
            {
                std::copy(controls, controls + numControls, mControls.begin());
            }
            else
            {
                Vector<T, N> zero{};
                std::fill(mControls.begin(), mControls.end(), zero);
            }
        }

        virtual ~BSplineSurface() = default;

        // Member access. The index 'dim' must be in {0,1}.
        BasisFunction<T> const& GetBasisFunction(std::size_t dim) const
        {
            GTL_ARGUMENT_ASSERT(
                dim <= 1,
                "Invalid dimension.");

            return mBasisFunction[dim];
        }

        std::size_t GetNumControls(std::size_t dim) const
        {
            GTL_ARGUMENT_ASSERT(
                dim <= 1,
                "Invalid dimension.");

            return mNumControls[dim];
        }

        inline Vector<T, N> const* GetControls() const
        {
            return mControls.data();
        }

        inline Vector<T, N>* GetControls()
        {
            return mControls.data();
        }

        void SetControl(std::size_t i0, std::size_t i1, Vector<T, N> const& control)
        {
            GTL_ARGUMENT_ASSERT(
                i0 < GetNumControls(0) && i1 < GetNumControls(1),
                "Invalid index.");

            mControls[i0 + mNumControls[0] * i1] = control;
        }

        Vector<T, N> const& GetControl(std::size_t i0, std::size_t i1) const
        {
            GTL_ARGUMENT_ASSERT(
                i0 < GetNumControls(0) && i1 < GetNumControls(1),
                "Invalid index.");

            return mControls[i0 + mNumControls[0] * i1];
        }

        // Evaluation of the surface. It is required that order <= 2, which
        // allows computing derivatives through order 2. If you want only the
        // position, pass in order of 0. If you want the position and first
        // derivatives, pass in order of 1, and so on. The output array 'jet'
        // must have enough storage to support the specified order. The values
        // are ordered as:
        //   jet[0] contains position X
        //   jet[1] contains first-order derivative dX/du
        //   jet[2] contains first-order derivative dX/dv
        //   jet[3] contains second-order derivative d2X/du2
        //   jet[4] contains second-order derivative d2X/dudv
        //   jet[5] contains second-order derivative d2X/dv2
        // and so on.
        virtual void Evaluate(T const& u, T const& v, std::size_t order, Vector<T, N>* jet) const override
        {
            std::size_t iumin = 0, iumax = 0, ivmin = 0, ivmax = 0;
            mBasisFunction[0].Evaluate(u, order, iumin, iumax);
            mBasisFunction[1].Evaluate(v, order, ivmin, ivmax);

            // Compute position.
            jet[0] = Compute(0, 0, iumin, iumax, ivmin, ivmax);
            if (order >= 1)
            {
                // Compute first-order derivatives.
                jet[1] = Compute(1, 0, iumin, iumax, ivmin, ivmax);
                jet[2] = Compute(0, 1, iumin, iumax, ivmin, ivmax);
                if (order >= 2)
                {
                    // Compute second-order derivatives.
                    jet[3] = Compute(2, 0, iumin, iumax, ivmin, ivmax);
                    jet[4] = Compute(1, 1, iumin, iumax, ivmin, ivmax);
                    jet[5] = Compute(0, 2, iumin, iumax, ivmin, ivmax);
                }
            }
        }

    private:
        // Support for Evaluate(...).
        Vector<T, N> Compute(std::size_t uOrder, std::size_t vOrder,
            std::size_t iumin, std::size_t iumax, std::size_t ivmin, std::size_t ivmax) const
        {
            // The j*-indices introduce a tiny amount of overhead in order to
            // handle both aperiodic and periodic splines. For aperiodic
            // splines, j* = i* always.

            std::size_t const numControls0 = mNumControls[0];
            std::size_t const numControls1 = mNumControls[1];
            Vector<T, N> result{};
            for (std::size_t iv = ivmin; iv <= ivmax; ++iv)
            {
                T tmpv = mBasisFunction[1].GetValue(vOrder, iv);
                std::size_t jv = (iv >= numControls1 ? iv - numControls1 : iv);
                for (std::size_t iu = iumin; iu <= iumax; ++iu)
                {
                    T tmpu = mBasisFunction[0].GetValue(uOrder, iu);
                    std::size_t ju = (iu >= numControls0 ? iu - numControls0 : iu);
                    result += (tmpu * tmpv) * mControls[ju + numControls0 * jv];
                }
            }
            return result;
        }

        std::array<BasisFunction<T>, 2> mBasisFunction;
        std::array<std::size_t, 2> mNumControls;
        std::vector<Vector<T, N>> mControls;

    private:
        friend class UnitTestBSplineSurface;
    };
}
