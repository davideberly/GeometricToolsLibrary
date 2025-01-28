// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

#include <GTL/Mathematics/Intersection/3D/IntrPlane3Plane3.h>
#include <GTL/Mathematics/Primitives/3D/Circle3.h>
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Plane3<T>, Circle3<T>>
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

        Output operator()(Plane3<T> const& plane, Circle3<T> const& circle)
        {
            Output output{};

            // Construct the plane of the circle.
            Plane3<T> cPlane(circle.normal, circle.center);

            // Compute the intersection of this plane with the input plane.
            FIQuery<T, Plane3<T>, Plane3<T>> ppQuery{};
            auto ppOutput = ppQuery(plane, cPlane);
            if (!ppOutput.intersect)
            {
                // The planes are parallel and nonintersecting.
                output.intersect = false;
                return output;
            }

            if (!ppOutput.isLine)
            {
                // The planes are the same, so the circle is the set of
                // intersection.
                output.intersect = true;
                return output;
            }

            // The planes intersect in a line. Locate one or two points that
            // are on the circle and line. If the line is t*D+P, the circle
            // center is C and the circle radius is r, then
            //   r^2 = |t*D+P-C|^2 = |D|^2*t^2 + 2*Dot(D,P-C)*t + |P-C|^2
            // This is a quadratic equation of the form
            // a2*t^2 + 2*a1*t + a0 = 0.
            Vector3<T> diff = ppOutput.line.origin - circle.center;
            T a2 = Dot(ppOutput.line.direction, ppOutput.line.direction);
            T a1 = Dot(diff, ppOutput.line.direction);
            T a0 = Dot(diff, diff) - circle.radius * circle.radius;

            // T-valued roots imply an intersection.
            T discr = a1 * a1 - a0 * a2;
            output.intersect = (discr >= C_<T>(0));
            return output;
        }

    private:
        friend class UnitTestIntrPlane3Circle3;
    };

    template <typename T>
    class FIQuery<T, Plane3<T>, Circle3<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0),
                point{},
                circle{}
            {
            }

            // If 'intersect' is false, the set of intersection is empty.
            // 'numIntersections' is 0 and 'points[]' and 'circle' have
            // members all set to 0.
            // 
            // If 'intersect' is true, the set of intersection contains either
            // 1 or 2 points or the entire circle.
            //
            // (1) When the set of intersection has 1 point, the circle is
            //     just touching the plane. 'numIntersections' is 1 and
            //     'point[0]' and 'point[1]' are the same point. The
            //     'circle' is set to invalid (center at the origin, normal
            //     is the zero vector, radius is 0).
            // 
            // (2) When the set of intersection has 2 points, the plane cuts
            //     the circle into 2 arcs. 'numIntersections' is 2 and
            //     'point[0]' and 'point[1]' are the distinct intersection
            //     points. The 'circle' is set to invalid (center at the
            //     origin, normal is the zero vector, radius is 0).
            // 
            // (3) When the set of intersection contains the entire circle, the
            //     plane of the circle and the input plane are the same.
            //     'numIntersections' is std::numeric_limits<std::size_t>::max().
            //     'points[0]' and 'points[1]' are set to the zero vector.
            //     'circle' is set to the input circle.
            //     
            bool intersect;
            std::size_t numIntersections;
            std::array<Vector3<T>, 2> point;
            Circle3<T> circle;
        };

        Output operator()(Plane3<T> const& plane, Circle3<T> const& circle)
        {
            // The 'output' members have initial values set by the default
            // constructor. In each return block, only the relevant members
            // are modified.
            Output output{};

            // Construct the plane of the circle.
            Plane3<T> cPlane(circle.normal, circle.center);

            // Compute the intersection of this plane with the input plane.
            FIQuery<T, Plane3<T>, Plane3<T>> ppQuery{};
            auto ppOutput = ppQuery(plane, cPlane);
            if (!ppOutput.intersect)
            {
                // The planes are parallel and nonintersecting.
                return output;
            }

            if (!ppOutput.isLine)
            {
                // The planes are the same, so the circle is the set of
                // intersection.
                output.intersect = true;
                output.numIntersections = std::numeric_limits<std::size_t>::max();
                output.circle = circle;
                return output;
            }

            // The planes intersect in a line. Locate one or two points that
            // are on the circle and line. If the line is t*D+P, the circle
            // center is C, and the circle radius is r, then
            //   r^2 = |t*D+P-C|^2 = |D|^2*t^2 + 2*Dot(D,P-C)*t + |P-C|^2
            // This is a quadratic equation of the form
            // a2*t^2 + 2*a1*t + a0 = 0.
            Vector3<T> diff = ppOutput.line.origin - circle.center;
            T a2 = Dot(ppOutput.line.direction, ppOutput.line.direction);
            T a1 = Dot(diff, ppOutput.line.direction);
            T a0 = Dot(diff, diff) - circle.radius * circle.radius;

            T discr = a1 * a1 - a0 * a2;
            if (discr < C_<T>(0))
            {
                // No real roots, the circle does not intersect the plane.
                return output;
            }

            if (discr == C_<T>(0))
            {
                // The quadratic polynomial has 1 real-valued repeated root.
                // The circle just touches the plane.
                output.intersect = true;
                output.numIntersections = 1;
                output.point[0] = ppOutput.line.origin - (a1 / a2) * ppOutput.line.direction;
                output.point[1] = output.point[0];
                return output;
            }

            // The quadratic polynomial has 2 distinct, real-valued roots.
            // The circle intersects the plane in two points.
            T root = std::sqrt(discr);
            output.intersect = true;
            output.numIntersections = 2;
            output.point[0] = ppOutput.line.origin - ((a1 + root) / a2) * ppOutput.line.direction;
            output.point[1] = ppOutput.line.origin - ((a1 - root) / a2) * ppOutput.line.direction;
            return output;
        }

    private:
        friend class UnitTestIntrPlane3Circle3;
    };
}
