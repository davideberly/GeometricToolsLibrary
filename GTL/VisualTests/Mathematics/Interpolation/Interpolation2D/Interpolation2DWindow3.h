// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.10

#pragma once

#include <GTL/Applications/Window3.h>
#include <GTL/Graphics/Effects/Texture2Effect.h>
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#include <GTL/Mathematics/Meshes/PlanarMesh.h>
#include <functional>
using namespace gtl;

class Interpolation2DWindow3 : public Window3
{
public:
    Interpolation2DWindow3(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y) override;

private:
    using Rational = BSRational<UIntegerAP32>;

    struct Vertex
    {
        Vector3<float> position;
        Vector2<float> tcoord;
    };

    bool SetEnvironment();
    void CreateCommonObjects();
    void OnPrecreateMesh();
    void OnPostcreateMesh();
    void CreatePlanarMesh();
    void CreateBilinearMesh();
    void CreateBicubicMesh(bool catmullRom);
    void CreateAkimaUniformMesh();
    void CreateThinPlateSplineMesh(float smooth);
    void CreateLinearNonuniform();
    void CreateQuadraticNonuniform(bool useGradients);

    std::string mName;
    std::shared_ptr<Visual> mMesh;
    std::shared_ptr<Texture2> mTexture;
    std::shared_ptr<Texture2Effect> mEffect;
    std::shared_ptr<RasterizerState> mNoCullSolidState;
    std::shared_ptr<RasterizerState> mNoCullWireState;

    // For use by Bilinear, Bicubic, Akima, ThinPlateSpline.  The grid size
    // is SAMPLE_BOUND-by-SAMPLE_BOUND.
    static std::size_t constexpr SAMPLE_BOUND = 8;
    static std::size_t constexpr SAMPLE_BOUNDSQR = SAMPLE_BOUND * SAMPLE_BOUND;

    // mFSample is a 2D array with SAMPLE_BOUNDSQR members.
    std::vector<float> mFSample;

    // Neighborhood information, each array with NUM_NEIGHBORS members.
    static std::size_t constexpr NUM_NEIGHBORS = 6;
    std::vector<float> mF, mDFDX, mDFDY;

    // For use by LinearNonuniform, QuadraticNonuniform.
    PlanarMesh<float> mPlanarMesh;
};
