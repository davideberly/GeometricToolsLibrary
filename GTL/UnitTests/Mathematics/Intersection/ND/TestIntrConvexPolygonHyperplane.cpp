#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/ND/IntrConvexPolygonHyperplane.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrConvexPolygonHyperplane
    {
    public:
        UnitTestIntrConvexPolygonHyperplane();

    private:
        void TestTIQuery2D();
        void TestFIQuery2D();
        void TestTIQuery3D();
        void TestFIQuery3D();
    };
}

UnitTestIntrConvexPolygonHyperplane::UnitTestIntrConvexPolygonHyperplane()
{
    UTInformation("Mathematics/Intersection/ND/IntrConvexPolygonHyperplane");

    TestTIQuery2D();
    TestFIQuery2D();
    TestTIQuery3D();
    TestFIQuery3D();
}

void UnitTestIntrConvexPolygonHyperplane::TestTIQuery2D()
{
    using TITriangleLine = TIQuery<double, std::vector<Vector<double, 2>>,
        Hyperplane<double, 2>>;
    TITriangleLine tiQuery{};
    TITriangleLine::Output tiOutput{};

    // The line is y = Dot((0,1), (x,y)) = 0.
    Hyperplane<double, 2> line({ 0.0, 1.0 }, 0.0);
    std::vector<Vector<double, 2>> triangle(3);

    // (n,p,z) = (0,3,0)
    triangle[0] = { 1.0, 1.0 };
    triangle[1] = { 2.0, 1.0 };
    triangle[2] = { 1.0, 2.0 };
    tiOutput = tiQuery(triangle, line);
    UTAssert(
        tiOutput.intersect == false &&
        tiOutput.configuration == TITriangleLine::Configuration::POSITIVE_SIDE_STRICT,
        "Invalid TIQuery.");

    // (n,p,z) = (3,0,0)
    triangle[0] = { 1.0, -1.0 };
    triangle[1] = { 2.0, -1.0 };
    triangle[2] = { 1.0, -2.0 };
    tiOutput = tiQuery(triangle, line);
    UTAssert(
        tiOutput.intersect == false &&
        tiOutput.configuration == TITriangleLine::Configuration::NEGATIVE_SIDE_STRICT,
        "Invalid TIQuery.");

    // (n,p,z) = (2,1,0)
    triangle[0] = { 1.0, -1.0 };
    triangle[1] = { 2.0, -1.0 };
    triangle[2] = { 1.5, +2.0 };
    tiOutput = tiQuery(triangle, line);
    UTAssert(
        tiOutput.intersect == true &&
        tiOutput.configuration == TITriangleLine::Configuration::SPLIT,
        "Invalid TIQuery.");

    // (n,p,z) = (1,2,0)
    triangle[0] = { 1.0, +1.0 };
    triangle[1] = { 2.0, +1.0 };
    triangle[2] = { 1.5, -2.0 };
    tiOutput = tiQuery(triangle, line);
    UTAssert(
        tiOutput.intersect == true &&
        tiOutput.configuration == TITriangleLine::Configuration::SPLIT,
        "Invalid TIQuery.");

    // (n,p,z) = (2,0,1)
    triangle[0] = { 1.0, -1.0 };
    triangle[1] = { 2.0, -1.0 };
    triangle[2] = { 1.5, 0.0 };
    tiOutput = tiQuery(triangle, line);
    UTAssert(
        tiOutput.intersect == true &&
        tiOutput.configuration == TITriangleLine::Configuration::NEGATIVE_SIDE_VERTEX,
        "Invalid TIQuery.");

    // (n,p,z) = (0,2,1)
    triangle[0] = { 1.0, +1.0 };
    triangle[1] = { 2.0, +1.0 };
    triangle[2] = { 1.5, 0.0 };
    tiOutput = tiQuery(triangle, line);
    UTAssert(
        tiOutput.intersect == true &&
        tiOutput.configuration == TITriangleLine::Configuration::POSITIVE_SIDE_VERTEX,
        "Invalid TIQuery.");

    // (n,p,z) = (1,0,2)
    triangle[0] = { 1.0, 0.0 };
    triangle[1] = { 2.0, 0.0 };
    triangle[2] = { 1.5, -1.0 };
    tiOutput = tiQuery(triangle, line);
    UTAssert(
        tiOutput.intersect == true &&
        tiOutput.configuration == TITriangleLine::Configuration::NEGATIVE_SIDE_EDGE,
        "Invalid TIQuery.");

    // (n,p,z) = (0,1,2)
    triangle[0] = { 1.0, 0.0 };
    triangle[1] = { 2.0, 0.0 };
    triangle[2] = { 1.5, +1.0 };
    tiOutput = tiQuery(triangle, line);
    UTAssert(
        tiOutput.intersect == true &&
        tiOutput.configuration == TITriangleLine::Configuration::POSITIVE_SIDE_EDGE,
        "Invalid TIQuery.");

    // In 2D, Configuration::CONTAINED can occur only when the triangle
    // is degenerate.
}

void UnitTestIntrConvexPolygonHyperplane::TestFIQuery2D()
{
    using FITriangleLine = FIQuery<double, std::vector<Vector<double, 2>>,
        Hyperplane<double, 2>>;
    FITriangleLine fiQuery{};
    FITriangleLine::Output fiOutput{};
    std::array<Vector<double, 2>, 4> expected{};
    std::array<double, 4> error{};

    // The line is y = Dot((0,1), (x,y)) = 0.
    Hyperplane<double, 2> line({ 0.0, 1.0 }, 0.0);
    std::vector<Vector<double, 2>> triangle(3);

    // (n,p,z) = (0,3,0)
    triangle[0] = { 1.0, 1.0 };
    triangle[1] = { 2.0, 1.0 };
    triangle[2] = { 1.0, 2.0 };
    fiOutput = fiQuery(triangle, line);
    UTAssert(
        fiOutput.intersect == false &&
        fiOutput.configuration == FITriangleLine::Configuration::POSITIVE_SIDE_STRICT,
        "Invalid FIQuery.");
    UTAssert(
        fiOutput.intersection.size() == 0 &&
        fiOutput.positivePolygon.size() == 3 &&
        fiOutput.negativePolygon.size() == 0,
        "Invalid FIQuery.");
    UTAssert(
        (fiOutput.positivePolygon[0] == Vector<double, 2>{ 1.0, 1.0 }) &&
        (fiOutput.positivePolygon[1] == Vector<double, 2>{ 2.0, 1.0 }) &&
        (fiOutput.positivePolygon[2] == Vector<double, 2>{ 1.0, 2.0 }),
        "Invalid FIQuery.");

    // (n,p,z) = (3,0,0)
    triangle[0] = { 1.0, -1.0 };
    triangle[1] = { 2.0, -1.0 };
    triangle[2] = { 1.0, -2.0 };
    fiOutput = fiQuery(triangle, line);
    UTAssert(
        fiOutput.intersect == false &&
        fiOutput.configuration == FITriangleLine::Configuration::NEGATIVE_SIDE_STRICT,
        "Invalid FIQuery.");
    UTAssert(
        fiOutput.intersection.size() == 0 &&
        fiOutput.positivePolygon.size() == 0 &&
        fiOutput.negativePolygon.size() == 3,
        "Invalid FIQuery.");
    UTAssert(
        (fiOutput.negativePolygon[0] == Vector<double, 2>{ 1.0, -1.0 }) &&
        (fiOutput.negativePolygon[1] == Vector<double, 2>{ 2.0, -1.0 }) &&
        (fiOutput.negativePolygon[2] == Vector<double, 2>{ 1.0, -2.0 }),
        "Invalid FIQuery.");

    // (n,p,z) = (2,1,0)
    triangle[0] = { 1.0, -1.0 };
    triangle[1] = { 2.0, -1.0 };
    triangle[2] = { 1.5, +2.0 };
    fiOutput = fiQuery(triangle, line);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.configuration == FITriangleLine::Configuration::SPLIT,
        "Invalid FIQuery.");
    UTAssert(
        fiOutput.intersection.size() == 2 &&
        fiOutput.positivePolygon.size() == 3 &&
        fiOutput.negativePolygon.size() == 4,
        "Invalid FIQuery.");
    expected[0] = { 165.0 / 90.0, 0.0 };
    expected[1] = { 105.0 / 90.0, 0.0 };
    error[0] = Length(fiOutput.intersection[0] - expected[0]);
    error[1] = Length(fiOutput.intersection[1] - expected[1]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    expected[0] = { 165.0 / 90.0, 0.0 };
    expected[1] = { 1.5, 2.0 };
    expected[2] = { 105.0 / 90.0, 0.0 };
    error[0] = Length(fiOutput.positivePolygon[0] - expected[0]);
    error[1] = Length(fiOutput.positivePolygon[1] - expected[1]);
    error[2] = Length(fiOutput.positivePolygon[2] - expected[2]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[2] <= 1e-15,
        "Invalid FIQuery.");
    expected[0] = { 105.0 / 90.0, 0.0 };
    expected[1] = { 1.0, -1.0 };
    expected[2] = { 2.0, -1.0 };
    expected[3] = { 165.0 / 90.0, 0.0 };
    error[0] = Length(fiOutput.negativePolygon[0] - expected[0]);
    error[1] = Length(fiOutput.negativePolygon[1] - expected[1]);
    error[2] = Length(fiOutput.negativePolygon[2] - expected[2]);
    error[3] = Length(fiOutput.negativePolygon[3] - expected[3]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[2] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[3] <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (1,2,0)
    triangle[0] = { 1.0, +1.0 };
    triangle[1] = { 2.0, +1.0 };
    triangle[2] = { 1.5, -2.0 };
    fiOutput = fiQuery(triangle, line);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.configuration == FITriangleLine::Configuration::SPLIT,
        "Invalid FIQuery.");
    UTAssert(
        fiOutput.intersection.size() == 2 &&
        fiOutput.positivePolygon.size() == 4 &&
        fiOutput.negativePolygon.size() == 3,
        "Invalid FIQuery.");
    expected[0] = { 165.0 / 90.0, 0.0 };
    expected[1] = { 105.0 / 90.0, 0.0 };
    error[0] = Length(fiOutput.intersection[0] - expected[0]);
    error[1] = Length(fiOutput.intersection[1] - expected[1]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    expected[0] = { 165.0 / 90.0, 0.0 };
    expected[1] = { 1.5, -2.0 };
    expected[2] = { 105.0 / 90.0, 0.0 };
    error[0] = Length(fiOutput.negativePolygon[0] - expected[0]);
    error[1] = Length(fiOutput.negativePolygon[1] - expected[1]);
    error[2] = Length(fiOutput.negativePolygon[2] - expected[2]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[2] <= 1e-15,
        "Invalid FIQuery.");
    expected[0] = { 105.0 / 90.0, 0.0 };
    expected[1] = { 1.0, 1.0 };
    expected[2] = { 2.0, 1.0 };
    expected[3] = { 165.0 / 90.0, 0.0 };
    error[0] = Length(fiOutput.positivePolygon[0] - expected[0]);
    error[1] = Length(fiOutput.positivePolygon[1] - expected[1]);
    error[2] = Length(fiOutput.positivePolygon[2] - expected[2]);
    error[3] = Length(fiOutput.positivePolygon[3] - expected[3]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[2] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[3] <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (2,0,1)
    triangle[0] = { 1.0, -1.0 };
    triangle[1] = { 2.0, -1.0 };
    triangle[2] = { 1.5, 0.0 };
    fiOutput = fiQuery(triangle, line);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.configuration == FITriangleLine::Configuration::NEGATIVE_SIDE_VERTEX,
        "Invalid FIQuery.");
    UTAssert(
        fiOutput.intersection.size() == 1 &&
        fiOutput.positivePolygon.size() == 0 &&
        fiOutput.negativePolygon.size() == 3,
        "Invalid FIQuery.");
    expected[0] = { 1.5, 0.0 };
    error[0] = Length(fiOutput.intersection[0] - expected[0]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    expected[0] = { 1.0, -1.0 };
    expected[1] = { 2.0, -1.0 };
    expected[2] = { 1.5, 0.0 };
    error[0] = Length(fiOutput.negativePolygon[0] - expected[0]);
    error[1] = Length(fiOutput.negativePolygon[1] - expected[1]);
    error[2] = Length(fiOutput.negativePolygon[2] - expected[2]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[2] <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (0,2,1)
    triangle[0] = { 1.0, +1.0 };
    triangle[1] = { 2.0, +1.0 };
    triangle[2] = { 1.5, 0.0 };
    fiOutput = fiQuery(triangle, line);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.configuration == FITriangleLine::Configuration::POSITIVE_SIDE_VERTEX,
        "Invalid FIQuery.");
    UTAssert(
        fiOutput.intersection.size() == 1 &&
        fiOutput.positivePolygon.size() == 3 &&
        fiOutput.negativePolygon.size() == 0,
        "Invalid FIQuery.");
    expected[0] = { 1.5, 0.0 };
    error[0] = Length(fiOutput.intersection[0] - expected[0]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    expected[0] = { 1.0, 1.0 };
    expected[1] = { 2.0, 1.0 };
    expected[2] = { 1.5, 0.0 };
    error[0] = Length(fiOutput.positivePolygon[0] - expected[0]);
    error[1] = Length(fiOutput.positivePolygon[1] - expected[1]);
    error[2] = Length(fiOutput.positivePolygon[2] - expected[2]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[2] <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (1,0,2)
    triangle[0] = { 1.0, 0.0 };
    triangle[1] = { 2.0, 0.0 };
    triangle[2] = { 1.5, -1.0 };
    fiOutput = fiQuery(triangle, line);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.configuration == FITriangleLine::Configuration::NEGATIVE_SIDE_EDGE,
        "Invalid FIQuery.");
    UTAssert(
        fiOutput.intersection.size() == 2 &&
        fiOutput.positivePolygon.size() == 0 &&
        fiOutput.negativePolygon.size() == 3,
        "Invalid FIQuery.");
    expected[0] = { 1.0, 0.0 };
    expected[1] = { 2.0, 0.0 };
    error[0] = Length(fiOutput.intersection[0] - expected[0]);
    error[1] = Length(fiOutput.intersection[1] - expected[1]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    expected[0] = { 1.0, 0.0 };
    expected[1] = { 2.0, 0.0 };
    expected[2] = { 1.5, -1.0 };
    error[0] = Length(fiOutput.negativePolygon[0] - expected[0]);
    error[1] = Length(fiOutput.negativePolygon[1] - expected[1]);
    error[2] = Length(fiOutput.negativePolygon[2] - expected[2]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[2] <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (0,1,2)
    triangle[0] = { 1.0, 0.0 };
    triangle[1] = { 2.0, 0.0 };
    triangle[2] = { 1.5, +1.0 };
    fiOutput = fiQuery(triangle, line);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.configuration == FITriangleLine::Configuration::POSITIVE_SIDE_EDGE,
        "Invalid FIQuery.");
    UTAssert(
        fiOutput.intersection.size() == 2 &&
        fiOutput.positivePolygon.size() == 3 &&
        fiOutput.negativePolygon.size() == 0,
        "Invalid FIQuery.");
    expected[0] = { 1.0, 0.0 };
    expected[1] = { 2.0, 0.0 };
    error[0] = Length(fiOutput.intersection[0] - expected[0]);
    error[1] = Length(fiOutput.intersection[1] - expected[1]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    expected[0] = { 1.0, 0.0 };
    expected[1] = { 2.0, 0.0 };
    expected[2] = { 1.5, 1.0 };
    error[0] = Length(fiOutput.positivePolygon[0] - expected[0]);
    error[1] = Length(fiOutput.positivePolygon[1] - expected[1]);
    error[2] = Length(fiOutput.positivePolygon[2] - expected[2]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[2] <= 1e-15,
        "Invalid FIQuery.");

    // In 2D, Configuration::CONTAINED can occur only when the triangle
    // is degenerate.
}

void UnitTestIntrConvexPolygonHyperplane::TestTIQuery3D()
{
    using TITriangleLine = TIQuery<double, std::vector<Vector<double, 3>>,
        Hyperplane<double, 3>>;
    TITriangleLine tiQuery{};
    TITriangleLine::Output tiOutput{};

    // The plane is z = Dot((0,0,1), (x,y,z)) = 0.
    Hyperplane<double, 3> plane({ 0.0, 0.0, 1.0 }, 0.0);
    std::vector<Vector<double, 3>> triangle(3);

    // (n,p,z) = (0,3,0)
    triangle[0] = { 1.0, 0.0, 1.0 };
    triangle[1] = { 2.0, 0.0, 1.0 };
    triangle[2] = { 1.0, 0.0, 2.0 };
    tiOutput = tiQuery(triangle, plane);
    UTAssert(
        tiOutput.intersect == false &&
        tiOutput.configuration == TITriangleLine::Configuration::POSITIVE_SIDE_STRICT,
        "Invalid TIQuery.");

    // (n,p,z) = (3,0,0)
    triangle[0] = { 1.0, 0.0, -1.0 };
    triangle[1] = { 2.0, 0.0, -1.0 };
    triangle[2] = { 1.0, 0.0, -2.0 };
    tiOutput = tiQuery(triangle, plane);
    UTAssert(
        tiOutput.intersect == false &&
        tiOutput.configuration == TITriangleLine::Configuration::NEGATIVE_SIDE_STRICT,
        "Invalid TIQuery.");

    // (n,p,z) = (2,1,0)
    triangle[0] = { 1.0, 0.0, -1.0 };
    triangle[1] = { 2.0, 0.0, -1.0 };
    triangle[2] = { 1.5, 0.0, +2.0 };
    tiOutput = tiQuery(triangle, plane);
    UTAssert(
        tiOutput.intersect == true &&
        tiOutput.configuration == TITriangleLine::Configuration::SPLIT,
        "Invalid TIQuery.");

    // (n,p,z) = (1,2,0)
    triangle[0] = { 1.0, 0.0, +1.0 };
    triangle[1] = { 2.0, 0.0, +1.0 };
    triangle[2] = { 1.5, 0.0, -2.0 };
    tiOutput = tiQuery(triangle, plane);
    UTAssert(
        tiOutput.intersect == true &&
        tiOutput.configuration == TITriangleLine::Configuration::SPLIT,
        "Invalid TIQuery.");

    // (n,p,z) = (2,0,1)
    triangle[0] = { 1.0, 0.0, -1.0 };
    triangle[1] = { 2.0, 0.0, -1.0 };
    triangle[2] = { 1.5, 0.0, 0.0 };
    tiOutput = tiQuery(triangle, plane);
    UTAssert(
        tiOutput.intersect == true &&
        tiOutput.configuration == TITriangleLine::Configuration::NEGATIVE_SIDE_VERTEX,
        "Invalid TIQuery.");

    // (n,p,z) = (0,2,1)
    triangle[0] = { 1.0, 0.0, +1.0 };
    triangle[1] = { 2.0, 0.0, +1.0 };
    triangle[2] = { 1.5, 0.0, 0.0 };
    tiOutput = tiQuery(triangle, plane);
    UTAssert(
        tiOutput.intersect == true &&
        tiOutput.configuration == TITriangleLine::Configuration::POSITIVE_SIDE_VERTEX,
        "Invalid TIQuery.");

    // (n,p,z) = (1,0,2)
    triangle[0] = { 1.0, 0.0, 0.0 };
    triangle[1] = { 2.0, 0.0, 0.0 };
    triangle[2] = { 1.5, 0.0, -1.0 };
    tiOutput = tiQuery(triangle, plane);
    UTAssert(
        tiOutput.intersect == true &&
        tiOutput.configuration == TITriangleLine::Configuration::NEGATIVE_SIDE_EDGE,
        "Invalid TIQuery.");

    // (n,p,z) = (0,1,2)
    triangle[0] = { 1.0, 0.0, 0.0 };
    triangle[1] = { 2.0, 0.0, 0.0 };
    triangle[2] = { 1.5, 0.0, +1.0 };
    tiOutput = tiQuery(triangle, plane);
    UTAssert(
        tiOutput.intersect == true &&
        tiOutput.configuration == TITriangleLine::Configuration::POSITIVE_SIDE_EDGE,
        "Invalid TIQuery.");

    // (n,p,z) = (0,0,3)
    triangle[0] = { 1.0, 0.0, 0.0 };
    triangle[1] = { 0.0, 1.0, 0.0 };
    triangle[2] = { 1.0, 1.0, 0.0 };
    tiOutput = tiQuery(triangle, plane);
    UTAssert(
        tiOutput.intersect == true &&
        tiOutput.configuration == TITriangleLine::Configuration::CONTAINED,
        "Invalid TIQuery.");
}

void UnitTestIntrConvexPolygonHyperplane::TestFIQuery3D()
{
    using FITriangleLine = FIQuery<double, std::vector<Vector<double, 3>>,
        Hyperplane<double, 3>>;
    FITriangleLine fiQuery{};
    FITriangleLine::Output fiOutput{};
    std::array<Vector<double, 3>, 4> expected{};
    std::array<double, 4> error{};

    // The plane is z = Dot((0,0,1), (x,y,z)) = 0.
    Hyperplane<double, 3> plane({ 0.0, 0.0, 1.0 }, 0.0);
    std::vector<Vector<double, 3>> triangle(3);

    // (n,p,z) = (0,3,0)
    triangle[0] = { 1.0, 0.0, 1.0 };
    triangle[1] = { 2.0, 0.0, 1.0 };
    triangle[2] = { 1.0, 0.0, 2.0 };
    fiOutput = fiQuery(triangle, plane);
    UTAssert(
        fiOutput.intersect == false &&
        fiOutput.configuration == FITriangleLine::Configuration::POSITIVE_SIDE_STRICT,
        "Invalid FIQuery.");
    UTAssert(
        fiOutput.intersection.size() == 0 &&
        fiOutput.positivePolygon.size() == 3 &&
        fiOutput.negativePolygon.size() == 0,
        "Invalid FIQuery.");
    expected[0] = { 1.0, 0.0, 1.0 };
    expected[1] = { 2.0, 0.0, 1.0 };
    expected[2] = { 1.0, 0.0, 2.0 };
    error[0] = Length(fiOutput.positivePolygon[0] - expected[0]);
    error[1] = Length(fiOutput.positivePolygon[1] - expected[1]);
    error[2] = Length(fiOutput.positivePolygon[2] - expected[2]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[2] <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (3,0,0)
    triangle[0] = { 1.0, 0.0, -1.0 };
    triangle[1] = { 2.0, 0.0, -1.0 };
    triangle[2] = { 1.0, 0.0, -2.0 };
    fiOutput = fiQuery(triangle, plane);
    UTAssert(
        fiOutput.intersect == false &&
        fiOutput.configuration == FITriangleLine::Configuration::NEGATIVE_SIDE_STRICT,
        "Invalid FIQuery.");
    UTAssert(
        fiOutput.intersection.size() == 0 &&
        fiOutput.positivePolygon.size() == 0 &&
        fiOutput.negativePolygon.size() == 3,
        "Invalid FIQuery.");
    expected[0] = { 1.0, 0.0, -1.0 };
    expected[1] = { 2.0, 0.0, -1.0 };
    expected[2] = { 1.0, 0.0, -2.0 };
    error[0] = Length(fiOutput.negativePolygon[0] - expected[0]);
    error[1] = Length(fiOutput.negativePolygon[1] - expected[1]);
    error[2] = Length(fiOutput.negativePolygon[2] - expected[2]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[2] <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (2,1,0)
    triangle[0] = { 1.0, 0.0, -1.0 };
    triangle[1] = { 2.0, 0.0, -1.0 };
    triangle[2] = { 1.5, 0.0, +2.0 };
    fiOutput = fiQuery(triangle, plane);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.configuration == FITriangleLine::Configuration::SPLIT,
        "Invalid FIQuery.");
    UTAssert(
        fiOutput.intersection.size() == 2 &&
        fiOutput.positivePolygon.size() == 3 &&
        fiOutput.negativePolygon.size() == 4,
        "Invalid FIQuery.");
    expected[0] = { 165.0 / 90.0, 0.0, 0.0 };
    expected[1] = { 105.0 / 90.0, 0.0, 0.0 };
    error[0] = Length(fiOutput.intersection[0] - expected[0]);
    error[1] = Length(fiOutput.intersection[1] - expected[1]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    expected[0] = { 165.0 / 90.0, 0.0, 0.0 };
    expected[1] = { 1.5, 0.0, 2.0 };
    expected[2] = { 105.0 / 90.0, 0.0, 0.0 };
    error[0] = Length(fiOutput.positivePolygon[0] - expected[0]);
    error[1] = Length(fiOutput.positivePolygon[1] - expected[1]);
    error[2] = Length(fiOutput.positivePolygon[2] - expected[2]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[2] <= 1e-15,
        "Invalid FIQuery.");
    expected[0] = { 105.0 / 90.0, 0.0, 0.0 };
    expected[1] = { 1.0, 0.0, -1.0 };
    expected[2] = { 2.0, 0.0, -1.0 };
    expected[3] = { 165.0 / 90.0, 0.0, 0.0 };
    error[0] = Length(fiOutput.negativePolygon[0] - expected[0]);
    error[1] = Length(fiOutput.negativePolygon[1] - expected[1]);
    error[2] = Length(fiOutput.negativePolygon[2] - expected[2]);
    error[3] = Length(fiOutput.negativePolygon[3] - expected[3]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[2] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[3] <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (1,2,0)
    triangle[0] = { 1.0, 0.0, +1.0 };
    triangle[1] = { 2.0, 0.0, +1.0 };
    triangle[2] = { 1.5, 0.0, -2.0 };
    fiOutput = fiQuery(triangle, plane);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.configuration == FITriangleLine::Configuration::SPLIT,
        "Invalid FIQuery.");
    UTAssert(
        fiOutput.intersection.size() == 2 &&
        fiOutput.positivePolygon.size() == 4 &&
        fiOutput.negativePolygon.size() == 3,
        "Invalid FIQuery.");
    expected[0] = { 165.0 / 90.0, 0.0, 0.0 };
    expected[1] = { 105.0 / 90.0, 0.0, 0.0 };
    error[0] = Length(fiOutput.intersection[0] - expected[0]);
    error[1] = Length(fiOutput.intersection[1] - expected[1]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    expected[0] = { 165.0 / 90.0, 0.0, 0.0 };
    expected[1] = { 1.5, 0.0, -2.0 };
    expected[2] = { 105.0 / 90.0, 0.0, 0.0 };
    error[0] = Length(fiOutput.negativePolygon[0] - expected[0]);
    error[1] = Length(fiOutput.negativePolygon[1] - expected[1]);
    error[2] = Length(fiOutput.negativePolygon[2] - expected[2]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[2] <= 1e-15,
        "Invalid FIQuery.");
    expected[0] = { 105.0 / 90.0, 0.0, 0.0 };
    expected[1] = { 1.0, 0.0, 1.0 };
    expected[2] = { 2.0, 0.0, 1.0 };
    expected[3] = { 165.0 / 90.0, 0.0, 0.0 };
    error[0] = Length(fiOutput.positivePolygon[0] - expected[0]);
    error[1] = Length(fiOutput.positivePolygon[1] - expected[1]);
    error[2] = Length(fiOutput.positivePolygon[2] - expected[2]);
    error[3] = Length(fiOutput.positivePolygon[3] - expected[3]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[2] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[3] <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (2,0,1)
    triangle[0] = { 1.0, 0.0, -1.0 };
    triangle[1] = { 2.0, 0.0, -1.0 };
    triangle[2] = { 1.5, 0.0, 0.0 };
    fiOutput = fiQuery(triangle, plane);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.configuration == FITriangleLine::Configuration::NEGATIVE_SIDE_VERTEX,
        "Invalid FIQuery.");
    UTAssert(
        fiOutput.intersection.size() == 1 &&
        fiOutput.positivePolygon.size() == 0 &&
        fiOutput.negativePolygon.size() == 3,
        "Invalid FIQuery.");
    expected[0] = { 1.5, 0.0, 0.0 };
    error[0] = Length(fiOutput.intersection[0] - expected[0]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    expected[0] = { 1.0, 0.0, -1.0 };
    expected[1] = { 2.0, 0.0, -1.0 };
    expected[2] = { 1.5, 0.0, 0.0 };
    error[0] = Length(fiOutput.negativePolygon[0] - expected[0]);
    error[1] = Length(fiOutput.negativePolygon[1] - expected[1]);
    error[2] = Length(fiOutput.negativePolygon[2] - expected[2]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[2] <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (0,2,1)
    triangle[0] = { 1.0, 0.0, +1.0 };
    triangle[1] = { 2.0, 0.0, +1.0 };
    triangle[2] = { 1.5, 0.0, 0.0 };
    fiOutput = fiQuery(triangle, plane);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.configuration == FITriangleLine::Configuration::POSITIVE_SIDE_VERTEX,
        "Invalid FIQuery.");
    UTAssert(
        fiOutput.intersection.size() == 1 &&
        fiOutput.positivePolygon.size() == 3 &&
        fiOutput.negativePolygon.size() == 0,
        "Invalid FIQuery.");
    expected[0] = { 1.5, 0.0, 0.0 };
    error[0] = Length(fiOutput.intersection[0] - expected[0]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    expected[0] = { 1.0, 0.0, 1.0 };
    expected[1] = { 2.0, 0.0, 1.0 };
    expected[2] = { 1.5, 0.0, 0.0 };
    error[0] = Length(fiOutput.positivePolygon[0] - expected[0]);
    error[1] = Length(fiOutput.positivePolygon[1] - expected[1]);
    error[2] = Length(fiOutput.positivePolygon[2] - expected[2]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[2] <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (1,0,2)
    triangle[0] = { 1.0, 0.0, 0.0 };
    triangle[1] = { 2.0, 0.0, 0.0 };
    triangle[2] = { 1.5, 0.0, -1.0 };
    fiOutput = fiQuery(triangle, plane);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.configuration == FITriangleLine::Configuration::NEGATIVE_SIDE_EDGE,
        "Invalid FIQuery.");
    UTAssert(
        fiOutput.intersection.size() == 2 &&
        fiOutput.positivePolygon.size() == 0 &&
        fiOutput.negativePolygon.size() == 3,
        "Invalid FIQuery.");
    expected[0] = { 1.0, 0.0, 0.0 };
    expected[1] = { 2.0, 0.0, 0.0 };
    error[0] = Length(fiOutput.intersection[0] - expected[0]);
    error[1] = Length(fiOutput.intersection[1] - expected[1]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    expected[0] = { 1.0, 0.0, 0.0 };
    expected[1] = { 2.0, 0.0, 0.0 };
    expected[2] = { 1.5, 0.0, -1.0 };
    error[0] = Length(fiOutput.negativePolygon[0] - expected[0]);
    error[1] = Length(fiOutput.negativePolygon[1] - expected[1]);
    error[2] = Length(fiOutput.negativePolygon[2] - expected[2]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[2] <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (0,1,2)
    triangle[0] = { 1.0, 0.0, 0.0 };
    triangle[1] = { 2.0, 0.0, 0.0 };
    triangle[2] = { 1.5, 0.0, +1.0 };
    fiOutput = fiQuery(triangle, plane);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.configuration == FITriangleLine::Configuration::POSITIVE_SIDE_EDGE,
        "Invalid FIQuery.");
    UTAssert(
        fiOutput.intersection.size() == 2 &&
        fiOutput.positivePolygon.size() == 3 &&
        fiOutput.negativePolygon.size() == 0,
        "Invalid FIQuery.");
    expected[0] = { 1.0, 0.0, 0.0 };
    expected[1] = { 2.0, 0.0, 0.0 };
    error[0] = Length(fiOutput.intersection[0] - expected[0]);
    error[1] = Length(fiOutput.intersection[1] - expected[1]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    expected[0] = { 1.0, 0.0, 0.0 };
    expected[1] = { 2.0, 0.0, 0.0 };
    expected[2] = { 1.5, 0.0, 1.0 };
    error[0] = Length(fiOutput.positivePolygon[0] - expected[0]);
    error[1] = Length(fiOutput.positivePolygon[1] - expected[1]);
    error[2] = Length(fiOutput.positivePolygon[2] - expected[2]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[2] <= 1e-15,
        "Invalid FIQuery.");

    // (n,p,z) = (0,0,3)
    triangle[0] = { 1.0, 0.0, 0.0 };
    triangle[1] = { 0.0, 1.0, 0.0 };
    triangle[2] = { 1.0, 1.0, 0.0 };
    fiOutput = fiQuery(triangle, plane);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.configuration == FITriangleLine::Configuration::CONTAINED,
        "Invalid FIQuery.");
    UTAssert(
        fiOutput.intersection.size() == 3 &&
        fiOutput.positivePolygon.size() == 0 &&
        fiOutput.negativePolygon.size() == 0,
        "Invalid FIQuery.");
    expected[0] = { 1.0, 0.0, 0.0 };
    expected[1] = { 0.0, 1.0, 0.0 };
    expected[2] = { 1.0, 1.0, 0.0 };
    error[0] = Length(fiOutput.intersection[0] - expected[0]);
    error[1] = Length(fiOutput.intersection[1] - expected[1]);
    error[2] = Length(fiOutput.intersection[2] - expected[2]);
    UTAssert(
        error[0] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[1] <= 1e-15,
        "Invalid FIQuery.");
    UTAssert(
        error[2] <= 1e-15,
        "Invalid FIQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/ND/IntrConvexPolygonHyperplane.h>

namespace gtl
{
    template class TIQuery<float, std::vector<Vector<float, 2>>, Hyperplane<float, 2>>;
    template class FIQuery<float, std::vector<Vector<float, 2>>, Hyperplane<float, 2>>;

    template class TIQuery<double, std::vector<Vector<double, 3>>, Hyperplane<double, 3>>;
    template class FIQuery<double, std::vector<Vector<double, 3>>, Hyperplane<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, std::vector<Vector<Rational, 4>>, Hyperplane<Rational, 4>>;
    template class FIQuery<Rational, std::vector<Vector<Rational, 4>>, Hyperplane<Rational, 4>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrConvexPolygonHyperplane)
