#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/ND/Lozenge.h>
using namespace gtl;

namespace gtl
{
    class UnitTestLozenge
    {
    public:
        UnitTestLozenge();

    private:
        void Test();
    };
}

UnitTestLozenge::UnitTestLozenge()
{
    UTInformation("Mathematics/Primitives/ND/Lozenge");

    Test();
}

void UnitTestLozenge::Test()
{
    Lozenge3<double> lozenge0{};
    UTAssert(IsZero(lozenge0.rectangle.center), "Expecting zero center.");
    UTAssert(IsZero(lozenge0.rectangle.axis[0]), "Expecting zero axis[0].");
    UTAssert(IsZero(lozenge0.rectangle.axis[1]), "Expecting zero axis[1].");
    UTAssert(IsZero(lozenge0.rectangle.extent), "Expecting zero extent.");
    UTAssert(lozenge0.radius == 0.0, "Expecting zero radius.");

    Vector3<double> center{ 1.0, 2.0, 3.0 };
    std::array<Vector3<double>, 2> axis =
    {
        Vector3<double>{ 1.0, 0.0, 0.0 },
        Vector3<double>{ 0.0, 1.0, 0.0 }
    };
    Vector2<double> extent{ 0.1, 0.2 };
    Rectangle3<double> rectangle(center, axis, extent);
    double radius = 1.0;
    Lozenge3<double> lozenge1(rectangle, radius);
    UTAssert(lozenge1.rectangle.center == center, "Incorrect center.");
    UTAssert(lozenge1.rectangle.axis[0] == axis[0], "Incorrect axis[0].");
    UTAssert(lozenge1.rectangle.axis[1] == axis[1], "Incorrect axis[1].");
    UTAssert(lozenge1.rectangle.extent == extent, "Incorrect extent.");
    UTAssert(lozenge1.radius == radius, "Incorrect radius.");

    bool result{};
    result = (lozenge0 == lozenge1);
    UTAssert(result == false, "operator== failed");
    result = (lozenge0 != lozenge1);
    UTAssert(result == true, "operator!= failed");
    result = (lozenge0 < lozenge1);
    UTAssert(result == true, "operator< failed");
    result = (lozenge0 <= lozenge1);
    UTAssert(result == true, "operator<= failed");
    result = (lozenge0 > lozenge1);
    UTAssert(result == false, "operator> failed");
    result = (lozenge0 >= lozenge1);
    UTAssert(result == false, "operator>= failed");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/ND/Lozenge.h>

namespace gtl
{
    template class Lozenge<float, 2>;
    template bool operator==(Lozenge<float, 2> const&, Lozenge<float, 2> const&);
    template bool operator!=(Lozenge<float, 2> const&, Lozenge<float, 2> const&);
    template bool operator< (Lozenge<float, 2> const&, Lozenge<float, 2> const&);
    template bool operator<=(Lozenge<float, 2> const&, Lozenge<float, 2> const&);
    template bool operator> (Lozenge<float, 2> const&, Lozenge<float, 2> const&);
    template bool operator>=(Lozenge<float, 2> const&, Lozenge<float, 2> const&);

    template class Lozenge<double, 3>;
    template bool operator==(Lozenge<double, 3> const&, Lozenge<double, 3> const&);
    template bool operator!=(Lozenge<double, 3> const&, Lozenge<double, 3> const&);
    template bool operator< (Lozenge<double, 3> const&, Lozenge<double, 3> const&);
    template bool operator<=(Lozenge<double, 3> const&, Lozenge<double, 3> const&);
    template bool operator> (Lozenge<double, 3> const&, Lozenge<double, 3> const&);
    template bool operator>=(Lozenge<double, 3> const&, Lozenge<double, 3> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Lozenge<Rational, 4>;
    template bool operator==(Lozenge<Rational, 4> const&, Lozenge<Rational, 4> const&);
    template bool operator!=(Lozenge<Rational, 4> const&, Lozenge<Rational, 4> const&);
    template bool operator< (Lozenge<Rational, 4> const&, Lozenge<Rational, 4> const&);
    template bool operator<=(Lozenge<Rational, 4> const&, Lozenge<Rational, 4> const&);
    template bool operator> (Lozenge<Rational, 4> const&, Lozenge<Rational, 4> const&);
    template bool operator>=(Lozenge<Rational, 4> const&, Lozenge<Rational, 4> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Lozenge)
