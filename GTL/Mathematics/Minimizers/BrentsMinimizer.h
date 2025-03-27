// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// Search for a minimum of F(t) on [t0,t1] using successive parabolic
// interpolation. The search is recursive based on the polyline associated
// with (t,F(t)) at the endpoints and the midpoint of an interval. Let
// f0 = F(t0), f1 = F(t1), tm is in (t0,t1) and fm = F(tm). The polyline
// is {(t0,f0),(tm,fm),(t1,f1)}.
//
// If the polyline is V-shaped, the interval [t0,t1] contains a minimum
// point. The polyline is fit with a parabola whose vertex tv is in
// (t0,t1). Let fv = tv. If {(t0,f0),(tv,fv),(tm,fm)}} is a minimum
// bracket, the parabolic interpolation continues in [t0,tm]. If instead
// {(tm,fm),(tv,fv),(t1,f1)}} is a minimum bracket, the parabolic
// interpolation continues in [tm,t1].
//
// If the polyline is not V-shaped, both subintervals [t0,tm] and [tm,t1]
// are searched for a minimum.

#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Utility/Exceptions.h>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>

namespace gtl
{
    template <typename T>
    class BrentsMinimizer
    {
    public:
        BrentsMinimizer(std::size_t maxSubdivisions, std::size_t maxBisections,
            T const& epsilon, T const& tolerance)
            :
            mFunction{},
            mMaxSubdivisions(maxSubdivisions),
            mMaxBisections(maxBisections),
            mNumBisections(0),
            mTMin(C_<T>(0)),
            mFMin(C_<T>(0)),
            mEpsilon(epsilon),
            mTolerance(tolerance)
        {
            GTL_ARGUMENT_ASSERT(
                mMaxSubdivisions > 0 &&
                mMaxBisections > 0 &&
                mEpsilon >= C_<T>(0) &&
                mTolerance >= C_<T>(0),
                "Invalid argument.");
        }

        // Member access.
        inline void SetMaxSubdivisions(std::size_t maxSubdivisions)
        {
            GTL_ARGUMENT_ASSERT(
                maxSubdivisions > 0,
                "The maximum number of subdivisions must be positive.");

            mMaxSubdivisions = maxSubdivisions;
        }

        inline void SetMaxBisections(std::size_t maxBisections)
        {
            GTL_ARGUMENT_ASSERT(
                maxBisections > 0,
                "The maximum number of bisections must be positive.");

            mMaxBisections = maxBisections;
        }

        inline void SetEpsilon(T const& epsilon)
        {
            GTL_ARGUMENT_ASSERT(
                epsilon >= C_<T>(0),
                "The epsilon threshold must be nonnegative.");

            mEpsilon = epsilon;
        }

        inline void SetTolerance(T const& tolerance)
        {
            GTL_ARGUMENT_ASSERT(
                tolerance >= C_<T>(0),
                "The tolerance threshold must be nonnegative.");

            mTolerance = tolerance;
        }

        inline std::size_t GetMaxSubdivisions() const
        {
            return mMaxSubdivisions;
        }

        inline std::size_t GetMaxBisections() const
        {
            return mMaxBisections;
        }

        inline T const& GetEpsilon() const
        {
            return mEpsilon;
        }

        inline T const& GetTolerance() const
        {
            return mTolerance;
        }

        // Get the number of bisections used in the recursive call to
        // Subdivide.
        inline std::size_t GetNumBisections() const
        {
            return mNumBisections;
        }

        // Search for a minimum of F(t) on the interval [t0,t1] using an
        // initial guess of (t0+t1)/2. The location of the minimum is tMin
        // and the value of the minimum is fMin = F(tMin).
        void operator()(std::function<T(T const&)> const& F,
            T const& t0, T const& t1, T& tMin, T& fMin)
        {
            T tInitial = C_<T>(1, 2) * (t0 + t1);
            operator()(F, t0, t1, tInitial, tMin, fMin);
        }

        // Search for a minimum of F(t) on the interval [t0,t1] using an
        // initial guess of tInitial. The location of the minimum is tMin
        // and the value of the minimum is fMin = F(tMin).
        void operator()(std::function<T(T const&)> const& F, T const& t0,
            T const& t1, T const& tInitial, T& tMin, T& fMin)
        {
            GTL_ARGUMENT_ASSERT(
                t0 <= tInitial && tInitial <= t1,
                "Invalid initial t value.");

            mFunction = F;

            // Compute the minimum for the 3 initial points.
            T f0 = mFunction(t0);
            mTMin = t0;
            mFMin = f0;

            T fInitial = mFunction(tInitial);
            if (fInitial < mFMin)
            {
                mTMin = tInitial;
                mFMin = fInitial;
            }

            T f1 = mFunction(t1);
            if (f1 < mFMin)
            {
                mTMin = t1;
                mFMin = f1;
            }

            // Search for the global minimum on [t0,t1] with tInitial chosen
            // hopefully to start with a minimum bracket.
            mNumBisections = 0;
            if (((fInitial < f0) && (f1 >= fInitial)) ||
                ((f1 > fInitial) && (f0 >= fInitial)))
            {
                // The polyline {(f0,f0), (tInitial,fInitial), (t1,f1)} is V-shaped.
                GetBracketedMinimum(t0, f0, tInitial, fInitial, t1, f1);
            }
            else
            {
                // The polyline {(f0,f0), (tInitial,fInitial), (t1,f1)} is not
                // V-shaped, so continue searching in subintervals
                // [t0,tInitial] and [tInitial,t1].
                Subdivide(t0, f0, tInitial, fInitial, mMaxSubdivisions);
                Subdivide(tInitial, fInitial, t1, f1, mMaxSubdivisions);
            }

            tMin = mTMin;
            fMin = mFMin;
        }

    private:
        // Search [t0,t1] recursively for a global minimum.
        void Subdivide(T const& t0, T const& f0, T const& t1, T const& f1,
            std::size_t subdivisionsRemaining)
        {
            if (subdivisionsRemaining-- == 0)
            {
                // The maximum number of subdivisions has been reached.
                return;
            }

            // Compute the function at the midpoint of [t0,t1].
            T tm = C_<T>(1, 2) * (t0 + t1);
            T fm = mFunction(tm);
            if (fm < mFMin)
            {
                mTMin = tm;
                mFMin = fm;
            }

            if (((fm < f0) && (f1 >= fm)) || ((f1 > fm) && (f0 >= fm)))
            {
                // The polyline {(f0,f0), (tm,fm), (t1,f1)} is V-shaped.
                GetBracketedMinimum(t0, f0, tm, fm, t1, f1);
            }
            else
            {
                // The polyline {(f0,f0), (tm,fm), (t1,f1)} is not V-shaped,
                // so continue searching in subintervals [t0,tm] and [tm,t1].
                Subdivide(t0, f0, tm, fm, subdivisionsRemaining);
                Subdivide(tm, fm, t1, f1, subdivisionsRemaining);
            }
        }

        // This is called when {f0,f1,f2} brackets a minimum.
        void GetBracketedMinimum(T t0, T f0, T tm, T fm, T t1, T f1)
        {
            std::size_t i{};
            for (i = 0; i < mMaxBisections; ++i)
            {
                // Update the minimum location and value.
                if (fm < mFMin)
                {
                    mTMin = tm;
                    mFMin = fm;
                }

                // Test for convergence.
                T dt10 = t1 - t0;
                T dtBound = C_<T>(2) * mTolerance * std::fabs(tm) + mEpsilon;
                if (dt10 <= dtBound)
                {
                    break;
                }

                // Compute the vertex of the interpolating parabola.
                T dt0m = t0 - tm;
                T dt1m = t1 - tm;
                T df0m = f0 - fm;
                T df1m = f1 - fm;
                T tmp0 = dt0m * df1m;
                T tmp1 = dt1m * df0m;
                T denom = tmp1 - tmp0;
                if (std::fabs(denom) <= mEpsilon)
                {
                    break;
                }

                // Compute tv and clamp to [t0,t1] to offset floating-point
                // rounding errors.
                T tv = tm + C_<T>(1, 2) * (dt1m * tmp1 - dt0m * tmp0) / denom;
                tv = std::max(t0, std::min(tv, t1));
                T fv = mFunction(tv);
                if (fv < mFMin)
                {
                    mTMin = tv;
                    mFMin = fv;
                }

                if (tv < tm)
                {
                    if (fv < fm)
                    {
                        t1 = tm;
                        f1 = fm;
                        tm = tv;
                        fm = fv;
                    }
                    else
                    {
                        t0 = tv;
                        f0 = fv;
                    }
                }
                else if (tv > tm)
                {
                    if (fv < fm)
                    {
                        t0 = tm;
                        f0 = fm;
                        tm = tv;
                        fm = fv;
                    }
                    else
                    {
                        t1 = tv;
                        f1 = fv;
                    }
                }
                else
                {
                    // The vertex of parabola is located at the middle sample
                    // point. A minimum could occur on either subinterval, but
                    // it is also possible the minimum occurs at the vertex.
                    // In either case, the search is continued by examining a
                    // neighborhood of the vertex. When two choices exist for
                    // a bracket, the one with the smallest function value at
                    // the midpoint is used.
                    T tm0 = C_<T>(1, 2) * (t0 + tm);
                    T fm0 = mFunction(tm0);
                    T tm1 = C_<T>(1, 2) * (tm + t1);
                    T fm1 = mFunction(tm1);

                    if (fm0 < fm)
                    {
                        if (fm1 < fm)
                        {
                            if (fm0 < fm1)
                            {
                                // {(t0,f0),(tm0,fm0),(tm,fm)}
                                t1 = tm;
                                f1 = fm;
                                tm = tm0;
                                fm = fm0;
                            }
                            else
                            {
                                // {(tm,fm),(tm1,fm1),(t1,f1)}
                                t0 = tm;
                                f0 = fm;
                                tm = tm1;
                                fm = fm1;
                            }
                        }
                        else // fm1 >= fm
                        {
                            // {(t0,f0),(tm0,fm0),(tm,fm)}
                            t1 = tm;
                            f1 = fm;
                            tm = tm0;
                            fm = fm0;
                        }
                    }
                    else if (fm0 > fm)
                    {
                        if (fm1 < fm)
                        {
                            // {(tm,fm),(tm1,fm1),(t1,f1)}
                            t0 = tm;
                            f0 = fm;
                            tm = tm1;
                            fm = fm1;
                        }
                        else // fm1 >= fm
                        {
                            // {(tm0,fm0),(tm,fm),(tm1,fm1)}
                            t0 = tm0;
                            f0 = fm0;
                            t1 = tm1;
                            f1 = fm1;
                        }
                    }
                    else  // fm0 = fm
                    {
                        if (fm1 < fm)
                        {
                            // {(tm,fm),(tm1,fm1),(t1,f1)}
                            t0 = tm;
                            f0 = fm;
                            tm = tm1;
                            fm = fm1;
                        }
                        else // fm1 >= fm
                        {
                            // {(tm0,fm0),(tm,fm),(tm1,fm1)}
                            t0 = tm0;
                            f0 = fm0;
                            t1 = tm1;
                            f1 = fm1;
                        }
                    }
                }
            }

            mNumBisections += i;
        }

        std::function<T(T)> mFunction;
        std::size_t mMaxSubdivisions;
        std::size_t mMaxBisections;
        std::size_t mNumBisections;
        T mTMin, mFMin;
        T mEpsilon, mTolerance;

    private:
        friend class UnitTestBrentsMinimizer;
    };
}
