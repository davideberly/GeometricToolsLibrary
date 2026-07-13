// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

#pragma once

// The topic of fast marching methods are discussed in the book
//   Level Set Methods and Fast Marching Methods:
//     Evolving Interfaces in Computational Geometry, Fluid Mechanics,
//     Computer Vision, and Materials Science
//   J.A. Sethian,
//   Cambridge University Press, 1999
//
// FastMarch is the abstract base class for FastMarch2 and FastMarch3.

#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Utility/MinHeap.h>
#include <cstddef>
#include <limits>
#include <vector>

namespace gtl
{
    template <typename T>
    class FastMarch
    {
        // Abstract base class.
    public:
        virtual ~FastMarch() = default;

    protected:
        // The seed points have a crossing time of 0. As the iterations occur,
        // some of the non-seed points are visited by the moving front. Define
        // maxReal to be std::numeric_limits<T>::max(). The valid crossing
        // times are 0 <= t < maxReal. A value of maxReal indicates the pixel
        // has not yet been reached by the moving front. If the speed value at
        // a pixel is 0, the pixel is marked with a time of -maxReal. Such
        // pixels can never be visited; the minus sign distinguishes these
        // from pixels not yet reached during iteration.
        //
        // Trial pixels are identified by having min-heap records associated
        // with them. Known or far pixels have no associated record.
        //
        // The speeds must be nonnegative and are inverted because the
        // reciprocals are all that are needed in the numerical method.

        FastMarch(std::size_t quantity, std::vector<std::size_t> const& seeds,
            std::vector<T> const& speeds)
            :
            mQuantity(quantity),
            mTimes(quantity, std::numeric_limits<T>::max()),
            mInvSpeeds(quantity),
            mHeap(quantity),
            mTrials(quantity, MinHeap<T>::invalid)
        {
            static_assert(std::is_floating_point<T>::value,
                "The template type must be 'float' or 'double'.");

            for (auto seed : seeds)
            {
                mTimes[seed] = C_<T>(0);
            }

            for (std::size_t i = 0; i < mQuantity; ++i)
            {
                if (speeds[i] > C_<T>(0))
                {
                    mInvSpeeds[i] = C_<T>(1) / speeds[i];
                }
                else
                {
                    mInvSpeeds[i] = std::numeric_limits<T>::max();
                    mTimes[i] = -std::numeric_limits<T>::max();
                }
            }
        }

        FastMarch(std::size_t quantity, std::vector<std::size_t> const& seeds, T const& speed)
            :
            mQuantity(quantity),
            mTimes(quantity, std::numeric_limits<T>::max()),
            mInvSpeeds(quantity, C_<T>(1) / speed),
            mHeap(quantity),
            mTrials(quantity, MinHeap<T>::invalid)
        {
            static_assert(std::is_floating_point<T>::value,
                "The template type must be 'float' or 'double'.");

            for (auto seed : seeds)
            {
                mTimes[seed] = C_<T>(0);
            }
        }

    public:
        // Member access.
        inline std::size_t GetQuantity() const
        {
            return mQuantity;
        }

        inline void SetTime(std::size_t i, T const& time)
        {
            mTimes[i] = time;
        }

        inline T const& GetTime(std::size_t i) const
        {
            return mTimes[i];
        }

        void GetTimeExtremes(T& minValue, T& maxValue) const
        {
            minValue = std::numeric_limits<T>::max();
            maxValue = -std::numeric_limits<T>::max();
            std::size_t i{};
            for (i = 0; i < mQuantity; ++i)
            {
                if (IsValid(i))
                {
                    minValue = mTimes[i];
                    maxValue = minValue;
                    break;
                }
            }

            // Assert:  At least one time must be valid, in which case
            // i < mQuantity at this point. If all times are invalid,
            // minValue = +maxReal and maxValue = -maxReal on exit.

            for ( ; i < mQuantity; ++i)
            {
                if (IsValid(i))
                {
                    if (mTimes[i] < minValue)
                    {
                        minValue = mTimes[i];
                    }
                    else if (mTimes[i] > maxValue)
                    {
                        maxValue = mTimes[i];
                    }
                }
            }
        }

        // Image element classification.
        inline bool IsValid(std::size_t i) const
        {
            return C_<T>(0) <= mTimes[i]
                && mTimes[i] < std::numeric_limits<T>::max();
        }

        inline bool IsTrial(std::size_t i) const
        {
            return mTrials[i] != MinHeap<T>::invalid;
        }

        inline bool IsFar(std::size_t i) const
        {
            return mTimes[i] == std::numeric_limits<T>::max();
        }

        inline bool IsZeroSpeed(std::size_t i) const
        {
            return mTimes[i] == -std::numeric_limits<T>::max();
        }

        inline bool IsInterior(std::size_t i) const
        {
            return IsValid(i) && !IsTrial(i);
        }

        void GetInterior(std::vector<std::size_t>& interior) const
        {
            interior.clear();
            for (std::size_t i = 0; i < mQuantity; ++i)
            {
                if (IsValid(i) && !IsTrial(i))
                {
                    interior.push_back(i);
                }
            }
        }

        virtual void GetBoundary(std::vector<std::size_t>& boundary) const = 0;
        virtual bool IsBoundary(std::size_t i) const = 0;

        // Run one step of the fast marching algorithm.
        virtual void Iterate() = 0;

    protected:
        std::size_t mQuantity;
        std::vector<T> mTimes;
        std::vector<T> mInvSpeeds;
        MinHeap<T> mHeap;
        std::vector<std::size_t> mTrials;

    private:
        friend class UnitTestFastMarch;
    };
}
