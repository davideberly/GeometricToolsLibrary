#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/1D/DisjointIntervals.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDisjointIntervals
    {
    public:
        UnitTestDisjointIntervals();

    private:
        void Test();
    };
}

UnitTestDisjointIntervals::UnitTestDisjointIntervals()
{
    UTInformation("Mathematics/Geometry/1D/DisjointIntervals");

    Test();
}

void UnitTestDisjointIntervals::Test()
{
    DisjointIntervals<float> S1{}, S2{};
    S1.Insert(1.0f, 3.0f);
    S1.Insert(4.0f, 9.0f);
    S1.Insert(10.0f, 12.0f);
    S1.Insert(16.0f, 17.0f);
    S2.Insert(0.0f, 2.0f);
    S2.Insert(6.0f, 7.0f);
    S2.Insert(8.0f, 11.0f);
    S2.Insert(13.0f, 14.0f);
    S2.Insert(15.0f, 18.0f);
    DisjointIntervals<float> S1uS2 = S1 | S2;  // union
    DisjointIntervals<float> S1iS2 = S1 & S2;  // intersection
    DisjointIntervals<float> S1dS2 = S1 - S2;  // difference
    DisjointIntervals<float> S1xS2 = S1 ^ S2;  // exclusive-or

    float end0{}, end1{};

    // S1uS2:
    UTAssert(S1uS2.GetNumIntervals() == 4, "Invalid number of intervals.");
    S1uS2.GetInterval(0, end0, end1);
    UTAssert(end0 == 0.0f && end1 == 3.0f, "Invalid interval.");
    S1uS2.GetInterval(1, end0, end1);
    UTAssert(end0 == 4.0f && end1 == 12.0f, "Invalid interval.");
    S1uS2.GetInterval(2, end0, end1);
    UTAssert(end0 == 13.0f && end1 == 14.0f, "Invalid interval.");
    S1uS2.GetInterval(3, end0, end1);
    UTAssert(end0 == 15.0f && end1 == 18.0f, "Invalid interval.");

    // S1iS2:
    UTAssert(S1iS2.GetNumIntervals() == 5, "Invalid number of intervals.");
    S1iS2.GetInterval(0, end0, end1);
    UTAssert(end0 == 1.0f && end1 == 2.0f, "Invalid interval.");
    S1iS2.GetInterval(1, end0, end1);
    UTAssert(end0 == 6.0f && end1 == 7.0f, "Invalid interval.");
    S1iS2.GetInterval(2, end0, end1);
    UTAssert(end0 == 8.0f && end1 == 9.0f, "Invalid interval.");
    S1iS2.GetInterval(3, end0, end1);
    UTAssert(end0 == 10.0f && end1 == 11.0f, "Invalid interval.");
    S1iS2.GetInterval(4, end0, end1);
    UTAssert(end0 == 16.0f && end1 == 17.0f, "Invalid interval.");

    // S1dS2:
    UTAssert(S1dS2.GetNumIntervals() == 4, "Invalid number of intervals.");
    S1dS2.GetInterval(0, end0, end1);
    UTAssert(end0 == 2.0f && end1 == 3.0f, "Invalid interval.");
    S1dS2.GetInterval(1, end0, end1);
    UTAssert(end0 == 4.0f && end1 == 6.0f, "Invalid interval.");
    S1dS2.GetInterval(2, end0, end1);
    UTAssert(end0 == 7.0f && end1 == 8.0f, "Invalid interval.");
    S1dS2.GetInterval(3, end0, end1);
    UTAssert(end0 == 11.0f && end1 == 12.0f, "Invalid interval.");

    // S1xS2:
    UTAssert(S1xS2.GetNumIntervals() == 9, "Invalid number of intervals.");
    S1xS2.GetInterval(0, end0, end1);
    UTAssert(end0 == 0.0f && end1 == 1.0f, "Invalid interval.");
    S1xS2.GetInterval(1, end0, end1);
    UTAssert(end0 == 2.0f && end1 == 3.0f, "Invalid interval.");
    S1xS2.GetInterval(2, end0, end1);
    UTAssert(end0 == 4.0f && end1 == 6.0f, "Invalid interval.");
    S1xS2.GetInterval(3, end0, end1);
    UTAssert(end0 == 7.0f && end1 == 8.0f, "Invalid interval.");
    S1xS2.GetInterval(4, end0, end1);
    UTAssert(end0 == 9.0f && end1 == 10.0f, "Invalid interval.");
    S1xS2.GetInterval(5, end0, end1);
    UTAssert(end0 == 11.0f && end1 == 12.0f, "Invalid interval.");
    S1xS2.GetInterval(6, end0, end1);
    UTAssert(end0 == 13.0f && end1 == 14.0f, "Invalid interval.");
    S1xS2.GetInterval(7, end0, end1);
    UTAssert(end0 == 15.0f && end1 == 16.0f, "Invalid interval.");
    S1xS2.GetInterval(8, end0, end1);
    UTAssert(end0 == 17.0f && end1 == 18.0f, "Invalid interval.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Geometry/1D/DisjointIntervals.h>

namespace gtl
{
    template class DisjointIntervals<std::int32_t>;
    template class DisjointIntervals<std::uint32_t>;
    template class DisjointIntervals<float>;
    template class DisjointIntervals<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSNumber<UIntegerAP32>;
    template class DisjointIntervals<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DisjointIntervals)
