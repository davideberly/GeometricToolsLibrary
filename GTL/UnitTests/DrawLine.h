// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#pragma once

#include <cstdint>
#include <functional>

inline void DrawLine(std::int32_t x0, std::int32_t y0, std::int32_t x1, std::int32_t y1,
    std::function<void(std::int32_t, std::int32_t)> const& DrawFunction)
{
    // Starting point of line.
    std::int32_t x = x0, y = y0;

    // Direction of line.
    std::int32_t dx = x1 - x0, dy = y1 - y0;

    // Increment or decrement depending on direction of line.
    std::int32_t sx = (dx > 0 ? 1 : (dx < 0 ? -1 : 0));
    std::int32_t sy = (dy > 0 ? 1 : (dy < 0 ? -1 : 0));

    // Decision parameters for pixel selection.
    if (dx < 0)
    {
        dx = -dx;
    }
    if (dy < 0)
    {
        dy = -dy;
    }
    std::int32_t ax = 2 * dx, ay = 2 * dy;
    std::int32_t decX, decY;

    // Determine largest direction component, single-step related
    // variable.
    std::int32_t maxValue = dx, var = 0;
    if (dy > maxValue)
    {
        var = 1;
    }

    // Traverse Bresenham line.
    switch (var)
    {
    case 0:  // Single-step in x-direction.
        decY = ay - dx;
        for (/**/; /**/; x += sx, decY += ay)
        {
            DrawFunction(x, y);

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
    case 1:  // Single-step in y-direction.
        decX = ax - dy;
        for (/**/; /**/; y += sy, decX += ax)
        {
            DrawFunction(x, y);

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
