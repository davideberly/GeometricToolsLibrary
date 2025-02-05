// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#include "ConstrainedDelaunay2DWindow2.h"
#include <random>

ConstrainedDelaunay2DWindow2::ConstrainedDelaunay2DWindow2(
    Parameters& parameters)
    :
    Window2(parameters)
{
    // Randomly generated points.
    std::mt19937 mte;
    std::uniform_real_distribution<float> rnd(0.125f, 0.875f);
    mVertices.resize(256);
    for (auto& v : mVertices)
    {
        v[0] = mXSize*rnd(mte);
        v[1] = mYSize*rnd(mte);
    }

    mDelaunay(mVertices);
    GTL_RUNTIME_ASSERT(
        mDelaunay.GetDimension() == 2,
        "Degenerate point set.");
}

void ConstrainedDelaunay2DWindow2::OnDisplay()
{
    uint32_t const white = 0xFFFFFFFF;
    uint32_t const gray = 0xFF808080;
    uint32_t const red = 0xFF0000FF;
    uint32_t const green = 0xFF00FF00;

    ClearScreen(white);

    int32_t x0{}, y0{}, x1{}, y1{}, x2{}, y2{};
    Vector2<float> v0{}, v1{}, v2{};

    // Draw the triangle mesh.
    std::vector<std::array<std::size_t, 3>> triangles{};
    mDelaunay.GetConstrainedTriangles(triangles);
    size_t i, numTriangles = triangles.size();
    for (i = 0; i < numTriangles; ++i)
    {
        auto const& triangle = triangles[i];
        v0 = mVertices[triangle[0]];
        x0 = static_cast<int32_t>(std::lrint(v0[0]));
        y0 = static_cast<int32_t>(std::lrint(v0[1]));

        v1 = mVertices[triangle[1]];
        x1 = static_cast<int32_t>(std::lrint(v1[0]));
        y1 = static_cast<int32_t>(std::lrint(v1[1]));

        v2 = mVertices[triangle[2]];
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
        auto const& triangle = triangles[i];

        std::array<EdgeKey<false>, 3> eKeys =
        {
            EdgeKey<false>(triangle[0], triangle[1]),
            EdgeKey<false>(triangle[1], triangle[2]),
            EdgeKey<false>(triangle[2], triangle[0])
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

    // Draw edges <0,5>, <5,9>, and <9,0>.
    v0 = mVertices[0];
    x0 = static_cast<int32_t>(std::lrint(v0[0]));
    y0 = static_cast<int32_t>(std::lrint(v0[1]));
    v1 = mVertices[5];
    x1 = static_cast<int32_t>(std::lrint(v1[0]));
    y1 = static_cast<int32_t>(std::lrint(v1[1]));
    DrawLine(x0, y0, x1, y1, green);

    v0 = mVertices[9];
    x0 = static_cast<int32_t>(std::lrint(v0[0]));
    y0 = static_cast<int32_t>(std::lrint(v0[1]));
    DrawLine(x0, y0, x1, y1, green);

    v1 = mVertices[0];
    x1 = static_cast<int32_t>(std::lrint(v1[0]));
    y1 = static_cast<int32_t>(std::lrint(v1[1]));
    DrawLine(x0, y0, x1, y1, green);

    mScreenTextureNeedsUpdate = true;
    Window2::OnDisplay();
}

bool ConstrainedDelaunay2DWindow2::OnCharPress(uint8_t key, int32_t x, int32_t y)
{
    std::vector<std::size_t> outEdge;
    switch (key)
    {
    case '0':
        mDelaunay.Insert({ 0, 5 }, outEdge);
        OnDisplay();
        return true;
    case '1':
        mDelaunay.Insert({ 5, 9 }, outEdge);
        OnDisplay();
        return true;
    case '2':
        mDelaunay.Insert({ 9, 0 }, outEdge);
        OnDisplay();
        return true;
    }
    return Window2::OnCharPress(key, x, y);
}
