// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.28

#pragma once

#include <GTL/Applications/Window3.h>
#include <GTL/Mathematics/Distance/3D/DistOrientedBox3Cone3.h>
#include "DistanceBoxQuad.h"
#include <random>
using namespace gtl;

class DistanceOrientedBoxConeFrustumWindow3 : public Window3
{
public:
    DistanceOrientedBoxConeFrustumWindow3(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y) override;

private:
    void CreateScene();
    void CreateBoxMesh();
    void CreateConeMesh();
    void CreateQuadMesh();
    void CreateSegmentMeshes();
    void CreateClosestPointMeshes();
    void Translate(std::size_t direction, float delta);
    void Rotate(std::size_t direction, float delta);
    void Update();

    struct Vertex
    {
        Vector3<float> position;
        Vector4<float> color;
    };

    std::shared_ptr<RasterizerState> mNoCullState;
    std::shared_ptr<RasterizerState> mNoCullWireState;
    std::shared_ptr<Visual> mBoxMesh;
    std::shared_ptr<Visual> mConeMesh;
    std::shared_ptr<Visual> mQuadMesh;
    std::shared_ptr<Visual> mBoxQuadSegmentMesh;
    std::shared_ptr<Visual> mBoxConeSegmentMesh;
    std::shared_ptr<Visual> mBoxClosestToQuadMesh;
    std::shared_ptr<Visual> mBoxClosestToConeMesh;
    std::shared_ptr<Visual> mQuadClosestMesh;
    std::shared_ptr<Visual> mConeClosestMesh;
    VertexFormat mVFormat;

    std::default_random_engine mDRE;
    std::uniform_real_distribution<float> mURD;

    OrientedBox3<float> mBox;
    Cone3<float> mCone;
    std::array<Vector3<float>, 4> mQuadrilateral;
    float mQuadAngle;
    float mBoxQuadDistance;
    float mBoxConeDistance;
    Vector3<float> mBoxClosestToQuad;
    Vector3<float> mBoxClosestToCone;
    Vector3<float> mQuadClosest;
    Vector3<float> mConeClosest;
    DistanceBoxQuad<float> mBoxQuadQuery;
    DCPQuery<float, OrientedBox3<float>, Cone3<float>> mBoxConeQuery;
};
