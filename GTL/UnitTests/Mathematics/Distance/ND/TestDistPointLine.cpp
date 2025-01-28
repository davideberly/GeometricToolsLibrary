#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/ND/DistPointLine.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistPointLine
    {
    public:
        UnitTestDistPointLine();

    private:
        void Test2D();
        void Test3D();
    };
}

UnitTestDistPointLine::UnitTestDistPointLine()
{
    UTInformation("Mathematics/Distance/ND/DistPointLine");

    Test2D();
    Test3D();
}

void UnitTestDistPointLine::Test2D()
{
    using DCPPointLineQuery = DCPQuery<double, Vector<double, 2>, Line<double, 2>>;
    DCPPointLineQuery query{};
    DCPPointLineQuery::Output output{};
    double expectedSqrDistance{}, expectedParameter{};
    Vector<double, 2> expectedClosest{};
    double error{};
    Line<double, 2> line{};
    Vector<double, 2> point{};

    // normalized
    line.origin = { 1.0, 1.0 };
    line.direction = { 1.0, 2.0 };
    Normalize(line.direction);

    point = { 2.0, 1.0 };
    output = query(point, line);
    expectedSqrDistance = 0.8;
    expectedParameter = 1.0 / std::sqrt(5.0);
    expectedClosest = { 1.2, 1.4 };
    error = std::fabs(output.sqrDistance - expectedSqrDistance);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter - expectedParameter);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");

    // unnormalized
    line.origin = { 1.0, 1.0 };
    line.direction = { 1.0, 2.0 };

    point = { 2.0, 1.0 };
    output = query(point, line);
    expectedSqrDistance = 0.8;
    expectedParameter = 0.2;
    expectedClosest = { 1.2, 1.4 };
    error = std::fabs(output.sqrDistance - expectedSqrDistance);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter - expectedParameter);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
}

void UnitTestDistPointLine::Test3D()
{
    using DCPPointLineQuery = DCPQuery<double, Vector<double, 3>, Line<double, 3>>;
    DCPPointLineQuery query{};
    DCPPointLineQuery::Output output{};
    double expectedSqrDistance{}, expectedParameter{};
    Vector<double, 3> expectedClosest{};
    double error{};
    Line<double, 3> line{};
    Vector<double, 3> point{};

    // normalized
    line.origin = { 1.0, 1.0, 1.0 };
    line.direction = { 1.0, 2.0, 3.0 };
    Normalize(line.direction);

    point = { 2.0, 1.0, -1.0 };
    output = query(point, line);
    expectedSqrDistance = 225.0 / 70.0;
    expectedParameter = -5.0 / std::sqrt(14.0);
    expectedClosest = { 9.0 / 14.0, 4.0 / 14.0, -1.0 / 14.0 };
    error = std::fabs(output.sqrDistance - expectedSqrDistance);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter - expectedParameter);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest = { 9.0 / 14.0, 4.0 / 14.0, -1.0 / 14.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");

    // unnormalized
    line.origin = { 1.0, 1.0, 1.0 };
    line.direction = { 1.0, 2.0, 3.0 };

    point = { 2.0, 1.0, -1.0 };
    output = query(point, line);
    expectedSqrDistance = 225.0 / 70.0;
    expectedParameter = -2.5 / 7.0;
    expectedClosest = { 9.0 / 14.0, 4.0 / 14.0, -1.0 / 14.0 };
    error = std::fabs(output.sqrDistance - expectedSqrDistance);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter - expectedParameter);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/ND/DistPointLine.h>

namespace gtl
{
    template class DCPQuery<float, Vector<float, 2>, Line<float, 2>>;
    template class DCPQuery<double, Vector<double, 3>, Line<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Vector<Rational, 4>, Line<Rational, 4>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPointLine)
