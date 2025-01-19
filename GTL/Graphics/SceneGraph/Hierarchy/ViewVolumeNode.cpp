// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/SceneGraph/Hierarchy/ViewVolumeNode.h>
using namespace gtl;

ViewVolumeNode::ViewVolumeNode(std::shared_ptr<ViewVolume> const& viewVolume)
    :
    mOnUpdate([](ViewVolumeNode*){})
{
    SetViewVolume(viewVolume);
}

void ViewVolumeNode::SetViewVolume(std::shared_ptr<ViewVolume> const& viewVolume)
{
    mViewVolume = viewVolume;

    if (mViewVolume)
    {
        Matrix4x4<float> rotate;
        rotate.SetCol(0, mViewVolume->GetDVector());
        rotate.SetCol(1, mViewVolume->GetUVector());
        rotate.SetCol(2, mViewVolume->GetRVector());
        rotate.SetCol(3, { 0.0f, 0.0f, 0.0f, 1.0f });
        localTransform.SetTranslation(HProject(mViewVolume->GetPosition()));
        localTransform.SetRotation(HProject(rotate));
        Update();
    }
}

void ViewVolumeNode::UpdateWorldData(double applicationTime)
{
    Node::UpdateWorldData(applicationTime);

    if (mViewVolume)
    {
        Vector4<float> position = HLift(worldTransform.GetTranslation(), 1.0f);

        Matrix4x4<float> const& rotate = worldTransform.GetH();
        Vector4<float> dVector = rotate.GetCol(0);
        Vector4<float> uVector = rotate.GetCol(1);
        Vector4<float> rVector = rotate.GetCol(2);
        mViewVolume->SetFrame(position, dVector, uVector, rVector);
        mOnUpdate(this);
    }
}
