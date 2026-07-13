// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

#pragma once

// The level set extraction algorithm implemented here is described
// in the document
// https://www.geometrictools.com/Documentation/ExtractLevelSurfaces.pdf

#include <GTL/Mathematics/ImageProcessing/SurfaceExtractor.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <map>
#include <set>
#include <utility>
#include <vector>

namespace gtl
{
    // The image type IntType must be one of the integer types: std::int8_t,
    // std::int16_t, std::int32_t, std::uint8_t, std::uint16_t or std::uint32_t. Internal integer
    // computations are performed using std::int64_t. The type T is for
    // extraction to floating-point vertices.
    template <typename IntType, typename T>
    class SurfaceExtractorTetrahedra : public SurfaceExtractor<IntType, T>
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
        SurfaceExtractorTetrahedra(std::size_t xBound, std::size_t yBound, std::size_t zBound,
            IntType const* inputVoxels)
            :
            SurfaceExtractor<IntType, T>(xBound, yBound, zBound, inputVoxels),
            mVMap{},
            mESet{},
            mTSet{},
            mNextVertex(0)
        {
        }

        // Extract level surfaces and return rational vertices. Use the
        // base-class Extract if you want real-valued vertices.
        virtual void Extract(IntType level, std::vector<Vertex>& vertices,
            std::vector<Triangle>& triangles) override
        {
            // Adjust the image so that the level set is F(x,y,z) = 0.
            std::int64_t levelI64 = static_cast<std::int64_t>(level);
            for (std::size_t i = 0; i < this->mVoxels.size(); ++i)
            {
                std::int64_t inputI64 = static_cast<std::int64_t>(this->mInputVoxels[i]);
                this->mVoxels[i] = inputI64 - levelI64;
            }

            mVMap.clear();
            mESet.clear();
            mTSet.clear();
            mNextVertex = 0;
            vertices.clear();
            triangles.clear();
            for (std::size_t z = 0, zp = 1; zp < this->mZBound; ++z, ++zp)
            {
                std::size_t zParity = (z & 1);
                for (std::size_t y = 0, yp = 1; yp < this->mYBound; ++y, ++yp)
                {
                    std::size_t yParity = (y & 1);
                    for (std::size_t x = 0, xp = 1; xp < this->mXBound; ++x, ++xp)
                    {
                        std::size_t xParity = (x & 1);

                        std::size_t i000 = x + this->mXBound * (y + this->mYBound * z);
                        std::size_t i100 = i000 + 1;
                        std::size_t i010 = i000 + this->mXBound;
                        std::size_t i110 = i010 + 1;
                        std::size_t i001 = i000 + this->mXYBound;
                        std::size_t i101 = i001 + 1;
                        std::size_t i011 = i001 + this->mXBound;
                        std::size_t i111 = i011 + 1;
                        std::int64_t f000 = static_cast<std::int64_t>(this->mVoxels[i000]);
                        std::int64_t f100 = static_cast<std::int64_t>(this->mVoxels[i100]);
                        std::int64_t f010 = static_cast<std::int64_t>(this->mVoxels[i010]);
                        std::int64_t f110 = static_cast<std::int64_t>(this->mVoxels[i110]);
                        std::int64_t f001 = static_cast<std::int64_t>(this->mVoxels[i001]);
                        std::int64_t f101 = static_cast<std::int64_t>(this->mVoxels[i101]);
                        std::int64_t f011 = static_cast<std::int64_t>(this->mVoxels[i011]);
                        std::int64_t f111 = static_cast<std::int64_t>(this->mVoxels[i111]);

                        if (xParity ^ yParity ^ zParity)
                        {
                            // 1205
                            ProcessTetrahedron(
                                xp, y, z, f100,
                                xp, yp, z, f110,
                                x, y, z, f000,
                                xp, y, zp, f101);

                            // 3027
                            ProcessTetrahedron(
                                x, yp, z, f010,
                                x, y, z, f000,
                                xp, yp, z, f110,
                                x, yp, zp, f011);

                            // 4750
                            ProcessTetrahedron(
                                x, y, zp, f001,
                                x, yp, zp, f011,
                                xp, y, zp, f101,
                                x, y, z, f000);

                            // 6572
                            ProcessTetrahedron(
                                xp, yp, zp, f111,
                                xp, y, zp, f101,
                                x, yp, zp, f011,
                                xp, yp, z, f110);

                            // 0752
                            ProcessTetrahedron(
                                x, y, z, f000,
                                x, yp, zp, f011,
                                xp, y, zp, f101,
                                xp, yp, z, f110);
                        }
                        else
                        {
                            // 0134
                            ProcessTetrahedron(
                                x, y, z, f000,
                                xp, y, z, f100,
                                x, yp, z, f010,
                                x, y, zp, f001);

                            // 2316
                            ProcessTetrahedron(
                                xp, yp, z, f110,
                                x, yp, z, f010,
                                xp, y, z, f100,
                                xp, yp, zp, f111);

                            // 5461
                            ProcessTetrahedron(
                                xp, y, zp, f101,
                                x, y, zp, f001,
                                xp, yp, zp, f111,
                                xp, y, z, f100);

                            // 7643
                            ProcessTetrahedron(
                                x, yp, zp, f011,
                                xp, yp, zp, f111,
                                x, y, zp, f001,
                                x, yp, z, f010);

                            // 6314
                            ProcessTetrahedron(
                                xp, yp, zp, f111,
                                x, yp, z, f010,
                                xp, y, z, f100,
                                x, y, zp, f001);
                        }
                    }
                }
            }

            // Pack vertices into an array.
            vertices.resize(mVMap.size());
            for (auto const& element : mVMap)
            {
                vertices[element.second] = element.first;
            }

            // Pack edges into an array (computed, but not reported to
            // caller).
            std::vector<Edge> edges(mESet.size());
            std::size_t i = 0;
            for (auto const& element : mESet)
            {
                edges[i++] = element;
            }

            // Pack triangles into an array.
            triangles.resize(mTSet.size());
            i = 0;
            for (auto const& element : mTSet)
            {
                triangles[i++] = element;
            }
        }

    protected:
        struct Edge
        {
            Edge()
                :
                v{ 0, 0 }
            {
            }

            Edge(std::size_t v0, std::size_t v1)
                :
                Edge{}
            {
                if (v0 < v1)
                {
                    v[0] = v0;
                    v[1] = v1;
                }
                else
                {
                    v[0] = v1;
                    v[1] = v0;
                }
            }

            bool operator==(Edge const& other) const
            {
                return v[0] == other.v[0] && v[1] == other.v[1];
            }

            bool operator<(Edge const& other) const
            {
                for (std::size_t i = 0; i < 2; ++i)
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

            std::array<std::size_t, 2> v;
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

            std::array<T, 3> grad{};

            if ((x & 1) ^ (y & 1) ^ (z & 1))
            {
                if (dx - dy - dz >= C_<T>(0))
                {
                    // 1205
                    grad[0] = +f100 - f000;
                    grad[1] = -f100 + f110;
                    grad[2] = -f100 + f101;
                }
                else if (dx - dy + dz <= C_<T>(0))
                {
                    // 3027
                    grad[0] = -f010 + f110;
                    grad[1] = +f010 - f000;
                    grad[2] = -f010 + f011;
                }
                else if (dx + dy - dz <= C_<T>(0))
                {
                    // 4750
                    grad[0] = -f001 + f101;
                    grad[1] = -f001 + f011;
                    grad[2] = +f001 - f000;
                }
                else if (dx + dy + dz >= C_<T>(0))
                {
                    // 6572
                    grad[0] = +f111 - f011;
                    grad[1] = +f111 - f101;
                    grad[2] = +f111 - f110;
                }
                else
                {
                    // 0752
                    grad[0] = C_<T>(1, 2) * (-f000 - f011 + f101 + f110);
                    grad[1] = C_<T>(1, 2) * (-f000 + f011 - f101 + f110);
                    grad[2] = C_<T>(1, 2) * (-f000 + f011 + f101 - f110);
                }
            }
            else
            {
                if (dx + dy + dz <= C_<T>(1))
                {
                    // 0134
                    grad[0] = -f000 + f100;
                    grad[1] = -f000 + f010;
                    grad[2] = -f000 + f001;
                }
                else if (dx + dy - dz >= C_<T>(1))
                {
                    // 2316
                    grad[0] = +f110 - f010;
                    grad[1] = +f110 - f100;
                    grad[2] = -f110 + f111;
                }
                else if (dx - dy + dz >= C_<T>(1))
                {
                    // 5461
                    grad[0] = +f101 - f001;
                    grad[1] = -f101 + f111;
                    grad[2] = +f101 - f100;
                }
                else if (-dx + dy + dz >= C_<T>(1))
                {
                    // 7643
                    grad[0] = -f011 + f111;
                    grad[1] = +f011 - f001;
                    grad[2] = +f011 - f010;
                }
                else
                {
                    // 6314
                    grad[0] = C_<T>(1, 2) * (f111 - f010 + f100 - f001);
                    grad[1] = C_<T>(1, 2) * (f111 + f010 - f100 - f001);
                    grad[2] = C_<T>(1, 2) * (f111 - f010 - f100 + f001);
                }
            }

            return grad;
        }

        std::size_t AddVertex(Vertex const& v)
        {
            auto iter = mVMap.find(v);
            if (iter != mVMap.end())
            {
                // Vertex already in map, just return its unique index.
                return iter->second;
            }
            else
            {
                // Vertex not in map, insert it and assign it a unique index.
                std::size_t i = mNextVertex++;
                mVMap.insert(std::make_pair(v, i));
                return i;
            }
        }

        void AddEdge(Vertex const& v0, Vertex const& v1)
        {
            std::size_t i0 = AddVertex(v0);
            std::size_t i1 = AddVertex(v1);
            mESet.insert(Edge(i0, i1));
        }

        void AddTriangle(Vertex const& v0, Vertex const& v1, Vertex const& v2)
        {
            std::size_t i0 = AddVertex(v0);
            std::size_t i1 = AddVertex(v1);
            std::size_t i2 = AddVertex(v2);

            // Nothing to do if triangle already exists.
            Triangle triangle(i0, i1, i2);
            if (mTSet.find(triangle) != mTSet.end())
            {
                return;
            }

            // Prevent double-sided triangles.
            std::swap(triangle.v[1], triangle.v[2]);
            if (mTSet.find(triangle) != mTSet.end())
            {
                return;
            }

            mESet.insert(Edge(i0, i1));
            mESet.insert(Edge(i1, i2));
            mESet.insert(Edge(i2, i0));

            mTSet.insert(triangle);
        }

        // Support for extraction with linear interpolation.
        void ProcessTetrahedron(
            std::int64_t x0, std::int64_t y0, std::int64_t z0, std::int64_t f0,
            std::int64_t x1, std::int64_t y1, std::int64_t z1, std::int64_t f1,
            std::int64_t x2, std::int64_t y2, std::int64_t z2, std::int64_t f2,
            std::int64_t x3, std::int64_t y3, std::int64_t z3, std::int64_t f3)
        {
            std::int64_t xn0{}, yn0{}, zn0{}, d0{};
            std::int64_t xn1{}, yn1{}, zn1{}, d1{};
            std::int64_t xn2{}, yn2{}, zn2{}, d2{};
            std::int64_t xn3{}, yn3{}, zn3{}, d3{};

            if (f0 != 0)
            {
                // convert to case +***
                if (f0 < 0)
                {
                    f0 = -f0;
                    f1 = -f1;
                    f2 = -f2;
                    f3 = -f3;
                }

                if (f1 > 0)
                {
                    if (f2 > 0)
                    {
                        if (f3 > 0)
                        {
                            // ++++
                            return;
                        }
                        else if (f3 < 0)
                        {
                            // +++-
                            d0 = f0 - f3;
                            xn0 = f0 * x3 - f3 * x0;
                            yn0 = f0 * y3 - f3 * y0;
                            zn0 = f0 * z3 - f3 * z0;
                            d1 = f1 - f3;
                            xn1 = f1 * x3 - f3 * x1;
                            yn1 = f1 * y3 - f3 * y1;
                            zn1 = f1 * z3 - f3 * z1;
                            d2 = f2 - f3;
                            xn2 = f2 * x3 - f3 * x2;
                            yn2 = f2 * y3 - f3 * y2;
                            zn2 = f2 * z3 - f3 * z2;
                            AddTriangle(
                                Vertex(xn0, d0, yn0, d0, zn0, d0),
                                Vertex(xn1, d1, yn1, d1, zn1, d1),
                                Vertex(xn2, d2, yn2, d2, zn2, d2));
                        }
                        else
                        {
                            // +++0
                            AddVertex(
                                Vertex(x3, 1, y3, 1, z3, 1));
                        }
                    }
                    else if (f2 < 0)
                    {
                        d0 = f0 - f2;
                        xn0 = f0 * x2 - f2 * x0;
                        yn0 = f0 * y2 - f2 * y0;
                        zn0 = f0 * z2 - f2 * z0;
                        d1 = f1 - f2;
                        xn1 = f1 * x2 - f2 * x1;
                        yn1 = f1 * y2 - f2 * y1;
                        zn1 = f1 * z2 - f2 * z1;

                        if (f3 > 0)
                        {
                            // ++-+
                            d2 = f3 - f2;
                            xn2 = f3 * x2 - f2 * x3;
                            yn2 = f3 * y2 - f2 * y3;
                            zn2 = f3 * z2 - f2 * z3;
                            AddTriangle(
                                Vertex(xn0, d0, yn0, d0, zn0, d0),
                                Vertex(xn1, d1, yn1, d1, zn1, d1),
                                Vertex(xn2, d2, yn2, d2, zn2, d2));
                        }
                        else if (f3 < 0)
                        {
                            // ++--
                            d2 = f0 - f3;
                            xn2 = f0 * x3 - f3 * x0;
                            yn2 = f0 * y3 - f3 * y0;
                            zn2 = f0 * z3 - f3 * z0;
                            d3 = f1 - f3;
                            xn3 = f1 * x3 - f3 * x1;
                            yn3 = f1 * y3 - f3 * y1;
                            zn3 = f1 * z3 - f3 * z1;
                            AddTriangle(
                                Vertex(xn0, d0, yn0, d0, zn0, d0),
                                Vertex(xn1, d1, yn1, d1, zn1, d1),
                                Vertex(xn2, d2, yn2, d2, zn2, d2));
                            AddTriangle(
                                Vertex(xn1, d1, yn1, d1, zn1, d1),
                                Vertex(xn3, d3, yn3, d3, zn3, d3),
                                Vertex(xn2, d2, yn2, d2, zn2, d2));
                        }
                        else
                        {
                            // ++-0
                            AddTriangle(
                                Vertex(xn0, d0, yn0, d0, zn0, d0),
                                Vertex(xn1, d1, yn1, d1, zn1, d1),
                                Vertex(x3, 1, y3, 1, z3, 1));
                        }
                    }
                    else
                    {
                        if (f3 > 0)
                        {
                            // ++0+
                            AddVertex(
                                Vertex(x2, 1, y2, 1, z2, 1));
                        }
                        else if (f3 < 0)
                        {
                            // ++0-
                            d0 = f0 - f3;
                            xn0 = f0 * x3 - f3 * x0;
                            yn0 = f0 * y3 - f3 * y0;
                            zn0 = f0 * z3 - f3 * z0;
                            d1 = f1 - f3;
                            xn1 = f1 * x3 - f3 * x1;
                            yn1 = f1 * y3 - f3 * y1;
                            zn1 = f1 * z3 - f3 * z1;
                            AddTriangle(
                                Vertex(xn0, d0, yn0, d0, zn0, d0),
                                Vertex(xn1, d1, yn1, d1, zn1, d1),
                                Vertex(x2, 1, y2, 1, z2, 1));
                        }
                        else
                        {
                            // ++00
                            AddEdge(
                                Vertex(x2, 1, y2, 1, z2, 1),
                                Vertex(x3, 1, y3, 1, z3, 1));
                        }
                    }
                }
                else if (f1 < 0)
                {
                    if (f2 > 0)
                    {
                        d0 = f0 - f1;
                        xn0 = f0 * x1 - f1 * x0;
                        yn0 = f0 * y1 - f1 * y0;
                        zn0 = f0 * z1 - f1 * z0;
                        d1 = f2 - f1;
                        xn1 = f2 * x1 - f1 * x2;
                        yn1 = f2 * y1 - f1 * y2;
                        zn1 = f2 * z1 - f1 * z2;

                        if (f3 > 0)
                        {
                            // +-++
                            d2 = f3 - f1;
                            xn2 = f3 * x1 - f1 * x3;
                            yn2 = f3 * y1 - f1 * y3;
                            zn2 = f3 * z1 - f1 * z3;
                            AddTriangle(
                                Vertex(xn0, d0, yn0, d0, zn0, d0),
                                Vertex(xn1, d1, yn1, d1, zn1, d1),
                                Vertex(xn2, d2, yn2, d2, zn2, d2));
                        }
                        else if (f3 < 0)
                        {
                            // +-+-
                            d2 = f0 - f3;
                            xn2 = f0 * x3 - f3 * x0;
                            yn2 = f0 * y3 - f3 * y0;
                            zn2 = f0 * z3 - f3 * z0;
                            d3 = f2 - f3;
                            xn3 = f2 * x3 - f3 * x2;
                            yn3 = f2 * y3 - f3 * y2;
                            zn3 = f2 * z3 - f3 * z2;
                            AddTriangle(
                                Vertex(xn0, d0, yn0, d0, zn0, d0),
                                Vertex(xn1, d1, yn1, d1, zn1, d1),
                                Vertex(xn2, d2, yn2, d2, zn2, d2));
                            AddTriangle(
                                Vertex(xn1, d1, yn1, d1, zn1, d1),
                                Vertex(xn3, d3, yn3, d3, zn3, d3),
                                Vertex(xn2, d2, yn2, d2, zn2, d2));
                        }
                        else
                        {
                            // +-+0
                            AddTriangle(
                                Vertex(xn0, d0, yn0, d0, zn0, d0),
                                Vertex(xn1, d1, yn1, d1, zn1, d1),
                                Vertex(x3, 1, y3, 1, z3, 1));
                        }
                    }
                    else if (f2 < 0)
                    {
                        d0 = f1 - f0;
                        xn0 = f1 * x0 - f0 * x1;
                        yn0 = f1 * y0 - f0 * y1;
                        zn0 = f1 * z0 - f0 * z1;
                        d1 = f2 - f0;
                        xn1 = f2 * x0 - f0 * x2;
                        yn1 = f2 * y0 - f0 * y2;
                        zn1 = f2 * z0 - f0 * z2;

                        if (f3 > 0)
                        {
                            // +--+
                            d2 = f1 - f3;
                            xn2 = f1 * x3 - f3 * x1;
                            yn2 = f1 * y3 - f3 * y1;
                            zn2 = f1 * z3 - f3 * z1;
                            d3 = f2 - f3;
                            xn3 = f2 * x3 - f3 * x2;
                            yn3 = f2 * y3 - f3 * y2;
                            zn3 = f2 * z3 - f3 * z2;
                            AddTriangle(
                                Vertex(xn0, d0, yn0, d0, zn0, d0),
                                Vertex(xn1, d1, yn1, d1, zn1, d1),
                                Vertex(xn2, d2, yn2, d2, zn2, d2));
                            AddTriangle(
                                Vertex(xn1, d1, yn1, d1, zn1, d1),
                                Vertex(xn3, d3, yn3, d3, zn3, d3),
                                Vertex(xn2, d2, yn2, d2, zn2, d2));
                        }
                        else if (f3 < 0)
                        {
                            // +---
                            d2 = f3 - f0;
                            xn2 = f3 * x0 - f0 * x3;
                            yn2 = f3 * y0 - f0 * y3;
                            zn2 = f3 * z0 - f0 * z3;
                            AddTriangle(
                                Vertex(xn0, d0, yn0, d0, zn0, d0),
                                Vertex(xn1, d1, yn1, d1, zn1, d1),
                                Vertex(xn2, d2, yn2, d2, zn2, d2));
                        }
                        else
                        {
                            // +--0
                            AddTriangle(
                                Vertex(xn0, d0, yn0, d0, zn0, d0),
                                Vertex(xn1, d1, yn1, d1, zn1, d1),
                                Vertex(x3, 1, y3, 1, z3, 1));
                        }
                    }
                    else
                    {
                        d0 = f1 - f0;
                        xn0 = f1 * x0 - f0 * x1;
                        yn0 = f1 * y0 - f0 * y1;
                        zn0 = f1 * z0 - f0 * z1;

                        if (f3 > 0)
                        {
                            // +-0+
                            d1 = f1 - f3;
                            xn1 = f1 * x3 - f3 * x1;
                            yn1 = f1 * y3 - f3 * y1;
                            zn1 = f1 * z3 - f3 * z1;
                            AddTriangle(
                                Vertex(xn0, d0, yn0, d0, zn0, d0),
                                Vertex(xn1, d1, yn1, d1, zn1, d1),
                                Vertex(x2, 1, y2, 1, z2, 1));
                        }
                        else if (f3 < 0)
                        {
                            // +-0-
                            d1 = f3 - f0;
                            xn1 = f3 * x0 - f0 * x3;
                            yn1 = f3 * y0 - f0 * y3;
                            zn1 = f3 * z0 - f0 * z3;
                            AddTriangle(
                                Vertex(xn0, d0, yn0, d0, zn0, d0),
                                Vertex(xn1, d1, yn1, d1, zn1, d1),
                                Vertex(x2, 1, y2, 1, z2, 1));
                        }
                        else
                        {
                            // +-00
                            AddTriangle(
                                Vertex(xn0, d0, yn0, d0, zn0, d0),
                                Vertex(x2, 1, y2, 1, z2, 1),
                                Vertex(x3, 1, y3, 1, z3, 1));
                        }
                    }
                }
                else
                {
                    if (f2 > 0)
                    {
                        if (f3 > 0)
                        {
                            // +0++
                            AddVertex(
                                Vertex(x1, 1, y1, 1, z1, 1));
                        }
                        else if (f3 < 0)
                        {
                            // +0+-
                            d0 = f0 - f3;
                            xn0 = f0 * x3 - f3 * x0;
                            yn0 = f0 * y3 - f3 * y0;
                            zn0 = f0 * z3 - f3 * z0;
                            d1 = f2 - f3;
                            xn1 = f2 * x3 - f3 * x2;
                            yn1 = f2 * y3 - f3 * y2;
                            zn1 = f2 * z3 - f3 * z2;
                            AddTriangle(
                                Vertex(xn0, d0, yn0, d0, zn0, d0),
                                Vertex(xn1, d1, yn1, d1, zn1, d1),
                                Vertex(x1, 1, y1, 1, z1, 1));
                        }
                        else
                        {
                            // +0+0
                            AddEdge(
                                Vertex(x1, 1, y1, 1, z1, 1),
                                Vertex(x3, 1, y3, 1, z3, 1));
                        }
                    }
                    else if (f2 < 0)
                    {
                        d0 = f2 - f0;
                        xn0 = f2 * x0 - f0 * x2;
                        yn0 = f2 * y0 - f0 * y2;
                        zn0 = f2 * z0 - f0 * z2;

                        if (f3 > 0)
                        {
                            // +0-+
                            d1 = f2 - f3;
                            xn1 = f2 * x3 - f3 * x2;
                            yn1 = f2 * y3 - f3 * y2;
                            zn1 = f2 * z3 - f3 * z2;
                            AddTriangle(
                                Vertex(xn0, d0, yn0, d0, zn0, d0),
                                Vertex(xn1, d1, yn1, d1, zn1, d1),
                                Vertex(x1, 1, y1, 1, z1, 1));
                        }
                        else if (f3 < 0)
                        {
                            // +0--
                            d1 = f0 - f3;
                            xn1 = f0 * x3 - f3 * x0;
                            yn1 = f0 * y3 - f3 * y0;
                            zn1 = f0 * z3 - f3 * z0;
                            AddTriangle(
                                Vertex(xn0, d0, yn0, d0, zn0, d0),
                                Vertex(xn1, d1, yn1, d1, zn1, d1),
                                Vertex(x1, 1, y1, 1, z1, 1));
                        }
                        else
                        {
                            // +0-0
                            AddTriangle(
                                Vertex(xn0, d0, yn0, d0, zn0, d0),
                                Vertex(x1, 1, y1, 1, z1, 1),
                                Vertex(x3, 1, y3, 1, z3, 1));
                        }
                    }
                    else
                    {
                        if (f3 > 0)
                        {
                            // +00+
                            AddEdge(
                                Vertex(x1, 1, y1, 1, z1, 1),
                                Vertex(x2, 1, y2, 1, z2, 1));
                        }
                        else if (f3 < 0)
                        {
                            // +00-
                            d0 = f0 - f3;
                            xn0 = f0 * x3 - f3 * x0;
                            yn0 = f0 * y3 - f3 * y0;
                            zn0 = f0 * z3 - f3 * z0;
                            AddTriangle(
                                Vertex(xn0, d0, yn0, d0, zn0, d0),
                                Vertex(x1, 1, y1, 1, z1, 1),
                                Vertex(x2, 1, y2, 1, z2, 1));
                        }
                        else
                        {
                            // +000
                            AddTriangle(
                                Vertex(x1, 1, y1, 1, z1, 1),
                                Vertex(x2, 1, y2, 1, z2, 1),
                                Vertex(x3, 1, y3, 1, z3, 1));
                        }
                    }
                }
            }
            else if (f1 != 0)
            {
                // convert to case 0+**
                if (f1 < 0)
                {
                    f1 = -f1;
                    f2 = -f2;
                    f3 = -f3;
                }

                if (f2 > 0)
                {
                    if (f3 > 0)
                    {
                        // 0+++
                        AddVertex(
                            Vertex(x0, 1, y0, 1, z0, 1));
                    }
                    else if (f3 < 0)
                    {
                        // 0++-
                        d0 = f2 - f3;
                        xn0 = f2 * x3 - f3 * x2;
                        yn0 = f2 * y3 - f3 * y2;
                        zn0 = f2 * z3 - f3 * z2;
                        d1 = f1 - f3;
                        xn1 = f1 * x3 - f3 * x1;
                        yn1 = f1 * y3 - f3 * y1;
                        zn1 = f1 * z3 - f3 * z1;
                        AddTriangle(
                            Vertex(xn0, d0, yn0, d0, zn0, d0),
                            Vertex(xn1, d1, yn1, d1, zn1, d1),
                            Vertex(x0, 1, y0, 1, z0, 1));
                    }
                    else
                    {
                        // 0++0
                        AddEdge(
                            Vertex(x0, 1, y0, 1, z0, 1),
                            Vertex(x3, 1, y3, 1, z3, 1));
                    }
                }
                else if (f2 < 0)
                {
                    d0 = f2 - f1;
                    xn0 = f2 * x1 - f1 * x2;
                    yn0 = f2 * y1 - f1 * y2;
                    zn0 = f2 * z1 - f1 * z2;

                    if (f3 > 0)
                    {
                        // 0+-+
                        d1 = f2 - f3;
                        xn1 = f2 * x3 - f3 * x2;
                        yn1 = f2 * y3 - f3 * y2;
                        zn1 = f2 * z3 - f3 * z2;
                        AddTriangle(
                            Vertex(xn0, d0, yn0, d0, zn0, d0),
                            Vertex(xn1, d1, yn1, d1, zn1, d1),
                            Vertex(x0, 1, y0, 1, z0, 1));
                    }
                    else if (f3 < 0)
                    {
                        // 0+--
                        d1 = f1 - f3;
                        xn1 = f1 * x3 - f3 * x1;
                        yn1 = f1 * y3 - f3 * y1;
                        zn1 = f1 * z3 - f3 * z1;
                        AddTriangle(
                            Vertex(xn0, d0, yn0, d0, zn0, d0),
                            Vertex(xn1, d1, yn1, d1, zn1, d1),
                            Vertex(x0, 1, y0, 1, z0, 1));
                    }
                    else
                    {
                        // 0+-0
                        AddTriangle(
                            Vertex(xn0, d0, yn0, d0, zn0, d0),
                            Vertex(x0, 1, y0, 1, z0, 1),
                            Vertex(x3, 1, y3, 1, z3, 1));
                    }
                }
                else
                {
                    if (f3 > 0)
                    {
                        // 0+0+
                        AddEdge(
                            Vertex(x0, 1, y0, 1, z0, 1),
                            Vertex(x2, 1, y2, 1, z2, 1));
                    }
                    else if (f3 < 0)
                    {
                        // 0+0-
                        d0 = f1 - f3;
                        xn0 = f1 * x3 - f3 * x1;
                        yn0 = f1 * y3 - f3 * y1;
                        zn0 = f1 * z3 - f3 * z1;
                        AddTriangle(
                            Vertex(xn0, d0, yn0, d0, zn0, d0),
                            Vertex(x0, 1, y0, 1, z0, 1),
                            Vertex(x2, 1, y2, 1, z2, 1));
                    }
                    else
                    {
                        // 0+00
                        AddTriangle(
                            Vertex(x0, 1, y0, 1, z0, 1),
                            Vertex(x2, 1, y2, 1, z2, 1),
                            Vertex(x3, 1, y3, 1, z3, 1));
                    }
                }
            }
            else if (f2 != 0)
            {
                // convert to case 00+*
                if (f2 < 0)
                {
                    f2 = -f2;
                    f3 = -f3;
                }

                if (f3 > 0)
                {
                    // 00++
                    AddEdge(
                        Vertex(x0, 1, y0, 1, z0, 1),
                        Vertex(x1, 1, y1, 1, z1, 1));
                }
                else if (f3 < 0)
                {
                    // 00+-
                    d0 = f2 - f3;
                    xn0 = f2 * x3 - f3 * x2;
                    yn0 = f2 * y3 - f3 * y2;
                    zn0 = f2 * z3 - f3 * z2;
                    AddTriangle(
                        Vertex(xn0, d0, yn0, d0, zn0, d0),
                        Vertex(x0, 1, y0, 1, z0, 1),
                        Vertex(x1, 1, y1, 1, z1, 1));
                }
                else
                {
                    // 00+0
                    AddTriangle(
                        Vertex(x0, 1, y0, 1, z0, 1),
                        Vertex(x1, 1, y1, 1, z1, 1),
                        Vertex(x3, 1, y3, 1, z3, 1));
                }
            }
            else if (f3 != 0)
            {
                // cases 000+ or 000-
                AddTriangle(
                    Vertex(x0, 1, y0, 1, z0, 1),
                    Vertex(x1, 1, y1, 1, z1, 1),
                    Vertex(x2, 1, y2, 1, z2, 1));
            }
            else
            {
                // case 0000
                AddTriangle(
                    Vertex(x0, 1, y0, 1, z0, 1),
                    Vertex(x1, 1, y1, 1, z1, 1),
                    Vertex(x2, 1, y2, 1, z2, 1));
                AddTriangle(
                    Vertex(x0, 1, y0, 1, z0, 1),
                    Vertex(x1, 1, y1, 1, z1, 1),
                    Vertex(x3, 1, y3, 1, z3, 1));
                AddTriangle(
                    Vertex(x0, 1, y0, 1, z0, 1),
                    Vertex(x2, 1, y2, 1, z2, 1),
                    Vertex(x3, 1, y3, 1, z3, 1));
                AddTriangle(
                    Vertex(x1, 1, y1, 1, z1, 1),
                    Vertex(x2, 1, y2, 1, z2, 1),
                    Vertex(x3, 1, y3, 1, z3, 1));
            }
        }

        std::map<Vertex, std::size_t> mVMap;
        std::set<Edge> mESet;
        std::set<Triangle> mTSet;
        std::int32_t mNextVertex;

    private:
        friend class UnitTestSurfaceExtractorTetrahedra;
    };
}
