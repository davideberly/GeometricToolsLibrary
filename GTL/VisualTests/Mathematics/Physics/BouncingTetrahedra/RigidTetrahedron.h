// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.12

#include <GTL/Mathematics/Physics/RigidBody.h>
#include <GTL/Mathematics/Primitives/ND/Hypersphere.h>
#include <GTL/Mathematics/Primitives/3D/Tetrahedron3.h>
#include <GTL/Mathematics/Physics/PolyhedralMassProperties.h>
#include <array>
using namespace gtl;

class RigidTetrahedron : public RigidBody<double>
{
public:
    RigidTetrahedron(Tetrahedron3<double> const& bodyTetrahedron,
        Vector3<double> const& position, double massDensity);

    virtual ~RigidTetrahedron() = default;

    inline Tetrahedron3<double> const& GetBodyTetrahedron() const
    {
        return mBodyTetrahedron;
    }

    inline Tetrahedron3<double> const& GetWorldTetrahedron() const
    {
        return mWorldTetrahedron;
    }

    inline Vector3<double> const& GetWorldCentroid() const
    {
        return mWorldCentroid;
    }

    inline Sphere3<double> const& GetWorldSphere() const
    {
        return mWorldSphere;
    }

    inline double GetRadius() const
    {
        return mWorldSphere.radius;
    }

    void UpdateWorldQuantities();

private:

    Tetrahedron3<double> mBodyTetrahedron;
    Tetrahedron3<double> mWorldTetrahedron;
    Vector3<double> mWorldCentroid;
    Sphere3<double> mWorldSphere;
};
