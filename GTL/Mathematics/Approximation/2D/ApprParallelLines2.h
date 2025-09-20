// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.09.20

#pragma once

// Least-squares fit of two parallel lines to points that presumably are
// clustered on the lines. The algorithm is described in
//   https://www.geometrictools.com/Documentation/FitParallelLinesToPoints2D.pdf

#include <GTL/Mathematics/Algebra/Polynomial.h>
#include <GTL/Mathematics/RootFinders/RootsGeneralPolynomial.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace gtl
{
    template <typename T>
    class ApprParallelLines2
    {
    public:
        ApprParallelLines2()
            :
            mR0(0), mR1(1), mR2(2), mR3(3), mR4(4), mR5(5), mR6(6)
        {
            // The constants are set here in case T is a rational type, which
            // avoids construction costs for those types.
        }

        void Fit(std::vector<Vector2<T>> const& P, Vector2<T>& C, Vector2<T>& V, T& radius)
        {
            // Compute the average of the samples.
            std::size_t const n = P.size();
            T const invN = static_cast<T>(1) / static_cast<T>(n);
            std::vector<Vector2<T>> PAdjust = P;
            Vector2<T> A{ mR0, mR0 };
            for (auto const& sample : PAdjust)
            {
                A += sample;
            }
            A *= invN;

            // Subtract the average from the samples so that the replacement
            // points have zero average.
            for (auto& sample : PAdjust)
            {
                sample -= A;
            }

            // Compute the Zpq terms.
            ZValues data(PAdjust);

            // Compute F(sigma,gamma) = f0(sigma) + gamma * f1(sigma).
            Polynomial1<T> f0{}, f1{};
            ComputeF(data, f0, f1);
            Polynomial1<T> freduced0(4), freduced1(3);
            for (std::size_t i = 0; i <= freduced0.GetDegree(); ++i)
            {
                freduced0[i] = f0[2 * i];
            }
            for (std::size_t i = 0; i <= freduced1.GetDegree(); ++i)
            {
                freduced1[i] = f1[2 * i + 1];
            }

            // Evaluate the error function at any (sigma,gamma). Choose (0,1)
            // so that we do not have to process a root sigma=0 later.
            T minSigma = mR0, minGamma = mR1;
            T minK = data.Z03 / (mR2 * data.Z02);
            T minKSqr = minK * minK;
            T minRSqr = minKSqr + data.Z02;
            T minError = data.Z04 - mR4 * minK * data.Z03 + (mR4 * minKSqr - data.Z02) * data.Z02;

            if (f1 != Polynomial1<T>{ mR0 })
            {
                Polynomial1<T> sigmaSqrPoly{ mR0, mR0, mR1 };
                Polynomial1<T> f0Sqr = f0 * f0, f1Sqr = f1 * f1;
                Polynomial1<T> h = sigmaSqrPoly * f1Sqr + (f0Sqr - f1Sqr);
                Polynomial1<T> hreduced(8);
                for (std::size_t i = 0; i <= hreduced.GetDegree(); ++i)
                {
                    hreduced[i] = h[2 * i];
                }

                std::vector<T> roots{}; // at most 8 roots
                RootsGeneralPolynomial<T>::Solve(hreduced.GetCoefficients(), true, roots);
                for (auto root : roots)
                {
                    T sigmaSqr = root;
                    if (sigmaSqr > mR0)
                    {
                        T sigma = std::sqrt(sigmaSqr);
                        T gamma = -freduced0(sigmaSqr) / (sigma * freduced1(sigmaSqr));
                        UpdateParameters(data, sigma, sigmaSqr, gamma,
                            minSigma, minGamma, minK, minRSqr, minError);
                    }
                }
            }
            else
            {
                Polynomial1<T> hreduced(4);
                for (int32_t i = 0; i <= 4; ++i)
                {
                    hreduced[i] = f0[2 * i];
                }

                std::vector<T> roots{};  // at most 4 roots
                RootsGeneralPolynomial<T>::Solve(hreduced.GetCoefficients(), true, roots);
                for (auto root : roots)
                {
                    T sigmaSqr = root;
                    if (sigmaSqr > mR0)
                    {
                        T sigma = std::sqrt(sigmaSqr);
                        T gamma = std::sqrt(sigma);
                        UpdateParameters(data, sigma, sigmaSqr, gamma,
                            minSigma, minGamma, minK, minRSqr, minError);

                        gamma = -gamma;
                        UpdateParameters(data, sigma, sigmaSqr, gamma,
                            minSigma, minGamma, minK, minRSqr, minError);
                    }
                }
            }

            // Compute the minimizers V, C and radius. The center minK*U must
            // have A added to it because the inputs P had A subtracted from
            // them. The addition no longer guarantees that Dot(V,C) = 0, so
            // the V-component of A+minK*U is projected out so that the
            // returned center has only a U-component.
            V = Vector2<T>{ minGamma, minSigma };
            C = A + minK * Vector2<T>{ -minSigma, minGamma };
            C -= Dot(C, V) * V;
            radius = std::sqrt(minRSqr);
        }
    private:
        struct ZValues
        {
            ZValues(std::vector<Vector2<T>> const& P)
                :
                Z20(0), Z11(0), Z02(0),
                Z30(0), Z21(0), Z12(0), Z03(0),
                Z40(0), Z31(0), Z22(0), Z13(0), Z04(0)
            {
                T const invN = static_cast<T>(1) / static_cast<T>(P.size());
                for (auto const& sample : P)
                {
                    T xx = sample[0] * sample[0];
                    T xy = sample[0] * sample[1];
                    T yy = sample[1] * sample[1];
                    T xxx = xx * sample[0];
                    T xxy = xy * sample[0];
                    T xyy = xy * sample[1];
                    T yyy = yy * sample[1];
                    T xxxx = xxx * sample[0];
                    T xxxy = xxx * sample[1];
                    T xxyy = xx * yy;
                    T xyyy = yyy * sample[0];
                    T yyyy = yyy * sample[1];
                    Z20 += xx;
                    Z11 += xy;
                    Z02 += yy;
                    Z30 += xxx;
                    Z21 += xxy;
                    Z12 += xyy;
                    Z03 += yyy;
                    Z40 += xxxx;
                    Z31 += xxxy;
                    Z22 += xxyy;
                    Z13 += xyyy;
                    Z04 += yyyy;
                }
                Z20 *= invN;
                Z11 *= invN;
                Z02 *= invN;
                Z30 *= invN;
                Z21 *= invN;
                Z12 *= invN;
                Z03 *= invN;
                Z40 *= invN;
                Z31 *= invN;
                Z22 *= invN;
                Z13 *= invN;
                Z04 *= invN;
            }

            T Z20, Z11, Z02;
            T Z30, Z21, Z12, Z03;
            T Z40, Z31, Z22, Z13, Z04;
        };

        // Given two polynomials A0+gamma*B0 and A1+gamma*B1, the product is
        // [A0*A1+(1-sigma^2)*B0*B1] + gamma*[A0*B1+B0*A1] = A2+gamma*B2.
        void ComputeProduct(
            Polynomial1<T> const& A0, Polynomial1<T> const& B0,
            Polynomial1<T> const& A1, Polynomial1<T> const& B1,
            Polynomial1<T>& A2, Polynomial1<T>& B2)
        {
            Polynomial1<T> gammaSqr{ mR1, mR0, -mR1 };
            A2 = A0 * A1 + gammaSqr * B0 * B1;
            B2 = A0 * B1 + B0 * A1;
        }

        void ComputeF(ZValues const& data, Polynomial1<T>& f0, Polynomial1<T>& f1)
        {
            // Compute the apq and bpq terms.
            Polynomial1<T> a11(2);
            a11[0] = data.Z11;
            a11[2] = -mR2 * data.Z11;

            Polynomial1<T> b11(1);
            b11[1] = data.Z02 - data.Z20;

            Polynomial1<T> a20(2);
            a20[0] = data.Z02;
            a20[2] = data.Z20 - data.Z02;

            Polynomial1<T> b20(1);
            b20[1] = -mR2 * data.Z11;

            Polynomial1<T> a30(3);
            a30[1] = -mR3;
            a30[3] = mR3 * data.Z12 - data.Z30;

            Polynomial1<T> b30(2);
            b30[0] = data.Z03;
            b30[2] = mR3 * data.Z21 - data.Z03;

            Polynomial1<T> a21(3);
            a21[1] = data.Z03 - mR2 * data.Z21;
            a21[3] = mR3 * data.Z21 - data.Z03;

            Polynomial1<T> b21(2);
            b21[0] = data.Z12;
            b21[2] = data.Z30 - mR3 * data.Z12;

            Polynomial1<T> a40(4);
            a40[0] = data.Z04;
            a40[2] = mR6 * data.Z22 - mR2 * data.Z04;
            a40[4] = data.Z40 - mR6 * data.Z22 + data.Z04;

            Polynomial1<T> b40(3);
            b40[1] = -mR4 * data.Z13;
            b40[3] = mR4 * (data.Z13 - data.Z31);

            Polynomial1<T> a31(4);
            a31[0] = data.Z13;
            a31[2] = mR3 * data.Z31 - mR5 * data.Z13;
            a31[4] = mR4 * (data.Z13 - data.Z31);

            Polynomial1<T> b31(3);
            b31[1] = data.Z04 - mR3 * data.Z22;
            b31[3] = mR6 * data.Z22 - data.Z40 - data.Z04;

            // Compute S20^2 = c0 + gamma*d0.
            Polynomial1<T> c0, d0;
            ComputeProduct(a20, b20, a20, b20, c0, d0);

            // Compute S31 * S20^2 = c1 + gamma*d1.
            Polynomial1<T> c1, d1;
            ComputeProduct(a31, b31, c0, d0, c1, d1);

            // Compute S21 * S20 = c2 + gamma*d2.
            Polynomial1<T> c2, d2;
            ComputeProduct(a21, b21, a20, b20, c2, d2);

            // Compute S30 * (S21 * S20) = c3 + gamma*d3.
            Polynomial1<T> c3, d3;
            ComputeProduct(a30, b30, c2, d2, c3, d3);

            // Compute S30 * S11 = c4 + gamma*d4.
            Polynomial1<T> c4, d4;
            ComputeProduct(a30, b30, a11, b11, c4, d4);

            // Compute S30 * (S30 * S11) = c5 + gamma*d5.
            Polynomial1<T> c5, d5;
            ComputeProduct(a30, b30, c4, d4, c5, d5);

            // Compute S20^2 * S11 = c6 + gamma*d6.
            Polynomial1<T> c6, d6;
            ComputeProduct(c0, d0, a11, b11, c6, d6);

            // Compute S20 * (S20^2 * S11) = c7 + gamma*d7.
            Polynomial1<T> c7, d7;
            ComputeProduct(a20, b20, c6, d6, c7, d7);

            // Compute F = 2*S31*S20^2 - 3*S30*S21*S20 + S30^2*S11
            // - 2*S20^3*S11 = f0 + gamma*f1, where f0 is even of degree 8
            // and f1 is odd of degree 7.
            f0 = mR2 * (c1 - c7) - mR3 * c3 + c5;
            f1 = mR2 * (d1 - d7) - mR3 * d3 + d5;
        }

        void UpdateParameters(ZValues const& data, T const& sigma, T const& sigmaSqr,
            T const& gamma, T& minSigma, T& minGamma, T& minK,
            T& minRSqr, T& minError)
        {
            // Rather than evaluate apq(sigma) and bpq(sigma), the
            // polynomials are evaluated at sigmaSqr to avoid the
            // rounding errors that are inherent by computing
            // s = sqrt(ssqr); ssqr = s * s;
            T A20 = data.Z02 + (data.Z20 - data.Z02) * sigmaSqr;
            T B20 = -mR2 * data.Z11 * sigma;
            T S20 = A20 + gamma * B20;
            T A30 = -sigma * (mR3 * data.Z12 + (data.Z30 - mR3 * data.Z12) * sigmaSqr);
            T B30 = data.Z03 + (mR3 * data.Z21 - data.Z03) * sigmaSqr;
            T S30 = A30 + gamma * B30;
            T A40 = data.Z04 + ((mR6 * data.Z22 - mR2 * data.Z04)
                + (data.Z40 - mR6 * data.Z22 + data.Z04) * sigmaSqr) * sigmaSqr;
            T B40 = -mR4 * sigma * (data.Z13 + (data.Z31 - data.Z13) * sigmaSqr);
            T S40 = A40 + gamma * B40;
            T k = S30 / (mR2 * S20);
            T ksqr = k * k;
            T rsqr = ksqr + S20;
            T error = S40 - mR4 * k * S30 + (mR4 * ksqr - S20) * S20;
            if (error < minError)
            {
                minSigma = sigma;
                minGamma = gamma;
                minK = k;
                minRSqr = rsqr;
                minError = error;
            }
        }

        T const mR0, mR1, mR2, mR3, mR4, mR5, mR6;

    private:
        friend class UnitTestApprParallelLines2;
};
}

