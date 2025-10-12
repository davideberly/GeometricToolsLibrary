// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.12

#pragma once

#include <GTL/Applications/Window2.h>
#include <GTL/Utility/Timer.h>
#include <GTL/Mathematics/Physics/RectangleManager.h>
#include <memory>
#include <random>
#include <vector>
using namespace gtl;

class IntersectingRectanglesWindow2 : public Window2
{
public:
    IntersectingRectanglesWindow2(Parameters& parameters);

    virtual void OnDisplay() override;
    virtual void OnIdle() override;

private:
    void ModifyRectangles();
    void DrawRectangles();

    std::vector<AlignedBox2<float>> mRectangles;
    std::unique_ptr<RectangleManager<float>> mManager;
    float mSize;
    Timer mTimer;
    double mLastIdle;
    std::default_random_engine mDRE;
    std::uniform_real_distribution<float> mPerturb;
};
