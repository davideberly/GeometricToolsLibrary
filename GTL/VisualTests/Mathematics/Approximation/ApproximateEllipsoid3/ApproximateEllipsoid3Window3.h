// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.09.22

#pragma once

#include <GTL/Applications/Window3.h>
#include <GTL/Mathematics/Approximation/3D/ApprEllipsoid3.h>
using namespace gtl;

class ApproximateEllipsoid3Window3 : public Window3
{
public:
    ApproximateEllipsoid3Window3(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y) override;

private:
    void CreateScene();
    std::shared_ptr<Visual> CreateEllipsoidMesh(Ellipsoid3<float> const& ellipsoid,
        Vector4<float> const& color);

    Ellipsoid3<float> mTrueEllipsoid, mApprEllipsoid;

    std::shared_ptr<RasterizerState> mWireState;
    std::shared_ptr<Visual> mTrueMesh, mApprMesh;
};
