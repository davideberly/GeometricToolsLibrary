#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrSegment3OrientedBox3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrSegment3OrientedBox3
    {
    public:
        UnitTestIntrSegment3OrientedBox3();

    private:
        void Test();
    };
}

UnitTestIntrSegment3OrientedBox3::UnitTestIntrSegment3OrientedBox3()
{
    UTInformation("Mathematics/Intersection/3D/IntrSegment3OrientedBox3");

    Test();
}

void UnitTestIntrSegment3OrientedBox3::Test()
{
    Quaternion<double> q{ 1.0, 2.0, 3.0, 4.0 };
    Normalize(q);
    Matrix<double, 3, 3> orient{};
    RigidMotion<double>::Convert(q, orient);

    OrientedBox3<double> box{};
    box.axis[0] = orient.GetCol(0);
    box.axis[1] = orient.GetCol(1);
    box.axis[2] = orient.GetCol(2);
    box.center = { 0.5, 0.5, 0.5 };
    box.extent = { 0.5, 0.5, 0.5 };

    Segment3<double> segment{};
    segment.p[0] = { -0.1, -0.2, -0.3 };
    segment.p[1] = segment.p[0] + Vector3<double>{ 2.0, 2.0, 2.0 };

    FIQuery<double, Segment3<double>, OrientedBox3<double>> fiQuery{};
    TIQuery<double, Segment3<double>, OrientedBox3<double>> tiQuery{};

    auto fiOutput = fiQuery(segment, box);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2,
        "Invalid FIQuery.");

    // The fiOutput.point[] values were computed using the segment and
    // segment t-parameters. Extract the box coordinates to verify that
    // the intersections are on box faces. At least one of the
    // P-components must be +1 or -1.
    Vector<double, 3> diff = fiOutput.point[0] - box.center;
    Vector<double, 3> P
    {
        Dot(box.axis[0], diff) / box.extent[0],
        Dot(box.axis[1], diff) / box.extent[1],
        Dot(box.axis[2], diff) / box.extent[2]
    };
    // The intersection is on the -z box face.
    double error = std::fabs(P[2] + 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    diff = fiOutput.point[1] - box.center;
    P =
    {
        Dot(box.axis[0], diff) / box.extent[0],
        Dot(box.axis[1], diff) / box.extent[1],
        Dot(box.axis[2], diff) / box.extent[2]
    };
    // The intersection is on the +z box face.
    error = std::fabs(P[2] - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    auto tiOutput = tiQuery(segment, box);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // no intersection
    segment.p[0] = box.center + 2.0 * box.extent[1] * box.axis[1] + 3.0 * box.extent[2] * box.axis[2];
    segment.p[1] = segment.p[0] + box.axis[0];
    fiOutput = fiQuery(segment, box);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(segment, box);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");

    // tangential intersection
    box.axis[0] = { 1.0, 0.0, 0.0 };
    box.axis[1] = { 0.0, 1.0, 0.0 };
    box.axis[2] = { 0.0, 0.0, 1.0 };
    box.center = { 0.5, 0.5, 0.5 };
    box.extent = { 0.5, 0.5, 0.5 };
    segment.p[0] = box.center + box.extent[1] * box.axis[1] + 0.5 * box.extent[2] * box.axis[2];
    segment.p[1] = segment.p[0] + 8.0 * box.axis[0];
    fiOutput = fiQuery(segment, box);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2,
        "Invalid FIQuery.");

    diff = fiOutput.point[0] - box.center;
    P =
    {
        Dot(box.axis[0], diff) / box.extent[0],
        Dot(box.axis[1], diff) / box.extent[1],
        Dot(box.axis[2], diff) / box.extent[2]
    };
    error = Length(P - Vector<double, 3>{0.0, 1.0, 0.5});
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    diff = fiOutput.point[1] - box.center;
    P =
    {
        Dot(box.axis[0], diff) / box.extent[0],
        Dot(box.axis[1], diff) / box.extent[1],
        Dot(box.axis[2], diff) / box.extent[2]
    };
    error = Length(P - Vector<double, 3>{1.0, 1.0, 0.5});
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
#include <GTL/Mathematics/Intersection/3D/IntrSegment3OrientedBox3.h>

namespace gtl
{
    template class TIQuery<float, Segment3<float>, OrientedBox3<float>>;
    template class FIQuery<float, Segment3<float>, OrientedBox3<float>>;

    template class TIQuery<double, Segment3<double>, OrientedBox3<double>>;
    template class FIQuery<double, Segment3<double>, OrientedBox3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Segment3<Rational>, OrientedBox3<Rational>>;
    template class FIQuery<Rational, Segment3<Rational>, OrientedBox3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrSegment3OrientedBox3)
