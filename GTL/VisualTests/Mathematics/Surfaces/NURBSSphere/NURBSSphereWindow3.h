// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.28

#pragma once

#include <GTL/Applications/Window3.h>
#include <GTL/Mathematics/Surfaces/NURBSSphere.h>
using namespace gtl;

class NURBSSphereWindow3 : public Window3
{
public:
    NURBSSphereWindow3(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y) override;

private:
    void CreateScene();
    void CreateEighthSphere();
    void CreateHalfSphere();
    void CreateFullSphere();

    std::shared_ptr<RasterizerState> mNoCullSolidState;
    std::shared_ptr<RasterizerState> mNoCullWireState;
    NURBSEighthSphereDegree4<float> mEighthSphere;
    std::shared_ptr<Visual> mEighthSphereVisual;
    NURBSHalfSphereDegree3<float> mHalfSphere;
    std::shared_ptr<Visual> mHalfSphereVisual;
    NURBSFullSphereDegree3<float> mFullSphere;
    std::shared_ptr<Visual> mFullSphereVisual;

    std::shared_ptr<Visual> mCurrentVisual;
};
