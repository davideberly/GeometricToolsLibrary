// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.12

#include <GTL/Mathematics/Physics/RigidBody.h>
#include <GTL/Mathematics/Primitives/ND/Hypersphere.h>
using namespace gtl;

class RigidSphere : public RigidBody<double>
{
public:
    RigidSphere(Sphere3<double> const& sphere, double massDensity);
    virtual ~RigidSphere() = default;

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
    Sphere3<double> mWorldSphere;
};
