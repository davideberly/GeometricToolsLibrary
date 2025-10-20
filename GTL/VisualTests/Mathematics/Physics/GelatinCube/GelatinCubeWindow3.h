// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.20

#pragma once

#include <GTL/Applications/Window3.h>
#include <GTL/Utility/Timer.h>
#include <GTL/Mathematics/Volumes/BSplineVolume.h>
#include "PhysicsModule.h"
#include <cstdint>
#include <memory>
using namespace gtl;

//#define GELATIN_CUBE_SINGLE_STEP

class GelatinCubeWindow3 : public Window3
{
public:
    GelatinCubeWindow3(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(std::uint8_t key, std::int32_t x, std::int32_t y) override;

private:
    bool SetEnvironment();
    void CreateScene();
    void CreateCube();
    void CreateSprings();
    void PhysicsTick();
    void GraphicsTick();

    // The vertex layout for the cube.
    struct Vertex
    {
        Vector3<float> position;
        Vector2<float> tcoord;
    };

    // Support for creating the cube faces.
    void CreateFaceVertices(std::uint32_t numRows, std::uint32_t numCols,
        float faceValue, std::array<std::uint32_t, 3> const& permute, Vertex* vertices,
        std::uint32_t& index);

    void CreateFaceIndices(std::uint32_t numRows, std::uint32_t numCols,
        bool ccw, std::uint32_t& vBase, std::uint32_t*& indices);

    void UpdateFaces();

    // The scene graph.
    std::shared_ptr<BlendState> mBlendState;
    std::shared_ptr<DepthStencilState> mDepthReadNoWriteState;
    std::shared_ptr<RasterizerState> mNoCullSolidState;
    std::shared_ptr<RasterizerState> mNoCullWireState;
    std::shared_ptr<Node> mScene;
    std::shared_ptr<Visual> mCube;

    // The physics system.
    std::unique_ptr<PhysicsModule> mModule;
    Timer mMotionTimer;

    // The masses are located at the control points of a spline surface.
    // The control points are connected in a mass-spring system.
    static std::uint32_t constexpr numSamplesPerDimension = 8;
    std::shared_ptr<BSplineVolume<float, 3>> mVolume;
    std::uint32_t mNumUSamples, mNumVSamples, mNumWSamples;
};
