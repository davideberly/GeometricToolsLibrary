// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#include "Delaunay2DWindow2.h"
#include <random>

Delaunay2DWindow2::Delaunay2DWindow2(Parameters& parameters)
    :
    Window2(parameters),
    mVertices{},
    mHull{},
    mDelaunay{},
    mCurrentTriX(-1),
    mCurrentTriY(-1)
{
#if 1
    // Randomly generated points.
    std::mt19937 mte;
    std::uniform_real_distribution<float> rnd(0.125f, 0.875f);
    mVertices.resize(256);
    for (auto& v : mVertices)
    {
        v[0] = mXSize*rnd(mte);
        v[1] = mYSize*rnd(mte);
    }
#endif

#if 0
    // A 3x3 square grid.
    mVertices.resize(9);
    mVertices[0] = Vector2<float>{  64.0f,  64.0f };
    mVertices[1] = Vector2<float>{  64.0f, 256.0f };
    mVertices[2] = Vector2<float>{  64.0f, 448.0f };
    mVertices[3] = Vector2<float>{ 256.0f,  64.0f };
    mVertices[4] = Vector2<float>{ 256.0f, 256.0f };
    mVertices[5] = Vector2<float>{ 256.0f, 448.0f };
    mVertices[6] = Vector2<float>{ 448.0f,  64.0f };
    mVertices[7] = Vector2<float>{ 448.0f, 256.0f };
    mVertices[8] = Vector2<float>{ 448.0f, 448.0f };
#endif

#if 0
    // Right-triangle grid with duplicated point.
    mVertices.resize(7);
    mVertices[0] = { 64.0f, 64.0f };
    mVertices[1] = { 64.0f, 448.0f };
    mVertices[2] = { 448.0f, 64.0f };
    mVertices[3] = { 256.0f,  256.0f };
    mVertices[4] = { 256.0f, 64.0f };
    mVertices[5] = { 64.0f,  256.0f };
    mVertices[6] = { 256.0f,  256.0f };
#endif

    mDelaunay(mVertices);
    GTL_RUNTIME_ASSERT(
        mDelaunay.GetDimension() == 2,
        "Degenerate point set.");
}

void Delaunay2DWindow2::OnDisplay()
{
    uint32_t const white = 0xFFFFFFFF;
    uint32_t const gray = 0xFF808080;
    uint32_t const red = 0xFF0000FF;

    ClearScreen(white);

    int32_t x0, y0, x1, y1, x2, y2;
    Vector2<float> v0, v1, v2;

    // Draw the triangle mesh.
    std::vector<size_t> const& indices = mDelaunay.GetIndices();
    size_t i, numTriangles = indices.size() / 3;
    for (i = 0; i < numTriangles; ++i)
    {
        v0 = mVertices[indices[3 * i]];
        x0 = static_cast<int32_t>(std::lrint(v0[0]));
        y0 = static_cast<int32_t>(std::lrint(v0[1]));

        v1 = mVertices[indices[3 * i + 1]];
        x1 = static_cast<int32_t>(std::lrint(v1[0]));
        y1 = static_cast<int32_t>(std::lrint(v1[1]));

        v2 = mVertices[indices[3 * i + 2]];
        x2 = static_cast<int32_t>(std::lrint(v2[0]));
        y2 = static_cast<int32_t>(std::lrint(v2[1]));

        DrawLine(x0, y0, x1, y1, gray);
        DrawLine(x1, y1, x2, y2, gray);
        DrawLine(x2, y2, x0, y0, gray);
    }

    // Get the convex hull.
    std::set<EdgeKey<false>> edges{};
    for (i = 0; i < numTriangles; ++i)
    {
        size_t j0 = indices[3 * i + 0];
        size_t j1 = indices[3 * i + 1];
        size_t j2 = indices[3 * i + 2];

        std::array<EdgeKey<false>, 3> eKeys =
        {
            EdgeKey<false>(j0, j1),
            EdgeKey<false>(j1, j2),
            EdgeKey<false>(j2, j0)
        };

        for (size_t k = 0; k < 3; ++k)
        {
            auto iter = edges.find(eKeys[k]);
            if (iter == edges.end())
            {
                edges.insert(eKeys[k]);
            }
            else
            {
                edges.erase(eKeys[k]);
            }
        }
    }

    // Draw the hull.
    for (auto const& eKey : edges)
    {
        v0 = mVertices[eKey[0]];
        x0 = static_cast<int32_t>(std::lrint(v0[0]));
        y0 = static_cast<int32_t>(std::lrint(v0[1]));

        v1 = mVertices[eKey[1]];
        x1 = static_cast<int32_t>(std::lrint(v1[0]));
        y1 = static_cast<int32_t>(std::lrint(v1[1]));

        DrawLine(x0, y0, x1, y1, red);
    }

    mScreenTextureNeedsUpdate = true;
    Window2::OnDisplay();
}
