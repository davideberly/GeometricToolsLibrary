// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

#include <GTL/Mathematics/Curves/ParametricCurve.h>
#include <GTL/Mathematics/Curves/BasisFunction.h>
#include <algorithm>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T, std::size_t N>
    class BSplineCurve : public ParametricCurve<T, N>
    {
    public:
        BSplineCurve()
            :
            ParametricCurve<T, N>(C_<T>(0), C_<T>(1)),
            mBasisFunction{},
            mControls{}
        {
        }

        // If the input controls is non-null, a copy is made of the controls.
        // To defer setting the control points, pass a null pointer and later
        // access the control points via GetControls() or SetControl() member
        // functions. The domain is t in [t[d],t[n]], where t[d] and t[n] are
        // knots with d the degree and n the number of control points.
        BSplineCurve(typename BasisFunction<T>::Input const& input,
            Vector<T, N> const* controls)
            :
            ParametricCurve<T, N>(C_<T>(0), C_<T>(1)),
            mBasisFunction(input),
            mControls(input.numControls)
        {
            // The mBasisFunction stores the domain, but copies are stored in
            // ParametricCurve.
            this->mTime.front() = mBasisFunction.GetMinDomain();
            this->mTime.back() = mBasisFunction.GetMaxDomain();

            // The replication of control points for periodic splines is
            // avoided by wrapping the i-loop index in Evaluate.
            if (controls)
            {
                std::copy(controls, controls + input.numControls, mControls.begin());
            }
            else
            {
                Vector<T, N> zero{};
                std::fill(mControls.begin(), mControls.end(), zero);
            }
        }

        virtual ~BSplineCurve() = default;

        // Member access.
        inline BasisFunction<T> const& GetBasisFunction() const
        {
            return mBasisFunction;
        }

        inline std::size_t GetNumControls() const
        {
            return mControls.size();
        }

        inline Vector<T, N> const* GetControls() const
        {
            return mControls.data();
        }

        inline Vector<T, N>* GetControls()
        {
            return mControls.data();
        }

        void SetControl(std::size_t i, Vector<T, N> const& control)
        {
            GTL_ARGUMENT_ASSERT(
                i < GetNumControls(),
                "Invalid index.");

            mControls[i] = control;
        }

        Vector<T, N> const& GetControl(std::size_t i) const
        {
            GTL_ARGUMENT_ASSERT(
                i < GetNumControls(),
                "Invalid index.");

            return mControls[i];
        }

        // Evaluation of the curve. It is required that order <= 3, which
        // allows computing derivatives through order 3. If you want only the
        // position, pass in order of 0. If you want the position and first
        // derivative, pass in order of 1, and so on. The output array 'jet'
        // must have enough storage to support the specified order. The values
        // are ordered as: position, first derivative, second derivative, and
        // so on.
        virtual void Evaluate(T const& t, std::size_t order, Vector<T, N>* jet) const override
        {
            std::size_t imin = 0, imax = 0;
            mBasisFunction.Evaluate(t, order, imin, imax);

            // Compute position.
            jet[0] = Compute(0, imin, imax);
            if (order >= 1)
            {
                // Compute first derivative.
                jet[1] = Compute(1, imin, imax);
                if (order >= 2)
                {
                    // Compute second derivative.
                    jet[2] = Compute(2, imin, imax);
                    if (order == 3)
                    {
                        jet[3] = Compute(3, imin, imax);
                    }
                }
            }
        }

    private:
        // Support for Evaluate(...).
        Vector<T, N> Compute(std::size_t order, std::size_t imin, std::size_t imax) const
        {
            // The j-index introduces a tiny amount of overhead in order to
            // handle both aperiodic and periodic splines. For aperiodic
            // splines, j = i always.

            std::size_t const numControls = GetNumControls();
            Vector<T, N> result{};
            for (std::size_t i = imin; i <= imax; ++i)
            {
                T tmp = mBasisFunction.GetValue(order, i);
                std::size_t j = (i >= numControls ? i - numControls : i);
                result += tmp * mControls[j];
            }
            return result;
        }

        BasisFunction<T> mBasisFunction;
        std::vector<Vector<T, N>> mControls;

    private:
        friend class UnitTestBSplineCurve;
    };
}
