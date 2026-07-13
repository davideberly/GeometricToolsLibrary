// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <type_traits>
#include <vector>

namespace gtl
{
    // The parameter SInteger must be either std::int32_t or std::int64_t.
    template <typename SInteger>
    class Rasterize3
    {
    public:
        using UInteger = std::make_unsigned<SInteger>;
        using Callback = std::function<void(SInteger, SInteger, SInteger)>;

        // Visit a single voxel at (x,y,z).
        static void DrawVoxel(SInteger x, SInteger y, SInteger z, Callback const& F)
        {
            static_assert(
                std::is_same<SInteger, std::int32_t>::value ||
                std::is_same<SInteger, std::int64_t>::value,
                "SInteger must be std::int32_t or std::int64_t.");

            F(x, y, z);
        }

        // Visit voxels in a (2 * thick + 1)^3 cube centered at (x,y,z). If
        // thick is a negative number, no voxels are drawn.
        static void DrawThickVoxel(SInteger x, SInteger y, SInteger z, SInteger thick,
            Callback const& F)
        {
            static_assert(
                std::is_same<SInteger, std::int32_t>::value ||
                std::is_same<SInteger, std::int64_t>::value,
                "SInteger must be std::int32_t or std::int64_t.");

            for (SInteger dz = -thick; dz <= thick; ++dz)
            {
                for (SInteger dy = -thick; dy <= thick; ++dy)
                {
                    for (SInteger dx = -thick; dx <= thick; ++dx)
                    {
                        F(x + dx, y + dy, z + dz);
                    }
                }
            }
        }

        // Visit voxels using Bresenham's line drawing algorithm.
        static void DrawLine(SInteger x0, SInteger y0, SInteger z0,
            SInteger x1, SInteger y1, SInteger z1, Callback const& F)
        {
            static_assert(
                std::is_same<SInteger, std::int32_t>::value ||
                std::is_same<SInteger, std::int64_t>::value,
                "SInteger must be std::int32_t or std::int64_t.");

            // Starting point of line.
            SInteger x = x0, y = y0, z = z0;

            // Direction of line.
            SInteger dx = x1 - x0, dy = y1 - y0, dz = z1 - z0;

            // Increment or decrement depending on direction of line.
            SInteger sx = (dx > 0 ? 1 : (dx < 0 ? -1 : 0));
            SInteger sy = (dy > 0 ? 1 : (dy < 0 ? -1 : 0));
            SInteger sz = (dz > 0 ? 1 : (dz < 0 ? -1 : 0));

            // Decision parameters for voxel selection.
            if (dx < 0)
            {
                dx = -dx;
            }
            if (dy < 0)
            {
                dy = -dy;
            }
            if (dz < 0)
            {
                dz = -dz;
            }
            SInteger ax = 2 * dx, ay = 2 * dy, az = 2 * dz;
            SInteger decX = 0, decY = 0, decZ = 0;

            // Determine largest direction component, single-step related
            // variable.
            SInteger maxValue = dx;
            std::uint32_t var = 0;
            if (dy > maxValue)
            {
                maxValue = dy;
                var = 1;
            }
            if (dz > maxValue)
            {
                var = 2;
            }

            // Traverse Bresenham line.
            switch (var)
            {
            case 0:
                // Single-step in x-direction.
                decY = ay - dx;
                decZ = az - dx;
                for (; ; x += sx, decY += ay, decZ += az)
                {
                    // Process voxel.
                    F(x, y, z);

                    // Take Bresenham step.
                    if (x == x1)
                    {
                        break;
                    }
                    if (decY >= 0)
                    {
                        decY -= ax;
                        y += sy;
                    }
                    if (decZ >= 0)
                    {
                        decZ -= ax;
                        z += sz;
                    }
                }
                break;
            case 1:
                // Single-step in y-direction.
                decX = ax - dy;
                decZ = az - dy;
                for (; ; y += sy, decX += ax, decZ += az)
                {
                    // Process voxel.
                    F(x, y, z);

                    // Take Bresenham step.
                    if (y == y1)
                    {
                        break;
                    }
                    if (decX >= 0)
                    {
                        decX -= ay;
                        x += sx;
                    }
                    if (decZ >= 0)
                    {
                        decZ -= ay;
                        z += sz;
                    }
                }
                break;
            case 2:
                // Single-step in z-direction.
                decX = ax - dz;
                decY = ay - dz;
                for (; ; z += sz, decX += ax, decY += ay)
                {
                    // Process voxel.
                    F(x, y, z);

                    // Take Bresenham step.
                    if (z == z1)
                    {
                        break;
                    }
                    if (decX >= 0)
                    {
                        decX -= az;
                        x += sx;
                    }
                    if (decY >= 0)
                    {
                        decY -= az;
                        y += sy;
                    }
                }
                break;
            }
        }

        // Visit voxels in a box of the specified dimensions. Set 'solid'
        // to false for drawing only the box. Set 'solid' to true to draw
        // all pixels on and inside the rectangle.
        static void DrawBox(SInteger xMin, SInteger yMin, SInteger xMax,
            SInteger yMax, SInteger zMin, SInteger zMax, bool solid, Callback const& F)
        {
            static_assert(
                std::is_same<SInteger, std::int32_t>::value ||
                std::is_same<SInteger, std::int64_t>::value,
                "SInteger must be std::int32_t or std::int64_t.");

            if (solid)
            {
                for (SInteger z = zMin; z <= zMax; ++z)
                {
                    for (SInteger y = yMin; y <= yMax; ++y)
                    {
                        for (SInteger x = xMin; x <= xMax; ++x)
                        {
                            F(x, y, z);
                        }
                    }
                }
            }
            else
            {
                // Draw faces z = zMin and z = zMax.
                for (SInteger y = yMin; y <= yMax; ++y)
                {
                    for (SInteger x = xMin; x <= xMax; ++x)
                    {
                        F(x, y, zMin);
                        F(x, y, zMax);
                    }
                }

                // Draw faces y = yMin and y = yMax.
                for (SInteger z = zMin + 1; z < zMax - 1; ++z)
                {
                    for (SInteger x = xMin; x <= xMax; ++x)
                    {
                        F(x, yMin, z);
                        F(x, yMax, z);
                    }
                }

                // Draw faces x = xMin and x = xMax.
                for (SInteger z = zMin + 1; z < zMax - 1; ++zMax)
                {
                    for (SInteger y = yMin + 1; y < yMax - 1; ++yMax)
                    {
                        F(xMin, y, z);
                        F(xMax, y, z);
                    }
                }
            }
        }

        // Use a depth-first search for filling a 6-connected region. This is
        // nonrecursive, simulated by using a heap-allocated stack. The input
        // (x,y,z) is the seed point that starts the fill. The x-value is in
        // {0..xSize-1}, the y-value is in {0..ySize-1} and the z-value is in
        // {0..zSize-1}.
        template <typename VoxelType>
        static void DrawFloodFill6(SInteger x, SInteger y, SInteger z,
            SInteger xSize, SInteger ySize, SInteger zSize,
            VoxelType foreground, VoxelType background,
            std::function<void(SInteger, SInteger, SInteger, VoxelType)> const& setCallback,
            std::function<VoxelType(SInteger, SInteger, SInteger)> const& getCallback)
        {
            static_assert(
                std::is_same<SInteger, std::int32_t>::value ||
                std::is_same<SInteger, std::int64_t>::value,
                "SInteger must be std::int32_t or std::int64_t.");

            // Test for a valid seed.
            if (x < 0 || x >= xSize || y < 0 || y >= ySize || z < 0 || z >= zSize)
            {
                // The seed point is outside the image domain, so nothing to
                // fill.
                return;
            }

            // Allocate the maximum amount of space needed for the stack.
            // An empty stack has top = std::numeric_limits<std::size_t>::max().
            std::size_t const sxSize = static_cast<std::size_t>(xSize);
            std::size_t const sySize = static_cast<std::size_t>(ySize);
            std::size_t const szSize = static_cast<std::size_t>(zSize);
            std::vector<std::array<SInteger, 3>> stack(sxSize * sySize * szSize);

            // Push seed point onto stack if it is background. All points
            // pushed onto stack are background.
            std::size_t top = 0;
            std::array<SInteger, 3> point{ x, y, z }, neighbor{ 0, 0, 0 };
            stack[top] = point;

            while (top != std::numeric_limits<std::size_t>::max())
            {
                // Read top of stack. Do not pop since we need to return to
                // this top value later to restart the fill in a different
                // direction.
                point = stack[top];

                // Visit the pixel (x,y,z).
                setCallback(point[0], point[1], point[2], foreground);

                neighbor = { point[0] + 1, point[1], point[2] };
                if (neighbor[0] < xSize &&
                    getCallback(neighbor[0], neighbor[1], neighbor[2]) == background)
                {
                    // Push background pixel.
                    stack[++top] = neighbor;
                    continue;
                }

                neighbor = { point[0] - 1, point[1], point[2] };
                if (0 <= neighbor[0] &&
                    getCallback(neighbor[0], neighbor[1], neighbor[2]) == background)
                {
                    // Push background pixel.
                    stack[++top] = neighbor;
                    continue;
                }

                neighbor = { point[0], point[1] + 1, point[2] };
                if (neighbor[1] < ySize &&
                    getCallback(neighbor[0], neighbor[1], neighbor[2]) == background)
                {
                    // Push background pixel.
                    stack[++top] = neighbor;
                    continue;
                }

                neighbor = { point[0], point[1] - 1, point[2] };
                if (0 <= neighbor[1] &&
                    getCallback(neighbor[0], neighbor[1], neighbor[2]) == background)
                {
                    // Push background pixel.
                    stack[++top] = neighbor;
                    continue;
                }

                neighbor = { point[0], point[1], point[2] + 1 };
                if (neighbor[2] < zSize &&
                    getCallback(neighbor[0], neighbor[1], neighbor[2]) == background)
                {
                    // Push background pixel.
                    stack[++top] = neighbor;
                    continue;
                }

                neighbor = { point[0], point[1], point[2] - 1 };
                if (0 <= neighbor[2] &&
                    getCallback(neighbor[0], neighbor[1], neighbor[2]) == background)
                {
                    // Push background pixel.
                    stack[++top] = neighbor;
                    continue;
                }

                // Done in all directions, pop and return to search other
                // directions of the predecessor.
                --top;
            }
        }

    private:
        friend class UnitTestRasterize3;
    };
}
