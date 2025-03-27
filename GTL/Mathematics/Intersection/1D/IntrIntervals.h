// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// The intervals are of the form [t0,t1], [t0,+infinity) or (-infinity,t1].
// Degenerate intervals are allowed (t0 = t1). The queries do not perform
// validation on the input intervals to test whether t0 <= t1.

#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Mathematics/Intersection/IntersectionQuery.h>
#include <algorithm>
#include <array>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class TIQuery<T, std::array<T, 2>, std::array<T, 2>>
    {
    public:
        // The query tests overlap, whether a single point or an entire 
        // interval.
        struct Output
        {
            Output()
                :
                intersect(false),
                firstTime(C_<T>(0)),
                lastTime(C_<T>(0))
            {
            }

            bool intersect;

            // Dynamic queries (intervals moving with constant speeds).
            // If 'intersect' is true, the contact times are valid and
            // 0 <= firstTime <= lastTime. The only exception is when
            // the intervals initially overlap and have the same speed.
            // In this case, firstTime is set to 0 and lastTime is set to -1.
            T firstTime, lastTime;
        };

        // Static query. The firstTime and lastTime values are set to zero by
        // the Output constructor, but they are invalid for the static query
        // regardless of the value of 'intersect'.
        Output operator()(std::array<T, 2> const& interval0, std::array<T, 2> const& interval1)
        {
            Output output{};
            output.intersect = (interval0[0] <= interval1[1] && interval0[1] >= interval1[0]);
            return output;
        }

        // Static queries where at least one interval is semiinfinite. The
        // two types of semiinfinite intervals are [a,+infinity), which I call
        // a positive-infinite interval, and (-infinity,a], which I call a
        // negative-infinite interval. The firstTime and lastTime values are
        // set to zero by the Output constructor, but they are invalid for the
        // static query regardless of the value of 'intersect'.
        Output operator()(std::array<T, 2> const& finite, T const& a, bool isPositiveInfinite)
        {
            Output output{};

            if (isPositiveInfinite)
            {
                output.intersect = (finite[1] >= a);
            }
            else  // is negative-infinite
            {
                output.intersect = (finite[0] <= a);
            }

            return output;
        }

        Output operator()(T const& a0, bool isPositiveInfinite0,
            T const& a1, bool isPositiveInfinite1)
        {
            Output output{};

            if (isPositiveInfinite0)
            {
                if (isPositiveInfinite1)
                {
                    output.intersect = true;
                }
                else  // interval1 is negative-infinite
                {
                    output.intersect = (a0 <= a1);
                }
            }
            else  // interval0 is negative-infinite
            {
                if (isPositiveInfinite1)
                {
                    output.intersect = (a0 >= a1);
                }
                else  // interval1 is negative-infinite
                {
                    output.intersect = true;
                }
            }

            return output;
        }

        // Dynamic query. The current time is 0.
        Output operator()(
            std::array<T, 2> const& interval0, T const& speed0,
            std::array<T, 2> const& interval1, T const& speed1)
        {
            Output output{};

            if (interval0[1] < interval1[0])
            {
                // interval0 initially to the left of interval1.
                T diffSpeed = speed0 - speed1;
                if (diffSpeed > C_<T>(0))
                {
                    // The intervals must move towards each other.
                    output.intersect = true;
                    output.firstTime = (interval1[0] - interval0[1]) / diffSpeed;
                    output.lastTime = (interval1[1] - interval0[0]) / diffSpeed;
                    return output;
                }
            }
            else if (interval0[0] > interval1[1])
            {
                // interval0 initially to the right of interval1.
                T diffSpeed = speed1 - speed0;
                if (diffSpeed > C_<T>(0))
                {
                    // The intervals must move towards each other.
                    output.intersect = true;
                    output.firstTime = (interval0[0] - interval1[1]) / diffSpeed;
                    output.lastTime = (interval0[1] - interval1[0]) / diffSpeed;
                    return output;
                }
            }
            else
            {
                // The intervals are initially intersecting.
                output.intersect = true;
                output.firstTime = C_<T>(0);
                if (speed1 > speed0)
                {
                    output.lastTime = (interval0[1] - interval1[0]) / (speed1 - speed0);
                }
                else if (speed1 < speed0)
                {
                    output.lastTime = (interval1[1] - interval0[0]) / (speed0 - speed1);
                }
                else
                {
                    // The intervals move in lock-step. Flag this for the
                    // caller by setting the last time to a number smaller
                    // than the first time.
                    output.lastTime = C_<T>(-1);
                }
                return output;
            }

            // The Output constructor set 'intersect' to false and the
            // 'firstTime' and 'lastTime' to zero.
            return output;
        }
    };

    template <typename T>
    class FIQuery<T, std::array<T, 2>, std::array<T, 2>>
    {
    public:
        // The query finds overlap, whether a single point or an entire
        // interval.
        struct Output
        {
            Output()
                :
                intersect(false),
                numIntersections(0),
                overlap{ C_<T>(0), C_<T>(0) },
                type(isEmpty),
                firstTime(C_<T>(0)),
                lastTime(C_<T>(0))
            {
            }

            bool intersect;

            // Static queries (no motion of intervals over time). The number
            // of number of intersections is 0 (no overlap), 1 (intervals are
            // just touching), or 2 (intervals overlap in an interval). If
            // 'intersect' is false, numIntersections is 0 and 'overlap' is
            // set to [0,0]. If 'intersect' is true, numIntersections is
            // 1 or 2. When 1, 'overlap' is set to [x,x], which is degenerate
            // and represents the single intersection point x. When 2,
            // 'overlap' is the interval of intersection.
            std::size_t numIntersections;
            std::array<T, 2> overlap;

            // No intersection.
            static std::size_t const isEmpty = 0;

            // Intervals touch at an endpoint, [t0,t0].
            static std::size_t const isPoint = 1;

            // Finite-length interval of intersection, [t0,t1].
            static std::size_t const isFinite = 2;

            // Semiinfinite interval of intersection, [t0,+infinity). The
            // output.overlap[0] is t0 and output.overlap[1] is +1 as a
            // message that the right endpoint is +infinity (you still need
            // the output.type to know this interpretation).
            static std::size_t const isPositiveInfinite = 3;

            // Semiinfinite interval of intersection, (-infinity,t1]. The
            // output.overlap[0] is -1 as a message that the left endpoint is
            // -infinity (you still need the output.type to know this
            // interpretation). The output.overlap[1] is t1.
            static std::size_t const isNegativeInfinite = 4;

            // The dynamic queries all set the type to isDynamicQuery because
            // the queries look for time of first and last contact.
            static std::size_t const isDynamicQuery = 5;

            // The type is one of isEmpty, isPoint, isFinite,
            // isPositiveInfinite, isNegativeInfinite or isDynamicQuery.
            std::size_t type;

            // Dynamic queries (intervals moving with constant speeds).
            // If 'intersect' is true, the contact times are valid and
            // 0 <= firstTime <= lastTime.
            T firstTime, lastTime;
        };

        // Static query.
        Output operator()(std::array<T, 2> const& interval0, std::array<T, 2> const& interval1)
        {
            Output output{};

            if (interval0[1] < interval1[0] || interval0[0] > interval1[1])
            {
                output.numIntersections = 0;
                output.overlap[0] = C_<T>(0);
                output.overlap[1] = C_<T>(0);
                output.type = Output::isEmpty;
            }
            else if (interval0[1] > interval1[0])
            {
                if (interval0[0] < interval1[1])
                {
                    output.overlap[0] = (interval0[0] < interval1[0] ? interval1[0] : interval0[0]);
                    output.overlap[1] = (interval0[1] > interval1[1] ? interval1[1] : interval0[1]);
                    if (output.overlap[0] < output.overlap[1])
                    {
                        output.numIntersections = 2;
                        output.type = Output::isFinite;
                    }
                    else
                    {
                        output.numIntersections = 1;
                        output.type = Output::isPoint;
                    }
                }
                else  // interval0[0] == interval1[1]
                {
                    output.numIntersections = 1;
                    output.overlap[0] = interval0[0];
                    output.overlap[1] = output.overlap[0];
                    output.type = Output::isPoint;
                }
            }
            else  // interval0[1] == interval1[0]
            {
                output.numIntersections = 1;
                output.overlap[0] = interval0[1];
                output.overlap[1] = output.overlap[0];
                output.type = Output::isPoint;
            }

            output.intersect = (output.numIntersections > 0);
            return output;
        }

        // Static queries where at least one interval is semiinfinite. The
        // two types of semiinfinite intervals are [a,+infinity), which I call
        // a positive-infinite interval, and (-infinity,a], which I call a
        // negative-infinite interval.
        Output operator()(std::array<T, 2> const& finite, T const& a, bool isPositiveInfinite)
        {
            Output output{};

            if (isPositiveInfinite)
            {
                if (finite[1] > a)
                {
                    output.overlap[0] = std::max(finite[0], a);
                    output.overlap[1] = finite[1];
                    if (output.overlap[0] < output.overlap[1])
                    {
                        output.numIntersections = 2;
                        output.type = Output::isFinite;
                    }
                    else
                    {
                        output.numIntersections = 1;
                        output.type = Output::isPoint;
                    }
                }
                else if (finite[1] == a)
                {
                    output.numIntersections = 1;
                    output.overlap[0] = a;
                    output.overlap[1] = output.overlap[0];
                    output.type = Output::isPoint;
                }
                else
                {
                    output.numIntersections = 0;
                    output.overlap[0] = C_<T>(0);
                    output.overlap[1] = C_<T>(0);
                    output.type = Output::isEmpty;
                }
            }
            else  // is negative-infinite
            {
                if (finite[0] < a)
                {
                    output.overlap[0] = finite[0];
                    output.overlap[1] = std::min(finite[1], a);
                    if (output.overlap[0] < output.overlap[1])
                    {
                        output.numIntersections = 2;
                        output.type = Output::isFinite;
                    }
                    else
                    {
                        output.numIntersections = 1;
                        output.type = Output::isPoint;
                    }
                }
                else if (finite[0] == a)
                {
                    output.numIntersections = 1;
                    output.overlap[0] = a;
                    output.overlap[1] = output.overlap[0];
                    output.type = Output::isPoint;
                }
                else
                {
                    output.numIntersections = 0;
                    output.overlap[0] = C_<T>(0);
                    output.overlap[1] = C_<T>(0);
                    output.type = Output::isEmpty;
                }
            }

            output.intersect = (output.numIntersections > 0);
            return output;
        }

        Output operator()(T const& a0, bool isPositiveInfinite0,
            T const& a1, bool isPositiveInfinite1)
        {
            Output output{};

            if (isPositiveInfinite0)
            {
                if (isPositiveInfinite1)
                {
                    // overlap[1] is +infinity, but set it to +1 because T
                    // might not have a representation for +infinity. The
                    // type indicates the interval is positive-infinite, so
                    // the +1 is a reminder that overlap[1] is +infinity.
                    output.numIntersections = 1;
                    output.overlap[0] = std::max(a0, a1);
                    output.overlap[1] = C_<T>(1);
                    output.type = Output::isPositiveInfinite;
                }
                else  // interval1 is negative-infinite
                {
                    if (a0 > a1)
                    {
                        output.numIntersections = 0;
                        output.overlap[0] = C_<T>(0);
                        output.overlap[1] = C_<T>(0);
                        output.type = Output::isEmpty;
                    }
                    else if (a0 < a1)
                    {
                        output.numIntersections = 2;
                        output.overlap[0] = a0;
                        output.overlap[1] = a1;
                        output.type = Output::isFinite;
                    }
                    else  // a0 == a1
                    {
                        output.numIntersections = 1;
                        output.overlap[0] = a0;
                        output.overlap[1] = output.overlap[0];
                        output.type = Output::isPoint;
                    }
                }
            }
            else  // interval0 is negative-infinite
            {
                if (isPositiveInfinite1)
                {
                    if (a0 < a1)
                    {
                        output.numIntersections = 0;
                        output.overlap[0] = C_<T>(0);
                        output.overlap[1] = C_<T>(0);
                        output.type = Output::isEmpty;
                    }
                    else if (a0 > a1)
                    {
                        output.numIntersections = 2;
                        output.overlap[0] = a1;
                        output.overlap[1] = a0;
                        output.type = Output::isFinite;
                    }
                    else
                    {
                        output.numIntersections = 1;
                        output.overlap[0] = a1;
                        output.overlap[1] = output.overlap[0];
                        output.type = Output::isPoint;
                    }
                    output.intersect = (a0 >= a1);
                }
                else  // interval1 is negative-infinite
                {
                    // overlap[0] is -infinity, but set it to -1 because T
                    // might not have a representation for -infinity. The
                    // type indicates the interval is negative-infinite, so
                    // the -1 is a reminder that overlap[0] is -infinity.
                    output.numIntersections = 1;
                    output.overlap[0] = -C_<T>(1);
                    output.overlap[1] = std::min(a0, a1);
                    output.type = Output::isNegativeInfinite;
                }
            }

            output.intersect = (output.numIntersections > 0);
            return output;
        }

        // Dynamic query. The current time is 0.
        Output operator()(
            std::array<T, 2> const& interval0, T const& speed0,
            std::array<T, 2> const& interval1, T const& speed1)
        {
            Output output{};
            output.type = Output::isDynamicQuery;

            if (interval0[1] < interval1[0])
            {
                // interval0 initially to the left of interval1.
                T diffSpeed = speed0 - speed1;
                if (diffSpeed > C_<T>(0))
                {
                    // The intervals must move towards each other..
                    output.intersect = true;
                    output.numIntersections = 1;
                    output.firstTime = (interval1[0] - interval0[1]) / diffSpeed;
                    output.lastTime = (interval1[1] - interval0[0]) / diffSpeed;
                    output.overlap[0] = interval0[0] + output.firstTime * speed0;
                    output.overlap[1] = output.overlap[0];
                    return output;
                }
            }
            else if (interval0[0] > interval1[1])
            {
                // interval0 initially to the right of interval1.
                T diffSpeed = speed1 - speed0;
                if (diffSpeed > C_<T>(0))
                {
                    // The intervals must move towards each other.
                    output.intersect = true;
                    output.numIntersections = 1;
                    output.firstTime = (interval0[0] - interval1[1]) / diffSpeed;
                    output.lastTime = (interval0[1] - interval1[0]) / diffSpeed;
                    output.overlap[0] = interval1[1] + output.firstTime * speed1;
                    output.overlap[1] = output.overlap[0];
                    return output;
                }
            }
            else
            {
                // The intervals are initially intersecting.
                output.intersect = true;
                output.firstTime = C_<T>(0);
                if (speed1 > speed0)
                {
                    output.lastTime = (interval0[1] - interval1[0]) / (speed1 - speed0);
                }
                else if (speed1 < speed0)
                {
                    output.lastTime = (interval1[1] - interval0[0]) / (speed0 - speed1);
                }
                else
                {
                    // The intervals move in lock-step. Flag this for the
                    // caller by setting the last time to a number smaller
                    // than the first time.
                    output.lastTime = C_<T>(-1);
                }

                // Compute the overlap of the initial intervals as the set of
                // first contact.
                output.overlap = operator()(interval0, interval1).overlap;
                return output;
            }

            // The Output constructor sets the correct state for no-intersection.
            return output;
        }

    private:
        friend class UnitTestIntrIntervals;
    };
}
