#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/2D/ConvexHull2.h>
#include <fstream>
using namespace gtl;

namespace gtl
{
    template class ConvexHull2<float>;

    class UnitTestConvexHull2
    {
    public:
        UnitTestConvexHull2();

    private:
        void TestRandom();
        void Test1Point();              // subhulls point-empty
        void Test2Points();             // subhulls point-point
        void Test3ColinearPoints();     // subhulls point-segment
        void Test3NoncolinearPoints();  // subhulls point-segment
        void Test4NoncolinearPoints();  // subhulls segment-segment
    };
}

UnitTestConvexHull2::UnitTestConvexHull2()
{
    // Also tested by GTL/VisualTests/Geometry/ConvexHull2D.
    UTInformation("Mathematics/Geometry/2D/ConvexHull2");

    TestRandom();
    Test1Point();
    Test2Points();
    Test3ColinearPoints();
    Test3NoncolinearPoints();
    Test4NoncolinearPoints();
}

void UnitTestConvexHull2::TestRandom()
{
    std::ifstream input(
        "Mathematics/Geometry/2D/Support/ConvexHull2DRandom256.binary",
        std::ios::binary);
    std::vector<Vector2<float>> points(256);
    input.read((char*)points.data(), 256 * sizeof(Vector2<float>));
    input.close();

    std::vector<std::size_t> expectedHull =
    {
        34, 240, 174, 98, 122, 15, 51, 206, 153, 12, 117, 11, 8
    };

    ConvexHull2<float> ch2;
    ch2(points);
    std::vector<std::size_t> hull = ch2.GetHull();
    UTAssert(ch2.GetDimension() == 2, "Invalid dimension.");
    UTAssert(hull.size() == expectedHull.size(), "Incorrect hull size.");
    for (std::size_t i = 0; i < hull.size(); ++i)
    {
        UTAssert(hull[i] == expectedHull[i], "Incorrect hull index.");
    }
}

void UnitTestConvexHull2::Test1Point()
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

void UnitTestConvexHull2::Test2Points()
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

void UnitTestConvexHull2::Test3ColinearPoints()
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

void UnitTestConvexHull2::Test3NoncolinearPoints()
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

void UnitTestConvexHull2::Test4NoncolinearPoints()
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

#include <GTL/Mathematics/Geometry/2D/ConvexHull2.h>

namespace gtl
{
    template class ConvexHull2<float>;
    template class ConvexHull2<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ConvexHull2)
