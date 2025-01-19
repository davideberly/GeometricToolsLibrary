// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Utility/HashCombine.h>
using namespace gtl;

namespace gtl
{
    class UnitTestHashCombine
    {
    public:
        UnitTestHashCombine();

    private:
        // Nothing to test.
    };
}

UnitTestHashCombine::UnitTestHashCombine()
{
    UTInformation("Mathematics/Utility/HashCombine");

    // Nothing to test.
}

#else

#include <GTL/Utility/HashCombine.h>
#include <cstdint>

namespace gtl
{
    template void gtl::HashCombine(std::size_t&, std::int32_t const&);
    template void gtl::HashValue(std::size_t&, std::int32_t const&);
    template void gtl::HashValue(std::size_t&, std::int32_t const&, std::int32_t const&);
    template void gtl::HashValue(std::size_t&, std::int32_t const&, std::int32_t const&, std::int32_t const&);
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(HashCombine)
