// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Graphics/SceneGraph/Hierarchy/ViewVolume.h>
#include <GTL/Graphics/SceneGraph/Hierarchy/Node.h>
#include <functional>

namespace gtl
{
    class ViewVolumeNode : public Node
    {
    public:
        // Construction and destruction.  The node's world translation is used
        // as the view volume's location.  The node's world rotation matrix is
        // used for the view volume's coordinate axes. Column 0 of the world
        // rotation matrix is the view volume's direction vector, column 1 of
        // the world rotation matrix is the view volume's up vector, and
        // column 2 of the world rotation matrix is the view volume's right
        // vector.
        //
        // On construction, the node's local transformation is set to the view
        // volume's coordinate system.
        ViewVolumeNode(std::shared_ptr<ViewVolume> const& viewVolume = nullptr);

        // When you set the view volume, the node's local transformation is
        // set to the view volumes's current current coordinate system.  The
        // node's world transformation is computed, and the view volume's
        // coordinate system is set to use the node's world transformation.
        void SetViewVolume(std::shared_ptr<ViewVolume> const& viewVolume);

        inline std::shared_ptr<ViewVolume> const& GetViewVolume() const
        {
            return mViewVolume;
        }

        // Additional semantics may be applied after UpdateWorldData updates
        // the view volume.
        inline void SetOnUpdate(std::function<void(ViewVolumeNode*)> const& onUpdate)
        {
            mOnUpdate = onUpdate;
        }

        inline std::function<void(ViewVolumeNode*)> const& GetOnUpdate() const
        {
            return mOnUpdate;
        }

    protected:
        // Geometric updates.
        virtual void UpdateWorldData(double applicationTime) override;

        std::shared_ptr<ViewVolume> mViewVolume;
        std::function<void(ViewVolumeNode*)> mOnUpdate;
    };
}
