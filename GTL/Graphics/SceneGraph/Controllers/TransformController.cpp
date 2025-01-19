// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/SceneGraph/Controllers/TransformController.h>
#include <GTL/Graphics/SceneGraph/Hierarchy/Spatial.h>
using namespace gtl;

TransformController::TransformController(AffineTransform<float> const& localTransform)
    :
    mLocalTransform(localTransform)
{
}

bool TransformController::Update(double applicationTime)
{
    if (!Controller::Update(applicationTime))
    {
        return false;
    }

    Spatial* spatial = reinterpret_cast<Spatial*>(mObject);
    spatial->localTransform = mLocalTransform;
    return true;
}
