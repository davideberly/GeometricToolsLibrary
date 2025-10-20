// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.20

#include "PhysicsModule.h"

PhysicsModule::PhysicsModule (std::size_t numSlices, std::size_t numRows, std::size_t numCols,
    float step, float viscosity)
    :
    MassSpringVolume<float, 3>(numSlices, numRows, numCols, step),
    mViscosity(viscosity)
{
}

Vector3<float> PhysicsModule::ExternalAcceleration(std::size_t i, float const&,
    std::vector<Vector3<float>> const&, std::vector<Vector3<float>> const& velocities)
{
    Vector3<float> acceleration = -mViscosity * velocities[i];
    return acceleration;
}
