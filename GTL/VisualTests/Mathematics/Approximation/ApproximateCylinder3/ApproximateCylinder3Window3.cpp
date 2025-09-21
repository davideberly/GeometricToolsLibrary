// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.09.21

#include "ApproximateCylinder3Window3.h"
#include <GTL/Graphics/SceneGraph/MeshFactory.h>
#include <GTL/Graphics/Effects/ConstantColorEffect.h>
#include <GTL/Mathematics/Approximation/3D/ApprCylinder3.h>
#include <iostream>

// Expose only one of these.
#define USE_MESH_POINTS
//#define USE_CYLINDER_RING
//#define USE_CYLINDER_SKEW

// Expose this if you want the fitter to use the eigenvector corresponding to
// the largest eigenvalue of the covariance matrix as the cylinder axis
// direction. Otherwise, a hemisphere of directions are searched for the
// one that produces the minimum error.
//#define USE_COVARIANCE_W_DIRECTION

// When the hemisphere is searched, we can do this in a single thread,
// which is slow. Or we can search using multiple threads. Expose this
// define if you want a multithreaded search.
#define USE_MULTIPLE_THREADS

ApproximateCylinder3Window3::ApproximateCylinder3Window3(Parameters& parameters)
    :
    Window3(parameters)
{
    if (!SetEnvironment())
    {
        parameters.created = false;
        return;
    }

    mNoCullWireState = std::make_shared<RasterizerState>();
    mNoCullWireState->cull = RasterizerState::Cull::NONE;
    mNoCullWireState->fill = RasterizerState::Fill::WIREFRAME;
    mEngine->SetClearColor({ 0.75f, 0.75f, 0.75f, 1.0f });

    CreateScene();

    InitializeCamera(60.0f, GetAspectRatio(), 0.01f, 100.0f, 0.005f, 0.002f,
        { -30.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });

    mTrackBall.Update();
    mPVWMatrices.Update();
}

void ApproximateCylinder3Window3::OnIdle()
{
    mTimer.Measure();

    if (mCameraRig.Move())
    {
        mPVWMatrices.Update();
    }

    mEngine->ClearBuffers();
    mEngine->Draw(mPoints);
    mEngine->SetRasterizerState(mNoCullWireState);
    mEngine->Draw(mCylinder);
    mEngine->SetDefaultRasterizerState();
    mEngine->DisplayColorBuffer(0);

    mTimer.UpdateFrameCount();
}

bool ApproximateCylinder3Window3::SetEnvironment()
{
    std::string path = GetGTLPath();
    if (path == "")
    {
        return false;
    }

    mEnvironment.Insert(path + "GTL/VisualTests/Mathematics/Approximation/ApproximateCylinder3/Data/");

    if (mEnvironment.GetPath("mesh.txt") == "")
    {
        GTL_RUNTIME_ERROR("Cannot find file mesh.txt.");
        return false;
    }

    return true;
}

void ApproximateCylinder3Window3::CreateScene()
{
    std::vector<Vector3<double>> positions;

#ifdef USE_MESH_POINTS
    std::ifstream input(mEnvironment.GetPath("mesh.txt"));
    std::size_t const numPoints = 10765;
    for (std::size_t i = 0; i < numPoints; ++i)
    {
        Vector3<double> data{};
        input >> data[0];
        input >> data[1];
        input >> data[2];
        positions.push_back(data);
    }
    input.close();
#endif

#ifdef USE_CYLINDER_RING
    for (std::size_t j = 0; j < 64; ++j)
    {
        double theta = C_TWO_PI<double> * static_cast<double>(j) / 64.0;
        double cstheta = std::cos(theta);
        double sntheta = std::sin(theta);
        for (std::size_t i = 0; i <= 64; ++i)
        {
            double t = -2.0 + 4.0 * static_cast<double>(i) / 64.0;
            Vector3<double> sample{ cstheta, sntheta, t };
            positions.push_back(sample);
        }
    }
#endif

#ifdef USE_CYLINDER_SKEW
    double const b = 0.25;
    for (std::size_t j = 0; j < 64; ++j)
    {
        double theta = C_TWO_PI<double> * static_cast<double>(j) / 64.0;
        double cstheta = std::cos(theta);
        double sntheta = std::sin(theta);
        for (std::size_t i = 0; i <= 64; ++i)
        {
            double t = -b + cstheta + 2.0 * b * static_cast<double>(i) / 64.0;
            Vector3<double> sample{ cstheta, sntheta, t };
            positions.push_back(sample);
        }
    }
#endif

    Cylinder3<double> cylinder{};
#ifdef USE_COVARIANCE_W_DIRECTION
    // Use the eigenvector associated with the largest eigenvalue of W.
    ApprCylinder3<double>::FitUsingEigendirection(positions.size(), positions.data(), 2, cylinder);
#else
#ifdef USE_MULTIPLE_THREADS
    // Use all hardware threads available (subject to OS scheduling).
    std::uint32_t numThreads = std::thread::hardware_concurrency();
    ApprCylinder3<double>::FitUsingHemisphereSearch(numThreads, positions.size(),
        positions.data(), 1024, 512, cylinder);
#else
    // Execute the algorithm on the main thread.
    ApprCylinder3<double>::FitUsingHemisphereSearch(0, positions.size(),
        positions.data(), 1024, 512, cylinder);
#endif
#endif
    std::cout << "center = "
        << cylinder.center[0] << " "
        << cylinder.center[1] << " "
        << cylinder.center[2] << std::endl;
    std::cout << "direction = "
        << cylinder.direction[0] << " "
        << cylinder.direction[1] << " "
        << cylinder.direction[2] << std::endl;
    std::cout << "radius = " << cylinder.radius << std::endl;
    std::cout << "height = " << cylinder.height << std::endl;

    // Create point cloud for display.
    VertexFormat vformat;
    vformat.Bind(VASemantic::POSITION, DF_R32G32B32_FLOAT, 0);
    std::uint32_t numVertices = static_cast<std::uint32_t>(positions.size());
    auto vbuffer = std::make_shared<VertexBuffer>(vformat, numVertices);
    auto* vertices = vbuffer->Get<Vector3<float>>();
    for (std::size_t i = 0; i < positions.size(); ++i)
    {
        for (std::size_t j = 0; j < 3; ++j)
        {
            vertices[i][j] = static_cast<float>(positions[i][j]);
        }
    }
    
    auto ibuffer = std::make_shared<IndexBuffer>(IP_POLYPOINT, numVertices);

    auto effect = std::make_shared<ConstantColorEffect>(mProgramFactory,
        Vector4<float>{ 1.0f, 0.0f, 1.0f, 1.0f });

    mPoints = std::make_shared<Visual>(vbuffer, ibuffer, effect);
    mPVWMatrices.Subscribe(mPoints->worldTransform, effect->GetPVWMatrixConstant());
    mTrackBall.Attach(mPoints);

    Vector3<float> translate{};
    for (std::size_t j = 0; j < 3; ++j)
    {
        translate[j] = static_cast<float>(-cylinder.center[j]);
    }
    mPoints->localTransform.SetTranslation(translate);

    Vector3<float> D{}, U{}, V{};
    for (std::size_t j = 0; j < 3; ++j)
    {
        D[j] = static_cast<float>(cylinder.direction[j]);
    }
    ComputeOrthogonalComplement(D, U, V);
    Matrix3x3<float> rotate{};
    rotate.SetCol(0, U);
    rotate.SetCol(1, V);
    rotate.SetCol(2, D);

    MeshFactory mf{};
    mf.SetVertexFormat(vformat);
    mf.SetIndexFormat(true);
    float radius = static_cast<float>(cylinder.radius);
    float height = static_cast<float>(cylinder.height);
#ifdef USE_MESH_POINTS
    mCylinder = mf.CreateCylinderOpen(8, 32, radius, height);
#else
    mCylinder = mf.CreateCylinderOpen(8, 32, radius, height);
#endif
    effect = std::make_shared<ConstantColorEffect>(mProgramFactory,
        Vector4<float>{ 0.0f, 0.0f, 1.0f, 1.0f} );
    mCylinder->SetEffect(effect);
    mCylinder->localTransform.SetRotation(rotate);
    mPVWMatrices.Subscribe(mCylinder);
    mTrackBall.Attach(mCylinder);
}
