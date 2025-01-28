#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrRay3Cylinder3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrRay3Cylinder3
    {
    public:
        UnitTestIntrRay3Cylinder3();

    private:
        void Test();
    };
}

UnitTestIntrRay3Cylinder3::UnitTestIntrRay3Cylinder3()
{
    UTInformation("Mathematics/Intersection/3D/IntrRay3Cylinder3");

    Test();
}

void UnitTestIntrRay3Cylinder3::Test()
{
    // x^2 + y^2 - r^2 = 1, |z| <= h/2 = 1
    Cylinder3<double> cylinder{};
    cylinder.center = { 0.0, 0.0, 0.0 };
    cylinder.direction = { 0.0, 0.0, 1.0 };
    cylinder.radius = 1.0;
    cylinder.height = 2.0;

    Ray3<double> ray{};
    FIQuery<double, Ray3<double>, Cylinder3<double>> fiQuery{};

    // Ray in same direction as cylinder axis, inside cylinder.
    ray.origin = { 0.5, 0.5, 0.0 };
    ray.direction = { 0.0, 0.0, 1.0 };
    auto fiOutput = fiQuery(ray, cylinder);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        fiOutput.parameter[0] == 0.0 &&
        fiOutput.parameter[1] == 1.0 &&
        (fiOutput.point[0] == Vector3<double>{ 0.5, 0.5, 0.0 }) &&
        (fiOutput.point[1] == Vector3<double>{ 0.5, 0.5, 1.0 }),
        "Invalid FIQuery.");

    // Ray perpendicular to cylinder axis, intersects cylinder.
    ray.origin = { 0.25, 0.5, 0.25 };
    ray.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(ray, cylinder);
    double error0 = std::fabs(fiOutput.parameter[0]);
    double error1 = std::fabs(fiOutput.parameter[1] - 0.61602540378443860);
    double error2 = Length(fiOutput.point[0] - ray.origin);
    double error3 = Length(fiOutput.point[1] - Vector3<double>{std::sqrt(0.75), 0.5, 0.25});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    // Ray perpendicular to cylinder axis, above top disk of cylinder.
    ray.origin = { 2.0, 0.5, 2.0 };
    ray.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(ray, cylinder);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    // Line intersects cylinder but not ray.
    ray.origin = { 2.0, 0.0, 0.0 };
    ray.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(ray, cylinder);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrRay3Cylinder3.h>

namespace gtl
{
    template class FIQuery<float, Ray3<float>, Cylinder3<float>>;
    template class FIQuery<double, Ray3<double>, Cylinder3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class FIQuery<Rational, Ray3<Rational>, Cylinder3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrRay3Cylinder3)
