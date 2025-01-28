// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.28

#pragma once

#include <GTL/Applications/Window2.h>
#include <GTL/Mathematics/Approximation/2D/ApprEllipse2.h>
using namespace gtl;

class ApproximateEllipse2Window2 : public Window2
{
public:
    ApproximateEllipse2Window2(Parameters& parameters);

    virtual void OnDisplay() override;
    virtual void DrawScreenOverlay() override;
    virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y) override;

private:
    void Get(Vector2<double> const& source, int32_t& x, int32_t& y) const
    {
        auto p = 64.0 * source + Vector2<double>{ 384.0, 384.0 };
        x = (int32_t)p[0];
        y = (int32_t)p[1];
    }

    Vector2<double> GetEllipsePoint(Ellipse2<double> const& ellipse, size_t imax, size_t i) const
    {
        double angle = C_TWO_PI<double> * (double)i / (double)imax;
        double cs = std::cos(angle);
        double sn = std::sin(angle);
        Vector2<double> p = ellipse.center +
            ellipse.extent[0] * cs * ellipse.axis[0] +
            ellipse.extent[1] * sn * ellipse.axis[1];
        return p;
    }

    void DrawMyEllipse(Ellipse2<double> const& ellipse, size_t numSamples, uint32_t color)
    {
        int32_t x0{}, y0{}, x1{}, y1{};
        Get(GetEllipsePoint(ellipse, numSamples, 0), x0, y0);
        for (size_t i = 1; i < numSamples; ++i)
        {
            Get(GetEllipsePoint(ellipse, numSamples, i), x1, y1);
            DrawLine(x0, y0, x1, y1, color);
            x0 = x1;
            y0 = y1;
        }
        Get(GetEllipsePoint(ellipse, numSamples, 0), x1, y1);
        DrawLine(x0, y0, x1, y1, color);
    }

    std::vector<Vector2<double>> mPoints;
    Ellipse2<double> mTrueEllipse, mApprEllipse;
    size_t mIteration, mNumIterations;
    double mError;
    std::string mMessage;
};
