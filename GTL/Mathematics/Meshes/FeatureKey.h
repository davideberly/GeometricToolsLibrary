// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// FeatureKey is the base class for EdgeKey, TriangleKey and TetrahedronKey.
// These classes are used for sets, maps and other container classes
// associated with meshes. The containers can be ordered (comparison
// predicates used) or unordered (hashing used).

#include <GTL/Utility/HashCombine.h>
#include <array>
#include <cstddef>
#include <limits>

namespace gtl
{
    template <std::size_t N, bool Ordered>
    class FeatureKey
    {
    public:
        // An ordered feature key has V[0] = min(V[]) with
        // (V[0],V[1],...,V[N-1]) a permutation of N inputs with an even
        // number of transpositions. An unordered feature key has
        // V[0] < V[1] < ... < V[N-1]. Note that Ordered is about the
        // topology of the feature, not the comparison order for any sorting.

        static std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();

        FeatureKey()
            :
            mVertexIndex{}
        {
            mVertexIndex.fill(invalid);
        }

        FeatureKey(std::array<std::size_t, N> const& inVertexIndex)
            :
            mVertexIndex(inVertexIndex)
        {
        }

        // Member access.
        std::size_t const& operator[](std::size_t i) const
        {
            return mVertexIndex[i];
        }

        // WARNING. Giving write access allows you to assign indices, which
        // can change the topological ordering of the members. Be careful to
        // use this member when you know the writes will not change that
        // ordering.
        std::size_t& operator[](std::size_t i)
        {
            return mVertexIndex[i];
        }

        inline operator std::array<std::size_t, N> const& () const
        {
            return mVertexIndex;
        }

        // The C++ Standard Library implementations for std::array can be
        // quite slow in Debug builds. The std::array comparison operators
        // are not used here to allow for better performance when debugging
        // appplications using the mesh classes.
        bool operator==(FeatureKey const& key) const
        {
            for (std::size_t i = 0; i < N; ++i)
            {
                if (mVertexIndex[i] != key.mVertexIndex[i])
                {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(FeatureKey const& key) const
        {
            return !operator==(key);
        }

        bool operator<(FeatureKey const& key) const
        {
            for (std::size_t i = 0; i < N; ++i)
            {
                if (mVertexIndex[i] < key.mVertexIndex[i])
                {
                    return true;
                }
                if (mVertexIndex[i] > key.mVertexIndex[i])
                {
                    return false;
                }
            }
            return false;
        }

        bool operator<=(FeatureKey const& key) const
        {
            return !key.operator<(*this);
        }

        bool operator>(FeatureKey const& key) const
        {
            return key.operator<(*this);
        }

        bool operator>=(FeatureKey const& key) const
        {
            return !operator<(key);
        }

        // Support for hashing in std::unordered*<T> container classes. The
        // first operator() is the hash function. The second operator() is
        // the equality comparison used for elements in the same bucket.
        std::size_t operator()(FeatureKey const& key) const
        {
            std::size_t seed = 0;
            for (auto value : key.mVertexIndex)
            {
                HashCombine(seed, value);
            }
            return seed;
        }

        bool operator()(FeatureKey const& v0, FeatureKey const& v1) const
        {
            return v0 == v1;
        }

        std::array<std::size_t, N> mVertexIndex;

    private:
        friend class UnitTestFeatureKey;
    };
}
