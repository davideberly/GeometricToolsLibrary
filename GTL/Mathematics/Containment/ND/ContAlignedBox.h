// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

#include <GTL/Mathematics/Primitives/ND/AlignedBox.h>
#include <algorithm>
#include <cstddef>
#include <vector>

namespace gtl
{
    // Compute the minimum size aligned bounding box of the points. The
    // extreme values are the minima and maxima of the point coordinates.
    template <typename T, std::size_t N>
    void GetContainer(std::vector<Vector<T, N>> const& points, AlignedBox<T, N>& box)
    {
        GTL_ARGUMENT_ASSERT(
            points.size() > 0,
            "At least one input point is required.");

        auto extremes = ComputeExtremes(points);
        box.min = extremes.first;
        box.max = extremes.second;
    }

    // Test for containment.
    template <typename T, std::size_t N>
    bool InContainer(Vector<T, N> const& point, AlignedBox<T, N> const& box)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            T const& value = point[i];
            if (value < box.min[i] || value > box.max[i])
            {
                return false;
            }
        }
        return true;
    }

    // Construct an aligned box that contains two other aligned boxes. The
    // result is the minimum size box containing the input boxes.
    template <typename T, std::size_t N>
    void MergeContainers(AlignedBox<T, N> const& box0, AlignedBox<T, N> const& box1,
        AlignedBox<T, N>& merge)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            merge.min[i] = std::min(box0.min[i], box1.min[i]);
            merge.max[i] = std::max(box0.max[i], box1.max[i]);
        }
    }
}
