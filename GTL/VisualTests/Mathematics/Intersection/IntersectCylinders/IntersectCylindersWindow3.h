// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

#include <GTL/Applications/Window3.h>
#include <GTL/Graphics/Effects/ConstantColorEffect.h>
#include <GTL/Mathematics/Intersection/3D/IntrCylinder3Cylinder3.h>
using namespace gtl;

class IntersectCylindersWindow3 : public Window3
{
public:
    IntersectCylindersWindow3(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y) override;

private:
    void CreateScene();
    void Translate(int32_t direction, float delta);
    void Rotate(int32_t direction, float delta);
    void UpdateCylinderMesh();
    void DoIntersectionQuery();

    std::shared_ptr<RasterizerState> mNoCullState;
    std::shared_ptr<RasterizerState> mNoCullWireState;
    std::shared_ptr<Visual> mTriangleMesh;
    std::array<std::shared_ptr<Visual>, 2> mCylinderMesh;
    std::shared_ptr<ConstantColorEffect> mRedEffect;
    std::shared_ptr<ConstantColorEffect> mMagentaEffect;
    std::shared_ptr<ConstantColorEffect> mBlueEffect;
    std::shared_ptr<ConstantColorEffect> mCyanEffect;

    size_t mNumLines;
    std::array<Cylinder3<float>, 2> mCylinder;
    TIQuery<float, Cylinder3<float>, Cylinder3<float>> mQuery;

    size_t mMotionObject;
    std::array<Matrix3x3<float>, 2> mCylinderBasis;
};
