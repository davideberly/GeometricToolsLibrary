#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistRay3Rectangle3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistRay3Rectangle3
    {
    public:
        UnitTestDistRay3Rectangle3();

    private:
        using RRQuery = DCPQuery<double, Ray<double, 3>, Rectangle<double, 3>>;
        using RROutput = RRQuery::Output;

        void Test();

        // The expected values are passed to this function.
        void Validate(RROutput const& output,
            double sqrDistance,
            double parameter,
            Vector<double, 2> const& rectangle,
            Vector<double, 3> const& closest0,
            Vector<double, 3> const& closest1);
    };
}

UnitTestDistRay3Rectangle3::UnitTestDistRay3Rectangle3()
{
    UTInformation("Mathematics/Distance/3D/DistRay3Rectangle3");

    Test();
}

void UnitTestDistRay3Rectangle3::Validate(RROutput const& output,
    double sqrDistance,
    double parameter,
    Vector<double, 2> const& cartesian,
    Vector<double, 3> const& closest0,
    Vector<double, 3> const& closest1)
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
    error = std::fabs(output.cartesian[0] - cartesian[0]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] - cartesian[1]);
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

void UnitTestDistRay3Rectangle3::Test()
{
    RRQuery query{};
    RROutput output{};
    Ray<double, 3> ray{};
    Rectangle<double, 3> rectangle{};

    Vector<double, 3> translate{ 0.1234, 5.6789, -1.9735 };
    Matrix3x3<double> rotate{};
    Quaternion<double> q(1.0, 2.0, 3.0, 4.0);
    Normalize(q);
    RigidMotion<double>::Convert(q, rotate);

    // line intersects rectangle, ray origin closest
    ray.origin = { 0.0, 0.0, 0.25 };
    ray.direction = { -0.0625, -0.0125, 1.0 };
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    output = query(ray, rectangle);
    Validate(output, 0.0625, 0.0,
        { 0.0, 0.0 },
        { 0.0, 0.0, 0.25 },
        { 0.0, 0.0, 0.0 });

    // line intersects rectangle, ray origin closest
    // arbitrary rigid motion first
    ray.origin = { 0.0, 0.0, 0.25 };
    ray.direction = { -0.0625, -0.0125, 1.0 };
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    ray.origin = rotate * ray.origin + translate;
    ray.direction = rotate * ray.direction;
    rectangle.center = rotate * rectangle.center + translate;
    rectangle.axis[0] = rotate * rectangle.axis[0];
    rectangle.axis[1] = rotate * rectangle.axis[1];
    output = query(ray, rectangle);
    Validate(output, 0.0625, 0.0,
        { 0.0, 0.0 },
        rotate* Vector<double, 3>{ 0.0, 0.0, 0.25 } + translate,
        rotate* Vector<double, 3>{ 0.0, 0.0, 0.0 } + translate);

    // line does not intersect rectangle, ray origin closest
    ray.origin = { 2.5, 0.5, 1.0 };
    ray.direction = { 0.1, 0.2, -0.01 };
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    output = query(ray, rectangle);
    Validate(output, 1.25, 0.0,
        { 2.0, 0.5 },
        { 2.5, 0.5, 1.0 },
        { 2.0, 0.5, 0.0 });

    // line does not intersect rectangle, ray origin closest
    // arbitrary rigid motion first
    ray.origin = { 2.5, 0.5, 1.0 };
    ray.direction = { 0.1, 0.2, -0.01 };
    rectangle.center = { 0.0, 0.0, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 2.0, 1.0 };
    ray.origin = rotate * ray.origin + translate;
    ray.direction = rotate * ray.direction;
    rectangle.center = rotate * rectangle.center + translate;
    rectangle.axis[0] = rotate * rectangle.axis[0];
    rectangle.axis[1] = rotate * rectangle.axis[1];
    output = query(ray, rectangle);
    Validate(output, 1.25, 0.0,
        { 2.0, 0.5 },
        rotate* Vector<double, 3>{ 2.5, 0.5, 1.0 } + translate,
        rotate* Vector<double, 3>{ 2.0, 0.5, 0.0 } + translate);
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistRay3Rectangle3.h>

namespace gtl
{
    template class DCPQuery<float, Ray<float, 3>, Rectangle<float, 3>>;
    template class DCPQuery<double, Ray<double, 3>, Rectangle<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Ray<Rational, 3>, Rectangle<Rational, 3>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistRay3Rectangle3)
