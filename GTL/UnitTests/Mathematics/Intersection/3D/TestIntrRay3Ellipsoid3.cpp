#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrRay3Ellipsoid3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrRay3Ellipsoid3
    {
    public:
        UnitTestIntrRay3Ellipsoid3();

    private:
        void Test();
    };
}

UnitTestIntrRay3Ellipsoid3::UnitTestIntrRay3Ellipsoid3()
{
    UTInformation("Mathematics/Intersection/3D/IntrRay3Ellipsoid3");

    Test();
}

void UnitTestIntrRay3Ellipsoid3::Test()
{
    Ellipsoid3<double> ellipsoid{};
    ellipsoid.center = { 0.5, 0.5, 0.5 };
    ellipsoid.axis[0] = { 1.0, 0.0, 0.0 };
    ellipsoid.axis[1] = { 0.0, 1.0, 0.0 };
    ellipsoid.axis[2] = { 0.0, 0.0, 1.0 };
    ellipsoid.extent = { 0.5, 0.51, 0.52 };

    Ray3<double> ray{};
    ray.origin = { -0.1, -0.2, -0.3 };
    ray.direction = { 1.0, 1.0, 1.0 };
    Normalize(ray.direction);

    FIQuery<double, Ray3<double>, Ellipsoid3<double>> fiQuery{};
    TIQuery<double, Ray3<double>, Ellipsoid3<double>> tiQuery{};

    auto fiOutput = fiQuery(ray, ellipsoid);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2,
        "Invalid FIQuery.");

    // The fiOutput.point[] values are computed from the ray and the
    // ray parameters. Verify that the points are on the ellipsoid.
    Matrix3x3<double> M;
    ellipsoid.GetM(M);
    Vector3<double> diff = fiOutput.point[0] - ellipsoid.center;
    double error = std::fabs(Dot(diff, M * diff) - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    diff = fiOutput.point[1] - ellipsoid.center;
    error = std::fabs(Dot(diff, M * diff) - 1.0);
    UTAssert(
        error <= 1e-14,
        "Invalid FIQuery.");

    auto tiOutput = tiQuery(ray, ellipsoid);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // The ray origin is inside the ellipsoid.
    ray.origin = 0.5 * (fiOutput.point[0] + fiOutput.point[1]);
    fiOutput = fiQuery(ray, ellipsoid);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        ray.origin == fiOutput.point[0],
        "Invalid FIQuery.");

    // The intersection point[1] is on the ellipsoid.
    diff = fiOutput.point[1] - ellipsoid.center;
    error = std::fabs(Dot(diff, M * diff) - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(ray, ellipsoid);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // no intersection
    ray.origin = { 1.1, 1.2, 1.3 };
    fiOutput = fiQuery(ray, ellipsoid);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(ray, ellipsoid);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");

    // tangential intersection
    ellipsoid.center = { 0.0, 0.0, 0.0 };
    ellipsoid.extent = { 1.0, 2.0, 4.0 };
    ray.origin = { 1.0, -1.0, 0.0 };
    ray.direction = { 0.0, 1.0, 0.0 };
    fiOutput = fiQuery(ray, ellipsoid);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 1,
        "Invalid FIQuery.");
    diff = fiOutput.point[0] - Vector3<double>{ 1.0, 0.0, 0.0 };
    error = Length(diff);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(ray, ellipsoid);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrRay3Ellipsoid3.h>

namespace gtl
{
    template class TIQuery<float, Ray3<float>, Ellipsoid3<float>>;
    template class FIQuery<float, Ray3<float>, Ellipsoid3<float>>;

    template class TIQuery<double, Ray3<double>, Ellipsoid3<double>>;
    template class FIQuery<double, Ray3<double>, Ellipsoid3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Ray3<Rational>, Ellipsoid3<Rational>>;
    template class FIQuery<Rational, Ray3<Rational>, Ellipsoid3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrRay3Ellipsoid3)
