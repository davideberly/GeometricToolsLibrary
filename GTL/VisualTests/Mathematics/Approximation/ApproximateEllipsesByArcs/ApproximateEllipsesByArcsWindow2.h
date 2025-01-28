// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.28

#pragma once

#include <GTL/Applications/Window2.h>
#include <GTL/Mathematics/Approximation/2D/ApprEllipseByArcs.h>
using namespace gtl;

class ApproximateEllipsesByArcsWindow2 : public Window2
{
public:
    ApproximateEllipsesByArcsWindow2(Parameters& parameters);

    virtual void OnDisplay() override;
    virtual void DrawScreenOverlay() override;
    virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y) override;

private:
    ApprEllipseByArcs<float> mFitter;

    // The ellipse extents 'a' and 'b' in (x/a)^2 + (y/b)^2 = 1.
    float mA, mB;

    // The arcs that approximate the ellipse.
    int32_t mNumArcs;
    std::vector<Vector2<float>> mPoints, mCenters;
    std::vector<float> mRadii;

    // For mapping ellipse points to screen coordinates.
    float mMultiplier;
    Vector2<float> mOffset;
};
