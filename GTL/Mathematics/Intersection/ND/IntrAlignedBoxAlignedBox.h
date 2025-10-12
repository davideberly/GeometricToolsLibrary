// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.12

#pragma once

// The queries consider the box to be a solid. The aligned-aligned queries use
// simple min-max comparisions. The interesection of aligned boxes is an
// aligned box, possibly degenerate, where min[d] == max[d] for at least one
// dimension d.

#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <GTL/Mathematics/Primitives/ND/AlignedBox.h>
#include <cstddef>

namespace gtl
{
    template <typename T, std::size_t N>
    class TIQuery<T, AlignedBox<T, N>, AlignedBox<T, N>>
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

        Output operator()(AlignedBox<T, N> const& box0, AlignedBox<T, N> const& box1)
        {
            Output output{};
            for (std::size_t i = 0; i < N; ++i)
            {
                if (box0.max[i] < box1.min[i] || box0.min[i] > box1.max[i])
                {
                    output.intersect = false;
                    return output;
                }
            }
            output.intersect = true;
            return output;
        }

    private:
        friend class UnitTestIntrAlignedBoxAlignedBox;
    };

    template <typename T, std::size_t N>
    class FIQuery<T, AlignedBox<T, N>, AlignedBox<T, N>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                box{}
            {
            }

            bool intersect;
            AlignedBox<T, N> box;
        };

        Output operator()(AlignedBox<T, N> const& box0, AlignedBox<T, N> const& box1)
        {
            Output output{};
            for (std::size_t i = 0; i < N; i++)
            {
                if (box0.max[i] < box1.min[i] || box0.min[i] > box1.max[i])
                {
                    output.intersect = false;
                    return output;
                }
            }

            for (std::size_t i = 0; i < N; i++)
            {
                if (box0.max[i] <= box1.max[i])
                {
                    output.box.max[i] = box0.max[i];
                }
                else
                {
                    output.box.max[i] = box1.max[i];
                }

                if (box0.min[i] <= box1.min[i])
                {
                    output.box.min[i] = box1.min[i];
                }
                else
                {
                    output.box.min[i] = box0.min[i];
                }
            }
            output.intersect = true;
            return output;
        }

    private:
        friend class UnitTestIntrAlignedBoxAlignedBox;
    };
}
