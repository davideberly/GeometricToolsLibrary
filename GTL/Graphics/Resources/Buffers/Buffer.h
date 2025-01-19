// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Resources/Resource.h>
#include <cstdint>
#include <functional>

namespace gtl
{
    class Buffer : public Resource
    {
    public:
        // Abstract base class.
        virtual ~Buffer() = default;
    protected:
        Buffer(uint32_t numElements, size_t elementSize, bool createStorage = true);
    };

    typedef std::function<void(std::shared_ptr<Buffer> const&)> BufferUpdater;
}
