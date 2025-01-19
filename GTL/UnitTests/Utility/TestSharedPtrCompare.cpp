// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Utility/SharedPtrCompare.h>
using namespace gtl;

namespace gtl
{
    class UnitTestSharedPtrCompare
    {
    public:
        UnitTestSharedPtrCompare();

    private:
        void Test();
    };
}

UnitTestSharedPtrCompare::UnitTestSharedPtrCompare()
{
    UTInformation("Mathematics/Utility/SharedPtrCompare");

    Test();
}

void UnitTestSharedPtrCompare::Test()
{
    auto sp0 = std::make_shared<std::int32_t>(0);
    auto sp1 = std::make_shared<std::int32_t>(1);
    auto sp2 = std::make_shared<std::int32_t>(1);
    bool result;

    result = SharedPtrEQ<std::int32_t>()(sp0, sp1);
    UTAssert(result == false, "Invalid EQ result (sp0,sp1)");
    result = SharedPtrNE<std::int32_t>()(sp0, sp1);
    UTAssert(result == true, "Invalid NE result (sp0,sp1)");
    result = SharedPtrLT<std::int32_t>()(sp0, sp1);
    UTAssert(result == true, "Invalid LT result (sp0,sp1)");
    result = SharedPtrLE<std::int32_t>()(sp0, sp1);
    UTAssert(result == true, "Invalid LE result (sp0,sp1)");
    result = SharedPtrGT<std::int32_t>()(sp0, sp1);
    UTAssert(result == false, "Invalid GT result (sp0,sp1)");
    result = SharedPtrGE<std::int32_t>()(sp0, sp1);
    UTAssert(result == false, "Invalid GE result (sp0,sp1)");

    result = SharedPtrEQ<std::int32_t>()(sp1, sp2);
    UTAssert(result == true, "Invalid EQ result (sp1,sp2)");
    result = SharedPtrNE<std::int32_t>()(sp1, sp2);
    UTAssert(result == false, "Invalid NE result (sp1,sp2)");
    result = SharedPtrLT<std::int32_t>()(sp1, sp2);
    UTAssert(result == false, "Invalid LT result (sp1,sp2)");
    result = SharedPtrLE<std::int32_t>()(sp1, sp2);
    UTAssert(result == true, "Invalid LE result (sp1,sp2)");
    result = SharedPtrGT<std::int32_t>()(sp1, sp2);
    UTAssert(result == false, "Invalid GT result (sp1,sp2)");
    result = SharedPtrGE<std::int32_t>()(sp1, sp2);
    UTAssert(result == true, "Invalid GE result (sp1,sp2)");

    result = SharedPtrEQ<std::int32_t>()(sp1, sp0);
    UTAssert(result == false, "Invalid EQ result (sp1,sp0)");
    result = SharedPtrNE<std::int32_t>()(sp1, sp0);
    UTAssert(result == true, "Invalid NE result (sp1,sp0)");
    result = SharedPtrLT<std::int32_t>()(sp1, sp0);
    UTAssert(result == false, "Invalid LT result (sp1,sp0)");
    result = SharedPtrLE<std::int32_t>()(sp1, sp0);
    UTAssert(result == false, "Invalid LE result (sp1,sp0)");
    result = SharedPtrGT<std::int32_t>()(sp1, sp0);
    UTAssert(result == true, "Invalid GT result (sp1,sp0)");
    result = SharedPtrGE<std::int32_t>()(sp1, sp0);
    UTAssert(result == true, "Invalid GE result (sp1,sp0)");

    result = SharedPtrEQ<std::int32_t>()(nullptr, sp1);
    UTAssert(result == false, "Invalid EQ result (nullptr,sp1)");
    result = SharedPtrEQ<std::int32_t>()(sp0, nullptr);
    UTAssert(result == false, "Invalid EQ result (sp0,nullptr)");
    result = SharedPtrEQ<std::int32_t>()(nullptr, nullptr);
    UTAssert(result == true, "Invalid EQ result (nullptr,nullptr)");

    result = SharedPtrLT<std::int32_t>()(nullptr, sp1);
    UTAssert(result == true, "Invalid LT result (nullptr,sp1)");
    result = SharedPtrLT<std::int32_t>()(sp0, nullptr);
    UTAssert(result == false, "Invalid LT result (sp0,nullptr)");
    result = SharedPtrLT<std::int32_t>()(nullptr, nullptr);
    UTAssert(result == false, "Invalid LT result (nullptr,nullptr)");
}

#else

#include <GTL/Utility/SharedPtrCompare.h>

namespace gtl
{
    template struct SharedPtrEQ<std::int32_t>;
    template struct SharedPtrNE<std::int32_t>;
    template struct SharedPtrLT<std::int32_t>;
    template struct SharedPtrLE<std::int32_t>;
    template struct SharedPtrGT<std::int32_t>;
    template struct SharedPtrGE<std::int32_t>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(SharedPtrCompare)
