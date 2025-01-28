#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/ND/Hypersphere.h>
using namespace gtl;

namespace gtl
{
    class UnitTestHypersphere
    {
    public:
        UnitTestHypersphere();

    private:
        void Test();
    };
}

UnitTestHypersphere::UnitTestHypersphere()
{
    UTInformation("Mathematics/Primitives/ND/Hypersphere");

    Test();
}

void UnitTestHypersphere::Test()
{
    Circle2<double> circle0{};
    UTAssert(IsZero(circle0.center), "Expecting zero center.");
    UTAssert(circle0.radius == 0.0, "Expecting zero radius.");

    Vector2<double> center{ 1.0, 2.0 };
    double radius = 1.0;
    Circle2<double> circle1(center, radius);
    UTAssert(circle1.center == center, "Incorrect center.");
    UTAssert(circle1.radius == radius, "Incorrect radius.");

    bool result{};
    result = (circle0 == circle1);
    UTAssert(result == false, "operator== failed");
    result = (circle0 != circle1);
    UTAssert(result == true, "operator!= failed");
    result = (circle0 < circle1);
    UTAssert(result == true, "operator< failed");
    result = (circle0 <= circle1);
    UTAssert(result == true, "operator<= failed");
    result = (circle0 > circle1);
    UTAssert(result == false, "operator> failed");
    result = (circle0 >= circle1);
    UTAssert(result == false, "operator>= failed");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/ND/Hypersphere.h>

namespace gtl
{
    template class Hypersphere<float, 2>;
    template bool operator==(Hypersphere<float, 2> const&, Hypersphere<float, 2> const&);
    template bool operator!=(Hypersphere<float, 2> const&, Hypersphere<float, 2> const&);
    template bool operator< (Hypersphere<float, 2> const&, Hypersphere<float, 2> const&);
    template bool operator<=(Hypersphere<float, 2> const&, Hypersphere<float, 2> const&);
    template bool operator> (Hypersphere<float, 2> const&, Hypersphere<float, 2> const&);
    template bool operator>=(Hypersphere<float, 2> const&, Hypersphere<float, 2> const&);

    template class Hypersphere<double, 3>;
    template bool operator==(Hypersphere<double, 3> const&, Hypersphere<double, 3> const&);
    template bool operator!=(Hypersphere<double, 3> const&, Hypersphere<double, 3> const&);
    template bool operator< (Hypersphere<double, 3> const&, Hypersphere<double, 3> const&);
    template bool operator<=(Hypersphere<double, 3> const&, Hypersphere<double, 3> const&);
    template bool operator> (Hypersphere<double, 3> const&, Hypersphere<double, 3> const&);
    template bool operator>=(Hypersphere<double, 3> const&, Hypersphere<double, 3> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Hypersphere<Rational, 4>;
    template bool operator==(Hypersphere<Rational, 4> const&, Hypersphere<Rational, 4> const&);
    template bool operator!=(Hypersphere<Rational, 4> const&, Hypersphere<Rational, 4> const&);
    template bool operator< (Hypersphere<Rational, 4> const&, Hypersphere<Rational, 4> const&);
    template bool operator<=(Hypersphere<Rational, 4> const&, Hypersphere<Rational, 4> const&);
    template bool operator> (Hypersphere<Rational, 4> const&, Hypersphere<Rational, 4> const&);
    template bool operator>=(Hypersphere<Rational, 4> const&, Hypersphere<Rational, 4> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Hypersphere)
