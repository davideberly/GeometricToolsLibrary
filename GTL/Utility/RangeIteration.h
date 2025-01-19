// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#pragma once

// Documentation for this class is
// https://www.geometrictools.com/Documentation/GTLUtility.pdf#RangeIteration

#include <iterator>
#include <utility>

namespace gtl
{
    template <typename Iterator>
    class ReversalObject
    {
    public:
        ReversalObject(Iterator inBegin, Iterator inEnd)
            :
            mBegin(inBegin),
            mEnd(inEnd)
        {
        }

        inline Iterator begin() const
        {
            return mBegin;
        }

        inline Iterator end() const
        {
            return mEnd;
        }

    private:
        Iterator mBegin, mEnd;

    private:
        friend class UnitTestRangeIteration;
    };

    // The reverse iteration is illustrated by the following example.
    //   std::vector<std::size_t> numbers = { a list of numbers };
    //   for (auto const& number : gtl::reverse(numbers))
    //   {
    //       <do something with the number>;
    //   }
    template
        <
        typename Iterable,
        typename Iterator = decltype(std::begin(std::declval<Iterable>())),
        typename ReverseIterator = std::reverse_iterator<Iterator>
        >
    ReversalObject<ReverseIterator> reverse(Iterable&& range)
    {
        return ReversalObject<ReverseIterator>(
            ReverseIterator(std::end(range)),
            ReverseIterator(std::begin(range)));
    }
}
