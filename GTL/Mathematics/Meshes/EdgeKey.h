// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// EdgeKey is used for sets, maps and other container classes associated with
// meshes. The containers can be ordered (comparison predicates used) or
// unordered (hashing used). An ordered edge has (V[0], V[1]) = (v0, v1).
// An unordered edge has (V[0], V[1]) = (min(V[0],V[1]), max(V[0],V[1])).

#include <GTL/Mathematics/Meshes/FeatureKey.h>
#include <GTL/Utility/TypeTraits.h>
#include <cstddef>

namespace gtl
{
    template <bool Ordered>
    class EdgeKey : public FeatureKey<2, Ordered>
    {
    public:
        // Initialize to invalid indices.
        EdgeKey() = default;

        // This constructor is specialized based on Ordered.
        EdgeKey(std::size_t v0, std::size_t v1)
        {
            Initialize(v0, v1);
        }

    private:
        // EdgeKey<true>
        template <bool _Ordered = Ordered, TraitSelector<_Ordered> = 0>
        void Initialize(std::size_t v0, std::size_t v1)
        {
            this->mVertexIndex[0] = v0;
            this->mVertexIndex[1] = v1;
        }

        // EdgeKey<false>
        template <bool _Ordered = Ordered, TraitSelector<!_Ordered> = 0>
        void Initialize(std::size_t v0, std::size_t v1)
        {
            if (v0 < v1)
            {
                // v0 is minimum
                this->mVertexIndex[0] = v0;
                this->mVertexIndex[1] = v1;
            }
            else
            {
                // v1 is minimum
                this->mVertexIndex[0] = v1;
                this->mVertexIndex[1] = v0;
            }
        }

    private:
        friend class UnitTestEdgeKey;
    };
}
