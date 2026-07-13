// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

#pragma once

// Morphological operations on images. Class Morphology is the abstract base
// class for Morphology2 and Morphology3.

#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <vector>

namespace gtl
{
    // The parameter SInteger must be either std::int32_t or std::int64_t.
    template <typename SInteger>
    class Morphology
    {
    public:
        using OffsetType = std::conditional<sizeof(std::size_t) == 8, std::int64_t, std::int32_t>::type;

    protected:
        // Connected component labeling using depth-first search.
        static void GetComponents(std::size_t numNeighbors, OffsetType const* neighbors,
            std::size_t numPixels, SInteger* image, std::vector<std::vector<std::size_t>>& components)
        {
            static_assert(
                std::is_same<SInteger, std::int32_t>::value ||
                std::is_same<SInteger, std::int64_t>::value,
                "SInteger must be std::int32_t or std::int64_t.");

            std::vector<std::size_t> numElements(numPixels);
            std::vector<std::size_t> vstack(numPixels);
            std::size_t numComponents = 0;
            SInteger label = 2;
            for (std::size_t i = 0; i < numPixels; ++i)
            {
                if (image[i] == 1)
                {
                    std::size_t top = std::numeric_limits<std::size_t>::max();
                    vstack[++top] = i;

                    std::size_t& count = numElements[numComponents + 1];
                    count = 0;
                    while (top != std::numeric_limits<std::size_t>::max())
                    {
                        std::size_t v = vstack[top];
                        image[v] = -1;
                        std::size_t j;
                        for (j = 0; j < numNeighbors; ++j)
                        {
                            std::size_t adj = v + neighbors[j];
                            if (image[adj] == 1)
                            {
                                vstack[++top] = adj;
                                break;
                            }
                        }
                        if (j == numNeighbors)
                        {
                            image[v] = label;
                            ++count;
                            --top;
                        }
                    }

                    ++numComponents;
                    ++label;
                }
            }

            if (numComponents > 0)
            {
                components.resize(numComponents + 1);
                for (std::size_t i = 1; i <= numComponents; ++i)
                {
                    components[i].resize(numElements[i]);
                    numElements[i] = 0;
                }

                for (std::size_t i = 0; i < numPixels; ++i)
                {
                    SInteger value = image[i];
                    if (value != 0)
                    {
                        // Labels started at 2 to support the depth-first
                        // search, so they need to be decremented for the
                        // correct labels.
                        image[i] = --value;
                        components[value][numElements[value]] = i;
                        ++numElements[value];
                    }
                }
            }
        }

    private:
        friend class UnitTestMorphology;
    };
}
