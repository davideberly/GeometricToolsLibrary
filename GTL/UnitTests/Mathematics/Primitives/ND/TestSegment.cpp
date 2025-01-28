#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/ND/Segment.h>
using namespace gtl;

namespace gtl
{
    class UnitTestSegment
    {
    public:
        UnitTestSegment();

    private:
        void Test();
    };
}

UnitTestSegment::UnitTestSegment()
{
    UTInformation("Mathematics/Primitives/ND/Segment");

    Test();
}

void UnitTestSegment::Test()
{
    Segment2<double> segment0{};
    UTAssert(IsZero(segment0.p[0]), "Expecting zero p[0].");
    UTAssert(IsZero(segment0.p[1]), "Expecting zero p[1].");

    Vector2<double> p0{1.0, 2.0};
    Vector2<double> p1{ 0.2, 0.1 };
    Segment2<double> segment1(p0, p1);
    UTAssert(segment1.p[0] == p0, "Incorrect p[0].");
    UTAssert(segment1.p[1] == p1, "Incorrect p[1].");

    bool result{};
    result = (segment0 == segment1);
    UTAssert(result == false, "operator== failed");
    result = (segment0 != segment1);
    UTAssert(result == true, "operator!= failed");
    result = (segment0 < segment1);
    UTAssert(result == true, "operator< failed");
    result = (segment0 <= segment1);
    UTAssert(result == true, "operator<= failed");
    result = (segment0 > segment1);
    UTAssert(result == false, "operator> failed");
    result = (segment0 >= segment1);
    UTAssert(result == false, "operator>= failed");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/ND/Segment.h>

namespace gtl
{
    template class Segment<float, 2>;
    template bool operator==(Segment<float, 2> const&, Segment<float, 2> const&);
    template bool operator!=(Segment<float, 2> const&, Segment<float, 2> const&);
    template bool operator< (Segment<float, 2> const&, Segment<float, 2> const&);
    template bool operator<=(Segment<float, 2> const&, Segment<float, 2> const&);
    template bool operator> (Segment<float, 2> const&, Segment<float, 2> const&);
    template bool operator>=(Segment<float, 2> const&, Segment<float, 2> const&);

    template class Segment<double, 3>;
    template bool operator==(Segment<double, 3> const&, Segment<double, 3> const&);
    template bool operator!=(Segment<double, 3> const&, Segment<double, 3> const&);
    template bool operator< (Segment<double, 3> const&, Segment<double, 3> const&);
    template bool operator<=(Segment<double, 3> const&, Segment<double, 3> const&);
    template bool operator> (Segment<double, 3> const&, Segment<double, 3> const&);
    template bool operator>=(Segment<double, 3> const&, Segment<double, 3> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Segment<Rational, 4>;
    template bool operator==(Segment<Rational, 4> const&, Segment<Rational, 4> const&);
    template bool operator!=(Segment<Rational, 4> const&, Segment<Rational, 4> const&);
    template bool operator< (Segment<Rational, 4> const&, Segment<Rational, 4> const&);
    template bool operator<=(Segment<Rational, 4> const&, Segment<Rational, 4> const&);
    template bool operator> (Segment<Rational, 4> const&, Segment<Rational, 4> const&);
    template bool operator>=(Segment<Rational, 4> const&, Segment<Rational, 4> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Segment)
