// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.12

#pragma once

#include <GTL/Mathematics/Primitives/ND/AlignedBox.h>
#include <GTL/Mathematics/Primitives/2D/Arc2.h>
#include <vector>

namespace gtl
{
    template <typename T>
    class ContAlignedBox2
    {
    public:
        // Compute an axis-aligned box of the points. The box sides are
        // computed using the extreme values of the x- and y-coordinates.
        // NOTE: This is a specialization
        //   ContAlignedBox<T,2>::GetContainer(
        //     std::vector<Vector<T,2>> const&, AlignedBox<T,2>&);
        static void GetContainer(std::vector<Vector2<T>> const& points, AlignedBox2<T>& box)
        {
            auto extremes = ComputeExtremes(points);
            box.min = extremes.first;
            box.max = extremes.second;
        }

        // Test for containment of the point in the box.
        // NOTE: This is a specialization
        //   ContAlignedBox<T,2>::InContainer(
        //     Vector<T,2> const&, AlignedBox<T,2>&);
        static bool InContainer(Vector2<T> const& point, AlignedBox2<T> const& box)
        {
            return box.min[0] <= point[0] && point[0] <= box.max[0]
                && box.min[1] <= point[1] && point[1] <= box.max[1];
        }

        // Construct an axis-aligned box that contains two other axis-aligned
        // boxes. The output box sides are computed using the extreme values
        // of the extremes of the x- and y-coordinates.
        // NOTE: This is a specialization
        //   ContAlignedBox<T,2>::MergeContainers(
        //     AlignedBox<T,2> const&, AlignedBox<T,2> const&,
        //     AlignedBox<T,2>&);
        static void MergeContainers(AlignedBox2<T> const& box0,
            AlignedBox2<T> const& box1, AlignedBox2<T>& merge)
        {
            merge.min[0] = std::min(box0.min[0], box1.min[0]);
            merge.max[0] = std::max(box0.max[0], box1.max[0]);
            merge.min[1] = std::min(box0.min[1], box1.min[1]);
            merge.max[1] = std::max(box0.max[1], box1.max[1]);
        }

        // Compute the smallest-area axis-aligned box containing an arc. Let
        // the arc have endpoints E[0] and E[1] and live on a circle with
        // center C and radius r. The extreme circle points in the axis
        // directions are P[0] = C+(r,0), P[1] = C-(r,0), P[2] = C+(0,r), and
        // P[3] = C-(0,r). The box is supported by the E0 and E1 and points
        // P[i] that are on the arc.
        static void GetContainer(Arc2<T> const& arc, AlignedBox2<T>& box)
        {
            // Store the arc endpoints.
            std::vector<Vector2<T>> points{};
            points.push_back(arc.end[0]);
            points.push_back(arc.end[1]);

            // Store the circle points that are on the arc.
            Vector2<T> point = { arc.center[0] + arc.radius, arc.center[1] };
            if (arc.Contains(point))
            {
                points.push_back(point);
            }

            point = { arc.center[0] - arc.radius, arc.center[1] };
            if (arc.Contains(point))
            {
                points.push_back(point);
            }

            point = { arc.center[0], arc.center[1] + arc.radius };
            if (arc.Contains(point))
            {
                points.push_back(point);
            }

            point = { arc.center[0], arc.center[1] - arc.radius };
            if (arc.Contains(point))
            {
                points.push_back(point);
            }

            // Compute the aligned bounding box.
            auto extremes = ComputeExtremes(points);
            box.min = extremes.first;
            box.max = extremes.second;
        }

    private:
        friend class UnitTestContAlignedBox2;
    };
}
