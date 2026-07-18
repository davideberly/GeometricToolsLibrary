// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.18

#pragma once

// The query is for finite cylinders. The cylinder and box are considered to
// be solids. The cylinder has center C, unit-length axis direction D, radius
// r and height h. The oriented box is converted to a canonical box after
// which a test-intersection query is performed on the finite cylinder and the
// canonical box. See the comments in IntrCanonicalBox3Cylinder3.h for a brief
// description. The details are in
//   https://www.geometrictools.com/Documentation/IntersectionBoxCylinder.pdf

#include <GTL/Mathematics/Intersection/3D/IntrCanonicalBox3Cylinder3.h>
#include <GTL/Mathematics/Primitives/ND/OrientedBox.h>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, OrientedBox3<T>, Cylinder3<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false)
            {
            }

            bool intersect;
        };

        Output operator()(OrientedBox3<T> const& box, Cylinder3<T> const& cylinder)
        {
            GTL_ARGUMENT_ASSERT(
                cylinder.IsFinite(),
                "Infinite cylinders are not yet supported.");

            // Convert the problem to one involving a finite cylinder and a
            // canonical box. This involves translating the box center to the
            // origin and then rotating the box axes to the standard coordinate
            // axes. The cylinder center must also be translated and rotated
            // accordingly.
            CanonicalBox3<T> cbox(box.extent);
            Vector3<T> diff = cylinder.center - box.center;
            Cylinder3<T> transformedCylinder{};
            transformedCylinder.radius = cylinder.radius;
            transformedCylinder.height = cylinder.height;
            for (std::size_t i = 0; i < 3; ++i)
            {
                transformedCylinder.center[i] = Dot(box.axis[i], diff);
                transformedCylinder.direction[i] = Dot(box.axis[i], cylinder.direction);
            }

            TIQuery<T, CanonicalBox3<T>, Cylinder3<T>> bcQuery{};
            auto bcOutput = bcQuery(cbox, transformedCylinder);
            Output output{};
            output.intersect = bcOutput.intersect;
            return output;
        }

    private:
        friend class UnitTestIntrOrientedBox3Cylinder3;
    };
}
