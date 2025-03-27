// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Primitives/3D/Plane3.h>
#include <GTL/Mathematics/Primitives/ND/Line.h>
#include <cmath>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Plane3<T>, Plane3<T>>
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

        Output operator()(Plane3<T> const& plane0, Plane3<T> const& plane1)
        {
            // If Cross(N0,N1) is zero, then either planes are parallel and
            // separated or the same plane. In both cases, 'false' is
            // returned; otherwise, the planes intersect. To avoid subtle
            // differences in reporting between Test() and Find(), the same
            // parallel test is used. Mathematically,
            //   |Cross(N0,N1)|^2 = Dot(N0,N0)*Dot(N1,N1)-Dot(N0,N1)^2
            //                    = 1 - Dot(N0,N1)^2
            // The last equality is true since planes are required to have
            // unit-length normal vectors. The test |Cross(N0,N1)| = 0 is the
            // same as |Dot(N0,N1)| = 1.

            Output output{};

            T dot = Dot(plane0.normal, plane1.normal);
            if (std::fabs(dot) < C_<T>(1))
            {
                output.intersect = true;
                return output;
            }

            // The planes are parallel. Check whether they are coplanar.
            T cDiff;
            if (dot >= C_<T>(0))
            {
                // Normals are in same direction, need to look at c0-c1.
                cDiff = plane0.constant - plane1.constant;
            }
            else
            {
                // Normals are in opposite directions, need to look at c0+c1.
                cDiff = plane0.constant + plane1.constant;
            }

            output.intersect = (std::fabs(cDiff) == C_<T>(0));
            return output;
        }

    private:
        friend class UnitTestIntrPlane3Plane3;
    };

    template <typename T>
    class FIQuery<T, Plane3<T>, Plane3<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                isLine(false),
                line{},
                plane{}
            {
            }

            // If 'intersect' is true, the intersection is either a line or
            // the planes are the same.  When a line, 'line' is valid.  When
            // the same plane, 'plane' is set to one of the planes.
            bool intersect;
            bool isLine;
            Line3<T> line;
            Plane3<T> plane;
        };

        Output operator()(Plane3<T> const& plane0, Plane3<T> const& plane1)
        {
            // If N0 and N1 are parallel, either the planes are parallel and
            // separated or the same plane. In both cases, 'false' is
            // returned. Otherwise, the intersection line is
            //   L(t) = t*Cross(N0,N1)/|Cross(N0,N1)| + c0*N0 + c1*N1
            // for some coefficients c0 and c1 and for t any real number (the
            // line parameter). Taking dot products with the normals,
            //   d0 = Dot(N0,L) = c0*Dot(N0,N0) + c1*Dot(N0,N1) = c0 + c1*d
            //   d1 = Dot(N1,L) = c0*Dot(N0,N1) + c1*Dot(N1,N1) = c0*d + c1
            // where d = Dot(N0,N1). These are two equations in two unknowns.
            // The solution is
            //   c0 = (d0 - d*d1)/det
            //   c1 = (d1 - d*d0)/det
            // where det = 1 - d^2.

            Output output{};

            T dot = Dot(plane0.normal, plane1.normal);
            if (std::fabs(dot) >= C_<T>(1))
            {
                // The planes are parallel. Check if they are coplanar.
                T cDiff;
                if (dot >= C_<T>(0))
                {
                    // Normals are in same direction, need to look at c0-c1.
                    cDiff = plane0.constant - plane1.constant;
                }
                else
                {
                    // Normals are in opposite directions, need to look at
                    // c0+c1.
                    cDiff = plane0.constant + plane1.constant;
                }

                if (std::fabs(cDiff) == C_<T>(0))
                {
                    // The planes are coplanar. The output.line member was
                    // initialized to zero-valued components in the Output
                    // constructor.
                    output.intersect = true;
                    output.isLine = false;
                    output.plane = plane0;
                }
                else
                {
                    // The planes are parallel but distinct. The output.line
                    // and output.plane members wer initialized to zero-valued
                    // components in the Output constructor.
                    output.intersect = false;
                }
            }
            else
            {
                // The planes are not parallel, so they intersect in a line.
                // The output.plane member was initialized to zero-valued
                // components in the Output constructor.
                T denom = C_<T>(1) - dot * dot;
                T c0 = (plane0.constant - dot * plane1.constant) / denom;
                T c1 = (plane1.constant - dot * plane0.constant) / denom;
                output.intersect = true;
                output.isLine = true;
                output.line.origin = c0 * plane0.normal + c1 * plane1.normal;
                output.line.direction = UnitCross(plane0.normal, plane1.normal);
            }

            return output;
        }

    private:
        friend class UnitTestIntrPlane3Plane3;
    };
}
