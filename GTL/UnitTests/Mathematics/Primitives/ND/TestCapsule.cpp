#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/ND/Capsule.h>
using namespace gtl;

namespace gtl
{
    class UnitTestCapsule
    {
    public:
        UnitTestCapsule();

    private:
        void Test();
    };
}

UnitTestCapsule::UnitTestCapsule()
{
    UTInformation("Mathematics/Primitives/ND/Capsule");

    Test();
}

void UnitTestCapsule::Test()
{
    Capsule3<double> capsule0{};
    UTAssert(IsZero(capsule0.segment.p[0]), "Expecting zero p[0].");
    UTAssert(IsZero(capsule0.segment.p[1]), "Expecting zero p[1].");
    UTAssert(capsule0.radius == 0.0, "Expecting zero radius.");

    Segment3<double> segment{};
    segment.p[0] = { 1.0, 2.0, 3.0 };
    segment.p[1] = { 2.0, 3.0, 4.0 };
    double radius = 1.0;
    Capsule3<double> capsule1(segment, radius);
    UTAssert(capsule1.segment.p[0] == segment.p[0], "Incorrect p[0].");
    UTAssert(capsule1.segment.p[1] == segment.p[1], "Incorrect p[1].");
    UTAssert(capsule1.radius == radius, "Incorrect radius.");

    bool result{};
    result = (capsule0 == capsule1);
    UTAssert(result == false, "operator== failed");
    result = (capsule0 != capsule1);
    UTAssert(result == true, "operator!= failed");
    result = (capsule0 < capsule1);
    UTAssert(result == true, "operator< failed");
    result = (capsule0 <= capsule1);
    UTAssert(result == true, "operator<= failed");
    result = (capsule0 > capsule1);
    UTAssert(result == false, "operator> failed");
    result = (capsule0 >= capsule1);
    UTAssert(result == false, "operator>= failed");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/ND/Capsule.h>

namespace gtl
{
    template class Capsule<float, 2>;
    template bool operator==(Capsule<float, 2> const&, Capsule<float, 2> const&);
    template bool operator!=(Capsule<float, 2> const&, Capsule<float, 2> const&);
    template bool operator< (Capsule<float, 2> const&, Capsule<float, 2> const&);
    template bool operator<=(Capsule<float, 2> const&, Capsule<float, 2> const&);
    template bool operator> (Capsule<float, 2> const&, Capsule<float, 2> const&);
    template bool operator>=(Capsule<float, 2> const&, Capsule<float, 2> const&);

    template class Capsule<double, 3>;
    template bool operator==(Capsule<double, 3> const&, Capsule<double, 3> const&);
    template bool operator!=(Capsule<double, 3> const&, Capsule<double, 3> const&);
    template bool operator< (Capsule<double, 3> const&, Capsule<double, 3> const&);
    template bool operator<=(Capsule<double, 3> const&, Capsule<double, 3> const&);
    template bool operator> (Capsule<double, 3> const&, Capsule<double, 3> const&);
    template bool operator>=(Capsule<double, 3> const&, Capsule<double, 3> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Capsule<Rational, 4>;
    template bool operator==(Capsule<Rational, 4> const&, Capsule<Rational, 4> const&);
    template bool operator!=(Capsule<Rational, 4> const&, Capsule<Rational, 4> const&);
    template bool operator< (Capsule<Rational, 4> const&, Capsule<Rational, 4> const&);
    template bool operator<=(Capsule<Rational, 4> const&, Capsule<Rational, 4> const&);
    template bool operator> (Capsule<Rational, 4> const&, Capsule<Rational, 4> const&);
    template bool operator>=(Capsule<Rational, 4> const&, Capsule<Rational, 4> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Capsule)
