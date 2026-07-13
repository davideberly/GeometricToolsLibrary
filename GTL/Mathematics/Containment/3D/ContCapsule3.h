// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.13

#pragma once

#include <GTL/Mathematics/Approximation/3D/ApprOrthogonalLine3.h>
#include <GTL/Mathematics/Distance/ND/DistPointLine.h>
#include <GTL/Mathematics/Distance/ND/DistPointSegment.h>
#include <GTL/Mathematics/Primitives/ND/Capsule.h>
#include <GTL/Mathematics/Primitives/3D/Sphere3.h>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class ContCapsule3
    {
    public:
        // Compute the axis of the capsule segment using least-squares
        // fitting. The radius is the maximum distance from the points to the
        // axis. Hemispherical caps are chosen as close together as possible.
        static void GetContainer(std::vector<Vector3<T>> const& points, Capsule3<T>& capsule)
        {
            Line3<T> line{};
            (void)ApprOrthogonalLine3<T>::Fit(points, line);

            DCPQuery<T, Vector3<T>, Line3<T>> plQuery{};
            T maxRadiusSqr = C_<T>(0);
            for (auto const& point : points)
            {
                auto result = plQuery(point, line);
                if (result.sqrDistance > maxRadiusSqr)
                {
                    maxRadiusSqr = result.sqrDistance;
                }
            }

            Vector3<T> W = line.direction, U{}, V{};
            ComputeOrthonormalBasis(1, W, U, V);

            Vector3<T> diff = points[0] - line.origin;
            T uDotDiff = Dot(U, diff);
            T vDotDiff = Dot(V, diff);
            T wDotDiff = Dot(W, diff);
            T discr = maxRadiusSqr - (uDotDiff * uDotDiff + vDotDiff * vDotDiff);
            T radical = std::sqrt(std::max(discr, C_<T>(0)));
            T minValue = wDotDiff + radical;
            T maxValue = wDotDiff - radical;
            for (std::size_t i = 1; i < points.size(); ++i)
            {
                diff = points[i] - line.origin;
                uDotDiff = Dot(U, diff);
                vDotDiff = Dot(V, diff);
                wDotDiff = Dot(W, diff);
                discr = maxRadiusSqr - (uDotDiff * uDotDiff + vDotDiff * vDotDiff);
                radical = std::sqrt(std::max(discr, C_<T>(0)));

                T test = wDotDiff + radical;
                if (test < minValue)
                {
                    minValue = test;
                }

                test = wDotDiff - radical;
                if (test > maxValue)
                {
                    maxValue = test;
                }
            }

            Vector3<T> center = line.origin + (C_<T>(1, 2) * (minValue + maxValue)) * line.direction;

            T extent;
            if (maxValue > minValue)
            {
                // Container is a capsule.
                extent = C_<T>(1, 2) * (maxValue - minValue);
            }
            else
            {
                // Container is a sphere.
                extent = C_<T>(0);
            }

            capsule.segment = Segment3<T>(center, line.direction, extent);
            capsule.radius = std::sqrt(maxRadiusSqr);
        }

        // Test for containment of a point by a capsule.
        static bool InContainer(Vector3<T> const& point, Capsule3<T> const& capsule)
        {
            DCPQuery<T, Vector3<T>, Segment3<T>> psQuery{};
            auto result = psQuery(point, capsule.segment);
            return result.distance <= capsule.radius;
        }

        // Test for containment of a sphere by a capsule.
        static bool InContainer(Sphere3<T> const& sphere, Capsule3<T> const& capsule)
        {
            T rDiff = capsule.radius - sphere.radius;
            if (rDiff >= C_<T>(0))
            {
                DCPQuery<T, Vector3<T>, Segment3<T>> psQuery{};
                auto result = psQuery(sphere.center, capsule.segment);
                return result.distance <= rDiff;
            }
            return false;
        }

        // Test for containment of a capsule by a capsule.
        static bool InContainer(Capsule3<T> const& testCapsule, Capsule3<T> const& capsule)
        {
            Sphere3<T> spherePosEnd(testCapsule.segment.p[1], testCapsule.radius);
            Sphere3<T> sphereNegEnd(testCapsule.segment.p[0], testCapsule.radius);
            return InContainer(spherePosEnd, capsule)
                && InContainer(sphereNegEnd, capsule);
        }

        // Compute a capsule that contains the input capsules. The returned
        // capsule is not necessarily the one of smallest volume that contains
        // the inputs.
        static void MergeContainers(Capsule3<T> const& capsule0,
            Capsule3<T> const& capsule1, Capsule3<T>& merge)
        {
            if (InContainer(capsule0, capsule1))
            {
                merge = capsule1;
                return;
            }

            if (InContainer(capsule1, capsule0))
            {
                merge = capsule0;
                return;
            }

            Vector3<T> P0{}, P1{}, D0{}, D1{};
            T extent0{}, extent1{};
            capsule0.segment.GetCenteredForm(P0, D0, extent0);
            capsule1.segment.GetCenteredForm(P1, D1, extent1);

            // Axis of final capsule. The origin is the average of the input
            // axis origins. The direction is the normalized average of the
            // input axis directions.
            Line3<T> line{};
            line.origin = C_<T>(1, 2) * (P0 + P1);
            if (Dot(D0, D1) >= C_<T>(0))
            {
                line.direction = D0 + D1;
            }
            else
            {
                line.direction = D0 - D1;
            }
            Normalize(line.direction);

            // The cylinder with the axis 'line' must contain the spheres
            // centered at the endpoints of the input capsules.
            DCPQuery<T, Vector3<T>, Line3<T>> plQuery{};
            Vector3<T> posEnd0 = capsule0.segment.p[1];
            T radius = plQuery(posEnd0, line).distance + capsule0.radius;

            Vector3<T> negEnd0 = capsule0.segment.p[0];
            T tmp = plQuery(negEnd0, line).distance + capsule0.radius;

            Vector3<T> posEnd1 = capsule1.segment.p[1];
            tmp = plQuery(posEnd1, line).distance + capsule1.radius;
            if (tmp > radius)
            {
                radius = tmp;
            }

            Vector3<T> negEnd1 = capsule1.segment.p[0];
            tmp = plQuery(negEnd1, line).distance + capsule1.radius;
            if (tmp > radius)
            {
                radius = tmp;
            }

            // In the following blocks of code, theoretically k1*k1-k0 >= 0, but
            // numerical rounding errors can make it slightly negative. Guard
            // against this.

            // Process sphere <posEnd0,r0>.
            T rDiff = radius - capsule0.radius;
            T rDiffSqr = rDiff * rDiff;
            Vector3<T> diff = line.origin - posEnd0;
            T k0 = Dot(diff, diff) - rDiffSqr;
            T k1 = Dot(diff, line.direction);
            T discr = k1 * k1 - k0;
            T root = std::sqrt(std::max(discr, C_<T>(0)));
            T tPos = -k1 - root;
            T tNeg = -k1 + root;

            // Process sphere <negEnd0,r0>.
            diff = line.origin - negEnd0;
            k0 = Dot(diff, diff) - rDiffSqr;
            k1 = Dot(diff, line.direction);
            discr = k1 * k1 - k0;
            root = std::sqrt(std::max(discr, C_<T>(0)));
            tmp = -k1 - root;
            if (tmp > tPos)
            {
                tPos = tmp;
            }
            tmp = -k1 + root;
            if (tmp < tNeg)
            {
                tNeg = tmp;
            }

            // Process sphere <posEnd1,r1>.
            rDiff = radius - capsule1.radius;
            rDiffSqr = rDiff * rDiff;
            diff = line.origin - posEnd1;
            k0 = Dot(diff, diff) - rDiffSqr;
            k1 = Dot(diff, line.direction);
            discr = k1 * k1 - k0;
            root = std::sqrt(std::max(discr, C_<T>(0)));
            tmp = -k1 - root;
            if (tmp > tPos)
            {
                tPos = tmp;
            }
            tmp = -k1 + root;
            if (tmp < tNeg)
            {
                tNeg = tmp;
            }

            // Process sphere <negEnd1,r1>.
            diff = line.origin - negEnd1;
            k0 = Dot(diff, diff) - rDiffSqr;
            k1 = Dot(diff, line.direction);
            discr = k1 * k1 - k0;
            root = std::sqrt(std::max(discr, C_<T>(0)));
            tmp = -k1 - root;
            if (tmp > tPos)
            {
                tPos = tmp;
            }
            tmp = -k1 + root;
            if (tmp < tNeg)
            {
                tNeg = tmp;
            }

            Vector3<T> center = line.origin + C_<T>(1, 2) * (tPos + tNeg) * line.direction;

            T extent{};
            if (tPos > tNeg)
            {
                // Container is a capsule.
                extent = C_<T>(1, 2) * (tPos - tNeg);
            }
            else
            {
                // Container is a sphere.
                extent = C_<T>(0);
            }

            merge.segment = Segment3<T>(center, line.direction, extent);
            merge.radius = radius;
        }

    private:
        friend class UnitTestContCapsule3;
    };
}
