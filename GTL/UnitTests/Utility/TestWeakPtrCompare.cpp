// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Utility/WeakPtrCompare.h>
using namespace gtl;

namespace gtl
{
    class UnitTestWeakPtrCompare
    {
    public:
        UnitTestWeakPtrCompare();

    private:
        void Test();
    };
}

UnitTestWeakPtrCompare::UnitTestWeakPtrCompare()
{
    UTInformation("Mathematics/Utility/WeakPtrCompare");

    Test();
}

void UnitTestWeakPtrCompare::Test()
{
    auto sp0 = std::make_shared<std::int32_t>(0);
    auto sp1 = std::make_shared<std::int32_t>(1);
    auto sp2 = std::make_shared<std::int32_t>(1);
    std::weak_ptr<std::int32_t> wp0(sp0);
    std::weak_ptr<std::int32_t> wp1(sp1);
    std::weak_ptr<std::int32_t> wp2(sp2);
    bool result;

    result = WeakPtrEQ<std::int32_t>()(wp0, wp1);
    UTAssert(result == false, "Invalid EQ result (wp0,wp1)");
    result = WeakPtrNE<std::int32_t>()(wp0, wp1);
    UTAssert(result == true, "Invalid NE result (wp0,wp1)");
    result = WeakPtrLT<std::int32_t>()(wp0, wp1);
    UTAssert(result == true, "Invalid LT result (wp0,wp1)");
    result = WeakPtrLE<std::int32_t>()(wp0, wp1);
    UTAssert(result == true, "Invalid LE result (wp0,wp1)");
    result = WeakPtrGT<std::int32_t>()(wp0, wp1);
    UTAssert(result == false, "Invalid GT result (wp0,wp1)");
    result = WeakPtrGE<std::int32_t>()(wp0, wp1);
    UTAssert(result == false, "Invalid GE result (wp0,wp1)");

    result = WeakPtrEQ<std::int32_t>()(wp1, wp2);
    UTAssert(result == true, "Invalid EQ result (wp1,wp2)");
    result = WeakPtrNE<std::int32_t>()(wp1, wp2);
    UTAssert(result == false, "Invalid NE result (wp1,wp2)");
    result = WeakPtrLT<std::int32_t>()(wp1, wp2);
    UTAssert(result == false, "Invalid LT result (wp1,wp2)");
    result = WeakPtrLE<std::int32_t>()(wp1, wp2);
    UTAssert(result == true, "Invalid LE result (wp1,wp2)");
    result = WeakPtrGT<std::int32_t>()(wp1, wp2);
    UTAssert(result == false, "Invalid GT result (wp1,wp2)");
    result = WeakPtrGE<std::int32_t>()(wp1, wp2);
    UTAssert(result == true, "Invalid GE result (wp1,wp2)");

    result = WeakPtrEQ<std::int32_t>()(wp1, wp0);
    UTAssert(result == false, "Invalid EQ result (wp1,wp0)");
    result = WeakPtrNE<std::int32_t>()(wp1, wp0);
    UTAssert(result == true, "Invalid NE result (wp1,wp0)");
    result = WeakPtrLT<std::int32_t>()(wp1, wp0);
    UTAssert(result == false, "Invalid LT result (wp1,wp0)");
    result = WeakPtrLE<std::int32_t>()(wp1, wp0);
    UTAssert(result == false, "Invalid LE result (wp1,wp0)");
    result = WeakPtrGT<std::int32_t>()(wp1, wp0);
    UTAssert(result == true, "Invalid GT result (wp1,wp0)");
    result = WeakPtrGE<std::int32_t>()(wp1, wp0);
    UTAssert(result == true, "Invalid GE result (wp1,wp0)");

    std::shared_ptr<std::int32_t> spnull;
    std::weak_ptr<std::int32_t> wpnull(spnull);
    result = WeakPtrEQ<std::int32_t>()(wpnull, wp1);
    UTAssert(result == false, "Invalid EQ result (nullptr,wp1)");
    result = WeakPtrEQ<std::int32_t>()(wp0, wpnull);
    UTAssert(result == false, "Invalid EQ result (wp0,nullptr)");
    result = WeakPtrEQ<std::int32_t>()(wpnull, wpnull);
    UTAssert(result == true, "Invalid EQ result (nullptr,nullptr)");

    result = WeakPtrLT<std::int32_t>()(wpnull, wp1);
    UTAssert(result == true, "Invalid LT result (nullptr,wp1)");
    result = WeakPtrLT<std::int32_t>()(wp0, wpnull);
    UTAssert(result == false, "Invalid LT result (wp0,nullptr)");
    result = WeakPtrLT<std::int32_t>()(wpnull, wpnull);
    UTAssert(result == false, "Invalid LT result (nullptr,nullptr)");
}

#else

#include <GTL/Utility/WeakPtrCompare.h>

namespace gtl
{
    template struct WeakPtrEQ<std::int32_t>;
    template struct WeakPtrNE<std::int32_t>;
    template struct WeakPtrLT<std::int32_t>;
    template struct WeakPtrLE<std::int32_t>;
    template struct WeakPtrGT<std::int32_t>;
    template struct WeakPtrGE<std::int32_t>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(WeakPtrCompare)

