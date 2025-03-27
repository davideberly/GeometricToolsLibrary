// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// The 3D line-circle distance algorithm is described in
// https://www.geometrictools.com/Documentation/DistanceToCircle3.pdf
// The notation used in the code matches that of the document. The circular
// arc has center C and the plane of the circle has unit-length normal N.
// The line has origin B and non-zero direction M. The parameterization is
// P(t) = t*M+B. It is not necessary that M be a unit-length vector. The
// type T can be a floating-point type or a rational type.
//
// The line-arc distance is formulated as a calculus problem. The domain
// of the function is
//   (t,angle) in (-infinity,+infinity)x(minAngle,maxAngle)
// The minimum occurs at an interior point (t,angle) where the gradient
// is (0,0), at a boundary point (t,minAngle), or at a boundary point
// (t,maxAngle).

#include <GTL/Mathematics/Distance/3D/DistLine3Circle3.h>
#include <GTL/Mathematics/Distance/ND/DistPointLine.h>
#include <algorithm>
#include <array>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class DCPQuery<T, Line3<T>, Arc3<T>>
    {
    public:
        using Output = typename DCPLinearCircularSupport3<T>::Output;
        using Critical = typename DCPLinearCircularSupport3<T>::Critical;

        // The 'critical' parameter is an out-parameter. Usually this is not
        // something an application requires, but it is used by the other
        // 3D linear-circular distance queries and by unit tests.
        Output operator()(Line3<T> const& line, Arc3<T> const& arc,
            Critical* outCritical = nullptr)
        {
            // Compute the line points closest to the circle.
            Circle3<T> circle(arc.center, arc.normal, arc.radius);
            Critical critical{};
            Output output = DCPQuery<T, Line3<T>, Circle3<T>>{}(line, circle, &critical);

            if (!output.equidistant)
            {
                // Create the set of candidates for a closest pair. These are
                // the critical points and boundary points mentioned in the
                // comments at the top of this file.
                Candidates candidates{};

                // Insert the arc endpoints.
                candidates.push_back(Candidate(arc.end[0], line));  // (t,minAngle)
                candidates.push_back(Candidate(arc.end[1], line));  // (t,maxAngle)

                // Insert critical points only if they are on the arc.
                for (std::size_t i = 0; i < critical.numPoints; ++i)
                {
                    if (arc.Contains(critical.circularPoint[i]))
                    {
                        // (t,angle) interior
                        candidates.push_back(Candidate(critical, i));
                    }
                }

                DCPLinearCircularSupport3<T>::GetOutputFromCandidates(
                    candidates, output);
            }
            else
            {
                Candidate candidate(arc.end[0], line);
                output.numClosestPairs = 1;
                output.linearClosest[0] = candidate.linearPoint;
                output.circularClosest[0] = candidate.circularPoint;
                output.distance = candidate.distance;
                output.sqrDistance = candidate.sqrDistance;
            }

            if (outCritical != nullptr)
            {
                *outCritical = critical;
            }

            return output;
        }

    private:
        using Candidate = typename DCPLinearCircularSupport3<T>::Candidate;
        using Candidates = typename DCPLinearCircularSupport3<T>::Candidates;

    private:
        friend class UnitTestDistLine3Arc3;
    };
}
