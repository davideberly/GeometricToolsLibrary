// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsPCH.h>
#include <GTL/Mathematics/Algebra/Rotation.h>
#include <GTL/Graphics/SceneGraph/Controllers/BlendTransformController.h>
#include <GTL/Graphics/SceneGraph/Hierarchy/Spatial.h>
using namespace gtl;

BlendTransformController::BlendTransformController(
    std::shared_ptr<TransformController> const& controller0,
    std::shared_ptr<TransformController> const& controller1,
    bool geometricRotation, bool geometricScale)
    :
    TransformController(AffineTransform<float>()),  // the identity transform
    mController0(controller0),
    mController1(controller1),
    mWeight(0.0f),
    mGeometricRotation(geometricRotation),
    mGeometricScale(geometricScale)
{
}

bool BlendTransformController::Update(double applicationTime)
{
    if (!Controller::Update(applicationTime))
    {
        return false;
    }

    mController0->Update(applicationTime);
    mController1->Update(applicationTime);

    AffineTransform<float> const& xfrm0 = mController0->GetTransform();
    AffineTransform<float> const& xfrm1 = mController1->GetTransform();
    float oneMinusWeight = 1.0f - mWeight;

    // Compute the blended translation.
    Vector3<float> trn0 = xfrm0.GetTranslation();
    Vector3<float> trn1 = xfrm1.GetTranslation();
    Vector3<float> blendTrn = oneMinusWeight * trn0 + mWeight * trn1;
    mLocalTransform.SetTranslation(blendTrn);

    // Compute the blended rotation.
    Matrix3x3<float> rot0 = xfrm0.GetRotation();
    Matrix3x3<float> rot1 = xfrm1.GetRotation();

    Quaternion<float> quat0 = Rotation<float>(rot0);
    Quaternion<float> quat1 = Rotation<float>(rot1);
    if (Dot(quat0, quat1) < 0.0f)
    {
        quat1 = -quat1;
    }

    Quaternion<float> blendQuat;
    if (mGeometricRotation)
    {
        blendQuat = Slerp(mWeight, quat0, quat1);
    }
    else
    {
        blendQuat = oneMinusWeight * quat0 + mWeight * quat1;
        Normalize(blendQuat);
    }

    Matrix3x3<float> blendRot = Rotation<float>(blendQuat);
    mLocalTransform.SetRotation(blendRot);

    // Compute the blended scale.
    Vector3<float> sca0 = xfrm0.GetScale();
    Vector3<float> sca1 = xfrm1.GetScale();
    Vector3<float> blendSca{};
    if (mGeometricScale)
    {
        for (int32_t i = 0; i < 3; ++i)
        {
            float s0 = sca0[i], s1 = sca1[i];
            if (s0 != 0.0f && s1 != 0.0f)
            {
                float sign0 = (s0 > 0.0f ? 1.0f : -1.0f);
                float sign1 = (s1 > 0.0f ? 1.0f : -1.0f);
                s0 = std::fabs(s0);
                s1 = std::fabs(s1);
                float pow0 = std::pow(s0, oneMinusWeight);
                float pow1 = std::pow(s1, mWeight);
                blendSca[i] = sign0 * sign1 * pow0 * pow1;
            }
            else
            {
                blendSca[i] = 0.0f;
            }
        }
    }
    else
    {
        blendSca = oneMinusWeight * sca0 + mWeight * sca1;
    }
    mLocalTransform.SetScale(blendSca);

    auto spatial = static_cast<Spatial*>(mObject);
    spatial->localTransform = mLocalTransform;
    return true;
}

void BlendTransformController::SetObject(ControlledObject* object)
{
    TransformController::SetObject(object);
    mController0->SetObject(object);
    mController1->SetObject(object);
}
