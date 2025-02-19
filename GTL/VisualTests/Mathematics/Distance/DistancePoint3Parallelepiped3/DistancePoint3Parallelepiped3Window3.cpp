// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.02.16

#include "DistancePoint3Parallelepiped3Window3.h"

DistancePoint3Parallelepiped3Window3::DistancePoint3Parallelepiped3Window3(Parameters& parameters)
    :
    Window3(parameters),
    mQuery{},
    mResult{},
    mPoint(Vector3<float>::Zero()),
    mClosest(Vector3<float>::Zero()),
    mParallelepiped{},
    mNoCullState{},
    mNoCullWireState{},
    mMeshPoint{},
    mMeshClosest{},
    mMeshSegment{},
    mMeshFace{},
    mRadius(3.0f),
    mTwoPi(C_TWO_PI<float>),
    mDelta(C_DEG_TO_RAD<float>),
    mTheta(0.0f),
    mPhi(0.0f)
{
    mNoCullState = std::make_shared<RasterizerState>();
    mNoCullState->cull = RasterizerState::NONE;
    mNoCullWireState = std::make_shared<RasterizerState>();
    mNoCullWireState->cull = RasterizerState::NONE;
    mNoCullWireState->fill = RasterizerState::WIREFRAME;
    mEngine->SetRasterizerState(mNoCullState);

    InitializeCamera(60.0f, GetAspectRatio(), 0.01f, 1000.0f, 0.001f, 0.001f,
        { 6.5f, 0.0f, 0.0f, }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });

    CreateScene();
    DoQuery();
}

void DistancePoint3Parallelepiped3Window3::OnIdle()
{
    mTimer.Measure();

    if (mCameraRig.Move())
    {
        mPVWMatrices.Update();
    }

    mEngine->ClearBuffers();
    for (std::size_t i = 0; i < 6; ++i)
    {
        mEngine->Draw(mMeshFace[i]);
    }
    mEngine->Draw(mMeshSegment);
    mEngine->Draw(mMeshPoint);
    mEngine->Draw(mMeshClosest);
    mEngine->Draw(8, mYSize - 8, { 0.0f, 0.0f, 0.0f, 1.0f }, mTimer.GetFPS());
    mEngine->DisplayColorBuffer(0);

    mTimer.UpdateFrameCount();
}

bool DistancePoint3Parallelepiped3Window3::OnCharPress(uint8_t key, int32_t x, int32_t y)
{
    float csTheta{}, snTheta{}, csPhi{}, snPhi{};

    switch (key)
    {
    case 'w':
    case 'W':
        if (mEngine->GetRasterizerState() == mNoCullState)
        {
            mEngine->SetRasterizerState(mNoCullWireState);
        }
        else
        {
            mEngine->SetRasterizerState(mNoCullState);
        }
        return true;

    case 'a':
        mTheta -= mDelta;
        if (mTheta < 0.0f)
        {
            mTheta += mTwoPi;
        }
        csTheta = std::cos(mTheta);
        snTheta = std::sin(mTheta);
        csPhi = std::cos(mPhi);
        snPhi = std::sin(mPhi);
        mPoint = mRadius * Vector3<float>{ csPhi* csTheta, csPhi* snTheta, snPhi };
        DoQuery();
        return true;

    case 'A':
        mTheta += mDelta;
        if (mTheta > mTwoPi)
        {
            mTheta -= mTwoPi;
        }
        csTheta = std::cos(mTheta);
        snTheta = std::sin(mTheta);
        csPhi = std::cos(mPhi);
        snPhi = std::sin(mPhi);
        mPoint = mRadius * Vector3<float>{ csPhi* csTheta, csPhi* snTheta, snPhi };
        DoQuery();
        return true;

    case 'b':
        mPhi -= mDelta;
        if (mPhi < -mTwoPi)
        {
            mPhi += mTwoPi;
        }
        csTheta = std::cos(mTheta);
        snTheta = std::sin(mTheta);
        csPhi = std::cos(mPhi);
        snPhi = std::sin(mPhi);
        mPoint = mRadius * Vector3<float>{ csPhi* csTheta, csPhi* snTheta, snPhi };
        DoQuery();
        return true;

    case 'B':
        mPhi += mDelta;
        if (mPhi > mTwoPi)
        {
            mPhi -= mTwoPi;
        }
        csTheta = std::cos(mTheta);
        snTheta = std::sin(mTheta);
        csPhi = std::cos(mPhi);
        snPhi = std::sin(mPhi);
        mPoint = mRadius * Vector3<float>{ csPhi* csTheta, csPhi* snTheta, snPhi };
        DoQuery();
        return true;

    case 'd':
    case 'D':
        DoQuery();
        return true;
    }

    return Window3::OnCharPress(key, x, y);
}

void DistancePoint3Parallelepiped3Window3::CreateScene()
{
    Vector3<float> center = Vector3<float>::Zero();
    std::array<Vector3<float>, 3> axis{};
    axis[0] = { 1.0f, 0.125f, 0.0f };
    axis[1] = { -1.0f, 0.5f, -0.375f };
    axis[2] = { -0.5f, -0.0125f, 1.5f };
    float dot = DotCross(axis[0], axis[1], axis[2]);
    if (dot < 0.0)
    {
        std::swap(axis[1], axis[2]);
    }
    mParallelepiped = Parallelepiped3<float>(center, axis);
    mPoint = { mRadius, 0.0f, 0.0f };

    VertexFormat vformat{};
    vformat.Bind(VASemantic::POSITION, DF_R32G32B32_FLOAT, 0);
    MeshFactory mf(vformat);

    mMeshPoint = mf.CreateSphere(16, 16, 0.0625);
    mMeshPoint->localTransform.SetTranslation(mPoint);
    auto effect = std::make_shared<ConstantColorEffect>(mProgramFactory,
        Vector4<float>{ 0.0f, 0.0f, 0.0f, 1.0f });
    mMeshPoint->SetEffect(effect);
    mPVWMatrices.Subscribe(mMeshPoint);
    mTrackBall.Attach(mMeshPoint);

    mMeshClosest = mf.CreateSphere(16, 16, 0.0625);
    mMeshClosest->localTransform.SetTranslation(mClosest);
    effect = std::make_shared<ConstantColorEffect>(mProgramFactory,
        Vector4<float>{ 0.0f, 0.0f, 0.0f, 1.0f });
    mMeshClosest->SetEffect(effect);
    mPVWMatrices.Subscribe(mMeshClosest);
    mTrackBall.Attach(mMeshClosest);

    auto vSegBuffer = std::make_shared<VertexBuffer>(vformat, 2);
    vSegBuffer->SetUsage(Resource::DYNAMIC_UPDATE);
    auto* vSegVertices = vSegBuffer->Get<Vector3<float>>();
    vSegVertices[0] = mPoint;
    vSegVertices[1] = mPoint;
    auto iSegBuffer = std::make_shared<IndexBuffer>(IP_POLYSEGMENT_DISJOINT, 1);
    effect = std::make_shared<ConstantColorEffect>(mProgramFactory,
        Vector4<float>{ 0.0f, 0.0f, 0.0f, 1.0f });
    mMeshSegment = std::make_shared<Visual>(vSegBuffer, iSegBuffer, effect);
    mMeshSegment->SetEffect(effect);
    mPVWMatrices.Subscribe(mMeshSegment);
    mTrackBall.Attach(mMeshSegment);

    std::array<Vector3<float>, 8> vertices{};
    mParallelepiped.GetVertices(vertices);
    //vertices[0] = center - axis[0] - axis[1] - axis[2];
    //vertices[1] = center + axis[0] - axis[1] - axis[2];
    //vertices[2] = center - axis[0] + axis[1] - axis[2];
    //vertices[3] = center + axis[0] + axis[1] - axis[2];
    //vertices[4] = center - axis[0] - axis[1] + axis[2];
    //vertices[5] = center + axis[0] - axis[1] + axis[2];
    //vertices[6] = center - axis[0] + axis[1] + axis[2];
    //vertices[7] = center + axis[0] + axis[1] + axis[2];

    // face x = -1
    CreateMeshFace(0, mf, vertices, { 0, 2, 4, 6 }, Vector4<float>{ 1.0f, 0.0f, 0.0f, 1.0f });

    // face x = +1
    CreateMeshFace(1, mf, vertices, { 1, 3, 5, 7 }, Vector4<float>{ 0.0f, 0.75f, 0.0f, 1.0f });

    // face y = -1
    CreateMeshFace(2, mf, vertices, { 0, 1, 4, 5 }, Vector4<float>{ 0.0f, 0.0f, 1.0f, 1.0f });

    // face y = +1
    CreateMeshFace(3, mf, vertices, { 2, 3, 6, 7 }, Vector4<float>{ 0.0f, 1.0f, 1.0f, 1.0f });

    // face z = -1
    CreateMeshFace(4, mf, vertices, { 0, 1, 2, 3 }, Vector4<float>{ 1.0f, 0.0f, 1.0f, 1.0f });

    // face z = +1
    CreateMeshFace(5, mf, vertices, { 4, 5, 6, 7 }, Vector4<float>{ 1.0f, 0.5f, 0.0f, 1.0f });

    mTrackBall.Update();
    mPVWMatrices.Update();
}

void DistancePoint3Parallelepiped3Window3::CreateMeshFace(std::size_t i, MeshFactory& mf,
    std::array<Vector3<float>, 8> const& vertices,
    std::array<std::size_t, 4> const& cornerIndices,
    Vector4<float> const& color)
{
    mMeshFace[i] = mf.CreateRectangle(2, 2, 1.0f, 1.0f);
    auto* corners = mMeshFace[i]->GetVertexBuffer()->Get<Vector3<float>>();
    corners[0] = vertices[cornerIndices[0]];
    corners[1] = vertices[cornerIndices[1]];
    corners[2] = vertices[cornerIndices[2]];
    corners[3] = vertices[cornerIndices[3]];
    auto effect = std::make_shared<ConstantColorEffect>(mProgramFactory, color);
    mMeshFace[i]->SetEffect(effect);
    mPVWMatrices.Subscribe(mMeshFace[i]);
    mTrackBall.Attach(mMeshFace[i]);

    auto const& vbuffer = mMeshFace[i]->GetVertexBuffer();
    auto* vtemp = vbuffer->Get<Vector3<float>>();
    auto const& ibuffer = mMeshFace[i]->GetIndexBuffer();
    auto* itemp = ibuffer->Get<std::uint32_t>();
    (void)vtemp;
    (void)itemp;
}

void DistancePoint3Parallelepiped3Window3::DoQuery()
{
    mResult = mQuery(mPoint, mParallelepiped);
    mClosest = mResult.closest[1];

    mMeshPoint->localTransform.SetTranslation(mPoint);
    mMeshClosest->localTransform.SetTranslation(mClosest);

    auto const& vbuffer = mMeshSegment->GetVertexBuffer();
    auto* vertices = vbuffer->Get<Vector3<float>>();
    vertices[0] = mPoint;
    vertices[1] = mClosest;
    mEngine->Update(vbuffer);

    mTrackBall.Update();
    mPVWMatrices.Update();
}
