#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Meshes/DynamicVETManifoldMesh.h>
#include <map>
#include <set>
#include <string>
using namespace gtl;

namespace gtl
{
    class UnitTestDynamicVETManifoldMesh
    {
    public:
        UnitTestDynamicVETManifoldMesh();

    private:
        class TVertex
        {
        public:
            TVertex(std::size_t vIndex)
                :
                V(vIndex),
                VAdjacent{},
                EAdjacent{},
                TAdjacent{}
            {
            }

            std::size_t V;
            std::set<std::size_t> VAdjacent;
            std::set<EdgeKey<false>> EAdjacent;
            std::set<TriangleKey<true>> TAdjacent;
        };

        using TVMap = std::map<std::size_t, std::unique_ptr<TVertex>>;

        void Convert(DynamicVETManifoldMesh const& mesh, TVMap& tvmap);
        void Print(TVMap const& tvmap, std::vector<std::string>& output);

        void TestTriangle3();
        void TestTriangle4();
        void TestTriangle5();
        void TestTriangle12();
    };
}

UnitTestDynamicVETManifoldMesh::UnitTestDynamicVETManifoldMesh()
{
    UTInformation("Mathematics/Meshes/DynamicVETManifoldMesh");

    // The base class member functions in DynamicETManifoldMesh have been
    // tested in TestDynamicETManifoldMesh.cpp. This code tests only the
    // vertex portion of DynamicVETManifoldMesh.
    TestTriangle3();
    TestTriangle4();
    TestTriangle5();
    TestTriangle12();
}

void UnitTestDynamicVETManifoldMesh::Convert(DynamicVETManifoldMesh const& mesh, TVMap& tvmap)
{
    auto const& vmap = mesh.GetVertices();
    for (auto const& vertex : vmap)
    {
        auto tvertex = std::make_unique<TVertex>(vertex.first);

        for (auto const& vadj : vertex.second->VAdjacent)
        {
            tvertex->VAdjacent.insert(vadj);
        }

        for (auto const& eadj : vertex.second->EAdjacent)
        {
            tvertex->EAdjacent.insert(EdgeKey<false>(eadj->V[0], eadj->V[1]));
        }

        for (auto const& tadj : vertex.second->TAdjacent)
        {
            tvertex->TAdjacent.insert(TriangleKey<true>(tadj->V[0], tadj->V[1], tadj->V[2]));
        }

        tvmap.insert(std::make_pair(tvertex->V, std::move(tvertex)));
    }
}

void UnitTestDynamicVETManifoldMesh::Print(TVMap const& tvmap, std::vector<std::string>& output)
{
    for (auto const& element : tvmap)
    {
        std::string message = std::to_string(element.first) + ":";
        output.push_back(message);

        std::size_t numItems = element.second->VAdjacent.size();
        std::size_t i = 0;
        message = "    ";
        for (auto const& vadj : element.second->VAdjacent)
        {
            message += std::to_string(vadj);
            ++i;
            if (i < numItems)
            {
                message += ",";
            }
        }
        output.push_back(message);

        numItems = element.second->EAdjacent.size();
        i = 0;
        message = "    ";
        for (auto const& eadj : element.second->EAdjacent)
        {
            message += "(";
            message += std::to_string(eadj[0]) + "," + std::to_string(eadj[1]) + ")";
            ++i;
            if (i < numItems)
            {
                message += ",";
            }
        }
        output.push_back(message);

        numItems = element.second->TAdjacent.size();
        i = 0;
        message = "    ";
        for (auto const& tadj : element.second->TAdjacent)
        {
            message += "(";
            message += std::to_string(tadj[0]) + "," + std::to_string(tadj[1]) + "," + std::to_string(tadj[2]) + ")";
            ++i;
            if (i < numItems)
            {
                message += ",";
            }
        }
        output.push_back(message);
    }
}

void UnitTestDynamicVETManifoldMesh::TestTriangle3()
{
    // v0 = (2,0), v1 = (1,2), v2 = (-1,1), v3 = (-1,-1), v4 = (1,-2), v5 = (0,0)
    std::vector<std::array<std::size_t, 3>> triangles(3);
    triangles[0] = { 5, 1, 2 };
    triangles[1] = { 5, 3, 4 };
    triangles[2] = { 5, 4, 0 };
    DynamicVETManifoldMesh mesh{};
    for (std::size_t t = 0; t < triangles.size(); ++t)
    {
        auto const& tri = triangles[t];
        mesh.Insert(tri[0], tri[1], tri[2]);
    }

    TVMap tvmap{};
    Convert(mesh, tvmap);
    std::vector<std::string> output{};
    Print(tvmap, output);

    std::vector<std::string> expected(24);
    expected[0] = "0:";
    expected[1] = "    4,5";
    expected[2] = "    (0,4),(0,5)";
    expected[3] = "    (0,5,4)";
    expected[4] = "1:";
    expected[5] = "    2,5";
    expected[6] = "    (1,2),(1,5)";
    expected[7] = "    (1,2,5)";
    expected[8] = "2:";
    expected[9] = "    1,5";
    expected[10] = "    (1,2),(2,5)";
    expected[11] = "    (1,2,5)";
    expected[12] = "3:";
    expected[13] = "    4,5";
    expected[14] = "    (3,4),(3,5)";
    expected[15] = "    (3,4,5)";
    expected[16] = "4:";
    expected[17] = "    0,3,5";
    expected[18] = "    (0,4),(3,4),(4,5)";
    expected[19] = "    (0,5,4),(3,4,5)";
    expected[20] = "5:";
    expected[21] = "    0,1,2,3,4";
    expected[22] = "    (0,5),(1,5),(2,5),(3,5),(4,5)";
    expected[23] = "    (0,5,4),(1,2,5),(3,4,5)";
    UTAssert(output == expected, "TestTriangle3 failed.");
}

void UnitTestDynamicVETManifoldMesh::TestTriangle4()
{
    // v0 = (2,0), v1 = (1,2), v2 = (-1,1), v3 = (-1,-1), v4 = (1,-2), v5 = (0,0)
    std::vector<std::array<std::size_t, 3>> triangles(4);
    triangles[0] = { 5, 1, 2 };
    triangles[1] = { 2, 3, 5 };
    triangles[2] = { 4, 5, 3 };
    triangles[3] = { 5, 4, 0 };
    DynamicVETManifoldMesh mesh{};
    for (std::size_t t = 0; t < triangles.size(); ++t)
    {
        auto const& tri = triangles[t];
        mesh.Insert(tri[0], tri[1], tri[2]);
    }

    TVMap tvmap{};
    Convert(mesh, tvmap);
    std::vector<std::string> output{};
    Print(tvmap, output);

    std::vector<std::string> expected(24);
    expected[0] = "0:";
    expected[1] = "    4,5";
    expected[2] = "    (0,4),(0,5)";
    expected[3] = "    (0,5,4)";
    expected[4] = "1:";
    expected[5] = "    2,5";
    expected[6] = "    (1,2),(1,5)";
    expected[7] = "    (1,2,5)";
    expected[8] = "2:";
    expected[9] = "    1,3,5";
    expected[10] = "    (1,2),(2,3),(2,5)";
    expected[11] = "    (1,2,5),(2,3,5)";
    expected[12] = "3:";
    expected[13] = "    2,4,5";
    expected[14] = "    (2,3),(3,4),(3,5)";
    expected[15] = "    (2,3,5),(3,4,5)";
    expected[16] = "4:";
    expected[17] = "    0,3,5";
    expected[18] = "    (0,4),(3,4),(4,5)";
    expected[19] = "    (0,5,4),(3,4,5)";
    expected[20] = "5:";
    expected[21] = "    0,1,2,3,4";
    expected[22] = "    (0,5),(1,5),(2,5),(3,5),(4,5)";
    expected[23] = "    (0,5,4),(1,2,5),(2,3,5),(3,4,5)";
    UTAssert(output == expected, "TestTriangle4 failed.");
}

void UnitTestDynamicVETManifoldMesh::TestTriangle5()
{
    // v0 = (2,0), v1 = (1,2), v2 = (-1,1), v3 = (-1,-1), v4 = (1,-2), v5 = (0,0)
    std::vector<std::array<std::size_t, 3>> triangles(5);
    triangles[0] = { 0, 1, 5 };
    triangles[1] = { 1, 2, 5 };
    triangles[2] = { 2, 3, 5 };
    triangles[3] = { 3, 4, 5 };
    triangles[4] = { 4, 0, 5 };
    DynamicVETManifoldMesh mesh{};
    for (std::size_t t = 0; t < triangles.size(); ++t)
    {
        auto const& tri = triangles[t];
        mesh.Insert(tri[0], tri[1], tri[2]);
    }

    TVMap tvmap{};
    Convert(mesh, tvmap);
    std::vector<std::string> output{};
    Print(tvmap, output);

    std::vector<std::string> expected(24);
    expected[0] = "0:";
    expected[1] = "    1,4,5";
    expected[2] = "    (0,1),(0,4),(0,5)";
    expected[3] = "    (0,1,5),(0,5,4)";
    expected[4] = "1:";
    expected[5] = "    0,2,5";
    expected[6] = "    (0,1),(1,2),(1,5)";
    expected[7] = "    (0,1,5),(1,2,5)";
    expected[8] = "2:";
    expected[9] = "    1,3,5";
    expected[10] = "    (1,2),(2,3),(2,5)";
    expected[11] = "    (1,2,5),(2,3,5)";
    expected[12] = "3:";
    expected[13] = "    2,4,5";
    expected[14] = "    (2,3),(3,4),(3,5)";
    expected[15] = "    (2,3,5),(3,4,5)";
    expected[16] = "4:";
    expected[17] = "    0,3,5";
    expected[18] = "    (0,4),(3,4),(4,5)";
    expected[19] = "    (0,5,4),(3,4,5)";
    expected[20] = "5:";
    expected[21] = "    0,1,2,3,4";
    expected[22] = "    (0,5),(1,5),(2,5),(3,5),(4,5)";
    expected[23] = "    (0,1,5),(0,5,4),(1,2,5),(2,3,5),(3,4,5)";
    UTAssert(output == expected, "TestTriangle5 failed.");
}

void UnitTestDynamicVETManifoldMesh::TestTriangle12()
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
    DynamicVETManifoldMesh mesh{};
    for (std::size_t t = 0; t < triangles.size(); ++t)
    {
        auto const& tri = triangles[t];
        mesh.Insert(tri[0], tri[1], tri[2]);
    }

    TVMap tvmap{};
    Convert(mesh, tvmap);
    std::vector<std::string> output{};
    Print(tvmap, output);

    std::vector<std::string> expected(48);
    expected[0] = "0:";
    expected[1] = "    1,4,5";
    expected[2] = "    (0,1),(0,4),(0,5)";
    expected[3] = "    (0,1,5),(0,5,4)";
    expected[4] = "1:";
    expected[5] = "    0,2,5";
    expected[6] = "    (0,1),(1,2),(1,5)";
    expected[7] = "    (0,1,5),(1,2,5)";
    expected[8] = "2:";
    expected[9] = "    1,3,5,6,7";
    expected[10] = "    (1,2),(2,3),(2,5),(2,6),(2,7)";
    expected[11] = "    (1,2,5),(2,3,5),(2,6,7)";
    expected[12] = "3:";
    expected[13] = "    2,4,5,7,9,10,11";
    expected[14] = "    (2,3),(3,4),(3,5),(3,7),(3,9),(3,10),(3,11)";
    expected[15] = "    (2,3,5),(3,4,5),(3,7,11),(3,10,9)";
    expected[16] = "4:";
    expected[17] = "    0,3,5";
    expected[18] = "    (0,4),(3,4),(4,5)";
    expected[19] = "    (0,5,4),(3,4,5)";
    expected[20] = "5:";
    expected[21] = "    0,1,2,3,4";
    expected[22] = "    (0,5),(1,5),(2,5),(3,5),(4,5)";
    expected[23] = "    (0,1,5),(0,5,4),(1,2,5),(2,3,5),(3,4,5)";
    expected[24] = "6:";
    expected[25] = "    2,7";
    expected[26] = "    (2,6),(6,7)";
    expected[27] = "    (2,6,7)";
    expected[28] = "7:";
    expected[29] = "    2,3,6,8,11";
    expected[30] = "    (2,7),(3,7),(6,7),(7,8),(7,11)";
    expected[31] = "    (2,6,7),(3,7,11),(7,8,11)";
    expected[32] = "8:";
    expected[33] = "    7,9,10,11";
    expected[34] = "    (7,8),(8,9),(8,10),(8,11)";
    expected[35] = "    (7,8,11),(8,9,10),(8,10,11)";
    expected[36] = "9:";
    expected[37] = "    3,8,10";
    expected[38] = "    (3,9),(8,9),(9,10)";
    expected[39] = "    (3,10,9),(8,9,10)";
    expected[40] = "10:";
    expected[41] = "    3,8,9,11";
    expected[42] = "    (3,10),(8,10),(9,10),(10,11)";
    expected[43] = "    (3,10,9),(8,9,10),(8,10,11)";
    expected[44] = "11:";
    expected[45] = "    3,7,8,10";
    expected[46] = "    (3,11),(7,11),(8,11),(10,11)";
    expected[47] = "    (3,7,11),(7,8,11),(8,10,11)";
    UTAssert(output == expected, "TestTriangle5 failed.");
}

#else

#include <GTL/Mathematics/Meshes/DynamicVETManifoldMesh.h>

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DynamicVETManifoldMesh)
