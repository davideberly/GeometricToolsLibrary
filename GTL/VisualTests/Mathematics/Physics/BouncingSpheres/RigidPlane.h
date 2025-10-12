// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.10.12

#include <GTL/Mathematics/Physics/RigidBody.h>
#include <GTL/Mathematics/Primitives/ND/Hyperplane.h>
using namespace gtl;

class RigidPlane : public RigidBody<double>
{
public:
    RigidPlane(Plane3<double> const& plane);
    virtual ~RigidPlane() = default;

    inline Plane3<double> const& GetPlane() const
    {
        return mPlane;
    }

    inline double GetSignedDistance(Vector3<double> const& point) const
    {
        return Dot(mPlane.normal, point) - mPlane.constant;
    }

private:
    Plane3<double> mPlane;
};
