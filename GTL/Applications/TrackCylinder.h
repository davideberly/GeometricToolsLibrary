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
    class TrackCylinder : public TrackObject
    {
    public:
        // Construction.  The window rectangle is assumed to be defined in
        // right-handed coordinates, so if you use a window client rectangle
        // for the trackball and this rectangle is in left-handed coordinates,
        // you must reflect the y-values in SetInitialPoint and SetFinalPoint
        // by (ySize - 1 - y).  A root node is used to represent the
        // trackcylinder/ orientation.  Objects may be attached and detached
        // as desired.
        virtual ~TrackCylinder() = default;
        TrackCylinder();
        TrackCylinder(int32_t xSize, int32_t ySize, std::shared_ptr<Camera> const& camera);

        // Reset the trackcylinder rotation to the identity.
        void Reset();

    protected:
        virtual void OnSetInitialPoint() override;
        virtual void OnSetFinalPoint() override;

        // The window rectangle is mXSize-by-mYSize.  Let the initial point
        // be (x0,y0) and the final point be (x1,y1).  Let dx = x1 - x0 and
        // dy = y1 - y0.  If dx >= dy, then the yaw angle in [-pi,pi] is
        // modified according to yaw = pi*dx/xSize.  If dy > dx, then the
        // pitch angle in [-pi/2,pi/2] is modified according to
        // pitch = -pi*dy/ySize but is clamped to [-pi/2,pi/2].

        // Rotations about the z-axis.
        float mInitialYaw, mYaw;

        // Rotations about the y-axis.
        float mInitialPitch, mPitch;
    };
}
