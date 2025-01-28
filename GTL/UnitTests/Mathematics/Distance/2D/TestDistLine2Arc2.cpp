#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/2D/DistLine2Arc2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistLine2Arc2
    {
    public:
        UnitTestDistLine2Arc2();

    private:
        using LCQuery = DCPQuery<double, Line2<double>, Arc2<double>>;
        using LCOutput = LCQuery::Output;

        void Test();

        // The expected values are passed to this function.
        void Validate(LCOutput const& output, LCOutput const& expected);
    };
}

UnitTestDistLine2Arc2::UnitTestDistLine2Arc2()
{
    UTInformation("Mathematics/Distance/2D/DistLine2Arc2");

    Test();
}

void UnitTestDistLine2Arc2::Validate(LCOutput const& output, LCOutput const& expected)
{
    double const maxError = 1e-14;
    double error{};

    UTAssert(output.numClosestPairs == expected.numClosestPairs, "Invalid number of pairs.");
    error = std::fabs(output.sqrDistance - expected.sqrDistance);
    UTAssert(error <= maxError, "Invalid sqrDistance.");
    for (std::size_t i = 0; i < output.numClosestPairs; ++i)
    {
        error = std::fabs(output.parameter[i] - expected.parameter[i]);
        UTAssert(error <= maxError, "Invalid parameter[" + std::to_string(i) + "].");
        error = Length(output.closest[i][0] - expected.closest[i][0]);
        UTAssert(error <= maxError, "Invalid closest[" + std::to_string(i) + "][0].");
        error = Length(output.closest[i][1] - expected.closest[i][1]);
        UTAssert(error <= maxError, "Invalid closest[" + std::to_string(i) + "][1].");
    }
}

void UnitTestDistLine2Arc2::Test()
{
    Line2<double> line{};
    Arc2<double> arc{};
    LCQuery query{};
    LCQuery::Output output{}, expected{};

    // line outside the circle
    arc.center = { 0.0, 0.0 };
    arc.radius = 1.0;
    arc.end[0] = arc.radius * Vector2<double>{ 1.0, 0.0 };
    arc.end[1] = arc.radius * Vector2<double>{ 0.0, 1.0 };
    line.origin = { 4.0, 3.0 };
    line.direction = { -1.0, 1.0 };
    output = query(line, arc);
    expected.distance = 3.9497474683058327;
    expected.sqrDistance = 15.600505063388335;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 0.5;
    expected.closest[0][0] = { 3.5, 3.5 };
    expected.closest[0][1] = { 0.70710678118654746, 0.70710678118654746 };
    Validate(output, expected);

    // line intersects arc in 2 points
    arc.center = { 0.0, 0.0 };
    arc.radius = 1.0;
    arc.end[0] = arc.radius * Vector2<double>{ 1.0, 0.0 };
    arc.end[1] = arc.radius * Vector2<double>{ 0.0, 1.0 };
    line.origin = Vector2<double>{ 4.0, 3.0 } / 5.1;
    line.direction = { -1.0, 1.0 };
    output = query(line, arc);
    expected.distance = 0.0;
    expected.sqrDistance = 0.0;
    expected.numClosestPairs = 2;
    expected.parameter[0] = -0.072334776441007123;
    expected.parameter[1] = 0.26841320781355615;
    expected.closest[0][0] = { 0.85664850193120334, 0.51590051767664002 };
    expected.closest[0][1] = { 0.85664850193120334, 0.51590051767664002 };
    expected.closest[1][0] = { 0.51590051767664002, 0.85664850193120334 };
    expected.closest[1][1] = { 0.51590051767664002, 0.85664850193120334 };
    Validate(output, expected);

    // line intersects circle but "below" arc, arc endpoints
    // are equidistant from line
    arc.center = { 0.0, 0.0 };
    arc.radius = 1.0;
    arc.end[0] = arc.radius * Vector2<double>{ 1.0, 0.0 };
    arc.end[1] = arc.radius * Vector2<double>{ 0.0, 1.0 };
    line.origin = Vector2<double>{ 4.0, 3.0 } / 8.0;
    line.direction = { -1.0, 1.0 };
    output = query(line, arc);
    expected.distance = 0.088388347648318447;
    expected.sqrDistance = 0.0078125;
    expected.numClosestPairs = 2;
    expected.parameter[0] = -0.4375;
    expected.parameter[1] = 0.5625;
    expected.closest[0][0] = { 0.9375, -0.0625 };
    expected.closest[0][1] = { 1.0, 0.0 };
    expected.closest[1][0] = { -0.0625, 0.9375 };
    expected.closest[1][1] = { 0.0, 1.0 };
    Validate(output, expected);

    // line intersects circle but "below" arc, arc.end[1] is
    // closer to line than arc.end[0]
    arc.center = { 0.0, 0.0 };
    arc.radius = 1.0;
    arc.end[0] = arc.radius * Vector2<double>{ 1.0, 0.0 };
    arc.end[1] = arc.radius * Vector2<double>{ 0.0, 1.0 };
    line.origin = Vector2<double>{ 4.0, 3.0 } / 8.0;
    line.direction = { -1.0, 1.1 };
    output = query(line, arc);
    expected.distance = 0.050450459549723416;
    expected.sqrDistance = 0.0025452488687782784;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 0.53733031674208143;
    expected.closest[0][0] = { -0.037330316742081426, 0.96606334841628960 };
    expected.closest[0][1] = { 0.0, 1.0 };
    Validate(output, expected);

    // line intersects circle but "below" arc, arc.end[0] is
    // closer to line than arc.end[1]
    arc.center = { 0.0, 0.0 };
    arc.radius = 1.0;
    arc.end[0] = arc.radius * Vector2<double>{ 1.0, 0.0 };
    arc.end[1] = arc.radius * Vector2<double>{ 0.0, 1.0 };
    line.origin = Vector2<double>{ 4.0, 3.0 } / 8.0;
    line.direction = { -1.1, 1.0 };
    output = query(line, arc);
    expected.distance = 0.058858869474677333;
    expected.sqrDistance = 0.0034643665158371030;
    expected.numClosestPairs = 1;
    expected.parameter[0] = -0.41855203619909503;
    expected.closest[0][0] = { 0.96040723981900455, -0.043552036199095034 };
    expected.closest[0][1] = { 1.0, 0.0 };
    Validate(output, expected);
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/2D/DistLine2Arc2.h>

namespace gtl
{
    template class DCPQuery<float, Line<float, 2>, Arc2<float>>;
    template class DCPQuery<double, Line<double, 2>, Arc2<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Line<Rational, 2>, Arc2<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistLine2Arc2)
