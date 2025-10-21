// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.21

#pragma once

#include <GTL/Mathematics/Physics/MassSpringArbitrary.h>
#include <cstddef>
#include <vector>
using namespace gtl;

class PhysicsModule : public MassSpringArbitrary<float, 3>
{
public:
    // Construction.
    PhysicsModule(std::size_t numParticles, std::size_t numSteps, float step, float viscosity);

    // External acceleration is due to viscous forces.
    virtual Vector3<float> ExternalAcceleration(std::size_t i, float const& time,
        std::vector<Vector3<float>> const& positions,
        std::vector<Vector3<float>> const& velocities);

protected:
    float mViscosity;
};
