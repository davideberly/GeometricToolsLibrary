// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// Let the torus center be C with plane of symmetry containing C and having
// directions D0 and D1. The axis of symmetry is the line containing C and
// having direction N (the plane normal). The radius from the center of the
// torus is r0 and the radius of the tube of the torus is r1. A point P may
// be written as P = C + x*D0 + y*D1 + z*N, where matrix [D0 D1 N] is
// orthogonal and has determinant 1. Thus, x = Dot(D0,P-C), y = Dot(D1,P-C)
// and z = Dot(N,P-C). The implicit equation defining the torus is
//     (|P-C|^2 + r0^2 - r1^2)^2 - 4*r0^2*(|P-C|^2 - (Dot(N,P-C))^2) = 0
// Observe that D0 and D1 are not present in the equation, which is to be
// expected by the symmetry.
//
// Define u = r0^2 and v = r0^2 - r1^2. Define
//     F(X;C,N,u,v) = (|P-C|^2 + v)^2 - 4*u*(|P-C|^2 - (Dot(N,P-C))^2)
// The nonlinear least-squares fitting of points {X[i]}_{i=0}^{n-1} computes
// C, N, u and v to minimize the error function
//     E(C,N,u,v) = sum_{i=0}^{n-1} F(X[i];C,N,u,v)^2
// When the sample points are distributed so that there is large coverage
// by a purported fitted torus, a variation on fitting is the following.
// Compute the least-squares plane with origin C and normal N that fits the
// points. Define G(X;u,v) = F(X;C,N,u,v); the only variables now are u and
// v. Define L[i] = |X[i]-C|^2 and S[i] = 4 * (L[i] - (Dot(N,X[i]-C))^2).
// Define the error function
//     H(u,v) = sum_{i=0}^{n-1} G(X[i];u,v)^2
//            = sum_{i=0}^{n-1} ((v + L[i])^2 - S[i]*u)^2
// The first-order partial derivatives are
//     dH/du = -2 sum_{i=0}^{n-1} ((v + L[i])^2 - S[i]*u) * S[i]
//     dH/dv =  4 sum_{i=0}^{n-1} ((v + L[i])^2 - S[i]*u) * (v + L[i])
// Setting these to zero and expanding the terms, we have
//     0 = a2 * v^2 + a1 * v + a0 - b0 * u
//     0 = c3 * v^3 + c2 * v^2 + c1 * v + c0 - u * (d1 * v + d0)
// where a2 = sum(S[i]), a1 = 2*sum(S[i]*L[i]), a2 = sum(S[i]*L[i]^2),
// b0 = sum(S[i]^2), c3 = sum(1) = n, c2 = 3*sum(L[i]), c1 = 3*sum(L[i]^2),
// c0 = sum(L[i]^3), d1 = sum(S[i]) = a2 and d0 = sum(S[i]*L[i]) = a1/2.
// The first equation is solved for
//     u = (a2 * v^2 + a1 * v + a0) / b0 = e2 * v^2 + e1 * v + e0
// and substituted into the second equation to obtain a cubic polynomial
// equation
//     0 = f3 * v^3 + f2 * v^2 + f1 * v + f0
// where f3 = c3 - d1 * e2, f2 = c2 - d1 * e1 - d0 * e2,
// f1 = c1 - d1 * e0 - d0 * e1 and f0 = c0 - d0 * e0. The positive v-roots
// are computed. For each root compute the corresponding u. For all pairs
// (u,v) with u > v > 0, evaluate H(u,v) and choose the pair that minimizes
// H(u,v). The torus radii are r0 = sqrt(u) and r1 = sqrt(u - v).

#include <GTL/Mathematics/Approximation/3D/ApprOrthogonalPlane3.h>
#include <GTL/Mathematics/Minimizers/GaussNewtonMinimizer.h>
#include <GTL/Mathematics/Minimizers/LevenbergMarquardtMinimizer.h>
#include <GTL/Mathematics/RootFinders/RootsCubic.h>
#include <cmath>
#include <cstddef>
#include <functional>
#include <map>
#include <vector>

namespace gtl
{
    template <typename T>
    class ApprTorus3
    {
    public:
        // When the samples are distributed approximately uniformly near a
        // torus, use this method. For example, if the purported torus has
        // center (0,0,0) and normal (0,0,1), you want the (x,y,z) samples to
        // occur in all 8 octants. If the samples occur, say, only in one
        // octant, this method will estimate a C and N that are nowhere near
        // (0,0,0) and (0,0,1). The function sets the output variables C, N,
        // r0 and r1 as the fitted torus.
        static bool Fit(std::vector<Vector3<T>> const& points,
            Vector3<T>& C, Vector3<T>& N, T& r0, T& r1)
        {
            if (!ApprOrthogonalPlane3<T>::Fit(points, C, N))
            {
                return false;
            }

            T a0 = C_<T>(0), a1 = C_<T>(0), a2 = C_<T>(0), b0 = C_<T>(0);
            T c0 = C_<T>(0), c1 = C_<T>(0), c2 = C_<T>(0), c3 = static_cast<T>(points.size());
            for (auto const& point : points)
            {
                Vector3<T> delta = point - C;
                T dot = Dot(N, delta);
                T L = Dot(delta, delta), L2 = L * L, L3 = L * L2;
                T S = C_<T>(4) * (L - dot * dot), S2 = S * S;
                a2 += S;
                a1 += S * L;
                a0 += S * L2;
                b0 += S2;
                c2 += L;
                c1 += L2;
                c0 += L3;
            }
            T d1 = a2;
            T d0 = a1;
            a1 *= C_<T>(2);
            c2 *= C_<T>(3);
            c1 *= C_<T>(3);
            T e0 = a0 / b0;
            T e1 = a1 / b0;
            T e2 = a2 / b0;

            T f0 = c0 - d0 * e0;
            T f1 = c1 - d1 * e0 - d0 * e1;
            T f2 = c2 - d1 * e1 - d0 * e2;
            T f3 = c3 - d1 * e2;
            std::array<PolynomialRoot<T>, 3> roots{};
            std::size_t numRoots = RootsCubic<T>::Solve(false, f0, f1, f2, f3, roots.data());

            T hmin = -C_<T>(1);
            T umin = C_<T>(0), vmin = C_<T>(0);
            for (std::size_t i = 0; i < numRoots; ++i)
            {
                T v = roots[i].x;
                if (v > C_<T>(0))
                {
                    T u = e0 + v * (e1 + v * e2);
                    if (u > v)
                    {
                        T h = C_<T>(0);
                        for (auto const& point : points)
                        {
                            Vector3<T> delta = point - C;
                            T dot = Dot(N, delta);
                            T L = Dot(delta, delta);
                            T S = C_<T>(4) * (L - dot * dot);
                            T sum = v + L;
                            T term = sum * sum - S * u;
                            h += term * term;
                        }
                        if (hmin == -C_<T>(1) || h < hmin)
                        {
                            hmin = h;
                            umin = u;
                            vmin = v;
                        }
                    }
                }
            }

            if (hmin >= C_<T>(0))
            {
                r0 = std::sqrt(umin);
                r1 = std::sqrt(umin - vmin);
                return true;
            }
            else
            {
                r0 = C_<T>(0);
                r1 = C_<T>(0);
                return false;
            }
        }

        // If you want to specify that C, N, r0 and r1 are the initial guesses
        // for the minimizer, set the parameter useTorusInputAsInitialGuess to
        // 'true'. If you want the function to compute initial guesses, set
        // useTorusInputAsInitialGuess to 'false'. A Gauss-Newton minimizer
        // is used to fit a torus using nonlinear least-squares. The fitted
        // torus is returned in C, N, r0 and r1.
        static typename GaussNewtonMinimizer<T>::Output
        Fit(std::vector<Vector3<T>> const& points, std::size_t maxIterations,
            T const& updateLengthTolerance, T const& errorDifferenceTolerance,
            bool useTorusInputAsInitialGuess,
            Vector3<T>& C, Vector3<T>& N, T& r0, T& r1)
        {
            std::function<void(Vector<T> const&, Vector<T>&)> FFunction{};
            std::function<void(Vector<T> const&, Matrix<T>&)> JFunction{};
            CreateFunctionObjects(points, FFunction, JFunction);

            GaussNewtonMinimizer<T> minimizer(7, points.size(), FFunction, JFunction);

            if (!useTorusInputAsInitialGuess)
            {
                if (!Fit(points, C, N, r0, r1))
                {
                    return typename GaussNewtonMinimizer<T>::Output{};
                }
            }

            auto initial = InitialGuess(C, N, r0, r1);

            auto output = minimizer(initial, maxIterations, updateLengthTolerance,
                errorDifferenceTolerance);

            // No test is made for output.converged so that we return some
            // estimates of the torus. The caller can decide how to respond
            // when result.converged is false.
            Finalize(output.minLocation, C, N, r0, r1);
            return output;
        }

        // If you want to specify that C, N, r0 and r1 are the initial guesses
        // for the minimizer, set the parameter useTorusInputAsInitialGuess to
        // 'true'. If you want the function to compute initial guesses, set
        // useTorusInputAsInitialGuess to 'false'. A Levenberg-Marquardt
        // minimizer is used to fit a torus using nonlinear least-squares. The
        // fitted torus is returned in C, N, r0 and r1.
        static typename LevenbergMarquardtMinimizer<T>::Output
        Fit(std::vector<Vector3<T>> const& points, std::size_t maxIterations,
            T const& updateLengthTolerance, T const& errorDifferenceTolerance,
            T const& lambdaFactor, T const& lambdaAdjust, std::size_t maxAdjustments,
            bool useTorusInputAsInitialGuess,
            Vector3<T>& C, Vector3<T>& N, T& r0, T& r1)
        {
            std::function<void(Vector<T> const&, Vector<T>&)> FFunction{};
            std::function<void(Vector<T> const&, Matrix<T>&)> JFunction{};
            CreateFunctionObjects(points, FFunction, JFunction);

            LevenbergMarquardtMinimizer<T> minimizer(7, points.size(), FFunction, JFunction);

            if (!useTorusInputAsInitialGuess)
            {
                if (!Fit(points, C, N, r0, r1))
                {
                    return typename LevenbergMarquardtMinimizer<T>::Output{};
                }
            }

            auto initial = InitialGuess(C, N, r0, r1);

            auto output = minimizer(initial, maxIterations, updateLengthTolerance,
                errorDifferenceTolerance, lambdaFactor, lambdaAdjust, maxAdjustments);

            // No test is made for result.converged so that we return some
            // estimates of the torus.  The caller can decide how to respond
            // when result.converged is false.
            Finalize(output.minLocation, C, N, r0, r1);
            return output;
        }

    private:
        static void CreateFunctionObjects(std::vector<Vector3<T>> const& points,
            std::function<void(Vector<T> const&, Vector<T>&)>& FFunction,
            std::function<void(Vector<T> const&, Matrix<T>&)>& JFunction)
        {
            // The unit-length normal is
            //   N = (cos(theta)*sin(phi), sin(theta)*sin(phi), cos(phi)
            // for theta in [0,2*pi) and phi in [0,*pi). The radii are
            // encoded as
            //   u = r0^2, v = r0^2 - r1^2
            // with 0 < v < u. Let D = C - X[i] where X[i] is a sample point.
            // The parameters P = (C0,C1,C2,theta,phi,u,v).

            // F[i](C,theta,phi,u,v) =
            //   (|D|^2 + v)^2 - 4*u*(|D|^2 - Dot(N,D)^2)
            FFunction = [&points](Vector<T> const& P, Vector<T>& F)
            {
                T csTheta = std::cos(P[3]);
                T snTheta = std::sin(P[3]);
                T csPhi = std::cos(P[4]);
                T snPhi = std::sin(P[4]);
                Vector3<T> C{ P[0], P[1], P[2] };
                Vector3<T> N{ csTheta * snPhi, snTheta * snPhi, csPhi };
                T u = P[5];
                T v = P[6];
                for (std::size_t i = 0; i < points.size(); ++i)
                {
                    Vector3<T> D = C - points[i];
                    T DdotD = Dot(D, D), NdotD = Dot(N, D);
                    T sum = DdotD + v;
                    F[i] = sum * sum - C_<T>(4) * u * (DdotD - NdotD * NdotD);
                }
            };

            // dF[i]/dC = 4 * (|D|^2 + v) * D - 8 * u * (I - N*N^T) * D
            // dF[i]/dTheta = 8 * u * Dot(dN/dTheta, D)
            // dF[i]/dPhi = 8 * u * Dot(dN/dPhi, D)
            // dF[i]/du = -4 * u * (|D|^2 - Dot(N,D)^2)
            // dF[i]/dv = 2 * (|D|^2 + v)
            JFunction = [&points](Vector<T> const& P, Matrix<T>& J)
            {
                T csTheta = std::cos(P[3]);
                T snTheta = std::sin(P[3]);
                T csPhi = std::cos(P[4]);
                T snPhi = std::sin(P[4]);
                Vector3<T> C{ P[0], P[1], P[2] };
                Vector3<T> N{ csTheta * snPhi, snTheta * snPhi, csPhi };
                T u = P[5];
                T v = P[6];
                for (std::size_t row = 0; row < points.size(); ++row)
                {
                    Vector3<T> D = C - points[row];
                    T DdotD = Dot(D, D), NdotD = Dot(N, D);
                    T sum = DdotD + v;
                    Vector3<T> dNdTheta{ -snTheta * snPhi, csTheta * snPhi, C_<T>(0) };
                    Vector3<T> dNdPhi{ csTheta * csPhi, snTheta * csPhi, -snPhi };
                    Vector3<T> temp = C_<T>(4) * sum * D - C_<T>(8) * u * (D - NdotD * N);
                    J(row, 0) = temp[0];
                    J(row, 1) = temp[1];
                    J(row, 2) = temp[2];
                    J(row, 3) = C_<T>(8) * u * Dot(dNdTheta, D);
                    J(row, 4) = C_<T>(8) * u * Dot(dNdPhi, D);
                    J(row, 5) = -C_<T>(4) * u * (DdotD - NdotD * NdotD);
                    J(row, 6) = C_<T>(2) * sum;
                }
            };
        }

        static Vector<T> InitialGuess(Vector3<T>& C, Vector3<T>& N, T& r0, T& r1)
        {
            // The initial guess for the plane origin.
            Vector<T> initial(7);
            initial[0] = C[0];
            initial[1] = C[1];
            initial[2] = C[2];

            // The initial guess for the plane normal. The angles must be
            // extracted for spherical coordinates.
            if (std::fabs(N[2]) < C_<T>(1))
            {
                initial[3] = std::atan2(N[1], N[0]);
                initial[4] = std::acos(N[2]);
            }
            else
            {
                initial[3] = C_<T>(0);
                initial[4] = C_<T>(0);
            }

            // The initial guess for the radii parameters.
            initial[5] = r0 * r0;
            initial[6] = initial[5] - r1 * r1;
            return initial;
        }

        static void Finalize(Vector<T> const& minLocation, Vector3<T>& C,
            Vector3<T>& N, T& r0, T& r1)
        {
            C[0] = minLocation[0];
            C[1] = minLocation[1];
            C[2] = minLocation[2];

            T theta = minLocation[3];
            T phi = minLocation[4];
            T csTheta = std::cos(theta);
            T snTheta = std::sin(theta);
            T csPhi = std::cos(phi);
            T snPhi = std::sin(phi);
            N[0] = csTheta * snPhi;
            N[1] = snTheta * snPhi;
            N[2] = csPhi;

            T u = minLocation[5];
            T v = minLocation[6];
            r0 = std::sqrt(u);
            r1 = std::sqrt(std::max(u - v, C_<T>(0)));
        }

    private:
        friend class UnitTestApprTorus3;
    };
}
