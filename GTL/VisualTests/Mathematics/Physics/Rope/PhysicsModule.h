// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.15

#pragma once

#include <GTL/Mathematics/Physics/MassSpringCurve.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <cstddef>
#include <random>
using namespace gtl;

class PhysicsModule : public MassSpringCurve<float, 3>
{
public:
    // Construction. Gravity is controlled by the input 'gravity'. Mass-spring
    // systems tend to exhibit stiffness in the sense of numerical stability.
    // To remedy this problem, a small amount of viscous friction is added to
    // the external force, -viscosity*velocity, where 'viscosity' is a small
    // positive constant. The initial wind force is specified by the caller.
    // The application of wind can be toggled by 'enableWind'. The member
    // 'enableWindChange' allows the wind direction to change randomly, but
    // each new direction is nearby the old direction in order to obtain some
    // sense of continuity of direction. The magnitude of the wind force is
    // constant, the length of the initial force.
    PhysicsModule(std::size_t numParticles, float step, Vector3<float> const& gravity,
        Vector3<float> const& wind, float windChangeAmplitude, float viscosity);

    bool enableWind;
    bool enableWindChange;

    // External acceleration is due to forces of gravitation, wind, and
    // viscous friction.  The wind forces are randomly generated.
    virtual Vector3<float> ExternalAcceleration(std::size_t i, float const& time,
        std::vector<Vector3<float>> const& position, std::vector<Vector3<float>> const& velocity);

protected:
    Vector3<float> mGravity, mWind;
    float mWindChangeAmplitude, mViscosity;
    std::default_random_engine mDRE;
    std::uniform_real_distribution<float> mURD;
};
