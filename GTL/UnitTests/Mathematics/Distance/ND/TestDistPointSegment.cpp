#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/ND/DistPointSegment.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistPointSegment
    {
    public:
        UnitTestDistPointSegment();

    private:
        void Test2D();
        void Test3D();
    };
}

UnitTestDistPointSegment::UnitTestDistPointSegment()
{
    UTInformation("Mathematics/Distance/ND/DistPointSegment");

    Test2D();
    Test3D();
}

void UnitTestDistPointSegment::Test2D()
{
    using DCPPointSegmentQuery = DCPQuery<double, Vector<double, 2>, Segment<double, 2>>;
    DCPPointSegmentQuery query{};
    DCPPointSegmentQuery::Output output{};
    Vector<double, 2> expectedClosest{};
    double error{};

    Segment<double, 2> segment{};
    Vector<double, 2> point{};

    segment.p[0] = { 1.0, 1.0 };
    segment.p[1] = { 2.0, 3.0 };

    point = { 2.0, 1.0 };
    output = query(point, segment);
    error = std::fabs(output.sqrDistance - 0.8);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.8));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter - 0.2);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest = { 1.2, 1.4 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");

    point = { 2.0, 0.0 };
    output = query(point, segment);
    error = std::fabs(output.sqrDistance - 2.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(2.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    UTAssert(
        output.parameter == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 1.0, 1.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");

    point = { 3.0, 3.0 };
    output = query(point, segment);
    error = std::fabs(output.sqrDistance - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    UTAssert(
        output.parameter == 1.0,
        "Invalid DCPQuery.");
    expectedClosest = { 2.0, 3.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
}

void UnitTestDistPointSegment::Test3D()
{
    using DCPPointSegmentQuery = DCPQuery<double, Vector<double, 3>, Segment<double, 3>>;
    DCPPointSegmentQuery query{};
    DCPPointSegmentQuery::Output output{};
    Vector<double, 3> expectedClosest{};
    double error{};

    Segment<double, 3> segment{};
    Vector<double, 3> point{};

    segment.p[0] = { 1.0, 1.0, 1.0 };
    segment.p[1] = { 2.0, 3.0, 4.0 };

    point = { 2.0, 1.0, 1.0 };
    output = query(point, segment);
    error = std::fabs(output.sqrDistance - 13.0 / 14.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(13.0 / 14.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter - 1.0 / 14.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest = { 15.0 / 14.0, 16.0 / 14.0, 17.0 / 14.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");

    point = { 2.0, 1.0, -1.0 };
    output = query(point, segment);
    error = std::fabs(output.sqrDistance - 5.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(5.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    UTAssert(
        output.parameter == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 1.0, 1.0, 1.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");

    point = { 3.0, 3.0, 4.0 };
    output = query(point, segment);
    error = std::fabs(output.sqrDistance - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    UTAssert(
        output.parameter == 1.0,
        "Invalid DCPQuery.");
    expectedClosest = { 2.0, 3.0, 4.0 };
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
#include <GTL/Mathematics/Distance/ND/DistPointSegment.h>

namespace gtl
{
    template class DCPQuery<float, Vector<float, 2>, Segment<float, 2>>;
    template class DCPQuery<double, Vector<double, 3>, Segment<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Vector<Rational, 4>, Segment<Rational, 4>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPointSegment)
