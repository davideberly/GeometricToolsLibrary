// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.09.23

#pragma once

#include <GTL/Applications/Window3.h>
#include <GTL/Mathematics/Intersection/3D/IntrAlignedBox3Sphere3.h>
#include <GTL/Mathematics/Intersection/3D/IntrOrientedBox3Sphere3.h>
using namespace gtl;

// The default is to test the query for aligned box and sphere.
// Uncomment this to test the query for oriented box and sphere.
//#define APP_USE_OBB

class IntersectMovingSphereBoxWindow3 : public Window3
{
public:
    IntersectMovingSphereBoxWindow3(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(std::uint8_t key, std::int32_t x, std::int32_t y) override;

private:
    static std::int32_t constexpr density = 32;

    void CreateScene();
    void CreateRoundedBoxVertices();
    void CreateRoundedBoxEdges();
    void CreateRoundedBoxFaces();
    void CreateBox();
    void CreateSpheres();
    void CreateMotionCylinder();
    void UpdateSphereVelocity();
    void UpdateSphereCenter();

    std::shared_ptr<BlendState> mBlendState;
    std::shared_ptr<RasterizerState> mNoCullState;
    float mAlpha;

    // Octants of spheres for the rounded box corners.
    std::array<std::shared_ptr<Visual>, 8> mVertexVisual;

    // Quarter cylinders for the rounded box edges.
    std::array<std::shared_ptr<Visual>, 12> mEdgeVisual;

    // Rectangles for the rounded box faces.
    std::array<std::shared_ptr<Visual>, 6> mFaceVisual;

    // The visual representation of mBox.
    std::shared_ptr<Visual> mBoxVisual;

    // The scene graph that represents the box and features.
    std::shared_ptr<Node> mBoxRoot;

    // The visual representation of mSphere.
    std::shared_ptr<Visual> mSphereVisual;
    std::shared_ptr<Visual> mSphereContactVisual;

    // The visual representation of the moving path of the sphere.
    std::shared_ptr<Visual> mVelocityVisual;

    // The contact point representation.
    std::shared_ptr<Visual> mPointContactVisual;

#if defined(APP_USE_OBB)
    OrientedBox3<float> mBox;
    FIQuery<float, OrientedBox3<float>, Sphere3<float>> mQuery;
#else
    AlignedBox3<float> mBox;
    FIQuery<float, AlignedBox3<float>, Sphere3<float>> mQuery;
#endif
    Sphere3<float> mSphere;
    Vector3<float> mBoxVelocity;
    Vector3<float> mSphereVelocity;
    std::int32_t mNumSamples0, mNumSamples1, mSample0, mSample1;
    float mDX, mDY, mDZ;
    std::string mMessage;
    bool mDrawSphereVisual;
};
