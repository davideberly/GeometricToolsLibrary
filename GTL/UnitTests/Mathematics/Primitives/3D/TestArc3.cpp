#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/3D/Arc3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestArc3
    {
    public:
        UnitTestArc3();

    private:
        void Test();
    };
}

UnitTestArc3::UnitTestArc3()
{
    UTInformation("Mathematics/Primitives/3D/Arc3");

    Test();
}

void UnitTestArc3::Test()
{
    Arc3<double> arc0{};
    UTAssert(IsZero(arc0.center), "Expecting zero center.");
    UTAssert(IsZero(arc0.normal), "Expecting zero normal.");
    UTAssert(arc0.radius == 0.0, "Expecting zero radius.");
    UTAssert(IsZero(arc0.end[0]) && IsZero(arc0.end[1]), "Expecting zero endpoints.");

    Vector3<double> center{ 1.0, 2.0, 1.0 };
    Vector3<double> normal{ 0.0, 0.0, 1.0 };
    double radius = 1.0;
    std::array<Vector3<double>, 2> end =
    {
        Vector3<double>{ 2.0, 2.0, 1.0 },
        Vector3<double>{ 1.0, 3.0, 1.0 }
    };
    Arc3<double> arc1(center, normal, radius, end);
    UTAssert(arc1.center == center, "Incorrect center.");
    UTAssert(arc1.normal == normal, "Incorrect normal.");
    UTAssert(arc1.radius == radius, "Incorrect radius.");
    UTAssert(arc1.end[0] == end[0] && arc1.end[1] == end[1], "Incorrect endpoints.");

    bool result{};
    result = (arc0 == arc1);
    UTAssert(result == false, "operator== failed");
    result = (arc0 != arc1);
    UTAssert(result == true, "operator!= failed");
    result = (arc0 < arc1);
    UTAssert(result == true, "operator< failed");
    result = (arc0 <= arc1);
    UTAssert(result == true, "operator<= failed");
    result = (arc0 > arc1);
    UTAssert(result == false, "operator> failed");
    result = (arc0 >= arc1);
    UTAssert(result == false, "operator>= failed");

    arc0.center = { 800.0, -200.0, 1.0 };
    arc0.end[0] = { 1591.8964578782, 30.0, 1.0 };
    arc0.end[1] = { 8.1035421218, 30.0, 1.0 };
    arc0.radius = Length(arc0.end[0] - arc0.center);
    // = Length(arc.end[1] - arc.center)
    // = 824.62112512355623

    // The point is not on the circular arc.
    Vector3<double> point{ 1560.6068185945, 30.0, 1.0 };
    double epsilon = 1e-08;
    bool contains = arc0.Contains(point, epsilon);
    UTAssert(contains == false, "Invalid result.");

    // Project the point onto the arc.
    Vector3<double> diff = point - arc0.center;
    Normalize(diff);
    Vector3<double> scaledPoint = arc0.center + arc0.radius * diff;
    contains = arc0.Contains(scaledPoint, epsilon);
    UTAssert(contains == true, "Invalid result.");

    // Test the non-epsilon version.
    contains = arc0.Contains(scaledPoint);
    UTAssert(contains == true, "Invalid result.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/3D/Arc3.h>

namespace gtl
{
    template class Arc3<float>;
    template bool operator==(Arc3<float> const&, Arc3<float> const&);
    template bool operator!=(Arc3<float> const&, Arc3<float> const&);
    template bool operator< (Arc3<float> const&, Arc3<float> const&);
    template bool operator<=(Arc3<float> const&, Arc3<float> const&);
    template bool operator> (Arc3<float> const&, Arc3<float> const&);
    template bool operator>=(Arc3<float> const&, Arc3<float> const&);

    template class Arc3<double>;
    template bool operator==(Arc3<double> const&, Arc3<double> const&);
    template bool operator!=(Arc3<double> const&, Arc3<double> const&);
    template bool operator< (Arc3<double> const&, Arc3<double> const&);
    template bool operator<=(Arc3<double> const&, Arc3<double> const&);
    template bool operator> (Arc3<double> const&, Arc3<double> const&);
    template bool operator>=(Arc3<double> const&, Arc3<double> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Arc3<Rational>;
    template bool operator==(Arc3<Rational> const&, Arc3<Rational> const&);
    template bool operator!=(Arc3<Rational> const&, Arc3<Rational> const&);
    template bool operator< (Arc3<Rational> const&, Arc3<Rational> const&);
    template bool operator<=(Arc3<Rational> const&, Arc3<Rational> const&);
    template bool operator> (Arc3<Rational> const&, Arc3<Rational> const&);
    template bool operator>=(Arc3<Rational> const&, Arc3<Rational> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Arc3)
