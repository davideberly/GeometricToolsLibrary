#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrRay3AlignedBox3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrRay3AlignedBox3
    {
    public:
        UnitTestIntrRay3AlignedBox3();

    private:
        void Test();
    };
}

UnitTestIntrRay3AlignedBox3::UnitTestIntrRay3AlignedBox3()
{
    UTInformation("Mathematics/Intersection/3D/IntrRay3AlignedBox3");

    Test();
}

void UnitTestIntrRay3AlignedBox3::Test()
{
    Ray3<double> ray{};
    AlignedBox3<double> box{};
    box.min = { 0.0, 0.0, 0.0 };
    box.max = { 1.0, 1.0, 1.0 };

    ray.origin = { -0.1, -0.2, -0.3 };
    ray.direction = { 1.0, 1.0, 1.0 };
    Normalize(ray.direction);

    FIQuery<double, Ray3<double>, AlignedBox3<double>> fiQuery{};
    TIQuery<double, Ray3<double>, AlignedBox3<double>> tiQuery{};

    // 2 intersections
    auto fiOutput = fiQuery(ray, box);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2,
        "Invalid FIQuery.");
    Vector3<double> diff = fiOutput.point[0] - Vector3<double>{0.2, 0.1, 0.0};
    double error = Length(diff);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");
    diff = fiOutput.point[1] - Vector3<double>{1.0, 0.9, 0.8};
    error = Length(diff);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    auto tiOutput = tiQuery(ray, box);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    ray.origin = 0.5 * (fiOutput.point[0] + fiOutput.point[1]);
    fiOutput = fiQuery(ray, box);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2,
        "Invalid FIQuery.");
    diff = fiOutput.point[0] - Vector3<double>{0.6, 0.5, 0.4};
    error = Length(diff);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");
    diff = fiOutput.point[1] - Vector3<double>{1.0, 0.9, 0.8};
    error = Length(diff);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(ray, box);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // no intersection
    ray.origin = { -0.1, -0.2, -0.3 };
    ray.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(ray, box);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(ray, box);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");

    ray.origin = { 2.0, 0.5, 0.5 };
    fiOutput = fiQuery(ray, box);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(ray, box);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");

    // tangential intersection
    ray.origin = { -1.0, 0.0, 0.3 };
    fiOutput = fiQuery(ray, box);
    
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        (fiOutput.point[0] == Vector3<double>{0.0, 0.0, 0.3}) &&
        (fiOutput.point[1] == Vector3<double>{1.0, 0.0, 0.3}),
        "Invalid FIQuery.");

    tiOutput = tiQuery(ray, box);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // 1 intersection
    ray.origin = box.min;
    ray.direction = { -1.0, 0.0, 0.0 };
    fiOutput = fiQuery(ray, box);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 1,
        "Invalid FIQuery.");
    diff = fiOutput.point[0] - box.min;
    error = Length(diff);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(ray, box);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    box.max = { 2.0, 1.0, 1.5 };
    ray.origin = { 4.0, 0.5, 0.0 };
    ray.direction = { -4.0, 0.0, 3.0 };
    fiOutput = fiQuery(ray, box);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 1,
        "Invalid FIQuery.");
    diff = fiOutput.point[0] - Vector3<double>{ 2.0, 0.5, 1.5 };
    error = Length(diff);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(ray, box);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrRay3AlignedBox3.h>

namespace gtl
{
    template class TIQuery<float, Ray3<float>, AlignedBox3<float>>;
    template class FIQuery<float, Ray3<float>, AlignedBox3<float>>;

    template class TIQuery<double, Ray3<double>, AlignedBox3<double>>;
    template class FIQuery<double, Ray3<double>, AlignedBox3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Ray3<Rational>, AlignedBox3<Rational>>;
    template class FIQuery<Rational, Ray3<Rational>, AlignedBox3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrRay3AlignedBox3)
