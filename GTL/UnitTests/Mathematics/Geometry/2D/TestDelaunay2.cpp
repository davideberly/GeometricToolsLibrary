#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/2D/Delaunay2.h>
#include <fstream>
using namespace gtl;

namespace gtl
{
    class UnitTestDelaunay2
    {
    public:
        UnitTestDelaunay2();

    private:
        void TestSorting();
        void TestRandom();
        void TestCoincidentPoints();
        void TestColinearPoints();
        void TestRegularGrid();
    };
}

UnitTestDelaunay2::UnitTestDelaunay2()
{
    // Also tested by GTL/VisualTests/Geometry/Delaunay2D.
    UTInformation("Mathematics/Geometry/2D/Delaunay2");

    TestSorting();
    TestRandom();
    TestCoincidentPoints();
    TestColinearPoints();
    TestRegularGrid();
}

void UnitTestDelaunay2::TestSorting()
{
    std::ifstream input(
        "Mathematics/Geometry/2D/Delaunay2DRandom256.binary",
        std::ios::binary);
    std::vector<Vector2<float>> points(256);
    input.read((char*)points.data(), 256 * sizeof(Vector2<float>));
    input.close();

    points[100] = points[17];
    points[150] = points[17];
    points[200] = points[17];
    points[8] = points[0];

    Delaunay2<float> del2;
    del2(points);

    UTAssert(del2.mEquivalentTo[0] == 0, "Incorrect mapping.");
    UTAssert(del2.mEquivalentTo[8] == 0, "Incorrect mapping.");
    UTAssert(del2.mEquivalentTo[17] == 17, "Incorrect mapping.");
    UTAssert(del2.mEquivalentTo[100] == 17, "Incorrect mapping.");
    UTAssert(del2.mEquivalentTo[150] == 17, "Incorrect mapping.");
    UTAssert(del2.mEquivalentTo[200] == 17, "Incorrect mapping.");
}

void UnitTestDelaunay2::TestRandom()
{
    std::ifstream input(
        "Mathematics/Geometry/2D/Delaunay2DRandom256.binary",
        std::ios::binary);
    std::vector<Vector2<float>> points(256);
    input.read((char*)points.data(), 256 * sizeof(Vector2<float>));
    input.close();

    input.open("Mathematics/Geometry/2D/Delaunay2DExpectedTriangles.txt");
    std::size_t const numTriangles = 497;
    std::vector<TriangleKey<true>> expectedTriangles(numTriangles);
    for (auto& tri : expectedTriangles)
    {
        input >> tri[0] >> tri[1] >> tri[2];
    }
    input.close();

    Delaunay2<float> del2;
    del2(points);
    std::vector<std::size_t> indices = del2.GetIndices();
    UTAssert(del2.GetDimension() == 2, "Invalid dimension.");
    UTAssert(indices.size() / 3 == numTriangles, "Incorrect index array size.");
    std::vector<TriangleKey<true>> triangles(numTriangles);
    for (std::size_t t = 0, i = 0; t < numTriangles; ++t)
    {
        std::size_t v0 = indices[i++];
        std::size_t v1 = indices[i++];
        std::size_t v2 = indices[i++];
        triangles[t] = TriangleKey<true>(v0, v1, v2);
    }
    std::sort(triangles.begin(), triangles.end());

    for (std::size_t t = 0; t < numTriangles; ++t)
    {
        UTAssert(triangles[t] == expectedTriangles[t], "Incorrect triangle.");
    }
}

void UnitTestDelaunay2::TestCoincidentPoints()
{
    std::vector<Vector2<float>> points(10);
    for (auto& p : points)
    {
        p = { 256.0f, 256.0f };
    }

    Delaunay2<float> del2;
    del2(points);
    std::vector<std::size_t> indices = del2.GetIndices();
    UTAssert(del2.GetDimension() == 0, "Invalid dimension.");
    UTAssert(indices.size() == 1, "Incorrect index array size.");
    UTAssert(indices[0] == 0, "Incorrect index.");
}

void UnitTestDelaunay2::TestColinearPoints()
{
    // A 1-dimensional dataset.
    std::vector<Vector2<float>> points(8);
    points[0] = { 376.853882f, 0.0f };
    points[1] = { 411.824097f, 0.0f };
    points[2] = { 112.762939f, 0.0f };
    points[3] = { 414.736328f, 0.0f };
    points[4] = { 306.825958f, 0.0f };
    points[5] = { 101.455513f, 0.0f };
    points[6] = { 170.943329f, 0.0f };
    points[7] = { 274.002502f, 0.0f };

    Delaunay2<float> del2;
    del2(points);
    std::vector<std::size_t> indices = del2.GetIndices();
    UTAssert(del2.GetDimension() == 1, "Invalid dimension.");
    UTAssert(indices.size() == 8, "Incorrect index array size.");

    std::vector<std::size_t> expectedIndices = { 5, 2, 6, 7, 4, 0, 1, 3 };
    for (std::size_t i = 0; i < indices.size(); ++i)
    {
        UTAssert(indices[i] == expectedIndices[i], "Incorrect index.");
    }
}

void UnitTestDelaunay2::TestRegularGrid()
{
    // A 3x3 square grid.
    std::vector<Vector2<float>> points(9);
    points[0] = Vector2<float>{ 64.0f,  64.0f };
    points[1] = Vector2<float>{ 64.0f, 256.0f };
    points[2] = Vector2<float>{ 64.0f, 448.0f };
    points[3] = Vector2<float>{ 256.0f,  64.0f };
    points[4] = Vector2<float>{ 256.0f, 256.0f };
    points[5] = Vector2<float>{ 256.0f, 448.0f };
    points[6] = Vector2<float>{ 448.0f,  64.0f };
    points[7] = Vector2<float>{ 448.0f, 256.0f };
    points[8] = Vector2<float>{ 448.0f, 448.0f };

    Delaunay2<float> del2;
    del2(points);
    std::vector<std::size_t> indices = del2.GetIndices();
    UTAssert(del2.GetDimension() == 2, "Invalid dimension.");
    UTAssert(indices.size() == 24, "Incorrect index array size.");

    std::vector<std::size_t> expectedIndices =
    {
        0, 3, 4,
        0, 4, 1,
        1, 5, 2,
        1, 4, 5,
        3, 6, 7,
        3, 7, 4,
        4, 8, 5,
        4, 7, 8
    };
    for (std::size_t i = 0; i < indices.size(); ++i)
    {
        UTAssert(indices[i] == expectedIndices[i], "Incorrect index.");
    }
}

#else

#include <GTL/Mathematics/Geometry/2D/Delaunay2.h>

namespace gtl
{
    template class Delaunay2<float>;
    template class Delaunay2<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Delaunay2)
