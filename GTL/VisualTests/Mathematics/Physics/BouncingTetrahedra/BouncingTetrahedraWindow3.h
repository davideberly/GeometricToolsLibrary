// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.12

#pragma once

#include <GTL/Applications/Window3.h>
#include <GTL/Utility/Timer.h>
#include <GTL/Mathematics/Physics/RigidBody.h>
#include "PhysicsModule.h"
#include <array>
#include <cstdint>
#include <memory>
using namespace gtl;

class BouncingTetrahedraWindow3 : public Window3
{
public:
    BouncingTetrahedraWindow3(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(std::uint8_t key, std::int32_t x, std::int32_t y) override;

private:
    struct VertexPC
    {
        VertexPC()
            :
            position(Vector3<float>::Zero()),
            color(Vector4<float>::Zero())
        {
        }

        Vector3<float> position;
        Vector4<float> color;
    };

    bool SetEnvironment();
    void CreateScene();
    void CreatePhysicsObjects();
    void CreateGraphicsObjects();
    void CreateWall(std::size_t index, VertexFormat const& vformat,
        Vector3<float> const& pos0, Vector3<float> const& pos1,
        Vector3<float> const& pos2, Vector3<float> const& pos3,
        Vector4<float> const& color);

    void PhysicsTick();
    void GraphicsTick();

    enum { NUM_TETRAHEDRA = 16 };
    std::unique_ptr<PhysicsModule> mModule;

    std::shared_ptr<RasterizerState> mNoCullState;
    std::shared_ptr<RasterizerState> mNoCullWireState;
    std::shared_ptr<Node> mScene;
    std::array<std::shared_ptr<Visual>, 4> mPlaneMesh;
    std::array<std::shared_ptr<Visual>, NUM_TETRAHEDRA> mTetraMesh;

    Timer mPhysicsTimer, mGraphicsTimer;
    double mLastPhysicsTime, mCurrPhysicsTime;
    double mSimulationTime, mSimulationDeltaTime;
    double mLastGraphicsTime, mCurrGraphicsTime;
    bool mSingleStep;
};
