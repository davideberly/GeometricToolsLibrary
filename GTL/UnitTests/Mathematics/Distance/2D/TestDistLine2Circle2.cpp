#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/2D/DistLine2Circle2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistLine2Circle2
    {
    public:
        UnitTestDistLine2Circle2();

    private:
        using LCQuery = DCPQuery<double, Line2<double>, Circle2<double>>;
        using LCOutput = LCQuery::Output;

        void Test();

        // The expected values are passed to this function.
        void Validate(LCOutput const& output, LCOutput const& expected);
    };
}

UnitTestDistLine2Circle2::UnitTestDistLine2Circle2()
{
    UTInformation("Mathematics/Distance/2D/DistLine2Circle2");

    Test();
}

void UnitTestDistLine2Circle2::Validate(LCOutput const& output, LCOutput const& expected)
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

void UnitTestDistLine2Circle2::Test()
{
    Line2<double> line{};
    Circle2<double> circle{};
    LCQuery query{};
    LCQuery::Output output{}, expected{};
    Vector2<double> trn = { 0.01234, 0.56789 };

    circle.center = { 0.0, 0.0 };
    circle.radius = 1.0;
    line.origin = { 4.0, 3.0 };
    line.direction = { -1.0, 1.0 };
    output = query(line, circle);
    expected.distance = 3.9497474683058327;
    expected.sqrDistance = 15.600505063388335;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 0.5;
    expected.closest[0][0] = { 3.5, 3.5 };
    expected.closest[0][1] = { 0.70710678118654746, 0.70710678118654746 };
    Validate(output, expected);

    circle.center += trn;
    line.origin += trn;
    output = query(line, circle);
    expected.distance = 3.9497474683058327;
    expected.sqrDistance = 15.600505063388336;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 0.5;
    expected.closest[0][0] = { 3.51234, 4.06789 };
    expected.closest[0][1] = { 0.71944678118654748, 1.2749967811865475 };
    Validate(output, expected);

    circle.center = { 0.0, 0.0 };
    circle.radius = 1.0;
    line.origin = { 4.0, 3.0 };
    line.direction = -line.direction;
    output = query(line, circle);
    expected.distance = 3.9497474683058327;
    expected.sqrDistance = 15.600505063388336;
    expected.numClosestPairs = 1;
    expected.parameter[0] = -0.5;
    expected.closest[0][0] = { 3.5, 3.5 };
    expected.closest[0][1] = { 0.70710678118654746, 0.70710678118654746 };
    Validate(output, expected);

    circle.center += trn;
    line.origin += trn;
    output = query(line, circle);
    expected.distance = 3.9497474683058327;
    expected.sqrDistance = 15.600505063388336;
    expected.numClosestPairs = 1;
    expected.parameter[0] = -0.5;
    expected.closest[0][0] = { 3.51234, 4.06789 };
    expected.closest[0][1] = { 0.71944678118654748, 1.2749967811865475 };
    Validate(output, expected);

    circle.center = { 0.0, 0.0 };
    circle.radius = 1.0;
    line.origin = { 0.25, 0.25 };
    line.direction = { 4.0, 3.0 };
    output = query(line, circle);
    expected.distance = 0.0;
    expected.sqrDistance = 0.0;
    expected.numClosestPairs = 2;
    expected.parameter[0] = -0.26974984355438175;
    expected.parameter[1] = 0.12974984355438179;
    expected.closest[0][0] = { -0.82899937421752701, -0.55924953066314531 };
    expected.closest[0][1] = { -0.82899937421752701, -0.55924953066314531 };
    expected.closest[1][0] = { 0.76899937421752718, 0.63924953066314538 };
    expected.closest[1][1] = { 0.76899937421752718, 0.63924953066314538 };
    Validate(output, expected);

    circle.center += trn;
    line.origin += trn;
    output = query(line, circle);
    expected.distance = 0.0;
    expected.sqrDistance = 0.0;
    expected.numClosestPairs = 2;
    expected.parameter[0] = -0.26974984355438175;
    expected.parameter[1] = 0.12974984355438179;
    expected.closest[0][0] = { -0.81665937421752699, 0.0086404693368546948 };
    expected.closest[0][1] = { -0.81665937421752699, 0.0086404693368546948 };
    expected.closest[1][0] = { 0.78133937421752719, 1.2071395306631454 };
    expected.closest[1][1] = { 0.78133937421752719, 1.2071395306631454 };
    Validate(output, expected);

    circle.center = { 0.0, 0.0 };
    circle.radius = 5.0;
    line.direction = { 4.0, -3.0 };
    line.origin = Vector2<double>{ 3.0, 4.0 } + 2.0 * line.direction;
    output = query(line, circle);
    expected.distance = 0.0;
    expected.sqrDistance = 0.0;
    expected.numClosestPairs = 1;
    expected.parameter[0] = -2.0;
    expected.closest[0][0] = { 3.0, 4.0 };
    expected.closest[0][1] = { 3.0, 4.0 };
    Validate(output, expected);

    circle.center += trn;
    line.origin += trn;
    output = query(line, circle);
    expected.distance = 0.0;
    expected.sqrDistance = 0.0;
    expected.numClosestPairs = 1;
    expected.parameter[0] = -2.0;
    expected.closest[0][0] = { 3.01234, 4.56789 };
    expected.closest[0][1] = { 3.01234, 4.56789 };
    Validate(output, expected);
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/2D/DistLine2Circle2.h>

namespace gtl
{
    template class DCPQuery<float, Line<float, 2>, Circle2<float>>;
    template class DCPQuery<double, Line<double, 2>, Circle2<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Line<Rational, 2>, Circle2<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistLine2Circle2)
