#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Meshes/StaticVETManifoldMesh.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <GTL/Utility/Timer.h>
#include <fstream>
#include <string>
using namespace gtl;

namespace gtl
{
    class UnitTestStaticVETManifoldMesh
    {
    public:
        UnitTestStaticVETManifoldMesh();

    private:
        static std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();
        static std::array<std::size_t, 3> constexpr invalid2{ invalid, invalid, invalid };
        using Entry = std::array<std::array<std::size_t, 3>, 4>;

        std::string PrintTriangle(std::array<std::size_t, 3> const& tri);
        void PrintEntries(std::vector<std::string>& output, std::vector<Entry> const& entries);
        void PrintMesh(std::vector<std::string>& output, StaticVETManifoldMesh const& mesh);

        void TestTriangle2();
        void TestTriangle3();
        void TestTriangle4();
        void TestTriangle5();
        void TestTriangle12();
    };
}

UnitTestStaticVETManifoldMesh::UnitTestStaticVETManifoldMesh()
{
    UTInformation("Mathematics/Meshes/StaticVETManifoldMesh");

    TestTriangle2();
    TestTriangle3();
    TestTriangle4();
    TestTriangle5();
    TestTriangle12();
}

std::string UnitTestStaticVETManifoldMesh::PrintTriangle(std::array<std::size_t, 3> const& tri)
{
    if (tri != invalid2)
    {
        return "(" + std::to_string(tri[0]) + "," +
            std::to_string(tri[1]) + "," + std::to_string(tri[2]) + ")";
    }
    else
    {
        return "(-,-,-)";
    }
}

void UnitTestStaticVETManifoldMesh::PrintEntries(std::vector<std::string>& output,
    std::vector<Entry> const& entries)
{
    for (std::size_t t = 0; t < entries.size(); ++t)
    {
        auto const& entry = entries[t];
        std::string triStr = PrintTriangle(entry[0]);
        std::string entryStr = triStr + ": ";
        for (std::size_t j1 = 1; j1 < 4; ++j1)
        {
            std::string adjStr = PrintTriangle(entry[j1]);
            entryStr += adjStr;
            if (j1 < 3)
            {
                entryStr += ", ";
            }
        }
        output.push_back(entryStr);
    }
}

void UnitTestStaticVETManifoldMesh::PrintMesh(std::vector<std::string>& output,
    StaticVETManifoldMesh const& mesh)
{
    output.clear();
    auto const& triangles = mesh.GetTriangles();
    auto const& adjacents = mesh.GetAdjacents();
    std::vector<Entry> entries(triangles.size());
    for (std::size_t t = 0; t < triangles.size(); ++t)
    {
        auto& entry = entries[t];
        entry[0] = triangles[t];
        for (std::size_t j0 = 0, j1 = 1; j0 < 3; ++j0, ++j1)
        {
            std::size_t adj = adjacents[t][j0];
            if (adj != invalid)
            {
                entry[j1] = triangles[adj];
            }
            else
            {
                entry[j1] = invalid2;
            }
        }
        std::swap(entry[3], entry[1]);
        std::swap(entry[2], entry[3]);
    }
    std::sort(entries.begin(), entries.end());
    PrintEntries(output, entries);
}

void UnitTestStaticVETManifoldMesh::TestTriangle2()
{
    // v0 = (0,0), v1 = (1,0), v2 = (0,1), v3 = (1,1)
    std::size_t constexpr numThreads = 0;
    std::vector<std::array<std::size_t, 3>> triangles(2);
    triangles[0] = { 0, 1, 2 };
    triangles[1] = { 1, 3, 2 };
    StaticVETManifoldMesh mesh(4, triangles, numThreads);
    std::vector<std::string> expected(2);
    expected[0] = "(0,1,2): (-,-,-), (1,3,2), (-,-,-)";
    expected[1] = "(1,3,2): (-,-,-), (-,-,-), (0,1,2)";
    std::vector<std::string> output{};
    PrintMesh(output, mesh);
    UTAssert(output == expected, "Incorrect output of PrintMesh.");
}

void UnitTestStaticVETManifoldMesh::TestTriangle3()
{
    // v0 = (2,0), v1 = (1,2), v2 = (-1,1), v3 = (-1,-1), v4 = (1,-2), v5 = (0,0)
    std::vector<std::array<std::size_t, 3>> triangles(3);
    triangles[0] = { 5, 1, 2 };
    triangles[1] = { 5, 3, 4 };
    triangles[2] = { 5, 4, 0 };
    StaticVETManifoldMesh mesh(6, triangles, 0);

    std::vector<std::string> expected(3);
    expected[0] = "(5,1,2): (-,-,-), (-,-,-), (-,-,-)";
    expected[1] = "(5,3,4): (-,-,-), (-,-,-), (5,4,0)";
    expected[2] = "(5,4,0): (5,3,4), (-,-,-), (-,-,-)";
    std::vector<std::string> output{};
    PrintMesh(output, mesh);
    UTAssert(output == expected, "Incorrect output of PrintMesh.");

    std::vector<std::vector<std::size_t>> components{};
    mesh.GetComponents(components);
    UTAssert(components.size() == 2, "Incorrect number of components.");
    UTAssert(components[0].size() == 1, "Incorrect number of elements in components[0].");
    UTAssert(components[0][0] == 0, "Incorrect triangle in components[0].");
    UTAssert(components[1].size() == 2, "Incorrect number of elements in components[1].");
    UTAssert(components[1][0] == 2, "Incorrect triangle in components[1][0].");
    UTAssert(components[1][1] == 1, "Incorrect triangle in components[1][1].");

    std::vector<std::vector<std::size_t>> polygons{};
    mesh.GetBoundaryPolygons(polygons, true);
    UTAssert(polygons.size() == 2, "Invalid number of polygons from GetBoundaryPolygons.");
    std::vector<std::size_t> expectedPolygon{ 0, 5, 3, 4, 0 };
    UTAssert(polygons[0].size() == 5, "Invalid number of vertices in polygons[0].");
    UTAssert(polygons[0] == expectedPolygon, "Invalid polygons[0].");
    expectedPolygon = { 1, 2, 5, 1 };
    UTAssert(polygons[1].size() == 4, "Invalid number of vertices in polygons[1].");
    UTAssert(polygons[1] == expectedPolygon, "Invalid polygons[1].");
}

void UnitTestStaticVETManifoldMesh::TestTriangle4()
{
    // v0 = (2,0), v1 = (1,2), v2 = (-1,1), v3 = (-1,-1), v4 = (1,-2), v5 = (0,0)
    std::vector<std::array<std::size_t, 3>> triangles(4);
    triangles[0] = { 5, 1, 2 };
    triangles[1] = { 2, 3, 5 };
    triangles[2] = { 4, 5, 3 };
    triangles[3] = { 5, 4, 0 };
    StaticVETManifoldMesh mesh(6, triangles, 0);

    std::vector<std::string> expected(4);
    expected[0] = "(2,3,5): (-,-,-), (4,5,3), (5,1,2)";
    expected[1] = "(4,5,3): (5,4,0), (2,3,5), (-,-,-)";
    expected[2] = "(5,1,2): (-,-,-), (-,-,-), (2,3,5)";
    expected[3] = "(5,4,0): (4,5,3), (-,-,-), (-,-,-)";
    std::vector<std::string> output{};
    PrintMesh(output, mesh);
    UTAssert(output == expected, "Incorrect output of PrintMesh.");

    std::vector<std::vector<std::size_t>> components{};
    mesh.GetComponents(components);
    UTAssert(components.size() == 1, "Incorrect number of components.");
    UTAssert(components[0].size() == 4, "Incorrect number of elements in components[0].");
    UTAssert(components[0][0] == 3, "Incorrect triangle in components[0][0].");
    UTAssert(components[0][1] == 2, "Incorrect triangle in components[0][1].");
    UTAssert(components[0][2] == 1, "Incorrect triangle in components[0][2].");
    UTAssert(components[0][3] == 0, "Incorrect triangle in components[0][3].");

    std::vector<std::vector<std::size_t>> polygons{};
    mesh.GetBoundaryPolygons(polygons, true);
    UTAssert(polygons.size() == 1, "Invalid number of polygons from GetBoundaryPolygons.");
    std::vector<std::size_t> expectedPolygon{ 0, 5, 1, 2, 3, 4, 0 };
    UTAssert(polygons[0].size() == 7, "Invalid number of vertices in polygons[0].");
    UTAssert(polygons[0] == expectedPolygon, "Invalid polygons[0].");
}

void UnitTestStaticVETManifoldMesh::TestTriangle5()
{
    // v0 = (2,0), v1 = (1,2), v2 = (-1,1), v3 = (-1,-1), v4 = (1,-2), v5 = (0,0)
    std::vector<std::array<std::size_t, 3>> triangles(5);
    triangles[0] = { 0, 1, 5 };
    triangles[1] = { 1, 2, 5 };
    triangles[2] = { 2, 3, 5 };
    triangles[3] = { 3, 4, 5 };
    triangles[4] = { 4, 0, 5 };
    StaticVETManifoldMesh mesh(6, triangles, 0);

    std::vector<std::string> expected(5);
    expected[0] = "(0,1,5): (-,-,-), (1,2,5), (4,0,5)";
    expected[1] = "(1,2,5): (-,-,-), (2,3,5), (0,1,5)";
    expected[2] = "(2,3,5): (-,-,-), (3,4,5), (1,2,5)";
    expected[3] = "(3,4,5): (-,-,-), (4,0,5), (2,3,5)";
    expected[4] = "(4,0,5): (-,-,-), (0,1,5), (3,4,5)";
    std::vector<std::string> output{};
    PrintMesh(output, mesh);
    UTAssert(output == expected, "Incorrect output of PrintMesh.");

    std::vector<std::vector<std::size_t>> components{};
    mesh.GetComponents(components);
    UTAssert(components.size() == 1, "Incorrect number of components.");
    UTAssert(components[0].size() == 5, "Incorrect number of elements in components[0].");
    UTAssert(components[0][0] == 4, "Incorrect triangle in components[0][0].");
    UTAssert(components[0][1] == 3, "Incorrect triangle in components[0][1].");
    UTAssert(components[0][2] == 2, "Incorrect triangle in components[0][2].");
    UTAssert(components[0][3] == 1, "Incorrect triangle in components[0][3].");
    UTAssert(components[0][4] == 0, "Incorrect triangle in components[0][4].");

    std::vector<std::vector<std::size_t>> polygons{};
    mesh.GetBoundaryPolygons(polygons, true);
    UTAssert(polygons.size() == 1, "Invalid number of polygons from GetBoundaryPolygons.");
    std::vector<std::size_t> expectedPolygon{ 0, 1, 2, 3, 4, 0 };
    UTAssert(polygons[0].size() == 6, "Invalid number of vertices in polygons[0].");
    UTAssert(polygons[0] == expectedPolygon, "Invalid polygons[0].");
}

void UnitTestStaticVETManifoldMesh::TestTriangle12()
{
    // v0 = (-1,0), v1 = (1, 0), v2 = (2, 1), v3 = (0, 2), v4 = (-2, 1),
    // v5 = (0, 1), v6 = (3, 1), v7 = (3, 3), v8 = (2, 4), v9 = (-2, 3),
    // v10 = (-1, 3), v11 = (1, 3)
    std::vector<std::array<std::size_t, 3>> triangles(11);
    triangles[0] = { 0, 1, 5 };
    triangles[1] = { 1, 2, 5 };
    triangles[2] = { 2, 3, 5 };
    triangles[3] = { 3, 4, 5 };
    triangles[4] = { 4, 0, 5 };
    triangles[5] = { 2, 6, 7 };
    triangles[6] = { 3, 7, 11 };
    triangles[7] = { 3, 10, 9 };
    triangles[8] = { 11, 7, 8 };
    triangles[9] = { 10, 8, 9 };
    triangles[10] = { 8, 10, 11 };
    StaticVETManifoldMesh mesh(12, triangles, 0);

    std::vector<std::string> expected(11);
    expected[0] = "(0,1,5): (-,-,-), (1,2,5), (4,0,5)";
    expected[1] = "(1,2,5): (-,-,-), (2,3,5), (0,1,5)";
    expected[2] = "(2,3,5): (-,-,-), (3,4,5), (1,2,5)";
    expected[3] = "(2,6,7): (-,-,-), (-,-,-), (-,-,-)";
    expected[4] = "(3,4,5): (-,-,-), (4,0,5), (2,3,5)";
    expected[5] = "(3,7,11): (-,-,-), (11,7,8), (-,-,-)";
    expected[6] = "(3,10,9): (-,-,-), (10,8,9), (-,-,-)";
    expected[7] = "(4,0,5): (-,-,-), (0,1,5), (3,4,5)";
    expected[8] = "(8,10,11): (10,8,9), (-,-,-), (11,7,8)";
    expected[9] = "(10,8,9): (8,10,11), (-,-,-), (3,10,9)";
    expected[10] = "(11,7,8): (3,7,11), (-,-,-), (8,10,11)";
    std::vector<std::string> output{};
    PrintMesh(output, mesh);
    UTAssert(output == expected, "Incorrect output of PrintMesh.");

    std::vector<std::vector<std::size_t>> components{};
    mesh.GetComponents(components);
    UTAssert(components.size() == 3, "Incorrect number of components.");
    UTAssert(components[0].size() == 5, "Incorrect number of elements in components[1].");
    UTAssert(components[0][0] == 4, "Incorrect triangle in components[0][0].");
    UTAssert(components[0][1] == 3, "Incorrect triangle in components[0][1].");
    UTAssert(components[0][2] == 2, "Incorrect triangle in components[0][2].");
    UTAssert(components[0][3] == 1, "Incorrect triangle in components[0][3].");
    UTAssert(components[0][4] == 0, "Incorrect triangle in components[0][4].");
    UTAssert(components[1].size() == 1, "Incorrect number of elements in components[1].");
    UTAssert(components[1][0] == 5, "Incorrect triangle in components[1][0].");
    UTAssert(components[2].size() == 5, "Incorrect number of elements in components[0].");
    UTAssert(components[2][0] == 7, "Incorrect triangle in components[2][0].");
    UTAssert(components[2][1] == 9, "Incorrect triangle in components[2][1].");
    UTAssert(components[2][2] == 10, "Incorrect triangle in components[2][2].");
    UTAssert(components[2][3] == 8, "Incorrect triangle in components[2][3].");
    UTAssert(components[2][4] == 6, "Incorrect triangle in components[2][4].");

    std::vector<std::vector<std::size_t>> polygons{};
    mesh.GetBoundaryPolygons(polygons, true);
    UTAssert(polygons.size() == 3, "Invalid number of polygons from GetBoundaryPolygons.");
    std::vector<std::size_t> expectedPolygon{ 0, 1, 2, 3, 4, 0 };
    UTAssert(polygons[0].size() == 6, "Invalid number of vertices in polygons[0].");
    UTAssert(polygons[0] == expectedPolygon, "Invalid polygons[0].");
    expectedPolygon = { 2, 6, 7, 2 };
    UTAssert(polygons[1].size() == 4, "Invalid number of vertices in polygons[1].");
    UTAssert(polygons[1] == expectedPolygon, "Invalid polygons[1].");
    expectedPolygon = { 3, 7, 8, 9, 3, 10, 11, 3 };
    UTAssert(polygons[2].size() == 8, "Invalid number of vertices in polygons[2].");
    UTAssert(polygons[2] == expectedPolygon, "Invalid polygons[2].");
}

#else

#include <GTL/Mathematics/Meshes/DynamicETManifoldMesh.h>

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(StaticVETManifoldMesh)
