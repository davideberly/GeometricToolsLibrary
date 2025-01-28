// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.28

#pragma once

#include <GTL/Applications/Window2.h>
#include <GTL/Mathematics/Geometry/2D/ConstrainedDelaunay2.h>
using namespace gtl;

// After the program launches, press the key '0' to see a constrained edge
// inserted into the triangulation.  Then press key '1', and then press
// key '2'.

class ConstrainedDelaunay2DWindow2 : public Window2
{
public:
    ConstrainedDelaunay2DWindow2(Parameters& parameters);

    virtual void OnDisplay() override;
    virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y) override;

private:
    std::vector<Vector2<float>> mVertices;
    ConstrainedDelaunay2<float> mDelaunay;
};
