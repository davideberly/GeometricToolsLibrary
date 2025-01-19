// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Graphics/SceneGraph/Hierarchy/Node.h>

namespace gtl
{
    class BillboardNode : public Node
    {
    public:
        // The model space of the billboard has an up vector of (0,1,0) that
        // is chosen to be the billboard's axis of rotation.

        // Construction.
        BillboardNode(std::shared_ptr<Camera> const& camera);

        // The camera to which the billboard is aligned.
        inline void AlignTo(std::shared_ptr<Camera> const& camera)
        {
            mCamera = camera;
        }

    protected:
        // Support for the geometric update.
        virtual void UpdateWorldData(double applicationTime) override;

        std::shared_ptr<Camera> mCamera;
    };
}
