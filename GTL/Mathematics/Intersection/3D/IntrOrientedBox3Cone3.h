// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.18

#pragma once

// Test for intersection of a box and a cone. The cone can be infinite
//   0 <= minHeight < maxHeight = std::numeric_limits<T>::max()
// or finite (cone frustum)
//   0 <= minHeight < maxHeight < std::numeric_limits<T>::max().
// The algorithm is described in
//   https://www.geometrictools.com/Documentation/IntersectionBoxCone.pdf
// and reports an intersection only when th intersection set has positive
// volume. For example, let the box be outside the cone. If the box is
// below the minHeight plane at the cone vertex and just touches the cone
// vertex, no intersection is reported. If the box is above the maxHeight
// plane and just touches the disk capping the cone, either at a single
// point, a line segment of points or a polygon of points, no intersection
// is reported.

// TODO: These queries were designed when an infinite cone was defined
// by choosing maxHeight of std::numeric_limits<T>::max(). The Cone<N,T>
// class has been redesigned not to use std::numeric_limits to allow for
// arithmetic systems that do not have representations for infinities
// (such as BSNumber and BSRational). The intersection queries need to be
// rewritten for the new class design. FOR NOW, the queries will work with
// float/double when you create a cone using the cone-frustum constructor
// Cone(ray, angle, minHeight, std::numeric_limits<T>::max()).

#include <GTL/Mathematics/Intersection/3D/IntrAlignedBox3Cone3.h>
#include <GTL/Mathematics/Primitives/ND/OrientedBox.h>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, OrientedBox3<T>, Cone3<T>>
        :
        public TIQuery<T, AlignedBox3<T>, Cone3<T>>
    {
    public:
        struct Output
            :
            public TIQuery<T, AlignedBox3<T>, Cone3<T>>::Output
        {
            Output()
                :
                TIQuery<T, AlignedBox3<T>, Cone3<T>>::Output{}
            {
            }

            // No additional information to compute.
        };

        Output operator()(OrientedBox3<T> const& box, Cone3<T> const& cone)
        {
            // Transform the cone and box so that the cone vertex is at the
            // origin and the box is axis aligned. This allows us to call the
            // base class operator()(...).
            Vector3<T> diff = box.center - cone.vertex;
            Vector3<T> xfrmBoxCenter
            {
                Dot(box.axis[0], diff),
                Dot(box.axis[1], diff),
                Dot(box.axis[2], diff)
            };
            AlignedBox3<T> xfrmBox{};
            xfrmBox.min = xfrmBoxCenter - box.extent;
            xfrmBox.max = xfrmBoxCenter + box.extent;

            Cone3<T> xfrmCone = cone;
            for (std::size_t i = 0; i < 3; ++i)
            {
                xfrmCone.vertex[i] = C_<T>(0);
                xfrmCone.direction[i] = Dot(box.axis[i], cone.direction);
            }

            // Test for intersection between the aligned box and the cone.
            auto bcOutput = TIQuery<T, AlignedBox3<T>, Cone3<T>>::operator()(xfrmBox, xfrmCone);
            Output output{};
            output.intersect = bcOutput.intersect;
            return output;
        }

    private:
        friend class UnitTestIntrOrientedBox3Cone3;
    };
}
