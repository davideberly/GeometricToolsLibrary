#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/2D/IntrLine2Triangle2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrLine2Triangle2
    {
    public:
        UnitTestIntrLine2Triangle2();

    private:
        void Test0(std::size_t i0, std::size_t i1, std::size_t i2);
        void Test1(std::size_t i0, std::size_t i1, std::size_t i2);
        void TestTwoPointRepresentation(std::size_t i0, std::size_t i1, std::size_t i2);
    };
}

UnitTestIntrLine2Triangle2::UnitTestIntrLine2Triangle2()
{
    UTInformation("Mathematics/Intersection/2D/IntrLine2Triangle2");

    Test0(0, 1, 2);
    Test0(0, 2, 1);
    Test0(1, 0, 2);
    Test0(1, 2, 0);
    Test0(2, 0, 1);
    Test0(2, 1, 0);
    Test1(0, 1, 2);
    Test1(0, 2, 1);
    Test1(1, 0, 2);
    Test1(1, 2, 0);
    Test1(2, 0, 1);
    Test1(2, 1, 0);

    TestTwoPointRepresentation(0, 1, 2);
}

void UnitTestIntrLine2Triangle2::Test0(std::size_t i0, std::size_t i1, std::size_t i2)
{
    Line2<double> line;
    Triangle2<double> triangle;

    using LTTestQuery = TIQuery<double, Line2<double>, Triangle2<double>>;
    using LTTestOutput = LTTestQuery::Output;
    LTTestQuery tiQuery{};
    LTTestOutput tiOutput{};

    using LTFindQuery = FIQuery<double, Line2<double>, Triangle2<double>>;
    using LTFindOutput = LTFindQuery::Output;
    LTFindQuery fiQuery{};
    LTFindOutput fiOutput{};

    // (n,p,z) = (0,3,0)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 2.0 };
    triangle.v[i2] = { 0.0, 1.0 };
    line.direction = { 1.0, 0.0 };
    line.origin = { -1.0, -1.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == false, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == false, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 0, "FIQuery failed.");

    // (n,p,z) = (0,2,1)
    triangle.v[i0] = { 1.0, 1.0 };
    triangle.v[i1] = { 3.0, 3.0 };
    triangle.v[i2] = { 1.0, 2.0 };
    line.direction = { 1.0, 0.0 };
    line.origin = { -1.0, 1.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 1, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 2.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 2.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");

    // (n,p,z) = (0,1,2)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 0.0 };
    triangle.v[i2] = { 0.0, 4.0 };
    line.origin = { -1.0, 0.0 };
    line.direction = { 1.0, 0.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 1.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 3.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.0, 0.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 2.0, 0.0 }), "FIQuery failed.");

    // (n,p,z) = (0,0,3)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 1.0, 0.0 };
    triangle.v[i2] = { 1.0, 0.0 };
    line.origin = { -1.0, 0.0 };
    line.direction = { 1.0, 0.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == false, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == false, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 0, "FIQuery failed.");

    // (n,p,z) = (1,2,0)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 2.0 };
    triangle.v[i2] = { 0.0, 1.0 };
    line.direction = { 1.0, 0.0 };
    line.origin = { -1.0, 0.5 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 1.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.5, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.0, 0.5 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 0.5, 0.5 }), "FIQuery failed.");

    // (n,p,z) = (1,1,1)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 1.0 };
    triangle.v[i2] = { 0.0, 2.0 };
    line.direction = { 1.0, 0.0 };
    line.origin = { -1.0, 1.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 1.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 3.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 2.0, 1.0 }), "FIQuery failed.");

    // (n,p,z) = (0,1,2)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 4.0 };
    triangle.v[i2] = { 0.0, 4.0 };
    line.origin = { -1.0, 4.0 };
    line.direction = { 1.0, 0.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 1.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 3.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.0, 4.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 2.0, 4.0 }), "FIQuery failed.");

    // (n,p,z) = (2,1,0)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 2.0 };
    triangle.v[i2] = { 0.0, 1.0 };
    line.direction = { 1.0, 0.0 };
    line.origin = { -1.0, 1.5 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 2.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 2.5, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 1.0, 1.5 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 1.5, 1.5 }), "FIQuery failed.");

    // (n,p,z) = (2,0,1)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 2.0 };
    triangle.v[i2] = { 0.0, 1.0 };
    line.direction = { 1.0, 0.0 };
    line.origin = { -1.0, 2.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 1, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 3.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 3.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 2.0, 2.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 2.0, 2.0 }), "FIQuery failed.");

    // (n,p,z) = (3,0,0)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 2.0 };
    triangle.v[i2] = { 0.0, 1.0 };
    line.direction = { 1.0, 0.0 };
    line.origin = { -1.0, 4.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == false, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == false, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 0, "FIQuery failed.");
}

void UnitTestIntrLine2Triangle2::Test1(std::size_t i0, std::size_t i1, std::size_t i2)
{
    Line2<double> line;
    Triangle2<double> triangle;

    using LTTestQuery = TIQuery<double, Line2<double>, Triangle2<double>>;
    using LTTestOutput = LTTestQuery::Output;
    LTTestQuery tiQuery{};
    LTTestOutput tiOutput{};

    using LTFindQuery = FIQuery<double, Line2<double>, Triangle2<double>>;
    using LTFindOutput = LTFindQuery::Output;
    LTFindQuery fiQuery{};
    LTFindOutput fiOutput{};

    // (n,p,z) = (0,3,0)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 2.0 };
    triangle.v[i2] = { 0.0, 1.0 };
    line.direction = { 1.0, 0.0 };
    line.direction = -line.direction;
    line.origin = { -1.0, -1.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == false, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == false, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 0, "FIQuery failed.");

    // (n,p,z) = (0,2,1)
    triangle.v[i0] = { 1.0, 1.0 };
    triangle.v[i1] = { 3.0, 3.0 };
    triangle.v[i2] = { 1.0, 2.0 };
    line.direction = { 1.0, 0.0 };
    line.direction = -line.direction;
    line.origin = { -1.0, 1.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 1, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == -2.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == -2.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");

    // (n,p,z) = (0,1,2)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 0.0 };
    triangle.v[i2] = { 0.0, 4.0 };
    line.origin = { -1.0, 0.0 };
    line.direction = { 1.0, 0.0 };
    line.direction = -line.direction;
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == -3.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == -1.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 2.0, 0.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 0.0, 0.0 }), "FIQuery failed.");

    // (n,p,z) = (0,0,3)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 1.0, 0.0 };
    triangle.v[i2] = { 1.0, 0.0 };
    line.origin = { -1.0, 0.0 };
    line.direction = { 1.0, 0.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == false, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == false, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 0, "FIQuery failed.");

    // (n,p,z) = (1,2,0)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 2.0 };
    triangle.v[i2] = { 0.0, 1.0 };
    line.direction = { 1.0, 0.0 };
    line.direction = -line.direction;
    line.origin = { -1.0, 0.5 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == -1.5, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == -1.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.5, 0.5 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 0.0, 0.5 }), "FIQuery failed.");

    // (n,p,z) = (1,1,1)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 1.0 };
    triangle.v[i2] = { 0.0, 2.0 };
    line.direction = { 1.0, 0.0 };
    line.direction = -line.direction;
    line.origin = { -1.0, 1.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == -3.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == -1.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 2.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");

    // (n,p,z) = (0,1,2)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 4.0 };
    triangle.v[i2] = { 0.0, 4.0 };
    line.origin = { -1.0, 4.0 };
    line.direction = { 1.0, 0.0 };
    line.direction = -line.direction;
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == -3.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == -1.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 2.0, 4.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 0.0, 4.0 }), "FIQuery failed.");

    // (n,p,z) = (2,1,0)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 2.0 };
    triangle.v[i2] = { 0.0, 1.0 };
    line.direction = { 1.0, 0.0 };
    line.direction = -line.direction;
    line.origin = { -1.0, 1.5 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == -2.5, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == -2.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 1.5, 1.5 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 1.0, 1.5 }), "FIQuery failed.");

    // (n,p,z) = (2,0,1)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 2.0 };
    triangle.v[i2] = { 0.0, 1.0 };
    line.direction = { 1.0, 0.0 };
    line.direction = -line.direction;
    line.origin = { -1.0, 2.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 1, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == -3.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == -3.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 2.0, 2.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 2.0, 2.0 }), "FIQuery failed.");

    // (n,p,z) = (3,0,0)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 2.0 };
    triangle.v[i2] = { 0.0, 1.0 };
    line.direction = { 1.0, 0.0 };
    line.direction = -line.direction;
    line.origin = { -1.0, 4.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == false, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == false, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 0, "FIQuery failed.");
}

void UnitTestIntrLine2Triangle2::TestTwoPointRepresentation(std::size_t i0, std::size_t i1, std::size_t i2)
{
    Line2<double> line;
    Vector2<double> p0, p1;
    Triangle2<double> triangle;

    using LTTestQuery = TIQuery<double, Line2<double>, Triangle2<double>>;
    using LTTestOutput = LTTestQuery::Output;
    LTTestQuery tiQuery{};
    LTTestOutput tiOutput{};

    using LTFindQuery = FIQuery<double, Line2<double>, Triangle2<double>>;
    using LTFindOutput = LTFindQuery::Output;
    LTFindQuery fiQuery{};
    LTFindOutput fiOutput{};

    // (n,p,z) = (0,3,0)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 2.0 };
    triangle.v[i2] = { 0.0, 1.0 };
    line.direction = { 2.0, 0.0 };
    line.origin = { -1.0, -1.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == false, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == false, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 0, "FIQuery failed.");

    // (n,p,z) = (0,2,1)
    triangle.v[i0] = { 1.0, 1.0 };
    triangle.v[i1] = { 3.0, 3.0 };
    triangle.v[i2] = { 1.0, 2.0 };
    line.direction = { 2.0, 0.0 };
    line.origin = { -1.0, 1.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 1, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 1.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");

    // (n,p,z) = (0,1,2)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 0.0 };
    triangle.v[i2] = { 0.0, 4.0 };
    line.origin = { -1.0, 0.0 };
    line.direction = { 2.0, 0.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.5, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.5, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.0, 0.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 2.0, 0.0 }), "FIQuery failed.");

    // (n,p,z) = (0,0,3)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 1.0, 0.0 };
    triangle.v[i2] = { 1.0, 0.0 };
    line.origin = { -1.0, 0.0 };
    line.direction = { 2.0, 0.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == false, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == false, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 0, "FIQuery failed.");

    // (n,p,z) = (1,2,0)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 2.0 };
    triangle.v[i2] = { 0.0, 1.0 };
    line.direction = { 2.0, 0.0 };
    line.origin = { -1.0, 0.5 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.5, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 0.75, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.0, 0.5 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 0.5, 0.5 }), "FIQuery failed.");

    // (n,p,z) = (1,1,1)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 1.0 };
    triangle.v[i2] = { 0.0, 2.0 };
    line.direction = { 2.0, 0.0 };
    line.origin = { -1.0, 1.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.5, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.5, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 2.0, 1.0 }), "FIQuery failed.");

    // (n,p,z) = (0,1,2)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 4.0 };
    triangle.v[i2] = { 0.0, 4.0 };
    line.origin = { -1.0, 4.0 };
    line.direction = { 2.0, 0.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.5, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.5, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.0, 4.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 2.0, 4.0 }), "FIQuery failed.");

    // (n,p,z) = (2,1,0)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 2.0 };
    triangle.v[i2] = { 0.0, 1.0 };
    line.direction = { 2.0, 0.0 };
    line.origin = { -1.0, 1.5 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 1.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.25, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 1.0, 1.5 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 1.5, 1.5 }), "FIQuery failed.");

    // (n,p,z) = (2,0,1)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 2.0 };
    triangle.v[i2] = { 0.0, 1.0 };
    line.direction = { 2.0, 0.0 };
    line.origin = { -1.0, 2.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 1, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 1.5, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.5, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 2.0, 2.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 2.0, 2.0 }), "FIQuery failed.");

    // (n,p,z) = (3,0,0)
    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 2.0 };
    triangle.v[i2] = { 0.0, 1.0 };
    line.direction = { 2.0, 0.0 };
    line.origin = { -1.0, 4.0 };
    tiOutput = tiQuery(line, triangle);
    UTAssert(tiOutput.intersect == false, "TIQuery failed.");
    fiOutput = fiQuery(line, triangle);
    UTAssert(fiOutput.intersect == false, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 0, "FIQuery failed.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/2D/IntrLine2Triangle2.h>

namespace gtl
{
    template class TIQuery<float, Line2<float>, Triangle2<float>>;
    template class FIQuery<float, Line2<float>, Triangle2<float>>;

    template class TIQuery<double, Line2<double>, Triangle2<double>>;
    template class FIQuery<double, Line2<double>, Triangle2<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Line2<Rational>, Triangle2<Rational>>;
    template class FIQuery<Rational, Line2<Rational>, Triangle2<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrLine2Triangle2)
