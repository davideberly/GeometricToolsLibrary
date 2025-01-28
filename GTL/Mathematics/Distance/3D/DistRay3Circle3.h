// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// The 3D ray-circle distance algorithm is described in
// https://www.geometrictools.com/Documentation/DistanceToCircle3.pdf
// The notation used in the code matches that of the document. The circle has
// center C and the plane of the circle has unit-length normal N. The ray has
// origin B and non-zero direction M. The parameterization is P(t) = t*M+B
// for t >= 0. The type T can be a floating-point type or a rational type.

#include <GTL/Mathematics/Distance/3D/DistLine3Circle3.h>
#include <GTL/Mathematics/Distance/3D/DistPoint3Circle3.h>
#include <GTL/Mathematics/Primitives/ND/Ray.h>

namespace gtl
{
    template <typename T>
    class DCPQuery<T, Ray3<T>, Circle3<T>>
    {
    public:
        using Output = typename DCPLinearCircularSupport3<T>::Output;
        using Critical = typename DCPLinearCircularSupport3<T>::Critical;

        // The 'critical' parameter is an out-parameter. Usually this is not
        // something an application requires, but it is used by the other
        // 3D linear-circular distance queries and by unit tests.
        Output operator()(Ray3<T> const& ray, Circle3<T> const& circle,
            Critical* outCritical = nullptr)
        {
            // Compute the line points closest to the circle.
            Line3<T> line(ray.origin, ray.direction);
            Critical critical{};
            Output output = DCPQuery<T, Line3<T>, Circle3<T>>{}(line, circle, &critical);

            // Clamp the query output to the ray domain.
            if (critical.numPoints == 1)
            {
                HasOneCriticalPoint(ray, circle, critical, output);
            }
            else
            {
                HasTwoCriticalPoints(ray, circle, critical, output);
            }

            if (outCritical != nullptr)
            {
                *outCritical = critical;
            }

            return output;
        }

    private:
        using PCQuery = DCPQuery<T, Vector3<T>, Circle3<T>>;
        using PCOutput = typename PCQuery::Output;

        void HasOneCriticalPoint(Ray3<T> const& ray, Circle3<T> const& circle,
            Critical const& critical, Output& output)
        {
            T const& t0 = critical.parameter[0];

            T const zero = C_<T>(0);
            if (t0 <= zero)
            {
                // The critical point is not on the ray. The ray origin is
                // the ray point closest to the circle. See the red ray of
                // the one-critical-point graph of figure 7 in the PDF.
                return RayOriginClosest(ray.origin, circle, output);
            }

            // At this time, t0 > 0. The closest line-circle pair is the
            // closest ray-origin circle pair. The output does not need to be
            // modified. See the green ray of the one-critical-point graph of
            // figure 7 in the PDF.
        }

        void HasTwoCriticalPoints(Ray3<T> const& ray, Circle3<T> const& circle,
            Critical const& critical, Output& output)
        {
            T const& t0 = critical.parameter[0];
            T const& t1 = critical.parameter[1];

            T const zero = static_cast<T>(0);
            if (t0 >= zero)
            {
                // The critical points are on the ray. The ray point closest
                // to the circle is the line point closest to the circle. The
                // output remains unchanged. See the green rays of the
                // two-critical-point graphs of figure 7 in the PDF.
                return;
            }

            if (t1 <= zero)
            {
                // The critical points are not on the ray. The ray origin is
                // the ray point closest to the circle. See the red rays of
                // the two-critical-point graphs of figure 7 in the PDF.
                return RayOriginClosest(ray.origin, circle, output);
            }

            // The ray point closest to the circle is either the ray origin or
            // the second critical point, whichever has minimum distance. See
            // the orange and purple rays of the two-critical-point graphs of
            // figure 7 in the PDF.
            SelectClosestPoint(ray.origin, critical.linearPoint[1], circle, output);
        }

        void RayOriginClosest(Vector3<T> const& rayOrigin, Circle3<T> const& circle,
            Output& output)
        {
            PCOutput pcOutput = PCQuery{}(rayOrigin, circle);
            output.numClosestPairs = 1;
            output.linearClosest[0] = rayOrigin;
            output.linearClosest[1] = { C_<T>(0), C_<T>(0), C_<T>(0) };
            output.circularClosest[0] = pcOutput.closest[1];
            output.circularClosest[1] = { C_<T>(0), C_<T>(0), C_<T>(0) };
            output.distance = pcOutput.distance;
            output.sqrDistance = output.distance * output.distance;
        }

        void SelectClosestPoint(Vector3<T> const& point0, Vector3<T> const& point1,
            Circle3<T> const& circle, Output& output)
        {
            PCOutput pcOutput0 = PCQuery{}(point0, circle);
            PCOutput pcOutput1 = PCQuery{}(point1, circle);
            if (pcOutput0.distance < pcOutput1.distance)
            {
                output.numClosestPairs = 1;
                output.linearClosest[0] = point0;
                output.linearClosest[1] = { C_<T>(0), C_<T>(0), C_<T>(0) };
                output.circularClosest[0] = pcOutput0.closest[1];
                output.circularClosest[1] = { C_<T>(0), C_<T>(0), C_<T>(0) };
                output.distance = pcOutput0.distance;
                output.sqrDistance = output.distance * output.distance;
            }
            else if (pcOutput0.distance > pcOutput1.distance)
            {
                output.numClosestPairs = 1;
                output.linearClosest[0] = point1;
                output.linearClosest[1] = { C_<T>(0), C_<T>(0), C_<T>(0) };
                output.circularClosest[0] = pcOutput1.closest[1];
                output.circularClosest[1] = { C_<T>(0), C_<T>(0), C_<T>(0) };
                output.distance = pcOutput1.distance;
                output.sqrDistance = output.distance * output.distance;
            }
            else // pcOutput0.distance = pcOutput1.distance
            {
                output.numClosestPairs = 2;
                output.linearClosest[0] = point0;
                output.linearClosest[1] = point1;
                output.circularClosest[0] = pcOutput0.closest[1];
                output.circularClosest[1] = pcOutput1.closest[1];
                output.distance = pcOutput0.distance;
                output.sqrDistance = output.distance * output.distance;
            }
        }

    private:
        friend class UnitTestDistRay3Circle3;
    };
}
