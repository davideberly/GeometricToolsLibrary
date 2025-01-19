// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Effects/Font.h>

namespace gtl
{
    class FontArialW400H14 : public Font
    {
    public:
        virtual ~FontArialW400H14() = default;
        FontArialW400H14(std::shared_ptr<ProgramFactory> const& factory, int32_t maxMessageLength);

    private:
        static int32_t msWidth;
        static int32_t msHeight;
        static uint8_t msTexels[];
        static float msCharacterData[];
    };
}
