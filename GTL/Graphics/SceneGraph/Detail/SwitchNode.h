// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/SceneGraph/Hierarchy/Node.h>

namespace gtl
{
    class SwitchNode : public Node
    {
    public:
        SwitchNode();
        virtual ~SwitchNode() = default;

        static int32_t constexpr invalidChild = -1;

        void SetActiveChild(int32_t activeChild);

        inline int32_t GetActiveChild() const
        {
            return mActiveChild;
        }

        inline void DisableAllChildren()
        {
            mActiveChild = invalidChild;
        }

    protected:
        // Support for hierarchical culling.
        virtual void GetVisibleSet(Culler& culler,
            std::shared_ptr<Camera> const& camera, bool noCull) override;

        int32_t mActiveChild;
    };
}
