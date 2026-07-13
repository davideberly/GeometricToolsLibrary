// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.13

#pragma once

#include <GTL/Mathematics/MatrixAnalysis/LinearSystem.h>
#include <GTL/Mathematics/Primitives/3D/Sphere3.h>
#include <cmath>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class ContSphere3
    {
    public:
        // Compute the smallest bounding sphere whose center is the average of
        // the input points.
        static void GetContainer(std::vector<Vector3<T>> const& points, Sphere3<T>& sphere)
        {
            GTL_ARGUMENT_ASSERT(
                points.size() > 0,
                "At least one input point is required.");

            sphere.center = points[0];
            for (std::size_t i = 1; i < points.size(); ++i)
            {
                sphere.center += points[i];
            }
            sphere.center /= static_cast<T>(points.size());

            sphere.radius = C_<T>(0);
            for (std::size_t i = 0; i < points.size(); ++i)
            {
                Vector3<T> diff = points[i] - sphere.center;
                T radiusSqr = Dot(diff, diff);
                if (radiusSqr > sphere.radius)
                {
                    sphere.radius = radiusSqr;
                }
            }

            sphere.radius = std::sqrt(sphere.radius);
        }

        // Test for containment of a point inside a sphere.
        static bool InContainer(Vector3<T> const& point, Sphere3<T> const& sphere)
        {
            Vector3<T> diff = point - sphere.center;
            return Length(diff) <= sphere.radius;
        }

        // Compute the smallest bounding sphere that contains the input
        // spheres.
        static void MergeContainers(Sphere3<T> const& sphere0, Sphere3<T> const& sphere1, Sphere3<T>& merge)
        {
            Vector3<T> cenDiff = sphere1.center - sphere0.center;
            T lenSqr = Dot(cenDiff, cenDiff);
            T rDiff = sphere1.radius - sphere0.radius;
            T rDiffSqr = rDiff * rDiff;

            if (rDiffSqr >= lenSqr)
            {
                merge = (rDiff >= C_<T>(0) ? sphere1 : sphere0);
            }
            else
            {
                T length = std::sqrt(lenSqr);
                if (length > C_<T>(0))
                {
                    T coeff = C_<T>(1, 2) * (length + rDiff) / length;
                    merge.center = sphere0.center + coeff * cenDiff;
                }
                else
                {
                    merge.center = sphere0.center;
                }

                merge.radius = C_<T>(1, 2) * (length + sphere0.radius + sphere1.radius);
            }
        }

        // Sphere circumscribing a tetrahedron. The function returns true
        // when successful, false otherwise (input points are coplanar or
        // collinear).
        static bool Circumscribe(Vector3<T> const& v0, Vector3<T> const& v1,
            Vector3<T> const& v2, Vector3<T> const& v3, Sphere3<T>& sphere)
        {
            Vector3<T> E10 = v1 - v0;
            Vector3<T> E20 = v2 - v0;
            Vector3<T> E30 = v3 - v0;

            Matrix3x3<T> A{};
            A.SetRow(0, E10);
            A.SetRow(1, E20);
            A.SetRow(2, E30);

            Vector3<T> B{ C_<T>(1, 2) * Dot(E10, E10), C_<T>(1, 2) * Dot(E20, E20), C_<T>(1, 2) * Dot(E30, E30) };

            Vector3<T> solution{};
            if (LinearSystem<T>::Solve(A, B, solution))
            {
                sphere.center = v0 + solution;
                sphere.radius = Length(solution);
                return true;
            }
            return false;
        }

        // Sphere inscribing a tetrahedron. The function returns true
        // when successful, false otherwise (input points are coplanar or
        // collinear).
        static bool Inscribe(Vector3<T> const& v0, Vector3<T> const& v1,
            Vector3<T> const& v2, Vector3<T> const& v3, Sphere3<T>& sphere)
        {
            // Edges.
            Vector3<T> E10 = v1 - v0;
            Vector3<T> E20 = v2 - v0;
            Vector3<T> E30 = v3 - v0;
            Vector3<T> E21 = v2 - v1;
            Vector3<T> E31 = v3 - v1;

            // Normals.
            Vector3<T> N0 = Cross(E31, E21);
            Vector3<T> N1 = Cross(E20, E30);
            Vector3<T> N2 = Cross(E30, E10);
            Vector3<T> N3 = Cross(E10, E20);

            // Normalize the normals.
            if (Normalize(N0) == C_<T>(0))
            {
                return false;
            }
            if (Normalize(N1) == C_<T>(0))
            {
                return false;
            }
            if (Normalize(N2) == C_<T>(0))
            {
                return false;
            }
            if (Normalize(N3) == C_<T>(0))
            {
                return false;
            }

            Matrix3x3<T> A{};
            A.SetRow(0, N1 - N0);
            A.SetRow(1, N2 - N0);
            A.SetRow(2, N3 - N0);
            Vector3<T> B{ C_<T>(0), C_<T>(0), -Dot(N3, E30) };
            Vector3<T> solution{};
            if (LinearSystem<T>::Solve(A, B, solution))
            {
                sphere.center = v3 + solution;
                sphere.radius = std::fabs(Dot(N0, solution));
                return true;
            }
            return false;
        }

    private:
        friend class UnitTestContSphere3;
    };
}
