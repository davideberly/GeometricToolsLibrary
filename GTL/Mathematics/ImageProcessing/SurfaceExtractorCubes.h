// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

#pragma once

// The level set extraction algorithm implemented here is described
// in Section 3.2 of the document
// https://www.geometrictools.com/Documentation/LevelSetExtraction.pdf

#include <GTL/Mathematics/ImageProcessing/SurfaceExtractor.h>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace gtl
{
    // The image type IntType must be one of the integer types: std::int8_t,
    // std::int16_t, std::int32_t, std::uint8_t, std::uint16_t or std::uint32_t. Internal integer
    // computations are performed using std::int64_t. The type T is for
    // extraction to floating-point vertices.
    template <typename IntType, typename T>
    class SurfaceExtractorCubes : public SurfaceExtractor<IntType, T>
    {
    public:
        // Convenient type definitions.
        using Vertex = typename SurfaceExtractor<IntType, T>::Vertex;
        using Triangle = typename SurfaceExtractor<IntType, T>::Triangle;

        // The input is a 3D image with lexicographically ordered voxels
        // (x,y,z) stored in a linear array. Voxel (x,y,z) is stored in the
        // array at location index = x + xBound * (y + yBound * z). The
        // inputs xBound, yBound and zBound must each be 2 or larger so that
        // there is at least one image cube to process. The inputVoxels must
        // be nonnull and point to contiguous storage that contains at least
        // xBound * yBound * zBound elements.
        SurfaceExtractorCubes(std::size_t xBound, std::size_t yBound, std::size_t zBound,
            IntType const* inputVoxels)
            :
            SurfaceExtractor<IntType, T>(xBound, yBound, zBound, inputVoxels)
        {
        }

        // Extract level surfaces and return rational vertices. Use the
        // base-class Extract if you want real-valued vertices.
        virtual void Extract(IntType level, std::vector<Vertex>& vertices,
            std::vector<Triangle>& triangles) override
        {
            // Adjust the image so that the level set is F(x,y,z) = 0. The
            // precondition for 'level' is that it is not exactly a voxel
            // value. However, IntType is an integer type, so we cannot pass
            // in a 'level' that has a fractional value. To circumvent this,
            // the voxel values are doubled so that they are even integers.
            // The level value is doubled and 1 added to obtain an odd
            // integer, guaranteeing 'level' is not a voxel value.
            std::int64_t levelI64 = 2 * static_cast<std::int64_t>(level) + 1;
            for (std::size_t i = 0; i < this->mVoxels.size(); ++i)
            {
                std::int64_t inputI64 = 2 * static_cast<std::int64_t>(this->mInputVoxels[i]);
                this->mVoxels[i] = inputI64 - levelI64;
            }

            vertices.clear();
            triangles.clear();
            for (std::size_t z = 0, zp = 1; zp < this->mZBound; ++z, ++zp)
            {
                for (std::size_t y = 0, yp = 1; yp < this->mYBound; ++y, ++yp)
                {
                    for (std::size_t x = 0, xp = 1; xp < this->mXBound; ++x, ++xp)
                    {
                        // Get vertices on edges of box (if any).
                        VETable table{};
                        std::int32_t type = GetVertices(x, y, z, table);
                        if (type != 0)
                        {
                            // Get edges on faces of box.
                            GetXMinEdges(x, y, z, type, table);
                            GetXMaxEdges(x, y, z, type, table);
                            GetYMinEdges(x, y, z, type, table);
                            GetYMaxEdges(x, y, z, type, table);
                            GetZMinEdges(x, y, z, type, table);
                            GetZMaxEdges(x, y, z, type, table);

                            // Ear-clip the wireframe mesh.
                            table.RemoveTriangles(vertices, triangles);
                        }
                    }
                }
            }
        }

    protected:
        // Bit flags for identifying the edge and face configurations.
        // The prefixes are EI for "edge index", EB for "edge bit",
        // FI for "face index" and FB for "face bit".
        static std::int32_t constexpr EI_XMIN_YMIN = 0;
        static std::int32_t constexpr EI_XMIN_YMAX = 1;
        static std::int32_t constexpr EI_XMAX_YMIN = 2;
        static std::int32_t constexpr EI_XMAX_YMAX = 3;
        static std::int32_t constexpr EI_XMIN_ZMIN = 4;
        static std::int32_t constexpr EI_XMIN_ZMAX = 5;
        static std::int32_t constexpr EI_XMAX_ZMIN = 6;
        static std::int32_t constexpr EI_XMAX_ZMAX = 7;
        static std::int32_t constexpr EI_YMIN_ZMIN = 8;
        static std::int32_t constexpr EI_YMIN_ZMAX = 9;
        static std::int32_t constexpr EI_YMAX_ZMIN = 10;
        static std::int32_t constexpr EI_YMAX_ZMAX = 11;
        static std::int32_t constexpr FI_XMIN = 12;
        static std::int32_t constexpr FI_XMAX = 13;
        static std::int32_t constexpr FI_YMIN = 14;
        static std::int32_t constexpr FI_YMAX = 15;
        static std::int32_t constexpr FI_ZMIN = 16;
        static std::int32_t constexpr FI_ZMAX = 17;

        static std::int32_t constexpr EB_XMIN_YMIN = (1 << EI_XMIN_YMIN);
        static std::int32_t constexpr EB_XMIN_YMAX = (1 << EI_XMIN_YMAX);
        static std::int32_t constexpr EB_XMAX_YMIN = (1 << EI_XMAX_YMIN);
        static std::int32_t constexpr EB_XMAX_YMAX = (1 << EI_XMAX_YMAX);
        static std::int32_t constexpr EB_XMIN_ZMIN = (1 << EI_XMIN_ZMIN);
        static std::int32_t constexpr EB_XMIN_ZMAX = (1 << EI_XMIN_ZMAX);
        static std::int32_t constexpr EB_XMAX_ZMIN = (1 << EI_XMAX_ZMIN);
        static std::int32_t constexpr EB_XMAX_ZMAX = (1 << EI_XMAX_ZMAX);
        static std::int32_t constexpr EB_YMIN_ZMIN = (1 << EI_YMIN_ZMIN);
        static std::int32_t constexpr EB_YMIN_ZMAX = (1 << EI_YMIN_ZMAX);
        static std::int32_t constexpr EB_YMAX_ZMIN = (1 << EI_YMAX_ZMIN);
        static std::int32_t constexpr EB_YMAX_ZMAX = (1 << EI_YMAX_ZMAX);
        static std::int32_t constexpr FB_XMIN = (1 << FI_XMIN);
        static std::int32_t constexpr FB_XMAX = (1 << FI_XMAX);
        static std::int32_t constexpr FB_YMIN = (1 << FI_YMIN);
        static std::int32_t constexpr FB_YMAX = (1 << FI_YMAX);
        static std::int32_t constexpr FB_ZMIN = (1 << FI_ZMIN);
        static std::int32_t constexpr FB_ZMAX = (1 << FI_ZMAX);

        // Vertex-edge-triangle table to support mesh topology.
        class VETable
        {
        public:
            VETable()
                :
                mVertex{}
            {
            }

            inline bool IsValidVertex(std::size_t i) const
            {
                return mVertex[i].valid;
            }

            inline std::int64_t GetXN(std::size_t i) const
            {
                return mVertex[i].position.xNumer;
            }

            inline std::int64_t GetXD(std::size_t i) const
            {
                return mVertex[i].position.xDenom;
            }

            inline std::int64_t GetYN(std::size_t i) const
            {
                return mVertex[i].position.yNumer;
            }

            inline std::int64_t GetYD(std::size_t i) const
            {
                return mVertex[i].position.yDenom;
            }

            inline std::int64_t GetZN(std::size_t i) const
            {
                return mVertex[i].position.zNumer;
            }

            inline std::int64_t GetZD(std::size_t i) const
            {
                return mVertex[i].position.zDenom;
            }

            void Insert(std::size_t i, Vertex const& position)
            {
                TVertex& vertex = mVertex[i];
                vertex.position = position;
                vertex.valid = true;
            }

            void Insert(std::size_t i0, std::size_t i1)
            {
                TVertex& vertex0 = mVertex[i0];
                TVertex& vertex1 = mVertex[i1];
                vertex0.adjacents[vertex0.numAdjacents++] = i1;
                vertex1.adjacents[vertex1.numAdjacents++] = i0;
            }

            void RemoveTriangles(std::vector<Vertex>& vertices, std::vector<Triangle>& triangles)
            {
                // Ear-clip the wireframe to get the triangles.
                Triangle triangle;
                while (Remove(triangle))
                {
                    std::size_t v0 = vertices.size();
                    std::size_t v1 = v0 + 1;
                    std::size_t v2 = v1 + 1;
                    triangles.push_back(Triangle(v0, v1, v2));
                    vertices.push_back(mVertex[triangle.v[0]].position);
                    vertices.push_back(mVertex[triangle.v[1]].position);
                    vertices.push_back(mVertex[triangle.v[2]].position);
                }
            }

        protected:
            void RemoveVertex(std::size_t i)
            {
                TVertex& vertex0 = mVertex[i];
                GTL_RUNTIME_ASSERT(
                    vertex0.numAdjacents == 2,
                    "Unexpected condition.");
                std::size_t a0 = vertex0.adjacents[0];
                std::size_t a1 = vertex0.adjacents[1];
                TVertex& adjVertex0 = mVertex[a0];
                TVertex& adjVertex1 = mVertex[a1];

                std::size_t j{};
                for (j = 0; j < adjVertex0.numAdjacents; ++j)
                {
                    if (adjVertex0.adjacents[j] == i)
                    {
                        adjVertex0.adjacents[j] = a1;
                        break;
                    }
                }
                GTL_RUNTIME_ASSERT(
                    j != adjVertex0.numAdjacents,
                    "Unexpected condition.");

                for (j = 0; j < adjVertex1.numAdjacents; j++)
                {
                    if (adjVertex1.adjacents[j] == i)
                    {
                        adjVertex1.adjacents[j] = a0;
                        break;
                    }
                }
                GTL_RUNTIME_ASSERT(
                    j != adjVertex1.numAdjacents,
                    "Unexpected condition.");

                vertex0.valid = false;

                if (adjVertex0.numAdjacents == 2)
                {
                    if (adjVertex0.adjacents[0] == adjVertex0.adjacents[1])
                    {
                        adjVertex0.valid = false;
                    }
                }

                if (adjVertex1.numAdjacents == 2)
                {
                    if (adjVertex1.adjacents[0] == adjVertex1.adjacents[1])
                    {
                        adjVertex1.valid = false;
                    }
                }
            }

            bool Remove(Triangle& triangle)
            {
                for (std::size_t i = 0; i < 18; ++i)
                {
                    TVertex& vertex = mVertex[i];
                    if (vertex.valid && vertex.numAdjacents == 2)
                    {
                        triangle.v[0] = i;
                        triangle.v[1] = vertex.adjacents[0];
                        triangle.v[2] = vertex.adjacents[1];
                        RemoveVertex(i);
                        return true;
                    }
                }
                return false;
            }

            class TVertex
            {
            public:
                TVertex()
                    :
                    position{},
                    numAdjacents(0),
                    adjacents{ 0, 0, 0, 0 },
                    valid(false)
                {
                }

                Vertex position;
                std::size_t numAdjacents;
                std::array<std::size_t, 4> adjacents;
                bool valid;
            };

            std::array<TVertex, 18> mVertex;
        };

        virtual std::array<T, 3> GetGradient(std::array<T, 3> const& position) override
        {
            std::int64_t ix = static_cast<std::int64_t>(std::floor(position[0]));
            if (ix < 0 || ix + 1 >= static_cast<std::int64_t>(this->mXBound))
            {
                return std::array<T, 3>{ C_<T>(0), C_<T>(0), C_<T>(0) };
            }

            std::int64_t iy = static_cast<std::int64_t>(position[1]);
            if (iy < 0 || iy + 1 >= static_cast<std::int64_t>(this->mYBound))
            {
                return std::array<T, 3>{ C_<T>(0), C_<T>(0), C_<T>(0) };
            }

            std::int64_t iz = static_cast<std::int64_t>(position[2]);
            if (iz < 0 || iz + 1 >= static_cast<std::int64_t>(this->mZBound))
            {
                return std::array<T, 3>{ C_<T>(0), C_<T>(0), C_<T>(0) };
            }

            std::size_t x = static_cast<std::size_t>(ix);
            std::size_t y = static_cast<std::size_t>(iy);
            std::size_t z = static_cast<std::size_t>(iz);

            // Get image values at corners of voxel.
            std::size_t i000 = x + this->mXBound * (y + this->mYBound * z);
            std::size_t i100 = i000 + 1;
            std::size_t i010 = i000 + this->mXBound;
            std::size_t i110 = i010 + 1;
            std::size_t i001 = i000 + this->mXYBound;
            std::size_t i101 = i001 + 1;
            std::size_t i011 = i001 + this->mXBound;
            std::size_t i111 = i011 + 1;
            T f000 = static_cast<T>(this->mVoxels[i000]);
            T f100 = static_cast<T>(this->mVoxels[i100]);
            T f010 = static_cast<T>(this->mVoxels[i010]);
            T f110 = static_cast<T>(this->mVoxels[i110]);
            T f001 = static_cast<T>(this->mVoxels[i001]);
            T f101 = static_cast<T>(this->mVoxels[i101]);
            T f011 = static_cast<T>(this->mVoxels[i011]);
            T f111 = static_cast<T>(this->mVoxels[i111]);

            T dx = position[0] - static_cast<T>(x);
            T dy = position[1] - static_cast<T>(y);
            T dz = position[2] - static_cast<T>(z);
            T oneMX = C_<T>(1) - dx;
            T oneMY = C_<T>(1) - dy;
            T oneMZ = C_<T>(1) - dz;

            std::array<T, 3> grad{};

            T tmp0 = oneMY * (f100 - f000) + dy * (f110 - f010);
            T tmp1 = oneMY * (f101 - f001) + dy * (f111 - f011);
            grad[0] = oneMZ * tmp0 + dz * tmp1;

            tmp0 = oneMX * (f010 - f000) + dx * (f110 - f100);
            tmp1 = oneMX * (f011 - f001) + dx * (f111 - f101);
            grad[1] = oneMZ * tmp0 + dz * tmp1;

            tmp0 = oneMX * (f001 - f000) + dx * (f101 - f100);
            tmp1 = oneMX * (f011 - f010) + dx * (f111 - f110);
            grad[2] = oneMY * tmp0 + dy * tmp1;

            return grad;
        }

        std::int32_t GetVertices(std::size_t x, std::size_t y, std::size_t z, VETable& table)
        {
            std::int32_t type = 0;

            // Get the image values at the corners of the voxel.
            std::size_t i000 = x + this->mXBound * (y + this->mYBound * z);
            std::size_t i100 = i000 + 1;
            std::size_t i010 = i000 + this->mXBound;
            std::size_t i110 = i010 + 1;
            std::size_t i001 = i000 + this->mXYBound;
            std::size_t i101 = i001 + 1;
            std::size_t i011 = i001 + this->mXBound;
            std::size_t i111 = i011 + 1;
            std::int64_t f000 = this->mVoxels[i000];
            std::int64_t f100 = this->mVoxels[i100];
            std::int64_t f010 = this->mVoxels[i010];
            std::int64_t f110 = this->mVoxels[i110];
            std::int64_t f001 = this->mVoxels[i001];
            std::int64_t f101 = this->mVoxels[i101];
            std::int64_t f011 = this->mVoxels[i011];
            std::int64_t f111 = this->mVoxels[i111];

            std::int64_t x0 = x;
            std::int64_t x1 = x + 1;
            std::int64_t y0 = y;
            std::int64_t y1 = y + 1;
            std::int64_t z0 = z;
            std::int64_t z1 = z + 1;
            std::int64_t d{};

            // xmin-ymin edge
            if (f000 * f001 < 0)
            {
                type |= EB_XMIN_YMIN;
                d = f001 - f000;
                table.Insert(EI_XMIN_YMIN, Vertex(x0, 1, y0, 1, z0 * d - f000, d));
            }

            // xmin-ymax edge
            if (f010 * f011 < 0)
            {
                type |= EB_XMIN_YMAX;
                d = f011 - f010;
                table.Insert(EI_XMIN_YMAX, Vertex(x0, 1, y1, 1, z0 * d - f010, d));
            }

            // xmax-ymin edge
            if (f100 * f101 < 0)
            {
                type |= EB_XMAX_YMIN;
                d = f101 - f100;
                table.Insert(EI_XMAX_YMIN, Vertex(x1, 1, y0, 1, z0 * d - f100, d));
            }

            // xmax-ymax edge
            if (f110 * f111 < 0)
            {
                type |= EB_XMAX_YMAX;
                d = f111 - f110;
                table.Insert(EI_XMAX_YMAX, Vertex(x1, 1, y1, 1, z0 * d - f110, d));
            }

            // xmin-zmin edge
            if (f000 * f010 < 0)
            {
                type |= EB_XMIN_ZMIN;
                d = f010 - f000;
                table.Insert(EI_XMIN_ZMIN, Vertex(x0, 1, y0 * d - f000, d, z0, 1));
            }

            // xmin-zmax edge
            if (f001 * f011 < 0)
            {
                type |= EB_XMIN_ZMAX;
                d = f011 - f001;
                table.Insert(EI_XMIN_ZMAX, Vertex(x0, 1, y0 * d - f001, d, z1, 1));
            }

            // xmax-zmin edge
            if (f100 * f110 < 0)
            {
                type |= EB_XMAX_ZMIN;
                d = f110 - f100;
                table.Insert(EI_XMAX_ZMIN, Vertex(x1, 1, y0 * d - f100, d, z0, 1));
            }

            // xmax-zmax edge
            if (f101 * f111 < 0)
            {
                type |= EB_XMAX_ZMAX;
                d = f111 - f101;
                table.Insert(EI_XMAX_ZMAX, Vertex(x1, 1, y0 * d - f101, d, z1, 1));
            }

            // ymin-zmin edge
            if (f000 * f100 < 0)
            {
                type |= EB_YMIN_ZMIN;
                d = f100 - f000;
                table.Insert(EI_YMIN_ZMIN, Vertex(x0 * d - f000, d, y0, 1, z0, 1));
            }

            // ymin-zmax edge
            if (f001 * f101 < 0)
            {
                type |= EB_YMIN_ZMAX;
                d = f101 - f001;
                table.Insert(EI_YMIN_ZMAX, Vertex(x0 * d - f001, d, y0, 1, z1, 1));
            }

            // ymax-zmin edge
            if (f010 * f110 < 0)
            {
                type |= EB_YMAX_ZMIN;
                d = f110 - f010;
                table.Insert(EI_YMAX_ZMIN, Vertex(x0 * d - f010, d, y1, 1, z0, 1));
            }

            // ymax-zmax edge
            if (f011 * f111 < 0)
            {
                type |= EB_YMAX_ZMAX;
                d = f111 - f011;
                table.Insert(EI_YMAX_ZMAX, Vertex(x0 * d - f011, d, y1, 1, z1, 1));
            }

            return type;
        }

        void GetXMinEdges(std::size_t x, std::size_t y, std::size_t z, std::int32_t type, VETable& table)
        {
            std::int32_t faceType = 0;
            if (type & EB_XMIN_YMIN)
            {
                faceType |= 0x01;
            }
            if (type & EB_XMIN_YMAX)
            {
                faceType |= 0x02;
            }
            if (type & EB_XMIN_ZMIN)
            {
                faceType |= 0x04;
            }
            if (type & EB_XMIN_ZMAX)
            {
                faceType |= 0x08;
            }

            switch (faceType)
            {
            case 0:
                break;
            case 3:
                table.Insert(EI_XMIN_YMIN, EI_XMIN_YMAX);
                break;
            case 5:
                table.Insert(EI_XMIN_YMIN, EI_XMIN_ZMIN);
                break;
            case 6:
                table.Insert(EI_XMIN_YMAX, EI_XMIN_ZMIN);
                break;
            case 9:
                table.Insert(EI_XMIN_YMIN, EI_XMIN_ZMAX);
                break;
            case 10:
                table.Insert(EI_XMIN_YMAX, EI_XMIN_ZMAX);
                break;
            case 12:
                table.Insert(EI_XMIN_ZMIN, EI_XMIN_ZMAX);
                break;
            case 15:
            {
                // Four vertices, one per edge, need to disambiguate.
                std::size_t i = x + this->mXBound * (y + this->mYBound * z);
                // F(x,y,z)
                std::int64_t f00 = this->mVoxels[i];
                i += this->mXBound;
                // F(x,y+1,z)
                std::int64_t f10 = this->mVoxels[i];
                i += this->mXYBound;
                // F(x,y+1,z+1)
                std::int64_t f11 = this->mVoxels[i];
                i -= this->mXBound;
                // F(x,y,z+1)
                std::int64_t f01 = this->mVoxels[i];
                std::int64_t det = f00 * f11 - f01 * f10;

                if (det > 0)
                {
                    // Disjoint hyperbolic segments, pair <P0,P2>, <P1,P3>.
                    table.Insert(EI_XMIN_YMIN, EI_XMIN_ZMIN);
                    table.Insert(EI_XMIN_YMAX, EI_XMIN_ZMAX);
                }
                else if (det < 0)
                {
                    // Disjoint hyperbolic segments, pair <P0,P3>, <P1,P2>.
                    table.Insert(EI_XMIN_YMIN, EI_XMIN_ZMAX);
                    table.Insert(EI_XMIN_YMAX, EI_XMIN_ZMIN);
                }
                else
                {
                    // Plus-sign configuration, add branch point to
                    // tessellation.
                    table.Insert(FI_XMIN, Vertex(
                        table.GetXN(EI_XMIN_ZMIN), table.GetXD(EI_XMIN_ZMIN),
                        table.GetYN(EI_XMIN_ZMIN), table.GetYD(EI_XMIN_ZMIN),
                        table.GetZN(EI_XMIN_YMIN), table.GetZD(EI_XMIN_YMIN)));

                    // Add edges sharing the branch point.
                    table.Insert(EI_XMIN_YMIN, FI_XMIN);
                    table.Insert(EI_XMIN_YMAX, FI_XMIN);
                    table.Insert(EI_XMIN_ZMIN, FI_XMIN);
                    table.Insert(EI_XMIN_ZMAX, FI_XMIN);
                }
                break;
            }
            default:
                GTL_RUNTIME_ERROR("Unexpected condition.");
            }
        }

        void GetXMaxEdges(std::size_t x, std::size_t y, std::size_t z, std::int32_t type, VETable& table)
        {
            std::int32_t faceType = 0;
            if (type & EB_XMAX_YMIN)
            {
                faceType |= 0x01;
            }
            if (type & EB_XMAX_YMAX)
            {
                faceType |= 0x02;
            }
            if (type & EB_XMAX_ZMIN)
            {
                faceType |= 0x04;
            }
            if (type & EB_XMAX_ZMAX)
            {
                faceType |= 0x08;
            }

            switch (faceType)
            {
            case 0:
                break;
            case 3:
                table.Insert(EI_XMAX_YMIN, EI_XMAX_YMAX);
                break;
            case 5:
                table.Insert(EI_XMAX_YMIN, EI_XMAX_ZMIN);
                break;
            case 6:
                table.Insert(EI_XMAX_YMAX, EI_XMAX_ZMIN);
                break;
            case 9:
                table.Insert(EI_XMAX_YMIN, EI_XMAX_ZMAX);
                break;
            case 10:
                table.Insert(EI_XMAX_YMAX, EI_XMAX_ZMAX);
                break;
            case 12:
                table.Insert(EI_XMAX_ZMIN, EI_XMAX_ZMAX);
                break;
            case 15:
            {
                // Four vertices, one per edge, need to disambiguate.
                std::size_t i = (x + 1) + this->mXBound * (y + this->mYBound * z);
                // F(x,y,z)
                std::int64_t f00 = this->mVoxels[i];
                i += this->mXBound;
                // F(x,y+1,z)
                std::int64_t f10 = this->mVoxels[i];
                i += this->mXYBound;
                // F(x,y+1,z+1)
                std::int64_t f11 = this->mVoxels[i];
                i -= this->mXBound;
                // F(x,y,z+1)
                std::int64_t f01 = this->mVoxels[i];
                std::int64_t det = f00 * f11 - f01 * f10;

                if (det > 0)
                {
                    // Disjoint hyperbolic segments, pair <P0,P2>, <P1,P3>.
                    table.Insert(EI_XMAX_YMIN, EI_XMAX_ZMIN);
                    table.Insert(EI_XMAX_YMAX, EI_XMAX_ZMAX);
                }
                else if (det < 0)
                {
                    // Disjoint hyperbolic segments, pair <P0,P3>, <P1,P2>.
                    table.Insert(EI_XMAX_YMIN, EI_XMAX_ZMAX);
                    table.Insert(EI_XMAX_YMAX, EI_XMAX_ZMIN);
                }
                else
                {
                    // Plus-sign configuration, add branch point to
                    // tessellation.
                    table.Insert(FI_XMAX, Vertex(
                        table.GetXN(EI_XMAX_ZMIN), table.GetXD(EI_XMAX_ZMIN),
                        table.GetYN(EI_XMAX_ZMIN), table.GetYD(EI_XMAX_ZMIN),
                        table.GetZN(EI_XMAX_YMIN), table.GetZD(EI_XMAX_YMIN)));

                    // Add edges sharing the branch point.
                    table.Insert(EI_XMAX_YMIN, FI_XMAX);
                    table.Insert(EI_XMAX_YMAX, FI_XMAX);
                    table.Insert(EI_XMAX_ZMIN, FI_XMAX);
                    table.Insert(EI_XMAX_ZMAX, FI_XMAX);
                }
                break;
            }
            default:
                GTL_RUNTIME_ERROR("Unexpected condition.");
            }
        }

        void GetYMinEdges(std::size_t x, std::size_t y, std::size_t z, std::int32_t type, VETable& table)
        {
            std::int32_t faceType = 0;
            if (type & EB_XMIN_YMIN)
            {
                faceType |= 0x01;
            }
            if (type & EB_XMAX_YMIN)
            {
                faceType |= 0x02;
            }
            if (type & EB_YMIN_ZMIN)
            {
                faceType |= 0x04;
            }
            if (type & EB_YMIN_ZMAX)
            {
                faceType |= 0x08;
            }

            switch (faceType)
            {
            case 0:
                break;
            case 3:
                table.Insert(EI_XMIN_YMIN, EI_XMAX_YMIN);
                break;
            case 5:
                table.Insert(EI_XMIN_YMIN, EI_YMIN_ZMIN);
                break;
            case 6:
                table.Insert(EI_XMAX_YMIN, EI_YMIN_ZMIN);
                break;
            case 9:
                table.Insert(EI_XMIN_YMIN, EI_YMIN_ZMAX);
                break;
            case 10:
                table.Insert(EI_XMAX_YMIN, EI_YMIN_ZMAX);
                break;
            case 12:
                table.Insert(EI_YMIN_ZMIN, EI_YMIN_ZMAX);
                break;
            case 15:
            {
                // Four vertices, one per edge, need to disambiguate.
                std::size_t i = x + this->mXBound * (y + this->mYBound * z);
                // F(x,y,z)
                std::int64_t f00 = this->mVoxels[i];
                ++i;
                // F(x+1,y,z)
                std::int64_t f10 = this->mVoxels[i];
                i += this->mXYBound;
                // F(x+1,y,z+1)
                std::int64_t f11 = this->mVoxels[i];
                --i;
                // F(x,y,z+1)
                std::int64_t f01 = this->mVoxels[i];
                std::int64_t det = f00 * f11 - f01 * f10;

                if (det > 0)
                {
                    // Disjoint hyperbolic segments, pair <P0,P2>, <P1,P3>.
                    table.Insert(EI_XMIN_YMIN, EI_YMIN_ZMIN);
                    table.Insert(EI_XMAX_YMIN, EI_YMIN_ZMAX);
                }
                else if (det < 0)
                {
                    // Disjoint hyperbolic segments, pair <P0,P3>, <P1,P2>.
                    table.Insert(EI_XMIN_YMIN, EI_YMIN_ZMAX);
                    table.Insert(EI_XMAX_YMIN, EI_YMIN_ZMIN);
                }
                else
                {
                    // Plus-sign configuration, add branch point to
                    // tessellation.
                    table.Insert(FI_YMIN, Vertex(
                        table.GetXN(EI_YMIN_ZMIN), table.GetXD(EI_YMIN_ZMIN),
                        table.GetYN(EI_XMIN_YMIN), table.GetYD(EI_XMIN_YMIN),
                        table.GetZN(EI_XMIN_YMIN), table.GetZD(EI_XMIN_YMIN)));

                    // Add edges sharing the branch point.
                    table.Insert(EI_XMIN_YMIN, FI_YMIN);
                    table.Insert(EI_XMAX_YMIN, FI_YMIN);
                    table.Insert(EI_YMIN_ZMIN, FI_YMIN);
                    table.Insert(EI_YMIN_ZMAX, FI_YMIN);
                }
                break;
            }
            default:
                GTL_RUNTIME_ERROR("Unexpected condition.");
            }
        }

        void GetYMaxEdges(std::size_t x, std::size_t y, std::size_t z, std::int32_t type, VETable& table)
        {
            std::int32_t faceType = 0;
            if (type & EB_XMIN_YMAX)
            {
                faceType |= 0x01;
            }
            if (type & EB_XMAX_YMAX)
            {
                faceType |= 0x02;
            }
            if (type & EB_YMAX_ZMIN)
            {
                faceType |= 0x04;
            }
            if (type & EB_YMAX_ZMAX)
            {
                faceType |= 0x08;
            }

            switch (faceType)
            {
            case 0:
                break;
            case 3:
                table.Insert(EI_XMIN_YMAX, EI_XMAX_YMAX);
                break;
            case 5:
                table.Insert(EI_XMIN_YMAX, EI_YMAX_ZMIN);
                break;
            case 6:
                table.Insert(EI_XMAX_YMAX, EI_YMAX_ZMIN);
                break;
            case 9:
                table.Insert(EI_XMIN_YMAX, EI_YMAX_ZMAX);
                break;
            case 10:
                table.Insert(EI_XMAX_YMAX, EI_YMAX_ZMAX);
                break;
            case 12:
                table.Insert(EI_YMAX_ZMIN, EI_YMAX_ZMAX);
                break;
            case 15:
            {
                // Four vertices, one per edge, need to disambiguate.
                std::size_t i = x + this->mXBound * ((y + 1) + this->mYBound * z);
                // F(x,y,z)
                std::int64_t f00 = this->mVoxels[i];
                ++i;
                // F(x+1,y,z)
                std::int64_t f10 = this->mVoxels[i];
                i += this->mXYBound;
                // F(x+1,y,z+1)
                std::int64_t f11 = this->mVoxels[i];
                --i;
                // F(x,y,z+1)
                std::int64_t f01 = this->mVoxels[i];
                std::int64_t det = f00 * f11 - f01 * f10;

                if (det > 0)
                {
                    // Disjoint hyperbolic segments, pair <P0,P2>, <P1,P3>.
                    table.Insert(EI_XMIN_YMAX, EI_YMAX_ZMIN);
                    table.Insert(EI_XMAX_YMAX, EI_YMAX_ZMAX);
                }
                else if (det < 0)
                {
                    // Disjoint hyperbolic segments, pair <P0,P3>, <P1,P2>.
                    table.Insert(EI_XMIN_YMAX, EI_YMAX_ZMAX);
                    table.Insert(EI_XMAX_YMAX, EI_YMAX_ZMIN);
                }
                else
                {
                    // Plus-sign configuration, add branch point to
                    // tessellation.
                    table.Insert(FI_YMAX, Vertex(
                        table.GetXN(EI_YMAX_ZMIN), table.GetXD(EI_YMAX_ZMIN),
                        table.GetYN(EI_XMIN_YMAX), table.GetYD(EI_XMIN_YMAX),
                        table.GetZN(EI_XMIN_YMAX), table.GetZD(EI_XMIN_YMAX)));

                    // Add edges sharing the branch point.
                    table.Insert(EI_XMIN_YMAX, FI_YMAX);
                    table.Insert(EI_XMAX_YMAX, FI_YMAX);
                    table.Insert(EI_YMAX_ZMIN, FI_YMAX);
                    table.Insert(EI_YMAX_ZMAX, FI_YMAX);
                }
                break;
            }
            default:
                GTL_RUNTIME_ERROR("Unexpected condition.");
            }
        }

        void GetZMinEdges(std::size_t x, std::size_t y, std::size_t z, std::int32_t type, VETable& table)
        {
            std::int32_t faceType = 0;
            if (type & EB_XMIN_ZMIN)
            {
                faceType |= 0x01;
            }
            if (type & EB_XMAX_ZMIN)
            {
                faceType |= 0x02;
            }
            if (type & EB_YMIN_ZMIN)
            {
                faceType |= 0x04;
            }
            if (type & EB_YMAX_ZMIN)
            {
                faceType |= 0x08;
            }

            switch (faceType)
            {
            case 0:
                break;
            case 3:
                table.Insert(EI_XMIN_ZMIN, EI_XMAX_ZMIN);
                break;
            case 5:
                table.Insert(EI_XMIN_ZMIN, EI_YMIN_ZMIN);
                break;
            case 6:
                table.Insert(EI_XMAX_ZMIN, EI_YMIN_ZMIN);
                break;
            case 9:
                table.Insert(EI_XMIN_ZMIN, EI_YMAX_ZMIN);
                break;
            case 10:
                table.Insert(EI_XMAX_ZMIN, EI_YMAX_ZMIN);
                break;
            case 12:
                table.Insert(EI_YMIN_ZMIN, EI_YMAX_ZMIN);
                break;
            case 15:
            {
                // Four vertices, one per edge, need to disambiguate.
                std::size_t i = x + this->mXBound * (y + this->mYBound * z);
                // F(x,y,z)
                std::int64_t f00 = this->mVoxels[i];
                ++i;
                // F(x+1,y,z)
                std::int64_t f10 = this->mVoxels[i];
                i += this->mXBound;
                // F(x+1,y+1,z)
                std::int64_t f11 = this->mVoxels[i];
                --i;
                // F(x,y+1,z)
                std::int64_t f01 = this->mVoxels[i];
                std::int64_t det = f00 * f11 - f01 * f10;

                if (det > 0)
                {
                    // Disjoint hyperbolic segments, pair <P0,P2>, <P1,P3>.
                    table.Insert(EI_XMIN_ZMIN, EI_YMIN_ZMIN);
                    table.Insert(EI_XMAX_ZMIN, EI_YMAX_ZMIN);
                }
                else if (det < 0)
                {
                    // Disjoint hyperbolic segments, pair <P0,P3>, <P1,P2>.
                    table.Insert(EI_XMIN_ZMIN, EI_YMAX_ZMIN);
                    table.Insert(EI_XMAX_ZMIN, EI_YMIN_ZMIN);
                }
                else
                {
                    // Plus-sign configuration, add branch point to
                    // tessellation.
                    table.Insert(FI_ZMIN, Vertex(
                        table.GetXN(EI_YMIN_ZMIN), table.GetXD(EI_YMIN_ZMIN),
                        table.GetYN(EI_XMIN_ZMIN), table.GetYD(EI_XMIN_ZMIN),
                        table.GetZN(EI_XMIN_ZMIN), table.GetZD(EI_XMIN_ZMIN)));

                    // Add edges sharing the branch point.
                    table.Insert(EI_XMIN_ZMIN, FI_ZMIN);
                    table.Insert(EI_XMAX_ZMIN, FI_ZMIN);
                    table.Insert(EI_YMIN_ZMIN, FI_ZMIN);
                    table.Insert(EI_YMAX_ZMIN, FI_ZMIN);
                }
                break;
            }
            default:
                GTL_RUNTIME_ERROR("Unexpected condition.");
            }
        }

        void GetZMaxEdges(std::size_t x, std::size_t y, std::size_t z, std::int32_t type, VETable& table)
        {
            std::int32_t faceType = 0;
            if (type & EB_XMIN_ZMAX)
            {
                faceType |= 0x01;
            }
            if (type & EB_XMAX_ZMAX)
            {
                faceType |= 0x02;
            }
            if (type & EB_YMIN_ZMAX)
            {
                faceType |= 0x04;
            }
            if (type & EB_YMAX_ZMAX)
            {
                faceType |= 0x08;
            }

            switch (faceType)
            {
            case 0:
                break;
            case 3:
                table.Insert(EI_XMIN_ZMAX, EI_XMAX_ZMAX);
                break;
            case 5:
                table.Insert(EI_XMIN_ZMAX, EI_YMIN_ZMAX);
                break;
            case 6:
                table.Insert(EI_XMAX_ZMAX, EI_YMIN_ZMAX);
                break;
            case 9:
                table.Insert(EI_XMIN_ZMAX, EI_YMAX_ZMAX);
                break;
            case 10:
                table.Insert(EI_XMAX_ZMAX, EI_YMAX_ZMAX);
                break;
            case 12:
                table.Insert(EI_YMIN_ZMAX, EI_YMAX_ZMAX);
                break;
            case 15:
            {
                // Four vertices, one per edge, need to disambiguate.
                std::size_t i = x + this->mXBound * (y + this->mYBound * (z + 1));
                // F(x,y,z)
                std::int64_t f00 = this->mVoxels[i];
                ++i;
                // F(x+1,y,z)
                std::int64_t f10 = this->mVoxels[i];
                i += this->mXBound;
                // F(x+1,y+1,z)
                std::int64_t f11 = this->mVoxels[i];
                --i;
                // F(x,y+1,z)
                std::int64_t f01 = this->mVoxels[i];
                std::int64_t det = f00 * f11 - f01 * f10;

                if (det > 0)
                {
                    // Disjoint hyperbolic segments, pair <P0,P2>, <P1,P3>.
                    table.Insert(EI_XMIN_ZMAX, EI_YMIN_ZMAX);
                    table.Insert(EI_XMAX_ZMAX, EI_YMAX_ZMAX);
                }
                else if (det < 0)
                {
                    // Disjoint hyperbolic segments, pair <P0,P3>, <P1,P2>.
                    table.Insert(EI_XMIN_ZMAX, EI_YMAX_ZMAX);
                    table.Insert(EI_XMAX_ZMAX, EI_YMIN_ZMAX);
                }
                else
                {
                    // Plus-sign configuration, add branch point to
                    // tessellation.
                    table.Insert(FI_ZMAX, Vertex(
                        table.GetXN(EI_YMIN_ZMAX), table.GetXD(EI_YMIN_ZMAX),
                        table.GetYN(EI_XMIN_ZMAX), table.GetYD(EI_XMIN_ZMAX),
                        table.GetZN(EI_XMIN_ZMAX), table.GetZD(EI_XMIN_ZMAX)));

                    // Add edges sharing the branch point.
                    table.Insert(EI_XMIN_ZMAX, FI_ZMAX);
                    table.Insert(EI_XMAX_ZMAX, FI_ZMAX);
                    table.Insert(EI_YMIN_ZMAX, FI_ZMAX);
                    table.Insert(EI_YMAX_ZMAX, FI_ZMAX);
                }
                break;
            }
            default:
                GTL_RUNTIME_ERROR("Unexpected condition.");
            }
        }

    private:
        friend class UnitTestSurfaceExtractorCubes;
    };
}
