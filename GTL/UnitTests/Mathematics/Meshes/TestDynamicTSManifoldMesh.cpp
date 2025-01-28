#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Meshes/DynamicTSManifoldMesh.h>
#include <string>
using namespace gtl;

namespace gtl
{
    class UnitTestDynamicTSManifoldMesh
    {
    public:
        UnitTestDynamicTSManifoldMesh();

    private:
        static std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();
        using Entry = std::array<std::array<std::size_t, 4>, 5>;
        std::array<std::size_t, 4> invalid3{ invalid, invalid, invalid, invalid };

        std::string PrintTetrahedron(std::array<std::size_t, 4> const& tet) const;
        void PrintEntries(std::vector<std::string>& output, std::vector<Entry> const& entries);
        void PrintMesh(std::vector<std::string>& output, DynamicTSManifoldMesh const& mesh);

        void TestTetrahedron4();
    };
}

UnitTestDynamicTSManifoldMesh::UnitTestDynamicTSManifoldMesh()
{
    UTInformation("Mathematics/Meshes/DynamicTSManifoldMesh");

    TestTetrahedron4();
    // This code was also tested by a large mesh output from Delaunay3.
}

std::string UnitTestDynamicTSManifoldMesh::PrintTetrahedron(std::array<std::size_t, 4> const& tet) const
{
    if (tet != invalid3)
    {
        return "(" + std::to_string(tet[0]) + "," + std::to_string(tet[1]) + "," +
            std::to_string(tet[2]) + "," + std::to_string(tet[3]) + ")";
    }
    else
    {
        return "(-,-,-,-)";
    }
}

void UnitTestDynamicTSManifoldMesh::PrintEntries(std::vector<std::string>& output,
    std::vector<Entry> const& entries)
{
    for (std::size_t t = 0; t < entries.size(); ++t)
    {
        auto const& entry = entries[t];
        std::string triStr = PrintTetrahedron(entry[0]);
        std::string entryStr = triStr + ": ";
        for (std::size_t j1 = 1; j1 < 5; ++j1)
        {
            std::string adjStr = PrintTetrahedron(entry[j1]);
            entryStr += adjStr;
            if (j1 < 4)
            {
                entryStr += ", ";
            }
        }
        output.push_back(entryStr);
    }
}

void UnitTestDynamicTSManifoldMesh::PrintMesh(std::vector<std::string>& output,
    DynamicTSManifoldMesh const& mesh)
{
    output.clear();
    auto const& sMap = mesh.GetTetrahedra();
    std::vector<Entry> entries(sMap.size());
    std::size_t t = 0;
    for (auto const& element : sMap)
    {
        auto& entry = entries[t++];
        auto const* tet = element.second.get();
        entry[0] = tet->V;
        for (std::size_t j0 = 0, j1 = 1, i = 3; j0 < 4; ++j0, ++j1, --i)
        {
            auto const* adj = tet->S[i];
            if (adj != nullptr)
            {
                entry[j1] = adj->V;
            }
            else
            {
                entry[j1] = invalid3;
            }
        }
    }
    std::sort(entries.begin(), entries.end());
    PrintEntries(output, entries);
}

void UnitTestDynamicTSManifoldMesh::TestTetrahedron4()
{
    // v0 = (0,0,0), v1 = (1,0,0), v2 = (0,1,0), v3 = (1,1,0),
    // v4 = (0,0,1), v5 = (1,0,1), v6 = (0,1,1), v7 = (1,1,1)
    std::vector<std::array<std::size_t, 4>> tetrahedra(5);
    tetrahedra[0] = { 3, 2, 1, 7 };
    tetrahedra[1] = { 0, 1, 2, 4 };
    tetrahedra[2] = { 7, 2, 1, 4 };
    tetrahedra[3] = { 6, 2, 7, 4 };
    tetrahedra[4] = { 5, 1, 4, 7 };
    DynamicTSManifoldMesh mesh{};
    for (std::size_t t = 0; t < tetrahedra.size(); ++t)
    {
        auto const& tet = tetrahedra[t];
        mesh.Insert(tet[0], tet[1], tet[2], tet[3]);
    }

    auto const& tmap = mesh.GetTriangles();
    UTAssert(tmap.size() == 16, "Incorrect number of triangles.");
    auto const& smap = mesh.GetTetrahedra();
    UTAssert(smap.size() == 5, "Incorrect number of tetrahedra.");

    for (std::size_t t = 0; t < tetrahedra.size(); ++t)
    {
        auto const& tet = tetrahedra[t];
        mesh.Remove(tet[0], tet[1], tet[2], tet[3]);
    }
    UTAssert(tmap.size() == 0, "Incorrect number of triangles.");
    UTAssert(smap.size() == 0, "Incorrect number of tetrahedra.");
    for (std::size_t t = 0; t < tetrahedra.size(); ++t)
    {
        auto const& tet = tetrahedra[t];
        mesh.Insert(tet[0], tet[1], tet[2], tet[3]);
    }
    std::vector<std::string> output{};
    PrintMesh(output, mesh);

    std::vector<std::string> expected(5);
    expected[0] = "(0,1,2,4): (-,-,-,-), (-,-,-,-), (-,-,-,-), (7,2,1,4)";
    expected[1] = "(3,2,1,7): (-,-,-,-), (-,-,-,-), (-,-,-,-), (7,2,1,4)";
    expected[2] = "(5,1,4,7): (-,-,-,-), (-,-,-,-), (-,-,-,-), (7,2,1,4)";
    expected[3] = "(6,2,7,4): (-,-,-,-), (-,-,-,-), (-,-,-,-), (7,2,1,4)";
    expected[4] = "(7,2,1,4): (3,2,1,7), (6,2,7,4), (5,1,4,7), (0,1,2,4)";
    UTAssert(output == expected, "TestTetrahedron4 failed.");
}

#else

#include <GTL/Mathematics/Meshes/DynamicTSManifoldMesh.h>

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DynamicTSManifoldMesh)
