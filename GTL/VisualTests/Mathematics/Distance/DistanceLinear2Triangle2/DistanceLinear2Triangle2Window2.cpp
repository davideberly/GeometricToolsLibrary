// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.30

#include "DistanceLinear2Triangle2Window2.h"

DistanceLinear2Triangle2Window2::DistanceLinear2Triangle2Window2(Parameters& parameters)
    :
    Window2(parameters),
    mTriangle{},
    mLength(static_cast<double>(mXSize)),
    mQuery{},
    mOutput{}
#if defined(USE_LT_QUERY)
    ,
    mLine{}
#endif
#if defined(USE_RT_QUERY)
    ,
    mRay{}
#endif
#if defined(USE_ST_QUERY)
    ,
    mSegment{}
#endif
{
#if defined(USE_LT_QUERY)
    mLine.origin = Vector2<double>{ (double)mXSize / 2, (double)mYSize / 2 };
    mLine.direction = Vector2<double>{ 1.0, 0.0 };
#endif
#if defined(USE_RT_QUERY)
    mRay.origin = Vector2<double>{ (double)mXSize / 2, (double)mYSize / 2 };
    mRay.direction = Vector2<double>{ 1.0, 0.0 };
#endif
#if defined(USE_ST_QUERY)
    mSegment.p[0] = Vector2<double>{ (double)mXSize / 2, (double)mYSize / 2 };
    mSegment.p[1] = mSegment.p[0] + 100.0 * Vector2<double>{1.0, 0.0};
#endif

    mTriangle.v[0] = { 200.0, 200.0 };
    mTriangle.v[1] = { 800.0, 300.0 };
    mTriangle.v[2] = { 300.0, 700.0 };

    DoQuery();

    mDoFlip = true;
    OnDisplay();
}

void DistanceLinear2Triangle2Window2::OnDisplay()
{
    std::uint32_t white = 0xFFFFFFFF;
    std::uint32_t blue = 0xFFFF0000;
    std::uint32_t red = 0xFF0000FF;
    std::uint32_t green = 0xFF00FF00;
    std::uint32_t black = 0xFF000000;
    std::uint32_t gray = 0xFFF0F0F0;
    ClearScreen(white);

    std::int32_t x0{}, y0{}, x1{}, y1{}, x2{}, y2{};
    x0 = (std::int32_t)mTriangle.v[0][0];
    y0 = (std::int32_t)mTriangle.v[0][1];
    x1 = (std::int32_t)mTriangle.v[1][0];
    y1 = (std::int32_t)mTriangle.v[1][1];
    x2 = (std::int32_t)mTriangle.v[2][0];
    y2 = (std::int32_t)mTriangle.v[2][1];
    DrawLine(x0, y0, x1, y1, blue);
    DrawLine(x1, y1, x2, y2, blue);
    DrawLine(x2, y2, x0, y0, blue);
    DrawFloodFill4((x0 + x1 + x2) / 3, (y0 + y1 + y2) / 3, gray, white);

    GetEndpoints(x0, y0, x1, y1);
    DrawLine(x0, y0, x1, y1, red);

    x0 = (std::int32_t)mOutput.closest[0][0];
    y0 = (std::int32_t)mOutput.closest[0][1];
    DrawThickPixel(x0, y0, 2, green);
    x0 = (std::int32_t)mOutput.closest[1][0];
    y0 = (std::int32_t)mOutput.closest[1][1];
    DrawThickPixel(x0, y0, 2, black);

    mScreenTextureNeedsUpdate = true;
    Window2::OnDisplay();
}

bool DistanceLinear2Triangle2Window2::OnCharPress(uint8_t key, int32_t x, int32_t y)
{
    double const translate = 1.0;
    double const angle = 0.01;
    Matrix2x2<double> rot{};

    switch (key)
    {
    case ' ':
        // For debugging. If a query fails on rotation or translation, set a
        // breakpoint on DoQuery and press the space-bar key to repeat the
        // query.
        DoQuery();
        OnDisplay();
        return true;

    case 'r':
        Rotation<double>::Convert(-angle, rot);
        Update(rot);
        DoQuery();
        OnDisplay();
        return true;

    case 'R':
        Rotation<double>::Convert(+angle, rot);
        Update(rot);
        DoQuery();
        OnDisplay();
        return true;

    case 'x':
        Update(0, -translate);
        DoQuery();
        OnDisplay();
        return true;

    case 'X':
        Update(0, +translate);
        DoQuery();
        OnDisplay();
        return true;

    case 'y':
        Update(1, -translate);
        DoQuery();
        OnDisplay();
        return true;

    case 'Y':
        Update(1, +translate);
        DoQuery();
        OnDisplay();
        return true;
    }

    return Window2::OnCharPress(key, x, y);
}

void DistanceLinear2Triangle2Window2::Update(Matrix2x2<double> const& rot)
{
#if defined(USE_LT_QUERY)
    mLine.direction = rot * mLine.direction;
#endif
#if defined(USE_RT_QUERY)
    mRay.direction = rot * mRay.direction;
#endif
#if defined(USE_ST_QUERY)
    Vector2<double> direction = mSegment.p[1] - mSegment.p[0];
    direction = rot * direction;
    mSegment.p[1] = mSegment.p[0] + direction;
#endif
}

void DistanceLinear2Triangle2Window2::Update(std::int32_t channel, double translate)
{
#if defined(USE_LT_QUERY)
    mLine.origin[channel] += translate;
#endif
#if defined(USE_RT_QUERY)
    mRay.origin[channel] += translate;
#endif
#if defined(USE_ST_QUERY)
    mSegment.p[0][channel] += translate;
    mSegment.p[1][channel] += translate;
#endif
}

void DistanceLinear2Triangle2Window2::DoQuery()
{
#if defined(USE_LT_QUERY)
    mOutput = mQuery(mLine, mTriangle);
#endif
#if defined(USE_RT_QUERY)
    mOutput = mQuery(mRay, mTriangle);
#endif
#if defined(USE_ST_QUERY)
    mOutput = mQuery(mSegment, mTriangle);
#endif
}

void DistanceLinear2Triangle2Window2::GetEndpoints(
    std::int32_t& x0, std::int32_t& y0,
    std::int32_t& x1, std::int32_t& y1)
{
#if defined(USE_LT_QUERY)
    x0 = (std::int32_t)(mLine.origin[0] - mLength * mLine.direction[0]);
    y0 = (std::int32_t)(mLine.origin[1] - mLength * mLine.direction[1]);
    x1 = (std::int32_t)(mLine.origin[0] + mLength * mLine.direction[0]);
    y1 = (std::int32_t)(mLine.origin[1] + mLength * mLine.direction[1]);
#endif
#if defined(USE_RT_QUERY)
    x0 = (std::int32_t)(mRay.origin[0]);
    y0 = (std::int32_t)(mRay.origin[1]);
    x1 = (std::int32_t)(mRay.origin[0] + mLength * mRay.direction[0]);
    y1 = (std::int32_t)(mRay.origin[1] + mLength * mRay.direction[1]);
#endif
#if defined(USE_ST_QUERY)
    x0 = (std::int32_t)(mSegment.p[0][0]);
    y0 = (std::int32_t)(mSegment.p[0][1]);
    x1 = (std::int32_t)(mSegment.p[1][0]);
    y1 = (std::int32_t)(mSegment.p[1][1]);
#endif
}
