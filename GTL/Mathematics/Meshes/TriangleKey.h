// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// TriangleKey is used for sets, maps and other container classes associated
// with meshes. The containers can be ordered (comparison predicates used) or
// unordered (hashing used).
// 
// An ordered triangle has V[0] = min(v0,v1,v2). Choose (V[0], V[1], V[2])
// to be a permutation of (v0,v1,v2) so that it is one of (v0,v1,v2),
// (v1,v2,v0) or (v2,v0,v1). The idea is that if v0 corresponds to (1,0,0),
// v1 corresponds to (0,1,0) and v2 corresponds to (0,0,1), the ordering
// (v0,v1,v2) corresponds to the 3x3 identity matrix I; the rows are the
// specified 3-tuples. The permutation (V[0],V[1],V[2]) induces a
// permutation of the rows of the identity matrix to form a permutation
// matrix P with det(P) = 1 = det(I).
//
// An unordered triangle stores a permutation of (v0,v1,v2) so that
// V[0] < V[1] < V[2].

#include <GTL/Mathematics/Meshes/FeatureKey.h>
#include <GTL/Utility/TypeTraits.h>
#include <algorithm>
#include <cstddef>

namespace gtl
{
    template <bool Ordered>
    class TriangleKey : public FeatureKey<3, Ordered>
    {
    public:
        // Initialize to invalid indices.
        TriangleKey() = default;

        // This constructor is specialized based on Ordered.
        TriangleKey(std::size_t v0, std::size_t v1, std::size_t v2)
        {
            Initialize(v0, v1, v2);
        }

    private:
        // TriangleKey<true>
        template <bool _Ordered = Ordered, TraitSelector<_Ordered> = 0>
        void Initialize(std::size_t v0, std::size_t v1, std::size_t v2)
        {
            if (v0 < v1)
            {
                if (v0 < v2)
                {
                    // v0 is minimum
                    this->mVertexIndex[0] = v0;
                    this->mVertexIndex[1] = v1;
                    this->mVertexIndex[2] = v2;
                }
                else
                {
                    // v2 is minimum
                    this->mVertexIndex[0] = v2;
                    this->mVertexIndex[1] = v0;
                    this->mVertexIndex[2] = v1;
                }
            }
            else
            {
                if (v1 < v2)
                {
                    // v1 is minimum
                    this->mVertexIndex[0] = v1;
                    this->mVertexIndex[1] = v2;
                    this->mVertexIndex[2] = v0;
                }
                else
                {
                    // v2 is minimum
                    this->mVertexIndex[0] = v2;
                    this->mVertexIndex[1] = v0;
                    this->mVertexIndex[2] = v1;
                }
            }
        }

        // TriangleKey<false>
        template <bool _Ordered = Ordered, TraitSelector<!_Ordered> = 0>
        void Initialize(std::size_t v0, std::size_t v1, std::size_t v2)
        {
            if (v0 < v1)
            {
                if (v0 < v2)
                {
                    // v0 is minimum
                    this->mVertexIndex[0] = v0;
                    this->mVertexIndex[1] = std::min(v1, v2);
                    this->mVertexIndex[2] = std::max(v1, v2);
                }
                else
                {
                    // v2 is minimum
                    this->mVertexIndex[0] = v2;
                    this->mVertexIndex[1] = std::min(v0, v1);
                    this->mVertexIndex[2] = std::max(v0, v1);
                }
            }
            else
            {
                if (v1 < v2)
                {
                    // v1 is minimum
                    this->mVertexIndex[0] = v1;
                    this->mVertexIndex[1] = std::min(v2, v0);
                    this->mVertexIndex[2] = std::max(v2, v0);
                }
                else
                {
                    // v2 is minimum
                    this->mVertexIndex[0] = v2;
                    this->mVertexIndex[1] = std::min(v0, v1);
                    this->mVertexIndex[2] = std::max(v0, v1);
                }
            }
        }

    private:
        friend class UnitTestTriangleKey;
    };
}
