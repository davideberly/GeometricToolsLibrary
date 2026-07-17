// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

#include <GTL/Mathematics/Geometry/3D/AlignedBoxBV.h>
#include <GTL/Mathematics/Geometry/3D/BVTreeOfPoints.h>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class AlignedBoxTreeOfPoints : public BVTreeOfPoints<T, AlignedBoxBV<T>>
    {
    public:
        AlignedBoxTreeOfPoints()
            :
            BVTreeOfPoints<T, AlignedBoxBV<T>>{}
        {
        }

    protected:
        // The bounding volume for the primitives' vertices depends on the
        // type of primitive. A derived class representing a primitive tree
        // must implement this.
        virtual void ComputeInteriorBoundingVolume(std::size_t i0, std::size_t i1,
            AlignedBoxBV<T>& boundingVolume) override
        {
            auto& box = boundingVolume.box;
            box.min = this->mVertices[this->mPartition[i0]];
            box.max = box.min;

            for (std::size_t i = i0; i <= i1; ++i)
            {
                Vector3<T> const& vertex = this->mVertices[this->mPartition[i]];
                for (std::size_t k = 0; k < 3; ++k)
                {
                    if (vertex[k] < box.min[k])
                    {
                        box.min[k] = vertex[k];
                    }
                    else if (vertex[k] > box.max[k])
                    {
                        box.max[k] = vertex[k];
                    }
                }
            }
        }

        // The bounding volume for a single primitive's vertices depends on
        // the type of primitive. A derived class representing a primitive
        // tree must implement this.
        virtual void ComputeLeafBoundingVolume(std::size_t i,
            AlignedBoxBV<T>& boundingVolume) override
        {
            auto& box = boundingVolume.box;
            box.min = this->mVertices[this->mPartition[i]];
            box.max = box.min;
        }

    private:
        friend class UnitTestAlignedBoxTreeOfPoints;
    };
}
