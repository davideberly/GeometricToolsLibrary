// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

#include <GTL/Mathematics/Intersection/1D/IntrIntervals.h>
#include <GTL/Mathematics/Intersection/2D/IntrLine2Line2.h>
#include <GTL/Mathematics/Primitives/ND/Ray.h>
#include <GTL/Mathematics/Primitives/ND/Segment.h>
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>

namespace gtl
{
    template <typename T>
    class TIQuery<T, Ray2<T>, Segment2<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0)
            {
            }

            // The number is 0 (no intersection), 1 (ray and segment intersect
            // in a single point), or 2 (ray and segment are collinear and
            // intersect in a segment).
            bool intersect;
            std::size_t numIntersections;
        };

        Output operator()(Ray2<T> const& ray, Segment2<T> const& segment)
        {
            Output output{};

            Vector2<T> segOrigin{}, segDirection{};
            T segExtent{};
            segment.GetCenteredForm(segOrigin, segDirection, segExtent);

            FIQuery<T, Line2<T>, Line2<T>> llQuery{};
            Line2<T> line0(ray.origin, ray.direction);
            Line2<T> line1(segOrigin, segDirection);
            auto llOutput = llQuery(line0, line1);
            if (llOutput.numIntersections == 1)
            {
                // Test whether the line-line intersection is on the ray and
                // segment.
                if (llOutput.line0Parameter[0] >= C_<T>(0) &&
                    std::fabs(llOutput.line1Parameter[0]) <= segExtent)
                {
                    output.intersect = true;
                    output.numIntersections = 1;
                }
                else
                {
                    output.intersect = false;
                    output.numIntersections = 0;
                }
            }
            else if (llOutput.numIntersections == std::numeric_limits<std::size_t>::max())
            {
                // Compute the location of the right-most point of the segment
                // relative to the ray direction.
                Vector2<T> diff = segOrigin - ray.origin;
                T t = Dot(ray.direction, diff) + segExtent;
                if (t > C_<T>(0))
                {
                    output.intersect = true;
                    output.numIntersections = 2;
                }
                else if (t < C_<T>(0))
                {
                    output.intersect = false;
                    output.numIntersections = 0;
                }
                else  // t == 0
                {
                    output.intersect = true;
                    output.numIntersections = 1;
                }
            }
            else
            {
                output.intersect = false;
                output.numIntersections = 0;
            }

            return output;
        }

    private:
        friend class UnitTestIntrRay2Segment2;
    };

    template <typename T>
    class FIQuery<T, Ray2<T>, Segment2<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0),
                rayParameter{ C_<T>(0), C_<T>(0) },
                segmentParameter{ C_<T>(0), C_<T>(0) },
                point{}
            {
            }

            // The number is 0 (no intersection), 1 (ray and segment intersect
            // in a single point), or 2 (ray and segment are collinear and
            // intersect in a segment).
            bool intersect;
            std::size_t numIntersections;

            // If numIntersections is 1, the intersection is
            //   point[0] = ray.origin + rayParameter[0] * ray.direction
            //     = segment.center + segmentParameter[0] * segment.direction
            // If numIntersections is 2, the endpoints of the segment of
            // intersection are
            //   point[i] = ray.origin + rayParameter[i] * ray.direction
            //     = segment.center + segmentParameter[i] * segment.direction
            // with rayParameter[0] <= rayParameter[1] and
            // segmentParameter[0] <= segmentParameter[1].
            std::array<T, 2> rayParameter, segmentParameter;
            std::array<Vector2<T>, 2> point;
        };

        Output operator()(Ray2<T> const& ray, Segment2<T> const& segment)
        {
            Output output{};

            Vector2<T> segOrigin{}, segDirection{};
            T segExtent{};
            segment.GetCenteredForm(segOrigin, segDirection, segExtent);

            FIQuery<T, Line2<T>, Line2<T>> llQuery{};
            Line2<T> line0(ray.origin, ray.direction);
            Line2<T> line1(segOrigin, segDirection);
            auto llOutput = llQuery(line0, line1);
            if (llOutput.numIntersections == 1)
            {
                // Test whether the line-line intersection is on the ray and
                // segment.
                if (llOutput.line0Parameter[0] >= C_<T>(0) &&
                    std::fabs(llOutput.line1Parameter[0]) <= segExtent)
                {
                    output.intersect = true;
                    output.numIntersections = 1;
                    output.rayParameter[0] = llOutput.line0Parameter[0];
                    output.segmentParameter[0] = llOutput.line1Parameter[0];
                    output.point[0] = llOutput.point;
                }
                else
                {
                    output.intersect = false;
                    output.numIntersections = 0;
                }
            }
            else if (llOutput.numIntersections == std::numeric_limits<std::size_t>::max())
            {
                // Compute t for which segment.origin =
                // ray.origin + t*ray.direction.
                Vector2<T> diff = segOrigin - ray.origin;
                T t = Dot(ray.direction, diff);

                // Get the ray interval.
                std::array<T, 2> interval0 =
                {
                    C_<T>(0), std::numeric_limits<T>::max()
                };

                // Compute the location of the segment endpoints relative to
                // the ray.
                std::array<T, 2> interval1 = { t - segExtent, t + segExtent };

                // Compute the intersection of [0,+infinity) and [tmin,tmax].
                FIQuery<T, std::array<T, 2>, std::array<T, 2>> iiQuery{};
                auto iiResult = iiQuery(interval0, interval1);
                if (iiResult.intersect)
                {
                    output.intersect = true;
                    output.numIntersections = iiResult.numIntersections;
                    for (std::size_t i = 0; i < iiResult.numIntersections; ++i)
                    {
                        output.rayParameter[i] = iiResult.overlap[i];
                        output.segmentParameter[i] = iiResult.overlap[i] - t;
                        output.point[i] = ray.origin + output.rayParameter[i] * ray.direction;
                    }
                }
                else
                {
                    output.intersect = false;
                    output.numIntersections = 0;
                }
            }
            else
            {
                output.intersect = false;
                output.numIntersections = 0;
            }
            return output;
        }

    private:
        friend class UnitTestIntrRay2Segment2;
    };
}
