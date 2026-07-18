// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// The test-intersection query is based on the document
// https://www.geometrictools.com/Documentation/IntersectionSphereCone.pdf
// The find-intersection returns a single point in the set of intersection
// when that intersection is not empty.

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Primitives/3D/Sphere3.h>
#include <GTL/Mathematics/Primitives/ND/Cone.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <algorithm>
#include <cmath>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Sphere3<T>, Cone3<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false)
            {
            }

            bool intersect;
        };

        Output operator()(Sphere3<T> const& sphere, Cone3<T> const& cone)
        {
            Output output{};
            if (cone.GetMinHeight() > C_<T>(0))
            {
                if (cone.IsFinite())
                {
                    output.intersect = DoQueryConeFrustum(sphere, cone);
                }
                else
                {
                    output.intersect = DoQueryInfiniteTruncatedCone(sphere, cone);
                }
            }
            else
            {
                if (cone.IsFinite())
                {
                    output.intersect = DoQueryFiniteCone(sphere, cone);
                }
                else
                {
                    output.intersect = DoQueryInfiniteCone(sphere, cone);
                }
            }
            return output;
        }

    private:
        bool DoQueryInfiniteCone(Sphere3<T> const& sphere, Cone3<T> const& cone)
        {
            Vector3<T> U = cone.vertex - (sphere.radius * cone.invSinAngle) * cone.direction;
            Vector3<T> CmU = sphere.center - U;
            T AdCmU = Dot(cone.direction, CmU);
            if (AdCmU > C_<T>(0))
            {
                T sqrLengthCmU = Dot(CmU, CmU);
                if (AdCmU * AdCmU >= sqrLengthCmU * cone.cosAngleSqr)
                {
                    Vector3<T> CmV = sphere.center - cone.vertex;
                    T AdCmV = Dot(cone.direction, CmV);
                    if (AdCmV < -sphere.radius)
                    {
                        return false;
                    }

                    T rSinAngle = sphere.radius * cone.sinAngle;
                    if (AdCmV >= -rSinAngle)
                    {
                        return true;
                    }

                    T sqrLengthCmV = Dot(CmV, CmV);
                    return sqrLengthCmV <= sphere.radius * sphere.radius;
                }
            }

            return false;
        }

        bool DoQueryInfiniteTruncatedCone(Sphere3<T> const& sphere, Cone3<T> const& cone)
        {
            Vector3<T> U = cone.vertex - (sphere.radius * cone.invSinAngle) * cone.direction;
            Vector3<T> CmU = sphere.center - U;
            T AdCmU = Dot(cone.direction, CmU);
            if (AdCmU > C_<T>(0))
            {
                T sqrLengthCmU = Dot(CmU, CmU);
                if (AdCmU * AdCmU >= sqrLengthCmU * cone.cosAngleSqr)
                {
                    Vector3<T> CmV = sphere.center - cone.vertex;
                    T AdCmV = Dot(cone.direction, CmV);
                    T minHeight = cone.GetMinHeight();
                    if (AdCmV < minHeight - sphere.radius)
                    {
                        return false;
                    }

                    T rSinAngle = sphere.radius * cone.sinAngle;
                    if (AdCmV >= -rSinAngle)
                    {
                        return true;
                    }

                    Vector3<T> D = CmV - minHeight * cone.direction;
                    T lengthAxD = Length(Cross(cone.direction, D));
                    T hminTanAngle = minHeight * cone.tanAngle;
                    if (lengthAxD <= hminTanAngle)
                    {
                        return true;
                    }

                    T AdD = AdCmV - minHeight;
                    T diff = lengthAxD - hminTanAngle;
                    T sqrLengthCmK = AdD * AdD + diff * diff;
                    return sqrLengthCmK <= sphere.radius * sphere.radius;
                }
            }

            return false;
        }

        bool DoQueryFiniteCone(Sphere3<T> const& sphere, Cone3<T> const& cone)
        {
            Vector3<T> U = cone.vertex - (sphere.radius * cone.invSinAngle) * cone.direction;
            Vector3<T> CmU = sphere.center - U;
            T AdCmU = Dot(cone.direction, CmU);
            if (AdCmU > C_<T>(0))
            {
                T sqrLengthCmU = Dot(CmU, CmU);
                if (AdCmU * AdCmU >= sqrLengthCmU * cone.cosAngleSqr)
                {
                    Vector3<T> CmV = sphere.center - cone.vertex;
                    T AdCmV = Dot(cone.direction, CmV);
                    if (AdCmV < -sphere.radius)
                    {
                        return false;
                    }

                    T maxHeight = cone.GetMaxHeight();
                    if (AdCmV > cone.GetMaxHeight() + sphere.radius)
                    {
                        return false;
                    }

                    T rSinAngle = sphere.radius * cone.sinAngle;
                    if (AdCmV >= -rSinAngle)
                    {
                        if (AdCmV <= maxHeight - rSinAngle)
                        {
                            return true;
                        }
                        else
                        {
                            Vector3<T> barD = CmV - maxHeight * cone.direction;
                            T lengthAxBarD = Length(Cross(cone.direction, barD));
                            T hmaxTanAngle = maxHeight * cone.tanAngle;
                            if (lengthAxBarD <= hmaxTanAngle)
                            {
                                return true;
                            }

                            T AdBarD = AdCmV - maxHeight;
                            T diff = lengthAxBarD - hmaxTanAngle;
                            T sqrLengthCmBarK = AdBarD * AdBarD + diff * diff;
                            return sqrLengthCmBarK <= sphere.radius * sphere.radius;
                        }
                    }
                    else
                    {
                        T sqrLengthCmV = Dot(CmV, CmV);
                        return sqrLengthCmV <= sphere.radius * sphere.radius;
                    }
                }
            }

            return false;
        }

        bool DoQueryConeFrustum(Sphere3<T> const& sphere, Cone3<T> const& cone)
        {
            Vector3<T> U = cone.vertex - (sphere.radius * cone.invSinAngle) * cone.direction;
            Vector3<T> CmU = sphere.center - U;
            T AdCmU = Dot(cone.direction, CmU);
            if (AdCmU > C_<T>(0))
            {
                T sqrLengthCmU = Dot(CmU, CmU);
                if (AdCmU * AdCmU >= sqrLengthCmU * cone.cosAngleSqr)
                {
                    Vector3<T> CmV = sphere.center - cone.vertex;
                    T AdCmV = Dot(cone.direction, CmV);
                    T minHeight = cone.GetMinHeight();
                    if (AdCmV < minHeight - sphere.radius)
                    {
                        return false;
                    }

                    T maxHeight = cone.GetMaxHeight();
                    if (AdCmV > maxHeight + sphere.radius)
                    {
                        return false;
                    }

                    T rSinAngle = sphere.radius * cone.sinAngle;
                    if (AdCmV >= minHeight - rSinAngle)
                    {
                        if (AdCmV <= maxHeight - rSinAngle)
                        {
                            return true;
                        }
                        else
                        {
                            Vector3<T> barD = CmV - maxHeight * cone.direction;
                            T lengthAxBarD = Length(Cross(cone.direction, barD));
                            T hmaxTanAngle = maxHeight * cone.tanAngle;
                            if (lengthAxBarD <= hmaxTanAngle)
                            {
                                return true;
                            }

                            T AdBarD = AdCmV - maxHeight;
                            T diff = lengthAxBarD - hmaxTanAngle;
                            T sqrLengthCmBarK = AdBarD * AdBarD + diff * diff;
                            return sqrLengthCmBarK <= sphere.radius * sphere.radius;
                        }
                    }
                    else
                    {
                        Vector3<T> D = CmV - minHeight * cone.direction;
                        T lengthAxD = Length(Cross(cone.direction, D));
                        T hminTanAngle = minHeight * cone.tanAngle;
                        if (lengthAxD <= hminTanAngle)
                        {
                            return true;
                        }

                        T AdD = AdCmV - minHeight;
                        T diff = lengthAxD - hminTanAngle;
                        T sqrLengthCmK = AdD * AdD + diff * diff;
                        return sqrLengthCmK <= sphere.radius * sphere.radius;
                    }
                }
            }

            return false;
        }

    private:
        friend class UnitTestIntrSphere3Cone3;
    };

    template <typename T>
    class FIQuery<T, Sphere3<T>, Cone3<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                point{}
            {
            }

            // If an intersection occurs, it is potentially an infinite set.
            // If the cone vertex is inside the sphere, 'point' is set to the
            // cone vertex. If the sphere center is inside the cone, 'point'
            // is set to the sphere center. Otherwise, 'point' is set to the
            // cone point that is closest to the cone vertex and inside the
            // sphere.
            bool intersect;
            Vector3<T> point;
        };

        Output operator()(Sphere3<T> const& sphere, Cone3<T> const& cone)
        {
            Output output{};

            // Test whether the cone vertex is inside the sphere.
            Vector3<T> diff = sphere.center - cone.vertex;
            T rSqr = sphere.radius * sphere.radius;
            T lenSqr = Dot(diff, diff);
            if (lenSqr <= rSqr)
            {
                // The cone vertex is inside the sphere, so the sphere and
                // cone intersect.
                output.intersect = true;
                output.point = cone.vertex;
                return output;
            }

            // Test whether the sphere center is inside the cone.
            T dot = Dot(diff, cone.direction);
            T dotSqr = dot * dot;
            if (dotSqr >= lenSqr * cone.cosAngleSqr && dot > C_<T>(0))
            {
                // The sphere center is inside cone, so the sphere and cone
                // intersect.
                output.intersect = true;
                output.point = sphere.center;
                return output;
            }

            // The sphere center is outside the cone. The problem now reduces
            // to computing an intersection between the circle and the ray in
            // the plane containing the cone vertex and spanned by the cone
            // axis and vector from the cone vertex to the sphere center.

            // The ray is parameterized by t * D + V with t >= 0, |D| = 1 and
            // dot(A,D) = cos(angle). Also, D = e * A + f * (C - V).
            // Substituting the ray equation into the sphere equation yields
            // R^2 = |t * D + V - C|^2, so the quadratic for intersections is
            // t^2 - 2 * dot(D, C - V) * t + |C - V|^2 - R^2 = 0. An
            // intersection occurs if and only if the discriminant is
            // nonnegative. This test becomes
            //     dot(D, C - V)^2 >= dot(C - V, C - V) - R^2
            // Note that if the right-hand side is nonpositive, then the
            // inequality is true (the sphere contains V). This is already
            // ruled out in the first block of code in this function.

            T uLen = std::sqrt(std::max(lenSqr - dotSqr, C_<T>(0)));
            T test = cone.cosAngle * dot + cone.sinAngle * uLen;
            T discr = test * test - lenSqr + rSqr;

            if (discr >= C_<T>(0) && test >= C_<T>(0))
            {
                // Compute the point of intersection closest to the cone
                // vertex.
                output.intersect = true;
                T t = test - std::sqrt(std::max(discr, C_<T>(0)));
                Vector3<T> B = diff - dot * cone.direction;
                T tmp = cone.sinAngle / uLen;
                output.point = t * (cone.cosAngle * cone.direction + tmp * B);
            }
            else
            {
                output.intersect = false;
            }

            return output;
        }

    private:
        friend class UnitTestIntrSphere3Cone3;
    };
}
