// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#include "ConvexHull2DWindow2.h"
#include <random>

ConvexHull2DWindow2::ConvexHull2DWindow2(Parameters& parameters)
    :
    Window2(parameters)
{
#if 1
    // Randomly generated points.
    std::mt19937 mte;
    std::uniform_real_distribution<float> rnd(0.125f, 0.875f);
    mVertices.resize(256);
    for (auto& v : mVertices)
    {
        v[0] = mXSize * rnd(mte);
        v[1] = mYSize * rnd(mte);
    }
#endif

#if 0
    // A 3x3 square grid.
    mVertices.resize(9);
    mVertices[0] = Vector2<float>(64.0f, 64.0f);
    mVertices[1] = Vector2<float>(64.0f, 256.0f);
    mVertices[2] = Vector2<float>(64.0f, 448.0f);
    mVertices[3] = Vector2<float>(256.0f, 64.0f);
    mVertices[4] = Vector2<float>(256.0f, 256.0f);
    mVertices[5] = Vector2<float>(256.0f, 448.0f);
    mVertices[6] = Vector2<float>(448.0f, 64.0f);
    mVertices[7] = Vector2<float>(448.0f, 256.0f);
    mVertices[8] = Vector2<float>(448.0f, 448.0f);
#endif

    mConvexHull(mVertices);
}

void ConvexHull2DWindow2::OnDisplay()
{
    ClearScreen(0xFFFFFFFF);

    uint32_t const black = 0xFF000000;
    uint32_t const gray = 0xFF808080;
    uint32_t const blue = 0xFFFF0000;

    int32_t x0, y0, x1, y1;
    Vector2<float> v0, v1;

    // Draw the convex polygon.
    std::vector<size_t> const& hull = mConvexHull.GetHull();
    size_t const numIndices = hull.size();
    for (size_t i0 = numIndices - 1, i1 = 0; i1 < numIndices; i0 = i1++)
    {
        v0 = mVertices[hull[i0]];
        x0 = static_cast<int32_t>(std::lrint(v0[0]));
        y0 = static_cast<int32_t>(std::lrint(v0[1]));

        v1 = mVertices[hull[i1]];
        x1 = static_cast<int32_t>(std::lrint(v1[0]));
        y1 = static_cast<int32_t>(std::lrint(v1[1]));

        DrawLine(x0, y0, x1, y1, gray);
    }

    // Draw the input points.
    for (auto const& v : mVertices)
    {
        x0 = static_cast<int32_t>(std::lrint(v[0]));
        y0 = static_cast<int32_t>(std::lrint(v[1]));
        DrawThickPixel(x0, y0, 1, blue);
    }

    // Draw the hull points.
    for (auto index : hull)
    {
        v0 = mVertices[index];
        x0 = static_cast<int32_t>(std::lrint(v0[0]));
        y0 = static_cast<int32_t>(std::lrint(v0[1]));
        DrawThickPixel(x0, y0, 1, black);
    }

    mScreenTextureNeedsUpdate = true;
    Window2::OnDisplay();
}
