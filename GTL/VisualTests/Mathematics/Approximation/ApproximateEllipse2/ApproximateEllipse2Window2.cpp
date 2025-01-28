// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.28

#include "ApproximateEllipse2Window2.h"
#include <random>

ApproximateEllipse2Window2::ApproximateEllipse2Window2(Parameters& parameters)
    :
    Window2(parameters),
    mPoints{},
    mTrueEllipse{},
    mApprEllipse{},
    mIteration(0),
    mNumIterations(1024),
    mError(-1.0),
    mMessage("")
{
    mTrueEllipse.center = { 0.0, 0.0 };
    mTrueEllipse.axis[0] = { 2.0, 1.0 };
    Normalize(mTrueEllipse.axis[0]);
    mTrueEllipse.axis[1] = Perp(mTrueEllipse.axis[0]);
    mTrueEllipse.extent = { 4.0, 1.0 };

    std::default_random_engine dre{};
    std::uniform_real_distribution<double> urd(-0.1, 0.1);
    size_t numPoints = 1024;
    mPoints.resize(numPoints);
    for (size_t i = 0; i < numPoints; ++i)
    {
        mPoints[i] = GetEllipsePoint(mTrueEllipse, numPoints, i) +
            Vector2<double>{ urd(dre), urd(dre) };
    }

    bool useEllipseForInitialGuess = false;
    double error = 0.0;
    error = ApprEllipse2<double>::Fit(mPoints, mNumIterations, 4096,
        useEllipseForInitialGuess, mApprEllipse);

    OnCharPress(' ', -1, 0);
    mDoFlip = true;
    OnDisplay();
}

void ApproximateEllipse2Window2::OnDisplay()
{
    ClearScreen(0xFFFFFFFF);

    int32_t x{}, y{};
    for (size_t i = 0; i < mPoints.size(); ++i)
    {
        Get(mPoints[i], x, y);
        DrawThickPixel(x, y, 1, 0xFF00FF00);
    }

    size_t numSamples = 2048;
    DrawMyEllipse(mTrueEllipse, numSamples, 0xFFFF0000);
    DrawMyEllipse(mApprEllipse, numSamples, 0xFF0000FF);

    mScreenTextureNeedsUpdate = true;
    Window2::OnDisplay();
}

void ApproximateEllipse2Window2::DrawScreenOverlay()
{
    std::array<float, 4> const black = { 0.0f, 0.0f, 0.0f, 1.0f };
    mEngine->Draw(8, 24, black, mMessage);
}

bool ApproximateEllipse2Window2::OnCharPress(uint8_t key, int32_t x, int32_t y)
{
    switch (key)
    {
    case ' ':
        if (mIteration < mNumIterations)
        {
            mMessage = "iteration = " + std::to_string(mIteration) +
                ", error = " + std::to_string(mError);

            bool useEllipseForInitialGuess = (x >= 0);
            if (useEllipseForInitialGuess)
            {
                mApprEllipse = mTrueEllipse;
            }
            mError = ApprEllipse2<double>::Fit(mPoints, 1, 4096,
                useEllipseForInitialGuess, mApprEllipse);
            ++mIteration;
            OnDisplay();
            return true;
        }
    }

    return Window2::OnCharPress(key, x, y);
}
