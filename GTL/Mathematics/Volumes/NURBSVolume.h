// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.24

#pragma once

#include <GTL/Mathematics/Volumes/ParametricVolume.h>
#include <GTL/Mathematics/Curves/BasisFunction.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T, std::size_t N>
    class NURBSVolume : public ParametricVolume<T, N>
    {
    public:
        // If the input controls is non-null, a copy is made of the controls.
        // To defer setting the control points or weights, pass null pointers
        // and later access the control points or weights via GetControls(),
        // GetWeights(), SetControl(), or SetWeight() member functions. The
        // 'controls' and 'weights' must be stored in lexicographical order,
        //   attribute[i0 + numControls0 * (i1 + numControls1 * i2)]
        // As a 3D array, this corresponds to attribute3D[i2][i1][i0].
        NURBSVolume(
            std::array<typename BasisFunction<T>::Input, 3> const& input,
            Vector<T, N> const* controls, T const* weights)
            :
            ParametricVolume<T, N>(
                C_<T>(0), C_<T>(1),
                C_<T>(0), C_<T>(1),
                C_<T>(0), C_<T>(1))
        {
            for (std::size_t i = 0; i < 3; ++i)
            {
                mNumControls[i] = input[i].numControls;
                mBasisFunction[i].Create(input[i]);
            }

            // The mBasisFunction stores the domain, but copies are stored in
            // ParametricVolume.
            this->mUMin = mBasisFunction[0].GetMinDomain();
            this->mUMax = mBasisFunction[0].GetMaxDomain();
            this->mVMin = mBasisFunction[1].GetMinDomain();
            this->mVMax = mBasisFunction[1].GetMaxDomain();
            this->mWMin = mBasisFunction[2].GetMinDomain();
            this->mWMax = mBasisFunction[2].GetMaxDomain();

            // The replication of control points for periodic splines is
            // avoided by wrapping the i-loop index in Evaluate.
            std::size_t numControls = mNumControls[0] * mNumControls[1] * mNumControls[2];
            mControls.resize(numControls);
            mWeights.resize(numControls);
            if (controls)
            {
                std::copy(controls, controls + numControls, mControls.begin());
            }
            else
            {
                Vector<T, N> vzero{};
                std::fill(mControls.begin(), mControls.end(), vzero);
            }
            if (weights)
            {
                std::copy(weights, weights + numControls, mWeights.begin());
            }
            else
            {
                std::fill(mWeights.begin(), mWeights.end(), C_<T>(1));
            }
        }

        // Member access. The index 'dim' must be in {0,1,2}.
        BasisFunction<T> const& GetBasisFunction(std::size_t dim) const
        {
            GTL_ARGUMENT_ASSERT(
                dim <= 2,
                "Invalid dimension.");

            return mBasisFunction[dim];
        }

        T GetMinDomain(std::size_t dim) const
        {
            GTL_ARGUMENT_ASSERT(
                dim <= 2,
                "Invalid dimension.");

            return mBasisFunction[dim].GetMinDomain();
        }

        T GetMaxDomain(std::size_t dim) const
        {
            GTL_ARGUMENT_ASSERT(
                dim <= 2,
                "Invalid dimension.");

            return mBasisFunction[dim].GetMaxDomain();
        }

        inline std::size_t GetNumControls(std::size_t dim) const
        {
            GTL_ARGUMENT_ASSERT(
                dim <= 2,
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

        void SetControl(std::size_t i0, std::size_t i1, std::size_t i2, Vector<T, N> const& control)
        {
            GTL_ARGUMENT_ASSERT(
                i0 < GetNumControls(0) && i1 < GetNumControls(1) && i2 < GetNumControls(2),
                "Invalid index.");

            mControls[i0 + mNumControls[0] * (i1 + mNumControls[1] * i2)] = control;
        }

        Vector<T, N> const& GetControl(std::size_t i0, std::size_t i1, std::size_t i2) const
        {
            GTL_ARGUMENT_ASSERT(
                i0 < GetNumControls(0) && i1 < GetNumControls(1) && i2 < GetNumControls(2),
                "Invalid index.");

            return mControls[i0 + mNumControls[0] * (i1 + mNumControls[1] * i2)];
        }

        inline T const* GetWeights() const
        {
            return mWeights.data();
        }

        inline T* GetWeights()
        {
            return mWeights.data();
        }

        void SetWeight(std::size_t i0, std::size_t i1, std::size_t i2, T const& weight)
        {
            GTL_ARGUMENT_ASSERT(
                i0 < GetNumControls(0) && i1 < GetNumControls(1) && i2 < GetNumControls(2),
                "Invalid index.");

            mWeights[i0 + mNumControls[0] * (i1 + mNumControls[1] * i2)] = weight;
        }

        T const& GetWeight(std::size_t i0, std::size_t i1, std::size_t i2) const
        {
            GTL_ARGUMENT_ASSERT(
                i0 < GetNumControls(0) && i1 < GetNumControls(1) && i2 < GetNumControls(2),
                "Invalid index.");

            return mWeights[i0 + mNumControls[0] * (i1 + mNumControls[1] * i2)];
        }

        // Evaluation of the volume. It is required that order <= 2, which
        // allows computing derivatives through order 2. If you want only the
        // position, pass in order of 0. If you want the position and first
        // derivatives, pass in order of 1, and so on. The output array 'jet'
        // must have enough storage to support the specified order. The values
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
            std::size_t order, Vector<T, N>* jet) const override
        {
            std::size_t iumin = 0, iumax = 0, ivmin = 0, ivmax = 0, iwmin = 0, iwmax = 0;
            mBasisFunction[0].Evaluate(u, order, iumin, iumax);
            mBasisFunction[1].Evaluate(v, order, ivmin, ivmax);
            mBasisFunction[2].Evaluate(w, order, iwmin, iwmax);

            // Compute position.
            Vector<T, N> X{};
            T h = C_<T>(0);
            Compute(0, 0, 0, iumin, iumax, ivmin, ivmax, iwmin, iwmax, X, h);
            T invH = C_<T>(1) / h;
            jet[0] = invH * X;

            if (order >= 1)
            {
                // Compute first-order derivatives.
                Vector<T, N> XDerU{};
                T hDerU = C_<T>(0);
                Compute(1, 0, 0, iumin, iumax, ivmin, ivmax, iwmin, iwmax, XDerU, hDerU);
                jet[1] = invH * (XDerU - hDerU * jet[0]);

                Vector<T, N> XDerV{};
                T hDerV = C_<T>(0);
                Compute(0, 1, 0, iumin, iumax, ivmin, ivmax, iwmin, iwmax, XDerV, hDerV);
                jet[2] = invH * (XDerV - hDerV * jet[0]);

                Vector<T, N> XDerW{};
                T hDerW = C_<T>(0);
                Compute(0, 0, 1, iumin, iumax, ivmin, ivmax, iwmin, iwmax, XDerW, hDerW);
                jet[3] = invH * (XDerW - hDerW * jet[0]);

                if (order >= 2)
                {
                    // Compute second-order derivatives.
                    Vector<T, N> XDerUU{};
                    T hDerUU = C_<T>(0);
                    Compute(2, 0, 0, iumin, iumax, ivmin, ivmax, iwmin, iwmax, XDerUU, hDerUU);
                    jet[4] = invH * (XDerUU - C_<T>(2) * hDerU * jet[1] - hDerUU * jet[0]);

                    Vector<T, N> XDerVV{};
                    T hDerVV = C_<T>(0);
                    Compute(0, 2, 0, iumin, iumax, ivmin, ivmax, iwmin, iwmax, XDerVV, hDerVV);
                    jet[5] = invH * (XDerVV - C_<T>(2) * hDerV * jet[2] - hDerVV * jet[0]);

                    Vector<T, N> XDerWW{};
                    T hDerWW = C_<T>(0);
                    Compute(0, 0, 2, iumin, iumax, ivmin, ivmax, iwmin, iwmax, XDerWW, hDerWW);
                    jet[6] = invH * (XDerWW - C_<T>(2) * hDerW * jet[3] - hDerWW * jet[0]);

                    Vector<T, N> XDerUV{};
                    T hDerUV = C_<T>(0);
                    Compute(1, 1, 0, iumin, iumax, ivmin, ivmax, iwmin, iwmax, XDerUV, hDerUV);
                    jet[7] = invH * (XDerUV - hDerU * jet[2] - hDerV * jet[1] - hDerUV * jet[0]);

                    Vector<T, N> XDerUW{};
                    T hDerUW = C_<T>(0);
                    Compute(1, 0, 1, iumin, iumax, ivmin, ivmax, iwmin, iwmax, XDerUW, hDerUW);
                    jet[8] = invH * (XDerUW - hDerU * jet[3] - hDerW * jet[1] - hDerUW * jet[0]);

                    Vector<T, N> XDerVW{};
                    T hDerVW = C_<T>(0);
                    Compute(0, 1, 1, iumin, iumax, ivmin, ivmax, iwmin, iwmax, XDerVW, hDerVW);
                    jet[9] = invH * (XDerVW - hDerV * jet[3] - hDerW * jet[2] - hDerVW * jet[0]);
                }
            }
        }

    private:
        // Support for Evaluate(...).
        void Compute(std::size_t uOrder, std::size_t vOrder, std::size_t wOrder,
            std::size_t iumin, std::size_t iumax, std::size_t ivmin, std::size_t ivmax,
            std::size_t iwmin, std::size_t iwmax, Vector<T, N>& X, T& h) const
        {
            // The j*-indices introduce a tiny amount of overhead in order to
            // handle both aperiodic and periodic splines.  For aperiodic
            // splines, j* = i* always.

            std::size_t const numControls0 = mNumControls[0];
            std::size_t const numControls1 = mNumControls[1];
            std::size_t const numControls2 = mNumControls[2];
            MakeZero(X);
            h = C_<T>(0);
            for (std::size_t iw = iwmin; iw <= iwmax; ++iw)
            {
                T tmpw = mBasisFunction[2].GetValue(wOrder, iw);
                std::size_t jw = (iw >= numControls2 ? iw - numControls2 : iw);
                for (std::size_t iv = ivmin; iv <= ivmax; ++iv)
                {
                    T tmpv = mBasisFunction[1].GetValue(vOrder, iv);
                    T tmpvw = tmpv * tmpw;
                    std::size_t jv = (iv >= numControls1 ? iv - numControls1 : iv);
                    for (std::size_t iu = iumin; iu <= iumax; ++iu)
                    {
                        T tmpu = mBasisFunction[0].GetValue(uOrder, iu);
                        std::size_t ju = (iu >= numControls0 ? iu - numControls0 : iu);
                        std::size_t index = ju + numControls0 * (jv + numControls1 * jw);
                        T tmp = (tmpu * tmpvw) * mWeights[index];
                        X += tmp * mControls[index];
                        h += tmp;
                    }
                }
            }
        }

        std::array<BasisFunction<T>, 3> mBasisFunction;
        std::array<std::size_t, 3> mNumControls;
        std::vector<Vector<T, N>> mControls;
        std::vector<T> mWeights;

    private:
        friend class UnitTestNURBSVolume;
    };
}
