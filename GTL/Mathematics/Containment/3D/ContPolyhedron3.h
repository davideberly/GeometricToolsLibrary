// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.13

#pragma once

// This class contains various implementations for point-in-polyhedron
// queries. The planes stored with the faces are used in all cases to
// reject ray-face intersection tests, a quick culling operation.
//
// The algorithm is to cast a ray from the input point P and test for
// intersection against each face of the polyhedron. If the ray only
// intersects faces at interior points (not vertices, not edge points),
// then the point is inside when the number of intersections is odd and
// the point is outside when the number of intersections is even. If the
// ray intersects an edge or a vertex, then the counting must be handled
// differently. The details are tedious. As an alternative, the approach
// here is to allow you to specify 2*N+1 rays, where N >= 0. You should
// choose these rays randomly. Each ray reports the point is inside or
// outside; whichever result occurs N+1 or more times is the winner. The
// input rayQuantity is 2*N+1. The input array of directions must have
// rayQuantity elements. If you are feeling lucky, choose rayQuantity
// to be 1.

#include <GTL/Mathematics/Containment/2D/ContPolygon2.h>
#include <GTL/Mathematics/Intersection/ND/IntrRayHyperplane.h>
#include <GTL/Mathematics/Intersection/3D/IntrRay3Triangle3.h>
#include <GTL/Mathematics/Primitives/3D/Plane3.h>
#include <array>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class ContPolyhedron3
    {
    public:
        enum FaceType
        {
            TRIANGLE,
            CONVEX,
            SIMPLE
        };

        class Face
        {
        public:
            // The members 'indices' and 'plane' are used for triangle faces,
            // for convex polygon faces, and for simple polygon faces. The
            // member 'triangles' is used only for simple faces that are not
            // convex.
            Face()
                :
                indices{},
                plane{},
                triangles{}
            {}

            // When you view the face from outside, the vertices are
            // counterclockwise ordered. The indices array stores the indices
            // into the vertex array.
            std::array<std::size_t, 3> indices;

            // The normal vector is unit length and points to the outside of
            // the polyhedron.
            Plane3<T> plane;

            // Each simple face may be triangulated. The indices are relative
            // to the vertex array. Each triple of indices represents a
            // triangle in the triangulation.
            std::vector<std::size_t> triangles;
        };

        // Test whether P is contained by the polyhedron. The query uses
        // ray-triangle intersection queries. This function will select the
        // actual algorithm based on the face type. Each type can have
        // multiple methods used for point-in-polygon tests where the
        // polygons are in the planes of the faces.
        // 
        // Triangle faces.
        //   0 : The parameter is unused. Ray-triangle tests are performed
        //       without projection on the planes of the triangles.
        // 
        // Convex faces.
        //   0 : Use a triangle fan and perform a ray-triangle intersection
        //       query for each triangle.
        //   1 : Find the point of intersection of ray and plane of polygon.
        //       Test whether that point is inside the convex polygon using an
        //       O(N) test.
        //   2 : Find the point of intersection of ray and plane of polygon.
        //       Test whether that point is inside the convex polygon using an
        //       O(log N) test.
        //
        // Simple faces that are not convex.
        //   0 : Iterate over the triangles of each face and perform a
        //       ray-triangle intersection query for each triangle. This
        //       requires that the Face::Triangles array be initialized for
        //       each face.
        //   1 : Find the point of intersection of ray and plane of polygon.
        //       Test whether that point is inside the polygon using an O(N)
        //       test. The Face::Triangles array is not used for this method,
        //       so it does not have to be initialized for each face.
        static bool InContainer(
            FaceType type,
            std::uint32_t method,
            Vector3<T> const& p,
            std::vector<Vector3<T>> const& points,
            std::vector<Face> const& faces,
            std::vector<Vector3<T>> const& directions)
        {
            if (type == TRIANGLE)
            {
                return ContainsT0(p, points, faces, directions);
            }

            if (type == CONVEX)
            {
                if (method == 0)
                {
                    return ContainsC0(p, points, faces, directions);
                }
                else // method is 1 or 2
                {
                    return ContainsC1C2(method, p, points, faces, directions);
                }
            }

            if (type == SIMPLE)
            {
                if (method == 0)
                {
                    return ContainsS0(p, points, faces, directions);
                }

                if (method == 1)
                {
                    return ContainsS1(method, p, points, faces, directions);
                }
            }

            return false;
        }

    private:
        // This function is used for queries involving all types of faces. The
        // ray origin is the test point. The ray direction is one of those
        // passed to the constructors. The plane origin is a point on the
        // plane of the face. The plane normal is a unit-length normal to the
        // face and that points outside the polyhedron.
        static bool FastNoIntersect(Ray3<T> const& ray, Plane3<T> const& plane)
        {
            T planeDistance = Dot(plane.normal, ray.origin) - plane.constant;
            T planeAngle = Dot(plane.normal, ray.direction);

            if (planeDistance < C_<T>(0))
            {
                // The ray origin is on the negative side of the plane.
                if (planeAngle <= C_<T>(0))
                {
                    // The ray points away from the plane.
                    return true;
                }
            }

            if (planeDistance > C_<T>(0))
            {
                // The ray origin is on the positive side of the plane.
                if (planeAngle >= C_<T>(0))
                {
                    // The ray points away from the plane.
                    return true;
                }
            }

            return false;
        }

        using ContainmentQuery = void (*)(
            std::uint32_t,                      // method
            std::vector<Vector2<T>> const&,     // projVertices
            Vector2<T> const&,                  // projIntersect
            bool&);                             // odd

        static void ContainsPointConvex(
            uint32_t method,
            std::vector<Vector2<T>> const& projVertices,
            Vector2<T> const& projIntersect,
            bool& odd)
        {
            if (method == 1)
            {
                if (ContPolygon2<T>::InContainerConvexOrderN(projVertices, projIntersect))
                {
                    // The ray intersects the triangle.
                    odd = !odd;
                }
            }
            else
            {
                if (ContPolygon2<T>::InContainerConvexOrderLogN(projVertices, projIntersect))
                {
                    // The ray intersects the triangle.
                    odd = !odd;
                }
            }
        }

        static void ContainsPointSimple(
            std::uint32_t,
            std::vector<Vector2<T>> const& projVertices,
            Vector2<T> const& projIntersect,
            bool& odd)
        {
            if (ContPolygon2<T>::InContainer(projVertices, projIntersect))
            {
                // The ray intersects the triangle.
                odd = !odd;
            }
        }

        // For triangle faces.
        static bool ContainsT0(
            Vector3<T> const& p,
            std::vector<Vector3<T>> const& points,
            std::vector<Face> const& faces,
            std::vector<Vector3<T>> const& directions)
        {
            std::size_t insideCount = 0;

            TIQuery<T, Ray3<T>, Triangle3<T>> rtQuery{};
            Triangle3<T> triangle{};
            Ray3<T> ray{};
            ray.origin = p;

            for (auto const& direction : directions)
            {
                ray.direction = direction;

                // Zero intersections to start with.
                bool odd = false;

                for (auto const& face : faces)
                {
                    // Attempt to quickly cull the triangle.
                    if (FastNoIntersect(ray, face.plane))
                    {
                        continue;
                    }

                    // Get the triangle vertices.
                    for (std::size_t i = 0; i < 3; ++i)
                    {
                        triangle.v[i] = points[face.indices[i]];
                    }

                    // Test for intersection.
                    if (rtQuery(ray, triangle).intersect)
                    {
                        // The ray intersects the triangle.
                        odd = !odd;
                    }
                }

                if (odd)
                {
                    insideCount++;
                }
            }

            return insideCount > directions.size() / 2;
        }

        // For convex faces.
        static bool ContainsC0(
            Vector3<T> const& p,
            std::vector<Vector3<T>> const& points,
            std::vector<Face> const& faces,
            std::vector<Vector3<T>> const& directions)
        {
            std::size_t insideCount = 0;

            TIQuery<T, Ray3<T>, Triangle3<T>> rtQuery{};
            Triangle3<T> triangle{};
            Ray3<T> ray{};
            ray.origin = p;

            for (auto const& direction : directions)
            {
                ray.direction = direction;

                // Zero intersections to start with.
                bool odd = false;

                for (auto const& face : faces)
                {
                    // Attempt to quickly cull the triangle.
                    if (FastNoIntersect(ray, face.plane))
                    {
                        continue;
                    }

                    // Process the triangles in a trifan of the face.
                    std::size_t numVerticesM1 = face.indices.size() - 1;
                    triangle.v[0] = points[face.indices[0]];
                    for (std::size_t i = 1; i < numVerticesM1; ++i)
                    {
                        triangle.v[1] = points[face.indices[i]];
                        triangle.v[2] = points[face.indices[i + 1]];

                        if (rtQuery(ray, triangle).intersect)
                        {
                            // The ray intersects the triangle.
                            odd = !odd;
                        }
                    }
                }

                if (odd)
                {
                    insideCount++;
                }
            }

            return insideCount > directions.size() / 2;
        }

        static bool ContainsC1C2(
            uint32_t method,
            Vector3<T> const& p,
            std::vector<Vector3<T>> const& points,
            std::vector<Face> const& faces,
            std::vector<Vector3<T>> const& directions)
        {
            return SharedContains(ContainsPointConvex, method, p, points, faces, directions);
        }

        // For simple faces.
        static bool ContainsS0(
            Vector3<T> const& p,
            std::vector<Vector3<T>> const& points,
            std::vector<Face> const& faces,
            std::vector<Vector3<T>> const& directions)
        {
            std::size_t insideCount = 0;

            TIQuery<T, Ray3<T>, Triangle3<T>> rtQuery{};
            Triangle3<T> triangle{};
            Ray3<T> ray{};
            ray.origin = p;

            for (auto const& direction : directions)
            {
                ray.direction = direction;

                // Zero intersections to start with.
                bool odd = false;

                for (auto const& face : faces)
                {
                    // Attempt to quickly cull the triangle.
                    if (FastNoIntersect(ray, face.plane))
                    {
                        continue;
                    }

                    // The triangulation must exist to use it.
                    std::size_t numTriangles = face.triangles.size() / 3;
                    GTL_RUNTIME_ASSERT(
                        numTriangles > 0,
                        "Triangulation must exist.");

                    // Process the triangles in a triangulation of the face.
                    std::size_t const* currentIndex = face.triangles.data();
                    for (std::size_t t = 0; t < numTriangles; ++t)
                    {
                        // Get the triangle vertices.
                        for (std::size_t i = 0; i < 3; ++i)
                        {
                            triangle.v[i] = points[*currentIndex++];
                        }

                        // Test for intersection.
                        if (rtQuery(ray, triangle).intersect)
                        {
                            // The ray intersects the triangle.
                            odd = !odd;
                        }
                    }
                }

                if (odd)
                {
                    insideCount++;
                }
            }

            return insideCount > directions.size() / 2;
        }

        static bool ContainsS1(
            uint32_t method,
            Vector3<T> const& p,
            std::vector<Vector3<T>> const& points,
            std::vector<Face> const& faces,
            std::vector<Vector3<T>> const& directions)
        {
            return SharedContains(ContainsPointSimple, method, p, points, faces, directions);
        }

        // Shared code for ContainsC1C2 and ContainsS1.
        static bool SharedContains(
            ContainmentQuery Contains,
            uint32_t method,
            Vector3<T> const& p,
            std::vector<Vector3<T>> const& points,
            std::vector<Face> const& faces,
            std::vector<Vector3<T>> const& directions)
        {
            std::size_t insideCount = 0;

            FIQuery<T, Ray3<T>, Plane3<T>> rpQuery{};
            Ray3<T> ray{};
            ray.origin = p;

            for (auto const& direction : directions)
            {
                ray.direction = direction;

                // Zero intersections to start with.
                bool odd = false;

                for (auto const& face : faces)
                {
                    // Attempt to quickly cull the triangle.
                    if (FastNoIntersect(ray, face.plane))
                    {
                        continue;
                    }

                    // Compute the ray-plane intersection.
                    auto result = rpQuery(ray, face.plane);

                    // If you trigger this assertion, numerical round-off
                    // errors have led to a discrepancy between
                    // FastNoIntersect and the Find() result.
                    GTL_RUNTIME_ASSERT(
                        result.intersect,
                        "Unexpected condition.");

                    // Get a coordinate system for the plane. Use vertex 0
                    // as the origin.
                    Vector3<T> const& V0 = points[face.indices[0]];
                    std::array<Vector3<T>, 3> basis{};
                    basis[0] = face.plane.normal;
                    ComputeOrthogonalComplement(basis[0], basis[1], basis[2]);

                    // Project the intersection onto the plane.
                    Vector3<T> diff = result.point - V0;
                    Vector2<T> projIntersect{ Dot(basis[1], diff), Dot(basis[2], diff) };

                    // Project the face vertices onto the plane of the face.
                    // Vertex 0 is always the origin.
                    std::size_t numIndices = face.indices.size();
                    std::vector<Vector2<T>> projVertices(numIndices);
                    projVertices[0] = Vector2<T>::Zero();
                    for (std::size_t i = 1; i < numIndices; ++i)
                    {
                        diff = points[face.indices[i]] - V0;
                        projVertices[i][0] = Dot(basis[1], diff);
                        projVertices[i][1] = Dot(basis[2], diff);
                    }

                    // Test whether the intersection point is in the convex
                    // polygon.
                    Contains(method, projVertices, projIntersect, odd);
                }

                if (odd)
                {
                    insideCount++;
                }
            }

            return insideCount > directions.size() / 2;
        }

    private:
        friend class UnitTestContPolyhedron3;
    };
}
