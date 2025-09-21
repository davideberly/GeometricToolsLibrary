// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.09.21

#pragma once

#include <GTL/Applications/Window3.h>
#include <GTL/Mathematics/Approximation/3D/ApprHelmertTransformation3.h>
using namespace gtl;

class ApproximateHelmertTransformationWindow3 : public Window3
{
public:
    ApproximateHelmertTransformationWindow3(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y) override;

private:
    void CreateScene();
    void Align();

    static std::size_t constexpr numPoints = 32;
    static std::size_t constexpr numIterations = 1024;
    std::vector<Vector3<double>> mPPoints;
    std::vector<Vector3<double>> mQPoints;
    ApprHelmertTransformation3<double> mHelmert;
    std::string mMessage;

    std::vector<std::shared_ptr<Visual>> mPPointSpheres;
    std::vector<std::shared_ptr<Visual>> mQPointSpheres;
};
