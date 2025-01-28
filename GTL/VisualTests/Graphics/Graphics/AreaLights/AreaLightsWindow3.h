// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2022 David Eberly
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Applications/Window3.h>
#include <GTL/Graphics/Effects/AreaLightEffect.h>
using namespace gtl;

class AreaLightsWindow3 : public Window3
{
public:
    AreaLightsWindow3(Parameters& parameters);

    virtual void OnIdle();

private:
    bool SetEnvironment();
    void CreateScene();
    void CreateSurface();
    void CreateAreaLightEffect();
    void UpdateConstants();

    std::shared_ptr<Visual> mSurface;
    std::shared_ptr<Texture2> mSurfaceTexture, mNormalTexture;
    Vector4<float> mALWorldPosition, mALWorldNormal, mALWorldAxis0, mALWorldAxis1;
    Vector4<float> mALExtent;
    std::shared_ptr<AreaLightEffect> mALEffect;
};
