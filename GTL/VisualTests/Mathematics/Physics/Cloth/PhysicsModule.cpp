// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.17

#include <GTL/Mathematics/Arithmetic/Constants.h>
#include "PhysicsModule.h"
#include <cmath>
#include <random>

PhysicsModule::PhysicsModule(std::size_t numRows, std::size_t numCols,
    float step, Vector3<float> const& gravity, Vector3<float> const& wind,
    float viscosity, float amplitude)
    :
    MassSpringSurface<float, 3>(numRows, numCols, step),
    mGravity(gravity),
    mWind(wind),
    mDirection(UnitCross(gravity, wind)),
    mViscosity(viscosity),
    mAmplitude(amplitude),
    mPhases(mNumParticles)
{
    std::default_random_engine dre{};
    std::uniform_real_distribution<float> urd(0.0f, C_PI<float>);
    for (std::size_t row = 0; row < mNumRows; ++row)
    {
        for (std::size_t col = 0; col < mNumCols; ++col)
        {
            mPhases[GetIndex(row, col)] = urd(dre);
        }
    }
}

Vector3<float> PhysicsModule::ExternalAcceleration(std::size_t i, float const& time,
    std::vector<Vector3<float>> const&, std::vector<Vector3<float>> const& velocity)
{
    // Acceleration due to gravity, wind, and viscosity.
    Vector3<float> acceleration = mGravity + mWind - mViscosity * velocity[i];

    // Add a sinusoidal perturbation.
    float amplitude = mAmplitude * std::sin(2.0f * time + mPhases[i]);
    acceleration += amplitude * mDirection;
    return acceleration;
}
