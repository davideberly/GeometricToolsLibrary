// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.21

#pragma once

#include <GTL/Applications/Window3.h>
#include <GTL/Utility/Timer.h>
#include "PhysicsModule.h"
#include <cstdint>
#include <memory>
#include <vector>
using namespace gtl;

//#define GELATIN_BLOB_SINGLE_STEP

class GelatinBlobWindow3 : public Window3
{
public:
    GelatinBlobWindow3(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(std::uint8_t key, std::int32_t x, std::int32_t y) override;

private:
    bool SetEnvironment();
    void CreateScene();
    void CreateIcosahedron();
    void CreateSprings();
    void CreateSegments();
    void PhysicsTick();
    void GraphicsTick();

    // The vertex layout for the icosahedron.
    struct Vertex
    {
        Vector3<float> position;
        Vector2<float> tcoord;
    };

    // The scene graph.
    std::shared_ptr<BlendState> mBlendState;
    std::shared_ptr<DepthStencilState> mDepthReadNoWriteState;
    std::shared_ptr<RasterizerState> mNoCullSolidState;
    std::shared_ptr<RasterizerState> mNoCullWireState;
    std::shared_ptr<Node> mScene, mSegmentRoot;
    std::shared_ptr<Visual> mIcosahedron;
    std::vector<std::shared_ptr<Visual>> mSegments;

    // The icosahedron has 12 vertices and 30 edges. Each vertex is a
    // particle in the system. Each edge represents a spring. To keep the
    // icosahedron from collapsing, 12 immovable particles are added, each
    // outside the icosahedron in the normal direction above a vertex. The
    // immovable particles are connected to their corresponding vertices with
    // springs.
    static std::size_t constexpr numParticles = 24;
    static std::size_t constexpr numSprings = 42;

    // The physics system.
    std::unique_ptr<PhysicsModule> mModule;
    Timer mMotionTimer;
};
