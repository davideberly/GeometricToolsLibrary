// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.13

#pragma once

#include <GTL/Mathematics/Primitives/3D/Circle3.h>

namespace gtl
{
    template <typename T>
    class ContCircle3
    {
    public:
        // Circle circumscribing a triangle in 3D.
        static bool Circumscribe(Vector3<T> const& v0, Vector3<T> const& v1,
            Vector3<T> const& v2, Circle3<T>& circle)
        {
            Vector3<T> E02 = v0 - v2;
            Vector3<T> E12 = v1 - v2;
            T e02e02 = Dot(E02, E02);
            T e02e12 = Dot(E02, E12);
            T e12e12 = Dot(E12, E12);
            T det = e02e02 * e12e12 - e02e12 * e02e12;
            if (det != C_<T>(0))
            {
                T halfInvDet = C_<T>(1, 2) / det;
                T u0 = halfInvDet * e12e12 * (e02e02 - e02e12);
                T u1 = halfInvDet * e02e02 * (e12e12 - e02e12);
                Vector3<T> tmp = u0 * E02 + u1 * E12;
                circle.center = v2 + tmp;
                circle.normal = UnitCross(E02, E12);
                circle.radius = Length(tmp);
                return true;
            }
            return false;
        }

        // Circle inscribing a triangle in 3D.
        static bool Inscribe(Vector3<T> const& v0, Vector3<T> const& v1,
            Vector3<T> const& v2, Circle3<T>& circle)
        {
            // Edges.
            Vector3<T> E0 = v1 - v0;
            Vector3<T> E1 = v2 - v1;
            Vector3<T> E2 = v0 - v2;

            // Plane normal.
            circle.normal = Cross(E1, E0);

            // Edge normals within the plane.
            Vector3<T> N0 = UnitCross(circle.normal, E0);
            Vector3<T> N1 = UnitCross(circle.normal, E1);
            Vector3<T> N2 = UnitCross(circle.normal, E2);

            T a0 = Dot(N1, E0);
            if (a0 == C_<T>(0))
            {
                return false;
            }

            T a1 = Dot(N2, E1);
            if (a1 == C_<T>(0))
            {
                return false;
            }

            T a2 = Dot(N0, E2);
            if (a2 == C_<T>(0))
            {
                return false;
            }

            T invA0 = C_<T>(1) / a0;
            T invA1 = C_<T>(1) / a1;
            T invA2 = C_<T>(1) / a2;

            circle.radius = C_<T>(1) / (invA0 + invA1 + invA2);
            circle.center = circle.radius * (invA0 * v0 + invA1 * v1 + invA2 * v2);
            Normalize(circle.normal);
            return true;
        }

    private:
        friend class UnitTestContCircle3;
    };
}
