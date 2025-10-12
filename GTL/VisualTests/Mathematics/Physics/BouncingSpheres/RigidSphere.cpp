// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.12

#include "RigidSphere.h"
using namespace gtl;

RigidSphere::RigidSphere(Sphere3<double> const& sphere, double massDensity)
    :
    RigidBody<double>{},
    mWorldSphere({ 0.0, 0.0, 0.0 }, sphere.radius)
{
    double rCubed = sphere.radius * sphere.radius * sphere.radius;
    double volume = 4.0 * C_PI<double> * rCubed / 3.0;
    double mass = massDensity * volume;
    Matrix3x3<double> bodyInertia = massDensity * Matrix3x3<double>::Identity();
    SetMass(mass);
    SetBodyInertia(bodyInertia);
    SetPosition(sphere.center);
    UpdateWorldQuantities();
}

void RigidSphere::UpdateWorldQuantities()
{
    mWorldSphere.center = GetPosition();
}
