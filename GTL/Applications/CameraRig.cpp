// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLApplicationsPCH.h>
#include <GTL/Mathematics/Algebra/Rotation.h>
#include <GTL/Applications/CameraRig.h>
using namespace gtl;

CameraRig::CameraRig()
    :
    mCamera{},
    mTranslationSpeed(0.0f),
    mRotationSpeed(0.0f),
    mWorldAxis{},
    mMotion{},
    mIndirectMap{},
    mNumActiveMotions(0),
    mActiveMotions{}
{
    Set(nullptr, 0.0f, 0.0f);
}

CameraRig::CameraRig(std::shared_ptr<Camera> const& camera,
    float translationSpeed, float rotationSpeed)
    :
    mCamera{},
    mTranslationSpeed(0.0f),
    mRotationSpeed(0.0f),
    mWorldAxis{},
    mMotion{},
    mIndirectMap{},
    mNumActiveMotions(0),
    mActiveMotions{}
{
    Set(camera, translationSpeed, rotationSpeed);
}

void CameraRig::Set(std::shared_ptr<Camera> const& camera,
    float translationSpeed, float rotationSpeed)
{
    mCamera = camera;
    mTranslationSpeed = translationSpeed;
    mRotationSpeed = rotationSpeed;
    ComputeWorldAxes();
    ClearMotions();
}

void CameraRig::ComputeWorldAxes()
{
    if (mCamera)
    {
        mWorldAxis[0] = mCamera->GetDVector();
        mWorldAxis[1] = mCamera->GetUVector();
        mWorldAxis[2] = mCamera->GetRVector();
    }
    else
    {
        MakeZero(mWorldAxis[0]);
        MakeZero(mWorldAxis[1]);
        MakeZero(mWorldAxis[2]);
    }
}

bool CameraRig::PushMotion(int32_t trigger)
{
    auto element = mIndirectMap.find(trigger);
    return (element != mIndirectMap.end() ? SetActive(element->second) : false);
}

bool CameraRig::PopMotion(int32_t trigger)
{
    auto element = mIndirectMap.find(trigger);
    return (element != mIndirectMap.end() ? SetInactive(element->second) : false);
}

bool CameraRig::Move()
{
    // The current semantics allow for processing all active motions,
    // which was the semantics in Wild Magic 5.  For example, if you
    // move the camera with the up-arrow (forward motion) and with
    // the right-arrow (turn-right motion), both will occur during the
    // idle loop.
    if (mNumActiveMotions > 0)
    {
        for (int32_t i = 0; i < mNumActiveMotions; ++i)
        {
            (this->*mActiveMotions[i])();
        }
        return true;
    }
    return false;

    // If you prefer the previous semantics where only one key press is
    // processed at a time, use this previous version of the code instead
    // of the block of code above.
    //
    // if (mMotion)
    // {
    //     (this->*mMotion)();
    //     return true;
    // }
    // return false;
}

void CameraRig::ClearMotions()
{
    mMotion = nullptr;
    mIndirectMap.clear();
    mNumActiveMotions = 0;
    std::fill(mActiveMotions.begin(), mActiveMotions.end(), nullptr);
}

void CameraRig::MoveForward()
{
    if (mCamera)
    {
        mCamera->SetPosition(mCamera->GetPosition() + mTranslationSpeed * mWorldAxis[0]);
    }
}

void CameraRig::MoveBackward()
{
    if (mCamera)
    {
        mCamera->SetPosition(mCamera->GetPosition() - mTranslationSpeed * mWorldAxis[0]);
    }
}

void CameraRig::MoveUp()
{
    if (mCamera)
    {
        mCamera->SetPosition(mCamera->GetPosition() + mTranslationSpeed*mWorldAxis[1]);
    }
}

void CameraRig::MoveDown()
{
    if (mCamera)
    {
        mCamera->SetPosition(mCamera->GetPosition() - mTranslationSpeed*mWorldAxis[1]);
    }
}

void CameraRig::MoveRight()
{
    if (mCamera)
    {
        mCamera->SetPosition(mCamera->GetPosition() + mTranslationSpeed*mWorldAxis[2]);
    }
}

void CameraRig::MoveLeft()
{
    if (mCamera)
    {
        mCamera->SetPosition(mCamera->GetPosition() - mTranslationSpeed*mWorldAxis[2]);
    }
}

void CameraRig::TurnRight()
{
    if (mCamera)
    {
        Matrix3x3<float> incremental = Rotation<float>(
            AxisAngle<float>(HProject(mWorldAxis[1]), -mRotationSpeed));

        Matrix4x4<float> incremental4 = HLift(incremental);

        mWorldAxis[0] = incremental4 * mWorldAxis[0];
        mWorldAxis[2] = incremental4 * mWorldAxis[2];
        mCamera->SetAxes(
            incremental4 * mCamera->GetDVector(),
            incremental4 * mCamera->GetUVector(),
            incremental4 * mCamera->GetRVector());
    }
}

void CameraRig::TurnLeft()
{
    if (mCamera)
    {
        Matrix3x3<float> incremental = Rotation<float>(
            AxisAngle<float>(HProject(mWorldAxis[1]), +mRotationSpeed));

        Matrix4x4<float> incremental4 = HLift(incremental);

        mWorldAxis[0] = incremental4 * mWorldAxis[0];
        mWorldAxis[2] = incremental4 * mWorldAxis[2];
        mCamera->SetAxes(
            incremental4 * mCamera->GetDVector(),
            incremental4 * mCamera->GetUVector(),
            incremental4 * mCamera->GetRVector());
    }
}

void CameraRig::LookUp()
{
    if (mCamera)
    {
        Matrix3x3<float> incremental = Rotation<float>(
            AxisAngle<float>(HProject(mWorldAxis[2]), +mRotationSpeed));

        Matrix4x4<float> incremental4 = HLift(incremental);

        mCamera->SetAxes(
            incremental4 * mCamera->GetDVector(),
            incremental4 * mCamera->GetUVector(),
            incremental4 * mCamera->GetRVector());
    }
}

void CameraRig::LookDown()
{
    if (mCamera)
    {
        Matrix3x3<float> incremental = Rotation<float>(
            AxisAngle<float>(HProject(mWorldAxis[2]), -mRotationSpeed));

        Matrix4x4<float> incremental4 = HLift(incremental);

        mCamera->SetAxes(
            incremental4 * mCamera->GetDVector(),
            incremental4 * mCamera->GetUVector(),
            incremental4 * mCamera->GetRVector());
    }
}

void CameraRig::RollClockwise()
{
    if (mCamera)
    {
        Matrix3x3<float> incremental = Rotation<float>(
            AxisAngle<float>(HProject(mWorldAxis[0]), +mRotationSpeed));

        Matrix4x4<float> incremental4 = HLift(incremental);

        mCamera->SetAxes(
            incremental4 * mCamera->GetDVector(),
            incremental4 * mCamera->GetUVector(),
            incremental4 * mCamera->GetRVector());
    }
}

void CameraRig::RollCounterclockwise()
{
    if (mCamera)
    {
        Matrix3x3<float> incremental = Rotation<float>(
            AxisAngle<float>(HProject(mWorldAxis[0]), -mRotationSpeed));

        Matrix4x4<float> incremental4 = HLift(incremental);

        mCamera->SetAxes(
            incremental4 * mCamera->GetDVector(),
            incremental4 * mCamera->GetUVector(),
            incremental4 * mCamera->GetRVector());
    }
}

void CameraRig::Register(int32_t trigger, MoveFunction function)
{
    if (trigger >= 0)
    {
        auto element = mIndirectMap.find(trigger);
        if (element == mIndirectMap.end())
        {
            mIndirectMap.insert(std::make_pair(trigger, function));
        }
    }
    else
    {
        for (auto const& element : mIndirectMap)
        {
            if (element.second == function)
            {
                mIndirectMap.erase(trigger);
                return;
            }
        }
    }
}

bool CameraRig::SetActive(MoveFunction function)
{
    for (int32_t i = 0; i < mNumActiveMotions; ++i)
    {
        if (mActiveMotions[i] == function)
        {
            return false;
        }
    }

    if (mNumActiveMotions < MAX_ACTIVE_MOTIONS)
    {
        mMotion = function;
        mActiveMotions[mNumActiveMotions] = function;
        ++mNumActiveMotions;
        return true;
    }

    return false;
}

bool CameraRig::SetInactive(MoveFunction function)
{
    for (int32_t i = 0; i < mNumActiveMotions; ++i)
    {
        if (mActiveMotions[i] == function)
        {
            for (int32_t j0 = i, j1 = j0 + 1; j1 < mNumActiveMotions; j0 = j1++)
            {
                mActiveMotions[j0] = mActiveMotions[j1];
            }
            --mNumActiveMotions;
            mActiveMotions[mNumActiveMotions] = nullptr;
            if (mNumActiveMotions > 0)
            {
                mMotion = mActiveMotions[static_cast<size_t>(mNumActiveMotions) - 1];
            }
            else
            {
                mMotion = nullptr;
            }
            return true;
        }
    }
    return false;
}
