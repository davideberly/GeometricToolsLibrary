// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.20

#pragma once

#include <GTL/Mathematics/Physics/MassSpringVolume.h>
#include <cstddef>
using namespace gtl;

class PhysicsModule : public MassSpringVolume<float, 3>
{
public:
    // Construction.
    PhysicsModule(std::size_t numSlices, std::size_t numRows, std::size_t numCols, float step, float viscosity);

    // External acceleration is due to viscous forces.
    virtual Vector3<float> ExternalAcceleration(std::size_t i, float const& time,
        std::vector<Vector3<float>> const& positions,
        std::vector<Vector3<float>> const& velocities);

protected:
    float mViscosity;
};
