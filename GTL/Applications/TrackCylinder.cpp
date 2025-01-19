// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLApplicationsPCH.h>
#include <GTL/Mathematics/Arithmetic/IEEEFunctions.h>
#include <GTL/Applications/TrackCylinder.h>
using namespace gtl;

TrackCylinder::TrackCylinder()
    :
    TrackObject(),
    mInitialYaw(0.0f),
    mYaw(0.0f),
    mInitialPitch(0.0f),
    mPitch(0.0f)
{
    mRoot = std::make_shared<Node>();
}

TrackCylinder::TrackCylinder(int32_t xSize, int32_t ySize, std::shared_ptr<Camera> const& camera)
    :
    TrackObject(xSize, ySize, camera),
    mInitialYaw(0.0f),
    mYaw(0.0f),
    mInitialPitch(0.0f),
    mPitch(0.0f)
{
    Set(xSize, ySize, camera);
    mRoot = std::make_shared<Node>();
}

void TrackCylinder::Reset()
{
    mInitialYaw = 0.0f;
    mInitialPitch = 0.0f;
    mYaw = 0.0f;
    mPitch = 0.0f;
    mRoot->localTransform.MakeIdentity();
    mRoot->Update();
}

void TrackCylinder::OnSetInitialPoint()
{
    mInitialYaw = mYaw;
    mInitialPitch = mPitch;
}

void TrackCylinder::OnSetFinalPoint()
{
    float const pi = C_PI<float>;
    float const halfPi = C_PI_DIV_2<float>;
    float dx = mX1 - mX0;
    float dy = mY1 - mY0;
    float angle = dx * pi;
    mYaw = mInitialYaw + angle;
    angle = -dy * pi;
    mPitch = mInitialPitch + angle;
    mPitch = clamp(mPitch, -halfPi, halfPi);

    // The angle order depends on camera {D=0, U=1, R=2}.
    AxisAngle<float> yawAxisAngle(Vector3<float>::Unit(2), mYaw);
    Matrix3x3<float> yawRotate = Rotation<float>(yawAxisAngle);
    AxisAngle<float> pitchAxisAngle(Vector3<float>::Unit(1), mPitch);
    Matrix3x3<float> pitchRotate = Rotation<float>(pitchAxisAngle);
    Matrix3x3<float> rotate = pitchRotate * yawRotate;

    NormalizeAndUpdateRoot(rotate);
}
