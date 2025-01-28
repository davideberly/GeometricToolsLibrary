#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrLine3OrientedBox3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrLine3OrientedBox3
    {
    public:
        UnitTestIntrLine3OrientedBox3();

    private:
        void Test();
    };
}

UnitTestIntrLine3OrientedBox3::UnitTestIntrLine3OrientedBox3()
{
    UTInformation("Mathematics/Intersection/3D/IntrLine3OrientedBox3");

    Test();
}

void UnitTestIntrLine3OrientedBox3::Test()
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

    Line3<double> line{};
    line.origin = { -0.1, -0.2, -0.3 };
    line.direction = { 1.0, 1.0, 1.0 };
    Normalize(line.direction);

    FIQuery<double, Line3<double>, OrientedBox3<double>> fiQuery{};
    TIQuery<double, Line3<double>, OrientedBox3<double>> tiQuery{};

    auto fiOutput = fiQuery(line, box);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2,
        "Invalid FIQuery.");

    // The fiOutput.point[] values were computed using the line and
    // line t-parameters. Extract the box coordinates to verify that
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

    auto tiOutput = tiQuery(line, box);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // no intersection
    line.direction = box.axis[0];
    line.origin = box.center + 2.0 * box.extent[1] * box.axis[1] + 3.0 * box.extent[2] * box.axis[2];
    fiOutput = fiQuery(line, box);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, box);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");

    // tangential intersection
    line.origin = box.center + box.extent[1] * box.axis[1] + 0.5 * box.extent[2] * box.axis[2];
    fiOutput = fiQuery(line, box);
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
    error = Length(P - Vector<double, 3>{-1.0, 1.0, 0.5});
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
    error = Length(P - Vector<double, 3>{0.0, 1.0, 0.5});
    UTAssert(
        error <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, box);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrLine3OrientedBox3.h>

namespace gtl
{
    template class TIQuery<float, Line3<float>, OrientedBox3<float>>;
    template class FIQuery<float, Line3<float>, OrientedBox3<float>>;

    template class TIQuery<double, Line3<double>, OrientedBox3<double>>;
    template class FIQuery<double, Line3<double>, OrientedBox3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Line3<Rational>, OrientedBox3<Rational>>;
    template class FIQuery<Rational, Line3<Rational>, OrientedBox3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrLine3OrientedBox3)
