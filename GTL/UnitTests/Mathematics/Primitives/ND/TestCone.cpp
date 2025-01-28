#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/ND/Cone.h>
using namespace gtl;

namespace gtl
{
    class UnitTestCone
    {
    public:
        UnitTestCone();

    private:
        void Test();
    };
}

UnitTestCone::UnitTestCone()
{
    UTInformation("Mathematics/Primitives/ND/Cone");

    Test();
}

void UnitTestCone::Test()
{
    double maxError = 1e-15;

    Cone3<double> cone0{};
    UTAssert(IsZero(cone0.vertex), "Expecting zero vertex.");
    UTAssert(IsZero(cone0.direction), "Expecting zero direction.");
    UTAssert(cone0.angle == 0.0, "Expecting zero angle.");
    UTAssert(cone0.cosAngle == 0.0, "Expecting zero cosAngle.");
    UTAssert(cone0.sinAngle == 0.0, "Expecting zero sinAngle.");
    UTAssert(cone0.tanAngle == 0.0, "Expecting zero tanAngle.");
    UTAssert(cone0.cosAngleSqr == 0.0, "Expecting zero cosAngleSqr.");
    UTAssert(cone0.sinAngleSqr == 0.0, "Expecting zero sinAngleSqr.");
    UTAssert(cone0.invSinAngle == 0.0, "Expecting zero invSinAngle.");
    UTAssert(cone0.GetMinHeight() == 0.0, "Expecting zero min height.");
    UTAssert(cone0.GetMaxHeight() == 0.0, "Expecting zero max height.");

    Vector3<double> vertex{ 1.0, 2.0, 3.0 };
    Vector3<double> direction{ 0.0, 0.0, 1.0 };
    double angle = 0.12345;
    Cone3<double> cone1(vertex, direction, angle);
    double cosAngle = std::cos(angle);
    double sinAngle = std::sin(angle);
    double tanAngle = std::tan(angle);
    double cosAngleSqr = cosAngle * cosAngle;
    double sinAngleSqr = sinAngle * sinAngle;
    double invSinAngle = 1.0 / sinAngle;
    UTAssert(cone1.vertex == vertex, "Incorrect vertex.");
    UTAssert(cone1.direction == direction, "Incorrect direction.");
    UTAssert(cone1.angle == angle, "Incorrect angle.");
    UTAssert(std::fabs(cone1.cosAngle - cosAngle) <= maxError, "Incorrect cosAngle.");
    UTAssert(std::fabs(cone1.sinAngle - sinAngle) <= maxError, "Incorrect sinAngle.");
    UTAssert(std::fabs(cone1.tanAngle - tanAngle) <= maxError, "Incorrect tanAngle.");
    UTAssert(std::fabs(cone1.cosAngleSqr - cosAngleSqr) <= maxError, "Incorrect cosAngleSqr.");
    UTAssert(std::fabs(cone1.sinAngleSqr - sinAngleSqr) <= maxError, "Incorrect sinAngleSqr.");
    UTAssert(std::fabs(cone1.invSinAngle - invSinAngle) <= maxError, "Incorrect invSinAngle.");
    UTAssert(cone1.GetMinHeight() == 0.0, "Incorrect min height.");
    UTAssert(cone1.IsInfinite(), "Cone should be infinite.");

    double minHeight = 1.0, maxHeight = 100.0;
    Cone3<double> cone2(vertex, direction, angle, minHeight, maxHeight);
    cosAngle = std::cos(angle);
    sinAngle = std::sin(angle);
    tanAngle = std::tan(angle);
    cosAngleSqr = cosAngle * cosAngle;
    sinAngleSqr = sinAngle * sinAngle;
    invSinAngle = 1.0 / sinAngle;
    UTAssert(cone2.vertex == vertex, "Incorrect vertex.");
    UTAssert(cone2.direction == direction, "Incorrect direction.");
    UTAssert(cone2.angle == angle, "Incorrect angle.");
    UTAssert(std::fabs(cone2.cosAngle - cosAngle) <= maxError, "Incorrect cosAngle.");
    UTAssert(std::fabs(cone2.sinAngle - sinAngle) <= maxError, "Incorrect sinAngle.");
    UTAssert(std::fabs(cone2.tanAngle - tanAngle) <= maxError, "Incorrect tanAngle.");
    UTAssert(std::fabs(cone2.cosAngleSqr - cosAngleSqr) <= maxError, "Incorrect cosAngleSqr.");
    UTAssert(std::fabs(cone2.sinAngleSqr - sinAngleSqr) <= maxError, "Incorrect sinAngleSqr.");
    UTAssert(std::fabs(cone2.invSinAngle - invSinAngle) <= maxError, "Incorrect invSinAngle.");
    UTAssert(cone2.GetMinHeight() == minHeight, "Incorrect min height.");
    UTAssert(cone2.GetMaxHeight() == maxHeight, "Incorrect max height.");
    UTAssert(cone2.IsFinite(), "Cone should be finite.");

    bool result{};
    result = (cone0 == cone1);
    UTAssert(result == false, "operator== failed");
    result = (cone0 != cone1);
    UTAssert(result == true, "operator!= failed");
    result = (cone0 < cone1);
    UTAssert(result == true, "operator< failed");
    result = (cone0 <= cone1);
    UTAssert(result == true, "operator<= failed");
    result = (cone0 > cone1);
    UTAssert(result == false, "operator> failed");
    result = (cone0 >= cone1);
    UTAssert(result == false, "operator>= failed");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/ND/Cone.h>

namespace gtl
{
    template class Cone<float, 2>;
    template bool operator==(Cone<float, 2> const&, Cone<float, 2> const&);
    template bool operator!=(Cone<float, 2> const&, Cone<float, 2> const&);
    template bool operator< (Cone<float, 2> const&, Cone<float, 2> const&);
    template bool operator<=(Cone<float, 2> const&, Cone<float, 2> const&);
    template bool operator> (Cone<float, 2> const&, Cone<float, 2> const&);
    template bool operator>=(Cone<float, 2> const&, Cone<float, 2> const&);

    template class Cone<double, 3>;
    template bool operator==(Cone<double, 3> const&, Cone<double, 3> const&);
    template bool operator!=(Cone<double, 3> const&, Cone<double, 3> const&);
    template bool operator< (Cone<double, 3> const&, Cone<double, 3> const&);
    template bool operator<=(Cone<double, 3> const&, Cone<double, 3> const&);
    template bool operator> (Cone<double, 3> const&, Cone<double, 3> const&);
    template bool operator>=(Cone<double, 3> const&, Cone<double, 3> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Cone<Rational, 4>;
    template bool operator==(Cone<Rational, 4> const&, Cone<Rational, 4> const&);
    template bool operator!=(Cone<Rational, 4> const&, Cone<Rational, 4> const&);
    template bool operator< (Cone<Rational, 4> const&, Cone<Rational, 4> const&);
    template bool operator<=(Cone<Rational, 4> const&, Cone<Rational, 4> const&);
    template bool operator> (Cone<Rational, 4> const&, Cone<Rational, 4> const&);
    template bool operator>=(Cone<Rational, 4> const&, Cone<Rational, 4> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Cone)
