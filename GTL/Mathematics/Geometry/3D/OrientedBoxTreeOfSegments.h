// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

#include <GTL/Mathematics/Containment/3D/ContOrientedBox3.h>
#include <GTL/Mathematics/Geometry/3D/OrientedBoxBV.h>
#include <GTL/Mathematics/Geometry/3D/BVTreeOfSegments.h>
#include <cmath>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class OrientedBoxTreeOfSegments : public BVTreeOfSegments<T, OrientedBoxBV<T>>
    {
    public:
        OrientedBoxTreeOfSegments()
            :
            BVTreeOfSegments<T, OrientedBoxBV<T>>{}
        {
        }

    protected:
        // The bounding volume for the primitives' vertices depends on the
        // type of primitive. A derived class representing a primitive tree
        // must implement this.
        virtual void ComputeInteriorBoundingVolume(std::size_t i0, std::size_t i1,
            OrientedBoxBV<T>& boundingVolume) override
        {
            auto& box = boundingVolume.box;

            std::vector<Vector3<T>> localVertices{};
            localVertices.reserve(2 * (i1 - i0 + 1));
            for (std::size_t i = i0; i <= i1; ++i)
            {
                auto const& seg = this->mSegments[this->mPartition[i]];
                localVertices.push_back(this->mVertices[seg[0]]);
                localVertices.push_back(this->mVertices[seg[1]]);
            }
            ContOrientedBox3<T>::GetContainer(localVertices, box);
        }

        // The bounding volume for a single primitive's vertices depends on
        // the type of primitive. A derived class representing a primitive
        // tree must implement this.
        virtual void ComputeLeafBoundingVolume(std::size_t i,
            OrientedBoxBV<T>& boundingVolume) override
        {
            auto& box = boundingVolume.box;

            auto const& seg = this->mSegments[this->mPartition[i]];
            std::vector<Vector3<T>> localVertices =
            {
                this->mVertices[seg[0]],
                this->mVertices[seg[1]]
            };
            ContOrientedBox3<T>::GetContainer(localVertices, box);

            // Numerical rounding errors in the Gaussian fit of GetContainer
            // will lead to 2 extents nearly zero. Locate them and set them
            // to zero.
            T maxAbsExtent = std::fabs(box.extent[0]);
            std::size_t maxIndex = 0;
            T absExtent = std::fabs(box.extent[1]);
            if (absExtent > maxAbsExtent)
            {
                maxAbsExtent = absExtent;
                maxIndex = 1;
            }
            absExtent = std::fabs(box.extent[2]);
            if (absExtent > maxAbsExtent)
            {
                maxAbsExtent = absExtent;
                maxIndex = 2;
            }

            box.extent[(maxIndex + 1) % 3] = C_<T>(0);
            box.extent[(maxIndex + 2) % 3] = C_<T>(0);
        }

    private:
        friend class UnitTestOrientedBoxTreeOfSegments;
    };
}
