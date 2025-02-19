// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.02.16

#pragma once

#include <GTL/Applications/Window3.h>
#include <GTL/Mathematics/Distance/3D/DistPoint3Parallelepiped3.h>
#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Graphics/SceneGraph/MeshFactory.h>
#include <GTL/Graphics/Effects/ConstantColorEffect.h>
using namespace gtl;

class DistancePoint3Parallelepiped3Window3 : public Window3
{
public:
    DistancePoint3Parallelepiped3Window3(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y) override;

private:
    void CreateScene();

    void CreateMeshFace(std::size_t i, MeshFactory& mf,
        std::array<Vector3<float>, 8> const& vertices,
        std::array<std::size_t, 4> const& cornerIndices,
        Vector4<float> const& color);

    void DoQuery();

    using PPQuery = DCPQuery<float, Vector3<float>, Parallelepiped3<float>>;
    using PPResult = PPQuery::Output;
    PPQuery mQuery;
    PPResult mResult;
    Vector3<float> mPoint, mClosest;
    Parallelepiped3<float> mParallelepiped;

    std::shared_ptr<RasterizerState> mNoCullState;
    std::shared_ptr<RasterizerState> mNoCullWireState;
    std::shared_ptr<Visual> mMeshPoint, mMeshClosest, mMeshSegment;

    // Faces of the parallelepiped. Indices
    // 0: -x face
    // 1: +x face
    // 2: -y face
    // 3: +y face
    // 4: -z face
    // 5: +z face
    std::array<std::shared_ptr<Visual>, 6> mMeshFace;

    float mRadius, mTwoPi, mDelta, mTheta, mPhi;
};
