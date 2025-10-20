// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.17

#pragma once

#include <GTL/Applications/Window3.h>
#include <GTL/Mathematics/Surfaces/BSplineSurface.h>
#include <GTL/Mathematics/Meshes/RectanglePatchMesh.h>
#include <GTL/Utility/Timer.h>
#include "PhysicsModule.h"
#include <cstdint>
#include <memory>
using namespace gtl;

class ClothWindow3 : public Window3
{
public:
    ClothWindow3(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(std::uint8_t key, std::int32_t x, std::int32_t y) override;

private:
    bool SetEnvironment();
    void CreateSprings();
    void CreateCloth();
    void PhysicsTick();
    void GraphicsTick();

    struct Vertex
    {
        Vector3<float> position;
        Vector2<float> tcoord;
    };

    std::shared_ptr<RasterizerState> mNoCullState;
    std::shared_ptr<RasterizerState> mWireNoCullState;
    std::shared_ptr<Visual> mCloth;

    // The masses are located at the control points of a spline curve. The
    // control points are connected by a mass-spring system.
    std::unique_ptr<PhysicsModule> mModule;
    std::shared_ptr<BSplineSurface<float, 3>> mSpline;
    std::unique_ptr<RectanglePatchMesh<float>> mSurface;

    Timer mAnimTimer;
    double mAnimStartTime;
};
