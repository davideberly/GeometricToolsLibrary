// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.17

#pragma once

// The intersection query considers the box and circle to be solids. The
// circle object includes the region inside the circular boundary and the
// box object includes the region inside the rectangular boundary. If the
// circle object and rectangle object overlap, the objects intersect. The
// find-intersection query is based on the document
// https://www.geometrictools.com/Documentation/IntersectionMovingCircleRectangle.pdf

#include <GTL/Mathematics/Intersection/2D/IntrAlignedBox2Circle2.h>
#include <GTL/Mathematics/Distance/ND/DistPointOrientedBox.h>
#include <array>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, OrientedBox2<T>, Circle2<T>>
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

        Output operator()(OrientedBox2<T> const& box, Circle2<T> const& circle)
        {
            DCPQuery<T, Vector2<T>, OrientedBox2<T>> pbQuery{};
            auto pbOutput = pbQuery(circle.center, box);
            Output output{};
            output.intersect = (pbOutput.sqrDistance <= circle.radius * circle.radius);
            return output;
        }

    private:
        friend class UnitTestIntrOrientedBox2Circle2;
    };

    template <typename T>
    class FIQuery<T, OrientedBox2<T>, Circle2<T>>
        :
        public FIQuery<T, AlignedBox2<T>, Circle2<T>>
    {
    public:
        // See the base class for the definition of 'struct Output'.
        typename FIQuery<T, AlignedBox2<T>, Circle2<T>>::Output
        operator()(OrientedBox2<T> const& box, Vector2<T> const& boxVelocity,
            Circle2<T> const& circle, Vector2<T> const& circleVelocity)
        {
            // Transform the oriented box to an axis-aligned box centered at
            // the origin and transform the circle accordingly.  Compute the
            // velocity of the circle relative to the box.
            Vector2<T> cdiff = circle.center - box.center;
            Vector2<T> vdiff = circleVelocity - boxVelocity;
            Vector2<T> C{}, V{};
            for (std::size_t i = 0; i < 2; ++i)
            {
                C[i] = Dot(cdiff, box.axis[i]);
                V[i] = Dot(vdiff, box.axis[i]);
            }

            // Change signs on components, if necessary, to transform C to the
            // first quadrant.  Adjust the velocity accordingly.
            std::array<T, 2> sign{ C_<T>(0), C_<T>(0) };
            for (std::size_t i = 0; i < 2; ++i)
            {
                if (C[i] >= C_<T>(0))
                {
                    sign[i] = C_<T>(1);
                }
                else
                {
                    C[i] = -C[i];
                    V[i] = -V[i];
                    sign[i] = C_<T>(-1);
                }
            }

            typename FIQuery<T, AlignedBox2<T>, Circle2<T>>::Output output{};
            this->DoQuery(box.extent, C, circle.radius, V, output);

            if (output.intersectionType != 0)
            {
                // Transform back to the original coordinate system.
                output.contactPoint = box.center
                    + (sign[0] * output.contactPoint[0]) * box.axis[0]
                    + (sign[1] * output.contactPoint[1]) * box.axis[1];
            }
            return output;
        }

    private:
        friend class UnitTestIntrOrientedBox2Circle2;
    };
}
