// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.18

#pragma once

// The queries consider the cone to be single sided and solid. The
// cone height range is [hmin,hmax]. The cone can be infinite where
// hmin = 0 and hmax = +infinity, infinite truncated where hmin > 0
// and hmax = +infinity, finite where hmin = 0 and hmax < +infinity,
// or a cone frustum where hmin > 0 and hmax < +infinity. The
// algorithm details are found in
// https://www.geometrictools.com/Documentation/IntersectionLineCone.pdf

#include <GTL/Mathematics/Intersection/3D/IntrLine3Cone3.h>
#include <GTL/Mathematics/Primitives/ND/Ray.h>
#include <algorithm>

namespace gtl
{
    template <typename T>
    class FIQuery<T, Ray3<T>, Cone3<T>>
        :
        public FIQuery<T, Line3<T>, Cone3<T>>
    {
    public:
        struct Output
            :
            public FIQuery<T, Line3<T>, Cone3<T>>::Output
        {
            Output()
                :
                FIQuery<T, Line3<T>, Cone3<T>>::Output{}
            {
            }

            // No additional information to compute.
        };

        Output operator()(Ray3<T> const& ray, Cone3<T> const& cone)
        {
            // Execute the line-cone query.
            Output output{};
            this->DoQuery(ray.origin, ray.direction, cone, output);

            // Adjust the t-interval depending on whether the line-cone
            // t-interval overlaps the ray interval [0,+infinity). The block
            // numbers are a continuation of those in IntrLine3Cone3.h.
            if (output.type != Output::isEmpty)
            {
                using QFN1 = typename FIQuery<T, Line3<T>, Cone3<T>>::QFN1;
                QFN1 qfzero(0, 0, output.t[0].d);

                if (output.type == Output::isPoint)
                {
                    if (output.t[0] < qfzero)
                    {
                        // Block 12.
                        this->SetEmpty(output);
                    }
                    // else: Block 13.
                }
                else if (output.type == Output::isSegment)
                {
                    if (output.t[1] > qfzero)
                    {
                        // Block 14.
                        this->SetSegment(std::max(output.t[0], qfzero), output.t[1], output);
                    }
                    else if (output.t[1] < qfzero)
                    {
                        // Block 15.
                        this->SetEmpty(output);
                    }
                    else  // output.t[1] == qfzero
                    {
                        // Block 16.
                        this->SetPoint(qfzero, output);
                    }
                }
                else if (output.type == Output::isRayPositive)
                {
                    // Block 17.
                    this->SetRayPositive(std::max(output.t[0], qfzero), output);
                }
                else  // output.type == Output::isRayNegative
                {
                    if (output.t[1] > qfzero)
                    {
                        // Block 18.
                        this->SetSegment(qfzero, output.t[1], output);
                    }
                    else if (output.t[1] < qfzero)
                    {
                        // Block 19.
                        this->SetEmpty(output);
                    }
                    else  // output.t[1] == qfzero
                    {
                        // Block 20.
                        this->SetPoint(qfzero, output);
                    }
                }
            }

            output.ComputePoints(ray.origin, ray.direction);
            output.intersect = (output.type != Output::isEmpty);
            return output;
        }

    private:
        friend class UnitTestIntrRay3Cone3;
    };
}
