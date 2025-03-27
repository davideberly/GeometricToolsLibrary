// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

#include <GTL/Mathematics/Intersection/2D/IntrSegment2Triangle2.h>
#include <GTL/Mathematics/Projection/ProjectPointsToCoordinatePlane.h>
#include <GTL/Mathematics/Primitives/ND/Segment.h>
#include <GTL/Mathematics/Primitives/ND/Triangle.h>
#include <array>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Segment3<T>, Triangle3<T>>
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

        Output operator()(Segment3<T> const& segment, Triangle3<T> const& triangle)
        {
            Output output{};

            Vector3<T> const& segOrigin = segment.p[0];
            Vector3<T> segDirection = segment.p[1] - segment.p[0];

            // Compute the offset origin, edges, and normal.
            Vector3<T> diff = segOrigin - triangle.v[0];
            Vector3<T> edge1 = triangle.v[1] - triangle.v[0];
            Vector3<T> edge2 = triangle.v[2] - triangle.v[0];
            Vector3<T> normal = Cross(edge1, edge2);

            // Solve Q + t*D = b1*E1 + b2*E2 (Q = diff, D = segment direction,
            // E1 = edge1, E2 = edge2, N = Cross(E1,E2)) by
            //   |Dot(D,N)|*b1 = sign(Dot(D,N))*Dot(D,Cross(Q,E2))
            //   |Dot(D,N)|*b2 = sign(Dot(D,N))*Dot(D,Cross(E1,Q))
            //   |Dot(D,N)|*t = -sign(Dot(D,N))*Dot(Q,N)
            T DdN = Dot(segDirection, normal);
            if (DdN != C_<T>(0))
            {
                // The segment and triangle are not parallel.
                T sign;
                if (DdN > C_<T>(0))
                {
                    sign = C_<T>(1);
                }
                else
                {
                    sign = -C_<T>(1);
                    DdN = -DdN;
                }

                T DdQxE2 = sign * DotCross(segDirection, diff, edge2);
                if (DdQxE2 >= C_<T>(0))
                {
                    T DdE1xQ = sign * DotCross(segDirection, edge1, diff);
                    if (DdE1xQ >= C_<T>(0))
                    {
                        if (DdQxE2 + DdE1xQ <= DdN)
                        {
                            // The line containing the segment intersects the
                            // triangle. Determine whether the segment does.
                            T QdN = -sign * Dot(diff, normal);
                            if (C_<T>(0) <= QdN && QdN <= DdN)
                            {
                                // The segment intersects the triangle.
                                output.intersect = true;
                            }
                            // else: t not in [0,1], no intersection
                        }
                        // else: b1+b2 > 1, no intersection
                    }
                    // else: b2 < 0, no intersection
                }
                // else: b1 < 0, no intersection
            }
            else
            {
                // The segment and triangle are parallel.
                T NdDiff = Dot(normal, diff);
                if (NdDiff == C_<T>(0))
                {
                    // The segment and triangle are coplanar. Project the
                    // objects onto a coordinate plane to convert the problem
                    // to one in 2D.
                    Segment2<T> projSegment{};
                    Triangle2<T> projTriangle{};
                    std::size_t maxIndex = 0;
                    std::array<std::size_t, 3> permute{ 0, 0, 0 };
                    ProjectPointsToCoordinatePlane<T>::Select(normal, maxIndex, permute);
                    ProjectPointsToCoordinatePlane<T>::Project(1, &diff,
                        permute, &projSegment.p[0]);
                    diff = segment.p[1] - triangle.v[0];
                    ProjectPointsToCoordinatePlane<T>::Project(1, &diff,
                        permute, &projSegment.p[1]);
                    projTriangle.v[0] = { C_<T>(0), C_<T>(0) };
                    ProjectPointsToCoordinatePlane<T>::Project(1, &edge1,
                        permute, &projTriangle.v[1]);
                    ProjectPointsToCoordinatePlane<T>::Project(1, &edge2,
                        permute, &projTriangle.v[2]);

                    TIQuery<T, Segment2<T>, Triangle2<T>> stQuery{};
                    auto stOutput = stQuery(projSegment, projTriangle);
                    output.intersect = stOutput.intersect;
                }
                // else: The segment and triangle are parallel but not
                // coplanar, so they do not intersect.
            }

            return output;
        }

    private:
        friend class UnitTestIntrSegment3Triangle3;
    };

    template <typename T>
    class FIQuery<T, Segment3<T>, Triangle3<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0),
                parameter{ C_<T>(0), C_<T>(0) },
                point{},
                barycentric{ {
                    { C_<T>(0), C_<T>(0), C_<T>(0) },
                    { C_<T>(0), C_<T>(0), C_<T>(0) } } }
            {
            }

            // The number of intersections is
            //   0: The segment and triangle do not intersect.
            //   1: The segment and the triangle intersect in a point.
            //   2: The segment and triangle are coplanar and intersect in
            //      a segment.
            // The arrays parameter[], point[] and barycentric[] have
            // numIntersections elements. When the intersection set is a
            // segment, these arrays represent the segment endpoints. The
            // parameter[] are relative to the segment and the barycentric[]
            // are relative to the triangle.
            bool intersect;
            std::size_t numIntersections;
            std::array<T, 2> parameter;
            std::array<Vector3<T>, 2> point;
            std::array<std::array<T, 3>, 2> barycentric;
        };

        Output operator()(Segment3<T> const& segment, Triangle3<T> const& triangle)
        {
            Output output{};

            Vector3<T> const& segOrigin = segment.p[0];
            Vector3<T> segDirection = segment.p[1] - segment.p[0];

            // Compute the offset origin, edges, and normal.
            Vector3<T> diff = segOrigin - triangle.v[0];
            Vector3<T> edge1 = triangle.v[1] - triangle.v[0];
            Vector3<T> edge2 = triangle.v[2] - triangle.v[0];
            Vector3<T> normal = Cross(edge1, edge2);

            // Solve Q + t*D = b1*E1 + b2*E2 (Q = diff, D = segment direction,
            // E1 = edge1, E2 = edge2, N = Cross(E1,E2)) by
            //   |Dot(D,N)|*b1 = sign(Dot(D,N))*Dot(D,Cross(Q,E2))
            //   |Dot(D,N)|*b2 = sign(Dot(D,N))*Dot(D,Cross(E1,Q))
            //   |Dot(D,N)|*t = -sign(Dot(D,N))*Dot(Q,N)
            T DdN = Dot(segDirection, normal);
            if (DdN != C_<T>(0))
            {
                // The segment and triangle are not parallel.
                T sign;
                if (DdN > C_<T>(0))
                {
                    sign = C_<T>(1);
                }
                else
                {
                    sign = -C_<T>(1);
                    DdN = -DdN;
                }

                T DdQxE2 = sign * DotCross(segDirection, diff, edge2);
                if (DdQxE2 >= C_<T>(0))
                {
                    T DdE1xQ = sign * DotCross(segDirection, edge1, diff);
                    if (DdE1xQ >= C_<T>(0))
                    {
                        if (DdQxE2 + DdE1xQ <= DdN)
                        {
                            // The line containing the segment intersects the
                            // triangle. Determine whether the segment does.
                            T QdN = -sign * Dot(diff, normal);
                            if (C_<T>(0) <= QdN && QdN <= DdN)
                            {
                                // The segment intersects the triangle.
                                output.intersect = true;
                                output.numIntersections = 1;

                                output.parameter[0] = QdN / DdN;
                                output.barycentric[0][1] = DdQxE2 / DdN;
                                output.barycentric[0][2] = DdE1xQ / DdN;
                                output.barycentric[0][0] =
                                    C_<T>(1) - output.barycentric[0][1] - output.barycentric[0][2];
                                output.point[0] = segOrigin + output.parameter[0] * segDirection;

                                output.parameter[1] = output.parameter[0];
                                output.barycentric[1] = output.barycentric[0];
                                output.point[1] = output.point[0];
                            }
                            // else: |t| > extent, no intersection
                        }
                        // else: b1+b2 > 1, no intersection
                    }
                    // else: b2 < 0, no intersection
                }
                // else: b1 < 0, no intersection
            }
            else
            {
                // The segment and triangle are parallel.
                T NdDiff = Dot(normal, diff);
                if (NdDiff == C_<T>(0))
                {
                    // The segment and triangle are coplanar. Project the
                    // objects onto a coordinate plane to convert the problem
                    // to one in 2D.
                    Segment2<T> projSegment{};
                    Triangle2<T> projTriangle{};
                    std::size_t maxIndex = 0;
                    std::array<std::size_t, 3> permute{ 0, 0, 0 };
                    ProjectPointsToCoordinatePlane<T>::Select(normal, maxIndex, permute);
                    ProjectPointsToCoordinatePlane<T>::Project(1, &diff,
                        permute, &projSegment.p[0]);
                    diff = segment.p[1] - triangle.v[0];
                    ProjectPointsToCoordinatePlane<T>::Project(1, &diff,
                        permute, &projSegment.p[1]);
                    projTriangle.v[0] = { C_<T>(0), C_<T>(0) };
                    ProjectPointsToCoordinatePlane<T>::Project(1, &edge1,
                        permute, &projTriangle.v[1]);
                    ProjectPointsToCoordinatePlane<T>::Project(1, &edge2,
                        permute, &projTriangle.v[2]);

                    FIQuery<T, Segment2<T>, Triangle2<T>> stQuery{};
                    auto stOutput = stQuery(projSegment, projTriangle);
                    if (stOutput.intersect)
                    {
                        output.intersect = true;
                        output.numIntersections = stOutput.numIntersections;
                    }
                    // else: The segment and triangle do not intersect in
                    // their common plane.
                }
                // else: The segment and triangle are parallel but not
                // coplanar, so they do not intersect.
            }

            return output;
        }

    private:
        friend class UnitTestIntrSegment3Triangle3;
    };
}
