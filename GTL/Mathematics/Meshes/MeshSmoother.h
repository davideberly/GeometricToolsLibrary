// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.15

#pragma once

#include <GTL/Mathematics/Algebra/Vector.h>
#include <algorithm>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class MeshSmoother
    {
    public:
        MeshSmoother()
            :
            mNumVertices(0),
            mVertices(nullptr),
            mNumTriangles(0),
            mIndices(nullptr),
            mNormals{},
            mMeans{},
            mNeighborCounts{}
        {
        }

        virtual ~MeshSmoother() = default;

        // The input to operator() is a triangle mesh with the specified
        // vertex buffer and index buffer. The number of elements of
        // 'indices' must be a multiple of 3, each triple of indices
        // (3*t, 3*t+1, 3*t+2) representing the triangle with vertices
        // (vertices[3*t], vertices[3*t+1], vertices[3*t+2]).
        void operator()(std::size_t numVertices, Vector3<T>* vertices,
            std::size_t numTriangles, std::size_t const* indices)
        {
            GTL_ARGUMENT_ASSERT(
                numVertices >= 3 && vertices != nullptr &&
                numTriangles >= 1 && indices != nullptr,
                "Invalid input.");

            mNumVertices = numVertices;
            mVertices = vertices;
            mNumTriangles = numTriangles;
            mIndices = indices;

            mNormals.resize(mNumVertices);
            mMeans.resize(mNumVertices);
            mNeighborCounts.resize(mNumVertices);

            // Count the number of vertex neighbors.
            std::fill(mNeighborCounts.begin(), mNeighborCounts.end(), 0);
            std::size_t const* current = mIndices;
            for (std::size_t i = 0; i < mNumTriangles; ++i)
            {
                mNeighborCounts[*current++] += 2;
                mNeighborCounts[*current++] += 2;
                mNeighborCounts[*current++] += 2;
            }
        }

        void operator()(std::vector<Vector3<T>>& vertices, std::vector<std::size_t> const& indices)
        {
            GTL_ARGUMENT_ASSERT(
                vertices.size() >= 3 && indices.size() >= 3 && (indices.size() % 3) == 0,
                "Invalid input.");

            operator()(vertices.size(), vertices.data(), indices.size() / 3, indices.data());
        }

        inline std::size_t GetNumVertices() const
        {
            return mNumVertices;
        }

        inline Vector3<T> const* GetVertices() const
        {
            return mVertices;
        }

        inline std::size_t GetNumTriangles() const
        {
            return mNumTriangles;
        }

        inline std::size_t const* GetIndices() const
        {
            return mIndices;
        }

        inline std::vector<Vector3<T>> const& GetNormals() const
        {
            return mNormals;
        }

        inline std::vector<Vector3<T>> const& GetMeans() const
        {
            return mMeans;
        }

        inline std::vector<std::size_t> const& GetNeighborCounts() const
        {
            return mNeighborCounts;
        }

        // Apply one iteration of the smoother. The input time is supported
        // for applications where the surface evolution is time-dependent.
        void Update(T t = C_<T>(0))
        {
            Vector3<T> vzero{};
            std::fill(mNormals.begin(), mNormals.end(), vzero);
            std::fill(mMeans.begin(), mMeans.end(), vzero);

            std::size_t const* current = mIndices;
            for (std::size_t i = 0; i < mNumTriangles; ++i)
            {
                std::size_t v0 = *current++;
                std::size_t v1 = *current++;
                std::size_t v2 = *current++;

                Vector3<T>& V0 = mVertices[v0];
                Vector3<T>& V1 = mVertices[v1];
                Vector3<T>& V2 = mVertices[v2];

                Vector3<T> edge1 = V1 - V0;
                Vector3<T> edge2 = V2 - V0;
                Vector3<T> normal = Cross(edge1, edge2);

                mNormals[v0] += normal;
                mNormals[v1] += normal;
                mNormals[v2] += normal;

                mMeans[v0] += V1 + V2;
                mMeans[v1] += V2 + V0;
                mMeans[v2] += V0 + V1;
            }

            for (std::size_t i = 0; i < mNumVertices; ++i)
            {
                Normalize(mNormals[i]);
                mMeans[i] /= static_cast<T>(mNeighborCounts[i]);
            }

            for (std::size_t i = 0; i < mNumVertices; ++i)
            {
                if (VertexInfluenced(i, t))
                {
                    Vector3<T> diff = mMeans[i] - mVertices[i];
                    T dotDifNor = Dot(diff, mNormals[i]);
                    Vector3<T> surfaceNormal = dotDifNor * mNormals[i];
                    Vector3<T> tangent = diff - surfaceNormal;

                    T tanWeight = GetTangentWeight(i, t);
                    T norWeight = GetNormalWeight(i, t);
                    mVertices[i] += tanWeight * tangent + norWeight * mNormals[i];
                }
            }
        }

    protected:
        // The input parameters are "std::size_t i, T t". They are unused by
        // default, so the names are hidden according to ANSI standards.
        virtual bool VertexInfluenced(std::size_t, T)
        {
            return true;
        }

        virtual T GetTangentWeight(std::size_t, T)
        {
            return C_<T>(1, 2);
        }

        virtual T GetNormalWeight(std::size_t, T)
        {
            return C_<T>(0);
        }

        std::size_t mNumVertices;
        Vector3<T>* mVertices;
        std::size_t mNumTriangles;
        std::size_t const* mIndices;

        std::vector<Vector3<T>> mNormals;
        std::vector<Vector3<T>> mMeans;
        std::vector<std::size_t> mNeighborCounts;

    private:
        friend class UnitTestMeshSmoother;
    };
}
