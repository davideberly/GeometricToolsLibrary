// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.30

#pragma once

#include <GTL/Applications/Window2.h>
#include <GTL/Mathematics/Algebra/Rotation.h>
#include <GTL/Mathematics/Distance/2D/DistLine2Triangle2.h>
#include <GTL/Mathematics/Distance/2D/DistRay2Triangle2.h>
#include <GTL/Mathematics/Distance/2D/DistSegment2Triangle2.h>
using namespace gtl;

#define USE_LT_QUERY
//#define USE_RT_QUERY
//#define USE_ST_QUERY

class DistanceLinear2Triangle2Window2 : public Window2
{
public:
    DistanceLinear2Triangle2Window2(Parameters& parameters);

    virtual void OnDisplay() override;
    virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y);

private:
    void Update(Matrix2x2<double> const& rot);
    void Update(std::int32_t channel, double translate);
    void DoQuery();
    void GetEndpoints(
        std::int32_t& x0, std::int32_t& y0,
        std::int32_t& x1, std::int32_t& y1);

    Triangle2<double> mTriangle;
    double mLength;

#if defined(USE_LT_QUERY)
    using Query = DCPQuery<double, Line2<double>, Triangle2<double>>;
    Line2<double> mLine;
#endif

#if defined(USE_RT_QUERY)
    using Query = DCPQuery<double, Ray2<double>, Triangle2<double>>;
    Ray2<double> mRay;
#endif

#if defined(USE_ST_QUERY)
    using Query = DCPQuery<double, Segment2<double>, Triangle2<double>>;
    Segment2<double> mSegment;
#endif
    using Output = Query::Output;
    Query mQuery;
    Output mOutput;
};
