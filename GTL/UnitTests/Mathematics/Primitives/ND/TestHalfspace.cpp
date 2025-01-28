#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/ND/Halfspace.h>
using namespace gtl;

namespace gtl
{
    class UnitTestHalfspace
    {
    public:
        UnitTestHalfspace();

    private:
        void Test();
    };
}

UnitTestHalfspace::UnitTestHalfspace()
{
    UTInformation("Mathematics/Primitives/ND/Halfspace");

    Test();
}

void UnitTestHalfspace::Test()
{
    Halfspace3<double> hspace0{};
    UTAssert(IsZero(hspace0.normal), "Expecting zero normal.");
    UTAssert(hspace0.constant == 0.0, "Expecting zero constant.");

    Vector3<double> normal{ 1.0, 2.0, 3.0 };
    Normalize(normal);
    double constant = 2.0;
    Halfspace3<double> hspace1(normal, constant);
    UTAssert(hspace1.normal == normal, "Incorrect normal.");
    UTAssert(hspace1.constant == constant, "Incorrect constant.");

    bool result{};
    result = (hspace0 == hspace1);
    UTAssert(result == false, "operator== failed");
    result = (hspace0 != hspace1);
    UTAssert(result == true, "operator!= failed");
    result = (hspace0 < hspace1);
    UTAssert(result == true, "operator< failed");
    result = (hspace0 <= hspace1);
    UTAssert(result == true, "operator<= failed");
    result = (hspace0 > hspace1);
    UTAssert(result == false, "operator> failed");
    result = (hspace0 >= hspace1);
    UTAssert(result == false, "operator>= failed");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/ND/Halfspace.h>

namespace gtl
{
    template class Halfspace<float, 2>;
    template bool operator==(Halfspace<float, 2> const&, Halfspace<float, 2> const&);
    template bool operator!=(Halfspace<float, 2> const&, Halfspace<float, 2> const&);
    template bool operator< (Halfspace<float, 2> const&, Halfspace<float, 2> const&);
    template bool operator<=(Halfspace<float, 2> const&, Halfspace<float, 2> const&);
    template bool operator> (Halfspace<float, 2> const&, Halfspace<float, 2> const&);
    template bool operator>=(Halfspace<float, 2> const&, Halfspace<float, 2> const&);

    template class Halfspace<double, 3>;
    template bool operator==(Halfspace<double, 3> const&, Halfspace<double, 3> const&);
    template bool operator!=(Halfspace<double, 3> const&, Halfspace<double, 3> const&);
    template bool operator< (Halfspace<double, 3> const&, Halfspace<double, 3> const&);
    template bool operator<=(Halfspace<double, 3> const&, Halfspace<double, 3> const&);
    template bool operator> (Halfspace<double, 3> const&, Halfspace<double, 3> const&);
    template bool operator>=(Halfspace<double, 3> const&, Halfspace<double, 3> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Halfspace<Rational, 4>;
    template bool operator==(Halfspace<Rational, 4> const&, Halfspace<Rational, 4> const&);
    template bool operator!=(Halfspace<Rational, 4> const&, Halfspace<Rational, 4> const&);
    template bool operator< (Halfspace<Rational, 4> const&, Halfspace<Rational, 4> const&);
    template bool operator<=(Halfspace<Rational, 4> const&, Halfspace<Rational, 4> const&);
    template bool operator> (Halfspace<Rational, 4> const&, Halfspace<Rational, 4> const&);
    template bool operator>=(Halfspace<Rational, 4> const&, Halfspace<Rational, 4> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Halfspace)
