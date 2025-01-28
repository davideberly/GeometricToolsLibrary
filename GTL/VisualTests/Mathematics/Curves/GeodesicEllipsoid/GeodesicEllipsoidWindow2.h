// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.28

#pragma once

#include <GTL/Applications/Window2.h>
#include <GTL/Mathematics/Curves/EllipsoidGeodesic.h>
using namespace gtl;

class GeodesicEllipsoidWindow2 : public Window2
{
public:
    GeodesicEllipsoidWindow2(Parameters& parameters);

    virtual void OnDisplay() override;
    virtual void DrawScreenOverlay() override;
    virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y) override;

private:
    void ComputeTruePath();
    void ComputeApprPath(bool subdivide);
    void ComputeApprLength();
    void ParamToXY(Vector<float> const& param, int32_t& x, int32_t& y) const;
    void XYToParam(int32_t x, int32_t y, Vector<float>& param) const;

    size_t mSize;
    EllipsoidGeodesic<float> mGeodesic;
    Vector<float> mParam0, mParam1;
    float mXMin, mXMax, mXDelta;
    float mYMin, mYMax, mYDelta;

    size_t mNumTruePoints;
    std::vector<Vector<float>> mTruePoints;
    size_t mNumApprPoints;
    std::vector<Vector<float>> mApprPoints;

    size_t mCurrNumApprPoints;
    float mTrueDistance;
    float mApprDistance;
    float mApprCurvature;

    std::array<float, 4> mTextColor;
};
