// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.17

#pragma once

#include <GTL/Mathematics/Physics/MassSpringSurface.h>
#include <cstddef>
#include <vector>
using namespace gtl;

class PhysicsModule : public MassSpringSurface<float, 3>
{
public:
    // Construction.  Gravity is controlled by the input 'gravity'.
    // Mass-spring systems tend to exhibit stiffness in the sense of numerical
    // stability.  To remedy this problem, a small amount of viscous friction
    // is added to the external force, -viscosity*velocity, where 'viscosity'
    // is a small positive constant.  The initial wind force is specified by
    // the caller.  The wind remains in effect throughout the simulation.  To
    // simulate oscillatory behavior locally, random forces are applied at
    // each mass in the direction perpendicular to the plane of the wind and
    // gravity vectors.  The amplitudes are sinusoidal.  The phases are
    // randomly generated.
    PhysicsModule(std::size_t numRows, std::size_t numCols, float step,
        Vector3<float> const& gravity, Vector3<float> const& wind,
        float viscosity, float amplitude);

    // External acceleration is due to forces of gravitation, wind, and
    // viscous friction.  The wind forces are randomly generated.
    virtual Vector3<float> ExternalAcceleration(std::size_t i, float const& time,
        std::vector<Vector3<float>> const& position,
        std::vector<Vector3<float>> const& velocity);

protected:
    Vector3<float> mGravity, mWind, mDirection;
    float mViscosity, mAmplitude;
    std::vector<float> mPhases;
};
