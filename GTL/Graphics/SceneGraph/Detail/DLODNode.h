// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Graphics/SceneGraph/Detail/SwitchNode.h>
#include <vector>

namespace gtl
{
    class DLODNode : public SwitchNode
    {
    public:
        DLODNode(int32_t numLevelsOfDetail);
        virtual ~DLODNode() = default;

        // Access to the center for level of detail (LOD).
        inline void SetModelLODCenter(Vector4<float> const& modelCenter)
        {
            mModelLODCenter = modelCenter;
        }

        inline Vector4<float> const& GetModelLODCenter() const
        {
            return mModelLODCenter;
        }

        inline void SetWorldLODCenter(Vector4<float> const& worldCenter)
        {
            mWorldLODCenter = worldCenter;
        }

        inline Vector4<float> const& GetWorldLODCenter() const
        {
            return mWorldLODCenter;
        }

        // Access to the distance intervals for children.
        inline int32_t GetNumLevelsOfDetail() const
        {
            return mNumLevelsOfDetail;
        }

        float GetModelMinDistance(int32_t i) const;
        float GetModelMaxDistance(int32_t i) const;
        float GetWorldMinDistance(int32_t i) const;
        float GetWorldMaxDistance(int32_t i) const;
        void SetModelDistance(int32_t i, float minDistance, float maxDistance);

    protected:
        // Switch the child based on distance from world LOD center to camera.
        void SelectLevelOfDetail(std::shared_ptr<Camera> const& camera);

        // Support for hierarchical culling.
        virtual void GetVisibleSet(Culler& culler,
            std::shared_ptr<Camera> const& camera, bool noCull) override;

        // The point whose distance to the camera determines the correct
        // child to activate.
        Vector4<float> mModelLODCenter;
        Vector4<float> mWorldLODCenter;

        // Squared distances for each LOD interval. The number of levels of
        // detail is the same as the number of children of the node.
        int32_t mNumLevelsOfDetail;
        std::vector<float> mModelMinDistance;
        std::vector<float> mModelMaxDistance;
        std::vector<float> mWorldMinDistance;
        std::vector<float> mWorldMaxDistance;
    };
}
