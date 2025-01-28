#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/ND/DistLineLine.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistLineLine
    {
    public:
        UnitTestDistLineLine();

    private:
        void Test2D();
        void Test3D();
    };
}

UnitTestDistLineLine::UnitTestDistLineLine()
{
    UTInformation("Mathematics/Distance/ND/DistLineLine");

    Test2D();
    Test3D();
}

void UnitTestDistLineLine::Test2D()
{
    using LineLineQuery = DCPQuery<double, Line<double, 2>, Line<double, 2>>;
    LineLineQuery query{};
    LineLineQuery::Output output{};
    std::array<Vector<double, 2>, 2> expectedClosest{};
    std::array<Line<double, 2>, 2> line{};
    double error{};

    // transverse intersection
    line[0].origin = { 0.0, 0.0 };
    line[0].direction = { 3.0 / 5.0, 4.0 / 5.0 };
    line[1].origin = { 2.0, 1.0 };
    line[1].direction = { -1.0, 1.0 };
    output = query(line[0], line[1]);
    error = std::fabs(output.sqrDistance - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 2.1428571428571428);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.71428571428571441);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 1.2857142857142856, 1.7142857142857144 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 1.2857142857142856, 1.7142857142857144 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // parallel but not coincident
    line[0].origin = { 0.0, 0.0 };
    line[0].direction = { 3.0 / 5.0, 4.0 / 5.0 };
    line[1].origin = { 2.0, 1.0 };
    line[1].direction = line[0].direction;
    output = query(line[0], line[1]);
    error = std::fabs(output.sqrDistance - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(1.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 2.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 1.2, 1.6 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 2.0, 1.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // same line
    line[0].origin = { 0.0, 0.0 };
    line[0].direction = { 3.0 / 5.0, 4.0 / 5.0 };
    line[1].origin = { 3.0, 4.0 };
    line[1].direction = { 3.0 / 2.0, 4.0 / 2.0 };
    output = query(line[0], line[1]);
    error = std::fabs(output.sqrDistance - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 5.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 3.0, 4.0 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 3.0, 4.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
}

void UnitTestDistLineLine::Test3D()
{
    using LineLineQuery = DCPQuery<double, Line<double, 3>, Line<double, 3>>;
    LineLineQuery query{};
    LineLineQuery::Output output{};
    std::array<Vector<double, 3>, 2> expectedClosest{};
    std::array<Line<double, 3>, 2> line{};
    double error{};

    // transverse intersection (2D)
    line[0].origin = { 0.0, 0.0, 0.0 };
    line[0].direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    line[1].origin = { 2.0, 1.0, 0.0 };
    line[1].direction = { -1.0, 1.0, 0.0 };
    output = query(line[0], line[1]);
    error = std::fabs(output.sqrDistance - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 2.1428571428571428);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.71428571428571441);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 1.2857142857142856, 1.7142857142857144, 0.0 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 1.2857142857142856, 1.7142857142857144, 0.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // parallel but not coincident (2D)
    line[0].origin = { 0.0, 0.0, 0.0 };
    line[0].direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    line[1].origin = { 2.0, 1.0, 0.0 };
    line[1].direction = line[0].direction;
    output = query(line[0], line[1]);
    error = std::fabs(output.sqrDistance - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(1.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 2.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 1.2, 1.6, 0.0 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 2.0, 1.0, 0.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // same line (2D)
    line[0].origin = { 0.0, 0.0, 0.0 };
    line[0].direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    line[1].origin = { 3.0, 4.0, 0.0 };
    line[1].direction = { 3.0 / 2.0, 4.0 / 2.0, 0.0 };
    output = query(line[0], line[1]);
    error = std::fabs(output.sqrDistance - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 5.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 3.0, 4.0, 0.0 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 3.0, 4.0, 0.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // no intersection, not parallel
    line[0].origin = { 0.0, 0.0, 0.0 };
    line[0].direction = { 3.0, 4.0, 5.0 };
    line[1].origin = { 1.0, 1.0, 1.0 };
    line[1].direction = { -1.0, 0.0, 2.0 };
    output = query(line[0], line[1]);
    error = std::fabs(output.sqrDistance - 0.0049751243781094526);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.0049751243781094526));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 0.26368159203980102);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.16915422885572140);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 0.79104477611940305, 1.0547263681592041, 1.3184079601990051 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 0.83084577114427860, 1.0, 1.3383084577114428 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/ND/DistLineLine.h>

namespace gtl
{
    template class DCPQuery<float, Line<float, 2>, Line<float, 2>>;
    template class DCPQuery<double, Line<double, 3>, Line<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Line<Rational, 4>, Line<Rational, 4>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistLineLine)
