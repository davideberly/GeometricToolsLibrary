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
#include <GTL/Mathematics/Primitives/ND/Segment.h>
#include <algorithm>

namespace gtl
{
    template <typename T>
    class FIQuery<T, Segment3<T>, Cone3<T>>
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

        Output operator()(Segment3<T> const& segment, Cone3<T> const& cone)
        {
            // Execute the line-cone query.
            Output output{};
            Vector3<T> segOrigin = segment.p[0];
            Vector3<T> segDirection = segment.p[1] - segment.p[0];
            this->DoQuery(segOrigin, segDirection, cone, output);

            // Adjust the t-interval depending on whether the line-cone
            // t-interval overlaps the segment interval [0,1]. The block
            // numbers are a continuation of those in IntrRay3Cone3.h, which
            // themselves are a continuation of those in IntrLine3Cone3.h.
            if (output.type != Output::isEmpty)
            {
                using QFN1 = typename FIQuery<T, Line3<T>, Cone3<T>>::QFN1;
                QFN1 qfzero(0, 0, output.t[0].d), qfone(1, 0, output.t[0].d);

                if (output.type == Output::isPoint)
                {
                    if (output.t[0] < qfzero || output.t[0] > qfone)
                    {
                        // Block 21.
                        this->SetEmpty(output);
                    }
                    // else: Block 22.
                }
                else if (output.type == Output::isSegment)
                {
                    if (output.t[1] < qfzero || output.t[0] > qfone)
                    {
                        // Block 23.
                        this->SetEmpty(output);
                    }
                    else
                    {
                        QFN1 t0 = std::max(qfzero, output.t[0]);
                        QFN1 t1 = std::min(qfone, output.t[1]);
                        if (t0 < t1)
                        {
                            // Block 24.
                            this->SetSegment(t0, t1, output);
                        }
                        else
                        {
                            // Block 25.
                            this->SetPoint(t0, output);
                        }
                    }
                }
                else if (output.type == Output::isRayPositive)
                {
                    if (qfone < output.t[0])
                    {
                        // Block 26.
                        this->SetEmpty(output);
                    }
                    else if (qfone > output.t[0])
                    {
                        // Block 27.
                        this->SetSegment(std::max(qfzero, output.t[0]), qfone, output);
                    }
                    else
                    {
                        // Block 28.
                        this->SetPoint(qfone, output);
                    }
                }
                else  // output.type == Output::isRayNegative
                {
                    if (qfzero > output.t[1])
                    {
                        // Block 29.
                        this->SetEmpty(output);
                    }
                    else if (qfzero < output.t[1])
                    {
                        // Block 30.
                        this->SetSegment(qfzero, std::min(qfone, output.t[1]), output);
                    }
                    else
                    {
                        // Block 31.
                        this->SetPoint(qfzero, output);
                    }
                }
            }

            output.ComputePoints(segment.p[0], segDirection);
            output.intersect = (output.type != Output::isEmpty);
            return output;
        }

    private:
        friend class UnitTestIntrSegment3Cone3;
    };
}
