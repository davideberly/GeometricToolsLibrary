// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

#include <GTL/Applications/Window2.h>
#include <GTL/Mathematics/Primitives/ND/Line.h>
#include <GTL/Mathematics/Primitives/ND/OrientedBox.h>
using namespace gtl;

class MinimumAreaBox2DWindow2 : public Window2
{
public:
    MinimumAreaBox2DWindow2(Parameters& parameters);

    virtual void OnDisplay() override;
    virtual void DrawScreenOverlay() override;

private:
    bool SetEnvironment();

    std::vector<Vector2<float>> mVertices;
    OrientedBox2<float> mMinimalAreaBox, mMinimalWidthBox;
    std::vector<size_t> mHull;
};
