// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.28

#pragma once

#include <GTL/Applications/Window2.h>
#include <GTL/Mathematics/Curves/BezierCurve.h>
#include <GTL/Mathematics/Primitives/2D/Arc2.h>
#include <functional>
using namespace gtl;

class ApproximateBezierCurveByArcsWindow2 : public Window2
{
public:
    ApproximateBezierCurveByArcsWindow2(Parameters& parameters);

    virtual void OnDisplay() override;
    virtual void DrawScreenOverlay();
    virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y);

private:
    // The client window is [0,512]x[0,512], mXSize = mYSize = 512.
    // Transform [0,4]x[0,5] to client subwindow [32,432]x[32,532] by
    // multiplying the sample coordinates by 100 and adding 32.
    inline void Transform(Vector2<double> const& point, int32_t& x, int32_t& y)
    {
        x = static_cast<int32_t>(100.0 * point[0] + 32.0);
        y = static_cast<int32_t>(100.0 * point[1] + 32.0);
    }

    std::shared_ptr<BezierCurve<double, 2>> mCurve;
    size_t mNumArcs;
    std::vector<double> mTimes;
    std::vector<Vector2<double>> mEndpoints;
    std::vector<Arc2<double>> mArcs;
    bool mDrawCurve, mDrawArcs, mDrawEndpoints, mDrawMidpoints, mDrawText;
};
