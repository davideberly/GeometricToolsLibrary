// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

#pragma once

// Abstract base class for CurveExtractorSquares and CurveExtractorTriangles.

#include <GTL/Utility/Exceptions.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <type_traits>
#include <utility>
#include <vector>

namespace gtl
{
    // The image type IntType must be one of the integer types: std::int8_t,
    // std::int16_t, std::int32_t, std::uint8_t, std::uint16_t or
    // std::uint32_t. Internal integer computations are performed using
    // std::int64_t. The type T is for extraction to floating-point vertices.
    template <typename IntType, typename T>
    class CurveExtractor
    {
    public:
        // Abstract base class.
        virtual ~CurveExtractor() = default;

        // The level curves form a graph of vertices and edges. The vertices
        // are computed as pairs of nonnegative rational numbers. Vertex
        // represents the rational pair (xNumer/xDenom, yNumer/yDenom) as
        // (xNumer, xDenom, yNumer, yDenom), where all components are
        // nonnegative. The edges connect pairs of vertices, forming a graph
        // that represents the level set.
        struct Vertex
        {
            Vertex()
                :
                xNumer(0),
                xDenom(0),
                yNumer(0),
                yDenom(0)
            {
            }

            Vertex(std::int64_t inXNumer, std::int64_t inXDenom, std::int64_t inYNumer, std::int64_t inYDenom)
                :
                Vertex{}
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
            }

            // The non-default constructor guarantees that xDenom > 0 and
            // yDenom > 0. The following comparison operators assume that
            // the denominators are positive.
            bool operator==(Vertex const& other) const
            {
                return
                    // xn0 / xd0 == xn1 / xd1
                    xNumer * other.xDenom == other.xNumer * xDenom
                    &&
                    // yn0/yd0 == yn1/yd1
                    yNumer * other.yDenom == other.yNumer * yDenom;
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
                // yn0/yd0 < yn1/yd1
                return yn0tyd1 < yn1tyd0;
            }

            std::int64_t xNumer, xDenom, yNumer, yDenom;
        };

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

        // Extract level curves and return rational vertices.
        virtual void Extract(IntType level, std::vector<Vertex>& vertices,
            std::vector<Edge>& edges) = 0;

        void Extract(IntType level, bool removeDuplicateVertices,
            std::vector<std::array<T, 2>>& vertices, std::vector<Edge>& edges)
        {
            std::vector<Vertex> rationalVertices{};
            Extract(level, rationalVertices, edges);
            if (removeDuplicateVertices)
            {
                MakeUnique(rationalVertices, edges);
            }
            Convert(rationalVertices, vertices);
        }

        // The extraction has duplicate vertices on edges shared by pixels.
        // This function will eliminate the duplicates.
        void MakeUnique(std::vector<Vertex>& vertices, std::vector<Edge>& edges)
        {
            std::size_t numVertices = vertices.size();
            std::size_t numEdges = edges.size();
            if (numVertices == 0 || numEdges == 0)
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

            // Compute the map of unique edges and assign to them new and
            // unique indices.
            std::map<Edge, std::size_t> emap{};
            std::size_t nextEdge = 0;
            for (std::size_t e = 0; e < numEdges; ++e)
            {
                // Replace old vertex indices by new vertex indices.
                Edge& edge = edges[e];
                for (std::size_t i = 0; i < 2; ++i)
                {
                    auto iter = vmap.find(vertices[edge.v[i]]);
                    GTL_RUNTIME_ASSERT(
                        iter != vmap.end(),
                        "Expecting the vertex to be in the vmap.");
                    edge.v[i] = iter->second;
                }

                // Keep only unique edges.
                auto result = emap.insert(std::make_pair(edge, nextEdge));
                if (result.second)
                {
                    ++nextEdge;
                }
            }

            // Pack the vertices into an array.
            vertices.resize(vmap.size());
            for (auto const& element : vmap)
            {
                vertices[element.second] = element.first;
            }

            // Pack the edges into an array.
            edges.resize(emap.size());
            for (auto const& element : emap)
            {
                edges[element.second] = element.first;
            }
        }

        // Convert from Vertex to std::array<T, 2> rationals.
        void Convert(std::vector<Vertex> const& input, std::vector<std::array<T, 2>>& output)
        {
            output.resize(input.size());
            for (std::size_t i = 0; i < input.size(); ++i)
            {
                T rxNumer = static_cast<T>(input[i].xNumer);
                T rxDenom = static_cast<T>(input[i].xDenom);
                T ryNumer = static_cast<T>(input[i].yNumer);
                T ryDenom = static_cast<T>(input[i].yDenom);
                output[i][0] = rxNumer / rxDenom;
                output[i][1] = ryNumer / ryDenom;
            }
        }

    protected:
        // The input is a 2D image with lexicographically ordered pixels (x,y)
        // stored in a linear array. Pixel (x,y) is stored in the array at
        // location index = x + xBound * y. The inputs xBound and yBound must
        // each be 2 or larger so that there is at least one image square to
        // process. The inputPixels must be nonnull and point to contiguous
        // storage that contains at least xBound * yBound elements.
        CurveExtractor(std::size_t xBound, std::size_t yBound, IntType const* inputPixels)
            :
            mXBound(xBound),
            mYBound(yBound),
            mInputPixels(inputPixels)
        {
            static_assert(
                std::is_integral<IntType>::value && sizeof(IntType) <= 4,
                "Type IntType must be std::int32_t{8,16,32}_t or uint{8,16,32}_t.");

            GTL_ARGUMENT_ASSERT(
                mXBound > 1 && mYBound > 1 && mInputPixels != nullptr,
                "Invalid input.");

            mPixels.resize(mXBound * mYBound);
        }

        void AddVertex(std::vector<Vertex>& vertices,
            std::int64_t xNumer, std::int64_t xDenom, std::int64_t yNumer, std::int64_t yDenom)
        {
            vertices.push_back(Vertex(xNumer, xDenom, yNumer, yDenom));
        }

        void AddEdge(std::vector<Vertex>& vertices, std::vector<Edge>& edges,
            std::int64_t xNumer0, std::int64_t xDenom0, std::int64_t yNumer0, std::int64_t yDenom0,
            std::int64_t xNumer1, std::int64_t xDenom1, std::int64_t yNumer1, std::int64_t yDenom1)
        {
            std::size_t v0 = vertices.size();
            std::size_t v1 = v0 + 1;
            edges.push_back(Edge(v0, v1));
            vertices.push_back(Vertex(xNumer0, xDenom0, yNumer0, yDenom0));
            vertices.push_back(Vertex(xNumer1, xDenom1, yNumer1, yDenom1));
        }

        std::size_t mXBound, mYBound;
        IntType const* mInputPixels;
        std::vector<std::int64_t> mPixels;

    private:
        friend class UnitTestCurveExtractor;
    };
}
