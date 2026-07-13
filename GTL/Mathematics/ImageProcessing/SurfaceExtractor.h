// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

#pragma once

// Abstract base class for SurfaceExtractorCubes and
// SurfaceExtractorTetrahedra.

#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Utility/Exceptions.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <map>
#include <type_traits>
#include <utility>
#include <vector>

namespace gtl
{
    // The image type IndexType must be one of the integer types:
    // std::uint16_t, std::uint32_t, std::uint64_t, std::size_t.
    // Internal integer computations are performed using
    // std::int64_t. The type T is for extraction to floating-point
    // vertices.
    template <typename IndexType, typename T>
    class SurfaceExtractor
    {
    public:
        // Abstract base class.
        virtual ~SurfaceExtractor() = default;

        // The level surfaces form a graph of vertices, edges and triangles.
        // The vertices are computed as triples of nonnegative rational
        // numbers. Vertex represents the rational triple
        //   (xNumer/xDenom, yNumer/yDenom, zNumer/zDenom)
        // as
        //   (xNumer, xDenom, yNumer, yDenom, zNumer, zDenom)
        // where all components are nonnegative. The edges connect pairs of
        // vertices and the triangles connect triples of vertices, forming
        // a graph that represents the level set.
        struct Vertex
        {
            Vertex()
                :
                xNumer(0),
                xDenom(0),
                yNumer(0),
                yDenom(0),
                zNumer(0),
                zDenom(0)
            {
            }

            Vertex(std::int64_t inXNumer, std::int64_t inXDenom, std::int64_t inYNumer, std::int64_t inYDenom,
                std::int64_t inZNumer, std::int64_t inZDenom)
                :
                Vertex()
            {
                // The vertex generation leads to the numerator and
                // denominator having the same sign. This constructor changes
                // sign to ensure the numerator and denominator are both
                // positive.
                if (inXDenom > 0)
                {
                    xNumer = inXNumer;
                    xDenom = inXDenom;
                }
                else
                {
                    xNumer = -inXNumer;
                    xDenom = -inXDenom;
                }

                if (inYDenom > 0)
                {
                    yNumer = inYNumer;
                    yDenom = inYDenom;
                }
                else
                {
                    yNumer = -inYNumer;
                    yDenom = -inYDenom;
                }

                if (inZDenom > 0)
                {
                    zNumer = inZNumer;
                    zDenom = inZDenom;
                }
                else
                {
                    zNumer = -inZNumer;
                    zDenom = -inZDenom;
                }
            }

            // The non-default constructor guarantees that xDenom > 0,
            // yDenom > 0 and zDenom > 0. The following comparison operators
            // assume that the denominators are positive.
            bool operator==(Vertex const& other) const
            {
                return
                    // xn0/xd0 == xn1/xd1
                    xNumer * other.xDenom == other.xNumer * xDenom
                    &&
                    // yn0/yd0 == yn1/yd1
                    yNumer * other.yDenom == other.yNumer * yDenom
                    &&
                    // zn0/zd0 == zn1/zd1
                    zNumer * other.zDenom == other.zNumer * zDenom;
            }

            bool operator<(Vertex const& other) const
            {
                std::int64_t xn0txd1 = xNumer * other.xDenom;
                std::int64_t xn1txd0 = other.xNumer * xDenom;
                if (xn0txd1 < xn1txd0)
                {
                    // xn0/xd0 < xn1/xd1
                    return true;
                }
                if (xn0txd1 > xn1txd0)
                {
                    // xn0/xd0 > xn1/xd1
                    return false;
                }

                std::int64_t yn0tyd1 = yNumer * other.yDenom;
                std::int64_t yn1tyd0 = other.yNumer * yDenom;
                if (yn0tyd1 < yn1tyd0)
                {
                    // yn0/yd0 < yn1/yd1
                    return true;
                }
                if (yn0tyd1 > yn1tyd0)
                {
                    // yn0/yd0 > yn1/yd1
                    return false;
                }

                std::int64_t zn0tzd1 = zNumer * other.zDenom;
                std::int64_t zn1tzd0 = other.zNumer * zDenom;
                // zn0/zd0 < zn1/zd1
                return zn0tzd1 < zn1tzd0;
            }

            std::int64_t xNumer, xDenom, yNumer, yDenom, zNumer, zDenom;
        };

        struct Triangle
        {
            Triangle()
                :
                v{ 0, 0, 0 }
            {

            }

            Triangle(std::size_t v0, std::size_t v1, std::size_t v2)
                :
                Triangle{}
            {
                // After the code is executed, (v[0],v[1],v[2]) is a cyclic
                // permutation of (v0,v1,v2) with v[0] = min{v0,v1,v2}.
                if (v0 < v1)
                {
                    if (v0 < v2)
                    {
                        v[0] = v0;
                        v[1] = v1;
                        v[2] = v2;
                    }
                    else
                    {
                        v[0] = v2;
                        v[1] = v0;
                        v[2] = v1;
                    }
                }
                else
                {
                    if (v1 < v2)
                    {
                        v[0] = v1;
                        v[1] = v2;
                        v[2] = v0;
                    }
                    else
                    {
                        v[0] = v2;
                        v[1] = v0;
                        v[2] = v1;
                    }
                }
            }

            bool operator==(Triangle const& other) const
            {
                return v[0] == other.v[0] && v[1] == other.v[1] && v[2] == other.v[2];
            }

            bool operator<(Triangle const& other) const
            {
                for (std::size_t i = 0; i < 3; ++i)
                {
                    if (v[i] < other.v[i])
                    {
                        return true;
                    }
                    if (v[i] > other.v[i])
                    {
                        return false;
                    }
                }
                return false;
            }

            std::array<std::size_t, 3> v;
        };

        // Extract level surfaces and return rational vertices.
        virtual void Extract(IndexType level, std::vector<Vertex>& vertices,
            std::vector<Triangle>& triangles) = 0;

        void Extract(IndexType level, bool removeDuplicateVertices,
            std::vector<std::array<T, 3>>& vertices, std::vector<Triangle>& triangles)
        {
            std::vector<Vertex> rationalVertices{};
            Extract(level, rationalVertices, triangles);
            if (removeDuplicateVertices)
            {
                MakeUnique(rationalVertices, triangles);
            }
            Convert(rationalVertices, vertices);
        }

        // The extraction has duplicate vertices on edges shared by voxels.
        // This function will eliminate the duplicates.
        void MakeUnique(std::vector<Vertex>& vertices, std::vector<Triangle>& triangles)
        {
            std::size_t numVertices = vertices.size();
            std::size_t numTriangles = triangles.size();
            if (numVertices == 0 || numTriangles == 0)
            {
                return;
            }

            // Compute the map of unique vertices and assign to them new and
            // unique indices.
            std::map<Vertex, std::size_t> vmap{};
            std::size_t nextVertex = 0;
            for (std::size_t v = 0; v < numVertices; ++v)
            {
                // Keep only unique vertices.
                auto result = vmap.insert(std::make_pair(vertices[v], nextVertex));
                if (result.second)
                {
                    ++nextVertex;
                }
            }

            // Compute the map of unique triangles and assign to them new and
            // unique indices.
            std::map<Triangle, std::size_t> tmap{};
            std::size_t nextTriangle = 0;
            for (std::size_t t = 0; t < numTriangles; ++t)
            {
                Triangle& triangle = triangles[t];
                for (std::size_t i = 0; i < 3; ++i)
                {
                    auto iter = vmap.find(vertices[triangle.v[i]]);
                    GTL_ARGUMENT_ASSERT(
                        iter != vmap.end(),
                        "Expecting the vertex to be in the vmap.");
                    triangle.v[i] = iter->second;
                }

                // Keep only unique triangles.
                auto result = tmap.insert(std::make_pair(triangle, nextTriangle));
                if (result.second)
                {
                    ++nextTriangle;
                }
            }

            // Pack the vertices into an array.
            vertices.resize(vmap.size());
            for (auto const& element : vmap)
            {
                vertices[element.second] = element.first;
            }

            // Pack the triangles into an array.
            triangles.resize(tmap.size());
            for (auto const& element : tmap)
            {
                triangles[element.second] = element.first;
            }
        }

        // Convert from Vertex to std::array<T, 3> rationals.
        void Convert(std::vector<Vertex> const& input, std::vector<std::array<T, 3>>& output)
        {
            output.resize(input.size());
            for (std::size_t i = 0; i < input.size(); ++i)
            {
                T rxNumer = static_cast<T>(input[i].xNumer);
                T rxDenom = static_cast<T>(input[i].xDenom);
                T ryNumer = static_cast<T>(input[i].yNumer);
                T ryDenom = static_cast<T>(input[i].yDenom);
                T rzNumer = static_cast<T>(input[i].zNumer);
                T rzDenom = static_cast<T>(input[i].zDenom);
                output[i][0] = rxNumer / rxDenom;
                output[i][1] = ryNumer / ryDenom;
                output[i][2] = rzNumer / rzDenom;
            }
        }

        // The extraction does not use any topological information about the
        // level surfaces. The triangles can be a mixture of clockwise-ordered
        // and counterclockwise-ordered. This function is an attempt to give
        // the triangles a consistent ordering by selecting a normal in
        // approximately the same direction as the average gradient at the
        // vertices (when sameDir is true), or in the opposite direction (when
        // sameDir is false). This might not always produce a consistent
        // order, but is fast. A consistent order can be computed by
        // choosing a winding order for each triangle so that any corner of
        // the voxel containing the triangle and that has positive sign sees
        // a counterclockwise order. Of course, you can also choose that the
        // positive sign corners of the voxel always see the voxel-contained
        // triangles in clockwise order.
        void OrientTriangles(std::vector<std::array<T, 3>>& vertices,
            std::vector<Triangle>& triangles, bool sameDir)
        {
            for (auto& triangle : triangles)
            {
                // Get the triangle vertices.
                std::array<T, 3> v0 = vertices[triangle.v[0]];
                std::array<T, 3> v1 = vertices[triangle.v[1]];
                std::array<T, 3> v2 = vertices[triangle.v[2]];

                // Construct the triangle normal based on the current
                // orientation.
                std::array<T, 3> edge1{}, edge2{}, normal{};
                for (std::size_t i = 0; i < 3; ++i)
                {
                    edge1[i] = v1[i] - v0[i];
                    edge2[i] = v2[i] - v0[i];
                }
                normal[0] = edge1[1] * edge2[2] - edge1[2] * edge2[1];
                normal[1] = edge1[2] * edge2[0] - edge1[0] * edge2[2];
                normal[2] = edge1[0] * edge2[1] - edge1[1] * edge2[0];

                // Get the image gradient at the vertices.
                std::array<T, 3> grad0 = GetGradient(v0);
                std::array<T, 3> grad1 = GetGradient(v1);
                std::array<T, 3> grad2 = GetGradient(v2);

                // Compute the average gradient.
                std::array<T, 3> gradAvr{};
                for (std::size_t i = 0; i < 3; ++i)
                {
                    gradAvr[i] = (grad0[i] + grad1[i] + grad2[i]) / C_<T>(3);
                }

                // Compute the dot product of normal and average gradient.
                T dot = gradAvr[0] * normal[0] + gradAvr[1] * normal[1] + gradAvr[2] * normal[2];

                // Choose triangle orientation based on gradient direction.
                if (sameDir)
                {
                    if (dot < C_<T>(0))
                    {
                        // Wrong orientation, reorder it.
                        std::swap(triangle.v[1], triangle.v[2]);
                    }
                }
                else
                {
                    if (dot > C_<T>(0))
                    {
                        // Wrong orientation, reorder it.
                        std::swap(triangle.v[1], triangle.v[2]);
                    }
                }
            }
        }

        // Use this function if you want vertex normals for dynamic lighting
        // of the mesh.
        void ComputeNormals(std::vector<std::array<T, 3>> const& vertices,
            std::vector<Triangle> const& triangles,
            std::vector<std::array<T, 3>>& normals)
        {
            // Compute a vertex normal to be area-weighted sums of the normals
            // to the triangles that share that vertex.
            std::array<T, 3> const azero{ C_<T>(0), C_<T>(0), C_<T>(0) };
            normals.resize(vertices.size());
            std::fill(normals.begin(), normals.end(), azero);

            for (auto const& triangle : triangles)
            {
                // Get the triangle vertices.
                std::array<T, 3> v0 = vertices[triangle.v[0]];
                std::array<T, 3> v1 = vertices[triangle.v[1]];
                std::array<T, 3> v2 = vertices[triangle.v[2]];

                // Construct the triangle normal.
                std::array<T, 3> edge1{}, edge2{}, normal{};
                for (std::size_t i = 0; i < 3; ++i)
                {
                    edge1[i] = v1[i] - v0[i];
                    edge2[i] = v2[i] - v0[i];
                }
                normal[0] = edge1[1] * edge2[2] - edge1[2] * edge2[1];
                normal[1] = edge1[2] * edge2[0] - edge1[0] * edge2[2];
                normal[2] = edge1[0] * edge2[1] - edge1[1] * edge2[0];

                // Maintain the sum of normals at each vertex.
                for (std::size_t i = 0; i < 3; ++i)
                {
                    for (std::size_t j = 0; j < 3; ++j)
                    {
                        normals[triangle.v[i]][j] += normal[j];
                    }
                }
            }

            // The normal vector storage was used to accumulate the sum of
            // triangle normals. Now these vectors must be rescaled to be
            // unit length.
            for (auto& normal : normals)
            {
                T sqrLength = normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2];
                T length = std::sqrt(sqrLength);
                if (length > C_<T>(0))
                {
                    for (std::size_t i = 0; i < 3; ++i)
                    {
                        normal[i] /= length;
                    }
                }
                else
                {
                    for (std::size_t i = 0; i < 3; ++i)
                    {
                        normal[i] = C_<T>(0);
                    }
                }
            }
        }

    protected:
        // The input is a 3D image with lexicographically ordered voxels
        // (x,y,z) stored in a linear array. Voxel (x,y,z) is stored in the
        // array at location index = x + xBound * (y + yBound * z). The
        // inputs xBound, yBound and zBound must each be 2 or larger so that
        // there is at least one image cube to process. The inputVoxels must
        // be nonnull and point to contiguous storage that contains at least
        // xBound * yBound * zBound elements.
        SurfaceExtractor(std::size_t xBound, std::size_t yBound, std::size_t zBound, IndexType const* inputVoxels)
            :
            mXBound(xBound),
            mYBound(yBound),
            mZBound(zBound),
            mXYBound(xBound * yBound),
            mInputVoxels(inputVoxels),
            mVoxels(xBound * yBound * zBound)
        {
            static_assert(
                std::is_same<IndexType, std::uint16_t>::value ||
                std::is_same<IndexType, std::uint32_t>::value ||
                std::is_same<IndexType, std::uint64_t>::value ||
                std::is_same<IndexType, std::size_t>::value,
                "Unsupported index type.");

            GTL_ARGUMENT_ASSERT(
                mXBound > 1 && mYBound > 1 && mZBound > 1 && mInputVoxels != nullptr,
                "Invalid input.");
        }

        virtual std::array<T, 3> GetGradient(std::array<T, 3> const& pos) = 0;

        std::size_t mXBound, mYBound, mZBound, mXYBound;
        IndexType const* mInputVoxels;
        std::vector<std::int64_t> mVoxels;

    private:
        friend class UnitTestSurfaceExtractor;
    };
}
