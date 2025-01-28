#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrRay3Capsule3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrRay3Capsule3
    {
    public:
        UnitTestIntrRay3Capsule3();

    private:
        void Test();
    };
}

UnitTestIntrRay3Capsule3::UnitTestIntrRay3Capsule3()
{
    UTInformation("Mathematics/Intersection/3D/IntrRay3Capsule3");

    Test();
}

void UnitTestIntrRay3Capsule3::Test()
{
    Capsule3<double> capsule{};
    capsule.segment.SetCenteredForm({ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 }, 1.0);
    capsule.radius = 1.0;

    Ray3<double> ray{};
    TIQuery<double, Ray3<double>, Capsule3<double>> tiQuery{};
    FIQuery<double, Ray3<double>, Capsule3<double>> fiQuery{};

    // Ray in same direction as capsule axis, inside capsule.
    ray.origin = { 0.5, 0.5, 0.0 };
    ray.direction = { 0.0, 0.0, 1.0 };
    auto fiOutput = fiQuery(ray, capsule);
    double error0 = std::fabs(fiOutput.parameter[0]);
    double error1 = std::fabs(fiOutput.parameter[1] - 1.7071067811865475);
    double error2 = Length(fiOutput.point[0] - Vector3<double>{ 0.5, 0.5, 0.0});
    double error3 = Length(fiOutput.point[1] - Vector3<double>{ 0.5, 0.5, 1.7071067811865475});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    auto tiOutput = tiQuery(ray, capsule);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // Ray perpendicular to capsule axis, intersects capsule.
    ray.origin = { 0.25, 0.5, 0.25 };
    ray.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(ray, capsule);
    error0 = std::fabs(fiOutput.parameter[0]);
    error1 = std::fabs(fiOutput.parameter[1] - 0.61602540378443860);
    error2 = Length(fiOutput.point[0] - ray.origin);
    error3 = Length(fiOutput.point[1] - Vector3<double>{0.86602540378443860, 0.5, 0.25});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(ray, capsule);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // Ray perpendicular to capsule axis, above top disk of capsule.
    ray.origin = { 2.0, 0.5, 2.0 };
    ray.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(ray, capsule);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    // Line intersects capsule but not ray.
    ray.origin = { 2.0, 0.0, 0.0 };
    ray.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(ray, capsule);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(ray, capsule);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrRay3Capsule3.h>

namespace gtl
{
    template class TIQuery<float, Ray3<float>, Capsule3<float>>;
    template class FIQuery<float, Ray3<float>, Capsule3<float>>;

    template class TIQuery<double, Ray3<double>, Capsule3<double>>;
    template class FIQuery<double, Ray3<double>, Capsule3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Ray3<Rational>, Capsule3<Rational>>;
    template class FIQuery<Rational, Ray3<Rational>, Capsule3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrRay3Capsule3)
