// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// A reader/writer for binary STL files. The file format is described at
// https://en.wikipedia.org/wiki/STL_(file_format).
//
// The class Tuple3 must represent 3 contiguous float-valued numbers where
// 'float' is an IEEE 32-bit floating-point number. The Triangle normal and
// vertex members are initialized by the Triangle constructor only if the
// Tuple3 default constructor does so.

#include <array>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

namespace gtl
{
    template <typename Tuple3>
    struct STLBinaryFile
    {
        struct Triangle
        {
            Triangle()
                :
                normal{},
                vertex{},
                attributeByteCount(0)
            {
            }

            Tuple3 normal;
            std::array<Tuple3, 3> vertex;
            std::uint16_t attributeByteCount;
        };

        STLBinaryFile()
            :
            header{},
            triangles{}
        {
            header.fill(0);
        }

        bool Load(std::string const& filename)
        {
            std::ifstream input(filename, std::ios::binary);
            if (!input)
            {
                return false;
            }

            if (input.read((char*)header.data(), header.size()).fail())
            {
                return false;
            }

            std::uint32_t numTriangles = 0;
            if (input.read((char*)&numTriangles, sizeof(numTriangles)).fail())
            {
                return false;
            }

            triangles.resize(static_cast<std::size_t>(numTriangles));
            for (auto& triangle : triangles)
            {
                if (input.read((char*)&triangle.normal, sizeof(triangle.normal)).fail())
                {
                    return false;
                }

                if (input.read((char*)triangle.vertex.data(), triangle.vertex.size() * sizeof(triangle.vertex[0])).fail())
                {
                    return false;
                }

                if (input.read((char*)&triangle.attributeByteCount, sizeof(triangle.attributeByteCount)).fail())
                {
                    return false;
                }
            }

            input.close();
            return true;
        }

        // The caller is responsible for populating the 'header' and
        // 'triangles' members of the STLBinaryFile object.
        bool Save(std::string const& filename)
        {
            std::ofstream output(filename, std::ios::binary);
            if (!output)
            {
                return false;
            }

            if (output.write((char const*)header.data(), header.size()).fail())
            {
                return false;
            }

            std::uint32_t numTriangles = static_cast<std::uint32_t>(triangles.size());
            if (output.write((char const*)&numTriangles, sizeof(numTriangles)).fail())
            {
                return false;
            }

            for (auto const& triangle : triangles)
            {
                if (output.write((char const*)&triangle.normal, sizeof(triangle.normal)).fail())
                {
                    return false;
                }

                if (output.write((char const*)triangle.vertex.data(), triangle.vertex.size() * sizeof(triangle.vertex[0])).fail())
                {
                    return false;
                }

                if (output.write((char const*)&triangle.attributeByteCount, sizeof(triangle.attributeByteCount)).fail())
                {
                    return false;
                }
            }

            output.close();
            return true;
        }

        std::array<std::uint8_t, 80> header;
        std::vector<Triangle> triangles;
    };
}
