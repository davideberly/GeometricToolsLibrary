// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.18

#pragma once

// The queries consider the cone to be single sided and solid. The
// cone height range is [hmin,hmax]. The cone can be infinite where
// hmin = 0 and hmax = +infinity, infinite truncated where hmin > 0
// and hmax = +infinity, finite where hmin = 0 and hmax < +infinity,
// or a cone frustum where hmin > 0 and hmax < +infinity. The
// algorithm details are found in
// https://www.geometrictools.com/Documentation/IntersectionLineCone.pdf

// TODO: The find-query uses QFNumber, which requires T to be a rational
// type. Implement the algorithm for floating-point types.

#include <GTL/Mathematics/Intersection/1D/IntrIntervals.h>
#include <GTL/Mathematics/Primitives/ND/Cone.h>
#include <GTL/Mathematics/Primitives/ND/Line.h>
#include <GTL/Mathematics/Arithmetic/QFNumber.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>

namespace gtl
{
    template <typename T>
    class FIQuery<T, Line3<T>, Cone3<T>>
    {
    public:
        // The rational quadratic field type with elements x + y * sqrt(d).
        // This type supports error-free computation.
        using QFN1 = QFNumber<T, 1>;

        // Convenient naming for interval find-intersection queries.
        using IIQuery = FIQuery<QFN1, std::array<QFN1, 2>, std::array<QFN1, 2>>;

        struct Output
        {
            // Because the intersection of line and cone with infinite height
            // can be a ray or a line, we use a 'type' value that allows you
            // to decide how to interpret the t[] and P[] values.

            // No interesection.
            static std::uint32_t constexpr isEmpty = 0;

            // t[0] is finite, t[1] is set to t[0], P[0] is the point of
            // intersection, P[1] is set to P[0].
            static std::uint32_t constexpr isPoint = 1;

            // t[0] and t[1] are finite with t[0] < t[1], P[0] and P[1] are
            // the endpoints of the segment of intersection.
            static std::uint32_t constexpr isSegment = 2;

            // Dot(line.direction, cone.direction) > 0:
            // t[0] is finite, t[1] is +infinity (set to +1), P[0] is the ray
            // origin, P[1] is the ray direction (set to line.direction).
            // NOTE: The ray starts at P[0] and you walk away from it in the
            // line direction.
            static std::uint32_t constexpr isRayPositive = 3;

            // Dot(line.direction, cone.direction) < 0:
            // t[0] is -infinity (set to -1), t[1] is finite, P[0] is the ray
            // endpoint, P[1] is the ray direction (set to line.direction).
            // NOTE: The ray ends at P[1] and you walk towards it in the line
            // direction.
            static std::uint32_t constexpr isRayNegative = 4;

            Output()
                :
                intersect(false),
                type(Output::isEmpty),
                t{},
                P{}
            {
            }

            void ComputePoints(Vector3<T> const& origin, Vector3<T> const& direction)
            {
                switch (type)
                {
                case Output::isEmpty:
                    for (std::size_t i = 0; i < 3; ++i)
                    {
                        P[0][i] = QFN1{};
                        P[1][i] = P[0][i];
                    }
                    break;
                case Output::isPoint:
                    for (std::size_t i = 0; i < 3; ++i)
                    {
                        P[0][i] = origin[i] + direction[i] * t[0];
                        P[1][i] = P[0][i];
                    }
                    break;
                case Output::isSegment:
                    for (std::size_t i = 0; i < 3; ++i)
                    {
                        P[0][i] = origin[i] + direction[i] * t[0];
                        P[1][i] = origin[i] + direction[i] * t[1];
                    }
                    break;
                case Output::isRayPositive:
                    for (std::size_t i = 0; i < 3; ++i)
                    {
                        P[0][i] = origin[i] + direction[i] * t[0];
                        P[1][i] = QFN1(direction[i], 0, t[0].d);
                    }
                    break;
                case Output::isRayNegative:
                    for (std::size_t i = 0; i < 3; ++i)
                    {
                        P[0][i] = origin[i] + direction[i] * t[1];
                        P[1][i] = QFN1(direction[i], 0, t[1].d);
                    }
                    break;
                default:
                    GTL_RUNTIME_ERROR(
                        "Invalid case.");
                    break;
                }
            }

            template <typename OutputType>
            static void Convert(QFN1 const& input, OutputType& output)
            {
                output = static_cast<T>(input);
            }

            template <typename OutputType>
            static void Convert(Vector3<QFN1> const& input, Vector3<OutputType>& output)
            {
                for (std::size_t i = 0; i < 3; ++i)
                {
                    output[i] = static_cast<T>(input[i]);
                }
            }

            bool intersect;
            std::uint32_t type;
            std::array<QFN1, 2> t;
            std::array<Vector3<QFN1>, 2> P;
        };

        Output operator()(Line3<T> const& line, Cone3<T> const& cone)
        {
            Output output{};
            DoQuery(line.origin, line.direction, cone, output);
            output.ComputePoints(line.origin, line.direction);
            output.intersect = (output.type != Output::isEmpty);
            return output;
        }

    protected:
        // The output.type and output.t[] values are computed by DoQuery. The
        // output.P[] and output.intersect values are computed from them in
        // the operator()(...) function.
        void DoQuery(Vector3<T> const& lineOrigin, Vector3<T> const& lineDirection,
            Cone3<T> const& cone, Output& output)
        {
            // The algorithm implemented in DoQuery avoids extra branches if
            // we choose a line whose direction forms an acute angle with the
            // cone direction.
            if (Dot(lineDirection, cone.direction) >= C_<T>(0))
            {
                DoQuerySpecial(lineOrigin, lineDirection, cone, output);
            }
            else
            {
                DoQuerySpecial(lineOrigin, -lineDirection, cone, output);
                output.t[0] = -output.t[0];
                output.t[1] = -output.t[1];
                std::swap(output.t[0], output.t[1]);
                if (output.type == Output::isRayPositive)
                {
                    output.type = Output::isRayNegative;
                }
            }
        }

        void DoQuerySpecial(Vector3<T> const& lineOrigin, Vector3<T> const& lineDirection,
            Cone3<T> const& cone, Output& output)
        {
            // Compute the number of real-valued roots and represent them
            // using rational quadratic field elements to support when T
            // is an exact rational arithmetic type. TODO: Adjust by noting
            // that we should use D/|D| because a normalized floating-point
            // D still might not have |D| = 1 (although it is close to 1).
            Vector3<T> PmV = lineOrigin - cone.vertex;
            T UdU = Dot(lineDirection, lineDirection);
            T DdU = Dot(cone.direction, lineDirection);  // >= 0
            T DdPmV = Dot(cone.direction, PmV);
            T UdPmV = Dot(lineDirection, PmV);
            T PmVdPmV = Dot(PmV, PmV);
            T c2 = DdU * DdU - cone.cosAngleSqr * UdU;
            T c1 = DdU * DdPmV - cone.cosAngleSqr * UdPmV;
            T c0 = DdPmV * DdPmV - cone.cosAngleSqr * PmVdPmV;

            if (c2 != C_<T>(0))
            {
                T discr = c1 * c1 - c0 * c2;
                if (discr < C_<T>(0))
                {
                    CaseC2NotZeroDiscrNeg(output);
                }
                else if (discr > C_<T>(0))
                {
                    CaseC2NotZeroDiscrPos(c1, c2, discr, DdU, DdPmV, cone, output);
                }
                else // discr == 0
                {
                    CaseC2NotZeroDiscrZero(c1, c2, UdU, UdPmV, DdU, DdPmV, cone, output);
                }
            }
            else if (c1 != C_<T>(0))
            {
                CaseC2ZeroC1NotZero(c0, c1, DdU, DdPmV, cone, output);
            }
            else
            {
                CaseC2ZeroC1Zero(c0, UdU, UdPmV, DdU, DdPmV, cone, output);
            }
        }

        void CaseC2NotZeroDiscrNeg(Output& output)
        {
            // Block 0. The quadratic has no real-valued roots. The line does
            // not intersect the double-sided cone.
            SetEmpty(output);
        }

        void CaseC2NotZeroDiscrPos(T const& c1, T const& c2, T const& discr,
            T const& DdU, T const& DdPmV, Cone3<T> const& cone, Output& output)
        {
            // The quadratic has two distinct real-valued roots, t[0] and t[1]
            // with t[0] < t[1].
            T x = -c1 / c2;
            T y = (c2 > C_<T>(0) ? (T)1 / c2 : (T)-1 / c2);
            std::array<QFN1, 2> t = { QFN1(x, -y, discr), QFN1(x, y, discr) };

            // Compute the signed heights at the intersection points, h[0] and
            // h[1] with h[0] <= h[1]. The ordering is guaranteed because we
            // have arranged for the input line to satisfy Dot(D,U) >= 0.
            std::array<QFN1, 2> h = { t[0] * DdU + DdPmV, t[1] * DdU + DdPmV };

            QFN1 qfzero(0, 0, discr);
            if (h[0] >= qfzero)
            {
                // Block 1. The line intersects the positive cone in two
                // points.
                SetSegmentClamp(t, h, DdU, DdPmV, cone, output);
            }
            else if (h[1] <= qfzero)
            {
                // Block 2. The line intersects the negative cone in two
                // points.
                SetEmpty(output);
            }
            else  // h[0] < 0 < h[1]
            {
                // Block 3. The line intersects the positive cone in a single
                // point and the negative cone in a single point.
                SetRayClamp(h[1], DdU, DdPmV, cone, output);
            }
        }

        void CaseC2NotZeroDiscrZero(T const& c1, T const& c2,
            T const& UdU, T const& UdPmV, T const& DdU, T const& DdPmV,
            Cone3<T> const& cone, Output& output)
        {
            T t = -c1 / c2;
            if (t * UdU + UdPmV == C_<T>(0))
            {
                // To get here, it must be that V = P + (-c1/c2) * U, where
                // U is not necessarily a unit-length vector. The line
                // intersects the cone vertex.
                if (c2 < C_<T>(0))
                {
                    // Block 4. The line is outside the double-sided cone and
                    // intersects it only at V.
                    SetPointClamp(QFN1(t, 0, 0), QFN1(0, 0, 0), cone, output);
                }
                else
                {
                    // Block 5. The line is inside the double-sided cone, so
                    // the intersection is a ray with origin V.
                    SetRayClamp(QFN1(0, 0, 0), DdU, DdPmV, cone, output);
                }
            }
            else
            {
                // The line is tangent to the cone at a point different from
                // the vertex.
                T h = t * DdU + DdPmV;
                if (h >= C_<T>(0))
                {
                    // Block 6. The line is tangent to the positive cone.
                    SetPointClamp(QFN1(t, 0, 0), QFN1(h, 0, 0), cone, output);
                }
                else
                {
                    // Block 7. The line is tangent to the negative cone.
                    SetEmpty(output);
                }
            }
        }

        void CaseC2ZeroC1NotZero(T const& c0, T const& c1, T const& DdU,
            T const& DdPmV, Cone3<T> const& cone, Output& output)
        {
            // U is a direction vector on the cone boundary. Compute the
            // t-value for the intersection point and compute the
            // corresponding height h to determine whether that point is on
            // the positive cone or negative cone.
            T t = C_<T>(-1, 2) * c0 / c1;
            T h = t * DdU + DdPmV;
            if (h > C_<T>(0))
            {
                // Block 8. The line intersects the positive cone and the ray
                // of intersection is interior to the positive cone. The
                // intersection is a ray or segment.
                SetRayClamp(QFN1(h, 0, 0), DdU, DdPmV, cone, output);
            }
            else
            {
                // Block 9. The line intersects the negative cone and the ray
                // of intersection is interior to the negative cone.
                SetEmpty(output);
            }
        }

        void CaseC2ZeroC1Zero(T const& c0, T const& UdU, T const& UdPmV,
            T const& DdU, T const& DdPmV, Cone3<T> const& cone, Output& output)
        {
            if (c0 != C_<T>(0))
            {
                // Block 10. The line does not intersect the double-sided
                // cone.
                SetEmpty(output);
            }
            else
            {
                // Block 11. The line is on the cone boundary. The
                // intersection with the positive cone is a ray that contains
                // the cone vertex.  The intersection is either a ray or
                // segment.
                T t = -UdPmV / UdU;
                T h = t * DdU + DdPmV;
                SetRayClamp(QFN1(h, 0, 0), DdU, DdPmV, cone, output);
            }
        }

        void SetEmpty(Output& output)
        {
            output.type = Output::isEmpty;
            output.t[0] = QFN1{};
            output.t[1] = QFN1{};
        }

        void SetPoint(QFN1 const& t, Output& output)
        {
            output.type = Output::isPoint;
            output.t[0] = t;
            output.t[1] = output.t[0];
        }

        void SetSegment(QFN1 const& t0, QFN1 const& t1, Output& output)
        {
            output.type = Output::isSegment;
            output.t[0] = t0;
            output.t[1] = t1;
        }

        void SetRayPositive(QFN1 const& t, Output& output)
        {
            output.type = Output::isRayPositive;
            output.t[0] = t;
            output.t[1] = QFN1(+1, 0, t.d);  // +infinity
        }

        void SetRayNegative(QFN1 const& t, Output& output)
        {
            output.type = Output::isRayNegative;
            output.t[0] = QFN1(-1, 0, t.d);  // +infinity
            output.t[1] = t;
        }

        void SetPointClamp(QFN1 const& t, QFN1 const& h,
            Cone3<T> const& cone, Output& output)
        {
            if (cone.HeightInRange(h.x[0]))
            {
                // P0.
                SetPoint(t, output);
            }
            else
            {
                // P1.
                SetEmpty(output);
            }
        }

        void SetSegmentClamp(std::array<QFN1, 2> const& t, std::array<QFN1, 2> const& h,
            T const& DdU, T const& DdPmV, Cone3<T> const& cone, Output& output)
        {
            std::array<QFN1, 2> hrange =
            {
                QFN1(cone.GetMinHeight(), 0, h[0].d),
                QFN1(cone.GetMaxHeight(), 0, h[0].d)
            };

            if (h[1] > h[0])
            {
                auto iir = (cone.IsFinite() ? IIQuery()(h, hrange) : IIQuery()(h, hrange[0], true));
                if (iir.numIntersections == 2)
                {
                    // S0.
                    SetSegment((iir.overlap[0] - DdPmV) / DdU, (iir.overlap[1] - DdPmV) / DdU, output);
                }
                else if (iir.numIntersections == 1)
                {
                    // S1.
                    SetPoint((iir.overlap[0] - DdPmV) / DdU, output);
                }
                else  // iir.numIntersections == 0
                {
                    // S2.
                    SetEmpty(output);
                }
            }
            else  // h[1] == h[0]
            {
                if (hrange[0] <= h[0] && (cone.IsFinite() ? h[0] <= hrange[1] : true))
                {
                    // S3. DdU > 0 and the line is not perpendicular to the
                    // cone axis.
                    SetSegment(t[0], t[1], output);
                }
                else
                {
                    // S4. DdU == 0 and the line is perpendicular to the
                    // cone axis.
                    SetEmpty(output);
                }
            }
        }

        void SetRayClamp(QFN1 const& h, T const& DdU, T const& DdPmV,
            Cone3<T> const& cone, Output& output)
        {
            std::array<QFN1, 2> hrange =
            {
                QFN1(cone.GetMinHeight(), 0, h.d),
                QFN1(cone.GetMaxHeight(), 0, h.d)
            };

            if (cone.IsFinite())
            {
                auto iir = IIQuery()(hrange, h, true);
                if (iir.numIntersections == 2)
                {
                    // R0.
                    SetSegment((iir.overlap[0] - DdPmV) / DdU, (iir.overlap[1] - DdPmV) / DdU, output);
                }
                else if (iir.numIntersections == 1)
                {
                    // R1.
                    SetPoint((iir.overlap[0] - DdPmV) / DdU, output);
                }
                else  // iir.numIntersections == 0
                {
                    // R2.
                    SetEmpty(output);
                }
            }
            else
            {
                // R3.
                SetRayPositive((std::max(hrange[0], h) - DdPmV) / DdU, output);
            }
        }

    private:
        friend class UnitTestIntrLine3Cone3;
    };
}
