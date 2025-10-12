// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.12

#include "RigidTetrahedron.h"
#include <cstddef>

RigidTetrahedron::RigidTetrahedron(Tetrahedron3<double> const& bodyTetrahedron,
    Vector3<double> const& position, double massDensity)
    :
    RigidBody<double>{},
    mBodyTetrahedron(bodyTetrahedron),
    mWorldTetrahedron(bodyTetrahedron),
    mWorldCentroid{ 0.0, 0.0, 0.0 },
    mWorldSphere({ 0.0, 0.0, 0.0 }, 0.0)
{
    // The computation of mass and body inertia tensor assumes the mass
    // density is 1. The output mass and body inertia tensor must be
    // multiplied by the mass density to obtain the correct values.
    auto const& faceIndices = Tetrahedron3<double>::GetAllFaceIndices();
    double mass{};
    Vector3<double> centroid{};
    Matrix3x3<double> bodyInertia{};
    PolyhedralMassProperties<double>::Execute(mBodyTetrahedron.v.data(), 4, faceIndices.data(), true,
        mass, centroid, bodyInertia);
    mass *= massDensity;
    bodyInertia *= massDensity;

    // Compute a bounding sphere for the tetrahedon. The center of the
    // bounding sphere is the center of mass. The radius is the maximum
    // distance from the center of mass to the vertices.
    mWorldSphere.center = centroid;
    mWorldSphere.radius = 0.0;
    for (std::size_t j = 0; j < 4; ++j)
    {
        double length = Length(mBodyTetrahedron.v[j] - centroid);
        if (length > mWorldSphere.radius)
        {
            mWorldSphere.radius = length;
        }
    }

    SetMass(mass);
    SetBodyInertia(bodyInertia);
    SetPosition(position);
    UpdateWorldQuantities();
}

void RigidTetrahedron::UpdateWorldQuantities()
{
    auto const& rotate = GetROrientation();
    auto const& translate = GetPosition();
    for (std::size_t j = 0; j < 4; ++j)
    {
        mWorldTetrahedron.v[j] = rotate * mBodyTetrahedron.v[j] + translate;
    }
    mWorldCentroid = mWorldTetrahedron.ComputeCentroid();
    mWorldSphere.center = translate;
}
