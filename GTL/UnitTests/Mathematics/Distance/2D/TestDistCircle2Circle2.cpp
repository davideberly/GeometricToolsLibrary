#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/2D/DistCircle2Circle2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistCircle2Circle2
    {
    public:
        UnitTestDistCircle2Circle2();

    private:
        using CCQuery = DCPQuery<double, Circle2<double>, Circle2<double>>;
        using CCOutput = CCQuery::Output;

        void Test();

        // The expected values are passed to this function.
        void Validate(CCOutput const& output, CCOutput const& expected);
    };
}

UnitTestDistCircle2Circle2::UnitTestDistCircle2Circle2()
{
    UTInformation("Mathematics/Distance/2D/DistCircle2Circle2");

    Test();
}

void UnitTestDistCircle2Circle2::Validate(CCOutput const& output, CCOutput const& expected)
{
    double const maxError = 1e-14;
    double error{};

    UTAssert(output.numClosestPairs == expected.numClosestPairs, "Invalid number of pairs.");
    error = std::fabs(output.sqrDistance - expected.sqrDistance);
    UTAssert(error <= maxError, "Invalid sqrDistance.");
    for (std::size_t i = 0; i < output.numClosestPairs; ++i)
    {
        error = Length(output.closest[i][0] - expected.closest[i][0]);
        UTAssert(error <= maxError, "Invalid closest[" + std::to_string(i) + "][0].");
        error = Length(output.closest[i][1] - expected.closest[i][1]);
        UTAssert(error <= maxError, "Invalid closest[" + std::to_string(i) + "][1].");
        UTAssert(output.concentric == expected.concentric, "Invalid concentric.");
        UTAssert(output.cocircular == expected.cocircular, "Invalid cocircular.");
    }
}

void UnitTestDistCircle2Circle2::Test()
{
    Circle2<double> circle0{}, circle1{};
    CCQuery query{};
    CCQuery::Output output{}, expected{};
    Vector2<double> trn = { 0.01234, 0.56789 };

    // Line 112. The circles are strictly separated.
    circle0.center = { 0.0, 0.0 };
    circle0.radius = 1.0;
    circle1.center = { 4.0, 2.0 };
    circle1.radius = 0.5;
    output = query(circle0, circle1);
    expected.distance = 2.9721359549995796;
    expected.sqrDistance = 8.8335921350012629;
    expected.numClosestPairs = 1;
    expected.closest[0][0] = { 0.89442719099991586, 0.44721359549995793 };
    expected.closest[0][1] = { 3.5527864045000421, 1.7763932022500211 };
    expected.concentric = false;
    expected.cocircular = false;
    Validate(output, expected);

    // Line 116. The circles are separated but tangent at (4,3).
    // Mathematica generates closest points (3.9986,3.00187). Lots of
    // numerical errors in their minimizer!
    circle0.center = { 0.0, 0.0 };
    circle0.radius = 5.0;
    circle1.center = { 8.0, 6.0 };
    circle1.radius = 5.0;
    output = query(circle0, circle1);
    expected.distance = 0.0;
    expected.sqrDistance = 0.0;
    expected.numClosestPairs = 1;
    expected.closest[0][0] = { 4.0, 3.0 };
    expected.closest[0][1] = { 4.0, 3.0 };
    expected.concentric = false;
    expected.cocircular = false;
    Validate(output, expected);

    // Line 136. Circle 1 is contained in circle 0 and is
    // tangent at (1,0).
    circle0.center = { 0.0, 0.0 };
    circle0.radius = 1.0;
    circle1.center = { 0.5, 0.0 };
    circle1.radius = 0.5;
    output = query(circle0, circle1);
    expected.distance = 0.0;
    expected.sqrDistance = 0.0;
    expected.numClosestPairs = 1;
    expected.closest[0][0] = { 1.0, 0.0 };
    expected.closest[0][1] = { 1.0, 0.0 };
    expected.concentric = false;
    expected.cocircular = false;
    Validate(output, expected);

    // Line 132. Circle 1 is strictly contained in circle 0.
    circle0.center = { 0.0, 0.0 };
    circle0.radius = 1.0;
    circle1.center = { 0.25, 0.0 };
    circle1.radius = 0.5;
    output = query(circle0, circle1);
    expected.distance = 0.25;
    expected.sqrDistance = 0.0625;
    expected.numClosestPairs = 1;
    expected.closest[0][0] = { 1.0, 0.0 };
    expected.closest[0][1] = { 0.75, 0.0 };
    expected.concentric = false;
    expected.cocircular = false;
    Validate(output, expected);

    // Line 132. Swap circle 0 and circle 1 of previous example.
    circle0.center = { 0.25, 0.0 };
    circle0.radius = 0.5;
    circle1.center = { 0.0, 0.0 };
    circle1.radius = 1.0;
    output = query(circle0, circle1);
    expected.distance = 0.25;
    expected.sqrDistance = 0.0625;
    expected.numClosestPairs = 1;
    expected.closest[0][0] = { 1.0, 0.0 };
    expected.closest[0][1] = { 0.75, 0.0 };
    expected.concentric = false;
    expected.cocircular = false;
    Validate(output, expected);

    // Line 149. Circles intersect in 2 points.
    circle0.center = { 0.0, 0.0 };
    circle0.radius = 1.0;
    circle1.center = { 1.25, 0.0 };
    circle1.radius = 0.5;
    output = query(circle0, circle1);
    expected.distance = 0.0;
    expected.sqrDistance = 0.0;
    expected.numClosestPairs = 2;
    expected.closest[0][0] = { 0.925, -0.37996710383926691 };
    expected.closest[0][1] = { 0.925, -0.37996710383926691 };
    expected.closest[1][0] = { 0.925, +0.37996710383926691 };
    expected.closest[1][1] = { 0.925, +0.37996710383926691 };
    expected.concentric = false;
    expected.cocircular = false;
    Validate(output, expected);

    // Line 179. Circles are concentric but not cocircular.
    circle0.center = { 0.0, 0.0 };
    circle0.radius = 1.0;
    circle1.center = circle0.center;
    circle1.radius = 0.5;
    output = query(circle0, circle1);
    expected.distance = 0.5;
    expected.sqrDistance = 0.25;
    expected.numClosestPairs = 2; // theoretically infinite
    expected.closest[0][0] = { -1.0, 0.0 };
    expected.closest[0][1] = { -0.5, 0.0 };
    expected.closest[1][0] = { +1.0, 0.0 };
    expected.closest[1][1] = { +0.5, 0.0 };
    expected.concentric = true;
    expected.cocircular = false;
    Validate(output, expected);

    // Line 179. Circles are cocircular.
    circle0.center = { 0.0, 0.0 };
    circle0.radius = 1.0;
    circle1.center = circle0.center;
    circle1.radius = 1.0;
    output = query(circle0, circle1);
    expected.distance = 0.0;
    expected.sqrDistance = 0.0;
    expected.numClosestPairs = 2; // theoretically infinite
    expected.closest[0][0] = { -1.0, 0.0 };
    expected.closest[0][1] = { -1.0, 0.0 };
    expected.closest[1][0] = { +1.0, 0.0 };
    expected.closest[1][1] = { +1.0, 0.0 };
    expected.concentric = true;
    expected.cocircular = true;
    Validate(output, expected);
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/2D/DistCircle2Circle2.h>

namespace gtl
{
    template class DCPQuery<float, Circle2<float>, Circle2<float>>;
    template class DCPQuery<double, Circle2<double>, Circle2<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Circle2<Rational>, Circle2<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistCircle2Circle2)
