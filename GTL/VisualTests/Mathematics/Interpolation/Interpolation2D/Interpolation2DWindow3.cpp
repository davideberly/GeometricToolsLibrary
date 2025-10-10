// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.10

#include "Interpolation2DWindow3.h"
#include <GTL/Applications/WICFileIO.h>
#include <GTL/Graphics/SceneGraph/MeshFactory.h>
#include <GTL/Mathematics/Interpolation/2D/IntpAkimaUniform2.h>
#include <GTL/Mathematics/Interpolation/2D/IntpCubic2.h>
#include <GTL/Mathematics/Interpolation/2D/IntpLinear2.h>
#include <GTL/Mathematics/Interpolation/2D/IntpLinearNonuniform2.h>
#include <GTL/Mathematics/Interpolation/2D/IntpQuadraticNonuniform2.h>
#include <GTL/Mathematics/Interpolation/2D/IntpThinPlateSpline2.h>
#include <random>

Interpolation2DWindow3::Interpolation2DWindow3(Parameters& parameters)
    :
    Window3(parameters),
    mName{},
    mMesh{},
    mTexture{},
    mEffect{},
    mNoCullSolidState{},
    mNoCullWireState{},
    mF(NUM_NEIGHBORS, 0.0f),
    mDFDX(NUM_NEIGHBORS, 0.0f),
    mDFDY(NUM_NEIGHBORS, 0.0f),
    mFSample(SAMPLE_BOUNDSQR, 0.0f),
    mPlanarMesh{}
{
    if (!SetEnvironment())
    {
        parameters.created = false;
        return;
    }

    CreateCommonObjects();
    CreatePlanarMesh();
    CreateBilinearMesh();
    OnDisplay();
}

void Interpolation2DWindow3::OnIdle()
{
    if (mCameraRig.Move())
    {
        mPVWMatrices.Update();
    }

    mEngine->ClearBuffers();
    mEngine->Draw(mMesh);
    mEngine->Draw(8, 24, { 0.0f, 0.0f, 0.0f, 1.0f }, mName);
    mEngine->DisplayColorBuffer(0);
}

bool Interpolation2DWindow3::OnCharPress(uint8_t key, int32_t x, int32_t y)
{
    switch (key)
    {
    case '0':
        if (mName != "Bilinear2")
        {
            CreateBilinearMesh();
        }
        return true;
    case '1':
        if (mName != "Bicubic2_CM")
        {
            CreateBicubicMesh(true);
        }
        return true;
    case '2':
        if (mName != "Bicubic2_BS")
        {
            CreateBicubicMesh(false);
        }
        return true;
    case '3':
        if (mName != "AkimaUniform2_BS")
        {
            CreateAkimaUniformMesh();
        }
        return true;
    case '4':
        if (mName != "ThinPlateSpline2_0")
        {
            CreateThinPlateSplineMesh(0.0f);
        }
        return true;
    case '5':
        if (mName != "ThinPlateSpline2_1")
        {
            CreateThinPlateSplineMesh(1.0f);
        }
        return true;
    case '6':
        if (mName != "ThinPlateSpline2_10")
        {
            CreateThinPlateSplineMesh(10.0f);
        }
        return true;
    case '7':
        if (mName != "LinearNonuniform")
        {
            CreateLinearNonuniform();
        }
        return true;
    case '8':
        if (mName != "QuadraticNonuniform_NOGRAD")
        {
            CreateQuadraticNonuniform(false);
        }
        return true;
    case '9':
        if (mName != "QuadraticNonuniform_GRAD")
        {
            CreateQuadraticNonuniform(true);
        }
        return true;
    case 'w':
    case 'W':
        if (mEngine->GetRasterizerState() == mNoCullSolidState)
        {
            mEngine->SetRasterizerState(mNoCullWireState);
        }
        else
        {
            mEngine->SetRasterizerState(mNoCullSolidState);
        }
        return true;
    }

    return Window3::OnCharPress(key, x, y);
}

bool Interpolation2DWindow3::SetEnvironment()
{
    std::string path = GetGTLPath();
    if (path == "")
    {
        return false;
    }

    mEnvironment.Insert(path + "GTL/VisualTests/Data/");

    if (mEnvironment.GetPath("Checkerboard.png") == "")
    {
        GTL_RUNTIME_ERROR("Cannot find file Checkerboard.png.");
        return false;
    }

    return true;
}

void Interpolation2DWindow3::CreateCommonObjects()
{
    InitializeCamera(60.0f, GetAspectRatio(), 0.1f, 100.0f, 0.001f, 0.001f,
        { 0.0f, 0.0f, 2.5f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f });

    std::string path = mEnvironment.GetPath("Checkerboard.png");
    mTexture = WICFileIO::Load(path, true);
    mTexture->AutogenerateMipmaps();
    mEffect = std::make_shared<Texture2Effect>(mProgramFactory, mTexture,
        SamplerState::Filter::MIN_L_MAG_L_MIP_L, SamplerState::Mode::CLAMP,
        SamplerState::Mode::CLAMP);

    mNoCullSolidState = std::make_shared<RasterizerState>();
    mNoCullSolidState->cull = RasterizerState::Cull::NONE;
    mNoCullSolidState->fill = RasterizerState::Fill::SOLID;
    mEngine->SetRasterizerState(mNoCullSolidState);

    mNoCullWireState = std::make_shared<RasterizerState>();
    mNoCullWireState->cull = RasterizerState::Cull::NONE;
    mNoCullWireState->fill = RasterizerState::Fill::WIREFRAME;

    float const maxRnd = 0.125f;
    std::mt19937 mte{};
    std::uniform_real_distribution<float> rnd(0.0f, maxRnd);
    for (std::size_t y = 0; y < SAMPLE_BOUND; ++y)
    {
        for (std::size_t x = 0; x < SAMPLE_BOUND; ++x)
        {
            mFSample[x + SAMPLE_BOUND * y] = rnd(mte);
        }
    }

    std::uniform_real_distribution<float> urd(-1.0f, 1.0f);
    for (std::size_t i = 0; i < NUM_NEIGHBORS; ++i)
    {
        mF[i] = urd(mte);
        mDFDX[i] = urd(mte);
        mDFDY[i] = urd(mte);
    }
}

void Interpolation2DWindow3::OnPrecreateMesh()
{
    if (mMesh)
    {
        mTrackBall.Detach(mMesh);
        mPVWMatrices.Unsubscribe(mMesh);
    }
}

void Interpolation2DWindow3::OnPostcreateMesh()
{
    mMesh->SetEffect(mEffect);
    mPVWMatrices.Subscribe(mMesh);
    mTrackBall.Attach(mMesh);
    mTrackBall.Update();
    mPVWMatrices.Update();
}

void Interpolation2DWindow3::CreatePlanarMesh()
{
    std::vector<Vector2<float>> positions(6);
    positions[0] = { 0.0f, 0.0f };
    positions[1] = { 0.5f, 0.0f };
    positions[2] = { 1.0f, 0.0f };
    positions[3] = { 0.0f, 0.5f };
    positions[4] = { 0.5f, 0.5f };
    positions[5] = { 0.0f, 1.0f };

    std::vector<std::array<std::size_t, 3>> triangles(4);
    triangles[0] = { 0, 1, 3 };
    triangles[1] = { 1, 2, 4 };
    triangles[2] = { 3, 4, 5 };
    triangles[3] = { 1, 4, 3 };

    std::size_t constexpr numThreads = 0;
    mPlanarMesh.Create(positions, triangles, numThreads);
}

void Interpolation2DWindow3::CreateBilinearMesh()
{
    OnPrecreateMesh();

    mName = "Bilinear2";

    float const spacing = 2.0f / static_cast<float>(SAMPLE_BOUND - 1);
    IntpLinear2<float> interp(SAMPLE_BOUND, SAMPLE_BOUND, -1.0f, spacing,
        -1.0f, spacing, mFSample.data());

    VertexFormat vformat{};
    vformat.Bind(VASemantic::POSITION, DF_R32G32B32_FLOAT, 0);
    vformat.Bind(VASemantic::TEXCOORD, DF_R32G32_FLOAT, 0);

    MeshFactory mf{};
    mf.SetVertexFormat(vformat);
    std::size_t constexpr numSamples = 64;
    mMesh = mf.CreateRectangle(static_cast<std::uint32_t>(numSamples),
        static_cast<std::uint32_t>(numSamples), 1.0f, 1.0f);
    auto* vertices = mMesh->GetVertexBuffer()->Get<Vertex>();
    for (std::size_t y = 0, i = 0; y < numSamples; ++y)
    {
        float fy = vertices[i].position[1];
        for (std::size_t x = 0; x < numSamples; ++x, ++i)
        {
            float fx = vertices[i].position[0];
            vertices[i].position[2] = interp(fx, fy);
        }
    }

    OnPostcreateMesh();
}

void Interpolation2DWindow3::CreateBicubicMesh(bool catmullRom)
{
    OnPrecreateMesh();

    if (catmullRom)
    {
        mName = "Bicubic2_CR";
    }
    else
    {
        mName = "Bicubic2_BS";
    }

    float const spacing = 2.0f / static_cast<float>(SAMPLE_BOUND - 1);
    IntpCubic2<float> interp(SAMPLE_BOUND, SAMPLE_BOUND, -1.0f, spacing,
        -1.0f, spacing, mFSample.data(), catmullRom);

    VertexFormat vformat{};
    vformat.Bind(VASemantic::POSITION, DF_R32G32B32_FLOAT, 0);
    vformat.Bind(VASemantic::TEXCOORD, DF_R32G32_FLOAT, 0);

    MeshFactory mf{};
    mf.SetVertexFormat(vformat);
    std::size_t constexpr numSamples = 64;
    mMesh = mf.CreateRectangle(static_cast<std::uint32_t>(numSamples),
        static_cast<std::uint32_t>(numSamples), 1.0f, 1.0f);
    auto* vertices = mMesh->GetVertexBuffer()->Get<Vertex>();
    for (std::size_t y = 0, i = 0; y < numSamples; ++y)
    {
        float fy = vertices[i].position[1];
        for (std::size_t x = 0; x < numSamples; ++x, ++i)
        {
            float fx = vertices[i].position[0];
            vertices[i].position[2] = interp(fx, fy);
        }
    }

    OnPostcreateMesh();
}

void Interpolation2DWindow3::CreateAkimaUniformMesh()
{
    OnPrecreateMesh();

    mName = "AkimaUniform2";

    IntpAkimaUniform2<float> interp(
        SAMPLE_BOUND, -1.0f, 1.0f,
        SAMPLE_BOUND, -1.0f, 1.0f,
        mFSample.data());

    VertexFormat vformat{};
    vformat.Bind(VASemantic::POSITION, DF_R32G32B32_FLOAT, 0);
    vformat.Bind(VASemantic::TEXCOORD, DF_R32G32_FLOAT, 0);

    MeshFactory mf{};
    mf.SetVertexFormat(vformat);
    std::size_t constexpr numSamples = 64;
    mMesh = mf.CreateRectangle(static_cast<std::uint32_t>(numSamples),
        static_cast<std::uint32_t>(numSamples), 1.0f, 1.0f);
    auto* vertices = mMesh->GetVertexBuffer()->Get<Vertex>();
    for (std::size_t y = 0, i = 0; y < numSamples; ++y)
    {
        float fy = vertices[i].position[1];
        for (std::size_t x = 0; x < numSamples; ++x, ++i)
        {
            float fx = vertices[i].position[0];
            vertices[i].position[2] = interp(fx, fy);
        }
    }

    OnPostcreateMesh();
}

void Interpolation2DWindow3::CreateThinPlateSplineMesh(float smooth)
{
    OnPrecreateMesh();

    if (smooth == 0.0f)
    {
        mName = "ThinPlateSpline2_0";
    }
    else if (smooth == 1.0f)
    {
        mName = "ThinPlateSpline2_1";
    }
    else
    {
        mName = "ThinPlateSpline2_10";
    }

    float const spacing = 2.0f / static_cast<float>(SAMPLE_BOUND - 1);
    std::vector<Vector3<float>> points(SAMPLE_BOUNDSQR);
    for (std::size_t y = 0, i = 0; y < SAMPLE_BOUND; ++y)
    {
        float fy = -1.0f + spacing * y;
        for (std::size_t x = 0; x < SAMPLE_BOUND; ++x, ++i)
        {
            float fx = -1.0f + spacing * x;
            points[i][0] = fx;
            points[i][1] = fy;
            points[i][2] = mFSample[i];
        }
    }

    IntpThinPlateSpline2<float> interp(points, smooth, false);

    VertexFormat vformat{};
    vformat.Bind(VASemantic::POSITION, DF_R32G32B32_FLOAT, 0);
    vformat.Bind(VASemantic::TEXCOORD, DF_R32G32_FLOAT, 0);

    MeshFactory mf{};
    mf.SetVertexFormat(vformat);
    std::size_t constexpr numSamples = 64;
    mMesh = mf.CreateRectangle(static_cast<std::uint32_t>(numSamples),
        static_cast<std::uint32_t>(numSamples), 1.0f, 1.0f);
    auto* vertices = mMesh->GetVertexBuffer()->Get<Vertex>();
    for (std::size_t y = 0, i = 0; y < numSamples; ++y)
    {
        float fy = vertices[i].position[1];
        for (std::size_t x = 0; x < numSamples; ++x, ++i)
        {
            float fx = vertices[i].position[0];
            vertices[i].position[2] = interp(fx, fy);
        }
    }

    OnPostcreateMesh();
}

void Interpolation2DWindow3::CreateLinearNonuniform()
{
    OnPrecreateMesh();

    mName = "LinearNonuniform";

    bool constexpr meshIsConvex = true;
    std::size_t constexpr numThreads = 0;
    IntpLinearNonuniform2<float> interpolator(mPlanarMesh, mF, meshIsConvex, numThreads);

    VertexFormat vformat{};
    vformat.Bind(VASemantic::POSITION, DF_R32G32B32_FLOAT, 0);
    vformat.Bind(VASemantic::TEXCOORD, DF_R32G32_FLOAT, 0);

    MeshFactory mf{};
    mf.SetVertexFormat(vformat);
    std::uint32_t constexpr numSamples = 64;
    mMesh = mf.CreateTriangle(numSamples, 1.0f, 1.0f);
    auto* vertices = mMesh->GetVertexBuffer()->Get<Vertex>();
    Vector2<float> P{};
    float F{};
    for (std::uint32_t y = 0, i = 0; y < numSamples; ++y)
    {
        for (std::uint32_t x = 0; x + y < numSamples; ++x, ++i)
        {
            // For the sake of the demonstration, ensure P is inside the
            // domain of the interpolator for all (x,y).
            P[0] = std::min(std::max(vertices[i].position[0], 0.0f), 1.0f);
            P[1] = std::min(std::max(vertices[i].position[1], 0.0f), 1.0f);
            float sum = P[0] + P[1];
            if (sum > 1.0f)
            {
                P[0] /= sum;
                P[1] /= sum;
            }

            if (interpolator.Evaluate(P, F))
            {
                // Theoretically, this is the expected case for evaluation.
                vertices[i].position[2] = F;
            }
            else
            {
                // Floating-point rounding errors can lead to incorrect
                // point-in-triangle tests in the PlanarMesh<T> class. The
                // P[] values might sum to 1 (with rounding errors) but as a
                // rational 2-tuple, P is reported to be outside the triangle.
                // Use rational arithmetic to attempt a fix.
                Vector2<Rational> rP{ P[0], P[1] };
                Rational rSum = rP[0] + rP[1];
                rP /= rSum;
                Rational rHalfExcess = Rational(0.5) * (rSum - Rational(1));
                if (rHalfExcess.GetSign() > 0)
                {
                    rP[0] -= rHalfExcess;
                    rP[1] -= rHalfExcess;
                    P[0] = rP[0];
                    P[1] = rP[1];
                }

                if (interpolator.Evaluate(P, F))
                {
                    vertices[i].position[2] = F;
                }
                else
                {
                    GTL_RUNTIME_ERROR("Rounding errors led to incorrect evaluation.");
                }
            }
        }
    }

    OnPostcreateMesh();
}

void Interpolation2DWindow3::CreateQuadraticNonuniform(bool useGradients)
{
    OnPrecreateMesh();

    bool constexpr meshIsConvex = true;
    std::size_t constexpr numThreads = 0;
    std::unique_ptr<IntpQuadraticNonuniform2<float>> interpolator{};
    if (useGradients)
    {
        mName = "QuadraticNonuniform_GRAD";
        interpolator = std::make_unique<IntpQuadraticNonuniform2<float>>(
            mPlanarMesh, mF, mDFDX, mDFDY, meshIsConvex, numThreads);
    }
    else
    {
        mName = "QuadraticNonuniform_NOGRAD";
        float constexpr spatialDelta = 0.125f;
        interpolator = std::make_unique<IntpQuadraticNonuniform2<float>>(
            mPlanarMesh, mF, spatialDelta, meshIsConvex, numThreads);
    }

    VertexFormat vformat{};
    vformat.Bind(VASemantic::POSITION, DF_R32G32B32_FLOAT, 0);
    vformat.Bind(VASemantic::TEXCOORD, DF_R32G32_FLOAT, 0);

    MeshFactory mf{};
    mf.SetVertexFormat(vformat);
    std::uint32_t const numSamples = 64;
    mMesh = mf.CreateTriangle(numSamples, 1.0f, 1.0f);
    auto* vertices = mMesh->GetVertexBuffer()->Get<Vertex>();
    Vector2<float> P{};
    float F{}, FX{}, FY{};
    for (std::uint32_t y = 0, i = 0; y < numSamples; ++y)
    {
        for (std::uint32_t x = 0; x + y < numSamples; ++x, ++i)
        {
            // For the sake of the demonstration, ensure P is inside the
            // domain of the interpolator for all (x,y).
            P[0] = std::min(std::max(vertices[i].position[0], 0.0f), 1.0f);
            P[1] = std::min(std::max(vertices[i].position[1], 0.0f), 1.0f);

            if (interpolator->Evaluate(P, F, FX, FY))
            {
                // Theoretically, this is the expected case for evaluation.
                vertices[i].position[2] = F;
            }
            else
            {
                // Floating-point rounding errors can lead to incorrect
                // point-in-triangle tests in the PlanarMesh<T> class. The
                // P[] values might sum to 1 (with rounding errors) but as a
                // rational 2-tuple, P is reported to be outside the triangle.
                // Use rational arithmetic to attempt a fix.
                Vector2<Rational> rP{ P[0], P[1] };
                Rational rSum = rP[0] + rP[1];
                rP /= rSum;
                Rational rHalfExcess = Rational(0.5) * (rSum - Rational(1));
                if (rHalfExcess.GetSign() > 0)
                {
                    rP[0] -= rHalfExcess;
                    rP[1] -= rHalfExcess;
                    P[0] = rP[0];
                    P[1] = rP[1];
                }

                if (interpolator->Evaluate(P, F, FX, FY))
                {
                    vertices[i].position[2] = F;
                }
                else
                {
                    GTL_RUNTIME_ERROR("Rounding errors led to incorrect evaluation.");
                }
            }
        }
    }

    OnPostcreateMesh();
}
