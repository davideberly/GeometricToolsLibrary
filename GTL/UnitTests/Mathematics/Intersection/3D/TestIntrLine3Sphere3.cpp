#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrLine3Sphere3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrLine3Sphere3
    {
    public:
        UnitTestIntrLine3Sphere3();

    private:
        void Test();
    };
}

UnitTestIntrLine3Sphere3::UnitTestIntrLine3Sphere3()
{
    UTInformation("Mathematics/Intersection/3D/IntrLine3Sphere3");

    Test();
}

void UnitTestIntrLine3Sphere3::Test()
{
    Sphere3<double> sphere{};
    sphere.center = { 0.5, 0.5, 0.5 };
    sphere.radius = 0.5;

    Line3<double> line{};
    line.origin = { -0.1, -0.2, -0.3 };
    line.direction = { 1.0, 1.0, 1.0 };
    Normalize(line.direction);

    FIQuery<double, Line3<double>, Sphere3<double>> fiQuery{};
    TIQuery<double, Line3<double>, Sphere3<double>> tiQuery{};

    auto fiOutput = fiQuery(line, sphere);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2,
        "Invalid FIQuery.");

    // The fiOutput.point[] values are computed from the line and the
    // line parameters. Verify that the points are on the sphere.
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

    auto tiOutput = tiQuery(line, sphere);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // no intersection
    line.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(line, sphere);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, sphere);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");

    // tangential intersection
    line.origin = { 0.0, 0.0, 0.5 };
    line.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(line, sphere);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 1,
        "Invalid FIQuery.");
    diff = fiOutput.point[0] - Vector3<double>{ 0.5, 0.0, 0.5 };
    error = Length(diff);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, sphere);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrLine3Sphere3.h>

namespace gtl
{
    template class TIQuery<float, Line3<float>, Sphere3<float>>;
    template class FIQuery<float, Line3<float>, Sphere3<float>>;

    template class TIQuery<double, Line3<double>, Sphere3<double>>;
    template class FIQuery<double, Line3<double>, Sphere3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Line3<Rational>, Sphere3<Rational>>;
    template class FIQuery<Rational, Line3<Rational>, Sphere3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrLine3Sphere3)
