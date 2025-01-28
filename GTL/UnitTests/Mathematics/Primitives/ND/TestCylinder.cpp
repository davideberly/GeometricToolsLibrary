#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/ND/Cylinder.h>
using namespace gtl;

namespace gtl
{
    class UnitTestCylinder
    {
    public:
        UnitTestCylinder();

    private:
        void Test();
    };
}

UnitTestCylinder::UnitTestCylinder()
{
    UTInformation("Mathematics/Primitives/ND/Cylinder");

    Test();
}

void UnitTestCylinder::Test()
{
    Cylinder3<double> cylinder0{};
    UTAssert(IsZero(cylinder0.center), "Expecting zero center.");
    UTAssert(IsZero(cylinder0.direction), "Expecting zero direction.");
    UTAssert(cylinder0.radius == 0.0, "Expecting zero radius.");
    UTAssert(cylinder0.height == 0.0, "Expecting zero height.");

    Vector3<double> center{ 1.0, 2.0, 3.0 };
    Vector3<double> direction{ 1.0, 0.0, 0.0 };
    double radius = 1.0;
    double height = 2.0;
    Cylinder3<double> cylinder1(center, direction, radius, height);
    UTAssert(cylinder1.center == center, "Incorrect center.");
    UTAssert(cylinder1.direction == direction, "Incorrect direction.");
    UTAssert(cylinder1.radius == radius, "Incorrect radius.");
    UTAssert(cylinder1.height == height, "Incorrect height.");

    bool result{};
    result = (cylinder0 == cylinder1);
    UTAssert(result == false, "operator== failed");
    result = (cylinder0 != cylinder1);
    UTAssert(result == true, "operator!= failed");
    result = (cylinder0 < cylinder1);
    UTAssert(result == true, "operator< failed");
    result = (cylinder0 <= cylinder1);
    UTAssert(result == true, "operator<= failed");
    result = (cylinder0 > cylinder1);
    UTAssert(result == false, "operator> failed");
    result = (cylinder0 >= cylinder1);
    UTAssert(result == false, "operator>= failed");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/ND/Cylinder.h>

namespace gtl
{
    template class Cylinder<float, 2>;
    template bool operator==(Cylinder<float, 2> const&, Cylinder<float, 2> const&);
    template bool operator!=(Cylinder<float, 2> const&, Cylinder<float, 2> const&);
    template bool operator< (Cylinder<float, 2> const&, Cylinder<float, 2> const&);
    template bool operator<=(Cylinder<float, 2> const&, Cylinder<float, 2> const&);
    template bool operator> (Cylinder<float, 2> const&, Cylinder<float, 2> const&);
    template bool operator>=(Cylinder<float, 2> const&, Cylinder<float, 2> const&);

    template class Cylinder<double, 3>;
    template bool operator==(Cylinder<double, 3> const&, Cylinder<double, 3> const&);
    template bool operator!=(Cylinder<double, 3> const&, Cylinder<double, 3> const&);
    template bool operator< (Cylinder<double, 3> const&, Cylinder<double, 3> const&);
    template bool operator<=(Cylinder<double, 3> const&, Cylinder<double, 3> const&);
    template bool operator> (Cylinder<double, 3> const&, Cylinder<double, 3> const&);
    template bool operator>=(Cylinder<double, 3> const&, Cylinder<double, 3> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Cylinder<Rational, 4>;
    template bool operator==(Cylinder<Rational, 4> const&, Cylinder<Rational, 4> const&);
    template bool operator!=(Cylinder<Rational, 4> const&, Cylinder<Rational, 4> const&);
    template bool operator< (Cylinder<Rational, 4> const&, Cylinder<Rational, 4> const&);
    template bool operator<=(Cylinder<Rational, 4> const&, Cylinder<Rational, 4> const&);
    template bool operator> (Cylinder<Rational, 4> const&, Cylinder<Rational, 4> const&);
    template bool operator>=(Cylinder<Rational, 4> const&, Cylinder<Rational, 4> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Cylinder)
