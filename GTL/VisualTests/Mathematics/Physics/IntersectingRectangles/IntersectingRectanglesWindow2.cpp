// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.12

#include "IntersectingRectanglesWindow2.h"
#include <cstddef>

IntersectingRectanglesWindow2::IntersectingRectanglesWindow2(Parameters& parameters)
    :
    Window2(parameters),
    mRectangles{},
    mManager{},
    mSize(static_cast<float>(mXSize)),
    mTimer{},
    mLastIdle(0.0),
    mDRE{},
    mPerturb(-4.0f, 4.0f)
{
    std::uniform_real_distribution<float> rnd(0.125f * mSize, 0.875f * mSize);
    std::uniform_real_distribution<float> intrrnd(8.0f, 64.0f);
    for (std::size_t i = 0; i < 16; ++i)
    {
        Vector2<float> min{ rnd(mDRE), rnd(mDRE) };
        Vector2<float> max{ min[0] + intrrnd(mDRE), min[1] + intrrnd(mDRE) };
        mRectangles.push_back(AlignedBox2<float>(min, max));
    }

    mManager = std::make_unique<RectangleManager<float>>(mRectangles);
    mLastIdle = mTimer.GetSeconds();
    OnDisplay();
}

void IntersectingRectanglesWindow2::OnDisplay()
{
    ClearScreen(0xFFFFFFFF);
    DrawRectangles();
    mScreenTextureNeedsUpdate = true;
    Window2::OnDisplay();
}

void IntersectingRectanglesWindow2::OnIdle()
{
    double currIdle = mTimer.GetSeconds();
    double diff = currIdle - mLastIdle;
    if (diff >= 1.0 / 30.0)
    {
        ModifyRectangles();
        OnDisplay();
        mLastIdle = currIdle;
    }
}

void IntersectingRectanglesWindow2::ModifyRectangles()
{
    std::size_t i = 0;
    for (AlignedBox2<float> rectangle : mRectangles)
    {
        float dx = mPerturb(mDRE);
        if (0.0f <= rectangle.min[0] + dx && rectangle.max[0] + dx < mSize)
        {
            rectangle.min[0] += dx;
            rectangle.max[0] += dx;
        }

        float dy = mPerturb(mDRE);
        if (0.0f <= rectangle.min[1] + dy && rectangle.max[1] + dy < mSize)
        {
            rectangle.min[1] += dy;
            rectangle.max[1] += dy;
        }

        mManager->SetRectangle(i, rectangle);
        ++i;
    }

    mManager->Update();
}

void IntersectingRectanglesWindow2::DrawRectangles()
{
    std::uint32_t constexpr gray = 0xFFC0C0C0;
    std::uint32_t constexpr black = 0x00000000;
    std::uint32_t constexpr red = 0xFF0000FF;
    std::int32_t xmin{}, xmax{}, ymin{}, ymax{};

    for (auto const& rectangle : mRectangles)
    {
        xmin = static_cast<std::int32_t>(std::lrint(rectangle.min[0]));
        xmax = static_cast<std::int32_t>(std::lrint(rectangle.max[0]));
        ymin = static_cast<std::int32_t>(std::lrint(rectangle.min[1]));
        ymax = static_cast<std::int32_t>(std::lrint(rectangle.max[1]));
        DrawRectangle(xmin, ymin, xmax, ymax, gray, true);
        DrawRectangle(xmin, ymin, xmax, ymax, black, false);
    }

    FIQuery<float, AlignedBox2<float>, AlignedBox2<float>> query{};
    for (auto const& overlap : mManager->GetOverlap())
    {
        auto result = query(mRectangles[overlap[0]], mRectangles[overlap[1]]);
        if (result.intersect)
        {
            xmin = static_cast<std::int32_t>(std::lrint(result.box.min[0]));
            xmax = static_cast<std::int32_t>(std::lrint(result.box.max[0]));
            ymin = static_cast<std::int32_t>(std::lrint(result.box.min[1]));
            ymax = static_cast<std::int32_t>(std::lrint(result.box.max[1]));
            DrawRectangle(xmin, ymin, xmax, ymax, red, true);
            DrawRectangle(xmin, ymin, xmax, ymax, black, false);
        }
    }
}
