// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// Support for drawing pixels in a 2D rectangular lattice.

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
    class Rasterize2
    {
    public:
        using UInteger = std::make_unsigned<SInteger>;
        using Callback = std::function<void(SInteger, SInteger)>;

        // Visit a single pixel at (x,y).
        static void DrawPixel(SInteger x, SInteger y, Callback const& F)
        {
            static_assert(
                std::is_same<SInteger, std::int32_t>::value ||
                std::is_same<SInteger, std::int64_t>::value,
                "SInteger must be std::int32_t or std::int64_t.");

            F(x, y);
        }

        // Visit pixels in a (2 * thick + 1)^2 square centered at (x,y). If
        // thick is a negative number, no pixels are drawn.
        static void DrawThickPixel(SInteger x, SInteger y, SInteger thick,
            Callback const& F)
        {
            static_assert(
                std::is_same<SInteger, std::int32_t>::value ||
                std::is_same<SInteger, std::int64_t>::value,
                "SInteger must be std::int32_t or std::int64_t.");

            for (SInteger dy = -thick; dy <= thick; ++dy)
            {
                for (SInteger dx = -thick; dx <= thick; ++dx)
                {
                    F(x + dx, y + dy);
                }
            }
        }

        // Visit pixels using Bresenham's line drawing algorithm.
        static void DrawLine(SInteger x0, SInteger y0, SInteger x1, SInteger y1,
            Callback const& F)
        {
            static_assert(
                std::is_same<SInteger, std::int32_t>::value ||
                std::is_same<SInteger, std::int64_t>::value,
                "SInteger must be std::int32_t or std::int64_t.");

            // Starting point of line.
            SInteger x = x0, y = y0;

            // Direction of line.
            SInteger dx = x1 - x0, dy = y1 - y0;

            // Increment or decrement depending on direction of line.
            SInteger sx = (dx > 0 ? 1 : (dx < 0 ? -1 : 0));
            SInteger sy = (dy > 0 ? 1 : (dy < 0 ? -1 : 0));

            // Decision parameters for pixel selection.
            if (dx < 0)
            {
                dx = -dx;
            }
            if (dy < 0)
            {
                dy = -dy;
            }
            SInteger ax = 2 * dx, ay = 2 * dy;
            SInteger decX = 0, decY = 0;

            // Determine largest direction component, single-step related
            // variable.
            std::uint32_t var = (dy <= dx ? 0 : 1);

            // Traverse the line segment.
            switch (var)
            {
            case 0:
                // Single-step in x-direction.
                decY = ay - dx;
                for (; ; x += sx, decY += ay)
                {
                    F(x, y);

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
                }
                break;
            case 1:
                // Single-step in y-direction.
                decX = ax - dy;
                for (; ; y += sy, decX += ax)
                {
                    F(x, y);

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
                }
                break;
            }
        }

        // Visit pixels using Bresenham's circle drawing algorithm. Set
        // 'solid' to false for drawing only the circle. Set 'solid' to true
        // to draw all pixels on and inside the circle.
        static void DrawCircle(SInteger xCenter, SInteger yCenter,
            SInteger radius, bool solid, Callback const& F)
        {
            static_assert(
                std::is_same<SInteger, std::int32_t>::value ||
                std::is_same<SInteger, std::int64_t>::value,
                "SInteger must be std::int32_t or std::int64_t.");

            if (solid)
            {
                for (SInteger x = 0, y = radius, dec = 3 - 2 * radius; x <= y; ++x)
                {
                    SInteger xValue = xCenter + x;
                    SInteger yMin = yCenter - y;
                    SInteger yMax = yCenter + y;
                    for (SInteger i = yMin; i <= yMax; ++i)
                    {
                        F(xValue, i);
                    }

                    xValue = xCenter - x;
                    for (SInteger i = yMin; i <= yMax; ++i)
                    {
                        F(xValue, i);
                    }

                    xValue = xCenter + y;
                    yMin = yCenter - x;
                    yMax = yCenter + x;
                    for (SInteger i = yMin; i <= yMax; ++i)
                    {
                        F(xValue, i);
                    }

                    xValue = xCenter - y;
                    for (SInteger i = yMin; i <= yMax; ++i)
                    {
                        F(xValue, i);
                    }

                    if (dec >= 0)
                    {
                        dec += -4 * (y--) + 4;
                    }
                    dec += 4 * x + 6;
                }
            }
            else
            {
                for (SInteger x = 0, y = radius, dec = 3 - 2 * radius; x <= y; ++x)
                {
                    F(xCenter + x, yCenter + y);
                    F(xCenter + x, yCenter - y);
                    F(xCenter - x, yCenter + y);
                    F(xCenter - x, yCenter - y);
                    F(xCenter + y, yCenter + x);
                    F(xCenter + y, yCenter - x);
                    F(xCenter - y, yCenter + x);
                    F(xCenter - y, yCenter - x);

                    if (dec >= 0)
                    {
                        dec += -4 * (y--) + 4;
                    }
                    dec += 4 * x + 6;
                }
            }
        }

        // Visit pixels in a rectangle of the specified dimensions. Set
        // 'solid' to false for drawing only the rectangle. Set 'solid' to
        // true to draw all pixels on and inside the rectangle.
        static void DrawRectangle(SInteger xMin, SInteger yMin, SInteger xMax,
            SInteger yMax, bool solid, Callback const& F)
        {
            static_assert(
                std::is_same<SInteger, std::int32_t>::value ||
                std::is_same<SInteger, std::int64_t>::value,
                "SInteger must be std::int32_t or std::int64_t.");

            if (solid)
            {
                for (SInteger y = yMin; y <= yMax; ++y)
                {
                    for (SInteger x = xMin; x <= xMax; ++x)
                    {
                        F(x, y);
                    }
                }
            }
            else
            {
                for (SInteger x = xMin; x <= xMax; ++x)
                {
                    F(x, yMin);
                    F(x, yMax);
                }
                for (SInteger y = yMin + 1; y <= yMax - 1; ++y)
                {
                    F(xMin, y);
                    F(xMax, y);
                }
            }
        }

        // Visit the pixels using Bresenham's algorithm for the axis-aligned
        // ellipse ((x-xc)/a)^2 + ((y-yc)/b)^2 = 1, where xCenter is xc,
        // yCenter is yc, xExtent is a and yExtent is b.
        static void DrawEllipse(SInteger xCenter, SInteger yCenter,
            SInteger xExtent, SInteger yExtent, Callback const& F)
        {
            static_assert(
                std::is_same<SInteger, std::int32_t>::value ||
                std::is_same<SInteger, std::int64_t>::value,
                "SInteger must be std::int32_t or std::int64_t.");

            SInteger xExtSqr = xExtent * xExtent;
            SInteger yExtSqr = yExtent * yExtent;

            SInteger x = 0;
            SInteger y = yExtent;
            SInteger dec = 2 * yExtSqr + xExtSqr * (1 - 2 * yExtent);
            for (; yExtSqr * x <= xExtSqr * y; ++x)
            {
                F(xCenter + x, yCenter + y);
                F(xCenter - x, yCenter + y);
                F(xCenter + x, yCenter - y);
                F(xCenter - x, yCenter - y);

                if (dec >= 0)
                {
                    dec += 4 * xExtSqr * (1 - y);
                    --y;
                }
                dec += yExtSqr * (4 * x + 6);
            }
            if (y == 0 && x < xExtent)
            {
                // The discretization caused us to reach the y-axis before the
                // x-values reached the ellipse vertices.  Draw a solid line
                // along the x-axis to those vertices.
                for (; x <= xExtent; ++x)
                {
                    F(xCenter + x, yCenter);
                    F(xCenter - x, yCenter);
                }
                return;
            }

            x = xExtent;
            y = 0;
            dec = 2 * xExtSqr + yExtSqr * (1 - 2 * xExtent);
            for (; xExtSqr * y <= yExtSqr * x; ++y)
            {
                F(xCenter + x, yCenter + y);
                F(xCenter - x, yCenter + y);
                F(xCenter + x, yCenter - y);
                F(xCenter - x, yCenter - y);

                if (dec >= 0)
                {
                    dec += 4 * yExtSqr * (1 - x);
                    --x;
                }
                dec += xExtSqr * (4 * y + 6);
            }
            if (x == 0 && y < yExtent)
            {
                // The discretization caused us to reach the x-axis before the
                // y-values reached the ellipse vertices.  Draw a solid line
                // along the y-axis to those vertices.
                for (; y <= yExtent; ++y)
                {
                    F(xCenter, yCenter + y);
                    F(xCenter, yCenter - y);
                }
            }
        }

        // Use a depth-first search for filling a 4-connected region. This is
        // nonrecursive, simulated by using a heap-allocated stack. The input
        // (x,y) is the seed point that starts the fill. The x-value is in
        // {0..xSize-1} and the y-value is in {0..ySize-1}.
        template <typename PixelType>
        static void DrawFloodFill4(SInteger x, SInteger y, SInteger xSize, SInteger ySize,
            PixelType foreground, PixelType background,
            std::function<void(SInteger, SInteger, PixelType)> const& setCallback,
            std::function<PixelType(SInteger, SInteger)> const& getCallback)
        {
            static_assert(
                std::is_same<SInteger, std::int32_t>::value ||
                std::is_same<SInteger, std::int64_t>::value,
                "SInteger must be std::int32_t or std::int64_t.");

            // Test for a valid seed.
            if (x < 0 || x >= xSize || y < 0 || y >= ySize)
            {
                // The seed point is outside the image domain, so nothing to
                // fill.
                return;
            }

            // Allocate the maximum amount of space needed for the stack.
            // An empty stack has top = std::numeric_limits<std::size_t>::max().
            std::size_t const sxSize = static_cast<std::size_t>(xSize);
            std::size_t const sySize = static_cast<std::size_t>(ySize);
            std::vector<std::array<SInteger, 2>> stack(sxSize * sySize);

            // Push seed point onto stack if it is background. All points
            // pushed onto stack are background.
            std::size_t top = 0;
            std::array<SInteger, 2> point{ x, y }, neighbor{ 0, 0 };
            stack[top] = point;

            while (top != std::numeric_limits<std::size_t>::max())
            {
                // Read top of stack. Do not pop since we need to return to
                // this top value later to restart the fill in a different
                // direction.
                point = stack[top];

                // Visit the pixel (x,y).
                setCallback(point[0], point[1], foreground);

                neighbor = { point[0] + 1, point[1] };
                if (neighbor[0] < xSize &&
                    getCallback(neighbor[0], neighbor[1]) == background)
                {
                    // Push background pixel.
                    stack[++top] = neighbor;
                    continue;
                }

                neighbor = { point[0] - 1, point[1] };
                if (0 <= neighbor[0] &&
                    getCallback(neighbor[0], neighbor[1]) == background)
                {
                    // Push background pixel.
                    stack[++top] = neighbor;
                    continue;
                }

                neighbor = { point[0], point[1] + 1 };
                if (neighbor[1] < ySize &&
                    getCallback(neighbor[0], neighbor[1]) == background)
                {
                    // Push background pixel.
                    stack[++top] = neighbor;
                    continue;
                }

                neighbor = { point[0], point[1] - 1 };
                if (0 <= neighbor[1] &&
                    getCallback(neighbor[0], neighbor[1]) == background)
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
        friend class UnitTestRasterize2;
    };
}
