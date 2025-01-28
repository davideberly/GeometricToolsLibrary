// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.28

#pragma once

#include <GTL/Applications/Window3.h>
#include <GTL/Mathematics/Curves/BSplineCurveFit.h>
using namespace gtl;

class BSplineCurveFitterWindow3 : public Window3
{
public:
    BSplineCurveFitterWindow3(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y) override;

private:
    void CreateScene();
    void CreateBSplinePolyline();

    enum { NUM_SAMPLES = 1000 };

    struct Vertex
    {
        Vector3<float> position;
        Vector4<float> color;
    };

    std::vector<Vector3<float>> mSamples;
    std::shared_ptr<Visual> mHelix, mPolyline;

    int32_t mDegree, mNumControls;
    BSplineCurve<float, 3> mSpline;
    float mAvrError, mRmsError;
    std::string mMessage;
};
