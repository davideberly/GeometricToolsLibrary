#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/2D/Arc2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestArc2
    {
    public:
        UnitTestArc2();

    private:
        void Test();
    };
}

UnitTestArc2::UnitTestArc2()
{
    UTInformation("Mathematics/Primitives/2D/Arc2");

    Test();
}

void UnitTestArc2::Test()
{
    Arc2<double> arc0{};
    UTAssert(IsZero(arc0.center), "Expecting zero center.");
    UTAssert(arc0.radius == 0.0, "Expecting zero radius.");
    UTAssert(IsZero(arc0.end[0]) && IsZero(arc0.end[1]), "Expecting zero endpoints.");

    Vector2<double> center{ 1.0, 2.0 };
    double radius = 1.0;
    std::array<Vector2<double>, 2> end =
    {
        Vector2<double>{ 2.0, 2.0 },
        Vector2<double>{ 1.0, 3.0 }
    };
    Arc2<double> arc1(center, radius, end);
    UTAssert(arc1.center == center, "Incorrect center.");
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

    arc0.center = { 800.0, -200.0 };
    arc0.end[0] = { 1591.8964578782, 30.0 };
    arc0.end[1] = { 8.1035421218, 30.0 };
    arc0.radius = Length(arc0.end[0] - arc0.center);
    // = Length(arc.end[1] - arc.center)
    // = 824.62112512355623

    // The point is not on the circular arc.
    Vector2<double> point{ 1560.6068185945, 30.0 };
    double epsilon = 1e-08;
    bool contains = arc0.Contains(point, epsilon);
    UTAssert(contains == false, "Invalid result.");

    // Project the point onto the arc.
    Vector2<double> diff = point - arc0.center;
    Normalize(diff);
    Vector2<double> scaledPoint = arc0.center + arc0.radius * diff;
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
#include <GTL/Mathematics/Primitives/2D/Arc2.h>

namespace gtl
{
    template class Arc2<float>;
    template bool operator==(Arc2<float> const&, Arc2<float> const&);
    template bool operator!=(Arc2<float> const&, Arc2<float> const&);
    template bool operator< (Arc2<float> const&, Arc2<float> const&);
    template bool operator<=(Arc2<float> const&, Arc2<float> const&);
    template bool operator> (Arc2<float> const&, Arc2<float> const&);
    template bool operator>=(Arc2<float> const&, Arc2<float> const&);

    template class Arc2<double>;
    template bool operator==(Arc2<double> const&, Arc2<double> const&);
    template bool operator!=(Arc2<double> const&, Arc2<double> const&);
    template bool operator< (Arc2<double> const&, Arc2<double> const&);
    template bool operator<=(Arc2<double> const&, Arc2<double> const&);
    template bool operator> (Arc2<double> const&, Arc2<double> const&);
    template bool operator>=(Arc2<double> const&, Arc2<double> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Arc2<Rational>;
    template bool operator==(Arc2<Rational> const&, Arc2<Rational> const&);
    template bool operator!=(Arc2<Rational> const&, Arc2<Rational> const&);
    template bool operator< (Arc2<Rational> const&, Arc2<Rational> const&);
    template bool operator<=(Arc2<Rational> const&, Arc2<Rational> const&);
    template bool operator> (Arc2<Rational> const&, Arc2<Rational> const&);
    template bool operator>=(Arc2<Rational> const&, Arc2<Rational> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Arc2)
