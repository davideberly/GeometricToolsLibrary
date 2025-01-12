// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Utility/RangeIteration.h>
#include <vector>
using namespace gtl;

namespace gtl
{
    class UnitTestRangeIteration
    {
    public:
        UnitTestRangeIteration();

    private:
        void Test();
    };
}

UnitTestRangeIteration::UnitTestRangeIteration()
{
    UTInformation("Mathematics/Utility/RangeIteration");

    Test();
}

void UnitTestRangeIteration::Test()
{
    std::vector<std::int32_t> numbers = { 0, 1, 2, 3 };
    std::size_t const numElements = numbers.size();
    std::vector<std::int32_t> reversed(numElements);
    std::size_t i = 0;
    for (auto number : reverse(numbers))
    {
        reversed[i] = number;
        ++i;
    }

    for (std::size_t i0 = 0, i1 = numElements - 1; i0 < numElements; ++i0, --i1)
    {
        UTAssert(reversed[i0] == numbers[i1], "Invalid ordering.");
    }
}
#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(RangeIteration)

