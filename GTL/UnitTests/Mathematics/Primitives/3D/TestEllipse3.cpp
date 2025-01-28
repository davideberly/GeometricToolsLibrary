#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/3D/Ellipse3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestEllipse3
    {
    public:
        UnitTestEllipse3();

    private:
        void Test();
    };
}

UnitTestEllipse3::UnitTestEllipse3()
{
    UTInformation("Mathematics/Primitives/3D/Ellipse3");

    Test();
}

void UnitTestEllipse3::Test()
{
    Ellipse3<double> ellipse0{};
    UTAssert(IsZero(ellipse0.center), "Expecting zero center.");
    UTAssert(IsZero(ellipse0.normal), "Expecting zero normal.");
    UTAssert(IsZero(ellipse0.axis[0]), "Expecting zero axis[0].");
    UTAssert(IsZero(ellipse0.axis[1]), "Expecting zero axis[1].");
    UTAssert(ellipse0.extent[0] == 0.0, "Expecting zero extent[0].");
    UTAssert(ellipse0.extent[1] == 0.0, "Expecting zero extent[1].");

    Vector3<double> center{ 1.0, 2.0, 3.0 };
    Vector3<double> normal{ 1.0, 1.0, 1.0 };
    std::array<Vector3<double>, 2> axis{};
    ComputeOrthonormalBasis(1, normal, axis[0], axis[1]);
    std::array<double, 2> extent{ 3.0, 5.0 };
    Ellipse3<double> ellipse1(center, normal, axis, extent);
    UTAssert(ellipse1.center == center, "Incorrect center.");
    UTAssert(ellipse1.normal == normal, "Incorrect normal.");
    UTAssert(ellipse1.axis[0] == axis[0], "Incorrect axis[0].");
    UTAssert(ellipse1.axis[1] == axis[1], "Incorrect axis[1].");
    UTAssert(ellipse1.extent[0] == extent[0], "Incorrect extent[0]");
    UTAssert(ellipse1.extent[1] == extent[1], "Incorrect extent[1]");

    bool result{};
    result = (ellipse0 == ellipse1);
    UTAssert(result == false, "operator== failed");
    result = (ellipse0 != ellipse1);
    UTAssert(result == true, "operator!= failed");
    result = (ellipse0 < ellipse1);
    UTAssert(result == true, "operator< failed");
    result = (ellipse0 <= ellipse1);
    UTAssert(result == true, "operator<= failed");
    result = (ellipse0 > ellipse1);
    UTAssert(result == false, "operator> failed");
    result = (ellipse0 >= ellipse1);
    UTAssert(result == false, "operator>= failed");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/3D/Ellipse3.h>

namespace gtl
{
    template class Ellipse3<float>;
    template bool operator==(Ellipse3<float> const&, Ellipse3<float> const&);
    template bool operator!=(Ellipse3<float> const&, Ellipse3<float> const&);
    template bool operator< (Ellipse3<float> const&, Ellipse3<float> const&);
    template bool operator<=(Ellipse3<float> const&, Ellipse3<float> const&);
    template bool operator> (Ellipse3<float> const&, Ellipse3<float> const&);
    template bool operator>=(Ellipse3<float> const&, Ellipse3<float> const&);

    template class Ellipse3<double>;
    template bool operator==(Ellipse3<double> const&, Ellipse3<double> const&);
    template bool operator!=(Ellipse3<double> const&, Ellipse3<double> const&);
    template bool operator< (Ellipse3<double> const&, Ellipse3<double> const&);
    template bool operator<=(Ellipse3<double> const&, Ellipse3<double> const&);
    template bool operator> (Ellipse3<double> const&, Ellipse3<double> const&);
    template bool operator>=(Ellipse3<double> const&, Ellipse3<double> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Ellipse3<Rational>;
    template bool operator==(Ellipse3<Rational> const&, Ellipse3<Rational> const&);
    template bool operator!=(Ellipse3<Rational> const&, Ellipse3<Rational> const&);
    template bool operator< (Ellipse3<Rational> const&, Ellipse3<Rational> const&);
    template bool operator<=(Ellipse3<Rational> const&, Ellipse3<Rational> const&);
    template bool operator> (Ellipse3<Rational> const&, Ellipse3<Rational> const&);
    template bool operator>=(Ellipse3<Rational> const&, Ellipse3<Rational> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Ellipse3)
