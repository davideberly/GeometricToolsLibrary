// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.12

#pragma once

#include <GTL/Applications/Window3.h>
#include <GTL/Utility/Timer.h>
#include <GTL/Graphics/Effects/DirectionalLightEffect.h>
#include <GTL/Mathematics/Physics/BoxManager.h>
#include <array>
#include <cstddef>
#include <memory>
#include <random>
#include <vector>
using namespace gtl;

class IntersectingBoxesWindow3 : public Window3
{
public:
    IntersectingBoxesWindow3(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y) override;

private:
    void CreateScene();
    void ModifyBoxes();
    void ModifyMesh(std::size_t i);
    void PhysicsTick();
    void GraphicsTick();

    std::vector<AlignedBox3<float>> mBoxes;
    std::unique_ptr<BoxManager<float>> mManager;
    bool mDoSimulation;
    Timer mSimulationTimer;
    double mLastIdle;
    float mSize;

    enum { NUM_BOXES = 16 };

    struct Vertex
    {
        Vector3<float> position, normal;
    };

    std::shared_ptr<Node> mScene;
    std::shared_ptr<RasterizerState> mWireState;
    std::default_random_engine mDRE;
    std::uniform_real_distribution<float> mPerturb;
    std::array<std::shared_ptr<Visual>, NUM_BOXES> mBoxMesh;
    std::array<std::shared_ptr<DirectionalLightEffect>, NUM_BOXES> mNoIntersectEffect;
    std::array<std::shared_ptr<DirectionalLightEffect>, NUM_BOXES> mIntersectEffect;
};
