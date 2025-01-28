// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// The test-intersection and find-intersection queries implemented here are
// discussed in the document
// https://www.geometrictools.com/Documentation/IntersectionOfEllipses.pdf
// The T type should support exact rational arithmetic in order for the
// polynomial root construction to be robust.s The classification of the
// intersections depends on various sign tests of computed values.  If these
// values are computed with floating-point arithmetic, the sign tests can
// lead to misclassification.
//
// The find-intersection query had some robustness issues when computing with
// floating-point only. The current implementation fixes those. The algorithm
// is described in
// https://www.geometrictools.com/Documentation/RobustIntersectionOfEllipses.pdf

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Primitives/ND/AlignedBox.h>
#include <GTL/Mathematics/Primitives/2D/Ellipse2.h>
#include <GTL/Mathematics/Arithmetic/IEEEFunctions.h>
#include <GTL/Mathematics/Algebra/Matrix.h>
#include <GTL/Mathematics/Algebra/Polynomial.h>
#include <GTL/Mathematics/RootFinders/RootsBisection1.h>
#include <GTL/Mathematics/RootFinders/RootsQuartic.h>
#include <GTL/Mathematics/MatrixAnalysis/SymmetricEigensolver.h>
#include <array>
#include <cmath>
#include <cstddef>
#include <functional>
#include <limits>
#include <map>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Ellipse2<T>, Ellipse2<T>>
    {
    public:
        // The query tests the relationship between the ellipses as solid
        // objects.
        enum class Output
        {
            ELLIPSES_UNKNOWN,
            ELLIPSES_SEPARATED,
            ELLIPSES_OVERLAP,
            ELLIPSE0_OUTSIDE_ELLIPSE1_BUT_TANGENT,
            ELLIPSE0_STRICTLY_CONTAINS_ELLIPSE1,
            ELLIPSE0_CONTAINS_ELLIPSE1_BUT_TANGENT,
            ELLIPSE1_STRICTLY_CONTAINS_ELLIPSE0,
            ELLIPSE1_CONTAINS_ELLIPSE0_BUT_TANGENT,
            ELLIPSES_EQUAL
        };

        // The ellipse axes are already normalized, which most likely
        // introduced rounding errors.
        Output operator()(Ellipse2<T> const& ellipse0, Ellipse2<T> const& ellipse1)
        {
            T const zero = C_<T>(0);
            T const one = C_<T>(1);

            // Get the parameters of ellipe0.
            Vector2<T> K0 = ellipse0.center;
            Matrix2x2<T> R0{};
            R0.SetCol(0, ellipse0.axis[0]);
            R0.SetCol(1, ellipse0.axis[1]);
            Matrix2x2<T> D0{
                { one / (ellipse0.extent[0] * ellipse0.extent[0]), zero },
                { zero, one / (ellipse0.extent[1] * ellipse0.extent[1]) }
            };

            // Get the parameters of ellipse1.
            Vector2<T> K1 = ellipse1.center;
            Matrix2x2<T> R1{};
            R1.SetCol(0, ellipse1.axis[0]);
            R1.SetCol(1, ellipse1.axis[1]);
            Matrix2x2<T> D1
            {
                { one / (ellipse1.extent[0] * ellipse1.extent[0]), zero },
                { zero, one / (ellipse1.extent[1] * ellipse1.extent[1]) }
            };

            // Compute K2 = D0^{1/2}*R0^T*(K1-K0). In the GTL code, the
            // quantity U = R0^T*(K1-K0) is a 2x1 vector which can be computed
            // in the GTL code by U = Transpose(R0)*(K1-K0). However, to avoid
            // the creation of the matrix object Transpose(R0), you can use
            // U^T = V^T*R0 which can be computed in the GTL code by
            // W = (K1-K0)*R0. The output W is mathematically a 1x2 vector,
            // but as a Vector<?> object, it is a 2-tuple. You can then
            // compute K2 = D0Half*W, where the 2-tuple W is now thought of
            // as a 2x1 vector. See Matrix.h, the operator function
            // Vector<?> operator*(Vector<?> const&, Matrix<?> const&) which
            // computes V^T*M for a Vector<?> V and a Matrix<?> M.
            Matrix2x2<T> D0NegHalf
            {
                { ellipse0.extent[0], zero },
                { zero, ellipse0.extent[1] }
            };

            Matrix2x2<T> D0Half
            {
                { one / ellipse0.extent[0], zero },
                { zero, one / ellipse0.extent[1] }
            };

            Vector2<T> K2 = D0Half * ((K1 - K0) * R0);

            // Compute M2.
            Matrix2x2<T> R1TR0D0NegHalf = MultiplyATB(R1, R0 * D0NegHalf);
            Matrix2x2<T> M2 = MultiplyATB(R1TR0D0NegHalf, D1) * R1TR0D0NegHalf;

            // Factor M2 = R*D*R^T.
            SymmetricEigensolver<T, 2> es{};
            es(M2(0, 0), M2(0, 1), M2(1, 1));
            std::array<T, 2> D = es.GetEigenvalues();
            std::array<std::array<T, 2>, 2> evec = es.GetEigenvectors();
            Matrix2x2<T> R{};
            R.SetCol(0, evec[0]);
            R.SetCol(1, evec[1]);

            // Compute K = R^T*K2.
            Vector2<T> K = K2 * R;

            // Transformed ellipse0 is Z^T*Z = 1 and transformed ellipse1 is
            // (Z-K)^T*D*(Z-K) = 0.

            // The minimum and maximum squared distances from the origin of
            // points on transformed ellipse1 are used to determine whether
            // the ellipses intersect, are separated or one contains the
            // other.
            T minSqrDistance = std::numeric_limits<T>::max();
            T maxSqrDistance = zero;

            if (IsZero(K))
            {
                // The special case of common centers must be handled
                // separately. It is not possible for the ellipses to be
                // separated.
                for (std::size_t i = 0; i < 2; ++i)
                {
                    T invD = one / D[i];
                    if (invD < minSqrDistance)
                    {
                        minSqrDistance = invD;
                    }
                    if (invD > maxSqrDistance)
                    {
                        maxSqrDistance = invD;
                    }
                }
                return Classify(minSqrDistance, maxSqrDistance, zero);
            }

            // The closest point P0 and farthest point P1 are solutions to
            // s0*D*(P0 - K) = P0 and s1*D1*(P1 - K) = P1 for some scalars s0
            // and s1 that are roots to the function
            //   f(s) = d0*k0^2/(d0*s-1)^2 + d1*k1^2/(d1*s-1)^2 - 1
            // where D = diagonal(d0,d1) and K = (k0,k1).
            T d0 = D[0], d1 = D[1];
            T c0 = K[0] * K[0], c1 = K[1] * K[1];

            // Sort the values so that d0 >= d1. This allows us to bound the
            // roots of f(s), of which there are at most 4.
            std::vector<std::pair<T, T>> param(2);
            if (d0 >= d1)
            {
                param[0] = std::make_pair(d0, c0);
                param[1] = std::make_pair(d1, c1);
            }
            else
            {
                param[0] = std::make_pair(d1, c1);
                param[1] = std::make_pair(d0, c0);
            }

            std::vector<std::pair<T, T>> valid{};
            valid.reserve(2);
            if (param[0].first > param[1].first)
            {
                // d0 > d1
                for (std::size_t i = 0; i < 2; ++i)
                {
                    if (param[i].second > zero)
                    {
                        valid.push_back(param[i]);
                    }
                }
            }
            else
            {
                // d0 = d1
                param[0].second += param[1].second;
                if (param[0].second > zero)
                {
                    valid.push_back(param[0]);
                }
            }

            std::size_t numValid = valid.size();
            std::size_t numRoots = 0;
            std::array<T, 4> roots{};
            if (numValid == 2)
            {
                GetRoots(valid[0].first, valid[1].first, valid[0].second,
                    valid[1].second, numRoots, roots.data());
            }
            else if (numValid == 1)
            {
                GetRoots(valid[0].first, valid[0].second, numRoots, roots.data());
            }
            // else: numValid cannot be zero because we already handled case
            // K = 0

            for (std::size_t i = 0; i < numRoots; ++i)
            {
                T s = roots[i];
                T p0 = d0 * K[0] * s / (d0 * s - (T)1);
                T p1 = d1 * K[1] * s / (d1 * s - (T)1);
                T sqrDistance = p0 * p0 + p1 * p1;
                if (sqrDistance < minSqrDistance)
                {
                    minSqrDistance = sqrDistance;
                }
                if (sqrDistance > maxSqrDistance)
                {
                    maxSqrDistance = sqrDistance;
                }
            }

            return Classify(minSqrDistance, maxSqrDistance, d0 * c0 + d1 * c1);
        }

    private:
        void GetRoots(T const& d0, T const& c0, std::size_t& numRoots, T* roots)
        {
            // f(s) = d0*c0/(d0*s-1)^2 - 1
            T const one = C_<T>(1);
            T temp = std::sqrt(d0 * c0);
            T inv = one / d0;
            numRoots = 2;
            roots[0] = (one - temp) * inv;
            roots[1] = (one + temp) * inv;
        }

        void GetRoots(T const& inD0, T const& inD1, T const& inC0, T const& inC1,
            std::size_t& numRoots, T* roots)
        {
            // f(s) = d0*c0/(d0*s-1)^2 + d1*c1/(d1*s-1)^2 - 1 with d0 > d1

            // Bisection using double precision is much faster than using
            // exact rational numbers.
            std::size_t const maxIterations = 1024;
            RootsBisection1<double> bisector(maxIterations);
            bool converged = false;
            double d0 = static_cast<double>(inD0);
            double d1 = static_cast<double>(inD1);
            double c0 = static_cast<double>(inC0);
            double c1 = static_cast<double>(inC1);
            double d0c0 = d0 * c0;
            double d1c1 = d1 * c1;
            double sum = d0c0 + d1c1;
            double sqrtsum = std::sqrt(sum);

            std::function<double(double const&)> F = [&, d0, d1, d0c0, d1c1](double const& s)
            {
                double invN0 = 1.0 / (d0 * s - 1.0);
                double invN1 = 1.0 / (d1 * s - 1.0);
                double term0 = d0c0 * invN0 * invN0;
                double term1 = d1c1 * invN1 * invN1;
                double f = term0 + term1 - 1.0;
                return f;
            };

            std::function<double(double const&)> DF = [&, d0, d1, d0c0, d1c1](double const& s)
            {
                double invN0 = 1.0 / (d0 * s - 1.0);
                double invN1 = 1.0 / (d1 * s - 1.0);
                double term0 = d0 * d0c0 * invN0 * invN0 * invN0;
                double term1 = d1 * d1c1 * invN1 * invN1 * invN1;
                double df = -2.0 * (term0 + term1);
                return df;
            };

            numRoots = 0;

            double invD0 = 1.0 / d0;
            double invD1 = 1.0 / d1;
            double smin{}, smax{}, fval{}, s = 0.0;

            // Compute root in (-infinity,1/d0).  Obtain a lower bound for the
            // root better than -std::numeric_limits<T>::max().
            smax = invD0;
            fval = sum - 1.0;
            if (fval > 0.0)
            {
                smin = (1.0 - sqrtsum) * invD1;  // < 0
                fval = F(smin);
                GTL_RUNTIME_ASSERT(fval <= 0.0, "Unexpected condition.");
            }
            else
            {
                smin = 0.0;
            }
            converged = bisector(F, smin, smax, -1.0, 1.0, s, fval);
            GTL_RUNTIME_ASSERT(converged, "Unexpected condition.");
            roots[numRoots++] = static_cast<T>(s);

            // Compute roots (if any) in (1/d0,1/d1). It is the case that
            //   F(1/d0) = +infinity, F'(1/d0) = -infinity
            //   F(1/d1) = +infinity, F'(1/d1) = +infinity
            //   F"(s) > 0 for all s in the domain of F
            // Compute the unique root r of F'(s) on (1/d0,1/d1). The
            // bisector needs only the signs at the endpoints, so we pass -1
            // and +1 instead of the infinite values. If F(r) < 0, F(s) has
            // two roots in the interval. If F(r) = 0, F(s) has only one root
            // in the interval.
            double rho = std::pow(d0 * d0c0 / (d1 * d1c1), 1.0 / 3.0);
            double smid = (1.0 + rho) / (d0 + rho * d1);
            double fmid = F(smid);
            if (fmid < 0.0)
            {
                // Pass in signs rather than infinities, because the bisector cares
                // only about the signs.
                converged = bisector(F, invD0, smid, 1.0, -1.0, s, fval);
                GTL_RUNTIME_ASSERT(converged, "Unexpected condition.");
                roots[numRoots++] = static_cast<T>(s);
                converged = bisector(F, smid, invD1, -1.0, 1.0, s, fval);
                GTL_RUNTIME_ASSERT(converged, "Unexpected condition.");
                roots[numRoots++] = static_cast<T>(s);
            }
            else if (fmid == 0.0)
            {
                roots[numRoots++] = static_cast<T>(smid);
            }

            // Compute root in (1/d1,+infinity). Obtain an upper bound for
            // the root better than std::numeric_limits<T>::max().
            smin = invD1;
            smax = (1.0 + sqrtsum) * invD1;  // > 1/d1
            fval = F(smax);
            GTL_RUNTIME_ASSERT(fval <= 0.0, "Unexpected condition.");
            converged = bisector(F, smin, smax, 1.0, -1.0, s, fval);
            GTL_RUNTIME_ASSERT(converged, "Unexpected condition.");
            roots[numRoots++] = static_cast<T>(s);
        }

        Output Classify(T const& minSqrDistance, T const& maxSqrDistance, T const& d0c0pd1c1)
        {
            T const one = C_<T>(1);

            if (maxSqrDistance < one)
            {
                return Output::ELLIPSE0_STRICTLY_CONTAINS_ELLIPSE1;
            }
            else if (maxSqrDistance > one)
            {
                if (minSqrDistance < one)
                {
                    return Output::ELLIPSES_OVERLAP;
                }
                else if (minSqrDistance > one)
                {
                    if (d0c0pd1c1 > one)
                    {
                        return Output::ELLIPSES_SEPARATED;
                    }
                    else
                    {
                        return Output::ELLIPSE1_STRICTLY_CONTAINS_ELLIPSE0;
                    }
                }
                else  // minSqrDistance = 1
                {
                    if (d0c0pd1c1 > one)
                    {
                        return Output::ELLIPSE0_OUTSIDE_ELLIPSE1_BUT_TANGENT;
                    }
                    else
                    {
                        return Output::ELLIPSE1_CONTAINS_ELLIPSE0_BUT_TANGENT;
                    }
                }
            }
            else  // maxSqrDistance = 1
            {
                if (minSqrDistance < one)
                {
                    return Output::ELLIPSE0_CONTAINS_ELLIPSE1_BUT_TANGENT;
                }
                else // minSqrDistance = 1
                {
                    return Output::ELLIPSES_EQUAL;
                }
            }
        }
    };

    template <typename T>
    class FIQuery<T, Ellipse2<T>, Ellipse2<T>>
    {
    public:
        // The queries find the intersections (if any) of the ellipses treated
        // as hollow objects. The implementations use the same concepts.
        struct Output
        {
            Output()
                :
                intersect(false),
                numPoints(0),
                points{},
                isTransverse{ false, false, false, false }
            {
            }

            // This value is true when the ellipses intersect in at least one
            // point.
            bool intersect;

            // If the ellipses are not the same, numPoints is 0 through 4 and
            // that number of elements of 'points' are valid. If the ellipses
            // are the same, numPoints is std::numeric_limits<std::size_t>::max()
            // and 'points' is invalid (set to zero-valued vectors).
            std::size_t numPoints;
            std::array<Vector2<T>, 4> points;
            std::array<bool, 4> isTransverse;
        };

        void GetStandardForm(Ellipse2<T> const& ellipse, Vector2<T>& C, Matrix2x2<T>& M)
        {
            Matrix2x2<T> UUTrn = OuterProduct(ellipse.axis[0], ellipse.axis[0]);
            Matrix2x2<T> VVTrn = OuterProduct(ellipse.axis[1], ellipse.axis[1]);
            T USqrLen = Trace(UUTrn);
            T aSqr = ellipse.extent[0] * ellipse.extent[0];
            T bSqr = ellipse.extent[1] * ellipse.extent[1];
            C = ellipse.center;
            M = (UUTrn / aSqr + VVTrn / bSqr) / USqrLen;
        }

        void ComputeAlignedBox(Ellipse2<T> const& ellipse, AlignedBox2<T>& box)
        {
            Vector2<T> C{};
            Matrix2x2<T> M{};
            GetStandardForm(ellipse, C, M);
            ComputeAlignedBox(C, M, box);
        }

        void ComputeAlignedBox(Vector2<T> const& C, Matrix2x2<T> const& M, AlignedBox2<T>& box)
        {
            T determinant = M(0, 0) * M(1, 1) - M(0, 1) * M(0, 1);
            std::array<T, 2> distance
            {
                std::sqrt(M(1, 1) / determinant),
                std::sqrt(M(0, 0) / determinant)
            };

            for (std::size_t i = 0; i < 2; ++i)
            {
                box.min[i] = C[i] - distance[i];
                box.max[i] = C[i] + distance[i];
            }
        }

        Output operator()(
            Vector2<T> const& C0, Matrix2x2<T> const& M0,
            Vector2<T> const& C1, Matrix2x2<T> const& M1,
            bool useEarlyExitNoIntersectionTest = true)
        {
            Output output{};

            // Test whether the ellipses are the same. If so, report that
            // there are infinitely many points of intersection.
            if (C0 == C1 && M0 == M1)
            {
                output.numPoints = std::numeric_limits<std::size_t>::max();
                return output;
            }

            if (useEarlyExitNoIntersectionTest)
            {
                // Test whether the axis-aligned bounding boxes are disjoint.
                // If so, the ellipses do not intersect.
                AlignedBox2<T> box0{}, box1{};
                ComputeAlignedBox(C0, M0, box0);
                ComputeAlignedBox(C1, M1, box1);
                for (std::size_t i = 0; i < 2; i++)
                {
                    if (box0.max[i] < box1.min[i] || box0.min[i] > box1.max[i])
                    {
                        // The member output.intersect is set to 'false' by the
                        // constructor.
                        return output;
                    }
                }
            }

            T const zero = static_cast<T>(0);
            T const one = static_cast<T>(1);
            T const two = static_cast<T>(2);

            T ell = M0(0, 1) / M0(0, 0);
            T d0 = M0(0, 0);
            T d1 = RobustDOP(M0(0, 0), M0(1, 1), M0(0, 1), M0(0, 1)) / M0(0, 0);
            T k0 = C1[0] - C0[0];
            T k1 = C1[1] - C0[1];
            T term0 = RobustSOP(k0, M1(0, 0), k1, M1(0, 1));
            T term1 = RobustSOP(k0, M1(0, 1), k1, M1(1, 1));
            T g0 = RobustSOP(k0, term0, k1, term1) - one;
            T g1 = -two * term0;
            T g2 = two * FMA(term0, ell, -term1);
            T g3 = M1(0, 0);
            T g4 = -two * FMA(M1(0, 0), ell, -M1(0, 1));
            T g5 = FMA(-ell, RobustDOP(two, M1(0, 1), ell, M1(0, 0)), M1(1, 1));
            T e0 = FMA(d1, g0, g5);
            T e1 = d1 * g1;
            T e2 = d1 * g2;
            T e3 = RobustDOP(d1, g3, d0, g5);
            T e4 = d1 * g4;

            if (e4 != zero)
            {
                CaseE4NotZero(C0, ell, d0, d1, e0, e1, e2, e3, e4, output);
            }
            else
            {
                if (e2 != zero)
                {
                    if (e3 != zero)
                    {
                        CaseE4ZeroE2NotZeroE3NotZero(C0, ell, d0, d1, e0, e1, e2, e3, output);
                    }
                    else
                    {
                        CaseE4ZeroE2NotZeroE3Zero(C0, ell, d0, d1, e0, e1, e2, output);
                    }
                }
                else
                {
                    if (e3 != zero)
                    {
                        CaseE4ZeroE2ZeroE3NotZero(C0, ell, d0, d1, e0, e1, e3, output);
                    }
                    else if (e1 != zero)
                    {
                        CaseE4ZeroE2ZeroE3Zero(C0, ell, d0, d1, e0, e1, output);
                    }
                    // else: The ellipses are axis-aligned and have the same
                    // center. The extent vectors are parallel but not equal.
                    // One ellipse is strictly inside the other, so there is
                    // no intersection.
                }
            }

            return output;
        }

        Output operator()(
            Ellipse2<T> const& ellipse0,
            Ellipse2<T> const& ellipse1,
            bool useEarlyExitNoIntersectionTest = true)
        {
            Vector2<T> C0{}, C1{};
            Matrix2x2<T> M0{}, M1{};
            GetStandardForm(ellipse0, C0, M0);
            GetStandardForm(ellipse1, C1, M1);
            return operator()(C0, M0, C1, M1, useEarlyExitNoIntersectionTest);
        }

    private:
        void CaseE4ZeroE2ZeroE3NotZero(
            Vector2<T> const& C0, T const& ell, T const& d0, T const& d1,
            T const& e0, T const& e1, T const& e3,
            Output& output)
        {
            T const zero = static_cast<T>(0);
            T const one = static_cast<T>(1);

            std::array<PolynomialRoot<T>, 2> roots{};
            std::size_t numRoots = RootsQuadratic<T>::Solve(false, e0, e1, e3, roots.data());
            for (std::size_t i = 0; i < numRoots; ++i)
            {
                T y0 = roots[i].x;
                T lambda = FMA(-d0, y0 * y0, one);
                if (lambda < zero)
                {
                    continue;
                }

                if (lambda > zero)
                {
                    T y1 = -std::sqrt(lambda / d1);
                    output.points[output.numPoints] = { FMA(-ell, y1, y0) + C0[0], y1 + C0[1] };
                    output.isTransverse[output.numPoints] = (roots[i].m == 1);
                    ++output.numPoints;
                    y1 = -y1;
                    output.points[output.numPoints] = { FMA(-ell, y1, y0) + C0[0], y1 + C0[1] };
                    output.isTransverse[output.numPoints] = (roots[i].m == 1);
                    ++output.numPoints;
                }
                else
                {
                    output.points[output.numPoints] = { y0 + C0[0], C0[1] };
                    output.isTransverse[output.numPoints] = false;
                    ++output.numPoints;
                }

                output.intersect = true;
            }
        }

        void CaseE4ZeroE2ZeroE3Zero(
            Vector2<T> const& C0, T const& ell, T const& d0, T const& d1,
            T const& e0, T const& e1,
            Output& output)
        {
            T const zero = static_cast<T>(0);
            T const one = static_cast<T>(1);

            T y0 = -e0 / e1;
            T lambda = FMA(-d0, y0 * y0, one);
            if (lambda < zero)
            {
                return;
            }

            if (lambda > zero)
            {
                T y1 = -std::sqrt(lambda / d1);
                output.points[output.numPoints] = { FMA(-ell, y1, y0) + C0[0], y1 + C0[1] };
                output.isTransverse[output.numPoints] = true;
                ++output.numPoints;
                y1 = -y1;
                output.points[output.numPoints] = { FMA(-ell, y1, y0) + C0[0], y1 + C0[1] };
                output.isTransverse[output.numPoints] = true;
                ++output.numPoints;
            }
            else
            {
                output.points[output.numPoints] = { y0 + C0[0], C0[1] };
                output.isTransverse[output.numPoints] = false;
                ++output.numPoints;
            }

            output.intersect = true;
        }

        void CaseE4ZeroE2NotZeroE3Zero(
            Vector2<T> const& C0, T const& ell, T const& d0, T const& d1,
            T const& e0, T const& e1, T const& e2,
            Output& output)
        {
            T const zero = static_cast<T>(0);
            T const one = static_cast<T>(1);

            Polynomial1<T> poly0{ -one, zero, d0 };
            Polynomial1<T> poly1{ e0, e1 };
            Polynomial1<T> H = e2 * e2 * poly0 + d1 * poly1 * poly1;
            std::array<PolynomialRoot<T>, 2> roots{};
            std::size_t numRoots = RootsQuadratic<T>::Solve(false, H[0], H[1], H[2], roots.data());
            for (std::size_t i = 0; i < numRoots; ++i)
            {
                T y0 = roots[i].x;
                T lambda = FMA(-d0, y0 * y0, one);
                if (lambda < zero)
                {
                    continue;
                }

                if (lambda > zero)
                {
                    // Choose the y1-root with smallest
                    // |(e0 + e1 * y0) + (e2) * y1|.
                    T y1cand0 = -std::sqrt(lambda / d1);
                    T test0 = std::fabs(e0 + RobustSOP(e1, y0, e2, y1cand0));
                    T y1cand1 = -y1cand0;
                    T test1 = std::fabs(e0 + RobustSOP(e1, y0, e2, y1cand1));
                    T y1 = (test0 <= test1 ? y1cand0 : y1cand1);
                    output.points[output.numPoints] = { FMA(-ell, y1, y0) + C0[0], y1 + C0[1] };
                }
                else
                {
                    output.points[output.numPoints] = { y0 + C0[0], C0[1] };
                }

                output.isTransverse[output.numPoints] = (roots[i].m == 1);
                ++output.numPoints;

                output.intersect = true;
            }
        }

        void CaseE4ZeroE2NotZeroE3NotZero(
            Vector2<T> const& C0, T const& ell, T const& d0, T const& d1,
            T const& e0, T const& e1, T const& e2, T const& e3,
            Output& output)
        {
            T const zero = static_cast<T>(0);
            T const one = static_cast<T>(1);

            Polynomial1<T> poly0{ -one, zero, d0 };
            Polynomial1<T> poly1{ e0, e1, e3 };
            Polynomial1<T> H = e2 * e2 * poly0 + d1 * poly1 * poly1;
            std::array<PolynomialRoot<T>, 4> roots{};
            std::size_t numRoots = RootsQuartic<T>::Solve(false, H[0], H[1], H[2], H[3], H[4], roots.data());
            for (std::size_t i = 0; i < numRoots; ++i)
            {
                T y0 = roots[i].x;
                T lambda = FMA(-d0, y0 * y0, one);
                if (lambda < zero)
                {
                    continue;
                }

                if (lambda > zero)
                {
                    // Choose the y1-root with smallest
                    // |(e0 + e1 * y0 + e3 * y0^2) + (e2) * y1|.
                    T term0 = FMA(e3, y0, e1);
                    T term1 = FMA(term0, y0, e0);
                    T y1cand0 = -std::sqrt(lambda / d1);
                    T test0 = std::fabs(FMA(e2, y1cand0, term1));
                    T y1cand1 = -y1cand0;
                    T test1 = std::fabs(FMA(e2, y1cand1, term1));
                    T y1 = (test0 < test1 ? y1cand0 : y1cand1);
                    output.points[output.numPoints] = { FMA(-ell, y1, y0) + C0[0], y1 + C0[1] };
                }
                else
                {
                    output.points[output.numPoints] = { y0 + C0[0], C0[1] };
                }

                output.isTransverse[output.numPoints] = (roots[i].m == 1);
                ++output.numPoints;

                output.intersect = true;
            }
        }

        void CaseE4NotZero(
            Vector2<T> const& C0, T const& ell, T const& d0, T const& d1,
            T const& e0, T const& e1, T const& e2, T const& e3, T const& e4,
            Output& output)
        {
            T const zero = static_cast<T>(0);
            T const one = static_cast<T>(1);

            Polynomial1<T> poly0{ -one, zero, d0 };
            Polynomial1<T> poly1{ e0, e1, e3 };
            Polynomial1<T> poly2{ e2, e4 };
            Polynomial1<T> H = poly2 * poly2 * poly0 + d1 * poly1 * poly1;
            std::array<PolynomialRoot<T>, 4> roots{};
            std::size_t numRoots = RootsQuartic<T>::Solve(false, H[0], H[1], H[2], H[3], H[4], roots.data());
            for (std::size_t i = 0; i < numRoots; ++i)
            {
                T y0 = roots[i].x;
                T lambda = FMA(-d0, y0 * y0, one);
                if (lambda < zero)
                {
                    continue;
                }

                T divisor = e2 + e4 * y0;
                if (divisor != zero)
                {
                    if (lambda > zero)
                    {
                        // Choose the y1-root with smallest
                        // |(e0 + e1 * y0 + e3 * y0^2) + (e2 + e4 * y0) * y1|.
                        T y1cand0 = -std::sqrt(lambda / d1);
                        T term0 = FMA(e3, y0, e1);
                        T term1 = FMA(term0, y0, e0);
                        T test0 = std::fabs(FMA(divisor, y1cand0, term1));
                        T y1cand1 = -y1cand0;
                        T test1 = std::fabs(FMA(divisor, y1cand1, term1));
                        T y1 = (test0 < test1 ? y1cand0 : y1cand1);
                        output.points[output.numPoints] = { FMA(-ell, y1, y0) + C0[0], y1 + C0[1] };
                    }
                    else
                    {
                        output.points[output.numPoints] = { y0 + C0[0], C0[1] };
                    }

                    output.isTransverse[output.numPoints] = (roots[i].m == 1);
                    ++output.numPoints;

                    output.intersect = true;
                }
                else
                {
                    if (lambda > zero)
                    {
                        T y1 = -std::sqrt(lambda / d1);
                        output.points[output.numPoints] = { FMA(-ell, y1, y0) + C0[0], y1 + C0[1] };
                        y1 = -y1;
                        output.points[output.numPoints] = { FMA(-ell, y1, y0) + C0[0], y1 + C0[1] };
                    }
                    else
                    {
                        output.points[output.numPoints] = { y0 + C0[0], C0[1] };
                    }

                    output.isTransverse[output.numPoints] = (lambda > zero);
                    ++output.numPoints;

                    output.intersect = true;
                }
            }
        }
    };

    // Template aliases for convenience.
    template <typename T>
    using TIEllipses2 = TIQuery<T, Ellipse2<T>, Ellipse2<T>>;

    template <typename T>
    using FIEllipses2 = FIQuery<T, Ellipse2<T>, Ellipse2<T>>;
}
