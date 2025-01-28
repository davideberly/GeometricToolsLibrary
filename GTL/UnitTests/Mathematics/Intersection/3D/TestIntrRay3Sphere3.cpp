#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrRay3Sphere3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrRay3Sphere3
    {
    public:
        UnitTestIntrRay3Sphere3();

    private:
        void Test();
    };
}

UnitTestIntrRay3Sphere3::UnitTestIntrRay3Sphere3()
{
    UTInformation("Mathematics/Intersection/3D/IntrRay3Sphere3");

    Test();
}

void UnitTestIntrRay3Sphere3::Test()
{
    Sphere3<double> sphere{};
    sphere.center = { 0.5, 0.5, 0.5 };
    sphere.radius = 0.5;

    Ray3<double> ray{};
    ray.origin = { -0.1, -0.2, -0.3 };
    ray.direction = { 1.0, 1.0, 1.0 };
    Normalize(ray.direction);

    FIQuery<double, Ray3<double>, Sphere3<double>> fiQuery{};
    TIQuery<double, Ray3<double>, Sphere3<double>> tiQuery{};

    auto fiOutput = fiQuery(ray, sphere);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2,
        "Invalid FIQuery.");

    // The fiOutput.point[] values are computed from the ray and the
    // ray parameters. Verify that the points are on the sphere.
    Vector3<double> diff = fiOutput.point[0] - sphere.center;
    double radius = Length(diff);
    double error = std::fabs(radius - sphere.radius);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    diff = fiOutput.point[1] - sphere.center;
    radius = Length(diff);
    error = std::fabs(radius - sphere.radius);
    UTAssert(
        error <= 1e-14,
        "Invalid FIQuery.");

    auto tiOutput = tiQuery(ray, sphere);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // The ray origin is inside the sphere.
    ray.origin = 0.5 * (fiOutput.point[0] + fiOutput.point[1]);
    fiOutput = fiQuery(ray, sphere);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        ray.origin == fiOutput.point[0],
        "Invalid FIQuery.");

    // The intersection point[1] is on the sphere.
    diff = fiOutput.point[1] - sphere.center;
    radius = Length(diff);
    error = std::fabs(radius - sphere.radius);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(ray, sphere);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // no intersection
    ray.origin = { 1.1, 1.2, 1.3 };
    fiOutput = fiQuery(ray, sphere);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(ray, sphere);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");

    // tangential intersection
    ray.origin = { 0.0, 0.0, 0.5 };
    ray.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(ray, sphere);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 1,
        "Invalid FIQuery.");
    diff = fiOutput.point[0] - Vector3<double>{ 0.5, 0.0, 0.5 };
    error = Length(diff);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(ray, sphere);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    ray.origin = { 0.6, 0.0, 0.5 };
    ray.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(ray, sphere);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(ray, sphere);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrRay3Sphere3.h>

namespace gtl
{
    template class TIQuery<float, Ray3<float>, Sphere3<float>>;
    template class FIQuery<float, Ray3<float>, Sphere3<float>>;

    template class TIQuery<double, Ray3<double>, Sphere3<double>>;
    template class FIQuery<double, Ray3<double>, Sphere3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Ray3<Rational>, Sphere3<Rational>>;
    template class FIQuery<Rational, Ray3<Rational>, Sphere3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrRay3Sphere3)
