#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/ND/DistRaySegment.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistRaySegment
    {
    public:
        UnitTestDistRaySegment();

    private:
        void Test2D();
        void Test3D();
    };
}

UnitTestDistRaySegment::UnitTestDistRaySegment()
{
    UTInformation("Mathematics/Distance/ND/DistRaySegment");

    Test2D();
    Test3D();
}

void UnitTestDistRaySegment::Test2D()
{
    using RaySegmentQuery = DCPQuery<double, Ray<double, 2>, Segment<double, 2>>;
    RaySegmentQuery query{};
    RaySegmentQuery::Output output{};
    std::array<Vector<double, 2>, 2> expectedClosest{};
    Ray<double, 2> ray{};
    Segment<double, 2> segment{};
    double error{};

    // lines transversely intersect, both closest points are interior
    ray.origin = { 0.0, 0.0 };
    ray.direction = { 3.0 / 5.0, 4.0 / 5.0 };
    segment.p[0] = { 2.0, 1.0 };
    segment.p[1] = { 0.0, 3.0 };
    output = query(ray, segment);
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
    ray.origin = { 0.0, 0.0 };
    ray.direction = { 3.0 / 5.0, 4.0 / 5.0 };
    segment.p[0] = { 2.0, 1.0 };
    segment.p[1] = { 4.0, 0.0 };
    output = query(ray, segment);
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
    ray.origin = { 0.0, 0.0 };
    ray.direction = { 3.0 / 5.0, 4.0 / 5.0 };
    segment.p[0] = { 4.0, 0.0 };
    segment.p[1] = { 2.0, 1.0 };
    output = query(ray, segment);
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

    // no intersection, ray origin is closest to segment interior
    ray.origin = { 0.0, 0.0 };
    ray.direction = { 3.0 / 5.0, 4.0 / 5.0 };
    segment.p[0] = { 0.0, -1.0 };
    segment.p[1] = { -1.0, 0.0 };
    output = query(ray, segment);
    error = std::fabs(output.sqrDistance - 0.5);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.5));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.5);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 0.0, 0.0 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { -0.5, -0.5 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // no intersection, ray origin is closest to segment.p[0]
    ray.origin = { 0.0, 0.0 };
    ray.direction = { 3.0 / 5.0, 4.0 / 5.0 };
    segment.p[0] = { 0.0, -1.0 };
    segment.p[1] = { 1.0, -1.0 };
    output = query(ray, segment);
    error = std::fabs(output.sqrDistance - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(1.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 0.0, 0.0 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 0.0, -1.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // no intersection, ray origin is closest to segment.p[1]
    ray.origin = { 0.0, 0.0 };
    ray.direction = { 3.0 / 5.0, 4.0 / 5.0 };
    segment.p[0] = { 1.0, -1.0 };
    segment.p[1] = { 0.0, -1.0 };
    output = query(ray, segment);
    error = std::fabs(output.sqrDistance - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(1.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 0.0, 0.0 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 0.0, -1.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // parallel but not coincident
    ray.origin = { 0.0, 0.0 };
    ray.direction = { 3.0, 4.0 };
    segment.p[0] = { -1.0, -1.0 };
    segment.p[1] = { 2.0, 3.0 };
    output = query(ray, segment);
    error = std::fabs(output.sqrDistance - 0.04);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.04));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 0.72);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 2.16, 2.88 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 2.0, 3.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // coincident
    ray.origin = { 0.0, 0.0 };
    ray.direction = { 3.0, 4.0 };
    segment.p[0] = { -1.5, -2.0 };
    segment.p[1] = { 1.5, 2.0 };
    output = query(ray, segment);
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
    error = std::fabs(output.parameter[1] - 1.0);
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

void UnitTestDistRaySegment::Test3D()
{
    using RaySegmentQuery = DCPQuery<double, Ray<double, 3>, Segment<double, 3>>;
    RaySegmentQuery query{};
    RaySegmentQuery::Output output{};
    std::array<Vector<double, 3>, 2> expectedClosest{};
    Ray<double, 3> ray{};
    Segment<double, 3> segment{};
    double error{};

    // lines transversely intersect, both closest points are interior
    ray.origin = { 0.0, 0.0, 0.0 };
    ray.direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    segment.p[0] = { 2.0, 1.0, 0.0 };
    segment.p[1] = { 0.0, 3.0, 0.0 };
    output = query(ray, segment);
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
    ray.origin = { 0.0, 0.0, 0.0 };
    ray.direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    segment.p[0] = { 2.0, 1.0, 0.0 };
    segment.p[1] = { 4.0, 0.0, 0.0 };
    output = query(ray, segment);
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
    ray.origin = { 0.0, 0.0, 0.0 };
    ray.direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    segment.p[0] = { 4.0, 0.0, 0.0 };
    segment.p[1] = { 2.0, 1.0, 0.0 };
    output = query(ray, segment);
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

    // no intersection, ray origin is closest to segment interior
    ray.origin = { 0.0, 0.0, 0.0 };
    ray.direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    segment.p[0] = { 0.0, -1.0, 0.0 };
    segment.p[1] = { -1.0, 0.0, 0.0 };
    output = query(ray, segment);
    error = std::fabs(output.sqrDistance - 0.5);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.5));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.5);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 0.0, 0.0, 0.0 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { -0.5, -0.5, 0.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // no intersection, ray origin is closest to segment.p[0]
    ray.origin = { 0.0, 0.0, 0.0 };
    ray.direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    segment.p[0] = { 0.0, -1.0, 0.0 };
    segment.p[1] = { 1.0, -1.0, 0.0 };
    output = query(ray, segment);
    error = std::fabs(output.sqrDistance - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(1.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 0.0, 0.0, 0.0 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 0.0, -1.0, 0.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // no intersection, ray origin is closest to segment.p[1]
    ray.origin = { 0.0, 0.0, 0.0 };
    ray.direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    segment.p[0] = { 1.0, -1.0, 0.0 };
    segment.p[1] = { 0.0, -1.0, 0.0 };
    output = query(ray, segment);
    error = std::fabs(output.sqrDistance - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(1.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 0.0, 0.0, 0.0 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 0.0, -1.0, 0.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // parallel but not coincident
    ray.origin = { 0.0, 0.0, 0.0 };
    ray.direction = { 3.0, 4.0, 0.0 };
    segment.p[0] = { -1.0, -1.0, 0.0 };
    segment.p[1] = { 2.0, 3.0, 0.0 };
    output = query(ray, segment);
    error = std::fabs(output.sqrDistance - 0.04);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.04));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 0.72);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 2.16, 2.88, 0.0 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 2.0, 3.0, 0.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // coincident
    ray.origin = { 0.0, 0.0, 0.0 };
    ray.direction = { 3.0, 4.0, 0.0 };
    segment.p[0] = { -1.5, -2.0, 0.0 };
    segment.p[1] = { 1.5, 2.0, 0.0 };
    output = query(ray, segment);
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
    error = std::fabs(output.parameter[1] - 1.0);
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
    ray.origin = { 0.0, 0.0, 0.0 };
    ray.direction = { 3.0, 4.0, 5.0 };
    segment.p[0] = { 1.0, 0.0, 0.0 };
    segment.p[1] = { 0.0, 1.0, 2.0 };
    output = query(ray, segment);
    error = std::fabs(output.sqrDistance - 0.050279329608938550);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.050279329608938550));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 0.16201117318435754);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.46368715083798884);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 0.48603351955307261, 0.64804469273743015, 0.81005586592178769 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 0.53631284916201116, 0.46368715083798884, 0.92737430167597767 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/ND/DistRaySegment.h>

namespace gtl
{
    template class DCPQuery<float, Ray<float, 2>, Segment<float, 2>>;
    template class DCPQuery<double, Ray<double, 3>, Segment<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Ray<Rational, 4>, Segment<Rational, 4>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistRaySegment)
