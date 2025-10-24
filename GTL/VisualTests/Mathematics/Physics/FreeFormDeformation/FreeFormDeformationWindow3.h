// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.24

#pragma once

#include <GTL/Applications/Window3.h>
#include <GTL/Utility/Timer.h>
#include <GTL/Graphics/SceneGraph/Picking/Picker.h>
#include <GTL/Mathematics/Volumes/NURBSVolume.h>
using namespace gtl;

class FreeFormDeformationWindow3 : public Window3
{
public:
    FreeFormDeformationWindow3(Parameters& parameters);

    virtual void OnIdle() override;

    virtual bool OnCharPress(std::uint8_t key, std::int32_t x, std::int32_t y) override;

    virtual bool OnMouseClick(MouseButton button, MouseState state,
        std::int32_t x, std::int32_t y, std::uint32_t modifiers) override;

    virtual bool OnMouseMotion(MouseButton button, std::int32_t x, std::int32_t y,
        std::uint32_t modifiers) override;

private:
    bool SetEnvironment();
    void CreateScene();
    void CreateBSplineVolume();
    void CreateSegments();
    void CreateBoxes();

    void UpdateMesh();
    void UpdateSegments();
    void UpdateBoxes();

    void DoRandomControlPoints();
    void OnMouseDown(std::int32_t x, std::int32_t y);
    void OnMouseMove(std::int32_t x, std::int32_t y);

    // The scene graph objects.
    struct Vertex
    {
        Vector3<float> position;
        Vector2<float> tcoord;
    };

    std::shared_ptr<RasterizerState> mWireState;
    std::shared_ptr<Node> mScene;
    std::shared_ptr<Visual> mMesh;
    Vector4<float> mRed, mGreen, mBlue, mGray;

    // The control volume for deformation. The mParameters are the
    // (u,v,w) for the mesh vertices. The grid is 4x4x4 with cubic
    // polynomials per dimension.
    static std::size_t constexpr quantity = 4;
    static std::size_t constexpr degree = 3;
    std::unique_ptr<NURBSVolume<float, 3>> mSpline;
    Vector3<float> mMin, mMax, mDelta;
    std::vector<Vector3<float>> mParameters;

    // Q control points per dimension, 3*Q^2*(Q-1) polysegments to connect them.
    std::shared_ptr<Node> mPolysegmentRoot;
    std::vector<std::shared_ptr<Visual>> mSegments;

    // User-adjusted controls.
    std::shared_ptr<Node> mControlRoot;
    std::shared_ptr<Visual> mSelected;
    Vector4<float> mOldWorldPosition;
    std::vector<std::shared_ptr<Visual>> mBoxes;

    // Toggle between automated random motion and user-adjusted controls.
    Timer mMotionTimer;
    float mAmplitude, mRadius;
    double mLastUpdateTime;
    bool mDoRandom;

    // Toggle drawing of segments/boxes.
    bool mDrawSegmentsBoxes;

    // Picking support for selecting and moving the control points.
    Picker mPicker;
    bool mMouseDown;
};
