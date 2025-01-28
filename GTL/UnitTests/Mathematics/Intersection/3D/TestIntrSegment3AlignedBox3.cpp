#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrSegment3AlignedBox3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrSegment3AlignedBox3
    {
    public:
        UnitTestIntrSegment3AlignedBox3();

    private:
        void Test();
    };
}

UnitTestIntrSegment3AlignedBox3::UnitTestIntrSegment3AlignedBox3()
{
    UTInformation("Mathematics/Intersection/3D/IntrSegment3AlignedBox3");

    Test();
}

void UnitTestIntrSegment3AlignedBox3::Test()
{
    Segment3<double> segment{};
    AlignedBox3<double> box{};
    box.min = { 0.0, 0.0, 0.0 };
    box.max = { 1.0, 1.0, 1.0 };

    segment.p[0] = { -0.1, -0.2, -0.3 };
    segment.p[1] = segment.p[0] + Vector3<double>{ 2.0, 2.0, 2.0 };

    FIQuery<double, Segment3<double>, AlignedBox3<double>> fiQuery{};
    TIQuery<double, Segment3<double>, AlignedBox3<double>> tiQuery{};

    // 2 intersections
    auto fiOutput = fiQuery(segment, box);
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

    auto tiOutput = tiQuery(segment, box);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    segment.p[0] = 0.5 * (fiOutput.point[0] + fiOutput.point[1]);
    fiOutput = fiQuery(segment, box);
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

    tiOutput = tiQuery(segment, box);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // no intersection
    segment.p[0] = { -0.1, -0.2, -0.3 };
    segment.p[1] = segment.p[0] + Vector3<double>{ 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(segment, box);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(segment, box);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");

    segment.p[0] = { 2.0, 0.5, 0.5 };
    segment.p[1] = segment.p[0] + Vector3<double>{ 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(segment, box);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(segment, box);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");

    segment.p[1] = { -1.0, 0.5, 0.5 };
    segment.p[0] = segment.p[1] - Vector3<double>{ 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(segment, box);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(segment, box);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");

    // tangential intersection
    segment.p[0] = { -1.0, 0.0, 0.3 };
    segment.p[1] = segment.p[0] + Vector3<double>{1.5, 0.0, 0.0};
    fiOutput = fiQuery(segment, box);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        (fiOutput.point[0] == Vector3<double>{0.0, 0.0, 0.3}) &&
        (fiOutput.point[1] == Vector3<double>{0.5, 0.0, 0.3}),
        "Invalid FIQuery.");

    tiOutput = tiQuery(segment, box);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // 1 intersection
    segment.p[0] = box.min;
    segment.p[1] = segment.p[0] + Vector3<double>{ -1.0, 0.0, 0.0 };
    fiOutput = fiQuery(segment, box);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 1,
        "Invalid FIQuery.");
    diff = fiOutput.point[0] - box.min;
    error = Length(diff);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(segment, box);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    box.max = { 2.0, 1.0, 1.5 };
    segment.p[0] = { 4.0, 0.5, 0.0 };
    segment.p[1] = { 0.0, 0.5, 3.0 };
    fiOutput = fiQuery(segment, box);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 1,
        "Invalid FIQuery.");
    diff = fiOutput.point[0] - Vector3<double>{ 2.0, 0.5, 1.5 };
    error = Length(diff);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(segment, box);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrSegment3AlignedBox3.h>

namespace gtl
{
    template class TIQuery<float, Segment3<float>, AlignedBox3<float>>;
    template class FIQuery<float, Segment3<float>, AlignedBox3<float>>;

    template class TIQuery<double, Segment3<double>, AlignedBox3<double>>;
    template class FIQuery<double, Segment3<double>, AlignedBox3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Segment3<Rational>, AlignedBox3<Rational>>;
    template class FIQuery<Rational, Segment3<Rational>, AlignedBox3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrSegment3AlignedBox3)
