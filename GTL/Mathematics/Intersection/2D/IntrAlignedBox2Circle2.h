// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

// The queries consider the box and circle to be solids. The circle object
// includes the region inside the circular boundary and the box object
// includes the region inside the rectangular boundary. If the circle object
// and box object overlap, the objects intersect. The find-intersection query
// is based on the document
// https://www.geometrictools.com/Documentation/IntersectionMovingCircleRectangle.pdf

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Primitives/ND/AlignedBox.h>
#include <GTL/Mathematics/Primitives/2D/Circle2.h>
#include <GTL/Mathematics/Distance/ND/DistPointAlignedBox.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace gtl
{
    template <typename T>
    class TIQuery<T, AlignedBox2<T>, Circle2<T>>
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

        Output operator()(AlignedBox2<T> const& box, Circle2<T> const& circle)
        {
            DCPQuery<T, Vector2<T>, AlignedBox2<T>> pbQuery{};
            auto pbOutput = pbQuery(circle.center, box);
            Output output{};
            output.intersect = (pbOutput.sqrDistance <= circle.radius * circle.radius);
            return output;
        }

    private:
        friend class UnitTestIntrAlignedBox2Circle2;
    };

    template <typename T>
    class FIQuery<T, AlignedBox2<T>, Circle2<T>>
    {
    public:
        // Currently, only a dynamic query is supported. A static query needs
        // to compute the intersection set of (solid) box and (solid) circle.
        struct Output
        {
            Output()
                :
                intersectionType(0),
                contactTime(C_<T>(0)),
                contactPoint(Vector2<T>::Zero())
            {
            }

            // The cases are
            // 1. Objects initially overlapping.  The contactPoint is only one
            //    of infinitely many points in the overlap.
            //      intersectionType = -1
            //      contactTime = 0
            //      contactPoint = circle.center
            // 2. Objects initially separated but do not intersect later.  The
            //      contactTime and contactPoint are invalid.
            //      intersectionType = 0
            //      contactTime = 0
            //      contactPoint = (0,0)
            // 3. Objects initially separated but intersect later.
            //      intersectionType = +1
            //      contactTime = first time T > 0
            //      contactPoint = corresponding first contact
            std::int32_t intersectionType;
            T contactTime;
            Vector2<T> contactPoint;
        };

        Output operator()(AlignedBox2<T> const& box, Vector2<T> const& boxVelocity,
            Circle2<T> const& circle, Vector2<T> const& circleVelocity)
        {
            Output output{};

            // Translate the circle and box so that the box center becomes the
            // origin. Compute the velocity of the circle relative to the box.
            Vector2<T> boxCenter = C_<T>(1, 2) * (box.max + box.min);
            Vector2<T> extent = C_<T>(1, 2) * (box.max - box.min);
            Vector2<T> C = circle.center - boxCenter;
            Vector2<T> V = circleVelocity - boxVelocity;

            // Change signs on components, if necessary, to transform C to the
            // first quadrant. Adjust the velocity accordingly.
            std::array<T, 2> sign = { C_<T>(0), C_<T>(0) };
            for (std::size_t i = 0; i < 2; ++i)
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

            DoQuery(extent, C, circle.radius, V, output);

            if (output.intersectionType != 0)
            {
                // Translate back to the original coordinate system.
                for (std::size_t i = 0; i < 2; ++i)
                {
                    if (sign[i] < C_<T>(0))
                    {
                        output.contactPoint[i] = -output.contactPoint[i];
                    }
                }

                output.contactPoint += boxCenter;
            }
            return output;
        }

    protected:
        void DoQuery(Vector2<T> const& K, Vector2<T> const& C,
            T const& radius, Vector2<T> const& V, Output& output)
        {
            Vector2<T> delta = C - K;
            if (delta[1] <= radius)
            {
                if (delta[0] <= radius)
                {
                    if (delta[1] <= C_<T>(0))
                    {
                        if (delta[0] <= C_<T>(0))
                        {
                            InteriorOverlap(C, output);
                        }
                        else
                        {
                            EdgeOverlap(0, 1, K, C, delta, radius, output);
                        }
                    }
                    else
                    {
                        if (delta[0] <= C_<T>(0))
                        {
                            EdgeOverlap(1, 0, K, C, delta, radius, output);
                        }
                        else
                        {
                            if (Dot(delta, delta) <= radius * radius)
                            {
                                VertexOverlap(K, delta, radius, output);
                            }
                            else
                            {
                                VertexSeparated(K, delta, V, radius, output);
                            }
                        }

                    }
                }
                else
                {
                    EdgeUnbounded(0, 1, K, C, radius, delta, V, output);
                }
            }
            else
            {
                if (delta[0] <= radius)
                {
                    EdgeUnbounded(1, 0, K, C, radius, delta, V, output);
                }
                else
                {
                    VertexUnbounded(K, C, radius, delta, V, output);
                }
            }
        }

    private:
        void InteriorOverlap(Vector2<T> const& C, Output& output)
        {
            output.intersectionType = -1;
            output.contactTime = C_<T>(0);
            output.contactPoint = C;
        }

        void EdgeOverlap(std::size_t i0, std::size_t i1, Vector2<T> const& K, Vector2<T> const& C,
            Vector2<T> const& delta, T radius, Output& output)
        {
            output.intersectionType = (delta[i0] < radius ? -1 : 1);
            output.contactTime = C_<T>(0);
            output.contactPoint[i0] = K[i0];
            output.contactPoint[i1] = C[i1];
        }

        void VertexOverlap(Vector2<T> const& K0, Vector2<T> const& delta,
            T const& radius, Output& output)
        {
            T sqrDistance = delta[0] * delta[0] + delta[1] * delta[1];
            T sqrRadius = radius * radius;
            output.intersectionType = (sqrDistance < sqrRadius ? -1 : 1);
            output.contactTime = C_<T>(0);
            output.contactPoint = K0;
        }

        void VertexSeparated(Vector2<T> const& K0, Vector2<T> const& delta0,
            Vector2<T> const& V, T const& radius, Output& output)
        {
            T q0 = -Dot(V, delta0);
            if (q0 > C_<T>(0))
            {
                T dotVPerpD0 = Dot(V, Perp(delta0));
                T q2 = Dot(V, V);
                T q1 = radius * radius * q2 - dotVPerpD0 * dotVPerpD0;
                if (q1 >= C_<T>(0))
                {
                    IntersectsVertex(0, 1, K0, q0, q1, q2, output);
                }
            }
        }

        void EdgeUnbounded(std::size_t i0, std::size_t i1, Vector2<T> const& K0, Vector2<T> const& C,
            T const& radius, Vector2<T> const& delta0, Vector2<T> const& V, Output& output)
        {
            if (V[i0] < C_<T>(0))
            {
                T dotVPerpD0 = V[i0] * delta0[i1] - V[i1] * delta0[i0];
                if (radius * V[i1] + dotVPerpD0 >= C_<T>(0))
                {
                    Vector2<T> K1{}, delta1{};
                    K1[i0] = K0[i0];
                    K1[i1] = -K0[i1];
                    delta1[i0] = C[i0] - K1[i0];
                    delta1[i1] = C[i1] - K1[i1];
                    T dotVPerpD1 = V[i0] * delta1[i1] - V[i1] * delta1[i0];
                    if (radius * V[i1] + dotVPerpD1 <= C_<T>(0))
                    {
                        IntersectsEdge(i0, i1, K0, C, radius, V, output);
                    }
                    else
                    {
                        T q2 = Dot(V, V);
                        T q1 = radius * radius * q2 - dotVPerpD1 * dotVPerpD1;
                        if (q1 >= C_<T>(0))
                        {
                            T q0 = -(V[i0] * delta1[i0] + V[i1] * delta1[i1]);
                            IntersectsVertex(i0, i1, K1, q0, q1, q2, output);
                        }
                    }
                }
                else
                {
                    T q2 = Dot(V, V);
                    T q1 = radius * radius * q2 - dotVPerpD0 * dotVPerpD0;
                    if (q1 >= C_<T>(0))
                    {
                        T q0 = -(V[i0] * delta0[i0] + V[i1] * delta0[i1]);
                        IntersectsVertex(i0, i1, K0, q0, q1, q2, output);
                    }
                }
            }
        }

        void VertexUnbounded(Vector2<T> const& K0, Vector2<T> const& C, T const& radius,
            Vector2<T> const& delta0, Vector2<T> const& V, Output& output)
        {
            if (V[0] < C_<T>(0) && V[1] < C_<T>(0))
            {
                T dotVPerpD0 = Dot(V, Perp(delta0));
                if (radius * V[0] - dotVPerpD0 <= C_<T>(0))
                {
                    if (-radius * V[1] - dotVPerpD0 >= C_<T>(0))
                    {
                        T q2 = Dot(V, V);
                        T q1 = radius * radius * q2 - dotVPerpD0 * dotVPerpD0;
                        T q0 = -Dot(V, delta0);
                        IntersectsVertex(0, 1, K0, q0, q1, q2, output);
                    }
                    else
                    {
                        Vector2<T> K1{ K0[0], -K0[1] };
                        Vector2<T> delta1 = C - K1;
                        T dotVPerpD1 = Dot(V, Perp(delta1));
                        if (-radius * V[1] - dotVPerpD1 >= C_<T>(0))
                        {
                            IntersectsEdge(0, 1, K0, C, radius, V, output);
                        }
                        else
                        {
                            T q2 = Dot(V, V);
                            T q1 = radius * radius * q2 - dotVPerpD1 * dotVPerpD1;
                            if (q1 >= C_<T>(0))
                            {
                                T q0 = -Dot(V, delta1);
                                IntersectsVertex(0, 1, K1, q0, q1, q2, output);
                            }
                        }
                    }
                }
                else
                {
                    Vector2<T> K2{ -K0[0], K0[1] };
                    Vector2<T> delta2 = C - K2;
                    T dotVPerpD2 = Dot(V, Perp(delta2));
                    if (radius * V[0] - dotVPerpD2 <= C_<T>(0))
                    {
                        IntersectsEdge(1, 0, K0, C, radius, V, output);
                    }
                    else
                    {
                        T q2 = Dot(V, V);
                        T q1 = radius * radius * q2 - dotVPerpD2 * dotVPerpD2;
                        if (q1 >= C_<T>(0))
                        {
                            T q0 = -Dot(V, delta2);
                            IntersectsVertex(1, 0, K2, q0, q1, q2, output);
                        }
                    }
                }
            }
        }

        void IntersectsVertex(std::size_t i0, std::size_t i1, Vector2<T> const& K,
            T const& q0, T const& q1, T const& q2, Output& output)
        {
            output.intersectionType = +1;
            output.contactTime = (q0 - std::sqrt(q1)) / q2;
            output.contactPoint[i0] = K[i0];
            output.contactPoint[i1] = K[i1];
        }

        void IntersectsEdge(std::size_t i0, std::size_t i1, Vector2<T> const& K0, Vector2<T> const& C,
            T const& radius, Vector2<T> const& V, Output& output)
        {
            output.intersectionType = +1;
            output.contactTime = (K0[i0] + radius - C[i0]) / V[i0];
            output.contactPoint[i0] = K0[i0];
            output.contactPoint[i1] = C[i1] + output.contactTime * V[i1];
        }

    private:
        friend class UnitTestIntrAlignedBox2Circle2;
    };
}
