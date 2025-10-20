// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.15

#pragma once

#include <GTL/Mathematics/Meshes/Mesh.h>
#include <GTL/Mathematics/Surfaces/ParametricSurface.h>
#include <array>
#include <cstdint>
#include <memory>
#include <vector>

namespace gtl
{
    template <typename T>
    class RectanglePatchMesh : public Mesh<T>
    {
    public:
        // Create a mesh (x(u,v),y(u,v),z(u,v)) defined by the specified
        // surface. It is required that surface->IsRectangular() return
        // 'true'.
        RectanglePatchMesh(Mesh<T>::Description const& description,
            std::shared_ptr<ParametricSurface<T, 3>> const& surface)
            :
            Mesh<T>(description),
            mSurface(surface),
            mDefaultTCoords{}
        {
            GTL_ARGUMENT_ASSERT(
                description.topology == Mesh<T>::Topology::RECTANGLE,
                "The topology must be that of a rectangle."
            );

            GTL_ARGUMENT_ASSERT(
                mSurface != nullptr && mSurface->IsRectangular(),
                "A nonnull rectanglar surface is required.");

            if (!this->mTCoords)
            {
                mDefaultTCoords.resize(this->mDescription.numVertices);
                this->mTCoords = mDefaultTCoords.data();
                this->mTCoordStride = sizeof(Vector2<T>);

                this->mDescription.allowUpdateFrame = this->mDescription.wantDynamicTangentSpaceUpdate;
                if (this->mDescription.allowUpdateFrame)
                {
                    if (!this->mDescription.hasTangentSpaceVectors)
                    {
                        this->mDescription.allowUpdateFrame = false;
                    }

                    if (!this->mNormals)
                    {
                        this->mDescription.allowUpdateFrame = false;
                    }
                }
            }

            this->ComputeIndices();
            InitializeTCoords();
            InitializePositions();
            if (this->mDescription.allowUpdateFrame)
            {
                InitializeFrame();
            }
            else if (this->mNormals)
            {
                InitializeNormals();
            }
        }

        // Member access.
        inline std::shared_ptr<ParametricSurface<T, 3>> const& GetSurface() const
        {
            return mSurface;
        }

    protected:
        void InitializeTCoords()
        {
            T uMin = mSurface->GetUMin();
            T uDelta = (mSurface->GetUMax() - uMin) / static_cast<T>(this->mDescription.numCols - 1);
            T vMin = mSurface->GetVMin();
            T vDelta = (mSurface->GetVMax() - vMin) / static_cast<T>(this->mDescription.numRows - 1);
            Vector2<T> tcoord{};
            for (std::uint32_t r = 0, i = 0; r < this->mDescription.numRows; ++r)
            {
                tcoord[1] = vMin + vDelta * static_cast<T>(r);
                for (std::uint32_t c = 0; c < this->mDescription.numCols; ++c, ++i)
                {
                    tcoord[0] = uMin + uDelta * static_cast<T>(c);
                    this->TCoord(i) = tcoord;
                }
            }
        }

        void InitializePositions()
        {
            for (std::uint32_t r = 0, i = 0; r < this->mDescription.numRows; ++r)
            {
                for (std::uint32_t c = 0; c < this->mDescription.numCols; ++c, ++i)
                {
                    Vector2<T> tcoord = this->TCoord(i);
                    this->Position(i) = mSurface->GetPosition(tcoord[0], tcoord[1]);
                }
            }
        }

        void InitializeNormals()
        {
            for (std::uint32_t r = 0, i = 0; r < this->mDescription.numRows; ++r)
            {
                for (std::uint32_t c = 0; c < this->mDescription.numCols; ++c, ++i)
                {
                    Vector2<T> tcoord = this->TCoord(i);
                    Vector3<T> values[6];
                    mSurface->Evaluate(tcoord[0], tcoord[1], 1, values);
                    Normalize(values[1]);
                    Normalize(values[2]);
                    this->Normal(i) = UnitCross(values[1], values[2]);
                }
            }
        }

        void InitializeFrame()
        {
            for (std::uint32_t r = 0, i = 0; r < this->mDescription.numRows; ++r)
            {
                for (std::uint32_t c = 0; c < this->mDescription.numCols; ++c, ++i)
                {
                    Vector2<T> tcoord = this->TCoord(i);
                    std::array<Vector3<T>, 6> values{};
                    mSurface->Evaluate(tcoord[0], tcoord[1], 1, values.data());
                    Normalize(values[1]);
                    Normalize(values[2]);

                    if (this->mDPDUs)
                    {
                        this->DPDU(i) = values[1];
                    }
                    if (this->mDPDVs)
                    {
                        this->DPDV(i) = values[2];
                    }

                    ComputeOrthonormalBasis(2, values[1], values[2], values[3]);

                    if (this->mNormals)
                    {
                        this->Normal(i) = values[3];
                    }
                    if (this->mTangents)
                    {
                        this->Tangent(i) = values[1];
                    }
                    if (this->mBitangents)
                    {
                        this->Bitangent(i) = values[2];
                    }
                }
            }
        }

        virtual void UpdatePositions() override
        {
            InitializePositions();
        }

        virtual void UpdateNormals() override
        {
            InitializeNormals();
        }

        virtual void UpdateFrame() override
        {
            InitializeFrame();
        }

        std::shared_ptr<ParametricSurface<T, 3>> mSurface;

        // If the client does not request texture coordinates, they will be
        // computed internally for use in evaluation of the surface geometry.
        std::vector<Vector2<T>> mDefaultTCoords;
    };
}
