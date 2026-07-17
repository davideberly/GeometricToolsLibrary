// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

// Class AlignedBoxBV is a bounding volume that supports the queries based on
// BVTree and its derived classes.

#include <GTL/Mathematics/Intersection/3D/IntrLine3OrientedBox3.h>
#include <GTL/Mathematics/Intersection/3D/IntrRay3OrientedBox3.h>
#include <GTL/Mathematics/Intersection/3D/IntrSegment3OrientedBox3.h>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class OrientedBoxBV
    {
    public:
        OrientedBoxBV()
            :
            box{}
        {
        }

        void GetSplittingAxis(Vector3<T>& origin, Vector3<T>& direction) const
        {
            origin = box.center;

            T maxExtent = box.extent[0];
            std::size_t maxIndex = 0;
            if (box.extent[1] > maxExtent)
            {
                maxExtent = box.extent[1];
                maxIndex = 1;
            }
            if (box.extent[2] > maxExtent)
            {
                maxExtent = box.extent[2];
                maxIndex = 2;
            }
            direction = box.axis[maxIndex];
        }

        static bool IntersectLine(Vector3<T> const& P, Vector3<T> const& Q,
            OrientedBoxBV<T> const& boundingVolume)
        {
            TIQuery<T, Line3<T>, OrientedBox3<T>> query{};
            auto output = query(Line3<T>(P, Q), boundingVolume.box);
            return output.intersect;
        }

        static bool IntersectRay(Vector3<T> const& P, Vector3<T> const& Q,
            OrientedBoxBV<T> const& boundingVolume)
        {
            TIQuery<T, Ray3<T>, OrientedBox3<T>> query{};
            auto output = query(Ray3<T>(P, Q), boundingVolume.box);
            return output.intersect;
        }

        static bool IntersectSegment(Vector3<T> const& P, Vector3<T> const& Q,
            OrientedBoxBV<T> const& boundingVolume)
        {
            TIQuery<T, Segment3<T>, OrientedBox3<T>> query{};
            auto output = query(Segment3<T>(P, Q), boundingVolume.box);
            return output.intersect;
        }

        OrientedBox3<T> box;
    };
}
