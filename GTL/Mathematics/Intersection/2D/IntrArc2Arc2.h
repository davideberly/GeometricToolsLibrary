// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

#include <GTL/Mathematics/Intersection/2D/IntrCircle2Circle2.h>
#include <GTL/Mathematics/Primitives/2D/Arc2.h>
#include <array>
#include <cstddef>
#include <limits>

namespace gtl
{
    template <typename T>
    class FIQuery<T, Arc2<T>, Arc2<T>>
    {
    public:
        // The possible 'configuration' in Output are listed as an
        // enumeration.  The valid array elements are listed in the comments.
        enum class Configuration
        {
            NO_INTERSECTION,
            NONCOCIRCULAR_ONE_POINT,        // point[0]
            NONCOCIRCULAR_TWO_POINTS,       // point[0], point[1]
            COCIRCULAR_ONE_POINT,           // point[0]
            COCIRCULAR_TWO_POINTS,          // point[0], point[1]
            COCIRCULAR_ONE_POINT_ONE_ARC,   // point[0], arc[0]
            COCIRCULAR_ONE_ARC,             // arc[0]
            COCIRCULAR_TWO_ARCS             // arc[0], arc[1]
        };

        struct Output
        {
            Output()
                :
                intersect(false),
                configuration(Configuration::NO_INTERSECTION),
                point{},
                arc{}
            {
            }

            // 'true' iff configuration != NO_INTERSECTION
            bool intersect;

            // one of the enumerations listed previously
            Configuration configuration;

            std::array<Vector2<T>, 2> point;
            std::array<Arc2<T>, 2> arc;
        };

        // See Arc2.h for a description of epsilon >= 0.
        Output operator()(Arc2<T> const& arc0, Arc2<T> const& arc1, T const& epsilon)
        {
            // Assume initially there are no intersections.  If we find at
            // least one intersection, we will set output.intersect to 'true'.
            Output output{};

            Circle2<T> circle0(arc0.center, arc0.radius);
            Circle2<T> circle1(arc1.center, arc1.radius);
            FIQuery<T, Circle2<T>, Circle2<T>> ccQuery{};
            auto ccOutput = ccQuery(circle0, circle1);
            if (!ccOutput.intersect)
            {
                // The arcs do not intersect.
                output.configuration = Configuration::NO_INTERSECTION;
                return output;
            }

            if (ccOutput.numIntersections == std::numeric_limits<std::size_t>::max())
            {
                // The arcs are cocircular. Determine whether they overlap.
                // Let arc0 be <A0,A1> and arc1 be <B0,B1>. The points are
                // ordered counterclockwise around the circle of the arc.
                if (arc1.Contains(arc0.end[0], epsilon))
                {
                    output.intersect = true;
                    if (arc1.Contains(arc0.end[1], epsilon))
                    {
                        if (arc0.Contains(arc1.end[0], epsilon) && arc0.Contains(arc1.end[1], epsilon))
                        {
                            if (arc0.end[0] == arc1.end[0] && arc0.end[1] == arc1.end[1])
                            {
                                // The arcs are the same.
                                output.configuration = Configuration::COCIRCULAR_ONE_ARC;
                                output.arc[0] = arc0;
                            }
                            else
                            {
                                // arc0 and arc1 overlap in two disjoint
                                // subsets.
                                if (arc0.end[0] != arc1.end[1])
                                {
                                    if (arc1.end[0] != arc0.end[1])
                                    {
                                        // The arcs overlap in two disjoint
                                        // subarcs, each of positive subtended
                                        // angle: <A0,B1>, <A1,B0>
                                        output.configuration = Configuration::COCIRCULAR_TWO_ARCS;
                                        output.arc[0] = Arc2<T>(arc0.center, arc0.radius, { arc0.end[0], arc1.end[1] });
                                        output.arc[1] = Arc2<T>(arc0.center, arc0.radius, { arc1.end[0], arc0.end[1] });
                                    }
                                    else  // B0 = A1
                                    {
                                        // The intersection is a point {A1}
                                        // and an arc <A0,B1>.
                                        output.configuration = Configuration::COCIRCULAR_ONE_POINT_ONE_ARC;
                                        output.point[0] = arc0.end[1];
                                        output.arc[0] = Arc2<T>(arc0.center, arc0.radius, { arc0.end[0], arc1.end[1] });
                                    }
                                }
                                else  // A0 = B1
                                {
                                    if (arc1.end[0] != arc0.end[1])
                                    {
                                        // The intersection is a point {A0}
                                        // and an arc <A1,B0>.
                                        output.configuration = Configuration::COCIRCULAR_ONE_POINT_ONE_ARC;
                                        output.point[0] = arc0.end[0];
                                        output.arc[0] = Arc2<T>(arc0.center, arc0.radius, { arc1.end[0], arc0.end[1] });
                                    }
                                    else
                                    {
                                        // The arcs shared endpoints, so the
                                        // union is a circle.
                                        output.configuration = Configuration::COCIRCULAR_TWO_POINTS;
                                        output.point[0] = arc0.end[0];
                                        output.point[1] = arc0.end[1];
                                    }
                                }
                            }
                        }
                        else
                        {
                            // Arc0 inside arc1, <B0,A0,A1,B1>.
                            output.configuration = Configuration::COCIRCULAR_ONE_ARC;
                            output.arc[0] = arc0;
                        }
                    }
                    else
                    {
                        if (arc0.end[0] != arc1.end[1])
                        {
                            // Arc0 and arc1 overlap, <B0,A0,B1,A1>.
                            output.configuration = Configuration::COCIRCULAR_ONE_ARC;
                            output.arc[0] = Arc2<T>(arc0.center, arc0.radius, { arc0.end[0], arc1.end[1] });
                        }
                        else
                        {
                            // Arc0 and arc1 share endpoint, <B0,A0,B1,A1>
                            // with A0 = B1.
                            output.configuration = Configuration::COCIRCULAR_ONE_POINT;
                            output.point[0] = arc0.end[0];
                        }
                    }
                    return output;
                }

                if (arc1.Contains(arc0.end[1], epsilon))
                {
                    output.intersect = true;
                    if (arc0.end[1] != arc1.end[0])
                    {
                        // Arc0 and arc1 overlap in a single arc,
                        // <A0,B0,A1,B1>.
                        output.configuration = Configuration::COCIRCULAR_ONE_ARC;
                        output.arc[0] = Arc2<T>(arc0.center, arc0.radius, { arc1.end[0], arc0.end[1] });
                    }
                    else
                    {
                        // Arc0 and arc1 share endpoint, <A0,B0,A1,B1>
                        // with B0 = A1.
                        output.configuration = Configuration::COCIRCULAR_ONE_POINT;
                        output.point[0] = arc1.end[0];
                    }
                    return output;
                }

                if (arc0.Contains(arc1.end[0], epsilon))
                {
                    // Arc1 inside arc0, <A0,B0,B1,A1>.
                    output.intersect = true;
                    output.configuration = Configuration::COCIRCULAR_ONE_ARC;
                    output.arc[0] = arc1;
                }
                else
                {
                    // Arcs do not overlap, <A0,A1,B0,B1>.
                    output.configuration = Configuration::NO_INTERSECTION;
                }
                return output;
            }

            // Test whether circle-circle intersection points are on the arcs.
            std::size_t numIntersections = 0;
            for (std::size_t i = 0; i < ccOutput.numIntersections; ++i)
            {
                if (arc0.Contains(ccOutput.point[i], epsilon) &&
                    arc1.Contains(ccOutput.point[i], epsilon))
                {
                    output.point[numIntersections] = ccOutput.point[i];
                    ++numIntersections;
                    output.intersect = true;
                }
            }

            if (numIntersections == 2)
            {
                output.configuration = Configuration::NONCOCIRCULAR_TWO_POINTS;
            }
            else if (numIntersections == 1)
            {
                output.configuration = Configuration::NONCOCIRCULAR_ONE_POINT;
            }
            else
            {
                output.configuration = Configuration::NO_INTERSECTION;
            }

            return output;
        }

    private:
        friend class UnitTestIntrArc2Arc2;
    };
}
