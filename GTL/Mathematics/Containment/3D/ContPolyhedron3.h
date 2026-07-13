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
        // For simple polyhedra with triangle faces.
        struct TriangleFace
        {
            TriangleFace()
                :
                indices{ 0, 0, 0 },
                plane{}
            {
            }

            // When you view the face from outside, the vertices are
            // counterclockwise ordered. The indices array stores the indices
            // into the vertex array.
            std::array<std::size_t, 3> indices;

            // The normal vector is unit length and points to the outside of
            // the polyhedron.
            Plane3<T> plane;
        };

        // For simple polyhedra with convex polygon faces.
        struct ConvexFace
        {
            ConvexFace()
                :
                indices{},
                plane{}
            {
            }

            // When you view the face from outside, the vertices are
            // counterclockwise ordered. The indices array stores the indices
            // into the vertex array.
            std::vector<std::size_t> indices;

            // The normal vector is unit length and points to the outside of
            // the polyhedron.
            Plane3<T> plane;
        };

        // For simple polyhedra with simple polygon faces that are generally
        // not all convex.
        struct SimpleFace
        {
            SimpleFace()
                :
                indices{},
                plane{},
                triangles{}
            {
            }

            // When you view the face from outside, the vertices are
            // counterclockwise ordered. The Indices array stores the indices
            // into the vertex array.
            std::vector<std::size_t> indices;

            // The normal vector is unit length and points to the outside of
            // the polyhedron.
            Plane3<T> plane;

            // Each simple face may be triangulated. The indices are relative
            // to the vertex array. Each triple of indices represents a
            // triangle in the triangulation.
            std::vector<std::size_t> triangles;
        };


        // Test whether P is contained by the polyhedron. The query uses
        // ray-triangle intersection queries.
        static bool InContainer(
            Vector3<T> const& P,
            std::vector<Vector3<T>> const& points,
            std::vector<TriangleFace> const& faces,
            std::vector<Vector3<T>> const& directions)
        {
            TIQuery<T, Ray3<T>, Triangle3<T>> rtQuery{};
            Triangle3<T> triangle{};
            Ray3<T> ray{};
            ray.origin = P;

            std::size_t insideCount = 0;
            for (auto const& direction : directions)
            {
                ray.direction = direction;

                // Zero intersections to start with.
                bool odd = false;
                for (auto const& face : faces)
                {
                    // Attempt to cull the triangle quickly.
                    if (FastNoIntersect(ray, face.plane))
                    {
                        continue;
                    }

                    // Get the triangle vertices.
                    for (std::size_t k = 0; k < 3; ++k)
                    {
                        triangle.v[k] = points[face.indices[k]];
                    }

                    // Test for intersection.
                    auto rtOutput = rtQuery(ray, triangle);
                    if (rtOutput.intersect)
                    {
                        // The ray intersects the triangle.
                        odd = !odd;
                    }
                }

                if (odd)
                {
                    ++insideCount;
                }
            }

            // If more than half the directions report 'inside', then the
            // vote is in favor of 'inside'.
            return insideCount > directions.size() / 2;
        }

        // Test whether P is contained by the polyhedron. The query uses
        // ray-convex_polygon intersection queries. A ray-convex_polygon
        // intersection query can be implemented in many ways. In this
        // context, 'method' is one of three values:
        //   0 : Use a triangle fan and perform a ray-triangle intersection
        //       query for each triangle.
        //   1 : Find the point of intersection of the ray and the plane of
        //       the convex-polygon face. Test whether that point is inside
        //       the convex polygon using an O(N) test.
        //   2 : Find the point of intersection of the ray and the plane of
        //       the convex-polygon face. Test whether that point is inside
        //       the convex polygon using an O(log N) test.
        static bool InContainer(
            Vector3<T> const& P,
            std::vector<Vector3<T>> const& points,
            std::vector<ConvexFace> const& faces,
            std::vector<Vector3<T>> const& directions,
            std::size_t method)
        {
            GTL_ARGUMENT_ASSERT(
                0 <= method && method <= 2,
                "Invalid method.");

            if (method == 0)
            {
                TIQuery<T, Ray3<T>, Triangle3<T>> rtQuery{};
                Triangle3<T> triangle{};
                Ray3<T> ray{};
                ray.origin = P;

                std::size_t insideCount = 0;
                for (auto const& direction : directions)
                {
                    ray.direction = direction;

                    // Zero intersections to start with.
                    bool odd = false;

                    for (auto const& face : faces)
                    {
                        // Attempt to cull the triangle quickly.
                        if (FastNoIntersect(ray, face.plane))
                        {
                            continue;
                        }

                        // Process the triangles in a trifan of the face.
                        triangle.v[0] = points[face.indices[0]];
                        for (std::size_t k = 1, kp1 = 2; kp1 < face.indices.size(); k = kp1++)
                        {
                            triangle.v[1] = points[face.indices[k]];
                            triangle.v[2] = points[face.indices[kp1]];

                            auto rtOutput = rtQuery(ray, triangle);
                            if (rtOutput.intersect)
                            {
                                // The ray intersects the triangle.
                                odd = !odd;
                            }
                        }
                    }

                    if (odd)
                    {
                        ++insideCount;
                    }
                }

                // If more than half the directions report 'inside', then the
                // vote is in favor of 'inside'.
                return insideCount > directions.size() / 2;
            }
            else
            {
                FIQuery<T, Ray3<T>, Plane3<T>> rpQuery{};
                Ray3<T> ray{};
                ray.origin = P;

                std::size_t insideCount = 0;
                for (auto const& direction : directions)
                {
                    ray.direction = direction;

                    // Zero intersections to start with.
                    bool odd = false;

                    for (auto const& face : faces)
                    {
                        // Attempt to cull the triangle quickly.
                        if (FastNoIntersect(ray, face.plane))
                        {
                            continue;
                        }

                        // Compute the ray-plane intersection.
                        auto rpOutput = rpQuery(ray, face.plane);

                        // This assertion can be triggered when floating-point
                        // rounding errors occur.
                        GTL_RUNTIME_ASSERT(
                            rpOutput.intersect,
                            "Unexpected condition.");

                        // TODO. The orthonormal basis approach is subject to
                        // floating-point rounding errors. Use the projection
                        // of points onto the most aligned coordinate plane.

                        // Get a coordinate system for the plane. Use vertex
                        // V0 as the origin.
                        Vector3<T> const& V0 = points[face.indices[0]];
                        std::array<Vector3<T>, 3> basis{};
                        basis[0] = face.plane.normal;
                        ComputeOrthonormalBasis(1, basis[0], basis[1], basis[2]);

                        // Project the intersection onto the plane.
                        Vector3<T> diff = rpOutput.point - V0;
                        Vector2<T> projIntersect{ Dot(basis[1], diff), Dot(basis[2], diff) };

                        // Project the face vertices onto the plane of the face.
                        std::vector<Vector2<T>> projVertices(face.indices.size());
                        projVertices[0] = { C_<T>(0), C_<T>(0) };
                        for (std::size_t k = 1; k < face.indices.size(); ++k)
                        {
                            diff = points[face.indices[k]] - V0;
                            projVertices[k][0] = Dot(basis[1], diff);
                            projVertices[k][1] = Dot(basis[2], diff);
                        }

                        // Test whether the intersection point is in the convex
                        // polygon.
                        if (method == 1)
                        {
                            if (ContPolygon2<T>::InContainerConvexOrderN(
                                projVertices, projIntersect))
                            {
                                // The ray intersects the triangle.
                                odd = !odd;
                            }
                        }
                        else
                        {
                            if (ContPolygon2<T>::InContainerConvexOrderLogN(
                                projVertices, projIntersect))
                            {
                                // The ray intersects the triangle.
                                odd = !odd;
                            }
                        }
                    }

                    if (odd)
                    {
                        ++insideCount;
                    }
                }

                // If more than half the directions report 'inside', then the
                // vote is in favor of 'inside'.
                return insideCount > directions.size() / 2;
            }
        }

        // Test whether P is contained by the polyhedron. The query uses
        // ray-simple_polygon intersection queries. A ray-simple_polygon
        // intersection query can be implemented in a couple of ways. In
        // this context, 'method' is one of two values:
        //   0 : Iterate over the triangles of each face and perform a
        //       ray-triangle intersection query for each triangle. This
        //       requires that the SimpleFace::Triangles array be initialized
        //       for each face.
        //   1 : Find the point of intersection of the ray and the plane of
        //       simple-polygon face. Test whether that point is inside the
        //       simple polygon using an O(N) test. The SimpleFace::triangles
        //       array is not used for this method, so it does not have to be
        //       initialized for each face.
        static bool InContainer(
            Vector3<T> const& P,
            std::vector<Vector3<T>> const& points,
            std::vector<SimpleFace> const& faces,
            std::vector<Vector3<T>> const& directions,
            std::size_t method)
        {
            GTL_ARGUMENT_ASSERT(
                0 <= method && method <= 1,
                "Invalid method.");

            if (method == 0)
            {
                TIQuery<T, Ray3<T>, Triangle3<T>> rtQuery{};
                Triangle3<T> triangle{};
                Ray3<T> ray{};
                ray.origin = P;

                std::size_t insideCount = 0;
                for (auto const& direction : directions)
                {
                    ray.direction = direction;

                    // Zero intersections to start with.
                    bool odd = false;

                    for (auto const& face : faces)
                    {
                        // Attempt to cull the triangle quickly.
                        if (FastNoIntersect(ray, face.plane))
                        {
                            continue;
                        }

                        // The triangulation must exist to use it.
                        std::size_t numTriangles = face.triangles.size() / 3;
                        GTL_RUNTIME_ASSERT(
                            numTriangles > 0,
                            "A triangulation must exist.");

                        // Process the triangles in a triangulation of the
                        // face.
                        std::size_t const* index = face.triangles.data();
                        for (std::size_t t = 0; t < numTriangles; ++t)
                        {
                            // Get the triangle vertices.
                            for (std::size_t k = 0; k < 3; ++k)
                            {
                                triangle.v[k] = points[*index++];
                            }

                            // Test for intersection.
                            auto rtOutput = rtQuery(ray, triangle);
                            if (rtOutput.intersect)
                            {
                                // The ray intersects the triangle.
                                odd = !odd;
                            }
                        }
                    }

                    if (odd)
                    {
                        ++insideCount;
                    }
                }

                // If more than half the directions report 'inside', then the
                // vote is in favor of 'inside'.
                return insideCount > directions.size() / 2;
            }
            else
            {
                FIQuery<T, Ray3<T>, Plane3<T>> rpQuery{};
                Ray3<T> ray{};
                ray.origin = P;

                std::size_t insideCount = 0;
                for (auto const& direction : directions)
                {
                    ray.direction = direction;

                    // Zero intersections to start with.
                    bool odd = false;

                    for (auto const& face : faces)
                    {
                        // Attempt to cull the triangle quickly.
                        if (FastNoIntersect(ray, face.plane))
                        {
                            continue;
                        }

                        // Compute the ray-plane intersection.
                        auto rpOutput = rpQuery(ray, face.plane);

                        // This assertion can be triggered when floating-point
                        // rounding errors occur.
                        GTL_RUNTIME_ASSERT(
                            rpOutput.intersect,
                            "Unexpected condition.");

                        // TODO. The orthonormal basis approach is subject to
                        // floating-point rounding errors. Use the projection
                        // of points onto the most aligned coordinate plane.

                        // Get a coordinate system for the plane. Vertex V0
                        // is the origin.
                        Vector3<T> const& V0 = points[face.indices[0]];
                        std::array<Vector3<T>, 3> basis{};
                        basis[0] = face.plane.normal;
                        ComputeOrthonormalBasis(1, basis[0], basis[1], basis[2]);

                        // Project the intersection onto the plane.
                        Vector3<T> diff = rpOutput.point - V0;
                        Vector2<T> projIntersect{ Dot(basis[1], diff), Dot(basis[2], diff) };

                        // Project the face vertices onto the plane of the face.
                        std::vector<Vector2<T>> projVertices(face.indices.size());
                        projVertices[0] = { C_<T>(0), C_<T>(0) };
                        for (std::size_t k = 1; k < face.indices.size(); ++k)
                        {
                            diff = points[face.indices[k]] - V0;
                            projVertices[k][0] = Dot(basis[1], diff);
                            projVertices[k][1] = Dot(basis[2], diff);
                        }

                        // Test whether the intersection point is in the convex
                        // polygon.
                        if (ContPolygon2<T>::InContainer(projVertices, projIntersect))
                        {
                            // The ray intersects the triangle.
                            odd = !odd;
                        }
                    }

                    if (odd)
                    {
                        ++insideCount;
                    }
                }

                // If more than half the directions report 'inside', then the
                // vote is in favor of 'inside'.
                return insideCount > directions.size() / 2;
            }
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

    private:
        friend class UnitTestContPolyhedron3;
    };
}
