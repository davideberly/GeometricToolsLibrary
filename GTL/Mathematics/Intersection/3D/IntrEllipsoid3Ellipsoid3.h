// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.18

#pragma once

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Primitives/3D/Ellipsoid3.h>
#include <GTL/Mathematics/Algebra/Matrix.h>
#include <GTL/Mathematics/RootFinders/RootsBisection1.h>
#include <GTL/Mathematics/RootFinders/RootsPolynomial.h>
#include <GTL/Mathematics/MatrixAnalysis/SymmetricEigensolver.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <functional>
#include <limits>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Ellipsoid3<T>, Ellipsoid3<T>>
    {
    public:
        enum class Classification
        {
            ELLIPSOIDS_SEPARATED,
            ELLIPSOIDS_INTERSECTING,
            ELLIPSOID0_CONTAINS_ELLIPSOID1,
            ELLIPSOID1_CONTAINS_ELLIPSOID0,
            INVALID
        };

        struct Output
        {
            Output()
                :
                intersect(false),
                classification(Classification::INVALID)
            {
            }

            // As solids, the ellipsoids intersect as long as they are not
            // separated.
            bool intersect;

            // This is C_<T>(1) of the four enumerations listed above.
            Classification classification;
        };

        // The precision must be set to 0 for floating-point T. It must be
        // positive for rational T.
        Output operator()(Ellipsoid3<T> const& ellipsoid0, Ellipsoid3<T> const& ellipsoid1,
            std::size_t precision)
        {
            Output output{};

            // Get the parameters of ellipsoid0.
            Vector3<T> K0 = ellipsoid0.center;
            Matrix3x3<T> R0{};
            R0.SetCol(0, ellipsoid0.axis[0]);
            R0.SetCol(1, ellipsoid0.axis[1]);
            R0.SetCol(2, ellipsoid0.axis[2]);
            Matrix3x3<T> D0{
                { C_<T>(1) / (ellipsoid0.extent[0] * ellipsoid0.extent[0]), C_<T>(0), C_<T>(0) },
                { C_<T>(0), C_<T>(1) / (ellipsoid0.extent[1] * ellipsoid0.extent[1]), C_<T>(0) },
                { C_<T>(0), C_<T>(0), C_<T>(1) / (ellipsoid0.extent[2] * ellipsoid0.extent[2]) }
            };

            // Get the parameters of ellipsoid1.
            Vector3<T> K1 = ellipsoid1.center;
            Matrix3x3<T> R1{};
            R1.SetCol(0, ellipsoid1.axis[0]);
            R1.SetCol(1, ellipsoid1.axis[1]);
            R1.SetCol(2, ellipsoid1.axis[2]);
            Matrix3x3<T> D1{
                { C_<T>(1) / (ellipsoid1.extent[0] * ellipsoid1.extent[0]), C_<T>(0), C_<T>(0) },
                { C_<T>(0), C_<T>(1) / (ellipsoid1.extent[1] * ellipsoid1.extent[1]), C_<T>(0) },
                { C_<T>(0), C_<T>(0), C_<T>(1) / (ellipsoid1.extent[2] * ellipsoid1.extent[2]) }
            };

            // Compute K2.
            Matrix3x3<T> D0NegHalf{
                { ellipsoid0.extent[0], C_<T>(0), C_<T>(0) },
                { C_<T>(0), ellipsoid0.extent[1], C_<T>(0) },
                { C_<T>(0), C_<T>(0), ellipsoid0.extent[2] }
            };
            Matrix3x3<T> D0Half{
                { C_<T>(1) / ellipsoid0.extent[0], C_<T>(0), C_<T>(0) },
                { C_<T>(0), C_<T>(1) / ellipsoid0.extent[1], C_<T>(0) },
                { C_<T>(0), C_<T>(0), C_<T>(1) / ellipsoid0.extent[2] }
            };
            Vector3<T> K2 = D0Half * ((K1 - K0) * R0);

            // Compute M2.
            Matrix3x3<T> R1TR0D0NegHalf = MultiplyATB(R1, R0 * D0NegHalf);
            Matrix3x3<T> M2 = MultiplyATB(R1TR0D0NegHalf, D1) * R1TR0D0NegHalf;

            // Factor M2 = R*D*R^T.
            SymmetricEigensolver<T, 3> es{};
            es(M2(0, 0), M2(0, 1), M2(0, 2), M2(1, 1), M2(1, 2), M2(2, 2), false, false);
            std::array<T, 3> const& D = es.GetEigenvalues();
            Matrix3x3<T> R{};
            for (std::size_t i = 0; i < 3; ++i)
            {
                R.SetCol(i, es.GetEigenvector(i));
            }

            // Compute K = R^T*K2.
            Vector3<T> K = K2 * R;

            // Transformed ellipsoid0 is Z^T*Z = 1 and transformed ellipsoid1
            // is (Z-K)^T*D*(Z-K) = 0.

            // The minimum and maximum squared distances from the origin of
            // points on transformed ellipsoid1 are used to determine whether
            // the ellipsoids intersect, are separated, or one contains the
            // other.
            T minSqrDistance = std::numeric_limits<T>::max();
            T maxSqrDistance = C_<T>(0);

            if (K == Vector3<T>::Zero())
            {
                // The special case of common centers must be handled
                // separately. It is not possible for the ellipsoids to be
                // separated.
                for (std::size_t i = 0; i < 3; ++i)
                {
                    T invD = C_<T>(1) / D[i];
                    if (invD < minSqrDistance)
                    {
                        minSqrDistance = invD;
                    }
                    if (invD > maxSqrDistance)
                    {
                        maxSqrDistance = invD;
                    }
                }

                if (maxSqrDistance < C_<T>(1))
                {
                    output.classification = Classification::ELLIPSOID0_CONTAINS_ELLIPSOID1;
                }
                else if (minSqrDistance > C_<T>(1))
                {
                    output.classification = Classification::ELLIPSOID1_CONTAINS_ELLIPSOID0;
                }
                else
                {
                    output.classification = Classification::ELLIPSOIDS_INTERSECTING;
                }
                output.intersect = true;
                return output;
            }

            // The closest point P0 and farthest point P1 are solutions to
            // s0*D*(P0 - K) = P0 and s1*D*(P1 - K) = P1 for some scalars s0
            // and s1 that are roots to the function
            //   f(s) = d0*k0^2/(d0*s-1)^2 + d1*k1^2/(d1*s-1)^2
            //          + d2*k2^2/(d2*s-1)^2 - 1
            // where D = diagonal(d0,d1,d2) and K = (k0,k1,k2).
            T d0 = D[0], d1 = D[1], d2 = D[2];
            T c0 = K[0] * K[0], c1 = K[1] * K[1], c2 = K[2] * K[2];

            // Sort the values so that d0 >= d1 >= d2.  This allows us to
            // bound the roots of f(s), of which there are at most 6.
            std::vector<std::pair<T, T>> param(3);
            param[0] = std::make_pair(d0, c0);
            param[1] = std::make_pair(d1, c1);
            param[2] = std::make_pair(d2, c2);
            std::sort(param.begin(), param.end(), std::greater<std::pair<T, T>>());

            std::vector<std::pair<T, T>> valid{};
            valid.reserve(3);
            if (param[0].first > param[1].first)
            {
                if (param[1].first > param[2].first)
                {
                    // d0 > d1 > d2
                    for (std::size_t i = 0; i < 3; ++i)
                    {
                        if (param[i].second > C_<T>(0))
                        {
                            valid.push_back(param[i]);
                        }
                    }
                }
                else
                {
                    // d0 > d1 = d2
                    if (param[0].second > C_<T>(0))
                    {
                        valid.push_back(param[0]);
                    }
                    param[1].second += param[0].second;
                    if (param[1].second > C_<T>(0))
                    {
                        valid.push_back(param[1]);
                    }
                }
            }
            else
            {
                if (param[1].first > param[2].first)
                {
                    // d0 = d1 > d2
                    param[0].second += param[1].second;
                    if (param[0].second > C_<T>(0))
                    {
                        valid.push_back(param[0]);
                    }
                    if (param[2].second > C_<T>(0))
                    {
                        valid.push_back(param[2]);
                    }
                }
                else
                {
                    // d0 = d1 = d2
                    param[0].second += param[1].second + param[2].second;
                    if (param[0].second > C_<T>(0))
                    {
                        valid.push_back(param[0]);
                    }
                }
            }

            std::size_t numValid = valid.size();
            std::size_t numRoots = 0;
            std::array<T, 6> roots{};
            if (numValid == 3)
            {
                GetRoots(precision, valid[0].first, valid[1].first, valid[2].first,
                    valid[0].second, valid[1].second, valid[2].second, numRoots, roots.data());
            }
            else if (numValid == 2)
            {
                GetRoots(precision, valid[0].first, valid[1].first, valid[0].second,
                    valid[1].second, numRoots, roots.data());
            }
            else if (numValid == 1)
            {
                GetRoots(valid[0].first, valid[0].second, numRoots, roots.data());
            }
            else
            {
                // numValid cannot be C_<T>(0) because we already handled case K = 0
                GTL_RUNTIME_ERROR(
                    "Unexpected condition.");
            }

            for (std::size_t i = 0; i < numRoots; ++i)
            {
                T s = roots[i];
                T p0 = d0 * K[0] * s / (d0 * s - C_<T>(1));
                T p1 = d1 * K[1] * s / (d1 * s - C_<T>(1));
                T p2 = d2 * K[2] * s / (d2 * s - C_<T>(1));
                T sqrDistance = p0 * p0 + p1 * p1 + p2 * p2;
                if (sqrDistance < minSqrDistance)
                {
                    minSqrDistance = sqrDistance;
                }
                if (sqrDistance > maxSqrDistance)
                {
                    maxSqrDistance = sqrDistance;
                }
            }

            if (maxSqrDistance < C_<T>(1))
            {
                output.intersect = true;
                output.classification = Classification::ELLIPSOID0_CONTAINS_ELLIPSOID1;
            }
            else if (minSqrDistance > C_<T>(1))
            {
                if (d0 * c0 + d1 * c1 + d2 * c2 > C_<T>(1))
                {
                    output.intersect = false;
                    output.classification = Classification::ELLIPSOIDS_SEPARATED;
                }
                else
                {
                    output.intersect = true;
                    output.classification = Classification::ELLIPSOID1_CONTAINS_ELLIPSOID0;
                }
            }
            else
            {
                output.intersect = true;
                output.classification = Classification::ELLIPSOIDS_INTERSECTING;
            }

            return output;
        }

    private:
        void GetRoots(T const& d0, T const& c0, std::size_t& numRoots, T* roots)
        {
            // f(s) = d0*c0/(d0*s-1)^2 - 1
            T temp = std::sqrt(d0 * c0);
            T inv = C_<T>(1) / d0;
            numRoots = 2;
            roots[0] = (C_<T>(1) - temp) * inv;
            roots[1] = (C_<T>(1) + temp) * inv;
        }

        void GetRoots(std::size_t precision,
            T const& d0, T const& d1, T const& c0, T const& c1,
            std::size_t& numRoots, T* roots)
        {
            // f(s) = d0*c0/(d0*s-1)^2 + d1*c1/(d1*s-1)^2 - 1
            // with d0 > d1

            std::size_t constexpr maxIterations = 1024;
            RootsBisection1<T> bisector(maxIterations, precision);
            bool converged{};
            numRoots = 0;

            T d0c0 = d0 * c0;
            T d1c1 = d1 * c1;

            std::function<T(T)> F = [d0, d1, d0c0, d1c1](T s)
            {
                T invN0 = C_<T>(1) / (d0 * s - C_<T>(1));
                T invN1 = C_<T>(1) / (d1 * s - C_<T>(1));
                T term0 = d0c0 * invN0 * invN0;
                T term1 = d1c1 * invN1 * invN1;
                T f = term0 + term1 - C_<T>(1);
                return f;
            };

            std::function<T(T)> DF = [d0, d1, d0c0, d1c1](T s)
            {
                T invN0 = C_<T>(1) / (d0 * s - C_<T>(1));
                T invN1 = C_<T>(1) / (d1 * s - C_<T>(1));
                T term0 = d0 * d0c0 * invN0 * invN0 * invN0;
                T term1 = d1 * d1c1 * invN1 * invN1 * invN1;
                T df = -C_<T>(2) * (term0 + term1);
                return df;
            };

            // TODO: What role does epsilon play?
            T const epsilon = C_<T>(1, 1000);
            T multiplier0 = std::sqrt(C_<T>(2) / (C_<T>(1) - epsilon));
            T multiplier1 = std::sqrt(C_<T>(1) / (C_<T>(1) + epsilon));
            T sqrtd0c0 = std::sqrt(d0c0);
            T sqrtd1c1 = std::sqrt(d1c1);
            T invD0 = C_<T>(1) / d0;
            T invD1 = C_<T>(1) / d1;
            T temp0{}, temp1{}, smin{}, smax{}, fmin{}, fmax{};
            T s{}, fAtS{};

            // Compute root in (-infinity,1/d0).
            temp0 = (C_<T>(1) - multiplier0 * sqrtd0c0) * invD0;
            temp1 = (C_<T>(1) - multiplier0 * sqrtd1c1) * invD1;

            smin = std::min(temp0, temp1);
            fmin = F(smin);
            GTL_RUNTIME_ASSERT(
                fmin < C_<T>(0),
                "Unexpected condition.");
            smax = (C_<T>(1) - multiplier1 * sqrtd0c0) * invD0;
            fmax = F(smax);
            GTL_RUNTIME_ASSERT(
                fmax > C_<T>(0),
                "Unexpected condition.");
            converged = bisector(F, smin, smax, fmin, fmax, s, fAtS);
            GTL_RUNTIME_ASSERT(
                converged && bisector.GetNumIterations() > 0,
                "Unexpected condition.");

            roots[numRoots++] = s;

            // Compute roots (if any) in (1/d0,1/d1).  It is the case that
            //   F(1/d0) = +infinity, F'(1/d0) = -infinity
            //   F(1/d1) = +infinity, F'(1/d1) = +infinity
            //   F"(s) > 0 for all s in the domain of F
            // Compute the unique root r of F'(s) on (1/d0,1/d1).  The
            // bisector needs only the signs at the endpoints, so we pass -1
            // and +1 instead of the infinite values.  If F(r) < 0, F(s) has
            // two roots in the interval.  If F(r) = 0, F(s) has only C_<T>(1) root
            // in the interval.
            T smid = C_<T>(0);
            converged = bisector(DF, invD0, invD1, -C_<T>(1), C_<T>(1), smid, fAtS);
            GTL_RUNTIME_ASSERT(
                converged && bisector.GetNumIterations() > 0,
                "Unexpected condition.");
            if (F(smid) < C_<T>(0))
            {
                // Pass in signs rather than infinities, because the bisector
                // cares only about the signs.
                converged = bisector(F, invD0, smid, C_<T>(1), -C_<T>(1), s, fAtS);
                GTL_RUNTIME_ASSERT(
                    converged && bisector.GetNumIterations() > 0,
                    "Unexpected condition.");
                roots[numRoots++] = s;
                converged = bisector(F, smid, invD1, -C_<T>(1), C_<T>(1), s, fAtS);
                GTL_RUNTIME_ASSERT(
                    converged && bisector.GetNumIterations() > 0,
                    "Unexpected condition.");
                roots[numRoots++] = s;
            }

            // Compute root in (1/d1,+infinity).
            temp0 = (C_<T>(1) + multiplier0 * sqrtd0c0) * invD0;
            temp1 = (C_<T>(1) + multiplier0 * sqrtd1c1) * invD1;
            smax = std::max(temp0, temp1);
            fmax = F(smax);
            GTL_RUNTIME_ASSERT(
                fmax < C_<T>(0),
                "Unexpected condition.");
            smin = (C_<T>(1) + multiplier1 * sqrtd1c1) * invD1;
            fmin = F(smin);
            GTL_RUNTIME_ASSERT(
                fmin > C_<T>(0),
                "Unexpected condition.");
            converged = bisector(F, smin, smax, fmin, fmax, s, fAtS);
            GTL_RUNTIME_ASSERT(
                converged && bisector.GetNumIterations() > 0,
                "Unexpected condition.");
            roots[numRoots++] = s;
        }

        void GetRoots(std::size_t precision,
            T const& d0, T const& d1, T const& d2, T const& c0, T const& c1, T const& c2,
            std::size_t& numRoots, T* roots)
        {
            // f(s) = d0*c0/(d0*s-1)^2 + d1*c1/(d1*s-1)^2
            // + d2*c2/(d2*s-1)^2 - 1 with d0 > d1 > d2

            std::size_t constexpr maxIterations = 1024;
            RootsBisection1<T> bisector(maxIterations, precision);
            bool converged{};
            numRoots = 0;

            T d0c0 = d0 * c0;
            T d1c1 = d1 * c1;
            T d2c2 = d2 * c2;

            std::function<T(T)> F = [d0, d1, d2, d0c0, d1c1, d2c2](T s)
            {
                T invN0 = C_<T>(1) / (d0 * s - C_<T>(1));
                T invN1 = C_<T>(1) / (d1 * s - C_<T>(1));
                T invN2 = C_<T>(1) / (d2 * s - C_<T>(1));
                T term0 = d0c0 * invN0 * invN0;
                T term1 = d1c1 * invN1 * invN1;
                T term2 = d2c2 * invN2 * invN2;
                T f = term0 + term1 + term2 - C_<T>(1);
                return f;
            };

            std::function<T(T)> DF = [d0, d1, d2, d0c0, d1c1, d2c2](T s)
            {
                T invN0 = C_<T>(1) / (d0 * s - C_<T>(1));
                T invN1 = C_<T>(1) / (d1 * s - C_<T>(1));
                T invN2 = C_<T>(1) / (d2 * s - C_<T>(1));
                T term0 = d0 * d0c0 * invN0 * invN0 * invN0;
                T term1 = d1 * d1c1 * invN1 * invN1 * invN1;
                T term2 = d2 * d2c2 * invN2 * invN2 * invN2;
                T df = -C_<T>(2) * (term0 + term1 + term2);
                return df;
            };

            // TODO: What role does epsilon play?
            T epsilon = C_<T>(1, 1000);
            T multiplier0 = std::sqrt(C_<T>(3) / (C_<T>(1) - epsilon));
            T multiplier1 = std::sqrt(C_<T>(1) / (C_<T>(1) + epsilon));
            T sqrtd0c0 = std::sqrt(d0c0);
            T sqrtd1c1 = std::sqrt(d1c1);
            T sqrtd2c2 = std::sqrt(d2c2);
            T invD0 = C_<T>(1) / d0;
            T invD1 = C_<T>(1) / d1;
            T invD2 = C_<T>(1) / d2;
            T temp0{}, temp1{}, temp2{}, smin{}, smax{}, fmin{}, fmax{};
            T s{}, fAtS{};

            // Compute root in (-infinity,1/d0).
            temp0 = (C_<T>(1) - multiplier0 * sqrtd0c0) * invD0;
            temp1 = (C_<T>(1) - multiplier0 * sqrtd1c1) * invD1;
            temp2 = (C_<T>(1) - multiplier0 * sqrtd2c2) * invD2;
            smin = std::min(std::min(temp0, temp1), temp2);
            fmin = F(smin);
            GTL_RUNTIME_ASSERT(
                fmin < C_<T>(0),
                "Unexpected condition.");
            smax = (C_<T>(1) - multiplier1 * sqrtd0c0) * invD0;
            fmax = F(smax);
            GTL_RUNTIME_ASSERT(
                fmax > C_<T>(0),
                "Unexpected condition.");
            converged = bisector(F, smin, smax, fmin, fmax, s, fAtS);
            GTL_RUNTIME_ASSERT(
                converged && bisector.GetNumIterations() > 0,
                "Unexpected condition.");
            roots[numRoots++] = s;

            // Compute roots (if any) in (1/d0,1/d1).  It is the case that
            //   F(1/d0) = +infinity, F'(1/d0) = -infinity
            //   F(1/d1) = +infinity, F'(1/d1) = +infinity
            //   F"(s) > 0 for all s in the domain of F
            // Compute the unique root r of F'(s) on (1/d0,1/d1).  The
            // bisector needs only the signs at the endpoints, so we pass -1
            // and +1 instead of the infinite values.  If F(r) < 0, F(s) has
            // two roots in the interval.  If F(r) = 0, F(s) has only C_<T>(1) root
            // in the interval.
            T smid = C_<T>(0);
            converged = bisector(DF, invD0, invD1, -C_<T>(1), C_<T>(1), smid, fAtS);
            GTL_RUNTIME_ASSERT(
                converged && bisector.GetNumIterations() > 0,
                "Unexpected condition.");
            if (F(smid) < C_<T>(0))
            {
                // Pass in signs rather than infinities, because the bisector cares
                // only about the signs.
                converged = bisector(F, invD0, smid, C_<T>(1), -C_<T>(1), s, fAtS);
                GTL_RUNTIME_ASSERT(
                    converged && bisector.GetNumIterations() > 0,
                    "Unexpected condition.");
                roots[numRoots++] = s;
                converged = bisector(F, smid, invD1, -C_<T>(1), C_<T>(1), s, fAtS);
                GTL_RUNTIME_ASSERT(
                    converged && bisector.GetNumIterations() > 0,
                    "Unexpected condition.");
                roots[numRoots++] = s;
            }

            // Compute roots (if any) in (1/d1,1/d2).  It is the case that
            //   F(1/d1) = +infinity, F'(1/d1) = -infinity
            //   F(1/d2) = +infinity, F'(1/d2) = +infinity
            //   F"(s) > 0 for all s in the domain of F
            // Compute the unique root r of F'(s) on (1/d1,1/d2).  The
            // bisector needs only the signs at the endpoints, so we pass -1
            // and +1 instead of the infinite values.  If F(r) < 0, F(s) has
            // two roots in the interval.  If F(r) = 0, F(s) has only C_<T>(1) root
            // in the interval.
            converged = bisector(DF, invD1, invD2, -C_<T>(1), C_<T>(1), smid, fAtS);
            GTL_RUNTIME_ASSERT(
                converged && bisector.GetNumIterations() > 0,
                "Unexpected condition.");
            if (F(smid) < C_<T>(0))
            {
                // Pass in signs rather than infinities, because the bisector
                // cares only about the signs.
                converged = bisector(F, invD1, smid, C_<T>(1), -C_<T>(1), s, fAtS);
                GTL_RUNTIME_ASSERT(
                    converged && bisector.GetNumIterations() > 0,
                    "Unexpected condition.");
                roots[numRoots++] = s;
                converged = bisector(F, smid, invD2, -C_<T>(1), C_<T>(1), s, fAtS);
                GTL_RUNTIME_ASSERT(
                    converged&& bisector.GetNumIterations() > 0,
                    "Unexpected condition.");
                roots[numRoots++] = s;
            }

            // Compute root in (1/d2,+infinity).
            temp0 = (C_<T>(1) + multiplier0 * sqrtd0c0) * invD0;
            temp1 = (C_<T>(1) + multiplier0 * sqrtd1c1) * invD1;
            temp2 = (C_<T>(1) + multiplier0 * sqrtd2c2) * invD2;
            smax = std::max(std::max(temp0, temp1), temp2);
            fmax = F(smax);
            GTL_RUNTIME_ASSERT(
                fmax < C_<T>(0),
                "Unexpected condition.");
            smin = (C_<T>(1) + multiplier1 * sqrtd2c2) * invD2;
            fmin = F(smin);
            GTL_RUNTIME_ASSERT(
                fmin > C_<T>(0),
                "Unexpected condition.");
            converged = bisector(F, smin, smax, fmin, fmax, s, fAtS);
            GTL_RUNTIME_ASSERT(
                converged && bisector.GetNumIterations() > 0,
                "Unexpected condition.");
            roots[numRoots++] = s;
        }

    private:
        friend class UnitTestIntrEllipsoid3Ellipsoid3;
    };
}
