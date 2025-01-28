#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/ND/DistPointHyperellipsoid.h>
#include <GTL/Mathematics/Primitives/2D/Ellipse2.h>
#include <GTL/Mathematics/Primitives/3D/Ellipsoid3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistPointHyperellipsoid
    {
    public:
        UnitTestDistPointHyperellipsoid();

    private:
        void Test2D();
        void Test3D();
    };
}

UnitTestDistPointHyperellipsoid::UnitTestDistPointHyperellipsoid()
{
    UTInformation("Mathematics/Distance/ND/DistPointHyperellipsoid");

    Test2D();
    Test3D();
}

void UnitTestDistPointHyperellipsoid::Test2D()
{
    using DCPPointEllipseQuery = DCPQuery<double, Vector<double, 2>, Hyperellipsoid<double, 2>>;
    DCPPointEllipseQuery query{};
    DCPPointEllipseQuery::Output output{};
    Vector<double, 2> expectedClosest{};
    double error{};
    Ellipse2<double> ellipse{};
    Vector<double, 2> point{};

    Vector<double, 2> translate{ 0.1234, 5.6789 };
    Matrix2x2<double> rotate{};
    double const angle = 0.98765;
    RigidMotion<double>::Convert(angle, rotate);

    // Test canonical ellipse.
    ellipse.center = { 0.0, 0.0 };
    ellipse.axis[0] = { 1.0, 0.0 };
    ellipse.axis[1] = { 0.0, 1.0 };
    ellipse.extent = { 2.0, 1.0 };
    point = { 2.0, 1.0 };
    output = query(point, ellipse.extent);
    error = std::fabs(output.sqrDistance - 0.31111865868329103);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.31111865868329103));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 1.6649685472319564, 0.55404867492132592 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Apply a rigid motion to canonical ellipse and point.
    point = rotate * point + translate;
    ellipse.center = rotate * ellipse.center + translate;
    ellipse.axis[0] = rotate * ellipse.axis[0];
    ellipse.axis[1] = rotate * ellipse.axis[1];
    ellipse.extent = { 2.0, 1.0 };
    output = query(point, ellipse);
    error = std::fabs(output.sqrDistance - 0.31111865868329097);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.31111865868329097));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 0.57773652815274745, 7.3737948394044617 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Test canonical ellipse.
    ellipse.center = { 0.0, 0.0 };
    ellipse.axis[0] = { 1.0, 0.0 };
    ellipse.axis[1] = { 0.0, 1.0 };
    ellipse.extent = { 2.0, 1.0 };
    point = { 0.0, 0.0 };
    output = query(point, ellipse.extent);
    error = std::fabs(output.sqrDistance - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(1.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 0.0, 1.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Apply a rigid motion to canonical ellipse and point.
    point = rotate * point + translate;
    ellipse.center = rotate * ellipse.center + translate;
    ellipse.axis[0] = rotate * ellipse.axis[0];
    ellipse.axis[1] = rotate * ellipse.axis[1];
    ellipse.extent = { 2.0, 1.0 };
    output = query(point, ellipse);
    error = std::fabs(output.sqrDistance - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(1.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { -0.71133425013928719, 6.2295530047538117 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Test canonical ellipse.
    ellipse.center = { 0.0, 0.0 };
    ellipse.axis[0] = { 1.0, 0.0 };
    ellipse.axis[1] = { 0.0, 1.0 };
    ellipse.extent = { 2.0, 1.0 };
    point = { 1.0, 0.0 };
    output = query(point, ellipse.extent);
    error = std::fabs(output.sqrDistance - 2.0 / 3.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(2.0 / 3.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 4.0 / 3.0, std::sqrt(5.0) / 3.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Apply a rigid motion to canonical ellipse and point.
    point = rotate * point + translate;
    ellipse.center = rotate * ellipse.center + translate;
    ellipse.axis[0] = rotate * ellipse.axis[0];
    ellipse.axis[1] = rotate * ellipse.axis[1];
    ellipse.extent = { 2.0, 1.0 };
    output = query(point, ellipse);
    error = std::fabs(output.sqrDistance - 2.0 / 3.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(2.0 / 3.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 0.23542983085216307, 7.2023115170670629 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Test canonical ellipse.
    ellipse.center = { 0.0, 0.0 };
    ellipse.axis[0] = { 1.0, 0.0 };
    ellipse.axis[1] = { 0.0, 1.0 };
    ellipse.extent = { 2.0, 1.0 };
    point = { 1.75, 0.0 };
    output = query(point, ellipse.extent);
    error = std::fabs(output.sqrDistance - 0.0625);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.0625));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 2.0, 0.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Apply a rigid motion to canonical ellipse and point.
    point = rotate * point + translate;
    ellipse.center = rotate * ellipse.center + translate;
    ellipse.axis[0] = rotate * ellipse.axis[0];
    ellipse.axis[1] = rotate * ellipse.axis[1];
    ellipse.extent = { 2.0, 1.0 };
    output = query(point, ellipse);
    error = std::fabs(output.sqrDistance - 0.0625);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.0625));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 1.2247060095076245, 7.3483685002785739 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Test canonical ellipse.
    ellipse.center = { 0.0, 0.0 };
    ellipse.axis[0] = { 1.0, 0.0 };
    ellipse.axis[1] = { 0.0, 1.0 };
    ellipse.extent = { 2.0, 1.0 };
    point = { 1.0, 0.25 };
    output = query(point, ellipse.extent);
    error = std::fabs(output.sqrDistance - 0.34245522832870634);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.34245522832870634));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 1.2071617665807557, 0.79730177306112104 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    double term0 = Dot(ellipse.axis[0], output.closest[1] - ellipse.center) / ellipse.extent[0];
    double term1 = Dot(ellipse.axis[1], output.closest[1] - ellipse.center) / ellipse.extent[1];
    error = std::fabs(term0 * term0 + term1 * term1 - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Apply a rigid motion to canonical ellipse and point.
    point = rotate * point + translate;
    ellipse.center = rotate * ellipse.center + translate;
    ellipse.axis[0] = rotate * ellipse.axis[0];
    ellipse.axis[1] = rotate * ellipse.axis[1];
    ellipse.extent = { 2.0, 1.0 };
    output = query(point, ellipse);
    error = std::fabs(output.sqrDistance - 0.34245522832870651);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.34245522832870651));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 0.12259215632071407, 7.1255958890552531 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    term0 = Dot(ellipse.axis[0], output.closest[1] - ellipse.center) / ellipse.extent[0];
    term1 = Dot(ellipse.axis[1], output.closest[1] - ellipse.center) / ellipse.extent[1];
    error = std::fabs(term0 * term0 + term1 * term1 - 1.0);
    UTAssert(
        error <= 1e-14,
        "Invalid DCPQuery.");
}

void UnitTestDistPointHyperellipsoid::Test3D()
{
    using DCPPointEllipsoidQuery = DCPQuery<double, Vector<double, 3>, Hyperellipsoid<double, 3>>;
    DCPPointEllipsoidQuery query{};
    DCPPointEllipsoidQuery::Output output{};
    Vector<double, 3> expectedClosest{};
    double error{};
    Ellipsoid3<double> ellipsoid{};
    Vector<double, 3> point{};

    Vector<double, 3> translate{ 0.1234, 5.6789, -1.9735 };
    Matrix3x3<double> rotate{};
    Quaternion<double> q(1.0, 2.0, 3.0, 4.0);
    Normalize(q);
    RigidMotion<double>::Convert(q, rotate);

    // Test canonical ellipsoid.
    ellipsoid.center = { 0.0, 0.0, 0.0 };
    ellipsoid.axis[0] = { 1.0, 0.0, 0.0 };
    ellipsoid.axis[1] = { 0.0, 1.0, 0.0 };
    ellipsoid.axis[2] = { 0.0, 0.0, 1.0 };
    ellipsoid.extent = { 3.0, 2.0, 1.0 };
    point = { 3.0, 2.0, 1.0 };
    output = query(point, ellipsoid.extent);
    error = std::fabs(output.sqrDistance - 1.7195130895885649);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(1.7195130895885649));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 2.2905863765120924, 1.1786589518792594, 0.26403513462241590 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Apply a rigid motion to canonical ellipsoid and point.
    point = rotate * point + translate;
    ellipsoid.center = rotate * ellipsoid.center + translate;
    ellipsoid.axis[0] = rotate * ellipsoid.axis[0];
    ellipsoid.axis[1] = rotate * ellipsoid.axis[1];
    ellipsoid.axis[2] = rotate * ellipsoid.axis[2];
    ellipsoid.extent = { 3.0, 2.0, 1.0 };
    output = query(point, ellipsoid);
    error = std::fabs(output.sqrDistance - 1.7195130895885655);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(1.7195130895885655));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { -0.16333535232812166, 8.2448716199873626, -1.7752327345029142 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Test canonical ellipsoid.
    ellipsoid.center = { 0.0, 0.0, 0.0 };
    ellipsoid.axis[0] = { 1.0, 0.0, 0.0 };
    ellipsoid.axis[1] = { 0.0, 1.0, 0.0 };
    ellipsoid.axis[2] = { 0.0, 0.0, 1.0 };
    ellipsoid.extent = { 3.0, 2.0, 1.0 };
    point = { 0.0, 0.0, 0.125 };
    output = query(point, ellipsoid.extent);
    error = std::fabs(output.sqrDistance - 0.765625);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.765625));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 0.0, 0.0, 1.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Apply a rigid motion to canonical ellipsoid and point.
    point = rotate * point + translate;
    ellipsoid.center = rotate * ellipsoid.center + translate;
    ellipsoid.axis[0] = rotate * ellipsoid.axis[0];
    ellipsoid.axis[1] = rotate * ellipsoid.axis[1];
    ellipsoid.axis[2] = rotate * ellipsoid.axis[2];
    ellipsoid.extent = { 3.0, 2.0, 1.0 };
    output = query(point, ellipsoid);
    error = std::fabs(output.sqrDistance - 0.765625);
    UTAssert(
        error <= 1e-14,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.765625));
    UTAssert(
        error <= 1e-14,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 0.85673333333333268, 5.8122333333333325, -1.3068333333333340 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Test canonical ellipsoid.
    ellipsoid.center = { 0.0, 0.0, 0.0 };
    ellipsoid.axis[0] = { 1.0, 0.0, 0.0 };
    ellipsoid.axis[1] = { 0.0, 1.0, 0.0 };
    ellipsoid.axis[2] = { 0.0, 0.0, 1.0 };
    ellipsoid.extent = { 3.0, 2.0, 1.0 };
    point = { 1.0, 0.0, 0.125 };
    output = query(point, ellipsoid.extent);
    error = std::fabs(output.sqrDistance - 0.65855532662493643);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.65855532662493643));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 1.1064014385777148, 0.0, 0.92950858323173235 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    double term0 = Dot(ellipsoid.axis[0], output.closest[1] - ellipsoid.center) / ellipsoid.extent[0];
    double term1 = Dot(ellipsoid.axis[1], output.closest[1] - ellipsoid.center) / ellipsoid.extent[1];
    double term2 = Dot(ellipsoid.axis[2], output.closest[1] - ellipsoid.center) / ellipsoid.extent[2];
    error = std::fabs(term0 * term0 + term1 * term1 + term2 * term2 - 1.0);
    UTAssert(
        error <= 1e-14,
        "Invalid DCPQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/ND/DistPointHyperellipsoid.h>

namespace gtl
{
    template class DCPQuery<float, Vector<float, 2>, Hyperellipsoid<float, 2>>;
    template class DCPQuery<double, Vector<double, 3>, Hyperellipsoid<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Vector<Rational, 4>, Hyperellipsoid<Rational, 4>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPointHyperellipsoid)
