#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrLine3Cylinder3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrLine3Cylinder3
    {
    public:
        UnitTestIntrLine3Cylinder3();

    private:
        void Test();
    };
}

UnitTestIntrLine3Cylinder3::UnitTestIntrLine3Cylinder3()
{
    UTInformation("Mathematics/Intersection/3D/IntrLine3Cylinder3");

    Test();
}

void UnitTestIntrLine3Cylinder3::Test()
{
    // x^2 + y^2 - r^2 = 1, |z| <= h/2 = 1
    Cylinder3<double> cylinder{};
    cylinder.center = { 0.0, 0.0, 0.0 };
    cylinder.direction = { 0.0, 0.0, 1.0 };
    cylinder.radius = 1.0;
    cylinder.height = 2.0;

    Line3<double> line{};
    FIQuery<double, Line3<double>, Cylinder3<double>> fiQuery{};

    // Line in same direction as cylinder axis, inside cylinder.
    line.origin = { 0.5, 0.5, 0.0 };
    line.direction = { 0.0, 0.0, 1.0 };
    auto fiOutput = fiQuery(line, cylinder);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        fiOutput.parameter[0] == -1.0 &&
        fiOutput.parameter[1] == 1.0 &&
        (fiOutput.point[0] == Vector3<double>{ 0.5, 0.5, -1.0 }) &&
        (fiOutput.point[1] == Vector3<double>{ 0.5, 0.5, 1.0 }),
        "Invalid FIQuery.");

    // Line in opposite direction as cylinder axis, inside cylinder.
    line.origin = { 0.5, 0.5, 0.0 };
    line.direction = { 0.0, 0.0, -1.0 };
    fiOutput = fiQuery(line, cylinder);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        fiOutput.parameter[0] == -1.0 &&
        fiOutput.parameter[1] == 1.0 &&
        (fiOutput.point[0] == Vector3<double>{ 0.5, 0.5, 1.0 }) &&
        (fiOutput.point[1] == Vector3<double>{ 0.5, 0.5, -1.0 }),
        "Invalid FIQuery.");

    // Line parallel to cylinder axis, outside cylinder.
    line.origin = { 1.0, 1.0, 0.0 };
    line.direction = { 0.0, 0.0, 1.0 };
    fiOutput = fiQuery(line, cylinder);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    // Line parallel to cylinder axis, on cylinder wall.
    line.origin = { 1.0, 0.0, 0.0 };
    line.direction = { 0.0, 0.0, 1.0 };
    fiOutput = fiQuery(line, cylinder);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        fiOutput.parameter[0] == -1.0 &&
        fiOutput.parameter[1] == 1.0 &&
        (fiOutput.point[0] == Vector3<double>{ 1.0, 0.0, -1.0 }) &&
        (fiOutput.point[1] == Vector3<double>{ 1.0, 0.0, 1.0 }),
        "Invalid FIQuery.");

    // Line perpendicular to cylinder axis, intersects cylinder.
    line.origin = { 2.0, 0.5, 0.25 };
    line.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(line, cylinder);
    double error0 = std::fabs(fiOutput.parameter[0] + 2.8660254037844384);
    double error1 = std::fabs(fiOutput.parameter[1] + 1.1339745962155614);
    double error2 = Length(fiOutput.point[0] - Vector3<double>{-std::sqrt(0.75), 0.5, 0.25});
    double error3 = Length(fiOutput.point[1] - Vector3<double>{std::sqrt(0.75), 0.5, 0.25});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    // Line perpendicular to cylinder axis, above top disk of cylinder.
    line.origin = { 2.0, 0.5, 2.0 };
    line.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(line, cylinder);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    // Line perpendicular to cylinder axis, coincident top disk of cylinder.
    line.origin = { 2.0, 0.5, 1.0 };
    line.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(line, cylinder);
    error0 = std::fabs(fiOutput.parameter[0] + 2.8660254037844384);
    error1 = std::fabs(fiOutput.parameter[1] + 1.1339745962155614);
    error2 = Length(fiOutput.point[0] - Vector3<double>{-std::sqrt(0.75), 0.5, 1.0});
    error3 = Length(fiOutput.point[1] - Vector3<double>{std::sqrt(0.75), 0.5, 1.0});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    // Line perpendicular to cylinder axis, below bottom disk of cylinder.
    line.origin = { 2.0, 0.5, -2.0 };
    line.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(line, cylinder);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    // Line perpendicular to cylinder axis, coincident bottom disk of cylinder.
    line.origin = { 2.0, 0.5, -1.0 };
    line.direction = { 1.0, 0.0, 0.0 };
    fiOutput = fiQuery(line, cylinder);
    error0 = std::fabs(fiOutput.parameter[0] + 2.8660254037844384);
    error1 = std::fabs(fiOutput.parameter[1] + 1.1339745962155614);
    error2 = Length(fiOutput.point[0] - Vector3<double>{-std::sqrt(0.75), 0.5, -1.0});
    error3 = Length(fiOutput.point[1] - Vector3<double>{std::sqrt(0.75), 0.5, -1.0});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    // Line perpendicular to cylinder axis, tangent to cylinder wall.
    line.origin = { 1.0, 0.0, 0.0 };
    line.direction = { 0.0, 1.0, 0.0 };
    fiOutput = fiQuery(line, cylinder);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 1 &&
        fiOutput.parameter[0] == 0.0 &&
        fiOutput.parameter[1] == 0.0 &&
        fiOutput.point[0] == line.origin &&
        fiOutput.point[1] == line.origin,
        "Invalid FIQuery.");

    // Line perpendicular to cylinder axis, outside cylinder wall.
    line.origin = { 1.01, 0.0, 0.0 };
    line.direction = { 0.0, 1.0, 0.0 };
    fiOutput = fiQuery(line, cylinder);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    // Line intersects top disk of cylinder.
    line.origin = { 2.0, 0.5, 2.0 };
    line.direction = Vector3<double>{ 0.25, 0.5, 1.0 } - line.origin;
    Normalize(line.direction);
    fiOutput = fiQuery(line, cylinder);
    error0 = std::fabs(fiOutput.parameter[0] - 2.0155644370746373);
    error1 = std::fabs(fiOutput.parameter[1] - 3.3009479312116525);
    error2 = Length(fiOutput.point[0] - Vector3<double>{0.25, 0.5, 1.0});
    error3 = Length(fiOutput.point[1] - Vector3<double>{-std::sqrt(0.75), 0.5, 0.36227119783746353});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");
    double radial =
        fiOutput.point[0][0] * fiOutput.point[0][0] +
        fiOutput.point[0][1] * fiOutput.point[0][1] -
        cylinder.radius * cylinder.radius;
    error0 = std::fabs(fiOutput.point[0][2] - 0.5 * cylinder.height);
    UTAssert(
        radial <= 0.0 &&
        error0 <= 1e-15,
        "Invalid FIQuery.");
    error0 = std::fabs(
        fiOutput.point[1][0] * fiOutput.point[1][0] +
        fiOutput.point[1][1] * fiOutput.point[1][1] -
        cylinder.radius * cylinder.radius);
    UTAssert(
        error0 <= 1e-15 &&
        -0.5 * cylinder.height <= fiOutput.point[1][2] &&
        fiOutput.point[1][2] <= 0.5 * cylinder.height,
        "Invalid FIQuery.");

    // Line intersects top disk of cylinder, opposite direction of previous.
    line.origin = { 2.0, 0.5, 2.0 };
    line.direction = Vector3<double>{ 0.25, 0.5, 1.0 } - line.origin;
    Normalize(line.direction);
    line.direction = -line.direction;
    fiOutput = fiQuery(line, cylinder);
    error0 = std::fabs(fiOutput.parameter[0] + 3.3009479312116525);
    error1 = std::fabs(fiOutput.parameter[1] + 2.0155644370746373);
    error2 = Length(fiOutput.point[0] - Vector3<double>{-std::sqrt(0.75), 0.5, 0.36227119783746353});
    error3 = Length(fiOutput.point[1] - Vector3<double>{0.25, 0.5, 1.0});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");
    radial =
        fiOutput.point[1][0] * fiOutput.point[1][0] +
        fiOutput.point[1][1] * fiOutput.point[1][1] -
        cylinder.radius * cylinder.radius;
    error0 = std::fabs(fiOutput.point[1][2] - 0.5 * cylinder.height);
    UTAssert(
        radial <= 0.0 &&
        error0 <= 1e-15,
        "Invalid FIQuery.");
    error0 = std::fabs(
        fiOutput.point[0][0] * fiOutput.point[0][0] +
        fiOutput.point[0][1] * fiOutput.point[0][1] -
        cylinder.radius * cylinder.radius);
    UTAssert(
        error0 <= 1e-15 &&
        -0.5 * cylinder.height <= fiOutput.point[1][2] &&
        fiOutput.point[1][2] <= 0.5 * cylinder.height,
        "Invalid FIQuery.");

    // Line intersects bottom disk of cylinder.
    line.origin = { 2.0, 0.5, -2.0 };
    line.direction = Vector3<double>{ 0.25, 0.5, -1.0 } - line.origin;
    Normalize(line.direction);
    fiOutput = fiQuery(line, cylinder);
    error0 = std::fabs(fiOutput.parameter[0] - 2.0155644370746373);
    error1 = std::fabs(fiOutput.parameter[1] - 3.3009479312116525);
    error2 = Length(fiOutput.point[0] - Vector3<double>{0.25, 0.5, -1.0});
    error3 = Length(fiOutput.point[1] - Vector3<double>{-std::sqrt(0.75), 0.5, -0.36227119783746353});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");
    radial =
        fiOutput.point[0][0] * fiOutput.point[0][0] +
        fiOutput.point[0][1] * fiOutput.point[0][1] -
        cylinder.radius * cylinder.radius;
    error0 = std::fabs(fiOutput.point[0][2] + 0.5 * cylinder.height);
    UTAssert(
        radial <= 0.0 &&
        error0 <= 1e-15,
        "Invalid FIQuery.");
    error0 = std::fabs(
        fiOutput.point[1][0] * fiOutput.point[1][0] +
        fiOutput.point[1][1] * fiOutput.point[1][1] -
        cylinder.radius * cylinder.radius);
    UTAssert(
        error0 <= 1e-15 &&
        -0.5 * cylinder.height <= fiOutput.point[1][2] &&
        fiOutput.point[1][2] <= 0.5 * cylinder.height,
        "Invalid FIQuery.");

    // Line intersects bottom disk of cylinder, opposite direction of previous.
    line.origin = { 2.0, 0.5, -2.0 };
    line.direction = Vector3<double>{ 0.25, 0.5, -1.0 } - line.origin;
    Normalize(line.direction);
    line.direction = -line.direction;
    fiOutput = fiQuery(line, cylinder);
    error0 = std::fabs(fiOutput.parameter[0] + 3.3009479312116525);
    error1 = std::fabs(fiOutput.parameter[1] + 2.0155644370746373);
    error2 = Length(fiOutput.point[0] - Vector3<double>{-std::sqrt(0.75), 0.5, -0.36227119783746353});
    error3 = Length(fiOutput.point[1] - Vector3<double>{0.25, 0.5, -1.0});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");
    radial =
        fiOutput.point[1][0] * fiOutput.point[1][0] +
        fiOutput.point[1][1] * fiOutput.point[1][1] -
        cylinder.radius * cylinder.radius;
    error0 = std::fabs(fiOutput.point[1][2] + 0.5 * cylinder.height);
    UTAssert(
        radial <= 0.0 &&
        error0 <= 1e-15,
        "Invalid FIQuery.");
    error0 = std::fabs(
        fiOutput.point[0][0] * fiOutput.point[0][0] +
        fiOutput.point[0][1] * fiOutput.point[0][1] -
        cylinder.radius * cylinder.radius);
    UTAssert(
        error0 <= 1e-15 &&
        -0.5 * cylinder.height <= fiOutput.point[0][2] &&
        fiOutput.point[0][2] <= 0.5 * cylinder.height,
        "Invalid FIQuery.");

    // Line intersects both end disks.
    line.origin = { 0.25, 0.5, 1.125 };
    line.direction = { 0.001, 0.002, 1.0 };
    Normalize(line.direction);
    fiOutput = fiQuery(line, cylinder);
    error0 = std::fabs(fiOutput.parameter[0] + 2.1250053124933594);
    error1 = std::fabs(fiOutput.parameter[1] + 0.12500031249960938);
    error2 = Length(fiOutput.point[0] - Vector3<double>{0.247875, 0.49575, -1.0});
    error3 = Length(fiOutput.point[1] - Vector3<double>{0.249875, 0.49975, 1.0});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    // Line intersects both end disks, opposite direction of previous.
    // Leads to std::swap call at end of DoQuery.
    line.origin = { 0.25, 0.5, 1.125 };
    line.direction = { 0.001, 0.002, 1.0 };
    Normalize(line.direction);
    line.direction = -line.direction;
    fiOutput = fiQuery(line, cylinder);
    error0 = std::fabs(fiOutput.parameter[0] - 0.12500031249960938);
    error1 = std::fabs(fiOutput.parameter[1] - 2.1250053124933594);
    error2 = Length(fiOutput.point[0] - Vector3<double>{0.249875, 0.49975, 1.0});
    error3 = Length(fiOutput.point[1] - Vector3<double>{0.247875, 0.49575, -1.0});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    // Line tangent to cylinder wall but not at end disks.
    line.origin = { 1.0, 0.0, 0.0 };
    line.direction = { 0.0, 0.9, 0.1 };
    Normalize(line.direction);
    fiOutput = fiQuery(line, cylinder);
    error0 = std::fabs(fiOutput.parameter[0]);
    error1 = std::fabs(fiOutput.parameter[1] - fiOutput.parameter[0]);
    error2 = Length(fiOutput.point[0] - Vector3<double>{1.0, 0.0, 0.0});
    error3 = Length(fiOutput.point[1] - Vector3<double>{1.0, 0.0, 0.0});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 1 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    // Line tangent to cylinder wall but not at end disks. Opposite
    // direction of previous.
    line.origin = { 1.0, 0.0, 0.0 };
    line.direction = { 0.0, -0.9, -0.1 };
    Normalize(line.direction);
    fiOutput = fiQuery(line, cylinder);
    error0 = std::fabs(fiOutput.parameter[0]);
    error1 = std::fabs(fiOutput.parameter[1] - fiOutput.parameter[0]);
    error2 = Length(fiOutput.point[0] - Vector3<double>{1.0, 0.0, 0.0});
    error3 = Length(fiOutput.point[1] - Vector3<double>{1.0, 0.0, 0.0});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 1 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrLine3Cylinder3.h>

namespace gtl
{
    template class FIQuery<float, Line3<float>, Cylinder3<float>>;
    template class FIQuery<double, Line3<double>, Cylinder3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class FIQuery<Rational, Line3<Rational>, Cylinder3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrLine3Cylinder3)
