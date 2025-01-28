#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrLine3Capsule3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrLine3Capsule3
    {
    public:
        UnitTestIntrLine3Capsule3();

    private:
        void Test();
    };
}

UnitTestIntrLine3Capsule3::UnitTestIntrLine3Capsule3()
{
    UTInformation("Mathematics/Intersection/3D/IntrLine3Capsule3");

    Test();
}

void UnitTestIntrLine3Capsule3::Test()
{
    Capsule3<double> capsule{};
    capsule.segment.SetCenteredForm({ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 }, 1.0);
    capsule.radius = 1.0;

    Line3<double> line{};
    TIQuery<double, Line3<double>, Capsule3<double>> tiQuery{};
    FIQuery<double, Line3<double>, Capsule3<double>> fiQuery{};

    // Line in same direction as capsule axis, inside capsule.
    line.origin = { 0.5, 0.5, 0.0 };
    line.direction = { 0.0, 0.0, 1.0 };
    auto fiOutput = fiQuery(line, capsule);
    double temp = 1.0 + std::sqrt(0.5);
    double error0 = std::fabs(fiOutput.parameter[0] + temp);
    double error1 = std::fabs(fiOutput.parameter[1] - temp);
    double error2 = Length(fiOutput.point[0] - Vector3<double>{ 0.5, 0.5, -temp});
    double error3 = Length(fiOutput.point[1] - Vector3<double>{ 0.5, 0.5, temp});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    auto tiOutput = tiQuery(line, capsule);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // Line in opposite direction as capsule axis, inside capsule.
    line.origin = { 0.5, 0.5, 0.0 };
    line.direction = { 0.0, 0.0, -1.0 };
    fiOutput = fiQuery(line, capsule);
    error0 = std::fabs(fiOutput.parameter[0] + temp);
    error1 = std::fabs(fiOutput.parameter[1] - temp);
    error2 = Length(fiOutput.point[0] - Vector3<double>{ 0.5, 0.5, temp});
    error3 = Length(fiOutput.point[1] - Vector3<double>{ 0.5, 0.5, -temp});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, capsule);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // Line parallel to capsule axis, outside capsule.
    line.origin = { 1.0, 1.0, 0.0 };
    line.direction = { 0.0, 0.0, 1.0 };
    fiOutput = fiQuery(line, capsule);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, capsule);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");

    // Line parallel to capsule axis, on capsule wall.
    line.origin = { 1.0, 0.0, 0.0 };
    line.direction = { 0.0, 0.0, 1.0 };
    fiOutput = fiQuery(line, capsule);
    error0 = std::fabs(fiOutput.parameter[0] + 1.0);
    error1 = std::fabs(fiOutput.parameter[1] - 1.0);
    error2 = Length(fiOutput.point[0] - Vector3<double>{ 1.0, 0.0, -1.0 });
    error3 = Length(fiOutput.point[1] - Vector3<double>{ 1.0, 0.0, 1.0 });
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        fiOutput.parameter[0] == -1.0 &&
        fiOutput.parameter[1] == 1.0 &&
        (fiOutput.point[0] == Vector3<double>{ 1.0, 0.0, -1.0 }) &&
        (fiOutput.point[1] == Vector3<double>{ 1.0, 0.0, 1.0 }),
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, capsule);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // Line perpendicular to capsule axis, intersects capsule.
    line.origin = { 2.0, 0.5, 0.25 };
    line.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(line, capsule);
    error0 = std::fabs(fiOutput.parameter[0] + 2.8660254037844384);
    error1 = std::fabs(fiOutput.parameter[1] + 1.1339745962155614);
    error2 = Length(fiOutput.point[0] - Vector3<double>{-std::sqrt(0.75), 0.5, 0.25});
    error3 = Length(fiOutput.point[1] - Vector3<double>{std::sqrt(0.75), 0.5, 0.25});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, capsule);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // Line perpendicular to capsule axis, above top hemisphere of capsule.
    line.origin = { 2.0, 0.5, 2.0 };
    line.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(line, capsule);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, capsule);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");

    // Line perpendicular to capsule axis, intersects top hemisphere of capsule.
    line.origin = { 2.0, 0.5, 1.25 };
    line.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(line, capsule);
    error0 = std::fabs(fiOutput.parameter[0] + 2.8291561975888500);
    error1 = std::fabs(fiOutput.parameter[1] + 1.1708438024111500);
    error2 = Length(fiOutput.point[0] - Vector3<double>{-0.82915619758884995, 0.5, 1.25});
    error3 = Length(fiOutput.point[1] - Vector3<double>{0.82915619758884995, 0.5, 1.25});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, capsule);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // Line perpendicular to capsule axis, below bottom hemisphere of capsule.
    line.origin = { 2.0, 0.5, -2.0 };
    line.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(line, capsule);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, capsule);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");

    // Line perpendicular to capsule axis, intersects bottom hemisphere of capsule.
    line.origin = { 2.0, 0.5, -1.25 };
    line.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(line, capsule);
    error0 = std::fabs(fiOutput.parameter[0] + 2.8291561975888500);
    error1 = std::fabs(fiOutput.parameter[1] + 1.1708438024111500);
    error2 = Length(fiOutput.point[0] - Vector3<double>{-0.82915619758884995, 0.5, -1.25});
    error3 = Length(fiOutput.point[1] - Vector3<double>{0.82915619758884995, 0.5, -1.25});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, capsule);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // Line perpendicular to capsule axis, tangent to capsule wall.
    line.origin = { 1.0, 0.0, 0.0 };
    line.direction = { 0.0, 1.0, 0.0 };
    fiOutput = fiQuery(line, capsule);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 1 &&
        fiOutput.parameter[0] == 0.0 &&
        fiOutput.parameter[1] == 0.0 &&
        fiOutput.point[0] == line.origin &&
        fiOutput.point[1] == line.origin,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, capsule);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // Line perpendicular to capsule axis, outside capsule wall.
    line.origin = { 1.01, 0.0, 0.0 };
    line.direction = { 0.0, 1.0, 0.0 };
    fiOutput = fiQuery(line, capsule);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, capsule);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");

    // Line intersects top hemisphere of capsule.
    line.origin = { 2.0, 0.5, 2.0 };
    line.direction = Vector3<double>{ 0.25, 0.5, 1.0 } - line.origin;
    Normalize(line.direction);
    fiOutput = fiQuery(line, capsule);
    error0 = std::fabs(fiOutput.parameter[0] - 1.3755281542317046);
    error1 = std::fabs(fiOutput.parameter[1] - 3.3009479312116525);
    error2 = Length(fiOutput.point[0] - Vector3<double>{0.80570711328920686, 0.5, 1.3175469218795468});
    error3 = Length(fiOutput.point[1] - Vector3<double>{-0.86602540378443882, 0.5, 0.36227119783746353});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, capsule);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // Line intersects top hemisphere of capsule, opposite direction of previous.
    line.origin = { 2.0, 0.5, 2.0 };
    line.direction = Vector3<double>{ 0.25, 0.5, 1.0 } - line.origin;
    Normalize(line.direction);
    line.direction = -line.direction;
    fiOutput = fiQuery(line, capsule);
    error0 = std::fabs(fiOutput.parameter[0] + 3.3009479312116525);
    error1 = std::fabs(fiOutput.parameter[1] + 1.3755281542317046);
    error2 = Length(fiOutput.point[0] - Vector3<double>{-0.86602540378443882, 0.5, 0.36227119783746353});
    error3 = Length(fiOutput.point[1] - Vector3<double>{0.80570711328920686, 0.5, 1.3175469218795468});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, capsule);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // Line intersects bottom hemisphere of capsule.
    line.origin = { 2.0, 0.5, -2.0 };
    line.direction = Vector3<double>{ 0.25, 0.5, -1.0 } - line.origin;
    Normalize(line.direction);
    fiOutput = fiQuery(line, capsule);
    error0 = std::fabs(fiOutput.parameter[0] - 1.3755281542317053);
    error1 = std::fabs(fiOutput.parameter[1] - 3.3009479312116525);
    error2 = Length(fiOutput.point[0] - Vector3<double>{0.80570711328920641, 0.5, -1.3175469218795466});
    error3 = Length(fiOutput.point[1] - Vector3<double>{-0.86602540378443882, 0.5, -0.36227119783746353});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, capsule);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // Line intersects bottom hemisphere of capsule, opposite direction of previous.
    line.origin = { 2.0, 0.5, -2.0 };
    line.direction = Vector3<double>{ 0.25, 0.5, -1.0 } - line.origin;
    Normalize(line.direction);
    line.direction = -line.direction;
    fiOutput = fiQuery(line, capsule);
    error0 = std::fabs(fiOutput.parameter[0] + 3.3009479312116525);
    error1 = std::fabs(fiOutput.parameter[1] + 1.3755281542317053);
    error2 = Length(fiOutput.point[0] - Vector3<double>{-0.86602540378443882, 0.5, -0.36227119783746353});
    error3 = Length(fiOutput.point[1] - Vector3<double>{0.80570711328920641, 0.5, -1.3175469218795466});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, capsule);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // Line intersects both end hemisphere.
    line.origin = { 0.25, 0.5, 1.125 };
    line.direction = { 0.001, 0.002, 1.0 };
    Normalize(line.direction);
    fiOutput = fiQuery(line, capsule);
    error0 = std::fabs(fiOutput.parameter[0] + 2.9585856349180419);
    error1 = std::fabs(fiOutput.parameter[1] - 0.70309583035521317);
    error2 = Length(fiOutput.point[0] - Vector3<double>{0.24704142176151830, 0.49408284352303661, -1.8335782384816910});
    error3 = Length(fiOutput.point[1] - Vector3<double>{0.25070309407262220, 0.50140618814524440, 1.8280940726222288});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, capsule);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // Line intersects both end hemispheres, opposite direction of previous.
    line.origin = { 0.25, 0.5, 1.125 };
    line.direction = { 0.001, 0.002, 1.0 };
    Normalize(line.direction);
    line.direction = -line.direction;
    fiOutput = fiQuery(line, capsule);
    error0 = std::fabs(fiOutput.parameter[0] + 0.70309583035521317);
    error1 = std::fabs(fiOutput.parameter[1] - 2.9585856349180419);
    error2 = Length(fiOutput.point[0] - Vector3<double>{0.25070309407262220, 0.50140618814524440, 1.8280940726222288});
    error3 = Length(fiOutput.point[1] - Vector3<double>{0.24704142176151830, 0.49408284352303661, -1.8335782384816910});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    tiOutput = tiQuery(line, capsule);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrLine3Capsule3.h>

namespace gtl
{
    template class TIQuery<float, Line3<float>, Capsule3<float>>;
    template class FIQuery<float, Line3<float>, Capsule3<float>>;

    template class TIQuery<double, Line3<double>, Capsule3<double>>;
    template class FIQuery<double, Line3<double>, Capsule3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Line3<Rational>, Capsule3<Rational>>;
    template class FIQuery<Rational, Line3<Rational>, Capsule3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrLine3Capsule3)
