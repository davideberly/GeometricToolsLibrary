#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/2D/DistRay2OrientedBox2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistRay2OrientedBox2
    {
    public:
        UnitTestDistRay2OrientedBox2();

    private:
        using RBQuery = DCPQuery<double, Ray2<double>, OrientedBox2<double>>;
        using RBOutput = RBQuery::Output;

        void Test();

        // The expected values are passed to this function.
        void Validate(RBOutput const& output,
            double sqrDistance,
            double parameter,
            Vector2<double> const& closest0,
            Vector2<double> const& closest1);
    };
}

UnitTestDistRay2OrientedBox2::UnitTestDistRay2OrientedBox2()
{
    UTInformation("Mathematics/Distance/2D/DistRay2OrientedBox2");

    Test();
}

void UnitTestDistRay2OrientedBox2::Validate(RBOutput const& output,
    double sqrDistance,
    double parameter,
    Vector2<double> const& closest0,
    Vector2<double> const& closest1)
{
    double constexpr maxError = 1e-14;

    double error = std::fabs(output.sqrDistance - sqrDistance);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(sqrDistance));
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.parameter - parameter);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - closest0);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = Length(output.closest[1] - closest1);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
}

void UnitTestDistRay2OrientedBox2::Test()
{
    RBQuery query{};
    RBOutput output{};
    Ray2<double> ray{};
    OrientedBox2<double> box{};

    box.center = { 264.0, 232.0 };
    double const boxAngle = C_PI<double> / 6.0;
    box.axis[0] = { std::cos(boxAngle), std::sin(boxAngle) };
    box.axis[1] = { -std::sin(boxAngle), std::cos(boxAngle) };
    box.extent = { 64.0, 32.0 };

    // ray interior is closest
    ray.origin = { 257.0, 256.0 };
    ray.direction = { 1.0, 0.0 };
    output = query(ray, box);
    Validate(output, 0.0, 67.044427995721080,
        { 324.04442799572104, 256.0 },
        { 324.04442799572104, 256.0 });

    // ray origin is closest
    ray.origin = { 256.0, 163.0 };
    ray.direction = { 1.0, 0.0 };
    output = query(ray, box);
    Validate(output, 564.33579399890891, 0.0,
        { 256.0, 163.0 },
        { 244.12212356943687, 183.57308546376024 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/2D/DistRay2OrientedBox2.h>

namespace gtl
{
    template class DCPQuery<float, Ray<float, 2>, OrientedBox<float, 2>>;
    template class DCPQuery<double, Ray<double, 2>, OrientedBox<double, 2>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Ray<Rational, 2>, OrientedBox<Rational, 2>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistRay2OrientedBox2)
