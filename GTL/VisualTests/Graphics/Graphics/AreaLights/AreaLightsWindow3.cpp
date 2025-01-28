// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2022 David Eberly
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include "AreaLightsWindow3.h"
#include <GTL/Graphics/SceneGraph/MeshFactory.h>
#include <GTL/Graphics/Effects/Material.h>
#include <GTL/Applications/WICFileIO.h>

AreaLightsWindow3::AreaLightsWindow3(Parameters& parameters)
    :
    Window3(parameters)
{
    if (!SetEnvironment())
    {
        parameters.created = false;
        return;
    }

    CreateScene();
    InitializeCamera(60.0f, GetAspectRatio(), 0.1f, 100.0f, 0.01f, 0.001f,
        { 12.0f, 0.0f, 4.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });

    mPVWMatrices.Update();
}

void AreaLightsWindow3::OnIdle()
{
    mTimer.Measure();

    if (mCameraRig.Move())
    {
        mPVWMatrices.Update();
    }
    UpdateConstants();

    mEngine->ClearBuffers();
    mEngine->Draw(mSurface);
    mEngine->Draw(8, mYSize - 8, { 0.0f, 0.0f, 0.0f, 1.0f }, mTimer.GetFPS());
    mEngine->DisplayColorBuffer(0);

    mTimer.UpdateFrameCount();
}

bool AreaLightsWindow3::SetEnvironment()
{
    std::string path = GetGTLPath();
    if (path == "")
    {
        return false;
    }

    mEnvironment.Insert(path + "GTL/VisualTests/Data/");
    std::vector<std::string> inputs =
    {
        "Bricks.png",
        "BricksNormal.png"
    };

    for (auto const& input : inputs)
    {
        if (mEnvironment.GetPath(input) == "")
        {
            GTL_RUNTIME_ERROR("Cannot find file " + input);
            return false;
        }
    }

    return true;
}

void AreaLightsWindow3::CreateScene()
{
    CreateSurface();
    CreateAreaLightEffect();

    mPVWMatrices.Subscribe(mSurface->worldTransform, mALEffect->GetPVWMatrixConstant());
    mTrackBall.Attach(mSurface);
    mTrackBall.Update();
}

void AreaLightsWindow3::CreateSurface()
{
    VertexFormat vformat;
    vformat.Bind(VASemantic::POSITION, DF_R32G32B32_FLOAT, 0);
    vformat.Bind(VASemantic::TEXCOORD, DF_R32G32_FLOAT, 0);

    MeshFactory mf;
    mf.SetVertexFormat(vformat);
    mSurface = mf.CreateRectangle(2, 2, 16.0f, 16.0f);
}

void AreaLightsWindow3::CreateAreaLightEffect()
{
    std::string path = mEnvironment.GetPath("Bricks.png");
    mSurfaceTexture = WICFileIO::Load(path, true);
    mSurfaceTexture->AutogenerateMipmaps();

    path = mEnvironment.GetPath("BricksNormal.png");
    mNormalTexture = WICFileIO::Load(path, true);
    mNormalTexture->AutogenerateMipmaps();

    mALEffect = std::make_shared<AreaLightEffect>(mProgramFactory,
        mSurfaceTexture, mNormalTexture, SamplerState::Filter::MIN_L_MAG_L_MIP_L,
        SamplerState::Mode::CLAMP, SamplerState::Mode::CLAMP);

    mSurface->SetEffect(mALEffect);

    auto& surfaceMaterial = *mALEffect->GetMaterialConstant()->Get<Material>();
    auto& areaLight = *mALEffect->GetAreaLightConstant()->Get<AreaLightEffect::Parameters>();

    // Gray material with tight specular.
    surfaceMaterial.emissive = { 0.0f, 0.0f, 0.0f, 1.0f };
    surfaceMaterial.ambient = { 0.25f, 0.25f, 0.25f, 1.0f };
    surfaceMaterial.diffuse = { 0.25f, 0.25f, 0.25f, 1.0f };
    surfaceMaterial.specular = { 0.5f, 0.5f, 0.5f, 128.0f };
    mEngine->Update(mALEffect->GetMaterialConstant());

    // White area light.
    areaLight.ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
    areaLight.diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
    areaLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };
    areaLight.attenuation = { 1.0f, 0.0f, 0.0f, 1.0f };

    // World-space geometric information for the rectangle of the light.
    mALWorldPosition = { 0.0f, 0.0f, 32.0f, 1.0f };
    mALWorldNormal = { 0.0f, 0.0f, -1.0f, 0.0f };
    mALWorldAxis0 = { 1.0f, 0.0f, 0.0f, 0.0f };
    mALWorldAxis1 = Cross(mALWorldNormal, mALWorldAxis0);
    mALExtent = { 1.0f, 8.0f, 0.0f, 0.0f };
    areaLight.extent = mALExtent;

    UpdateConstants();
}

void AreaLightsWindow3::UpdateConstants()
{
    auto& areaLight = *mALEffect->GetAreaLightConstant()->Get<AreaLightEffect::Parameters>();
    auto& cameraModelPosition = *mALEffect->GetCameraConstant()->Get<Vector4<float>>();

    Matrix4x4<float> hinverse = mSurface->worldTransform.GetInverseH();
    areaLight.position = hinverse * mALWorldPosition;
    areaLight.normal = hinverse * mALWorldNormal;
    areaLight.axis0 = hinverse * mALWorldAxis0;
    areaLight.axis1 = hinverse * mALWorldAxis1;
    cameraModelPosition = hinverse * mCamera->GetPosition();

    mEngine->Update(mALEffect->GetAreaLightConstant());
    mEngine->Update(mALEffect->GetCameraConstant());
}
