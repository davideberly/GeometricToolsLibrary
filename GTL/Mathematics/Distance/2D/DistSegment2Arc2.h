// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// Compute the distance between a segment and an arc in 2D.
//
// The segment has endpoints P0 and P1 and is parameterized by
// P0 + t * (P1 - P0). The t-value satisfies 0 <= t <= 1.
//
// The circle containing the arc has center C and radius r. The arc has two
// endpoints E0 and E1 on the circle so that E1 is obtained from E0 by
// traversing counterclockwise. The application is responsible for ensuring
// that E0 and E1 are on the circle and that they are properly ordered.
//
// The number of pairs of closest points is output.numClosestPairs which is
// 1 or 2. If output.numClosestPairs is 1, output.parameter[0] is the segment
// t-value for its closest point output.closest[0][0]. The arc closest point
// is output.closest[0][1]. If output.numClosestPairs is 2,
// output.parameter[0] and output.parameter[1] are the segment t-values for
// its closest points output.closest[0][0] and output.closest[1][0]. The arc
// closest points are output.closest[0][1] and output.closest[1][1].

#include <GTL/Mathematics/Distance/2D/DistSegment2Circle2.h>
#include <GTL/Mathematics/Distance/ND/DistPointSegment.h>
#include <GTL/Mathematics/Distance/2D/DistPoint2Arc2.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class DCPQuery<T, Segment2<T>, Arc2<T>>
    {
    public:
        using LCQuery = DCPQuery<T, Line2<T>, Circle2<T>>;
        using Output = typename LCQuery::Output;

        Output operator()(Segment2<T> const& segment, Arc2<T> const& arc)
        {
            // Execute the query for segment-circle. Test whether the circle
            // closest points are on or off the arc. If any closest point is
            // on the arc, there is no need to test arc endpoints for
            // closeness.
            Circle2<T> circle(arc.center, arc.radius);
            auto rcResult = DCPQuery<T, Segment2<T>, Circle2<T>>{}(segment, circle);
            Output output{};
            for (std::size_t i = 0; i < rcResult.numClosestPairs; ++i)
            {
                if (arc.Contains(rcResult.closest[i][1]))
                {
                    std::size_t j = output.numClosestPairs++;
                    output.distance = rcResult.distance;
                    output.sqrDistance = rcResult.sqrDistance;
                    output.parameter[j] = rcResult.parameter[i];
                    output.closest[j][0] = rcResult.closest[i][0];
                    output.closest[j][1] = rcResult.closest[i][1];
                }
            }

            if (output.numClosestPairs > 0)
            {
                // At least one circle closest point is on the arc. There is
                // no need to test arc endpoints.
                return output;
            }

            // No circle closest points are on the arc. Compute distances to
            // the arc endpoints and from segment endpoints to the arc and
            // then select the minima.
            DCPQuery<T, Vector2<T>, Segment2<T>> psQuery{};
            DCPQuery<T, Vector2<T>, Arc2<T>> paQuery{};
            auto psResult0 = psQuery(arc.end[0], segment);
            auto psResult1 = psQuery(arc.end[1], segment);
            auto paResult2 = paQuery(segment.p[0], arc);
            auto paResult3 = paQuery(segment.p[1], arc);

            std::array<SortItem, 4> items{};
            items[0].distance = std::sqrt(psResult0.sqrDistance);
            items[0].sqrDistance = psResult0.sqrDistance;
            items[0].parameter = psResult0.parameter;
            items[0].closest[0] = psResult0.closest[1];
            items[0].closest[1] = arc.end[0];
            items[1].distance = std::sqrt(psResult1.sqrDistance);
            items[1].sqrDistance = psResult1.sqrDistance;
            items[1].parameter = psResult1.parameter;
            items[1].closest[0] = psResult1.closest[1];
            items[1].closest[1] = arc.end[1];
            items[2].distance = paResult2.distance;
            items[2].sqrDistance = paResult2.sqrDistance;
            items[2].parameter = C_<T>(0);
            items[2].closest[0] = paResult2.closest[0];
            items[2].closest[1] = paResult2.closest[1];
            items[3].distance = paResult3.distance;
            items[3].sqrDistance = paResult3.sqrDistance;
            items[3].parameter = C_<T>(1);
            items[3].closest[0] = paResult3.closest[0];
            items[3].closest[1] = paResult3.closest[1];
            std::sort(items.begin(), items.end());

            auto const& item0 = items[0];
            auto const& item1 = items[1];
            if (item0.sqrDistance < item1.sqrDistance ||
                item0.closest[1] == item1.closest[1])
            {
                // The arc point closest to the segment is unique.
                output.distance = item0.distance;
                output.sqrDistance = item0.sqrDistance;
                output.numClosestPairs = 1;
                output.parameter[0] = item0.parameter;
                output.closest[0][0] = item0.closest[0];
                output.closest[0][1] = item0.closest[1];
            }
            else
            {
                // Two arc points are equidistant from the segment.
                output.distance = item0.distance;
                output.sqrDistance = item0.sqrDistance;
                output.numClosestPairs = 2;
                output.parameter[0] = item0.parameter;
                output.parameter[1] = item1.parameter;
                output.closest[0][0] = item0.closest[0];
                output.closest[0][1] = item0.closest[1];
                output.closest[1][0] = item1.closest[0];
                output.closest[1][1] = item1.closest[1];
            }
            return output;
        }

    private:
        struct SortItem
        {
            SortItem()
                :
                distance(C_<T>(0)),
                sqrDistance(C_<T>(0)),
                parameter(C_<T>(0)),
                closest{}
            {
            }

            bool operator< (SortItem const& other) const
            {
                return sqrDistance < other.sqrDistance;
            }

            T distance, sqrDistance;
            T parameter;
            std::array<Vector2<T>, 2> closest;
        };
    };
}
