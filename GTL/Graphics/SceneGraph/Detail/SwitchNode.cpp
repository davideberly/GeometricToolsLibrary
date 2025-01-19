// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/SceneGraph/Detail/SwitchNode.h>
using namespace gtl;

SwitchNode::SwitchNode()
    :
    mActiveChild(invalidChild)
{
}

void SwitchNode::SetActiveChild(int32_t activeChild)
{
    GTL_ARGUMENT_ASSERT(
        activeChild == invalidChild || (0 <= activeChild && activeChild < GetNumChildren()),
        "Invalid active child specified.");

    mActiveChild = activeChild;
}

void SwitchNode::GetVisibleSet(Culler& culler,
    std::shared_ptr<Camera> const& camera, bool noCull)
{
    if (mActiveChild > invalidChild)
    {
        // All Visual objects in the active subtree are added to the
        // visible set.
        auto const& child = mChild[mActiveChild];
        if (child)
        {
            child->OnGetVisibleSet(culler, camera, noCull);
        }
    }
}
