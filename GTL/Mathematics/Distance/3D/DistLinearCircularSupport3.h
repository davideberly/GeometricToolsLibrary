// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// Classes and functions that support 3D linear-circular distance queries
// where linear is in {line, ray, segment} and circular is in {circle, arc}.
// The goal is to share code that is common to the queries rather than
// duplicating the code in each query class.
#include <GTL/Mathematics/Distance/3D/DistPoint3Arc3.h>
#include <GTL/Mathematics/Distance/ND/DistPointLine.h>
#include <GTL/Mathematics/Distance/ND/DistPointRay.h>
#include <GTL/Mathematics/Distance/ND/DistPointSegment.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class DCPLinearCircularSupport3
    {
    public:
        struct Output
        {
            Output()
                :
                numClosestPairs(0),
                linearClosest{},
                circularClosest{},
                distance(C_<T>(0)),
                sqrDistance(C_<T>(0)),
                equidistant(false)
            {
            }

            std::size_t numClosestPairs;
            std::array<Vector3<T>, 3> linearClosest, circularClosest;
            T distance, sqrDistance;
            bool equidistant;
        };

        struct Critical
        {
            Critical()
                :
                numPoints(0),
                linearPoint{},
                circularPoint{},
                parameter{ C_<T>(0), C_<T>(0) },
                distance{ C_<T>(0), C_<T>(0) }
            {
            }

            std::size_t numPoints;
            std::array<Vector3<T>, 2> linearPoint, circularPoint;
            std::array<T, 2> parameter;
            std::array<T, 2> distance;
        };

        struct Candidate
        {
            Candidate()
                :
                linearPoint{},
                circularPoint{},
                distance(static_cast<T>(0)),
                sqrDistance(static_cast<T>(0))
            {
            }

            Candidate(Vector3<T> const& arcEndpoint, Line3<T> const& line)
                :
                linearPoint{},
                circularPoint(arcEndpoint),
                distance(static_cast<T>(0)),
                sqrDistance(static_cast<T>(0))
            {
                auto output = DCPQuery<T, Vector3<T>, Line3<T>>{}(arcEndpoint, line);
                linearPoint = output.closest[1];
                circularPoint = output.closest[0];
                distance = output.distance;
                sqrDistance = output.sqrDistance;
            }

            Candidate(Vector3<T> const& arcEndpoint, Ray3<T> const& ray)
                :
                linearPoint{},
                circularPoint(arcEndpoint),
                distance(static_cast<T>(0)),
                sqrDistance(static_cast<T>(0))
            {
                auto output = DCPQuery<T, Vector3<T>, Ray3<T>>{}(arcEndpoint, ray);
                linearPoint = output.closest[1];
                circularPoint = output.closest[0];
                distance = output.distance;
                sqrDistance = output.sqrDistance;
            }

            Candidate(Vector3<T> const& arcEndpoint, Segment3<T> const& segment)
                :
                linearPoint{},
                circularPoint(arcEndpoint),
                distance(static_cast<T>(0)),
                sqrDistance(static_cast<T>(0))
            {
                auto output = DCPQuery<T, Vector3<T>, Segment3<T>>{}(arcEndpoint, segment);
                linearPoint = output.closest[1];
                circularPoint = output.closest[0];
                distance = output.distance;
                sqrDistance = output.sqrDistance;
            }

            Candidate(Vector3<T> const& point, Arc3<T> const& arc)
                :
                linearPoint{},
                circularPoint{},
                distance(static_cast<T>(0)),
                sqrDistance(static_cast<T>(0))
            {
                auto output = DCPQuery<T, Vector3<T>, Arc3<T>>{}(point, arc);
                linearPoint = output.closest[0];
                circularPoint = output.closest[1];
                distance = output.distance;
                sqrDistance = output.sqrDistance;
            }

            Candidate(Critical const& critical, std::size_t index)
                :
                linearPoint(critical.linearPoint[index]),
                circularPoint(critical.circularPoint[index]),
                distance(critical.distance[index]),
                sqrDistance(distance* distance)
            {
            }

            bool operator==(Candidate const& other) const
            {
                return linearPoint == other.linearPoint
                    && circularPoint == other.circularPoint
                    && distance == other.distance
                    && sqrDistance == other.sqrDistance;
            }

            bool operator!=(Candidate const& other) const
            {
                return !operator==(other);
            }

            bool operator<(Candidate const& other) const
            {
                return distance < other.distance;
            }

            Vector3<T> linearPoint, circularPoint;
            T distance, sqrDistance;
        };

        // TODO: We should be able to determine an upper bound on size at
        // compile time, allowing us to use a std::array instead.
        using Candidates = std::vector<Candidate>;

        static void GetOutputFromCandidates(Candidates& candidates,
            Output& output)
        {
            // Sort the candidates by distance and eliminate duplicates.
            std::sort(candidates.begin(), candidates.end());
            auto end = std::unique(candidates.begin(), candidates.end());
            candidates.erase(end, candidates.end());

            // Copy the relevant information for closest pairs to the output.
            output.numClosestPairs = 1;
            output.linearClosest[0] = candidates[0].linearPoint;
            output.circularClosest[0] = candidates[0].circularPoint;
            output.distance = candidates[0].distance;
            output.sqrDistance = candidates[0].sqrDistance;
            output.equidistant = false;

            for (std::size_t i = 1; i < candidates.size(); ++i)
            {
                if (candidates[i].distance == output.distance)
                {
                    // TODO: Handle the duplicate points here? Numerical
                    // rounding errors can make it difficult to decide what is
                    // (and what is not) duplicated.
                    //
                    // Perhaps Candidate needs a bit flag that stores the
                    // point type (arc endpoint, segment endpoint, and
                    // or critical point).
                    output.linearClosest[i] = candidates[i].linearPoint;
                    output.circularClosest[i] = candidates[i].circularPoint;
                    ++output.numClosestPairs;
                }
                else
                {
                    // The candidates are ordered by distance. The first time
                    // this block is encounted, the current and all later
                    // candidates cannot be closest pairs.
                    break;
                }
            }
        }

    private:
        friend class UnitTestLinearCircularSupport3;
    };
}
