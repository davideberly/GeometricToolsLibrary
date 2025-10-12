// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.12

#include "RigidPlane.h"
using namespace gtl;

RigidPlane::RigidPlane(Plane3<double> const& plane)
    :
    RigidBody<double>{},
    mPlane(plane)
{
    SetMass(0.0);
    SetBodyInertia(Matrix3x3<double>::Zero());
    SetPosition(mPlane.origin);
}
