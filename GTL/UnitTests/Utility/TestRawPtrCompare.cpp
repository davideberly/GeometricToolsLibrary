// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Utility/RawPtrCompare.h>
using namespace gtl;

namespace gtl
{
    class UnitTestRawPtrCompare
    {
    public:
        UnitTestRawPtrCompare();

    private:
        void Test();
    };
}

UnitTestRawPtrCompare::UnitTestRawPtrCompare()
{
    UTInformation("Mathematics/Utility/RawPtrCompare");

    Test();
}

void UnitTestRawPtrCompare::Test()
{
    std::int32_t v0 = 0, v1 = 1, v2 = 1;
    auto p0 = &v0;
    auto p1 = &v1;
    auto p2 = &v2;
    bool result;

    result = RawPtrEQ<std::int32_t>()(p0, p1);
    UTAssert(result == false, "Invalid EQ result (p0,p1)");
    result = RawPtrNE<std::int32_t>()(p0, p1);
    UTAssert(result == true, "Invalid NE result (p0,p1)");
    result = RawPtrLT<std::int32_t>()(p0, p1);
    UTAssert(result == true, "Invalid LT result (p0,p1)");
    result = RawPtrLE<std::int32_t>()(p0, p1);
    UTAssert(result == true, "Invalid LE result (p0,p1)");
    result = RawPtrGT<std::int32_t>()(p0, p1);
    UTAssert(result == false, "Invalid GT result (p0,p1)");
    result = RawPtrGE<std::int32_t>()(p0, p1);
    UTAssert(result == false, "Invalid GE result (p0,p1)");

    result = RawPtrEQ<std::int32_t>()(p1, p2);
    UTAssert(result == true, "Invalid EQ result (p1,p2)");
    result = RawPtrNE<std::int32_t>()(p1, p2);
    UTAssert(result == false, "Invalid NE result (p1,p2)");
    result = RawPtrLT<std::int32_t>()(p1, p2);
    UTAssert(result == false, "Invalid LT result (p1,p2)");
    result = RawPtrLE<std::int32_t>()(p1, p2);
    UTAssert(result == true, "Invalid LE result (p1,p2)");
    result = RawPtrGT<std::int32_t>()(p1, p2);
    UTAssert(result == false, "Invalid GT result (p1,p2)");
    result = RawPtrGE<std::int32_t>()(p1, p2);
    UTAssert(result == true, "Invalid GE result (p1,p2)");

    result = RawPtrEQ<std::int32_t>()(p1, p0);
    UTAssert(result == false, "Invalid EQ result (p1,p0)");
    result = RawPtrNE<std::int32_t>()(p1, p0);
    UTAssert(result == true, "Invalid NE result (p1,p0)");
    result = RawPtrLT<std::int32_t>()(p1, p0);
    UTAssert(result == false, "Invalid LT result (p1,p0)");
    result = RawPtrLE<std::int32_t>()(p1, p0);
    UTAssert(result == false, "Invalid LE result (p1,p0)");
    result = RawPtrGT<std::int32_t>()(p1, p0);
    UTAssert(result == true, "Invalid GT result (p1,p0)");
    result = RawPtrGE<std::int32_t>()(p1, p0);
    UTAssert(result == true, "Invalid GE result (p1,p0)");

    result = RawPtrEQ<std::int32_t>()(nullptr, p1);
    UTAssert(result == false, "Invalid EQ result (nullptr,p1)");
    result = RawPtrEQ<std::int32_t>()(p0, nullptr);
    UTAssert(result == false, "Invalid EQ result (p0,nullptr)");
    result = RawPtrEQ<std::int32_t>()(nullptr, nullptr);
    UTAssert(result == true, "Invalid EQ result (nullptr,nullptr)");

    result = RawPtrLT<std::int32_t>()(nullptr, p1);
    UTAssert(result == true, "Invalid LT result (nullptr,p1)");
    result = RawPtrLT<std::int32_t>()(p0, nullptr);
    UTAssert(result == false, "Invalid LT result (p0,nullptr)");
    result = RawPtrLT<std::int32_t>()(nullptr, nullptr);
    UTAssert(result == false, "Invalid LT result (nullptr,nullptr)");
}

#else

#include <GTL/Utility/RawPtrCompare.h>
#include <cstdint>

namespace gtl
{
    template struct RawPtrEQ<std::int32_t>;
    template struct RawPtrNE<std::int32_t>;
    template struct RawPtrLT<std::int32_t>;
    template struct RawPtrLE<std::int32_t>;
    template struct RawPtrGT<std::int32_t>;
    template struct RawPtrGE<std::int32_t>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(RawPtrCompare)
