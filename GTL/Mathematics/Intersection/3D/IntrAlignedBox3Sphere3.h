// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.09.23

#pragma once

// The find-intersection query is based on the document
// https://www.geometrictools.com/Documentation/IntersectionMovingSphereBox.pdf
// and also uses the method of separating axes.
// https://www.geometrictools.com/Documentation/MethodOfSeparatingAxes.pdf

#include <GTL/Mathematics/Distance/ND/DistPointAlignedBox.h>
#include <GTL/Mathematics/Intersection/3D/IntrRay3AlignedBox3.h>
#include <GTL/Mathematics/Primitives/3D/Sphere3.h>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace gtl
{
    template <typename T>
    class TIQuery<T, AlignedBox3<T>, Sphere3<T>>
    {
    public:
        // The intersection query considers the box and sphere to be solids;
        // that is, the sphere object includes the region inside the spherical
        // boundary and the box object includes the region inside the cuboid
        // boundary. If the sphere object and box object object overlap, the
        // objects intersect.
        struct Output
        {
            Output()
                :
                intersect(false)
            {
            }

            bool intersect;
        };

        Output operator()(AlignedBox3<T> const& box, Sphere3<T> const& sphere)
        {
            DCPQuery<T, Vector3<T>, AlignedBox3<T>> pbQuery{};
            auto pbOutput = pbQuery(sphere.center, box);
            Output output{};
            output.intersect = (pbOutput.sqrDistance <= sphere.radius * sphere.radius);
            return output;
        }

    private:
        friend class UnitTestIntrAlignedBox3Sphere3;
    };

    template <typename T>
    class FIQuery<T, AlignedBox3<T>, Sphere3<T>>
    {
    public:
        // Currently, only a dynamic query is supported. A static query will
        // need to compute the intersection set of (solid) box and sphere.
        struct Output
        {
            Output()
                :
                intersectionType(0),
                contactTime(C_<T>(0)),
                contactPoint(Vector3<T>::Zero())
            {
            }

            // The cases are
            // 1. Objects initially overlapping. The contactPoint is only one
            //    of infinitely many points in the overlap.
            //      intersectionType = -1
            //      contactTime = 0
            //      contactPoint = sphere.center
            // 2. Objects initially separated but do not intersect later. The
            //      contactTime and contactPoint are invalid.
            //      intersectionType = 0
            //      contactTime = 0
            //      contactPoint = (0,0,0)
            // 3. Objects initially separated but intersect later.
            //      intersectionType = +1
            //      contactTime = first time T > 0
            //      contactPoint = corresponding first contact
            std::int32_t intersectionType;
            T contactTime;
            Vector3<T> contactPoint;

            // TODO: To support arbitrary precision for the contactTime,
            // return q0, q1 and q2 where contactTime = (q0 - sqrt(q1)) / q2.
            // The caller can compute contactTime to desired number of digits
            // of precision. These are valid when intersectionType is +1 but
            // are set to zero (invalid) in the other cases. Do the same for
            // the contactPoint.
        };

        Output operator()(AlignedBox3<T> const& box, Vector3<T> const& boxVelocity,
            Sphere3<T> const& sphere, Vector3<T> const& sphereVelocity)
        {
            Output output{};

            // Translate the sphere and box so that the box center becomes
            // the origin. Compute the velocity of the sphere relative to
            // the box.
            Vector3<T> boxCenter = C_<T>(1, 2) * (box.max + box.min);
            Vector3<T> extent = C_<T>(1, 2) * (box.max - box.min);
            Vector3<T> C = sphere.center - boxCenter;
            Vector3<T> V = sphereVelocity - boxVelocity;

            // Test for no-intersection that leads to an early exit. The test
            // is fast, using the method of separating axes.
            AlignedBox3<T> superBox{};
            for (std::size_t i = 0; i < 3; ++i)
            {
                superBox.max[i] = extent[i] + sphere.radius;
                superBox.min[i] = -superBox.max[i];
            }
            TIQuery<T, Ray3<T>, AlignedBox3<T>> rbQuery{};
            auto rbResult = rbQuery(Ray3<T>(C, V), superBox);
            if (rbResult.intersect)
            {
                DoQuery(extent, C, sphere.radius, V, output);

                // Translate the contact point back to the coordinate system
                // of the original sphere and box.
                output.contactPoint += boxCenter;
            }
            return output;
        }

    protected:
        // The query assumes the box is axis-aligned with center at the
        // origin. Callers need to convert the results back to the original
        // coordinate system of the query.
        void DoQuery(Vector3<T> const& K, Vector3<T> const& inC,
            T radius, Vector3<T> const& inV, Output& output)
        {
            // Change signs on components, if necessary, to transform C to the
            // first quadrant. Adjust the velocity accordingly.
            Vector3<T> C = inC, V = inV;
            std::array<T, 3> sign = { C_<T>(0), C_<T>(0), C_<T>(0) };
            for (std::size_t i = 0; i < 3; ++i)
            {
                if (C[i] >= C_<T>(0))
                {
                    sign[i] = C_<T>(1);
                }
                else
                {
                    C[i] = -C[i];
                    V[i] = -V[i];
                    sign[i] = C_<T>(-1);
                }
            }

            Vector3<T> delta = C - K;
            if (delta[2] <= radius)
            {
                if (delta[1] <= radius)
                {
                    if (delta[0] <= radius)
                    {
                        if (delta[2] <= C_<T>(0))
                        {
                            if (delta[1] <= C_<T>(0))
                            {
                                if (delta[0] <= C_<T>(0))
                                {
                                    InteriorOverlap(C, output);
                                }
                                else
                                {
                                    // x-face
                                    FaceOverlap(0, 1, 2, K, C, radius, delta, output);
                                }
                            }
                            else
                            {
                                if (delta[0] <= C_<T>(0))
                                {
                                    // y-face
                                    FaceOverlap(1, 2, 0, K, C, radius, delta, output);
                                }
                                else
                                {
                                    // xy-edge
                                    if (delta[0] * delta[0] + delta[1] * delta[1] <= radius * radius)
                                    {
                                        EdgeOverlap(0, 1, 2, K, C, radius, delta, output);
                                    }
                                    else
                                    {
                                        EdgeSeparated(0, 1, 2, K, C, radius, delta, V, output);
                                    }
                                }
                            }
                        }
                        else
                        {
                            if (delta[1] <= C_<T>(0))
                            {
                                if (delta[0] <= C_<T>(0))
                                {
                                    // z-face
                                    FaceOverlap(2, 0, 1, K, C, radius, delta, output);
                                }
                                else
                                {
                                    // xz-edge
                                    if (delta[0] * delta[0] + delta[2] * delta[2] <= radius * radius)
                                    {
                                        EdgeOverlap(2, 0, 1, K, C, radius, delta, output);
                                    }
                                    else
                                    {
                                        EdgeSeparated(2, 0, 1, K, C, radius, delta, V, output);
                                    }
                                }
                            }
                            else
                            {
                                if (delta[0] <= C_<T>(0))
                                {
                                    // yz-edge
                                    if (delta[1] * delta[1] + delta[2] * delta[2] <= radius * radius)
                                    {
                                        EdgeOverlap(1, 2, 0, K, C, radius, delta, output);
                                    }
                                    else
                                    {
                                        EdgeSeparated(1, 2, 0, K, C, radius, delta, V, output);
                                    }
                                }
                                else
                                {
                                    // xyz-vertex
                                    if (Dot(delta, delta) <= radius * radius)
                                    {
                                        VertexOverlap(K, radius, delta, output);
                                    }
                                    else
                                    {
                                        VertexSeparated(K, radius, delta, V, output);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        // x-face
                        FaceUnbounded(0, 1, 2, K, C, radius, delta, V, output);
                    }
                }
                else
                {
                    if (delta[0] <= radius)
                    {
                        // y-face
                        FaceUnbounded(1, 2, 0, K, C, radius, delta, V, output);
                    }
                    else
                    {
                        // xy-edge
                        EdgeUnbounded(0, 1, 2, K, C, radius, delta, V, output);
                    }
                }
            }
            else
            {
                if (delta[1] <= radius)
                {
                    if (delta[0] <= radius)
                    {
                        // z-face
                        FaceUnbounded(2, 0, 1, K, C, radius, delta, V, output);
                    }
                    else
                    {
                        // xz-edge
                        EdgeUnbounded(2, 0, 1, K, C, radius, delta, V, output);
                    }
                }
                else
                {
                    if (delta[0] <= radius)
                    {
                        // yz-edge
                        EdgeUnbounded(1, 2, 0, K, C, radius, delta, V, output);
                    }
                    else
                    {
                        // xyz-vertex
                        VertexUnbounded(K, C, radius, delta, V, output);
                    }
                }
            }

            if (output.intersectionType != 0)
            {
                // Translate back to the coordinate system of the
                // tranlated box and sphere.
                for (std::size_t i = 0; i < 3; ++i)
                {
                    if (sign[i] < C_<T>(0))
                    {
                        output.contactPoint[i] = -output.contactPoint[i];
                    }
                }
            }
        }

    private:
        void InteriorOverlap(Vector3<T> const& C, Output& output)
        {
            output.intersectionType = -1;
            output.contactTime = C_<T>(0);
            output.contactPoint = C;
        }

        void VertexOverlap(Vector3<T> const& K, T radius,
            Vector3<T> const& delta, Output& output)
        {
            output.intersectionType = (Dot(delta, delta) < radius * radius ? -1 : 1);
            output.contactTime = C_<T>(0);
            output.contactPoint = K;
        }

        void EdgeOverlap(std::size_t i0, std::size_t i1, std::size_t i2, Vector3<T> const& K,
            Vector3<T> const& C, T radius, Vector3<T> const& delta,
            Output& output)
        {
            output.intersectionType = (delta[i0] * delta[i0] + delta[i1] * delta[i1] < radius * radius ? -1 : 1);
            output.contactTime = C_<T>(0);
            output.contactPoint[i0] = K[i0];
            output.contactPoint[i1] = K[i1];
            output.contactPoint[i2] = C[i2];
        }

        void FaceOverlap(std::size_t i0, std::size_t i1, std::size_t i2, Vector3<T> const& K,
            Vector3<T> const& C, T radius, Vector3<T> const& delta,
            Output& output)
        {
            output.intersectionType = (delta[i0] < radius ? -1 : 1);
            output.contactTime = C_<T>(0);
            output.contactPoint[i0] = K[i0];
            output.contactPoint[i1] = C[i1];
            output.contactPoint[i2] = C[i2];
        }

        void VertexSeparated(Vector3<T> const& K, T radius,
            Vector3<T> const& delta, Vector3<T> const& V, Output& output)
        {
            if (V[0] < C_<T>(0) || V[1] < C_<T>(0) || V[2] < C_<T>(0))
            {
                DoQueryRayRoundedVertex(K, radius, delta, V, output);
            }
        }

        void EdgeSeparated(std::size_t i0, std::size_t i1, std::size_t i2, Vector3<T> const& K,
            Vector3<T> const& C, T radius, Vector3<T> const& delta,
            Vector3<T> const& V, Output& output)
        {
            if (V[i0] < C_<T>(0) || V[i1] < C_<T>(0))
            {
                DoQueryRayRoundedEdge(i0, i1, i2, K, C, radius, delta, V, output);
            }
        }

        void VertexUnbounded(Vector3<T> const& K, Vector3<T> const& C, T radius,
            Vector3<T> const& delta, Vector3<T> const& V, Output& output)
        {
            if (V[0] < C_<T>(0) && V[1] < C_<T>(0) && V[2] < C_<T>(0))
            {
                // Determine the face of the rounded box that is intersected
                // by the ray C+T*V.
                T tmax = (radius - delta[0]) / V[0];
                std::size_t j0 = 0;
                T temp = (radius - delta[1]) / V[1];
                if (temp > tmax)
                {
                    tmax = temp;
                    j0 = 1;
                }
                temp = (radius - delta[2]) / V[2];
                if (temp > tmax)
                {
                    tmax = temp;
                    j0 = 2;
                }

                // The j0-rounded face is the candidate for intersection.
                std::size_t j1 = (j0 + 1) % 3;
                std::size_t j2 = (j1 + 1) % 3;
                DoQueryRayRoundedFace(j0, j1, j2, K, C, radius, delta, V, output);
            }
        }

        void EdgeUnbounded(std::size_t i0, std::size_t i1, std::size_t, Vector3<T> const& K,
            Vector3<T> const& C, T radius, Vector3<T> const& delta,
            Vector3<T> const& V, Output& output)
        {
            if (V[i0] < C_<T>(0) && V[i1] < C_<T>(0))
            {
                // Determine the face of the rounded box that is intersected
                // by the ray C+T*V.
                T tmax = (radius - delta[i0]) / V[i0];
                std::size_t j0 = i0;
                T temp = (radius - delta[i1]) / V[i1];
                if (temp > tmax)
                {
                    tmax = temp;
                    j0 = i1;
                }

                // The j0-rounded face is the candidate for intersection.
                std::size_t j1 = (j0 + 1) % 3;
                std::size_t j2 = (j1 + 1) % 3;
                DoQueryRayRoundedFace(j0, j1, j2, K, C, radius, delta, V, output);
            }
        }

        void FaceUnbounded(std::size_t i0, std::size_t i1, std::size_t i2, Vector3<T> const& K,
            Vector3<T> const& C, T radius, Vector3<T> const& delta,
            Vector3<T> const& V, Output& output)
        {
            if (V[i0] < C_<T>(0))
            {
                DoQueryRayRoundedFace(i0, i1, i2, K, C, radius, delta, V, output);
            }
        }

        void DoQueryRayRoundedVertex(Vector3<T> const& K, T radius,
            Vector3<T> const& delta, Vector3<T> const& V, Output& output)
        {
            T a1 = Dot(V, delta);
            if (a1 < C_<T>(0))
            {
                // The caller must ensure that a0 > 0 and a2 > 0.
                T a0 = Dot(delta, delta) - radius * radius;
                T a2 = Dot(V, V);
                T adiscr = a1 * a1 - a2 * a0;
                if (adiscr >= C_<T>(0))
                {
                    // The ray intersects the rounded vertex, so the sphere-box
                    // contact point is the vertex.
                    output.intersectionType = 1;
                    output.contactTime = -(a1 + std::sqrt(adiscr)) / a2;
                    output.contactPoint = K;
                }
            }
        }

        void DoQueryRayRoundedEdge(std::size_t i0, std::size_t i1, std::size_t i2, Vector3<T> const& K,
            Vector3<T> const& C, T radius, Vector3<T> const& delta,
            Vector3<T> const& V, Output& output)
        {
            T b1 = V[i0] * delta[i0] + V[i1] * delta[i1];
            if (b1 < C_<T>(0))
            {
                // The caller must ensure that b0 > 0 and b2 > 0.
                T b0 = delta[i0] * delta[i0] + delta[i1] * delta[i1] - radius * radius;
                T b2 = V[i0] * V[i0] + V[i1] * V[i1];
                T bdiscr = b1 * b1 - b2 * b0;
                if (bdiscr >= C_<T>(0))
                {
                    T tmax = -(b1 + std::sqrt(bdiscr)) / b2;
                    T p2 = C[i2] + tmax * V[i2];
                    if (-K[i2] <= p2)
                    {
                        if (p2 <= K[i2])
                        {
                            // The ray intersects the finite cylinder of the
                            // rounded edge, so the sphere-box contact point
                            // is on the corresponding box edge.
                            output.intersectionType = 1;
                            output.contactTime = tmax;
                            output.contactPoint[i0] = K[i0];
                            output.contactPoint[i1] = K[i1];
                            output.contactPoint[i2] = p2;
                        }
                        else
                        {
                            // The ray intersects the infinite cylinder but
                            // not the finite cylinder of the rounded edge.
                            // It is possible the ray intersects the rounded
                            // vertex for K.
                            DoQueryRayRoundedVertex(K, radius, delta, V, output);
                        }
                    }
                    else
                    {
                        // The ray intersects the infinite cylinder but
                        // not the finite cylinder of the rounded edge.
                        // It is possible the ray intersects the rounded
                        // vertex for otherK.
                        Vector3<T> otherK, otherDelta;
                        otherK[i0] = K[i0];
                        otherK[i1] = K[i1];
                        otherK[i2] = -K[i2];
                        otherDelta[i0] = C[i0] - otherK[i0];
                        otherDelta[i1] = C[i1] - otherK[i1];
                        otherDelta[i2] = C[i2] - otherK[i2];
                        DoQueryRayRoundedVertex(otherK, radius, otherDelta, V, output);
                    }
                }
            }
        }

        void DoQueryRayRoundedFace(std::size_t i0, std::size_t i1, std::size_t i2, Vector3<T> const& K,
            Vector3<T> const& C, T radius, Vector3<T> const& delta,
            Vector3<T> const& V, Output& output)
        {
            Vector3<T> otherK, otherDelta;

            T tmax = (radius - delta[i0]) / V[i0];
            T p1 = C[i1] + tmax * V[i1];
            T p2 = C[i2] + tmax * V[i2];

            if (p1 < -K[i1])
            {
                // The ray potentially intersects the rounded (i0,i1)-edge
                // whose top-most vertex is otherK.
                otherK[i0] = K[i0];
                otherK[i1] = -K[i1];
                otherK[i2] = K[i2];
                otherDelta[i0] = C[i0] - otherK[i0];
                otherDelta[i1] = C[i1] - otherK[i1];
                otherDelta[i2] = C[i2] - otherK[i2];
                DoQueryRayRoundedEdge(i0, i1, i2, otherK, C, radius, otherDelta, V, output);
                if (output.intersectionType == 0)
                {
                    if (p2 < -K[i2])
                    {
                        // The ray potentially intersects the rounded
                        // (i2,i0)-edge whose right-most vertex is otherK.
                        otherK[i0] = K[i0];
                        otherK[i1] = K[i1];
                        otherK[i2] = -K[i2];
                        otherDelta[i0] = C[i0] - otherK[i0];
                        otherDelta[i1] = C[i1] - otherK[i1];
                        otherDelta[i2] = C[i2] - otherK[i2];
                        DoQueryRayRoundedEdge(i2, i0, i1, otherK, C, radius, otherDelta, V, output);
                    }
                    else if (p2 > K[i2])
                    {
                        // The ray potentially intersects the rounded
                        // (i2,i0)-edge whose right-most vertex is K.
                        DoQueryRayRoundedEdge(i2, i0, i1, K, C, radius, delta, V, output);
                    }
                }
            }
            else if (p1 <= K[i1])
            {
                if (p2 < -K[i2])
                {
                    // The ray potentially intersects the rounded
                    // (i2,i0)-edge whose right-most vertex is otherK.
                    otherK[i0] = K[i0];
                    otherK[i1] = K[i1];
                    otherK[i2] = -K[i2];
                    otherDelta[i0] = C[i0] - otherK[i0];
                    otherDelta[i1] = C[i1] - otherK[i1];
                    otherDelta[i2] = C[i2] - otherK[i2];
                    DoQueryRayRoundedEdge(i2, i0, i1, otherK, C, radius, otherDelta, V, output);
                }
                else if (p2 <= K[i2])
                {
                    // The ray intersects the i0-face of the rounded box, so
                    // the sphere-box contact point is on the corresponding
                    // box face.
                    output.intersectionType = 1;
                    output.contactTime = tmax;
                    output.contactPoint[i0] = K[i0];
                    output.contactPoint[i1] = p1;
                    output.contactPoint[i2] = p2;
                }
                else  // p2 > K[i2]
                {
                    // The ray potentially intersects the rounded
                    // (i2,i0)-edge whose right-most vertex is K.
                    DoQueryRayRoundedEdge(i2, i0, i1, K, C, radius, delta, V, output);
                }
            }
            else // p1 > K[i1]
            {
                // The ray potentially intersects the rounded (i0,i1)-edge
                // whose top-most vertex is K.
                DoQueryRayRoundedEdge(i0, i1, i2, K, C, radius, delta, V, output);
                if (output.intersectionType == 0)
                {
                    if (p2 < -K[i2])
                    {
                        // The ray potentially intersects the rounded
                        // (i2,i0)-edge whose right-most vertex is otherK.
                        otherK[i0] = K[i0];
                        otherK[i1] = K[i1];
                        otherK[i2] = -K[i2];
                        otherDelta[i0] = C[i0] - otherK[i0];
                        otherDelta[i1] = C[i1] - otherK[i1];
                        otherDelta[i2] = C[i2] - otherK[i2];
                        DoQueryRayRoundedEdge(i2, i0, i1, otherK, C, radius, otherDelta, V, output);
                    }
                    else if (p2 > K[i2])
                    {
                        // The ray potentially intersects the rounded
                        // (i2,i0)-edge whose right-most vertex is K.
                        DoQueryRayRoundedEdge(i2, i0, i1, K, C, radius, delta, V, output);
                    }
                }
            }
        }

    private:
        friend class UnitTestIntrAlignedBox3Sphere3;
    };
}
