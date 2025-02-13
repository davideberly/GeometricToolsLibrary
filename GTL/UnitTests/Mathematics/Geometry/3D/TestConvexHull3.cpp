#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/3D/ConvexHull3.h>
#include <GTL/Utility/Timer.h>
#include <fstream>
using namespace gtl;

namespace gtl
{
    template class ConvexHull3<float>;

    class UnitTestConvexHull3
    {
    public:
        UnitTestConvexHull3();

    private:
        void TestFull();                // large set of points
        void TestTetrahedron();
        void Test1Point();              // subhulls point-empty
        void Test2Points();             // subhulls point-point
        void Test3ColinearPoints();     // subhulls point-segment
        void Test3NoncolinearPoints();  // subhulls point-segment
        void Test4NoncolinearPoints();  // subhulls segment-segment
    };
}

UnitTestConvexHull3::UnitTestConvexHull3()
{
    // Also tested by GTL/VisualTests/Geometry/ConvexHull3D.
    UTInformation("Mathematics/Geometry/3D/ConvexHull3");

    TestFull();
    TestTetrahedron();
    Test1Point();
    Test2Points();
    Test3ColinearPoints();
    Test3NoncolinearPoints();
    Test4NoncolinearPoints();
}

void UnitTestConvexHull3::TestFull()
{
    std::size_t const numPoints = 121975;  // 121880 unique points
    std::vector<Vector3<float>> points(numPoints);
    std::ifstream input("Mathematics/Geometry/3D/Spray3.txt");
    for (std::size_t i = 0; i < numPoints; ++i)
    {
        input >> points[i][0] >> points[i][1] >> points[i][2];
        float dummy;
        input >> dummy;
        input >> dummy;
    }
    input.close();

    ConvexHull3<float> ch;
    std::vector<std::size_t> vertices;
    std::vector<std::size_t> hull;
    Timer timer;
    std::int64_t msecs = 0;

    timer.Reset();
    ch(points.size(), points.data(), 0);
    msecs = timer.GetMilliseconds();
    vertices = ch.GetVertices();
    hull = ch.GetHull();
    std::size_t const numTriangles = hull.size() / 3;
    std::cout << "Main thread" << std::endl;
    std::cout << "time = " << msecs << " milliseconds" << std::endl;
    std::cout << "points = " << vertices.size() << std::endl;
    std::cout << "triangles = " << numTriangles << std::endl << std::endl;
    // time = 1.871000 seconds, V = 9640, T = 19276
    UTAssert(vertices.size() == 9640,
        "Incorrect number of vertices (" + std::to_string(vertices.size()) + ", should be 9640.");
    UTAssert(numTriangles == 19276,
        "Incorrect number of triangles (" + std::to_string(numTriangles) + ", should be 19276.");

    std::vector<TriangleKey<true>> triangles(numTriangles);
    for (std::size_t t = 0; t < numTriangles; ++t)
    {
        TriangleKey<true> tkey;
        tkey[0] = hull[3 * t];
        tkey[1] = hull[3 * t + 1];
        tkey[2] = hull[3 * t + 2];
        triangles[t] = tkey;
    }
    std::sort(triangles.begin(), triangles.end());

    input.open("Mathematics/Geometry/3D/Spray3ExpectedHull.txt");
    std::vector<TriangleKey<true>> expectedTriangles(numTriangles);
    for (std::size_t t = 0; t < numTriangles; ++t)
    {
        std::size_t v0, v1, v2;
        input >> v0 >> v1 >> v2;
        expectedTriangles[t] = TriangleKey<true>(v0, v1, v2);
    }
    input.close();

    for (std::size_t t = 0; t < numTriangles; ++t)
    {
        UTAssert(triangles[t] == expectedTriangles[t], "Incorrect triangle at t = " + std::to_string(t));
    }

    timer.Reset();
    ch(points, 1);
    msecs = timer.GetMilliseconds();
    vertices = ch.GetVertices();
    hull = ch.GetHull();
    std::cout << "2 threads" << std::endl;
    std::cout << "time = " << msecs << " milliseconds" << std::endl;
    std::cout << "points = " << vertices.size() << std::endl;
    std::cout << "triangles = " << hull.size() / 3 << std::endl << std::endl;
    // time = 1.218760 seconds, V = 9640, T = 19276

    timer.Reset();
    ch(points, 2);
    msecs = timer.GetMilliseconds();
    vertices = ch.GetVertices();
    hull = ch.GetHull();
    std::cout << "4 threads" << std::endl;
    std::cout << "time = " << msecs << " milliseconds" << std::endl;
    std::cout << "points = " << vertices.size() << std::endl;
    std::cout << "triangles = " << hull.size() / 3 << std::endl << std::endl;
    // time = 0.963599 seconds, V = 9637, T = 19270

    timer.Reset();
    ch(points, 3);
    msecs = timer.GetMilliseconds();
    vertices = ch.GetVertices();
    hull = ch.GetHull();
    std::cout << "8 threads" << std::endl;
    std::cout << "time = " << msecs << " milliseconds" << std::endl;
    std::cout << "points = " << vertices.size() << std::endl;
    std::cout << "triangles = " << hull.size() / 3 << std::endl << std::endl;
    // time = 0.867018 seconds, V = 9637, T = 19270
}

void UnitTestConvexHull3::TestTetrahedron()
{
    std::vector<Vector3<float>> points(4);
    ConvexHull3<float> ch;

    points[0] = { 0.0f, 0.0f, 0.0f };
    points[1] = { 1.0f, 0.0f, 0.0f };
    points[2] = { 0.0f, 1.0f, 0.0f };
    points[3] = { 1.0f, 1.0f, -1.0f };
    ch(points, 0);
    // (0,3,2), (0,2,1), (0,1,3), (1,2,3)

    points[0] = { 0.0f, 0.0f, 0.0f };
    points[1] = { 1.0f, 0.0f, 0.0f };
    points[2] = { 0.0f, 1.0f, 0.0f };
    points[3] = { 0.0f, 0.0f, 1.0f };
    ch(points, 0);
    // (0,3,2), (0,2,1), (0,1,3), (1,2,3)

    points[0] = { 0.0f, 0.0f, 0.0f };
    points[2] = { 1.0f, 0.0f, 0.0f };
    points[1] = { 0.0f, 1.0f, 0.0f };
    points[3] = { 0.0f, 0.0f, 1.0f };
    ch(points, 0);
    // (0,3,1), (0,1,2), (0,2,3), (1,3,2)
}

void UnitTestConvexHull3::Test1Point()
{
    std::vector<Vector2<float>> points(1);
    points[0] = { 256.0f, 512.0f };

    ConvexHull2<float> ch2;
    ch2(points);
    std::vector<std::size_t> hull = ch2.GetHull();
    UTAssert(ch2.GetDimension() == 0, "Invalid dimension.");
    UTAssert(hull.size() == 1, "Incorrect hull size.");
    UTAssert(hull[0] == 0, "Incorrect hull index.");
}

void UnitTestConvexHull3::Test2Points()
{
    std::vector<Vector2<float>> points(2);
    points[0] = { 256.0f, 512.0f };
    points[1] = { 786.0f, 512.0f };

    ConvexHull2<float> ch2;
    ch2(points);
    std::vector<std::size_t> hull = ch2.GetHull();
    UTAssert(ch2.GetDimension() == 1, "Invalid dimension.");
    UTAssert(hull.size() == 2, "Incorrect hull size.");
    UTAssert(hull[0] == 0 && hull[1] == 1, "Incorrect hull index.");
}

void UnitTestConvexHull3::Test3ColinearPoints()
{
    std::vector<Vector2<float>> points(3);
    points[0] = { 256.0f, 512.0f };
    points[1] = { 512.0f, 512.0f };
    points[2] = { 786.0f, 512.0f };

    ConvexHull2<float> ch2;
    ch2(points);
    std::vector<std::size_t> hull = ch2.GetHull();
    UTAssert(ch2.GetDimension() == 1, "Invalid dimension.");
    UTAssert(hull.size() == 2, "Incorrect hull size.");
    UTAssert(hull[0] == 0 && hull[1] == 2, "Incorrect hull index.");
}

void UnitTestConvexHull3::Test3NoncolinearPoints()
{
    std::vector<Vector2<float>> points(3);
    points[0] = { 256.0f, 512.0f };
    points[1] = { 512.0f, 786.0f };
    points[2] = { 786.0f, 512.0f };

    ConvexHull2<float> ch2;
    ch2(points.size(), points.data());
    std::vector<std::size_t> hull = ch2.GetHull();
    UTAssert(ch2.GetDimension() == 2, "Invalid dimension.");
    UTAssert(hull.size() == 3, "Incorrect hull size.");
    UTAssert(hull[0] == 1 && hull[1] == 0 && hull[2] == 2, "Incorrect hull index.");
}

void UnitTestConvexHull3::Test4NoncolinearPoints()
{
    std::vector<Vector2<float>> points(4);
    points[0] = { 256.0f, 512.0f };
    points[1] = { 512.0f, 512.0f };
    points[2] = { 512.0f, 786.0f };
    points[3] = { 786.0f, 640.0f };

    ConvexHull2<float> ch2;
    ch2(points);
    std::vector<std::size_t> hull = ch2.GetHull();
    UTAssert(ch2.GetDimension() == 2, "Invalid dimension.");
    UTAssert(hull.size() == 4, "Incorrect hull size.");
    UTAssert(hull[0] == 0 && hull[1] == 1 && hull[2] == 3 && hull[3] == 2, "Incorrect hull index.");
}

#else

#include <GTL/Mathematics/Geometry/3D/ConvexHull3.h>

namespace gtl
{
    template class ConvexHull3<float>;
    template class ConvexHull3<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ConvexHull3)
