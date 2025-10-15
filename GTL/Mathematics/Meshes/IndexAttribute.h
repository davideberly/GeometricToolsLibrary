// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.15

#pragma once

// The IndexAttribute class represents an array of triples of indices into a
// vertex array for an indexed triangle mesh. For now, the source must be
// either std::uint16_t or std::uint32_t.

#include <GTL/Utility/Exceptions.h>
#include <cstddef>
#include <cstdint>

namespace gtl
{
    struct IndexAttribute
    {
        // Construction.
        inline IndexAttribute(void* inSource = nullptr, std::size_t inSize = 0)
            :
            source(inSource),
            size(inSize)
        {
        }

        // Triangle access.
        inline void SetTriangle(std::uint32_t t, std::uint32_t v0, std::uint32_t v1, std::uint32_t v2)
        {
            if (size == sizeof(std::uint32_t))
            {
                std::uint32_t* index = reinterpret_cast<std::uint32_t*>(source) + 3 * static_cast<std::size_t>(t);
                index[0] = v0;
                index[1] = v1;
                index[2] = v2;
                return;
            }

            if (size == sizeof(std::uint16_t))
            {
                uint16_t* index = reinterpret_cast<uint16_t*>(source) + 3 * static_cast<std::size_t>(t);
                index[0] = static_cast<uint16_t>(v0);
                index[1] = static_cast<uint16_t>(v1);
                index[2] = static_cast<uint16_t>(v2);
                return;
            }

            GTL_RUNTIME_ERROR("Unsupported index type.");
        }

        inline void GetTriangle(std::uint32_t t, std::uint32_t& v0, std::uint32_t& v1, std::uint32_t& v2) const
        {
            if (size == sizeof(std::uint32_t))
            {
                std::uint32_t* index = reinterpret_cast<std::uint32_t*>(source) + 3 * static_cast<std::size_t>(t);
                v0 = index[0];
                v1 = index[1];
                v2 = index[2];
                return;
            }

            if (size == sizeof(std::uint16_t))
            {
                std::uint16_t* index = reinterpret_cast<std::uint16_t*>(source) + 3 * static_cast<std::size_t>(t);
                v0 = static_cast<std::uint32_t>(index[0]);
                v1 = static_cast<std::uint32_t>(index[1]);
                v2 = static_cast<std::uint32_t>(index[2]);
                return;
            }

            GTL_RUNTIME_ERROR("Unsupported index type.");
        }

        // The source pointer must be 4-byte aligned, which is guaranteed on
        // 32-bit and 64-bit architectures.  The size is the number of bytes
        // per index.
        void* source;
        std::size_t size;
    };
}
