// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

#include <GTL/Applications/Window2.h>
#include <GTL/Mathematics/Curves/NURBSCurve.h>
using namespace gtl;

class NURBSCurveExampleWindow2 : public Window2
{
public:
    NURBSCurveExampleWindow2(Parameters& parameters);

    virtual void OnDisplay() override;
    virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y) override;

private:
    void DoSimulation1();
    void DoSimulation2();
    void InitialConfiguration();
    void NextConfiguration();

    inline void Get(Vector2<float> const& position, int32_t& x, int32_t& y) const
    {
        x = static_cast<int32_t>(position[0] + 0.5f);
        y = mSize - 1 - static_cast<int32_t>(position[1] + 0.5f);
    }

    std::shared_ptr<NURBSCurve<float, 2>> mSpline;
    std::shared_ptr<NURBSCurve<float, 2>> mCircle;
    std::vector<Vector2<float>> mControls;
    std::vector<Vector2<float>> mTargets;
    int32_t mSize;
    float mH, mD;
    float mSimTime, mSimDelta;
    bool mDrawControlPoints;
};
