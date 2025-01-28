// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.28

#pragma once

#include <GTL/Applications/Window3.h>
#include <GTL/Mathematics/Surfaces/BSplineSurfaceFit.h>
using namespace gtl;

class BSplineSurfaceFitterWindow3 : public Window3
{
public:
    BSplineSurfaceFitterWindow3(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y) override;

private:
    bool SetEnvironment();
    void CreateScene();

    struct VertexPT
    {
        Vector3<float> position;
        Vector2<float> tcoord;
    };

    struct VertexPC
    {
        Vector3<float> position;
        Vector4<float> color;
    };

    std::shared_ptr<RasterizerState> mNoCullState;
    std::shared_ptr<RasterizerState> mNoCullWireState;
    std::shared_ptr<BlendState> mBlendState;
    std::shared_ptr<Visual> mHeightField, mFittedField;
    BSplineSurface<float, 3> mSpline;
};
