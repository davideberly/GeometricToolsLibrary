// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// The queries consider the cylinder to be a solid.

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Primitives/ND/Cylinder.h>
#include <GTL/Mathematics/Primitives/ND/Line.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class FIQuery<T, Line3<T>, Cylinder3<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0),
                parameter{ C_<T>(0), C_<T>(0) },
                point{}
            {
            }

            bool intersect;
            std::size_t numIntersections;
            std::array<T, 2> parameter;
            std::array<Vector3<T>, 2> point;
        };

        Output operator()(Line3<T> const& line, Cylinder3<T> const& cylinder)
        {
            Output output{};
            DoQuery(line.origin, line.direction, cylinder, output);
            if (output.intersect)
            {
                for (std::size_t i = 0; i < 2; ++i)
                {
                    output.point[i] = line.origin + output.parameter[i] * line.direction;
                }
            }
            return output;
        }

    protected:
        // The caller must ensure that on entry, 'output' is default
        // constructed as if there is no intersection. If an intersection is
        // found, the 'output' values will be modified accordingly.
        void DoQuery(Vector3<T> const& lineOrigin,
            Vector3<T> const& lineDirection, Cylinder3<T> const& cylinder,
            Output& output)
        {
            // Create a coordinate system for the cylinder. In this system,
            // the cylinder segment center C is the origin and the cylinder
            // axis direction W is the z-axis. U and V are the other
            // coordinate axis directions. If P = x*U+y*V+z*W, the cylinder
            // is x^2 + y^2 = r^2, where r is the cylinder radius. The end
            // caps are |z| = h/2, where h is the cylinder height.
            Vector3<T> W = cylinder.direction, U{}, V{};
            ComputeOrthonormalBasis(1, W, U, V);
            T halfHeight = C_<T>(1, 2) * cylinder.height;
            T rSqr = cylinder.radius * cylinder.radius;

            // Convert incoming line origin to capsule coordinates.
            Vector3<T> diff = lineOrigin - cylinder.center;
            Vector3<T> P{ Dot(U, diff), Dot(V, diff), Dot(W, diff) };

            // Get the z-value, in cylinder coordinates, of the incoming
            // line's unit-length direction.
            T dz = Dot(W, lineDirection);
            if (std::fabs(dz) == C_<T>(1))
            {
                // The line is parallel to the cylinder axis. Determine
                // whether the line intersects the cylinder end disks.
                T radialSqrDist = rSqr - P[0] * P[0] - P[1] * P[1];
                if (radialSqrDist >= C_<T>(0))
                {
                    // The line intersects the cylinder end disks.
                    output.intersect = true;
                    output.numIntersections = 2;
                    if (dz > C_<T>(0))
                    {
                        output.parameter[0] = -P[2] - halfHeight;
                        output.parameter[1] = -P[2] + halfHeight;
                    }
                    else
                    {
                        output.parameter[0] = P[2] - halfHeight;
                        output.parameter[1] = P[2] + halfHeight;
                    }
                }
                // else:  The line is outside the cylinder, no intersection.
                return;
            }

            // Convert the incoming line unit-length direction to cylinder
            // coordinates.
            Vector3<T> D{ Dot(U, lineDirection), Dot(V, lineDirection), dz };
            if (D[2] == C_<T>(0))
            {
                // The line is perpendicular to the cylinder axis.
                if (std::fabs(P[2]) <= halfHeight)
                {
                    // Test intersection of line P+t*D with infinite cylinder
                    // x^2+y^2 = r^2. This reduces to computing the roots of a
                    // quadratic equation. If P = (px,py,pz) and
                    // D = (dx,dy,dz), then the quadratic equation is
                    // (dx^2+dy^2)*t^2+2*(px*dx+py*dy)*t+(px^2+py^2-r^2) = 0.
                    T a0 = P[0] * P[0] + P[1] * P[1] - rSqr;
                    T a1 = P[0] * D[0] + P[1] * D[1];
                    T a2 = D[0] * D[0] + D[1] * D[1];
                    T discr = a1 * a1 - a0 * a2;
                    if (discr > C_<T>(0))
                    {
                        // The line intersects the cylinder in two places.
                        output.intersect = true;
                        output.numIntersections = 2;
                        T root = std::sqrt(discr);
                        output.parameter[0] = (-a1 - root) / a2;
                        output.parameter[1] = (-a1 + root) / a2;
                    }
                    else if (discr == C_<T>(0))
                    {
                        // The line is tangent to the cylinder.
                        output.intersect = true;
                        output.numIntersections = 1;
                        output.parameter[0] = -a1 / a2;
                        output.parameter[1] = output.parameter[0];
                    }
                    // else: The line does not intersect the cylinder.
                }
                // else: The line is outside the planes of the cylinder end
                // disks.
                return;
            }

            // At this time, the line direction is neither parallel nor
            // perpendicular to the cylinder axis. The line must
            // intersect both planes of the end disk, the intersection with
            // the cylinder being a segment. The t-interval of the segment
            // is [t0,t1].

            // Test for intersections with the planes of the end disks.
            T t0 = (-halfHeight - P[2]) / D[2];
            T xTmp = P[0] + t0 * D[0];
            T yTmp = P[1] + t0 * D[1];
            if (xTmp * xTmp + yTmp * yTmp <= rSqr)
            {
                // Plane intersection inside the bottom cylinder end disk.
                output.parameter[output.numIntersections++] = t0;
            }

            T t1 = (+halfHeight - P[2]) / D[2];
            xTmp = P[0] + t1 * D[0];
            yTmp = P[1] + t1 * D[1];
            if (xTmp * xTmp + yTmp * yTmp <= rSqr)
            {
                // Plane intersection inside the top cylinder end disk.
                output.parameter[output.numIntersections++] = t1;
            }

            if (output.numIntersections < 2)
            {
                // Test for intersection with the cylinder wall.
                T a0 = P[0] * P[0] + P[1] * P[1] - rSqr;
                T a1 = P[0] * D[0] + P[1] * D[1];
                T a2 = D[0] * D[0] + D[1] * D[1];
                T discr = a1 * a1 - a0 * a2;
                if (discr > C_<T>(0))
                {
                    T root = std::sqrt(discr);
                    T tValue = (-a1 - root) / a2;
                    if (t0 <= t1)
                    {
                        if (t0 <= tValue && tValue <= t1)
                        {
                            output.parameter[output.numIntersections++] = tValue;
                        }
                    }
                    else
                    {
                        if (t1 <= tValue && tValue <= t0)
                        {
                            output.parameter[output.numIntersections++] = tValue;
                        }
                    }

                    if (output.numIntersections < 2)
                    {
                        tValue = (-a1 + root) / a2;
                        if (t0 <= t1)
                        {
                            if (t0 <= tValue && tValue <= t1)
                            {
                                output.parameter[output.numIntersections++] = tValue;
                            }
                        }
                        else
                        {
                            if (t1 <= tValue && tValue <= t0)
                            {
                                output.parameter[output.numIntersections++] = tValue;
                            }
                        }
                    }
                    // else: Line intersects end disk and cylinder wall.
                }
                else if (discr == C_<T>(0))
                {
                    T tValue = -a1 / a2;
                    if (t0 <= t1)
                    {
                        if (t0 <= tValue && tValue <= t1)
                        {
                            output.parameter[output.numIntersections++] = tValue;
                        }
                    }
                    else
                    {
                        if (t1 <= tValue && tValue <= t0)
                        {
                            output.parameter[output.numIntersections++] = tValue;
                        }
                    }
                }
                // else: Line does not intersect cylinder wall.
            }
            // else: Line intersects both top and bottom cylinder end disks.

            if (output.numIntersections == 2)
            {
                output.intersect = true;
                if (output.parameter[0] > output.parameter[1])
                {
                    std::swap(output.parameter[0], output.parameter[1]);
                }
            }
            else if (output.numIntersections == 1)
            {
                output.intersect = true;
                output.parameter[1] = output.parameter[0];
            }
        }

    private:
        friend class UnitTestIntrLine3Cylinder3;
    };
}
