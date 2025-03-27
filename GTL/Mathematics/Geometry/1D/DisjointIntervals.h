// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// Compute Boolean operations of disjoint sets of half-open intervals of the
// form [xmin,xmax) with xmin < xmax.

#include <GTL/Utility/Exceptions.h>
#include <cstddef>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T>
    class DisjointIntervals
    {
    public:
        DisjointIntervals()
            :
            mEndpoints{}
        {
        }

        DisjointIntervals(T const& xmin, T const& xmax)
            :
            mEndpoints{xmin, xmax}
        {
            GTL_ARGUMENT_ASSERT(
                xmin < xmax,
                "Invalid order of arguments.");
        }

        ~DisjointIntervals() = default;

        // Copy semantics.
        DisjointIntervals(DisjointIntervals const& other)
            :
            mEndpoints(other.mEndpoints)
        {
        }

        DisjointIntervals& operator=(DisjointIntervals const& other)
        {
            mEndpoints = other.mEndpoints;
            return *this;
        }

        // Move semantics.
        DisjointIntervals(DisjointIntervals&& other) noexcept
            :
            mEndpoints(std::move(other.mEndpoints))
        {
        }

        DisjointIntervals& operator=(DisjointIntervals&& other) noexcept
        {
            mEndpoints = std::move(other.mEndpoints);
            return *this;
        }

        // The number of intervals in the set.
        inline std::size_t GetNumIntervals() const
        {
            return mEndpoints.size() / 2;
        }

        std::vector<T> const& GetIntervals() const
        {
            return mEndpoints;
        }

        // The i-th interval is [xmin,xmax). The values xmin and xmax are
        // valid only when i < GetNumIntervals().
        void GetInterval(std::size_t i, T& xmin, T& xmax) const
        {
            std::size_t index = 2 * i;
            GTL_ARGUMENT_ASSERT(
                index < mEndpoints.size(),
                "Input index out of range.");

            xmin = mEndpoints[index];
            xmax = mEndpoints[++index];
        }

        // Make this set empty.
        inline void Clear()
        {
            mEndpoints.clear();
        }

        // Insert [xmin,xmax) into the set. This is a Boolean 'union'
        // operation.
        void Insert(T const& xmin, T const& xmax)
        {
            DisjointIntervals input(xmin, xmax);
            DisjointIntervals output = *this | input;
            mEndpoints = std::move(output.mEndpoints);
        }

        // Remove [xmin,xmax) from the set. This is a Boolean 'difference'
        // operation.
        void Remove(T const& xmin, T const& xmax)
        {
            DisjointIntervals input(xmin, xmax);
            DisjointIntervals output = *this - input;
            mEndpoints = std::move(output.mEndpoints);
        }

        // Get the union of the interval sets, input0 union input1.
        friend DisjointIntervals operator|(DisjointIntervals const& input0, DisjointIntervals const& input1)
        {
            DisjointIntervals output{};

            std::size_t const numEndpoints0 = input0.mEndpoints.size();
            std::size_t const numEndpoints1 = input1.mEndpoints.size();
            std::size_t i0 = 0, i1 = 0;
            std::size_t parity0 = 0, parity1 = 0;
            while (i0 < numEndpoints0 && i1 < numEndpoints1)
            {
                T const& value0 = input0.mEndpoints[i0];
                T const& value1 = input1.mEndpoints[i1];

                if (value0 < value1)
                {
                    if (parity0 == 0)
                    {
                        parity0 = 1;
                        if (parity1 == 0)
                        {
                            output.mEndpoints.push_back(value0);
                        }
                    }
                    else
                    {
                        if (parity1 == 0)
                        {
                            output.mEndpoints.push_back(value0);
                        }
                        parity0 = 0;
                    }
                    ++i0;
                }
                else if (value1 < value0)
                {
                    if (parity1 == 0)
                    {
                        parity1 = 1;
                        if (parity0 == 0)
                        {
                            output.mEndpoints.push_back(value1);
                        }
                    }
                    else
                    {
                        if (parity0 == 0)
                        {
                            output.mEndpoints.push_back(value1);
                        }
                        parity1 = 0;
                    }
                    ++i1;
                }
                else  // value0 == value1
                {
                    if (parity0 == parity1)
                    {
                        output.mEndpoints.push_back(value0);
                    }
                    parity0 ^= 1;
                    parity1 ^= 1;
                    ++i0;
                    ++i1;
                }
            }

            while (i0 < numEndpoints0)
            {
                output.mEndpoints.push_back(input0.mEndpoints[i0]);
                ++i0;
            }

            while (i1 < numEndpoints1)
            {
                output.mEndpoints.push_back(input1.mEndpoints[i1]);
                ++i1;
            }

            return output;
        }

        // Get the intersection of the interval sets, input0 intersect is1.
        friend DisjointIntervals operator&(DisjointIntervals const& input0, DisjointIntervals const& input1)
        {
            DisjointIntervals output{};

            std::size_t const numEndpoints0 = input0.mEndpoints.size();
            std::size_t const numEndpoints1 = input1.mEndpoints.size();
            std::size_t i0 = 0, i1 = 0;
            std::size_t parity0 = 0, parity1 = 0;
            while (i0 < numEndpoints0 && i1 < numEndpoints1)
            {
                T const& value0 = input0.mEndpoints[i0];
                T const& value1 = input1.mEndpoints[i1];

                if (value0 < value1)
                {
                    if (parity0 == 0)
                    {
                        parity0 = 1;
                        if (parity1 == 1)
                        {
                            output.mEndpoints.push_back(value0);
                        }
                    }
                    else
                    {
                        if (parity1 == 1)
                        {
                            output.mEndpoints.push_back(value0);
                        }
                        parity0 = 0;
                    }
                    ++i0;
                }
                else if (value1 < value0)
                {
                    if (parity1 == 0)
                    {
                        parity1 = 1;
                        if (parity0 == 1)
                        {
                            output.mEndpoints.push_back(value1);
                        }
                    }
                    else
                    {
                        if (parity0 == 1)
                        {
                            output.mEndpoints.push_back(value1);
                        }
                        parity1 = 0;
                    }
                    ++i1;
                }
                else  // value0 == value1
                {
                    if (parity0 == parity1)
                    {
                        output.mEndpoints.push_back(value0);
                    }
                    parity0 ^= 1;
                    parity1 ^= 1;
                    ++i0;
                    ++i1;
                }
            }

            return output;
        }

        // Get the differences of the interval sets, input0 minus input1.
        friend DisjointIntervals operator-(DisjointIntervals const& input0, DisjointIntervals const& input1)
        {
            DisjointIntervals output{};

            std::size_t const numEndpoints0 = input0.mEndpoints.size();
            std::size_t const numEndpoints1 = input1.mEndpoints.size();
            std::size_t i0 = 0, i1 = 0;
            std::size_t parity0 = 0, parity1 = 1;
            while (i0 < numEndpoints0 && i1 < numEndpoints1)
            {
                T const& value0 = input0.mEndpoints[i0];
                T const& value1 = input1.mEndpoints[i1];

                if (value0 < value1)
                {
                    if (parity0 == 0)
                    {
                        parity0 = 1;
                        if (parity1 == 1)
                        {
                            output.mEndpoints.push_back(value0);
                        }
                    }
                    else
                    {
                        if (parity1 == 1)
                        {
                            output.mEndpoints.push_back(value0);
                        }
                        parity0 = 0;
                    }
                    ++i0;
                }
                else if (value1 < value0)
                {
                    if (parity1 == 0)
                    {
                        parity1 = 1;
                        if (parity0 == 1)
                        {
                            output.mEndpoints.push_back(value1);
                        }
                    }
                    else
                    {
                        if (parity0 == 1)
                        {
                            output.mEndpoints.push_back(value1);
                        }
                        parity1 = 0;
                    }
                    ++i1;
                }
                else  // value0 == value1
                {
                    if (parity0 == parity1)
                    {
                        output.mEndpoints.push_back(value0);
                    }
                    parity0 ^= 1;
                    parity1 ^= 1;
                    ++i0;
                    ++i1;
                }
            }

            while (i0 < numEndpoints0)
            {
                output.mEndpoints.push_back(input0.mEndpoints[i0]);
                ++i0;
            }

            return output;
        }

        // Get the exclusive or of the interval sets, input0 xor input1 =
        // (input0 minus input1) or (input1 minus input0).
        friend DisjointIntervals operator^(DisjointIntervals const& input0, DisjointIntervals const& input1)
        {
            DisjointIntervals output{};

            std::size_t const numEndpoints0 = input0.mEndpoints.size();
            std::size_t const numEndpoints1 = input1.mEndpoints.size();
            std::size_t i0 = 0, i1 = 0;
            while (i0 < numEndpoints0 && i1 < numEndpoints1)
            {
                T const& value0 = input0.mEndpoints[i0];
                T const& value1 = input1.mEndpoints[i1];

                if (value0 < value1)
                {
                    output.mEndpoints.push_back(value0);
                    ++i0;
                }
                else if (value1 < value0)
                {
                    output.mEndpoints.push_back(value1);
                    ++i1;
                }
                else  // value0 == value1
                {
                    ++i0;
                    ++i1;
                }
            }

            while (i0 < numEndpoints0)
            {
                output.mEndpoints.push_back(input0.mEndpoints[i0]);
                ++i0;
            }

            while (i1 < numEndpoints1)
            {
                output.mEndpoints.push_back(input1.mEndpoints[i1]);
                ++i1;
            }

            return output;
        }

    private:
        // The array of endpoints has an even number of elements. The i-th
        // interval is [mEndPoints[2 * i], mEndPoints[2 * i + 1]).
        std::vector<T> mEndpoints;

    private:
        friend class UnitTestDisjointIntervals;
    };
}
