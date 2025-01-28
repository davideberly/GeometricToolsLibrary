#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/ND/DistLineSegment.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistLineSegment
    {
    public:
        UnitTestDistLineSegment();

    private:
        void Test2D();
        void Test3D();
    };
}

UnitTestDistLineSegment::UnitTestDistLineSegment()
{
    UTInformation("Mathematics/Distance/ND/DistLineSegment");

    Test2D();
    Test3D();
}

void UnitTestDistLineSegment::Test2D()
{
    using LineSegmentQuery = DCPQuery<double, Line<double, 2>, Segment<double, 2>>;
    LineSegmentQuery query{};
    LineSegmentQuery::Output output{};
    std::array<Vector<double, 2>, 2> expectedClosest{};
    Line<double, 2> line{};
    Segment<double, 2> segment{};
    double error{};

    // lines transversely intersect at segment interior point
    line.origin = { 0.0, 0.0 };
    line.direction = { 3.0 / 5.0, 4.0 / 5.0 };
    segment.p[0] = { 2.0, 1.0 };
    segment.p[1] = { 0.0, 3.0 };
    output = query(line, segment);
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
    error = std::fabs(output.parameter[1] - 0.35714285714285721);
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

    // no intersection, segment.p[0] is closest
    line.origin = { 0.0, 0.0 };
    line.direction = { 3.0 / 5.0, 4.0 / 5.0 };
    segment.p[0] = { 2.0, 1.0 };
    segment.p[1] = { 4.0, 0.0 };
    output = query(line, segment);
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

    // no intersection, segment.p[1] is closest
    line.origin = { 0.0, 0.0 };
    line.direction = { 3.0 / 5.0, 4.0 / 5.0 };
    segment.p[0] = { 4.0, 0.0 };
    segment.p[1] = { 2.0, 1.0 };
    output = query(line, segment);
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
    error = std::fabs(output.parameter[1] - 1.0);
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

    // parallel but not coincident
    line.origin = { 0.0, 0.0 };
    line.direction = { 3.0, 4.0 };
    segment.p[0] = { 1.0, 1.0 };
    segment.p[1] = { 4.0, 5.0 };
    output = query(line, segment);
    error = std::fabs(output.sqrDistance - 0.04);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.04));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 0.28);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 0.84, 1.12 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 1.0, 1.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // parallel but not coincident (opposite direction)
    line.origin = { 0.0, 0.0 };
    line.direction = { 3.0, 4.0 };
    segment.p[0] = { 1.0, 1.0 };
    segment.p[1] = { -2.0, -3.0 };
    output = query(line, segment);
    error = std::fabs(output.sqrDistance - 0.04);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.04));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 0.28);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 0.84, 1.12 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 1.0, 1.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // coincident
    line.origin = { 0.0, 0.0 };
    line.direction = { 3.0, 4.0 };
    segment.p[0] = { 1.5, 2.0 };
    segment.p[1] = { 4.5, 6.0 };
    output = query(line, segment);
    error = std::fabs(output.sqrDistance - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 0.5);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 1.5, 2.0 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 1.5, 2.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
}

void UnitTestDistLineSegment::Test3D()
{
    using LineSegmentQuery = DCPQuery<double, Line<double, 3>, Segment<double, 3>>;
    LineSegmentQuery query{};
    LineSegmentQuery::Output output{};
    std::array<Vector<double, 3>, 2> expectedClosest{};
    Line<double, 3> line{};
    Segment<double, 3> segment{};
    double error{};

    // lines transversely intersect at segment interior point
    line.origin = { 0.0, 0.0, 0.0 };
    line.direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    segment.p[0] = { 2.0, 1.0, 0.0 };
    segment.p[1] = { 0.0, 3.0, 0.0 };
    output = query(line, segment);
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
    error = std::fabs(output.parameter[1] - 0.35714285714285721);
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

    // no intersection, segment.p[0] is closest
    line.origin = { 0.0, 0.0, 0.0 };
    line.direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    segment.p[0] = { 2.0, 1.0, 0.0 };
    segment.p[1] = { 4.0, 0.0, 0.0 };
    output = query(line, segment);
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

    // no intersection, segment.p[1] is closest
    line.origin = { 0.0, 0.0, 0.0 };
    line.direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    segment.p[0] = { 4.0, 0.0, 0.0 };
    segment.p[1] = { 2.0, 1.0, 0.0 };
    output = query(line, segment);
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
    error = std::fabs(output.parameter[1] - 1.0);
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

    // parallel but not coincident
    line.origin = { 0.0, 0.0, 0.0 };
    line.direction = { 3.0, 4.0, 0.0 };
    segment.p[0] = { 1.0, 1.0, 0.0 };
    segment.p[1] = { 4.0, 5.0, 0.0 };
    output = query(line, segment);
    error = std::fabs(output.sqrDistance - 0.04);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.04));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 0.28);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 0.84, 1.12, 0.0 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 1.0, 1.0, 0.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // parallel but not coincident (opposite direction)
    line.origin = { 0.0, 0.0, 0.0 };
    line.direction = { 3.0, 4.0, 0.0 };
    segment.p[0] = { 1.0, 1.0, 0.0 };
    segment.p[1] = { -2.0, -3.0, 0.0 };
    output = query(line, segment);
    error = std::fabs(output.sqrDistance - 0.04);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.04));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 0.28);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 0.84, 1.12, 0.0 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 1.0, 1.0, 0.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // coincident
    line.origin = { 0.0, 0.0, 0.0 };
    line.direction = { 3.0, 4.0, 0.0 };
    segment.p[0] = { 1.5, 2.0, 0.0 };
    segment.p[1] = { 4.5, 6.0, 0.0 };
    output = query(line, segment);
    error = std::fabs(output.sqrDistance - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 0.5);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 1.5, 2.0, 0.0 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 1.5, 2.0, 0.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // no intersection, not parallel
    line.origin = { 0.0, 0.0, 0.0 };
    line.direction = { 3.0, 4.0, 5.0 };
    segment.p[0] = { -1.0, 0.0, 0.0 };
    segment.p[1] = { -2.0, 0.0, 2.0 };
    output = query(line, segment);
    error = std::fabs(output.sqrDistance - 0.82);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.82));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] + 0.06);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { -0.18, -0.24, -0.3 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { -1.0, 0.0, 0.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/ND/DistLineSegment.h>

namespace gtl
{
    template class DCPQuery<float, Line<float, 2>, Segment<float, 2>>;
    template class DCPQuery<double, Line<double, 3>, Segment<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Line<Rational, 4>, Segment<Rational, 4>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistLineSegment)
