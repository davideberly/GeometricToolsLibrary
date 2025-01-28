#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/2D/DistRay2AlignedBox2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistRay2AlignedBox2
    {
    public:
        UnitTestDistRay2AlignedBox2();

    private:
        using RBQuery = DCPQuery<double, Ray2<double>, AlignedBox2<double>>;
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

UnitTestDistRay2AlignedBox2::UnitTestDistRay2AlignedBox2()
{
    UTInformation("Mathematics/Distance/2D/DistRay2AlignedBox2");

    Test();
}

void UnitTestDistRay2AlignedBox2::Validate(RBOutput const& output,
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

void UnitTestDistRay2AlignedBox2::Test()
{
    RBQuery query{};
    RBOutput output{};
    Ray2<double> ray{};
    AlignedBox2<double> box{};

    box.min = { 200.0, 200.0 };
    box.max = { 328.0, 264.0 };

    // ray interior is closest
    ray.origin = { 131.0, 257.0 };
    ray.direction = { 0.89100652418836779, 0.45399049973954703 };
    output = query(ray, box);
    Validate(output, 629.42273731583327, 64.657383667174201,
        { 188.61015068440264, 286.35383792291202 },
        { 200.0, 264.0 });

    // ray origin is closest
    ray.origin = { 256.0, 297.0 };
    ray.direction = { 0.97814760073380569, 0.20791169081775931 };
    output = query(ray, box);
    Validate(output, 1089.0, 0.0,
        { 256.0, 297.0 },
        { 256.0, 264.0 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/2D/DistRay2AlignedBox2.h>

namespace gtl
{
    template class DCPQuery<float, Ray<float, 2>, AlignedBox<float, 2>>;
    template class DCPQuery<double, Ray<double, 2>, AlignedBox<double, 2>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Ray<Rational, 2>, AlignedBox<Rational, 2>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistRay2AlignedBox2)
