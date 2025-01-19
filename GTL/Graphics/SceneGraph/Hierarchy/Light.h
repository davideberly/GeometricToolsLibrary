// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/SceneGraph/Hierarchy/ViewVolume.h>
#include <GTL/Graphics/Effects/Lighting.h>
#include <memory>

namespace gtl
{
    class Light : public ViewVolume
    {
    public:
        // Construction.  The depth range for DirectX is [0,1] and for OpenGL
        // is [-1,1].  For DirectX, set isDepthRangeZeroToOne to true.  For
        // OpenGL, set isDepthRangeZeroOne to false.
        Light(bool isPerspective, bool isDepthRangeZeroOne);

        std::shared_ptr<Lighting> lighting;
    };
}
