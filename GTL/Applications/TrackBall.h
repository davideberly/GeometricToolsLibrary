// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Applications/TrackObject.h>

namespace gtl
{
    class TrackBall : public TrackObject
    {
    public:
        // Construction and destruction.  The trackball is the largest circle
        // centered in the rectangle of dimensions xSize-by-ySize.  The
        // rectangle is assumed to be defined in right-handed coordinates, so
        // y-values in SetInitialPoint and SetFinalPoint are reflected to
        // (ySize - 1 - y).
        virtual ~TrackBall() = default;
        TrackBall();
        TrackBall(int32_t xSize, int32_t ySize, std::shared_ptr<Camera> const& camera);

        // Reset the trackball rotation to the identity.
        void Reset();

    protected:
        virtual void OnSetInitialPoint() override;
        virtual void OnSetFinalPoint() override;

        Matrix3x3<float> mInitialOrientation;
    };
}
