#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/ND/DistLineRay.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistLineRay
    {
    public:
        UnitTestDistLineRay();

    private:
        void Test2D();
        void Test3D();
    };
}

UnitTestDistLineRay::UnitTestDistLineRay()
{
    UTInformation("Mathematics/Distance/ND/DistLineRay");

    Test2D();
    Test3D();
}

void UnitTestDistLineRay::Test2D()
{
    using LineRayQuery = DCPQuery<double, Line<double, 2>, Ray<double, 2>>;
    LineRayQuery query{};
    LineRayQuery::Output output{};
    std::array<Vector<double, 2>, 2> expectedClosest{};
    Line<double, 2> line{};
    Ray<double, 2> ray{};
    double error{};

    // lines transversely intersect, ray origin is not closest
    line.origin = { 0.0, 0.0 };
    line.direction = { 3.0 / 5.0, 4.0 / 5.0 };
    ray.origin = { 2.0, 1.0 };
    ray.direction = { -1.0, 1.0 };
    output = query(line, ray);
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

    // lines transversely intersect, ray origin is closest
    line.origin = { 0.0, 0.0 };
    line.direction = { 3.0 / 5.0, 4.0 / 5.0 };
    ray.origin = { 0.0, 1.0 };
    ray.direction = { -1.0, 1.0 };
    output = query(line, ray);
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

    // parallel but not coincident
    line.origin = { 0.0, 0.0 };
    line.direction = { 3.0 / 5.0, 4.0 / 5.0 };
    ray.origin = { 2.0, 1.0 };
    ray.direction = line.direction;
    output = query(line, ray);
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

    // line and ray are coincident
    line.origin = { 0.0, 0.0 };
    line.direction = { 3.0 / 5.0, 4.0 / 5.0 };
    ray.origin = { 3.0, 4.0 };
    ray.direction = { 3.0 / 2.0, 4.0 / 2.0 };
    output = query(line, ray);
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

void UnitTestDistLineRay::Test3D()
{
    using LineRayQuery = DCPQuery<double, Line<double, 3>, Ray<double, 3>>;
    LineRayQuery query{};
    LineRayQuery::Output output{};
    std::array<Vector<double, 3>, 2> expectedClosest{};
    Line<double, 3> line{};
    Ray<double, 3> ray{};
    double error{};

    // lines transversely intersect, ray origin is not closest
    line.origin = { 0.0, 0.0, 0.0 };
    line.direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    ray.origin = { 2.0, 1.0, 0.0 };
    ray.direction = { -1.0, 1.0, 0.0 };
    output = query(line, ray);
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

    // lines transversely intersect, ray origin is closest
    line.origin = { 0.0, 0.0, 0.0 };
    line.direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    ray.origin = { 0.0, 1.0, 0.0 };
    ray.direction = { -1.0, 1.0, 0.0 };
    output = query(line, ray);
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

    // parallel but not coincident
    line.origin = { 0.0, 0.0, 0.0 };
    line.direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    ray.origin = { 2.0, 1.0, 0.0 };
    ray.direction = line.direction;
    output = query(line, ray);
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

    // line and ray are coincident
    line.origin = { 0.0, 0.0, 0.0 };
    line.direction = { 3.0 / 5.0, 4.0 / 5.0, 0.0 };
    ray.origin = { 3.0, 4.0, 0.0 };
    ray.direction = { 3.0 / 2.0, 4.0 / 2.0, 0.0 };
    output = query(line, ray);
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

    // no intersection, not parallel, ray origin not closest
    line.origin = { 0.0, 0.0, 0.0 };
    line.direction = { 3.0, 4.0, 5.0 };
    ray.origin = { 1.0, 1.0, 1.0 };
    ray.direction = { -1.0, 0.0, 2.0 };
    output = query(line, ray);
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

    // no intersection, not parallel, ray origin closest
    line.origin = { 0.0, 0.0, 0.0 };
    line.direction = { 3.0, 4.0, 5.0 };
    ray.origin = { -1.0, 0.0, 0.0 };
    ray.direction = { -1.0, 0.0, 2.0 };
    output = query(line, ray);
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
#include <GTL/Mathematics/Distance/ND/DistLineRay.h>

namespace gtl
{
    template class DCPQuery<float, Line<float, 2>, Ray<float, 2>>;
    template class DCPQuery<double, Line<double, 3>, Ray<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Line<Rational, 4>, Ray<Rational, 4>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistLineRay)
