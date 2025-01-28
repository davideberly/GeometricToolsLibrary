#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/ND/DistRayRay.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistRayRay
    {
    public:
        UnitTestDistRayRay();

    private:
        void Test2D();
        void Test3D();
    };
}

UnitTestDistRayRay::UnitTestDistRayRay()
{
    UTInformation("Mathematics/Distance/ND/DistRayRay");

    Test2D();
    Test3D();
}

void UnitTestDistRayRay::Test2D()
{
    using RayRayQuery = DCPQuery<double, Ray<double, 2>, Ray<double, 2>>;
    RayRayQuery query{};
    RayRayQuery::Output output{};
    std::array<Vector<double, 2>, 2> expectedClosest{};
    std::array<Ray<double, 2>, 2> ray{};
    double error{};

    // lines transversely intersect, both ray points interior
    ray[0].origin = { 0.0, 0.0 };
    ray[0].direction = { 3.0 / 5.0, 4.0 / 5.0 };
    ray[1].origin = { 2.0, 1.0 };
    ray[1].direction = { -1.0, 1.0 };
    output = query(ray[0], ray[1]);
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

    // lines transversely intersect, ray[1] origin is closest
    ray[0].origin = { 0.0, 0.0 };
    ray[0].direction = { 3.0 / 5.0, 4.0 / 5.0 };
    ray[1].origin = { 0.0, 1.0 };
    ray[1].direction = { -1.0, 1.0 };
    output = query(ray[0], ray[1]);
    error = std::fabs(output.sqrDistance - 0.36);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.36));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 0.8);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 0.48, 0.64 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 0.0, 1.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // lines transversely intersect, ray[0] origin is closest
    ray[0].origin = { 0.0, 0.0 };
    ray[0].direction = { 3.0 / 5.0, 4.0 / 5.0 };
    ray[1].origin = { 0.0, -1.0 };
    ray[1].direction = { -1.0, 1.0 };
    output = query(ray[0], ray[1]);
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

    // lines transversely intersect, ray[0] and ray[1] origins are closest
    ray[0].origin = { 0.0, 0.0 };
    ray[0].direction = { 3.0 / 5.0, 4.0 / 5.0 };
    ray[1].origin = { 0.0, -1.0 };
    ray[1].direction = { 1.0, -1.0 };
    output = query(ray[0], ray[1]);
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

    // parallel but not coincident
    ray[0].origin = { 0.0, 0.0 };
    ray[0].direction = { 3.0 / 5.0, 4.0 / 5.0 };
    ray[1].origin = { 2.0, 1.0 };
    ray[1].direction = ray[0].direction;
    output = query(ray[0], ray[1]);
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

    // ray[0] and ray[1] are coincident
    ray[0].origin = { 0.0, 0.0 };
    ray[0].direction = { 3.0 / 5.0, 4.0 / 5.0 };
    ray[1].origin = { 3.0, 4.0 };
    ray[1].direction = { 3.0 / 2.0, 4.0 / 2.0 };
    output = query(ray[0], ray[1]);
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

    output = query(ray[1], ray[0]);
    error = std::fabs(output.sqrDistance - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 5.0);
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

void UnitTestDistRayRay::Test3D()
{
    using RayRayQuery = DCPQuery<double, Ray<double, 3>, Ray<double, 3>>;
    RayRayQuery query{};
    RayRayQuery::Output output{};
    std::array<Vector<double, 3>, 2> expectedClosest{};
    std::array<Ray<double, 3>, 2> ray{};
    double error{};

    // lines transversely intersect, both ray points interior
    ray[0].origin = { 0.0, 0.0, 0.0 };
    ray[0].direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    ray[1].origin = { 2.0, 1.0, 0.0 };
    ray[1].direction = { -1.0, 1.0, 0.0 };
    output = query(ray[0], ray[1]);
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

    // lines transversely intersect, ray[1] origin is closest
    ray[0].origin = { 0.0, 0.0, 0.0 };
    ray[0].direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    ray[1].origin = { 0.0, 1.0, 0.0 };
    ray[1].direction = { -1.0, 1.0, 0.0 };
    output = query(ray[0], ray[1]);
    error = std::fabs(output.sqrDistance - 0.36);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.36));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 0.8);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { 0.48, 0.64, 0.0 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { 0.0, 1.0, 0.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // lines transversely intersect, ray[0] origin is closest
    ray[0].origin = { 0.0, 0.0, 0.0 };
    ray[0].direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    ray[1].origin = { 0.0, -1.0, 0.0 };
    ray[1].direction = { -1.0, 1.0, 0.0 };
    output = query(ray[0], ray[1]);
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

    // lines transversely intersect, ray[0] and ray[1] origins are closest
    ray[0].origin = { 0.0, 0.0, 0.0 };
    ray[0].direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    ray[1].origin = { 0.0, -1.0, 0.0 };
    ray[1].direction = { 1.0, -1.0, 0.0 };
    output = query(ray[0], ray[1]);
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

    // parallel but not coincident
    ray[0].origin = { 0.0, 0.0, 0.0 };
    ray[0].direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    ray[1].origin = { 2.0, 1.0, 0.0 };
    ray[1].direction = ray[0].direction;
    output = query(ray[0], ray[1]);
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

    // ray[0] and ray[1] are coincident
    ray[0].origin = { 0.0, 0.0, 0.0 };
    ray[0].direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    ray[1].origin = { 3.0, 4.0, 0.0 };
    ray[1].direction = { 3.0 / 2.0, 4.0 / 2.0, 0.0 };
    output = query(ray[0], ray[1]);
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

    output = query(ray[1], ray[0]);
    error = std::fabs(output.sqrDistance - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 5.0);
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

    // no intersection, not parallel, ray origins closest
    ray[0].origin = { 0.0, 0.0, 0.0 };
    ray[0].direction = { 3.0, 4.0, 5.0 };
    ray[1].origin = { -1.0, 0.0, 0.0 };
    ray[1].direction = { -1.0, 0.0, 2.0 };
    output = query(ray[0], ray[1]);
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
    expectedClosest[1] = { -1.0, 0.0, 0.0 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // no intersection, not parallel, interiors closest
    ray[0].origin = { -3.0, -4.0, -5.0 };
    ray[0].direction = { 3.0, 4.0, 5.0 };
    ray[1].origin = { -1.0, 0.0, 0.0 };
    ray[1].direction = { 1.0, 0.0, 2.0 };
    output = query(ray[0], ray[1]);
    error = std::fabs(output.sqrDistance - 0.79012345679012341);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.79012345679012341));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[0] - 0.97530864197530864);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter[1] - 0.13580246913580246);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[0] = { -0.074074074074073959, -0.098765432098765427, -0.12345679012345645 };
    error = Length(output.closest[0] - expectedClosest[0]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    expectedClosest[1] = { -0.86419753086419759, 0.0, 0.27160493827160492 };
    error = Length(output.closest[1] - expectedClosest[1]);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/ND/DistRayRay.h>

namespace gtl
{
    template class DCPQuery<float, Ray<float, 2>, Ray<float, 2>>;
    template class DCPQuery<double, Ray<double, 3>, Ray<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Ray<Rational, 4>, Ray<Rational, 4>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistRayRay)
