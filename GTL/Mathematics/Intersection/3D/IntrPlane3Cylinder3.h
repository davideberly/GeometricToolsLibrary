// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// The intersection queries between a plane and a cylinder (finite or
// infinite) are described in
// https://www.geometrictools.com/Documentation/IntersectionCylinderPlane.pdf
//
// The plane is Dot(N, X - P) = 0, where P is a point on the plane and N is a
// nonzero vector that is not necessarily unit length.
// 
// The cylinder is (X - C)^T * (I - W * W^T) * (X - C) = r^2, where C is the
// center, W is the axis direction and r > 0 is the radius. The cylinder has
// height h. In the intersection queries, an infinite cylinder is specified
// by setting h = -1. Read the aforementioned PDF for details about this
// choice.

#include <GTL/Mathematics/Algebra/Matrix.h>
#include <GTL/Mathematics/Intersection/3D/IntrPlane3Plane3.h>
#include <GTL/Mathematics/Primitives/2D/Ellipse2.h>
#include <GTL/Mathematics/Primitives/3D/Ellipse3.h>
#include <GTL/Mathematics/Primitives/ND/Cylinder.h>
#include <array>
#include <cmath>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Plane3<T>, Cylinder3<T>>
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

        // For an infinite cylinder, call cylinder.MakeInfiniteCylinder().
        // Internally, the height is set to -1. This avoids the problem
        // of setting height to std::numeric_limits<T>::max() or
        // std::numeric_limits<T>::infinity() that are designed for
        // floating-point types but that do not work for exact rational types.
        //
        // For a finite cylinder, set cylinder.height > 0.
        Output operator()(Plane3<T> const& plane, Cylinder3<T> const& cylinder)
        {
            Output output{};

            // Convenient names.
            auto const& P = plane.origin;
            auto const& N = plane.normal;
            auto const& C = cylinder.center;
            auto const& W = cylinder.direction;
            auto const& r = cylinder.radius;
            auto const& h = cylinder.height;

            if (cylinder.IsInfinite())
            {
                if (Dot(N, W) != C_<T>(0))
                {
                    // The cylinder direction and plane are not parallel.
                    output.intersect = true;
                }
                else
                {
                    // The cylinder direction and plane are parallel.
                    T dotNCmP = Dot(N, C - P);
                    output.intersect = (std::fabs(dotNCmP) <= r);
                }
            }
            else // cylinder is finite
            {
                T dotNCmP = Dot(N, C - P);
                T dotNW = Dot(N, W);
                Vector3<T> crossNW = Cross(N, W);
                T lhs = std::fabs(dotNCmP);
                T rhs = r * Length(crossNW) + C_<T>(1, 2) * h * std::fabs(dotNW);
                output.intersect = (lhs <= rhs);
            }

            return output;
        }

    private:
        friend class UnitTestIntrPlane3Cylinder3;
    };

    template <typename T>
    class FIQuery<T, Plane3<T>, Cylinder3<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                type(Type::NO_INTERSECTION),
                line{},
                ellipse{},
                trimLine{}
            {
            }

            // The type of intersection.
            enum class Type
            {
                // The cylinder and plane are separated.
                NO_INTERSECTION,

                // The plane is tangent to the cylinder direction.
                SINGLE_LINE,

                // The cylinder direction is parallel to the plane and the
                // plane cuts through the cylinder in two lines.
                PARALLEL_LINES,

                // The cylinder direction is perpendicular to the plane.
                CIRCLE,

                // The cylinder direction is not parallel to the plane. When
                // the direction is perpendicular to the plane, the
                // intersection is a circle which is an ellipse with equal
                // extents.
                ELLIPSE
            };

            // The output members are set according to type.
            // 
            // type = NONE
            //   intersect = false
            //   line[0,1] and ellipse have all zero members
            //
            // type = SINGLE_LINE
            //   intersect = true
            //   line[0] is valid
            //   line[1] and ellipse have all zero members
            //
            // type = PARALLEL_LINES
            //   intersect = true
            //   line[0] and line[1] are valid
            //   ellipse has all zero members
            // 
            // type = CIRCLE
            //   intersect = true
            //   ellipse is valid (with extent[0] = extent[1])
            //   line[0,1] have all zero members
            //
            // type = ELLIPSE
            //   intersect = true
            //   ellipse is valid
            //   line[0,1] have all zero members
            bool intersect;
            Type type;
            std::array<Line3<T>, 2> line;
            Ellipse3<T> ellipse;

            // Trim lines when the cylinder is finite. They are computed when
            // the plane and infinite cylinder intersect. If there is no
            // intersection, the trim lines have all zero members.
            std::array<Line3<T>, 2> trimLine;
        };

        Output operator()(Plane3<T> const& plane, Cylinder3<T> const& cylinder)
        {
            Output output{};

            TIQuery<T, Plane3<T>, Cylinder3<T>> tiQuery{};
            auto tiOutput = tiQuery(plane, cylinder);
            if (tiOutput.intersect)
            {
                T dotNW = Dot(plane.normal, cylinder.direction);
                if (dotNW != C_<T>(0))
                {
                    // The cylinder direction is not parallel to the plane.
                    // The intersection is an ellipse or circle.
                    GetEllipseOfIntersection(plane, cylinder, output);
                    GetTrimLines(plane, cylinder, output.trimLine);
                }
                else
                {
                    // The cylinder direction is parallel to the plane. There
                    // are no trim lines for this geometric configuration.
                    GetLinesOfIntersection(plane, cylinder, output);
                }
            }

            return output;
        }

    private:
        // The cylinder is infinite and its direction is not parallel to the
        // plane.
        static void GetEllipseOfIntersection(Plane3<T> const& plane,
            Cylinder3<T> const& cylinder, Output& output)
        {
            // Convenient names.
            auto const& P = plane.origin;
            auto const& N = plane.normal;
            auto const& C = cylinder.center;
            auto const& W = cylinder.direction;
            auto const& r = cylinder.radius;

            // Compute a right-handed orthonormal basis {N,A,B}. The
            // plane is spanned by A and B.
            Vector3<T> normal = N, A{}, B{};
            ComputeOrthonormalBasis(1, normal, A, B);

            // Compute the projection matrix M = I - W * W^T.
            Matrix3x3<T> M{};
            MakeIdentity(M);
            M = M - OuterProduct(W, W);

            // Compute the coefficients of the quadratic equation
            // c00 + c10*x + c01*y + c20*x^2 + c11*x*y + c02*y^2 = 0.
            Vector3<T> PmC = P - C;
            Vector3<T> MtPmC = M * PmC;
            Vector3<T> MtA = M * A;
            Vector3<T> MtB = M * B;
            std::array<T, 6> coefficients
            {
                Dot(PmC, MtPmC) - r * r,
                C_<T>(2)* Dot(A, MtPmC),
                C_<T>(2)* Dot(B, MtPmC),
                Dot(A, MtA),
                C_<T>(2)* Dot(A, MtB),
                Dot(B, MtB)
            };

            // Compute the 2D ellipse parameters in plane coordinates.
            Ellipse2<T> ellipse2{};
            (void)ellipse2.FromCoefficients(coefficients);

            // Lift the 2D ellipse/circle to the 3D ellipse/circle.
            output.intersect = true;
            output.type = (ellipse2.extent[0] != ellipse2.extent[1] ?
                Output::Type::ELLIPSE : Output::Type::CIRCLE);
            output.ellipse.center = plane.origin + ellipse2.center[0] * A +
                ellipse2.center[1] * B;
            output.ellipse.normal = plane.normal;
            output.ellipse.axis[0] = ellipse2.axis[0][0] * A +
                ellipse2.axis[0][1] * B;
            output.ellipse.axis[1] = ellipse2.axis[1][0] * A +
                ellipse2.axis[1][1] * B;
            output.ellipse.extent = ellipse2.extent;
        }

        // The cylinder is infinite and its direction is parallel to the
        // plane.
        static void GetLinesOfIntersection(Plane3<T> const& plane,
            Cylinder3<T> const& cylinder, Output& output)
        {
            // Convenient names.
            auto const& P = plane.origin;
            auto const& N = plane.normal;
            auto const& C = cylinder.center;
            auto const& W = cylinder.direction;
            auto const& r = cylinder.radius;

            Vector3<T> CmP = C - P;
            T dotNCmP = Dot(N, CmP);
            T ellSqr = r * r - dotNCmP * dotNCmP;  // r^2 - d^2
            if (ellSqr > C_<T>(0))
            {
                // The plane cuts through the cylinder in two lines.
                output.intersect = true;
                output.type = Output::Type::PARALLEL_LINES;
                Vector3<T> projC = C - dotNCmP * N;
                Vector3<T> crsNW = Cross(N, W);
                T ell = std::sqrt(ellSqr);
                output.line[0].origin = projC - ell * crsNW;
                output.line[0].direction = W;
                output.line[1].origin = projC + ell * crsNW;
                output.line[1].direction = W;
            }
            else if (ellSqr < C_<T>(0))
            {
                // The cylinder does not intersect the plane.
                output.intersect = false;
                output.type = Output::Type::NO_INTERSECTION;
            }
            else  // ellSqr = 0
            {
                // The plane is tangent to the cylinder.
                output.intersect = true;
                output.type = Output::Type::SINGLE_LINE;
                output.line[0].origin = C - dotNCmP * N;
                output.line[0].direction = W;
            }
        }

        static void GetTrimLines(Plane3<T> const& plane, Cylinder3<T> const& cylinder,
            std::array<Line3<T>, 2>& trimLine)
        {
            // Compute the cylinder end planes.
            auto const& C = cylinder.center;
            auto const& D = cylinder.direction;
            auto const& h = cylinder.height;
            Vector3<T> offset = (C_<T>(1, 2) * h) * D;

            FIQuery<T, Plane3<T>, Plane3<T>> ppQuery{};

            Plane3<T> endPlaneNeg(D, C - offset);
            auto ppOutput = ppQuery(plane, endPlaneNeg);
            trimLine[0] = ppOutput.line;

            Plane3<T> endPlanePos(D, C + offset);
            ppOutput = ppQuery(plane, endPlanePos);
            trimLine[1] = ppOutput.line;
        }

    private:
        friend class UnitTestIntrPlane3Cylinder3;
    };
}
