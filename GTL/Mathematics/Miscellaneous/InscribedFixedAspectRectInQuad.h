// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// Compute the maximum-area, fixed-aspect-ratio, and axis-aligned rectangle
// inscribed in a convex quadrilateral. The algorithm is described in
// https://www.geometrictools.com/Documentation/MaximumAreaAspectRectangle.pdf

#include <GTL/Mathematics/Intersection/1D/IntrIntervals.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>
#include <cmath>
#include <cstddef>
#include <utility>

namespace gtl
{
    template <typename T>
    class InscribedFixedAspectRectInQuad
    {
    public:
        // The returned 'bool' is 'true' when there is a unique solution or
        // 'false' when there are infinitely many solutions. The caller is
        // responsible for the 'quad' vertices occuring in counterclockwise
        // order. The output 'rectOrigin' is (u,v), the 'rectWidth' is w, and
        // the 'rectHeight' is h. The rectangle vertices are (u, v),
        // (u + w, v), (u + w, v + h), and (u, v + h) in counterclockwise
        // order.
        static bool Execute(std::array<Vector2<T>, 4> const& quad, T const& aspectRatio,
            Vector2<T>& rectOrigin, T& rectWidth, T& rectHeight)
        {
            bool isUnique = false;

            // The i-th edge lies on a line with origin quad[i] and non-unit
            // direction edges[i] = quad[(i + 1) % 3] - quad[i]. The lines
            // containing the edges have these inner-pointing normal vectors.
            std::array<Vector2<T>, 4> normals =
            {
                Perp(quad[0] - quad[1]),
                Perp(quad[1] - quad[2]),
                Perp(quad[2] - quad[3]),
                Perp(quad[3] - quad[0])
            };

            // Compute the 4 linear inequality constraints of the form
            // Dot(N[i], R[floor(2*angle[i]/pi)] - V[i]) >= 0, where V[i] is a
            // quad vertex and N[i] is a corresponding normal. The angle[i] is
            // the angle formed by N[i] with the positive x-axis and is in
            // [0,2*pi). Each constraint is of the form
            //   Dot((m0,m1,m2),(u,v,w)) - c >= 0
            // and stored as a pair, the first is the 3D normal vector
            // (m0, m1, m2) and the second is the constant c.
            T const zero = C_<T>(0);
            T const twoPi = C_TWO_PI<T>;
            T const invTwoDivPi = C_INV_HALF_PI<T>;

            std::array<std::pair<Vector3<T>, T>, 4> constraints{};
            for (std::size_t i = 0; i < 4; ++i)
            {
                T angle = std::atan2(normals[i][1], normals[i][0]);
                if (angle < zero)
                {
                    angle += twoPi;
                }

                std::size_t j = static_cast<size_t>(std::floor(invTwoDivPi * angle));
                constraints[i].first[0] = normals[i][0];
                constraints[i].first[1] = normals[i][1];
                constraints[i].second = Dot(normals[i], quad[i]);
                if (j == 0)
                {
                    // rect[0] = (u, v)
                    constraints[i].first[2] = zero;
                }
                else if (j == 1)
                {
                    // rect[1] = (u, v) + (w, 0)
                    constraints[i].first[2] = normals[i][0];
                }
                else if (j == 2)
                {
                    // rect[2] = (u, v) + (w, w / r)
                    constraints[i].first[2] = normals[i][0] + normals[i][1] / aspectRatio;
                }
                else // j == 3
                {
                    // rect[3] = (u, v) + (0, w / r)
                    constraints[i].first[2] = normals[i][1] / aspectRatio;
                }
            }

            // The problem is to maximize w > 0 subject to the 4 linear
            // inequality constraints. It is sufficient to solve linear
            // equations to compute the vertices of the convex polyhedron
            // domain defined by the constrants, examining only those with
            // w > 0.
            Vector3<T> origin{}, direction{};
            bool isLine = FindIntersection(
                constraints[0].first, constraints[0].second,
                constraints[2].first, constraints[2].second,
                origin, direction);
            GTL_RUNTIME_ASSERT(
                isLine,
                "Theoretically, the intersection of the planes is a line.");

            T alpha1 = Dot(constraints[1].first, direction);
            T beta1 = Dot(constraints[1].first, origin) - constraints[1].second;
            T alpha3 = Dot(constraints[3].first, direction);
            T beta3 = Dot(constraints[3].first, origin) - constraints[3].second;
            GTL_RUNTIME_ASSERT(
                alpha1 != zero && alpha3 != zero,
                "Theoretically, the alpha values are nonzero.");

            T end1 = -beta1 / alpha1;
            bool isPositiveInfinite1 = (alpha1 > zero);
            T end3 = -beta3 / alpha3;
            bool isPositiveInfinite3 = (alpha3 > zero);
            IIOutput output = IIQuery{}(end1, isPositiveInfinite1, end3, isPositiveInfinite3);
            if (output.type == IIOutput::isFinite)
            {
                Vector3<T> solution0 = output.overlap[0] * direction + origin;
                Vector3<T> solution1 = output.overlap[1] * direction + origin;
                if (solution0[2] > solution1[2])
                {
                    rectOrigin[0] = solution0[0];
                    rectOrigin[1] = solution0[1];
                    rectWidth = solution0[2];
                    rectHeight = rectWidth / aspectRatio;
                }
                else
                {
                    rectOrigin[0] = solution1[0];
                    rectOrigin[1] = solution1[1];
                    rectWidth = solution1[2];
                    rectHeight = rectWidth / aspectRatio;
                }

                isUnique = (solution0[2] != solution1[2]);
            }
            else if (output.type == IIOutput::isPoint)
            {
                Vector3<T> solution = output.overlap[0] * direction + origin;
                rectOrigin[0] = solution[0];
                rectOrigin[1] = solution[1];
                rectWidth = solution[2];
                rectHeight = rectWidth / aspectRatio;

                isUnique = true;
            }
            else if (output.type == IIOutput::isEmpty)
            {
                isLine = FindIntersection(
                    constraints[1].first, constraints[1].second,
                    constraints[3].first, constraints[3].second,
                    origin, direction);
                GTL_RUNTIME_ASSERT(
                    isLine,
                    "Theoretically, the intersection of the planes is a line.");

                T alpha0 = Dot(constraints[0].first, direction);
                T beta0 = Dot(constraints[0].first, origin) - constraints[0].second;
                T alpha2 = Dot(constraints[2].first, direction);
                T beta2 = Dot(constraints[2].first, origin) - constraints[2].second;
                GTL_RUNTIME_ASSERT(
                    alpha0 != zero && alpha2 != zero,
                    "Theoretically, the alpha values are nonzero.");

                T end0 = -beta0 / alpha0;
                bool isPositiveInfinite0 = (alpha0 > zero);
                T end2 = -beta2 / alpha2;
                bool isPositiveInfinite2 = (alpha2 > zero);
                output = IIQuery{}(end0, isPositiveInfinite0, end2, isPositiveInfinite2);
                if (output.type == IIOutput::isFinite)
                {
                    Vector3<T> solution0 = output.overlap[0] * direction + origin;
                    Vector3<T> solution1 = output.overlap[1] * direction + origin;
                    if (solution0[2] > solution1[2])
                    {
                        rectOrigin[0] = solution0[0];
                        rectOrigin[1] = solution0[1];
                        rectWidth = solution0[2];
                        rectHeight = rectWidth / aspectRatio;
                    }
                    else
                    {
                        rectOrigin[0] = solution1[0];
                        rectOrigin[1] = solution1[1];
                        rectWidth = solution1[2];
                        rectHeight = rectWidth / aspectRatio;
                    }

                    isUnique = (solution0[2] != solution1[2]);
                }
                else if (output.type == IIOutput::isPoint)
                {
                    Vector3<T> solution = output.overlap[0] * direction + origin;
                    rectOrigin[0] = solution[0];
                    rectOrigin[1] = solution[1];
                    rectWidth = solution[2];
                    rectHeight = rectWidth / aspectRatio;

                    isUnique = true;
                }
                else
                {
                    GTL_RUNTIME_ERROR(
                        "Theoretically, the intersection is empty or a single point.");
                }
            }
            else
            {
                GTL_RUNTIME_ERROR(
                    "Theoretically, the intersection is empty, a single point, or an interval.");
            }

            return isUnique;
        }

    private:
        using IIQuery = FIQuery<T, std::array<T, 2>, std::array<T, 2>>;
        using IIOutput = typename IIQuery::Output;

        static bool FindIntersection(
            Vector3<T> const& normal0, T const& constant0,
            Vector3<T> const& normal1, T const& constant1,
            Vector3<T>& origin, Vector3<T>& direction)
        {
            // The intersection line is of the form
            // t * Cross(normal0, normal1) + a0 * normal0 + a1 * normal1

            direction = Cross(normal0, normal1);
            if (!IsZero(direction))
            {
                T dotN0N0 = Dot(normal0, normal0);
                T dotN0N1 = Dot(normal0, normal1);
                T dotN1N1 = Dot(normal1, normal1);
                T det = Dot(direction, direction);
                T a0 = (dotN1N1 * constant0 - dotN0N1 * constant1) / det;
                T a1 = (dotN0N0 * constant1 - dotN0N1 * constant0) / det;
                origin = a0 * normal0 + a1 * normal1;
                return true;
            }
            else
            {
                MakeZero(origin);
                return false;
            }
        }

    private:
        friend class UnitTestInscribedFixedAspectRectInQuad;
    };
}
