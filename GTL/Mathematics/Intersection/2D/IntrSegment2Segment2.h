// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

#include <GTL/Mathematics/Intersection/1D/IntrIntervals.h>
#include <GTL/Mathematics/Intersection/2D/IntrLine2Line2.h>
#include <GTL/Mathematics/Primitives/ND/Segment.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Segment2<T>, Segment2<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0)
            {
            }

            // The number is 0 (no intersection), 1 (segments intersect in a
            // single point), or 2 (segments are collinear and intersect in a
            // segment).
            bool intersect;
            std::size_t numIntersections;
        };

        // This version of the query uses Segment3<T>::GetCenteredForm, which
        // has a Normalize call. This generates rounding errors, so the query
        // should be used only with float or double.
        Output operator()(Segment2<T> const& segment0, Segment2<T> const& segment1)
        {
            Output output{};
            Vector2<T> seg0Origin{}, seg0Direction{}, seg1Origin{}, seg1Direction{};
            T seg0Extent{}, seg1Extent{};
            segment0.GetCenteredForm(seg0Origin, seg0Direction, seg0Extent);
            segment1.GetCenteredForm(seg1Origin, seg1Direction, seg1Extent);

            FIQuery<T, Line2<T>, Line2<T>> llQuery{};
            Line2<T> line0(seg0Origin, seg0Direction);
            Line2<T> line1(seg1Origin, seg1Direction);
            auto llOutput = llQuery(line0, line1);
            if (llOutput.numIntersections == 1)
            {
                // Test whether the line-line intersection is on the segments.
                if (std::fabs(llOutput.line0Parameter[0]) <= seg0Extent &&
                    std::fabs(llOutput.line1Parameter[0]) <= seg1Extent)
                {
                    output.intersect = true;
                    output.numIntersections = 1;
                }
                else
                {
                    output.intersect = false;
                    output.numIntersections = 0;
                }
            }
            else if (llOutput.numIntersections == std::numeric_limits<std::size_t>::max())
            {
                // Compute the location of segment1 endpoints relative to
                // segment0.
                Vector2<T> diff = seg1Origin - seg0Origin;
                T t = Dot(seg0Direction, diff);

                // Get the parameter intervals of the segments relative to
                // segment0.
                std::array<T, 2> interval0 = { -seg0Extent, seg0Extent };
                std::array<T, 2> interval1 = { t - seg1Extent, t + seg1Extent };

                // Compute the intersection of the intervals.
                FIQuery<T, std::array<T, 2>, std::array<T, 2>> iiQuery{};
                auto iiOutput = iiQuery(interval0, interval1);
                output.intersect = iiOutput.intersect;
                output.numIntersections = iiOutput.numIntersections;
            }
            else
            {
                output.intersect = false;
                output.numIntersections = 0;
            }

            return output;
        }

        // This version of the query supports rational arithmetic.
        Output Exact(Segment2<T> const& segment0, Segment2<T> const& segment1)
        {
            Output output{};

            FIQuery<T, Line2<T>, Line2<T>> llQuery{};
            Vector2<T> seg0Direction = segment0.p[1] - segment0.p[0];
            Vector2<T> seg1Direction = segment1.p[1] - segment1.p[0];
            Line2<T> line0(segment0.p[0], seg0Direction);
            Line2<T> line1(segment1.p[0], seg1Direction);
            auto llOutput = llQuery(line0, line1);
            if (llOutput.numIntersections == 1)
            {
                // The lines are not parallel, so they intersect in a single
                // point. Test whether the line-line intersection is on the
                // segments.
                if (C_<T>(0) <= llOutput.line0Parameter[0] && llOutput.line0Parameter[1] <= C_<T>(1) &&
                    C_<T>(0) <= llOutput.line1Parameter[0] && llOutput.line1Parameter[1] <= C_<T>(1))
                {
                    output.intersect = true;
                    output.numIntersections = 1;
                }
                else
                {
                    output.intersect = false;
                    output.numIntersections = 0;
                }
            }
            else if (llOutput.numIntersections == std::numeric_limits<std::size_t>::max())
            {
                // The lines are the same. Compute the location of segment1
                // endpoints relative to segment0.
                T dotD0D0 = Dot(seg0Direction, seg0Direction);
                Vector2<T> diff = segment1.p[0] - segment0.p[0];
                T t0 = Dot(seg0Direction, diff) / dotD0D0;
                diff = segment1.p[1] - segment0.p[0];
                T t1 = Dot(seg0Direction, diff) / dotD0D0;

                // Get the parameter intervals of the segments relative to
                // segment0.
                std::array<T, 2> interval0 = { C_<T>(0), C_<T>(1) };
                std::array<T, 2> interval1{};
                if (t1 >= t0)
                {
                    interval1[0] = t0;
                    interval1[1] = t1;
                }
                else
                {
                    interval1[0] = t1;
                    interval1[1] = t0;
                }

                // Compute the intersection of the intervals.
                FIQuery<T, std::array<T, 2>, std::array<T, 2>> iiQuery{};
                auto iiOutput = iiQuery(interval0, interval1);
                output.intersect = iiOutput.intersect;
                output.numIntersections = iiOutput.numIntersections;
            }
            else
            {
                // The lines are parallel but not the same, so the segments
                // cannot intersect.
                output.intersect = false;
                output.numIntersections = 0;
            }

            return output;
        }

    private:
        friend class UnitTestIntrSegment2Segment2;
    };

    template <typename T>
    class FIQuery<T, Segment2<T>, Segment2<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0),
                segment0Parameter{ C_<T>(0), C_<T>(0) },
                segment1Parameter{ C_<T>(0), C_<T>(0) },
                point{}
            {
            }

            // The number is 0 (no intersection), 1 (segments intersect in a
            // a single point), or 2 (segments are collinear and intersect
            // in a segment).
            bool intersect;
            std::size_t numIntersections;

            // If numIntersections is 1, the intersection is
            //   point[0]
            //   = segment0.origin + segment0Parameter[0] * segment0.direction
            //   = segment1.origin + segment1Parameter[0] * segment1.direction
            // If numIntersections is 2, the endpoints of the segment of
            // intersection are
            //   point[i]
            //   = segment0.origin + segment0Parameter[i] * segment0.direction
            //   = segment1.origin + segment1Parameter[i] * segment1.direction
            // with segment0Parameter[0] <= segment0Parameter[1] and
            // segment1Parameter[0] <= segment1Parameter[1].
            std::array<T, 2> segment0Parameter, segment1Parameter;
            std::array<Vector2<T>, 2> point;
        };

        // This version of the query uses Segment3<T>::GetCenteredForm, which
        // has a Normalize call. This generates rounding errors, so the query
        // should be used only with float or double. NOTE: The parameters are
        // are relative to the centered form of the segment. Each segment has
        // a center C, a unit-length direction D and an extent e > 0. A
        // segment point is C+t*D where |t| <= e.
        Output operator()(Segment2<T> const& segment0, Segment2<T> const& segment1)
        {
            Output output{};
            Vector2<T> seg0Origin{}, seg0Direction{}, seg1Origin{}, seg1Direction{};
            T seg0Extent{}, seg1Extent{};
            segment0.GetCenteredForm(seg0Origin, seg0Direction, seg0Extent);
            segment1.GetCenteredForm(seg1Origin, seg1Direction, seg1Extent);

            FIQuery<T, Line2<T>, Line2<T>> llQuery{};
            Line2<T> line0(seg0Origin, seg0Direction);
            Line2<T> line1(seg1Origin, seg1Direction);
            auto llOutput = llQuery(line0, line1);
            if (llOutput.numIntersections == 1)
            {
                // Test whether the line-line intersection is on the segments.
                if (std::fabs(llOutput.line0Parameter[0]) <= seg0Extent &&
                    std::fabs(llOutput.line1Parameter[0]) <= seg1Extent)
                {
                    output.intersect = true;
                    output.numIntersections = 1;
                    output.segment0Parameter[0] = llOutput.line0Parameter[0];
                    output.segment0Parameter[1] = output.segment0Parameter[0];
                    output.segment1Parameter[0] = llOutput.line1Parameter[0];
                    output.segment1Parameter[1] = output.segment1Parameter[0];
                    output.point[0] = llOutput.point;
                    output.point[1] = output.point[0];
                }
                else
                {
                    output.intersect = false;
                    output.numIntersections = 0;
                }
            }
            else if (llOutput.numIntersections == std::numeric_limits<std::size_t>::max())
            {
                // Compute the location of segment1 endpoints relative to
                // segment0.
                Vector2<T> diff = seg1Origin - seg0Origin;
                T t = Dot(seg0Direction, diff);

                // Get the parameter intervals of the segments relative to
                // segment0.
                std::array<T, 2> interval0 = { -seg0Extent, seg0Extent };
                std::array<T, 2> interval1 = { t - seg1Extent, t + seg1Extent };

                // Compute the intersection of the intervals.
                FIQuery<T, std::array<T, 2>, std::array<T, 2>> iiQuery{};
                auto iiOutput = iiQuery(interval0, interval1);
                if (iiOutput.intersect)
                {
                    output.intersect = true;
                    output.numIntersections = iiOutput.numIntersections;
                    for (std::size_t i = 0; i < iiOutput.numIntersections; ++i)
                    {
                        output.segment0Parameter[i] = iiOutput.overlap[i];
                        output.segment1Parameter[i] = iiOutput.overlap[i] - t;
                        output.point[i] = seg0Origin + output.segment0Parameter[i] * seg0Direction;
                    }
                    if (iiOutput.numIntersections == 1)
                    {
                        output.segment0Parameter[1] = output.segment0Parameter[0];
                        output.segment1Parameter[1] = output.segment1Parameter[0];
                        output.point[1] = output.point[0];
                    }
                }
                else
                {
                    output.intersect = false;
                    output.numIntersections = 0;
                }
            }
            else
            {
                output.intersect = false;
                output.numIntersections = 0;
            }

            return output;
        }

        // This version of the query supports rational arithmetic. NOTE: The
        // parameters are relative to the endpoint form of the segment. Each
        // segment has endpoints P0 and P1. A segment point is P0+t*(P1-P0)
        // where 0 <= t <= 1.
        Output Exact(Segment2<T> const& segment0, Segment2<T> const& segment1)
        {
            Output output{};

            FIQuery<T, Line2<T>, Line2<T>> llQuery{};
            Vector2<T> seg0Direction = segment0.p[1] - segment0.p[0];
            Vector2<T> seg1Direction = segment1.p[1] - segment1.p[0];
            Line2<T> line0(segment0.p[0], seg0Direction);
            Line2<T> line1(segment1.p[0], seg1Direction);
            auto llOutput = llQuery(line0, line1);
            if (llOutput.numIntersections == 1)
            {
                // The lines are not parallel, so they intersect in a single
                // point. Test whether the line-line intersection is on the
                // segments.
                if (C_<T>(0) <= llOutput.line0Parameter[0] && llOutput.line0Parameter[1] <= C_<T>(1) &&
                    C_<T>(0) <= llOutput.line1Parameter[0] && llOutput.line1Parameter[1] <= C_<T>(1))
                {
                    output.intersect = true;
                    output.numIntersections = 1;
                    output.segment0Parameter[0] = llOutput.line0Parameter[0];
                    output.segment0Parameter[1] = output.segment0Parameter[0];
                    output.segment1Parameter[0] = llOutput.line1Parameter[0];
                    output.segment1Parameter[1] = output.segment1Parameter[0];
                    output.point[0] = llOutput.point;
                    output.point[1] = output.point[0];
                }
                else
                {
                    output.intersect = false;
                    output.numIntersections = 0;
                }
            }
            else if (llOutput.numIntersections == std::numeric_limits<std::size_t>::max())
            {
                // The lines are the same. Compute the location of segment1
                // endpoints relative to segment0.
                T dotD0D0 = Dot(seg0Direction, seg0Direction);
                Vector2<T> diff = segment1.p[0] - segment0.p[0];
                T t0 = Dot(seg0Direction, diff) / dotD0D0;
                diff = segment1.p[1] - segment0.p[0];
                T t1 = Dot(seg0Direction, diff) / dotD0D0;

                // Get the parameter intervals of the segments relative to
                // segment0.
                std::array<T, 2> interval0 = { C_<T>(0), C_<T>(1) };
                std::array<T, 2> interval1{};
                if (t1 >= t0)
                {
                    interval1[0] = t0;
                    interval1[1] = t1;
                }
                else
                {
                    interval1[0] = t1;
                    interval1[1] = t0;
                }

                // Compute the intersection of the intervals.
                FIQuery<T, std::array<T, 2>, std::array<T, 2>> iiQuery{};
                auto iiOutput = iiQuery(interval0, interval1);
                if (iiOutput.intersect)
                {
                    output.intersect = true;
                    output.numIntersections = iiOutput.numIntersections;

                    // Compute the results for segment0.
                    for (std::size_t i = 0; i < iiOutput.numIntersections; ++i)
                    {
                        output.segment0Parameter[i] = iiOutput.overlap[i];
                        output.point[i] = segment0.p[0] + output.segment0Parameter[i] * seg0Direction;
                    }

                    // Compute the results for segment1. The interval1 was
                    // computed relative to segment0, so we have to reverse
                    // the process to obtain the parameters.
                    T dotD1D1 = Dot(seg1Direction, seg1Direction);
                    for (std::size_t i = 0; i < iiOutput.numIntersections; ++i)
                    {
                        diff = output.point[i] - segment1.p[0];
                        output.segment1Parameter[i] = Dot(seg1Direction, diff) / dotD1D1;
                    }

                    if (iiOutput.numIntersections == 1)
                    {
                        output.segment0Parameter[1] = output.segment0Parameter[0];
                        output.segment1Parameter[1] = output.segment1Parameter[0];
                        output.point[1] = output.point[0];
                    }
                    else
                    {
                        if (t1 < t0)
                        {
                            std::swap(
                                output.segment1Parameter[0],
                                output.segment1Parameter[1]);
                        }
                    }
                }
                else
                {
                    output.intersect = false;
                    output.numIntersections = 0;
                }
            }
            else
            {
                output.intersect = false;
                output.numIntersections = 0;
            }

            return output;
        }

    private:
        friend class UnitTestIntrSegment2Segment2;
    };
}
