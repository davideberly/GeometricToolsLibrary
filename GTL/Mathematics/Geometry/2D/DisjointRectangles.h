// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// Compute Boolean operations of disjoint sets of half-open rectangles of the
// form [xmin,xmax)x[ymin,ymax) with xmin < xmax and ymin < ymax.

#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Mathematics/Geometry/1D/DisjointIntervals.h>
#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T>
    class DisjointRectangles
    {
    public:
        // Convenient type definition.
        using ISet = DisjointIntervals<T>;

        DisjointRectangles()
            :
            mNumRectangles(0),
            mStrips{}
        {
        }

        DisjointRectangles(T const& xmin, T const& xmax, T const& ymin, T const& ymax)
        {
            GTL_ARGUMENT_ASSERT(
                xmin < xmax && ymin < ymax,
                "Invalid input ordering.");

            mNumRectangles = 1;
            mStrips.push_back(Strip(ymin, ymax, ISet(xmin, xmax)));
        }

        ~DisjointRectangles() = default;

        // Copy semantics.
        DisjointRectangles(DisjointRectangles const& other)
        {
            *this = other;
        }

        DisjointRectangles& operator=(DisjointRectangles const& other)
        {
            mNumRectangles = other.mNumRectangles;
            mStrips = other.mStrips;
            return *this;
        }

        // Move semantics.
        DisjointRectangles(DisjointRectangles&& other) noexcept
        {
            *this = std::move(other);
        }

        DisjointRectangles& operator=(DisjointRectangles&& other) noexcept
        {
            mNumRectangles = other.mNumRectangles;
            mStrips = std::move(other.mStrips);
            return *this;
        }

        // The rectangle set consists of y-strips of interval sets.
        class Strip
        {
        public:
            // Construction and destruction.
            Strip()
                :
                ymin(C_<T>(0)),
                ymax(C_<T>(0)),
                intervalSet{}
            {
            }

            Strip(T const& inYMin, T const& inYMax, ISet const& inIntervalSet)
                :
                ymin(inYMin),
                ymax(inYMax),
                intervalSet(inIntervalSet)
            {
            }

            ~Strip() = default;

            // Copy semantics.
            Strip(Strip const& other)
                :
                ymin(C_<T>(0)),
                ymax(C_<T>(0)),
                intervalSet{}
            {
                *this = other;
            }

            Strip& operator=(Strip const& other)
            {
                ymin = other.ymin;
                ymax = other.ymax;
                intervalSet = other.intervalSet;
                return *this;
            }

            // Move semantics.
            Strip(Strip&& other) noexcept
                :
                ymin(C_<T>(0)),
                ymax(C_<T>(0)),
                intervalSet{}
            {
                *this = std::move(other);
            }

            Strip& operator=(Strip&& other) noexcept
            {
                ymin = other.ymin;
                ymax = other.ymax;
                intervalSet = std::move(other.intervalSet);
                other.ymin = C_<T>(0);
                other.ymax = C_<T>(0);
                return *this;
            }

            // Member access.
            T ymin, ymax;
            ISet intervalSet;
        };

        // The number of rectangles in the set.
        inline std::size_t GetNumRectangles() const
        {
            return mNumRectangles;
        }

        // The i-th rectangle is [xmin,xmax)x[ymin,ymax).  The values xmin,
        // xmax, ymin and ymax are valid when 0 <= i < GetNumRectangles().
        bool GetRectangle(std::size_t i, T& xmin, T& xmax, T& ymin, T& ymax) const
        {
            std::size_t totalQuantity = 0;
            for (auto const& strip : mStrips)
            {
                ISet const& intervalSet = strip.intervalSet;
                std::size_t xQuantity = intervalSet.GetNumIntervals();
                std::size_t nextTotalQuantity = totalQuantity + xQuantity;
                if (i < nextTotalQuantity)
                {
                    i -= totalQuantity;
                    intervalSet.GetInterval(i, xmin, xmax);
                    ymin = strip.ymin;
                    ymax = strip.ymax;
                    return true;
                }
                totalQuantity = nextTotalQuantity;
            }
            return false;  // TODO: Can this line be reached?
        }

        // Make this set empty.
        inline void Clear()
        {
            mNumRectangles = 0;
            mStrips.clear();
        }

        // The number of y-strips in the set.
        inline std::size_t GetNumStrips() const
        {
            return mStrips.size();
        }

        // Get the i-th strip.
        void GetStrip(std::size_t i, T& ymin, T& ymax, ISet& xIntervalSet) const
        {
            GTL_OUTOFRANGE_ASSERT(
                i < GetNumStrips(),
                "Index out of range.");

            Strip const& strip = mStrips[i];
            ymin = strip.ymin;
            ymax = strip.ymax;
            xIntervalSet = strip.intervalSet;
        }

        // Insert [xmin,xmax)x[ymin,ymax) into the set. This is a Boolean
        // union operation.
        void Insert(T const& xmin, T const& xmax, T const& ymin, T const& ymax)
        {
            GTL_RUNTIME_ASSERT(
                xmin < xmax && ymin < ymax,
                "Invalid ordering.");

            DisjointRectangles input(xmin, xmax, ymin, ymax);
            DisjointRectangles output = *this | input;
            *this = std::move(output);
        }

        // Remove [xmin,xmax)x[ymin,ymax) from the set. This is a Boolean
        // difference operation.
        void Remove(T const& xmin, T const& xmax, T const& ymin, T const& ymax)
        {
            GTL_RUNTIME_ASSERT(
                xmin < xmax && ymin < ymax,
                "Invalid ordering.");

            DisjointRectangles input(xmin, xmax, ymin, ymax);
            DisjointRectangles output = *this - input;
            *this = std::move(output);
        }

        // Get the union of the rectangle sets sets, input0 union input1.
        friend DisjointRectangles operator|(DisjointRectangles const& input0, DisjointRectangles const& input1)
        {
            return Execute(
                [](ISet const& i0, ISet const& i1) { return i0 | i1; },
                true, true, input0, input1);
        }

        // Get the intersection of the rectangle sets, input0 intersect is1.
        friend DisjointRectangles operator&(DisjointRectangles const& input0, DisjointRectangles const& input1)
        {
            return Execute(
                [](ISet const& i0, ISet const& i1) { return i0 & i1; },
                false, false, input0, input1);
        }

        // Get the differences of the rectangle sets, input0 minus input1.
        friend DisjointRectangles operator-(DisjointRectangles const& input0, DisjointRectangles const& input1)
        {
            return Execute(
                [](ISet const& i0, ISet const& i1) { return i0 - i1; },
                false, true, input0, input1);
        }

        // Get the exclusive or of the rectangle sets, input0 xor input1 =
        // (input0 minus input1) or (input1 minus input0).
        friend DisjointRectangles operator^(DisjointRectangles const& input0, DisjointRectangles const& input1)
        {
            return Execute(
                [](ISet const& i0, ISet const& i1) { return i0 ^ i1; },
                true, true, input0, input1);
        }

    private:
        static DisjointRectangles Execute(
            std::function<ISet(ISet const&, ISet const&)> const& operation,
            bool unionExclusiveOr, bool unionExclusiveOrDifference,
            DisjointRectangles const& input0, DisjointRectangles const& input1)
        {
            DisjointRectangles output{};

            std::size_t const numStrips0 = input0.GetNumStrips();
            std::size_t const numStrips1 = input1.GetNumStrips();
            std::size_t i0 = 0, i1 = 0;
            bool getOriginal0 = true, getOriginal1 = true;
            T ymin0 = C_<T>(0);
            T ymax0 = C_<T>(0);
            T ymin1 = C_<T>(0);
            T ymax1 = C_<T>(0);

            while (i0 < numStrips0 && i1 < numStrips1)
            {
                ISet const& intr0 = input0.mStrips[i0].intervalSet;
                if (getOriginal0)
                {
                    ymin0 = input0.mStrips[i0].ymin;
                    ymax0 = input0.mStrips[i0].ymax;
                }

                ISet const& intr1 = input1.mStrips[i1].intervalSet;
                if (getOriginal1)
                {
                    ymin1 = input1.mStrips[i1].ymin;
                    ymax1 = input1.mStrips[i1].ymax;
                }

                // Case 1.
                if (ymax1 <= ymin0)
                {
                    // operator(empty,strip1)
                    if (unionExclusiveOr)
                    {
                        output.mStrips.push_back(Strip(ymin1, ymax1, intr1));
                    }

                    ++i1;
                    getOriginal0 = false;
                    getOriginal1 = true;
                    continue;  // using next ymin1/ymax1
                }

                // Case 11.
                if (ymin1 >= ymax0)
                {
                    // operator(strip0,empty)
                    if (unionExclusiveOrDifference)
                    {
                        output.mStrips.push_back(Strip(ymin0, ymax0, intr0));
                    }

                    ++i0;
                    getOriginal0 = true;
                    getOriginal1 = false;
                    continue;  // using next ymin0/ymax0
                }

                // Reduce cases 2, 3, 4 to cases 5, 6, 7.
                if (ymin1 < ymin0)
                {
                    // operator(empty,[ymin1,ymin0))
                    if (unionExclusiveOr)
                    {
                        output.mStrips.push_back(Strip(ymin1, ymin0, intr1));
                    }

                    ymin1 = ymin0;
                    getOriginal1 = false;
                }

                // Reduce cases 8, 9, 10 to cases 5, 6, 7.
                if (ymin1 > ymin0)
                {
                    // operator([ymin0,ymin1),empty)
                    if (unionExclusiveOrDifference)
                    {
                        output.mStrips.push_back(Strip(ymin0, ymin1, intr0));
                    }

                    ymin0 = ymin1;
                    getOriginal0 = false;
                }

                // Case 5.
                if (ymax1 < ymax0)
                {
                    // operator(strip0,[ymin1,ymax1))
                    auto result = operation(intr0, intr1);
                    output.mStrips.push_back(Strip(ymin1, ymax1, result));

                    ymin0 = ymax1;
                    ++i1;
                    getOriginal0 = false;
                    getOriginal1 = true;
                    continue;  // using next ymin1/ymax1
                }

                // Case 6.
                if (ymax1 == ymax0)
                {
                    // operator(strip0,[ymin1,ymax1))
                    auto result = operation(intr0, intr1);
                    output.mStrips.push_back(Strip(ymin1, ymax1, result));

                    ++i0;
                    ++i1;
                    getOriginal0 = true;
                    getOriginal1 = true;
                    continue;  // using next ymin0/ymax0 and ymin1/ymax1
                }

                // Case 7.
                if (ymax1 > ymax0)
                {
                    // operator(strip0,[ymin1,ymax0))
                    auto result = operation(intr0, intr1);
                    output.mStrips.push_back(Strip(ymin1, ymax0, result));

                    ymin1 = ymax0;
                    ++i0;
                    getOriginal0 = true;
                    getOriginal1 = false;
                    // continue;  using current ymin1/ymax1
                }
            }

            if (unionExclusiveOrDifference)
            {
                while (i0 < numStrips0)
                {
                    if (getOriginal0)
                    {
                        ymin0 = input0.mStrips[i0].ymin;
                        ymax0 = input0.mStrips[i0].ymax;
                    }
                    else
                    {
                        getOriginal0 = true;
                    }

                    // operator(strip0,empty)
                    output.mStrips.push_back(Strip(ymin0, ymax0,
                        input0.mStrips[i0].intervalSet));

                    ++i0;
                }
            }

            if (unionExclusiveOr)
            {
                while (i1 < numStrips1)
                {
                    if (getOriginal1)
                    {
                        ymin1 = input1.mStrips[i1].ymin;
                        ymax1 = input1.mStrips[i1].ymax;
                    }
                    else
                    {
                        getOriginal1 = true;
                    }

                    // operator(empty,strip1)
                    output.mStrips.push_back(Strip(ymin1, ymax1,
                        input1.mStrips[i1].intervalSet));

                    ++i1;
                }
            }

            output.ComputeRectangleQuantity();
            return output;
        }

        void ComputeRectangleQuantity()
        {
            mNumRectangles = 0;
            for (auto strip : mStrips)
            {
                mNumRectangles += strip.intervalSet.GetNumIntervals();
            }
        }

        // The number of rectangles in the set.
        std::size_t mNumRectangles;

        // The y-strips of the set, each containing an x-interval set.
        std::vector<Strip> mStrips;

    private:
        friend class UnitTestDisjointRectangles;
    };
}
