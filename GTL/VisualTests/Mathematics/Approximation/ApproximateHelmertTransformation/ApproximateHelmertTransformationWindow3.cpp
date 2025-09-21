// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.09.21

#include "ApproximateHelmertTransformationWindow3.h"
#include <GTL/Graphics/Effects/ConstantColorEffect.h>
#include <GTL/Graphics/SceneGraph/MeshFactory.h>
#include <GTL/Mathematics/Algebra/Rotation.h>
#include <cstddef>
#include <iomanip>
#include <memory>
#include <random>
#include <sstream>

ApproximateHelmertTransformationWindow3::ApproximateHelmertTransformationWindow3(Parameters& parameters)
    :
    Window3(parameters),
    mPPoints(numPoints),
    mQPoints(numPoints),
    mHelmert{},
    mMessage{},
    mPPointSpheres(numPoints),
    mQPointSpheres(numPoints)
{
    InitializeCamera(60.0f, GetAspectRatio(), 0.01f, 100.0f, 0.001f, 0.001f,
        { -4.5f, 0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f});

    CreateScene();
}

void ApproximateHelmertTransformationWindow3::OnIdle()
{
    mTimer.Measure();

    if (mCameraRig.Move())
    {
        mPVWMatrices.Update();
    }

    mEngine->ClearBuffers();
    for (std::size_t i = 0; i < numPoints; ++i)
    {
        mEngine->Draw(mPPointSpheres[i]);
        mEngine->Draw(mQPointSpheres[i]);
    }
    mEngine->Draw(8, 24, { 0.0f, 0.0f, 0.0f, 1.0f }, mMessage);
    mEngine->Draw(8, mYSize - 8, { 0.0f, 0.0f, 0.0f, 1.0f }, mTimer.GetFPS());
    mEngine->DisplayColorBuffer(0);

    mTimer.UpdateFrameCount();
}

bool ApproximateHelmertTransformationWindow3::OnCharPress(uint8_t key, int32_t x, int32_t y)
{
    if (key == ' ')
    {
        Align();
        return true;
    }

    return Window3::OnCharPress(key, x, y);
}

void ApproximateHelmertTransformationWindow3::CreateScene()
{
    std::default_random_engine dre{};
    std::uniform_real_distribution<double> urd(-1.0, 1.0);
    Quaternion<double> qq(urd(dre), urd(dre), urd(dre), urd(dre));
    Normalize(qq);
    Matrix3x3<double> rotate = Rotation<double>(qq);
    Vector3<double> translate{ urd(dre), urd(dre), urd(dre) };
    double scale = 1.01;

    double epsilon = 1e-06;
    for (std::size_t i = 0; i < numPoints; ++i)
    {
        mPPoints[i][0] = urd(dre);
        mPPoints[i][1] = urd(dre);
        mPPoints[i][2] = urd(dre);
        mQPoints[i][0] = mPPoints[i][0] + epsilon * urd(dre);
        mQPoints[i][1] = mPPoints[i][1] + epsilon * urd(dre);
        mQPoints[i][2] = mPPoints[i][2] + epsilon * urd(dre);
        mQPoints[i] = scale * (rotate * mQPoints[i]) + translate;
    }

    VertexFormat vformat{};
    vformat.Bind(VASemantic::POSITION, DF_R32G32B32_FLOAT, 0);
    MeshFactory mf(vformat);

    for (std::size_t i = 0; i < numPoints; ++i)
    {
        mPPointSpheres[i] = mf.CreateSphere(8, 8, 0.01f);
        mPPointSpheres[i]->localTransform.SetTranslation(
            static_cast<float>(mPPoints[i][0]),
            static_cast<float>(mPPoints[i][1]),
            static_cast<float>(mPPoints[i][2]));
        auto effect = std::make_shared<ConstantColorEffect>(mProgramFactory,
            Vector4<float>{ 1.0f, 0.0f, 0.0f, 1.0f});
        mPPointSpheres[i]->SetEffect(effect);
        mPVWMatrices.Subscribe(mPPointSpheres[i]);
        mTrackBall.Attach(mPPointSpheres[i]);

        mQPointSpheres[i] = mf.CreateSphere(8, 8, 0.01f);
        mQPointSpheres[i]->localTransform.SetTranslation(
            static_cast<float>(mQPoints[i][0]),
            static_cast<float>(mQPoints[i][1]),
            static_cast<float>(mQPoints[i][2]));
        effect = std::make_shared<ConstantColorEffect>(mProgramFactory,
            Vector4<float>{ 0.0f, 0.0f, 1.0f, 1.0f});
        mQPointSpheres[i]->SetEffect(effect);
        mPVWMatrices.Subscribe(mQPointSpheres[i]);
        mTrackBall.Attach(mQPointSpheres[i]);
    }

    mTrackBall.Update();
    mPVWMatrices.Update();
}

void ApproximateHelmertTransformationWindow3::Align()
{
    Matrix3x3<double> outRotate{};
    Vector3<double> outTranslate{};
    double outScale{};
    double outFunction{};
    std::size_t iteration = mHelmert.Fit(mPPoints, mQPoints, numIterations,
        outRotate, outTranslate, outScale, outFunction);

    for (std::size_t i = 0; i < numPoints; ++i)
    {
        mQPoints[i] = outScale * (outRotate * mQPoints[i]) + outTranslate;
        mQPointSpheres[i]->localTransform.SetTranslation(
            static_cast<float>(mQPoints[i][0]),
            static_cast<float>(mQPoints[i][1]),
            static_cast<float>(mQPoints[i][2]));
    }
    mTrackBall.Update();
    mPVWMatrices.Update();

    std::ostringstream stream{};
    stream << std::setprecision(17) << "maxIterations = " << numIterations
        << ", iterations = " << iteration
        << ", RMS error = " << outFunction;
    mMessage = stream.str();
}
