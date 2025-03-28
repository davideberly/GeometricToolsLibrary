// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

#include <GTL/Applications/Window2.h>
#include <GTL/Mathematics/Geometry/2D/Delaunay2.h>
using namespace gtl;

class Delaunay2DWindow2 : public Window2
{
public:
    Delaunay2DWindow2(Parameters& parameters);

    virtual void OnDisplay() override;

private:
    std::vector<Vector2<float>> mVertices;
    std::vector<int32_t> mHull;
    Delaunay2<float> mDelaunay;
    int32_t mCurrentTriX, mCurrentTriY;
};
