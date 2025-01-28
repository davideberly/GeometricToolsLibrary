#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/ND/DistPointRay.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistPointRay
    {
    public:
        UnitTestDistPointRay();

    private:
        void Test2D();
        void Test3D();
    };
}

UnitTestDistPointRay::UnitTestDistPointRay()
{
    UTInformation("Mathematics/Distance/ND/DistPointRay");

    Test2D();
    Test3D();
}

void UnitTestDistPointRay::Test2D()
{
    using DCPPointRayQuery = DCPQuery<double, Vector<double, 2>, Ray<double, 2>>;
    DCPPointRayQuery query{};
    DCPPointRayQuery::Output output{};
    double expectedSqrDistance{}, expectedParameter{};
    Vector<double, 2> expectedClosest{};
    double error{};
    Ray<double, 2> ray{};
    Vector<double, 2> point{};

    // normalized
    ray.origin = { 1.0, 1.0 };
    ray.direction = { 1.0, 2.0 };
    Normalize(ray.direction);

    point = { 2.0, 1.0 };
    output = query(point, ray);
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

    point = { 2.0, 0.0 };
    output = query(point, ray);
    expectedSqrDistance = 2.0;
    expectedParameter = 0.0;
    expectedClosest = { 1.0, 1.0 };
    error = std::fabs(output.sqrDistance - 2.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter - expectedParameter);
    UTAssert(
        error == 0.0,
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

    // unnormalized
    ray.origin = { 1.0, 1.0 };
    ray.direction = { 1.0, 2.0 };

    point = { 2.0, 1.0 };
    output = query(point, ray);
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

    point = { 2.0, 0.0 };
    output = query(point, ray);
    expectedSqrDistance = 2.0;
    expectedParameter = 0.0;
    expectedClosest = { 1.0, 1.0 };
    error = std::fabs(output.sqrDistance - expectedSqrDistance);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter - expectedParameter);
    UTAssert(
        error == 0.0,
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

void UnitTestDistPointRay::Test3D()
{
    using DCPPointRayQuery = DCPQuery<double, Vector<double, 3>, Ray<double, 3>>;
    DCPPointRayQuery query{};
    DCPPointRayQuery::Output output{};
    double expectedSqrDistance{}, expectedParameter{};
    Vector<double, 3> expectedClosest{};
    double error{};
    Ray<double, 3> ray{};
    Vector<double, 3> point{};

    // normalized
    ray.origin = { 1.0, 1.0, 1.0 };
    ray.direction = { 1.0, 2.0, 3.0 };
    Normalize(ray.direction);

    point = { 2.0, 1.0, 1.0 };
    output = query(point, ray);
    expectedSqrDistance = 13.0 / 14.0;
    expectedParameter = 1.0 / std::sqrt(14.0);
    expectedClosest = { 15.0 / 14.0, 16.0 / 14.0, 17.0 / 14.0 };
    error = std::fabs(output.sqrDistance - expectedSqrDistance);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter - expectedParameter);
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
    output = query(point, ray);
    expectedSqrDistance = 5.0;
    expectedParameter = 0.0;
    expectedClosest = { 1.0, 1.0, 1.0 };
    error = std::fabs(output.sqrDistance - expectedSqrDistance);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter - expectedParameter);
    UTAssert(
        error == 0.0,
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
    ray.origin = { 1.0, 1.0, 1.0 };
    ray.direction = { 1.0, 2.0, 3.0 };

    point = { 2.0, 1.0, 1.0 };
    output = query(point, ray);
    expectedSqrDistance = 13.0 / 14.0;
    expectedParameter = 0.5 / 7.0;
    expectedClosest = { 15.0 / 14.0, 16.0 / 14.0, 17.0 / 14.0 };
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

    point = { 2.0, 1.0, -1.0 };
    output = query(point, ray);
    expectedSqrDistance = 5.0;
    expectedParameter = 0.0;
    expectedClosest = { 1.0, 1.0, 1.0 };
    error = std::fabs(output.sqrDistance - expectedSqrDistance);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter - expectedParameter);
    UTAssert(
        error == 0.0,
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
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/ND/DistPointRay.h>

namespace gtl
{
    template class DCPQuery<float, Vector<float, 2>, Ray<float, 2>>;
    template class DCPQuery<double, Vector<double, 3>, Ray<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Vector<Rational, 4>, Ray<Rational, 4>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPointRay)
