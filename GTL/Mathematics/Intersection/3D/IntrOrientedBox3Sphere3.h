// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.09.23

#pragma once

#include <GTL/Mathematics/Distance/ND/DistPointOrientedBox.h>
#include <GTL/Mathematics/Intersection/3D/IntrAlignedBox3Sphere3.h>

namespace gtl
{
    template <typename T>
    class TIQuery<T, OrientedBox3<T>, Sphere3<T>>
        :
        public TIQuery<T, AlignedBox3<T>, Sphere3<T>>
    {
    public:
        // The intersection query considers the box and sphere to be solids.
        // For example, if the sphere is strictly inside the box (does not
        // touch the box faces), the objects intersect.
        struct Output
            :
            public TIQuery<T, AlignedBox3<T>, Sphere3<T>>::Output
        {
            Output()
                :
                TIQuery<T, AlignedBox3<T>, Sphere3<T>>::Output{}
            {
            }

            // No additional information to compute.
        };

        Output operator()(OrientedBox3<T> const& box, Sphere3<T> const& sphere)
        {
            DCPQuery<T, Vector3<T>, OrientedBox3<T>> pbQuery{};
            auto pbOutput = pbQuery(sphere.center, box);
            Output output{};
            output.intersect = (pbOutput.sqrDistance <= sphere.radius * sphere.radius);
            return output;
        }

    private:
        friend class UnitTestIntrOrientedBox3Sphere3;
    };

    template <typename T>
    class FIQuery<T, OrientedBox3<T>, Sphere3<T>>
        :
        public FIQuery<T, AlignedBox3<T>, Sphere3<T>>
    {
    public:
        // Currently, only a dynamic query is supported. The static query
        // must compute the intersection set of (solid) box and sphere.
        struct Output
            :
            public FIQuery<T, AlignedBox3<T>, Sphere3<T>>::Output
        {
            Output()
                :
                FIQuery<T, AlignedBox3<T>, Sphere3<T>>::Output{}
            {
            }

            // No additional information to compute.
        };

        Output operator()(OrientedBox3<T> const& box, Vector3<T> const& boxVelocity,
            Sphere3<T> const& sphere, Vector3<T> const& sphereVelocity)
        {
            Output output{};
            output.intersectionType = 0;
            output.contactTime = C_<T>(0);
            output.contactPoint = { C_<T>(0), C_<T>(0), C_<T>(0) };

            // Transform the sphere and box so that the box center becomes
            // the origin and the box is axis aligned.  Compute the velocity
            // of the sphere relative to the box.
            Vector3<T> temp = sphere.center - box.center;
            Vector3<T> C
            {
                Dot(temp, box.axis[0]),
                Dot(temp, box.axis[1]),
                Dot(temp, box.axis[2])
            };

            temp = sphereVelocity - boxVelocity;
            Vector3<T> V
            {
                Dot(temp, box.axis[0]),
                Dot(temp, box.axis[1]),
                Dot(temp, box.axis[2])
            };

            this->DoQuery(box.extent, C, sphere.radius, V, output);

            // Transform back to the original coordinate system.
            if (output.intersectionType != 0)
            {
                auto& P = output.contactPoint;
                P = box.center + P[0] * box.axis[0] + P[1] * box.axis[1] + P[2] * box.axis[2];
            }
            return output;
        }

    private:
        friend class UnitTestIntrOrientedBox3Sphere3;
    };
}
