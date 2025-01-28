#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/ND/DistPointRectangle.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistPointRectangle
    {
    public:
        UnitTestDistPointRectangle();

    private:
        void Test2D();
        void Test3D();
    };
}

UnitTestDistPointRectangle::UnitTestDistPointRectangle()
{
    UTInformation("Mathematics/Distance/ND/DistPointRectangle");

    Test2D();
    Test3D();
}

void UnitTestDistPointRectangle::Test2D()
{
    using DCPPointRectangleQuery = DCPQuery<double, Vector<double, 2>, Rectangle<double, 2>>;
    DCPPointRectangleQuery query{};
    DCPPointRectangleQuery::Output output{};
    Vector<double, 2> expectedClosest{};
    double error{};

    Rectangle<double, 2> rectangle{};
    rectangle.center = { 1.0, 1.0 };
    rectangle.axis[0] = { 2.0, 1.0 };
    rectangle.axis[1] = { -1.0, 2.0 };
    Normalize(rectangle.axis[0]);
    Normalize(rectangle.axis[1]);
    rectangle.extent = { std::sqrt(5.0), 0.5 * std::sqrt(5.0) };
    std::array<Vector<double, 2>, 4> vertices{};
    rectangle.GetVertices(vertices);
    UTAssert(
        (vertices[0] == Vector<double, 2>{ -0.5, -1.0 }) &&
        (vertices[1] == Vector<double, 2>{ 3.5, 1.0 }) &&
        (vertices[2] == Vector<double, 2>{ -1.5, 1.0 }) &&
        (vertices[3] == Vector<double, 2>{ 2.5, 3.0 }),
        "Invalid vertex.");
    // v0 = C - e0 * U0 - e1 * U1
    // v1 = C + e0 * U0 - e1 * U1
    // v2 = C - e0 * U0 + e1 * U1
    // v3 = C + e0 * U0 + e1 * U1

    Vector<double, 2> point{};

    // inside rectangle
    point = { 2.0, 1.0 };
    output = query(point, rectangle);
    error = std::fabs(output.sqrDistance);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[0] - 0.89442719099991586);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] + 0.44721359549995793);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = point;
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Voronoi region for <v0>
    point = { -1.0, -3.0 };
    output = query(point, rectangle);
    error = std::fabs(output.sqrDistance - 4.25);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(4.25));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[0] + 2.2360679774997898);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] + 1.1180339887498949);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { -0.5, -1.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Voronoi region for <v0,v1>
    point = { 2.0, -1.0 };
    output = query(point, rectangle);
    error = std::fabs(output.sqrDistance - 1.25);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(1.25));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[0] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] + 1.1180339887498949);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 1.5, 0.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Voronoi region for <v1>
    point = { 6.0, 0.0 };
    output = query(point, rectangle);
    error = std::fabs(output.sqrDistance - 7.25);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(7.25));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[0] - 2.2360679774997898);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] + 1.1180339887498949);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 3.5, 1.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Voronoi region for <v1,v2>
    point = { 3.5, 2.0 };
    output = query(point, rectangle);
    error = std::fabs(output.sqrDistance - 0.2);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.2));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[0] - 2.2360679774997898);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] + 0.22360679774997905);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 3.1, 1.8 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Voronoi region for <v2>
    point = { 2.5, 4.0 };
    output = query(point, rectangle);
    error = std::fabs(output.sqrDistance - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(1.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[0] - 2.2360679774997898);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] - 1.1180339887498949);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 2.5, 3.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Voronoi region for <v2,v3>
    point = { 1.5, 3.0 };
    output = query(point, rectangle);
    error = std::fabs(output.sqrDistance - 0.2);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.2));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[0] - 1.3416407864998738);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] - 1.1180339887498949);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 1.7, 2.6 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Voronoi region for <v3>
    point = { -2.0, 1.1 };
    output = query(point, rectangle);
    error = std::fabs(output.sqrDistance - 0.26);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.26));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[0] + 2.2360679774997898);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] - 1.1180339887498949);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { -1.5, 1.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Voronoi region for <v3,v1>
    point = { -2.0, 0.0 };
    output = query(point, rectangle);
    error = std::fabs(output.sqrDistance - 0.8);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(0.8));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[0] + 2.2360679774997898);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] - 0.44721359549995798);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { -1.2, 0.4 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
}

void UnitTestDistPointRectangle::Test3D()
{
    using DCPPointRectangleQuery = DCPQuery<double, Vector<double, 3>, Rectangle<double, 3>>;
    DCPPointRectangleQuery query{};
    DCPPointRectangleQuery::Output output{};
    Vector<double, 3> expectedClosest{};
    double error{};

    Rectangle<double, 3> rectangle{};
    rectangle.center = { 1.0, 1.0, 1.0 };
    rectangle.axis[0] = { 2.0, 1.0, 0.0 };
    rectangle.axis[1] = { -1.0, 2.0, 0.0 };
    Normalize(rectangle.axis[0]);
    Normalize(rectangle.axis[1]);
    rectangle.extent = { std::sqrt(5.0), 0.5 * std::sqrt(5.0) };
    std::array<Vector<double, 3>, 4> vertices{};
    rectangle.GetVertices(vertices);
    UTAssert(
        (vertices[0] == Vector<double, 3>{ -0.5, -1.0, 1.0 }) &&
        (vertices[1] == Vector<double, 3>{ 3.5, 1.0, 1.0 }) &&
        (vertices[2] == Vector<double, 3>{ -1.5, 1.0, 1.0 }) &&
        (vertices[3] == Vector<double, 3>{ 2.5, 3.0, 1.0 }),
        "Invalid vertex.");
    // v0 = C - e0 * U0 - e1 * U1
    // v1 = C + e0 * U0 - e1 * U1
    // v2 = C - e0 * U0 + e1 * U1
    // v3 = C + e0 * U0 + e1 * U1

    Vector<double, 3> point{};

    // inside rectangle
    point = { 2.0, 1.0, 2.0 };
    output = query(point, rectangle);
    error = std::fabs(output.sqrDistance - 1.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(1.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[0] - 0.89442719099991586);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] + 0.44721359549995793);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 2.0, 1.0, 1.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Voronoi region for <v0>
    point = { -1.0, -3.0, 2.0 };
    output = query(point, rectangle);
    error = std::fabs(output.sqrDistance - 5.25);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(5.25));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[0] + 2.2360679774997898);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] + 1.1180339887498949);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { -0.5, -1.0, 1.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Voronoi region for <v0,v1>
    point = { 2.0, -1.0, 2.0 };
    output = query(point, rectangle);
    error = std::fabs(output.sqrDistance - 2.25);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(2.25));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[0] - 0.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] + 1.1180339887498949);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 1.5, 0.0, 1.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Voronoi region for <v1>
    point = { 6.0, 0.0, 2.0 };
    output = query(point, rectangle);
    error = std::fabs(output.sqrDistance - 8.25);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(8.25));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[0] - 2.2360679774997898);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] + 1.1180339887498949);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 3.5, 1.0, 1.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Voronoi region for <v1,v2>
    point = { 3.5, 2.0, 2.0 };
    output = query(point, rectangle);
    error = std::fabs(output.sqrDistance - 1.2);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(1.2));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[0] - 2.2360679774997898);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] + 0.22360679774997905);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 3.1, 1.8, 1.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Voronoi region for <v2>
    point = { 2.5, 4.0, 2.0 };
    output = query(point, rectangle);
    error = std::fabs(output.sqrDistance - 2.0);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(2.0));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[0] - 2.2360679774997898);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] - 1.1180339887498949);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 2.5, 3.0, 1.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Voronoi region for <v2,v3>
    point = { 1.5, 3.0, 2.0 };
    output = query(point, rectangle);
    error = std::fabs(output.sqrDistance - 1.2);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(1.2));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[0] - 1.3416407864998738);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] - 1.1180339887498949);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { 1.7, 2.6, 1.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Voronoi region for <v3>
    point = { -2.0, 1.1, 2.0 };
    output = query(point, rectangle);
    error = std::fabs(output.sqrDistance - 1.26);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(1.26));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[0] + 2.2360679774997898);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] - 1.1180339887498949);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { -1.5, 1.0, 1.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");

    // Voronoi region for <v3,v1>
    point = { -2.0, 0.0, 2.0 };
    output = query(point, rectangle);
    error = std::fabs(output.sqrDistance - 1.8);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(1.8));
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[0] + 2.2360679774997898);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] - 0.44721359549995798);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - point);
    UTAssert(
        error == 0.0,
        "Invalid DCPQuery.");
    expectedClosest = { -1.2, 0.4, 1.0 };
    error = Length(output.closest[1] - expectedClosest);
    UTAssert(
        error <= 1e-15,
        "Invalid DCPQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/ND/DistPointRectangle.h>

namespace gtl
{
    template class DCPQuery<float, Vector<float, 2>, Rectangle<float, 2>>;
    template class DCPQuery<double, Vector<double, 3>, Rectangle<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Vector<Rational, 4>, Rectangle<Rational, 4>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPointRectangle)
