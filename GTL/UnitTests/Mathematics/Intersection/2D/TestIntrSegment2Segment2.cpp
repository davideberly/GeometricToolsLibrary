#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/2D/IntrSegment2Segment2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrSegment2Segment2
    {
    public:
        UnitTestIntrSegment2Segment2();

    private:
    };
}

UnitTestIntrSegment2Segment2::UnitTestIntrSegment2Segment2()
{
    UTInformation("Mathematics/Intersection/2D/IntrSegment2Segment2 (ported from GTE, no logic changes)");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/2D/IntrSegment2Segment2.h>

namespace gtl
{
    template class TIQuery<float, Segment2<float>, Segment2<float>>;
    template class FIQuery<float, Segment2<float>, Segment2<float>>;

    template class TIQuery<double, Segment2<double>, Segment2<double>>;
    template class FIQuery<double, Segment2<double>, Segment2<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Segment2<Rational>, Segment2<Rational>>;
    template class FIQuery<Rational, Segment2<Rational>, Segment2<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrSegment2Segment2)
