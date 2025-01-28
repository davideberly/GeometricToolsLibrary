#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Meshes/DynamicETManifoldMesh.h>
#include <string>
using namespace gtl;

namespace gtl
{
    class UnitTestDynamicETManifoldMesh
    {
    public:
        UnitTestDynamicETManifoldMesh();

    private:
        static std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();
        static std::array<std::size_t, 3> constexpr invalid2{ invalid, invalid, invalid };
        using Entry = std::array<std::array<std::size_t, 3>, 4>;

        std::string PrintTriangle(std::array<std::size_t, 3> const& tri);
        void PrintEntries(std::vector<std::string>& output, std::vector<Entry> const& entries);
        void PrintMesh(std::vector<std::string>& output, DynamicETManifoldMesh const& mesh);
        void PrintCompactMesh(std::vector<std::string>& output,
            std::vector<std::array<std::size_t, 3>> const& triangles,
            std::vector<std::array<std::size_t, 3>>const& adjacents);

        void TestTriangle3();
        void TestTriangle4();
        void TestTriangle5();
        void TestTriangle12();
    };
}

UnitTestDynamicETManifoldMesh::UnitTestDynamicETManifoldMesh()
{
    UTInformation("Mathematics/Meshes/DynamicETManifoldMesh");

    TestTriangle3();
    TestTriangle4();
    TestTriangle5();
    TestTriangle12();
}

std::string UnitTestDynamicETManifoldMesh::PrintTriangle(std::array<std::size_t, 3> const& tri)
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

void UnitTestDynamicETManifoldMesh::PrintEntries(std::vector<std::string>& output,
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

void UnitTestDynamicETManifoldMesh::PrintMesh(std::vector<std::string>& output,
    DynamicETManifoldMesh const& mesh)
{
    output.clear();
    auto const& tmap = mesh.GetTriangles();
    std::vector<Entry> entries(tmap.size());
    std::size_t t = 0;
    for (auto const& element : tmap)
    {
        auto& entry = entries[t++];
        auto const* tri = element.second.get();
        entry[0] = tri->V;
        for (std::size_t j0 = 0, j1 = 1; j0 < 3; ++j0, ++j1)
        {
            auto const* adj = tri->T[j0];
            if (adj != nullptr)
            {
                entry[j1] = adj->V;
            }
            else
            {
                entry[j1] = invalid2;
            }
        }
    }
    std::sort(entries.begin(), entries.end());
    PrintEntries(output, entries);
}

void UnitTestDynamicETManifoldMesh::PrintCompactMesh(std::vector<std::string>& output,
    std::vector<std::array<std::size_t, 3>> const& triangles,
    std::vector<std::array<std::size_t, 3>> const& adjacents)
{
    output.clear();
    std::vector<Entry> entries(triangles.size());
    for (std::size_t t = 0; t < triangles.size(); ++t)
    {
        auto& entry = entries[t];
        entry[0] = triangles[t];
        for (std::size_t j0 = 0, j1 = 1; j0 < 3; ++j0, ++j1)
        {
            std::size_t a = adjacents[t][j0];
            if (a != invalid)
            {
                entry[j1] = triangles[a];
            }
            else
            {
                entry[j1] = invalid2;
            }
        }
    }
    std::sort(entries.begin(), entries.end());
    PrintEntries(output, entries);
}

void UnitTestDynamicETManifoldMesh::TestTriangle3()
{
    // v0 = (2,0), v1 = (1,2), v2 = (-1,1), v3 = (-1,-1), v4 = (1,-2), v5 = (0,0)
    std::vector<std::array<std::size_t, 3>> triangles(3);
    triangles[0] = { 5, 1, 2 };
    triangles[1] = { 5, 3, 4 };
    triangles[2] = { 5, 4, 0 };
    DynamicETManifoldMesh mesh{};
    for (std::size_t t = 0; t < triangles.size(); ++t)
    {
        auto const& tri = triangles[t];
        mesh.Insert(tri[0], tri[1], tri[2]);
    }

    auto const& emap = mesh.GetEdges();
    UTAssert(emap.size() == 8, "Incorrect number of edges.");
    auto const& tmap = mesh.GetTriangles();
    UTAssert(tmap.size() == 3, "Incorrect number of triangles.");
    UTAssert(mesh.IsClosed() == false, "The mesh must be open.");
    UTAssert(mesh.IsOriented() == true, "The mesh must be oriented.");

    for (std::size_t t = 0; t < triangles.size(); ++t)
    {
        auto const& tri = triangles[t];
        mesh.Remove(tri[0], tri[1], tri[2]);
    }
    UTAssert(emap.size() == 0, "Incorrect number of edges.");
    UTAssert(tmap.size() == 0, "Incorrect number of triangles.");
    for (std::size_t t = 0; t < triangles.size(); ++t)
    {
        auto const& tri = triangles[t];
        mesh.Insert(tri[0], tri[1], tri[2]);
    }

    std::vector<std::string> expected(3);
    expected[0] = "(5,1,2): (-,-,-), (-,-,-), (-,-,-)";
    expected[1] = "(5,3,4): (-,-,-), (-,-,-), (5,4,0)";
    expected[2] = "(5,4,0): (5,3,4), (-,-,-), (-,-,-)";
    std::vector<std::string> output{};
    PrintMesh(output, mesh);
    UTAssert(output == expected, "Incorrect output of PrintMesh.");

    std::vector<std::vector<TriangleKey<true>>> components{};
    mesh.GetComponents(components);
    UTAssert(components.size() == 2, "Incorrect number of components.");
    UTAssert(components[0].size() == 1, "Incorrect number of elements in components[0].");
    UTAssert(components[0][0] == TriangleKey<true>(1, 2, 5), "Incorrect triangle in components[0].");
    UTAssert(components[1].size() == 2, "Incorrect number of elements in components[1].");
    UTAssert(components[1][0] == TriangleKey<true>(0, 5, 4), "Incorrect triangle in components[1][0].");
    UTAssert(components[1][1] == TriangleKey<true>(3, 4, 5), "Incorrect triangle in components[1][1].");

    std::vector<std::vector<std::size_t>> polygons{};
    mesh.GetBoundaryPolygons(polygons, true);
    UTAssert(polygons.size() == 2, "Invalid number of polygons from GetBoundaryPolygons.");
    std::vector<std::size_t> expectedPolygon{ 0, 5, 3, 4, 0 };
    UTAssert(polygons[0].size() == 5, "Invalid number of vertices in polygons[0].");
    UTAssert(polygons[0] == expectedPolygon, "Invalid polygons[0].");
    expectedPolygon = { 1, 2, 5, 1 };
    UTAssert(polygons[1].size() == 4, "Invalid number of vertices in polygons[1].");
    UTAssert(polygons[1] == expectedPolygon, "Invalid polygons[1].");

    std::vector<std::array<std::size_t, 3>> compTriangles{}, compAdjacents{};
    mesh.CreateCompactGraph(compTriangles, compAdjacents);
    PrintCompactMesh(output, compTriangles, compAdjacents);
    UTAssert(output == expected, "Incorrect output of PrintCompactMesh.");

    std::vector<std::size_t> icomponents{}, numComponentTriangles{};
    mesh.GetComponentsConsistentChirality(compTriangles, compAdjacents,
        icomponents, numComponentTriangles);
    UTAssert(icomponents.size() == 3, "GetComponentsConsistentChirality failed.");
    UTAssert(icomponents[0] == 0 && icomponents[1] == 1 && icomponents[2] == 2,
        "GetComponentsConsistentChirality failed.");
    UTAssert(numComponentTriangles.size() == 2, "GetComponentsConsistentChirality failed.");
    UTAssert(numComponentTriangles[0] == 1 && numComponentTriangles[1] == 2,
        "GetComponentsConsistentChirality failed.");

    // Reverse the order of vertices in the first triangle of the
    // 2-triangle connected component so that first and second triangles
    // have different orderings (CW and CCW).
    std::swap(triangles[1][1], triangles[1][2]);
    mesh.Clear();
    mesh.ThrowOnNonmanifoldInsertion(false);
    for (std::size_t t = 0; t < triangles.size(); ++t)
    {
        auto const& tri = triangles[t];
        mesh.Insert(tri[0], tri[1], tri[2]);
    }
    mesh.MakeConsistentChirality();
    PrintMesh(output, mesh);
    expected[0] = "(4,5,0): (5,4,3), (-,-,-), (-,-,-)";
    expected[1] = "(5,1,2): (-,-,-), (-,-,-), (-,-,-)";
    expected[2] = "(5,4,3): (4,5,0), (-,-,-), (-,-,-)";
    UTAssert(output == expected, "MakeConsistentChirality failed.");
}

void UnitTestDynamicETManifoldMesh::TestTriangle4()
{
    // v0 = (2,0), v1 = (1,2), v2 = (-1,1), v3 = (-1,-1), v4 = (1,-2), v5 = (0,0)
    std::vector<std::array<std::size_t, 3>> triangles(4);
    triangles[0] = { 5, 1, 2 };
    triangles[1] = { 2, 3, 5 };
    triangles[2] = { 4, 5, 3 };
    triangles[3] = { 5, 4, 0 };
    DynamicETManifoldMesh mesh{};
    for (std::size_t t = 0; t < triangles.size(); ++t)
    {
        auto const& tri = triangles[t];
        mesh.Insert(tri[0], tri[1], tri[2]);
    }

    auto const& emap = mesh.GetEdges();
    UTAssert(emap.size() == 9, "Incorrect number of edges.");
    auto const& tmap = mesh.GetTriangles();
    UTAssert(tmap.size() == 4, "Incorrect number of triangles.");
    UTAssert(mesh.IsClosed() == false, "The mesh must be open.");
    UTAssert(mesh.IsOriented() == true, "The mesh must be oriented.");

    std::vector<std::string> expected(4);
    expected[0] = "(2,3,5): (-,-,-), (4,5,3), (5,1,2)";
    expected[1] = "(4,5,3): (5,4,0), (2,3,5), (-,-,-)";
    expected[2] = "(5,1,2): (-,-,-), (-,-,-), (2,3,5)";
    expected[3] = "(5,4,0): (4,5,3), (-,-,-), (-,-,-)";
    std::vector<std::string> output{};
    PrintMesh(output, mesh);
    UTAssert(output == expected, "Incorrect output of PrintMesh.");

    std::vector<std::vector<TriangleKey<true>>> components{};
    mesh.GetComponents(components);
    UTAssert(components.size() == 1, "Incorrect number of components.");
    UTAssert(components[0].size() == 4, "Incorrect number of elements in components[0].");
    UTAssert(components[0][0] == TriangleKey<true>(0, 5, 4), "Incorrect triangle in components[0][0].");
    UTAssert(components[0][1] == TriangleKey<true>(3, 4, 5), "Incorrect triangle in components[0][1].");
    UTAssert(components[0][2] == TriangleKey<true>(2, 3, 5), "Incorrect triangle in components[0][2].");
    UTAssert(components[0][3] == TriangleKey<true>(1, 2, 5), "Incorrect triangle in components[0][3].");

    std::vector<std::vector<std::size_t>> polygons{};
    mesh.GetBoundaryPolygons(polygons, true);
    UTAssert(polygons.size() == 1, "Invalid number of polygons from GetBoundaryPolygons.");
    std::vector<std::size_t> expectedPolygon{ 0, 5, 1, 2, 3, 4, 0 };
    UTAssert(polygons[0].size() == 7, "Invalid number of vertices in polygons[0].");
    UTAssert(polygons[0] == expectedPolygon, "Invalid polygons[0].");

    std::vector<std::array<std::size_t, 3>> compTriangles{}, compAdjacents{};
    mesh.CreateCompactGraph(compTriangles, compAdjacents);
    PrintCompactMesh(output, compTriangles, compAdjacents);
    UTAssert(output == expected, "Incorrect output of PrintCompactMesh.");

    std::vector<std::size_t> icomponents{}, numComponentTriangles{};
    mesh.GetComponentsConsistentChirality(compTriangles, compAdjacents,
        icomponents, numComponentTriangles);
    UTAssert(icomponents.size() == 4, "GetComponentsConsistentChirality failed.");
    std::vector<std::size_t> expectedComponent{ 0, 1, 2, 3 };
    UTAssert(icomponents == expectedComponent,
        "GetComponentsConsistentChirality failed.");
    UTAssert(numComponentTriangles.size() == 1, "GetComponentsConsistentChirality failed.");
    UTAssert(numComponentTriangles[0] == 4,
        "GetComponentsConsistentChirality failed.");

    // Reverse the order of vertices in the only connected component so that
    // the orderings are CCW, CW, CCW, CW.
    std::swap(triangles[1][1], triangles[1][2]);
    std::swap(triangles[3][1], triangles[3][2]);
    mesh.Clear();
    mesh.ThrowOnNonmanifoldInsertion(false);
    for (std::size_t t = 0; t < triangles.size(); ++t)
    {
        auto const& tri = triangles[t];
        mesh.Insert(tri[0], tri[1], tri[2]);
    }
    mesh.MakeConsistentChirality();
    PrintMesh(output, mesh);
    expected[0] = "(4,0,5): (-,-,-), (-,-,-), (4,5,3)";
    expected[1] = "(4,5,3): (4,0,5), (5,2,3), (-,-,-)";
    expected[2] = "(5,1,2): (-,-,-), (-,-,-), (5,2,3)";
    expected[3] = "(5,2,3): (5,1,2), (-,-,-), (4,5,3)";
    UTAssert(output == expected, "MakeConsistentChirality failed.");
}

void UnitTestDynamicETManifoldMesh::TestTriangle5()
{
    // v0 = (2,0), v1 = (1,2), v2 = (-1,1), v3 = (-1,-1), v4 = (1,-2), v5 = (0,0)
    std::vector<std::array<std::size_t, 3>> triangles(5);
    triangles[0] = { 0, 1, 5 };
    triangles[1] = { 1, 2, 5 };
    triangles[2] = { 2, 3, 5 };
    triangles[3] = { 3, 4, 5 };
    triangles[4] = { 4, 0, 5 };
    DynamicETManifoldMesh mesh{};
    for (std::size_t t = 0; t < triangles.size(); ++t)
    {
        auto const& tri = triangles[t];
        mesh.Insert(tri[0], tri[1], tri[2]);
    }

    auto const& emap = mesh.GetEdges();
    UTAssert(emap.size() == 10, "Incorrect number of edges.");
    auto const& tmap = mesh.GetTriangles();
    UTAssert(tmap.size() == 5, "Incorrect number of triangles.");
    UTAssert(mesh.IsClosed() == false, "The mesh must be open.");
    UTAssert(mesh.IsOriented() == true, "The mesh must be oriented.");

    std::vector<std::string> expected(5);
    expected[0] = "(0,1,5): (-,-,-), (1,2,5), (4,0,5)";
    expected[1] = "(1,2,5): (-,-,-), (2,3,5), (0,1,5)";
    expected[2] = "(2,3,5): (-,-,-), (3,4,5), (1,2,5)";
    expected[3] = "(3,4,5): (-,-,-), (4,0,5), (2,3,5)";
    expected[4] = "(4,0,5): (-,-,-), (0,1,5), (3,4,5)";
    std::vector<std::string> output{};
    PrintMesh(output, mesh);
    UTAssert(output == expected, "Incorrect output of PrintMesh.");

    std::vector<std::vector<TriangleKey<true>>> components{};
    mesh.GetComponents(components);
    UTAssert(components.size() == 1, "Incorrect number of components.");
    UTAssert(components[0].size() == 5, "Incorrect number of elements in components[0].");
    UTAssert(components[0][0] == TriangleKey<true>(1, 2, 5), "Incorrect triangle in components[0][0].");
    UTAssert(components[0][1] == TriangleKey<true>(0, 1, 5), "Incorrect triangle in components[0][1].");
    UTAssert(components[0][2] == TriangleKey<true>(0, 5, 4), "Incorrect triangle in components[0][2].");
    UTAssert(components[0][3] == TriangleKey<true>(3, 4, 5), "Incorrect triangle in components[0][3].");
    UTAssert(components[0][4] == TriangleKey<true>(2, 3, 5), "Incorrect triangle in components[0][4].");

    std::vector<std::vector<std::size_t>> polygons{};
    mesh.GetBoundaryPolygons(polygons, true);
    UTAssert(polygons.size() == 1, "Invalid number of polygons from GetBoundaryPolygons.");
    std::vector<std::size_t> expectedPolygon{ 0, 1, 2, 3, 4, 0 };
    UTAssert(polygons[0].size() == 6, "Invalid number of vertices in polygons[0].");
    UTAssert(polygons[0] == expectedPolygon, "Invalid polygons[0].");

    std::vector<std::array<std::size_t, 3>> compTriangles{}, compAdjacents{};
    mesh.CreateCompactGraph(compTriangles, compAdjacents);
    PrintCompactMesh(output, compTriangles, compAdjacents);
    UTAssert(output == expected, "Incorrect output of PrintCompactMesh.");

    std::vector<std::size_t> icomponents{}, numComponentTriangles{};
    mesh.GetComponentsConsistentChirality(compTriangles, compAdjacents,
        icomponents, numComponentTriangles);
    UTAssert(icomponents.size() == 5, "GetComponentsConsistentChirality failed.");
    std::vector<std::size_t> expectedComponent{ 0, 3, 2, 4, 1 };
    UTAssert(icomponents == expectedComponent,
        "GetComponentsConsistentChirality failed.");
    UTAssert(numComponentTriangles.size() == 1, "GetComponentsConsistentChirality failed.");
    UTAssert(numComponentTriangles[0] == 5,
        "GetComponentsConsistentChirality failed.");

    // Reverse the order of vertices in the only connected component so that
    // the orderings are CCW, CW, CCW, CW, CW.
    std::swap(triangles[1][1], triangles[1][2]);
    std::swap(triangles[3][1], triangles[3][2]);
    std::swap(triangles[4][1], triangles[4][2]);
    mesh.Clear();
    mesh.ThrowOnNonmanifoldInsertion(false);
    for (std::size_t t = 0; t < triangles.size(); ++t)
    {
        auto const& tri = triangles[t];
        mesh.Insert(tri[0], tri[1], tri[2]);
    }
    mesh.MakeConsistentChirality();
    PrintMesh(output, mesh);
    expected[0] = "(0,1,5): (-,-,-), (1,2,5), (5,4,0)";
    expected[1] = "(1,2,5): (-,-,-), (2,3,5), (0,1,5)";
    expected[2] = "(2,3,5): (-,-,-), (5,3,4), (1,2,5)";
    expected[3] = "(5,3,4): (2,3,5), (-,-,-), (5,4,0)";
    expected[4] = "(5,4,0): (5,3,4), (-,-,-), (0,1,5)";
    UTAssert(output == expected, "MakeConsistentChirality failed.");
}

void UnitTestDynamicETManifoldMesh::TestTriangle12()
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
    DynamicETManifoldMesh mesh{};
    for (std::size_t t = 0; t < triangles.size(); ++t)
    {
        auto const& tri = triangles[t];
        mesh.Insert(tri[0], tri[1], tri[2]);
    }

    auto const& emap = mesh.GetEdges();
    UTAssert(emap.size() == 24, "Incorrect number of edges.");
    auto const& tmap = mesh.GetTriangles();
    UTAssert(tmap.size() == 11, "Incorrect number of triangles.");
    UTAssert(mesh.IsClosed() == false, "The mesh must be open.");
    UTAssert(mesh.IsOriented() == true, "The mesh must be oriented.");

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

    std::vector<std::vector<TriangleKey<true>>> components{};
    mesh.GetComponents(components);
    for (std::size_t c = 0; c < components.size(); ++c)
    {
        auto& component = components[c];
        std::sort(component.begin(), component.end());
    }
    std::sort(components.begin(), components.end(),
        [](std::vector<TriangleKey<true>> const& component0, std::vector<TriangleKey<true>> const& component1)
        {
            return component0[0] < component1[0];
        });
    UTAssert(components.size() == 3, "Incorrect number of components.");
    UTAssert(components[0].size() == 5, "Incorrect number of elements in components[1].");
    UTAssert(components[0][0] == TriangleKey<true>(0, 1, 5), "Incorrect triangle in components[0][0].");
    UTAssert(components[0][1] == TriangleKey<true>(0, 5, 4), "Incorrect triangle in components[0][1].");
    UTAssert(components[0][2] == TriangleKey<true>(1, 2, 5), "Incorrect triangle in components[0][2].");
    UTAssert(components[0][3] == TriangleKey<true>(2, 3, 5), "Incorrect triangle in components[0][3].");
    UTAssert(components[0][4] == TriangleKey<true>(3, 4, 5), "Incorrect triangle in components[0][4].");
    UTAssert(components[1].size() == 1, "Incorrect number of elements in components[1].");
    UTAssert(components[1][0] == TriangleKey<true>(2, 6, 7), "Incorrect triangle in components[1][0].");
    UTAssert(components[2].size() == 5, "Incorrect number of elements in components[0].");
    UTAssert(components[2][0] == TriangleKey<true>(3, 7, 11), "Incorrect triangle in components[2][0].");
    UTAssert(components[2][1] == TriangleKey<true>(3, 10, 9), "Incorrect triangle in components[2][1].");
    UTAssert(components[2][2] == TriangleKey<true>(7, 8, 11), "Incorrect triangle in components[2][2].");
    UTAssert(components[2][3] == TriangleKey<true>(8, 9, 10), "Incorrect triangle in components[2][3].");
    UTAssert(components[2][4] == TriangleKey<true>(8, 10, 11), "Incorrect triangle in components[2][4].");

    std::vector<std::vector<std::size_t>> polygons{};
    mesh.GetBoundaryPolygons(polygons, true);
    std::sort(polygons.begin(), polygons.end(),
        [](std::vector<std::size_t> const& polygon0, std::vector<std::size_t> const& polygon1)
        {
            if (polygon0[0] < polygon1[0])
            {
                return true;
            }
            if (polygon0[0] > polygon1[0])
            {
                return false;
            }
            return polygon0 < polygon1;
        });
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

    std::vector<std::array<std::size_t, 3>> compTriangles{}, compAdjacents{};
    mesh.CreateCompactGraph(compTriangles, compAdjacents);
    PrintCompactMesh(output, compTriangles, compAdjacents);
    UTAssert(output == expected, "Incorrect output of PrintCompactMesh.");

    std::vector<std::size_t> icomponents{}, numComponentTriangles{};
    mesh.GetComponentsConsistentChirality(compTriangles, compAdjacents,
        icomponents, numComponentTriangles);
    UTAssert(icomponents.size() == 11, "GetComponentsConsistentChirality failed.");
    std::vector<std::size_t> expectedComponent{ 0, 9, 6, 10, 3, 1, 5, 4, 7, 2, 8 };
    UTAssert(icomponents == expectedComponent,
        "GetComponentsConsistentChirality failed.");
    UTAssert(numComponentTriangles.size() == 3, "GetComponentsConsistentChirality failed.");
    UTAssert(numComponentTriangles[0] == 5, "GetComponentsConsistentChirality failed.");
    UTAssert(numComponentTriangles[1] == 5, "GetComponentsConsistentChirality failed.");
    UTAssert(numComponentTriangles[2] == 1, "GetComponentsConsistentChirality failed.");

    // Reverse the order of vertices some triangles so that there are CCW
    // and CW triangles.
    std::swap(triangles[1][1], triangles[1][2]);
    std::swap(triangles[3][1], triangles[3][2]);
    std::swap(triangles[10][1], triangles[10][2]);
    mesh.Clear();
    mesh.ThrowOnNonmanifoldInsertion(false);
    for (std::size_t t = 0; t < triangles.size(); ++t)
    {
        auto const& tri = triangles[t];
        mesh.Insert(tri[0], tri[1], tri[2]);
    }
    mesh.MakeConsistentChirality();
    PrintMesh(output, mesh);
    expected[0] = "(0,1,5): (-,-,-), (5,1,2), (4,0,5)";
    expected[1] = "(2,3,5): (-,-,-), (3,4,5), (5,1,2)";
    expected[2] = "(2,6,7): (-,-,-), (-,-,-), (-,-,-)";
    expected[3] = "(3,4,5): (-,-,-), (4,0,5), (2,3,5)";
    expected[4] = "(3,7,11): (-,-,-), (11,7,8), (-,-,-)";
    expected[5] = "(3,10,9): (-,-,-), (10,8,9), (-,-,-)";
    expected[6] = "(4,0,5): (-,-,-), (0,1,5), (3,4,5)";
    expected[7] = "(5,1,2): (0,1,5), (-,-,-), (2,3,5)";
    expected[8] = "(10,8,9): (11,8,10), (-,-,-), (3,10,9)";
    expected[9] = "(11,7,8): (3,7,11), (-,-,-), (11,8,10)";
    expected[10] = "(11,8,10): (11,7,8), (10,8,9), (-,-,-)";
    UTAssert(output == expected, "MakeConsistentChirality failed.");
}

#else

#include <GTL/Mathematics/Meshes/DynamicETManifoldMesh.h>

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DynamicETManifoldMesh)
