// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLApplicationsPCH.h>
#include <GTL/Applications/TrackBall.h>
using namespace gtl;

TrackBall::TrackBall()
    :
    TrackObject()
{
    mRoot = std::make_shared<Node>();
    MakeIdentity(mInitialOrientation);
}

TrackBall::TrackBall(int32_t xSize, int32_t ySize, std::shared_ptr<Camera> const& camera)
    :
    TrackObject(xSize, ySize, camera)
{
    Set(xSize, ySize, camera);
    mRoot = std::make_shared<Node>();
    MakeIdentity(mInitialOrientation);
}

void TrackBall::Reset()
{
    MakeIdentity(mInitialOrientation);
    mRoot->localTransform.MakeIdentity();
    mRoot->Update();
}

void TrackBall::OnSetInitialPoint()
{
    mInitialOrientation = mRoot->localTransform.GetRotation();
}

void TrackBall::OnSetFinalPoint()
{
    // Get the first vector on the sphere.
    float sqrLength0 = mX0 * mX0 + mY0 * mY0;
    float length0 = std::sqrt(sqrLength0), invLength0 = 0.0f, z0, z1;
    if (length0 > 1.0f)
    {
        // Outside the unit disk, project onto it.
        invLength0 = 1.0f / length0;
        mX0 *= invLength0;
        mY0 *= invLength0;
        z0 = 0.0f;
    }
    else
    {
        // Compute point (mX0,mY0,z0) on negative unit hemisphere.
        z0 = 1.0f - sqrLength0;
        z0 = (z0 <= 0.0f ? 0.0f : std::sqrt(z0));
    }
    z0 *= -1.0f;

    // Use camera world coordinates, order is (D,U,R), so point is (z,y,x).
    Vector4<float> vec0{ z0, mY0, mX0, 0.0f };

    // Get the second vector on the sphere.
    float sqrLength1 = mX1 * mX1 + mY1 * mY1;
    float length1 = std::sqrt(sqrLength1), invLength1 = 0.0f;
    if (length1 > 1.0f)
    {
        // Outside unit disk, project onto it.
        invLength1 = 1.0f / length1;
        mX1 *= invLength1;
        mY1 *= invLength1;
        z1 = 0.0f;
    }
    else
    {
        // Compute point (mX1,mY1,z1) on negative unit hemisphere.
        z1 = 1.0f - sqrLength1;
        z1 = (z1 <= 0.0f ? 0.0f : std::sqrt(z1));
    }
    z1 *= -1.0f;

    // Use camera world coordinates whose order is (D,U,R), so the
    // point is (z,y,x).
    Vector4<float> vec1{ z1, mY1, mX1, 0.0f };

    // Create axis and angle for the rotation.
    Vector4<float> axis = Cross(vec0, vec1);
    float dot = Dot(vec0, vec1);
    float angle;
    if (Normalize(axis) > 0.0f)
    {
        angle = std::acos(std::min(std::max(dot, -1.0f), 1.0f));
    }
    else  // Vectors are parallel.
    {
        if (dot < 0.0f)
        {
            // Rotated pi radians.
            axis[0] = mY0 * invLength0;
            axis[1] = -mX0 * invLength0;
            axis[2] = 0.0f;
            angle = C_PI<float>;
        }
        else
        {
            // Rotation by zero radians.
            MakeUnit(0, axis);
            angle = 0.0f;
        }
    }

    // Compute the rotation matrix implied by trackball motion.  The axis
    // vector was computed in camera coordinates.  It must be converted
    // to world coordinates.  Once again, I use the camera ordering (D,U,R).
    Vector4<float> worldAxis =
        axis[0] * mCamera->GetDVector() +
        axis[1] * mCamera->GetUVector() +
        axis[2] * mCamera->GetRVector();

    Matrix3x3<float> incrRotate = Rotation<float>(
        AxisAngle<float>(HProject(worldAxis), angle));

    // Compute the new rotation, which is the incremental rotation of
    // the trackball appiled after the object has been rotated by its old
    // rotation.  If mRoot has a parent, you have to convert the incremental
    // rotation by a change of basis in the parent's coordinate space.
    auto const* parent = mRoot->GetParent();
    Matrix3x3<float> rotate;
    if (parent)
    {
        Matrix3x3<float> parWRotate = parent->worldTransform.GetRotation();
        Matrix3x3<float> trnParWRotate = Transpose(parWRotate);
        rotate = trnParWRotate * (incrRotate * (parWRotate * mInitialOrientation));
    }
    else
    {
        rotate = incrRotate * mInitialOrientation;
    }

    NormalizeAndUpdateRoot(rotate);
}
