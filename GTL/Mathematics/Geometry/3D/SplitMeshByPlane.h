// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

// The algorithm for splitting a mesh by a plane is described in
// https://www.geometrictools.com/Documentation/ClipMesh.pdf
// Currently, the code here does not include generating a closed
// mesh (from the "positive" and "zero" vertices) by attaching
// triangulated faces to the mesh, where the those faces live in
// the splitting plane. (TODO: Add this code.)

#include <GTL/Mathematics/Distance/ND/DistPointHyperplane.h>
#include <GTL/Mathematics/Meshes/EdgeKey.h>
#include <cstddef>
#include <map>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T>
    class SplitMeshByPlane
    {
    public:
        SplitMeshByPlane()
            :
            mSignedDistances{},
            mEMap{}
        {
        }

        // The 'indices' are lookups into the 'vertices' array. The indices
        // represent a triangle mesh. The number of indices must be a
        // multiple of 3, each triple representing a triangle. If t is a
        // triangle index, then the triangle is formed by
        // vertices[indices[3 * t + i]] for 0 <= i <= 2. The outputs
        // 'negIndices' and 'posIndices' are formatted similarly.
        void operator()(
            std::vector<Vector3<T>> const& vertices,
            std::vector<std::size_t> const& indices,
            Plane3<T> const& plane,
            std::vector<Vector3<T>>& clipVertices,
            std::vector<std::size_t>& negIndices,
            std::vector<std::size_t>& posIndices)
        {
            mSignedDistances.resize(vertices.size());
            mEMap.clear();

            // Make a copy of the incoming vertices. If the mesh intersects
            // the plane, new vertices must be generated. These are appended
            // to the clipVertices array.
            clipVertices = vertices;

            ClassifyVertices(clipVertices, plane);
            ClassifyEdges(clipVertices, indices);
            ClassifyTriangles(indices, negIndices, posIndices);
        }

    private:
        void ClassifyVertices(std::vector<Vector3<T>> const& clipVertices,
            Plane3<T> const& plane)
        {
            DCPQuery<T, Vector3<T>, Plane3<T>> query{};
            for (std::size_t i = 0; i < clipVertices.size(); ++i)
            {
                mSignedDistances[i] = query(clipVertices[i], plane).signedDistance;
            }
        }

        void ClassifyEdges(std::vector<Vector3<T>>& clipVertices,
            std::vector<std::size_t> const& indices)
        {
            std::size_t const numTriangles = indices.size() / 3;
            std::size_t nextIndex = clipVertices.size();
            for (std::size_t i = 0, j = 0; i < numTriangles; ++i)
            {
                std::size_t v0 = indices[j++];
                std::size_t v1 = indices[j++];
                std::size_t v2 = indices[j++];
                T sDist0 = mSignedDistances[v0];
                T sDist1 = mSignedDistances[v1];
                T sDist2 = mSignedDistances[v2];

                EdgeKey<false> key{};
                T t{};
                Vector3<T> intr{}, diff{};

                // The change-in-sign tests are structured this way to avoid
                // numerical round-off problems. For example, sDist0 > 0 and
                // sDist1 < 0, but both are very small and sDist0 * sDist1 = 0
                // because of round-off errors. The tests also guarantee
                // consistency between this function and ClassifyTriangles,
                // the latter function using sign tests only on the individual
                // sDist values.

                if ((sDist0 > C_<T>(0) && sDist1 < C_<T>(0)) ||
                    (sDist0 < C_<T>(0) && sDist1 > C_<T>(0)))
                {
                    key = EdgeKey<false>(v0, v1);
                    if (mEMap.find(key) == mEMap.end())
                    {
                        t = sDist0 / (sDist0 - sDist1);
                        diff = clipVertices[v1] - clipVertices[v0];
                        intr = clipVertices[v0] + t * diff;
                        clipVertices.push_back(intr);
                        mEMap[key] = std::make_pair(intr, nextIndex);
                        ++nextIndex;
                    }
                }

                if ((sDist1 > C_<T>(0) && sDist2 < C_<T>(0)) ||
                    (sDist1 < C_<T>(0) && sDist2 >C_<T>(0)))
                {
                    key = EdgeKey<false>(v1, v2);
                    if (mEMap.find(key) == mEMap.end())
                    {
                        t = sDist1 / (sDist1 - sDist2);
                        diff = clipVertices[v2] - clipVertices[v1];
                        intr = clipVertices[v1] + t * diff;
                        clipVertices.push_back(intr);
                        mEMap[key] = std::make_pair(intr, nextIndex);
                        ++nextIndex;
                    }
                }

                if ((sDist2 > C_<T>(0) && sDist0 < C_<T>(0)) ||
                    (sDist2 < C_<T>(0) && sDist0 >C_<T>(0)))
                {
                    key = EdgeKey<false>(v2, v0);
                    if (mEMap.find(key) == mEMap.end())
                    {
                        t = sDist2 / (sDist2 - sDist0);
                        diff = clipVertices[v0] - clipVertices[v2];
                        intr = clipVertices[v2] + t * diff;
                        clipVertices.push_back(intr);
                        mEMap[key] = std::make_pair(intr, nextIndex);
                        ++nextIndex;
                    }
                }
            }
        }

        void ClassifyTriangles(std::vector<std::size_t> const& indices,
            std::vector<std::size_t>& negIndices, std::vector<std::size_t>& posIndices)
        {
            std::size_t const numTriangles = indices.size() / 3;
            for (std::size_t i = 0, j = 0; i < numTriangles; ++i)
            {
                std::size_t v0 = indices[j++];
                std::size_t v1 = indices[j++];
                std::size_t v2 = indices[j++];
                T sDist0 = mSignedDistances[v0];
                T sDist1 = mSignedDistances[v1];
                T sDist2 = mSignedDistances[v2];

                if (sDist0 > C_<T>(0))
                {
                    if (sDist1 > C_<T>(0))
                    {
                        if (sDist2 > C_<T>(0))
                        {
                            // +++
                            AppendTriangle(posIndices, v0, v1, v2);
                        }
                        else if (sDist2 < C_<T>(0))
                        {
                            // ++-
                            SplitTrianglePPM(negIndices, posIndices, v0, v1, v2);
                        }
                        else
                        {
                            // ++0
                            AppendTriangle(posIndices, v0, v1, v2);
                        }
                    }
                    else if (sDist1 < C_<T>(0))
                    {
                        if (sDist2 > C_<T>(0))
                        {
                            // +-+
                            SplitTrianglePPM(negIndices, posIndices, v2, v0, v1);
                        }
                        else if (sDist2 < C_<T>(0))
                        {
                            // +--
                            SplitTriangleMMP(negIndices, posIndices, v1, v2, v0);
                        }
                        else
                        {
                            // +-0
                            SplitTrianglePMZ(negIndices, posIndices, v0, v1, v2);
                        }
                    }
                    else
                    {
                        if (sDist2 > C_<T>(0))
                        {
                            // +0+
                            AppendTriangle(posIndices, v0, v1, v2);
                        }
                        else if (sDist2 < C_<T>(0))
                        {
                            // +0-
                            SplitTriangleMPZ(negIndices, posIndices, v2, v0, v1);
                        }
                        else
                        {
                            // +00
                            AppendTriangle(posIndices, v0, v1, v2);
                        }
                    }
                }
                else if (sDist0 < C_<T>(0))
                {
                    if (sDist1 > C_<T>(0))
                    {
                        if (sDist2 > C_<T>(0))
                        {
                            // -++
                            SplitTrianglePPM(negIndices, posIndices, v1, v2, v0);
                        }
                        else if (sDist2 < C_<T>(0))
                        {
                            // -+-
                            SplitTriangleMMP(negIndices, posIndices, v2, v0, v1);
                        }
                        else
                        {
                            // -+0
                            SplitTriangleMPZ(negIndices, posIndices, v0, v1, v2);
                        }
                    }
                    else if (sDist1 < C_<T>(0))
                    {
                        if (sDist2 > C_<T>(0))
                        {
                            // --+
                            SplitTriangleMMP(negIndices, posIndices, v0, v1, v2);
                        }
                        else if (sDist2 < C_<T>(0))
                        {
                            // ---
                            AppendTriangle(negIndices, v0, v1, v2);
                        }
                        else
                        {
                            // --0
                            AppendTriangle(negIndices, v0, v1, v2);
                        }
                    }
                    else
                    {
                        if (sDist2 > C_<T>(0))
                        {
                            // -0+
                            SplitTrianglePMZ(negIndices, posIndices, v2, v0, v1);
                        }
                        else if (sDist2 < C_<T>(0))
                        {
                            // -0-
                            AppendTriangle(negIndices, v0, v1, v2);
                        }
                        else
                        {
                            // -00
                            AppendTriangle(negIndices, v0, v1, v2);
                        }
                    }
                }
                else
                {
                    if (sDist1 > C_<T>(0))
                    {
                        if (sDist2 > C_<T>(0))
                        {
                            // 0++
                            AppendTriangle(posIndices, v0, v1, v2);
                        }
                        else if (sDist2 < C_<T>(0))
                        {
                            // 0+-
                            SplitTrianglePMZ(negIndices, posIndices, v1, v2, v0);
                        }
                        else
                        {
                            // 0+0
                            AppendTriangle(posIndices, v0, v1, v2);
                        }
                    }
                    else if (sDist1 < C_<T>(0))
                    {
                        if (sDist2 > C_<T>(0))
                        {
                            // 0-+
                            SplitTriangleMPZ(negIndices, posIndices, v1, v2, v0);
                        }
                        else if (sDist2 < C_<T>(0))
                        {
                            // 0--
                            AppendTriangle(negIndices, v0, v1, v2);
                        }
                        else
                        {
                            // 0-0
                            AppendTriangle(negIndices, v0, v1, v2);
                        }
                    }
                    else
                    {
                        if (sDist2 > C_<T>(0))
                        {
                            // 00+
                            AppendTriangle(posIndices, v0, v1, v2);
                        }
                        else if (sDist2 < C_<T>(0))
                        {
                            // 00-
                            AppendTriangle(negIndices, v0, v1, v2);
                        }
                        else
                        {
                            // 000, reject triangles lying in the plane
                        }
                    }
                }
            }
        }

        void AppendTriangle(std::vector<std::size_t>& indices, std::size_t v0, std::size_t v1, std::size_t v2)
        {
            indices.push_back(v0);
            indices.push_back(v1);
            indices.push_back(v2);
        }

        void SplitTrianglePPM(std::vector<std::size_t>& negIndices,
            std::vector<std::size_t>& posIndices, std::size_t v0, std::size_t v1, std::size_t v2)
        {
            std::size_t v12 = mEMap[EdgeKey<false>(v1, v2)].second;
            std::size_t v20 = mEMap[EdgeKey<false>(v2, v0)].second;
            posIndices.push_back(v0);
            posIndices.push_back(v1);
            posIndices.push_back(v12);
            posIndices.push_back(v0);
            posIndices.push_back(v12);
            posIndices.push_back(v20);
            negIndices.push_back(v2);
            negIndices.push_back(v20);
            negIndices.push_back(v12);
        }

        void SplitTriangleMMP(std::vector<std::size_t>& negIndices,
            std::vector<std::size_t>& posIndices, std::size_t v0, std::size_t v1, std::size_t v2)
        {
            std::size_t v12 = mEMap[EdgeKey<false>(v1, v2)].second;
            std::size_t v20 = mEMap[EdgeKey<false>(v2, v0)].second;
            negIndices.push_back(v0);
            negIndices.push_back(v1);
            negIndices.push_back(v12);
            negIndices.push_back(v0);
            negIndices.push_back(v12);
            negIndices.push_back(v20);
            posIndices.push_back(v2);
            posIndices.push_back(v20);
            posIndices.push_back(v12);
        }

        void SplitTrianglePMZ(std::vector<std::size_t>& negIndices,
            std::vector<std::size_t>& posIndices, std::size_t v0, std::size_t v1, std::size_t v2)
        {
            std::size_t v01 = mEMap[EdgeKey<false>(v0, v1)].second;
            posIndices.push_back(v2);
            posIndices.push_back(v0);
            posIndices.push_back(v01);
            negIndices.push_back(v2);
            negIndices.push_back(v01);
            negIndices.push_back(v1);
        }

        void SplitTriangleMPZ(std::vector<std::size_t>& negIndices,
            std::vector<std::size_t>& posIndices, std::size_t v0, std::size_t v1, std::size_t v2)
        {
            std::size_t v01 = mEMap[EdgeKey<false>(v0, v1)].second;
            negIndices.push_back(v2);
            negIndices.push_back(v0);
            negIndices.push_back(v01);
            posIndices.push_back(v2);
            posIndices.push_back(v01);
            posIndices.push_back(v1);
        }

        // Stores the signed distances from the vertices to the plane.
        std::vector<T> mSignedDistances;

        // Stores the edges whose vertices are on opposite sides of the
        // plane. The key is a pair of indices into the vertex array.
        // The value is the point of intersection of the edge with the
        // plane and an index into m_kVertices (the index is larger or
        // equal to the number of vertices of incoming rkVertices).
        std::map<EdgeKey<false>, std::pair<Vector3<T>, std::size_t>> mEMap;

    private:
        friend class UnitTestSplitMeshByPlane;
    };
}
