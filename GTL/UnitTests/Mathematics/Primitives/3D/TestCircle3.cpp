#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/3D/Circle3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestCircle3
    {
    public:
        UnitTestCircle3();

    private:
        void Test();
    };
}

UnitTestCircle3::UnitTestCircle3()
{
    UTInformation("Mathematics/Primitives/3D/Circle3");

    Test();
}

void UnitTestCircle3::Test()
{
    Circle3<double> circle0{};
    UTAssert(IsZero(circle0.center), "Expecting zero center.");
    UTAssert(IsZero(circle0.normal), "Expecting zero normal.");
    UTAssert(circle0.radius == 0.0, "Expecting zero radius.");

    Vector3<double> center{ 1.0, 2.0, 3.0 };
    Vector3<double> normal{ 1.0, 1.0, 1.0 };
    Normalize(normal);
    double radius = 4.0;
    Circle3<double> circle1(center, normal, radius);
    UTAssert(circle1.center == center, "Incorrect center.");
    UTAssert(circle1.normal == normal, "Incorrect normal.");
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
#include <GTL/Mathematics/Primitives/3D/Circle3.h>

namespace gtl
{
    template class Circle3<float>;
    template bool operator==(Circle3<float> const&, Circle3<float> const&);
    template bool operator!=(Circle3<float> const&, Circle3<float> const&);
    template bool operator< (Circle3<float> const&, Circle3<float> const&);
    template bool operator<=(Circle3<float> const&, Circle3<float> const&);
    template bool operator> (Circle3<float> const&, Circle3<float> const&);
    template bool operator>=(Circle3<float> const&, Circle3<float> const&);

    template class Circle3<double>;
    template bool operator==(Circle3<double> const&, Circle3<double> const&);
    template bool operator!=(Circle3<double> const&, Circle3<double> const&);
    template bool operator< (Circle3<double> const&, Circle3<double> const&);
    template bool operator<=(Circle3<double> const&, Circle3<double> const&);
    template bool operator> (Circle3<double> const&, Circle3<double> const&);
    template bool operator>=(Circle3<double> const&, Circle3<double> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Circle3<Rational>;
    template bool operator==(Circle3<Rational> const&, Circle3<Rational> const&);
    template bool operator!=(Circle3<Rational> const&, Circle3<Rational> const&);
    template bool operator< (Circle3<Rational> const&, Circle3<Rational> const&);
    template bool operator<=(Circle3<Rational> const&, Circle3<Rational> const&);
    template bool operator> (Circle3<Rational> const&, Circle3<Rational> const&);
    template bool operator>=(Circle3<Rational> const&, Circle3<Rational> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Circle3)
