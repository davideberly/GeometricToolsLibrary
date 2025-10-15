// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.15

#pragma once

// Create a mesh (x(u,v),y(u,v),z(u,v)) defined by the specified medial curve
// and radial function. The mesh has torus topology when 'closed' is true and
// has cylinder topology when 'closed' is false. The client is responsible
// for setting the topology correctly in the 'description' input. The rows
// correspond to medial samples and the columns correspond to radial samples.
// The medial curve is sampled according to its natural t-parameter when
// 'sampleByArcLength' is false; otherwise, it is sampled uniformly in
// arclength. TODO: Allow TORUS and remove the 'closed' input.

#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Mathematics/Meshes/Mesh.h>
#include <GTL/Mathematics/Curves/FrenetFrame.h>
#include <array>
#include <cmath>
#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

namespace gtl
{
    template <typename T>
    class TubeMesh : public Mesh<T>
    {
    public:
        TubeMesh(Mesh<T>::Description const& description,
            std::shared_ptr<ParametricCurve<T, 3>> const& medial,
            std::function<T(T)> const& radial, bool closed,
            bool sampleByArcLength, Vector3<T> upVector)
            :
            Mesh<T>(description),
            mMedial(medial),
            mRadial(radial),
            mClosed(closed),
            mSampleByArcLength(sampleByArcLength),
            mUpVector(upVector),
            mCosAngle{},
            mSinAngle{},
            mTSampler{},
            mFSampler{},
            mDefaultTCoords{}
        {
            GTL_ARGUMENT_ASSERT(
                description.topology == Mesh<T>::Topology::CYLINDER,
                "The topology must be that of a cylinder."
            );

            GTL_ARGUMENT_ASSERT(
                mMedial != nullptr,
                "A nonnull medial curve is required.");

            mCosAngle.resize(this->mDescription.numCols);
            mSinAngle.resize(this->mDescription.numCols);
            T invRadialSamples = C_<T>(1) / static_cast<T>(this->mDescription.numCols - 1);
            for (std::uint32_t i = 0; i < this->mDescription.numCols - 1; ++i)
            {
                T angle = static_cast<T>(i) * invRadialSamples * C_TWO_PI<T>;
                mCosAngle[i] = std::cos(angle);
                mSinAngle[i] = std::sin(angle);
            }
            mCosAngle[this->mDescription.numCols - 1] = mCosAngle[0];
            mSinAngle[this->mDescription.numCols - 1] = mSinAngle[0];

            T invDenom{};
            if (mClosed)
            {
                invDenom = C_<T>(1) / static_cast<T>(this->mDescription.numRows);
            }
            else
            {
                invDenom = C_<T>(1) / static_cast<T>(this->mDescription.numRows - 1);
            }

            T factor{};
            if (mSampleByArcLength)
            {
                factor = mMedial->GetTotalLength() * invDenom;
                mTSampler = [this, factor](std::uint32_t row)
                {
                    return mMedial->GetTime(static_cast<T>(row) * factor);
                };
            }
            else
            {
                factor = (mMedial->GetTMax() - mMedial->GetTMin()) * invDenom;
                mTSampler = [this, factor](std::uint32_t row)
                {
                    return mMedial->GetTMin() + static_cast<T>(row) * factor;
                };
            }

            if (mUpVector != Vector3<T>::Zero())
            {
                mFSampler = [this](T t)
                {
                    std::array<Vector3<T>, 4> frame{};
                    frame[0] = mMedial->GetPosition(t);
                    frame[1] = mMedial->GetTangent(t);
                    frame[3] = UnitCross(frame[1], mUpVector);
                    frame[2] = UnitCross(frame[3], frame[1]);
                    return frame;
                };
            }
            else
            {
                mFSampler = [this](T t)
                {
                    std::array<Vector3<T>, 4> frame{};
                    FrenetFrame3<T>::GetFrame(*mMedial, t, frame[0], frame[1], frame[2], frame[3]);
                    return frame;
                };
            }

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
            UpdatePositions();
            if (this->mDescription.allowUpdateFrame)
            {
                this->UpdateFrame();
            }
            else if (this->mNormals)
            {
                this->UpdateNormals();
            }
        }

        // Member access.
        inline std::shared_ptr<ParametricCurve<T, 3>> const& GetMedial() const
        {
            return mMedial;
        }

        inline std::function<T(T)> const& GetRadial() const
        {
            return mRadial;
        }

        inline bool IsClosed() const
        {
            return mClosed;
        }

        inline bool IsSampleByArcLength() const
        {
            return mSampleByArcLength;
        }

        inline Vector3<T> const& GetUpVector() const
        {
            return mUpVector;
        }

    private:
        void InitializeTCoords()
        {
            Vector2<T>tcoord{};
            for (std::uint32_t r = 0, i = 0; r < this->mDescription.numRows; ++r)
            {
                tcoord[1] = static_cast<T>(r) / static_cast<T>(this->mDescription.rMax);
                for (std::uint32_t c = 0; c <= this->mDescription.numCols; ++c, ++i)
                {
                    tcoord[0] = static_cast<T>(c) / static_cast<T>(this->mDescription.numCols);
                    this->TCoord(i) = tcoord;
                }
            }
        }

        virtual void UpdatePositions() override
        {
            std::uint32_t row{}, col{}, v{}, save{};
            for (row = 0, v = 0; row < this->mDescription.numRows; ++row, ++v)
            {
                T t = mTSampler(row);
                T radius = mRadial(t);
                // frame = (position, tangent, normal, binormal)
                std::array<Vector3<T>, 4> frame = mFSampler(t);
                for (col = 0, save = v; col < this->mDescription.numCols; ++col, ++v)
                {
                    this->Position(v) = frame[0] + radius * (mCosAngle[col] * frame[2] +
                        mSinAngle[col] * frame[3]);
                }
                this->Position(v) = this->Position(save);
            }

            if (mClosed)
            {
                for (col = 0; col < this->mDescription.numCols; ++col)
                {
                    std::uint32_t i0 = col;
                    std::uint32_t i1 = col + this->mDescription.numCols * (this->mDescription.numRows - 1);
                    this->Position(i1) = this->Position(i0);
                }
            }
        }

        std::shared_ptr<ParametricCurve<T, 3>> mMedial;
        std::function<T(T)> mRadial;
        bool mClosed, mSampleByArcLength;
        Vector3<T> mUpVector;
        std::vector<T> mCosAngle, mSinAngle;
        std::function<T(std::uint32_t)> mTSampler;
        std::function<std::array<Vector3<T>, 4>(T)> mFSampler;

        // If the client does not request texture coordinates, they will be
        // computed internally for use in evaluation of the surface geometry.
        std::vector<Vector2<T>> mDefaultTCoords;
    };
}
