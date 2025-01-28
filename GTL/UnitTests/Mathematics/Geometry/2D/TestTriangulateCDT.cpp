#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/2D/TriangulateCDT.h>
#include <GTL/Utility/Timer.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestTriangulateCDT
    {
    public:
        UnitTestTriangulateCDT();

    private:
        void Test0();
        void Test1();
        void Test2();
        void Test3();
        void Test4();
        void Test5();
        void Test6();
        void Test7();
        void Test8();
        void Profile();

        void Equal(
            std::vector<std::array<std::size_t, 3>>& triangulation0,
            std::vector<std::array<std::size_t, 3>>& triangulation1,
            std::string const& message) const
        {
            UTAssert(triangulation0.size() == triangulation1.size(), "Mismatched sizes.");
            std::sort(triangulation0.begin(), triangulation0.end());
            std::sort(triangulation1.begin(), triangulation1.end());
            for (std::size_t i = 0; i < triangulation0.size(); ++i)
            {
                UTAssert(triangulation0[i] == triangulation1[i], message);
            }
        }
    };
}

UnitTestTriangulateCDT::UnitTestTriangulateCDT()
{
    UTInformation("Mathematics/Geometry/2D/TriangulateCDT");

    Test0();
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    Test6();
    Test7();
    Test8();
    Profile();
}

void UnitTestTriangulateCDT::Test0()
{
    std::vector<Vector2<double>> points(12);
    points[0] = { 0.0, 0.0 };
    points[1] = { 2.0, 0.0 };
    points[2] = { 1.0, 1.0 };
    points[3] = { 2.0, 2.0 };
    points[4] = { 0.0, 2.0 };
    points[5] = points[2];
    points[6] = { 0.375, 0.125 };
    points[7] = { 1.0, 0.125 };
    points[8] = { 1.125, 0.125 };
    points[9] = { 0.375, 0.25 };
    points[10] = { 1.0, 0.25 };
    points[11] = { 1.125, 0.0625 };

    auto tree = std::make_shared<PolygonTree>();
    tree->polygon = { 0, 1, 2, 3, 4, 5 };
    tree->child.resize(1);
    tree->child[0] = std::make_shared<PolygonTree>();
    tree->child[0]->polygon = { 6, 9, 10, 7, 8, 11, 7 };

    TriangulateCDT<double> triangulator;
    PolygonTreeEx output;
    triangulator(points, tree, output);

    std::vector<std::size_t> indices = { 0, 1, 5, 3, 4, 5 };
    UTAssert(
        output.nodes[0].polygon == indices,
        "Incorrect nodes[0].polygon");

    std::vector<std::array<std::size_t, 3>> triangulation =
    {
        {0,1,11}, {0,6,9}, {0,9,5}, {0,11,6}, {1,5,8}, {1,8,11},
        {5,3,4}, {5,9,10}, {5,10,8}, {6,11,7}, {10,7,8}
    };
    Equal(triangulation, output.nodes[0].triangulation,
        "Incorrect nodes[0].triangulation");

    indices = { 6, 9, 10, 7, 8, 11, 7 };
    UTAssert(
        output.nodes[1].polygon == indices,
        "Incorrect nodes[1].polygon");

    triangulation =
    {
        {6,9,10}, {6,10,7}, {7,8,11}
    };
    Equal(triangulation, output.nodes[1].triangulation,
        "Incorrect nodes[1].triangulation");

    triangulation =
    {
        {0,1,11}, {0,6,9}, {0,9,5}, {0,11,6}, {1,5,8}, {1,8,11},
        {5,3,4}, {5,9,10}, {5,10,8}, {6,11,7}, {10,7,8}
    };
    Equal(triangulation, output.interiorTriangles,
        "Incorrect interiorTriangles");

    indices = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    UTAssert(
        output.interiorNodeIndices == indices,
        "Incorrect interiorNodeIndices");

    triangulation =
    {
        {6,9,10}, {6,10,7}, {7,8,11}
    };
    Equal(triangulation, output.exteriorTriangles,
        "Incorrect exteriorTriangles");

    indices = { 1, 1, 1 };
    UTAssert(
        output.exteriorNodeIndices == indices,
        "Incorrect exteriorNodeIndices");

    triangulation =
    {
        {0,1,11}, {0,6,9}, {0,9,5}, {0,11,6}, {1,5,8}, {1,8,11},
        {5,3,4}, {5,9,10}, {5,10,8}, {6,11,7}, {10,7,8},
        {6,9,10}, {6,10,7}, {7,8,11}
    };
    Equal(triangulation, output.insideTriangles,
        "Incorrect insideTriangles");

    indices = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 };
    UTAssert(
        output.insideNodeIndices == indices,
        "Incorrect insideNodeIndices");

    triangulation =
    {
        {1,3,5}, {0,5,4}
    };
    Equal(triangulation, output.outsideTriangles,
        "Incorrect outsideTriangles");

    // All triangles are CCW ordered, so the exterior triangle orderings
    // (triangles in holes) are reversed from their original CW orderings.
    triangulation =
    {
        {0,6,9},    // node 0 (interior)
        {0,11,6},   // node 0 (interior)
        {6,10,9},   // node 1 (exterior)
        {6,7,10},   // node 1 (exterior)
        {0,1,11},   // node 0 (interior)
        {5,9,10},   // node 0 (interior)
        {6,11,7},   // node 0 (interior)
        {0,9,5},    // node 0 (interior)
        {5,10,8},   // node 0 (interior)
        {7,11,8},   // node 1 (exterior)
        {10,7,8},   // node 0 (interior)
        {1,8,11},   // node 0 (interior)
        {1,5,8},    // node 0 (interior)
        {5,3,4},    // node 0 (interior)
        {1,3,5},    // outside
        {0,5,4}     // outside
    };
   Equal(triangulation, output.allTriangles,
        "Incorrect allTriangles");
}

void UnitTestTriangulateCDT::Test1()
{
    // Same geometric structure as Test0, but the bow-tie-like single inner
    // polygon of Test0 is now represented as two simple inner polygons
    // that share a vertex.
    std::vector<Vector2<double>> points(12);
    points[0] = { 0.0, 0.0 };
    points[1] = { 2.0, 0.0 };
    points[2] = { 1.0, 1.0 };
    points[3] = { 2.0, 2.0 };
    points[4] = { 0.0, 2.0 };
    points[5] = { 1.0, 1.0 };
    points[6] = { 0.375, 0.125 };
    points[7] = { 1.0, 0.125 };
    points[8] = { 1.125, 0.125 };
    points[9] = { 0.375, 0.25 };
    points[10] = { 1.0, 0.25 };
    points[11] = { 1.125, 0.0625 };

    auto tree = std::make_shared<PolygonTree>();
    tree->polygon = { 0, 1, 2, 3, 4, 5 };
    tree->child.resize(2);
    tree->child[0] = std::make_shared<PolygonTree>();
    tree->child[0]->polygon = { 6, 9, 10, 7 };
    tree->child[1] = std::make_shared<PolygonTree>();
    tree->child[1]->polygon = { 7, 8, 11 };

    TriangulateCDT<double> triangulator;
    PolygonTreeEx output;
    triangulator(points, tree, output);

    std::vector<std::size_t> indices = { 0, 1, 5, 3, 4, 5 };
    UTAssert(
        output.nodes[0].polygon == indices,
        "Incorrect nodes[0].polygon");

    std::vector<std::array<std::size_t, 3>> triangulation =
    {
        {0,1,11}, {0,6,9}, {0,9,5}, {0,11,6}, {1,5,8}, {1,8,11},
        {5,3,4}, {5,9,10}, {5,10,8}, {6,11,7}, {10,7,8}
    };
    Equal(triangulation, output.nodes[0].triangulation,
        "Incorrect nodes[0].triangulation");

    indices = { 6, 9, 10, 7 };
    UTAssert(
        output.nodes[1].polygon == indices,
        "Incorrect nodes[1].polygon");

    triangulation =
    {
        {6,9,10}, {6,10,7}
    };
    Equal(triangulation, output.nodes[1].triangulation,
        "Incorrect nodes[1].triangulation");

    indices = { 7, 8, 11 };
    UTAssert(
        output.nodes[2].polygon == indices,
        "Incorrect nodes[2].polygon");

    triangulation =
    {
        {7,8,11}
    };
    Equal(triangulation, output.nodes[2].triangulation,
        "Incorrect nodes[2].triangulation");

    triangulation =
    {
        {0,1,11}, {0,6,9}, {0,9,5}, {0,11,6}, {1,5,8}, {1,8,11},
        {5,3,4}, {5,9,10}, {5,10,8}, {6,11,7}, {10,7,8}
    };
    Equal(triangulation, output.interiorTriangles,
        "Incorrect interiorTriangles");

    indices = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    UTAssert(
        output.interiorNodeIndices == indices,
        "Incorrect interiorNodeIndices");

    triangulation =
    {
        {6,9,10}, {6,10,7}, {7,8,11}
    };
    Equal(triangulation, output.exteriorTriangles,
        "Incorrect exteriorTriangles");

    indices = { 1, 1, 2 };
    UTAssert(
        output.exteriorNodeIndices == indices,
        "Incorrect exteriorNodeIndices");

    triangulation =
    {
        {0,1,11}, {0,6,9}, {0,9,5}, {0,11,6}, {1,5,8}, {1,8,11},
        {5,3,4}, {5,9,10}, {5,10,8}, {6,11,7}, {10,7,8}, {6,9,10}, {6,10,7}, {7,8,11}
    };
    Equal(triangulation, output.insideTriangles,
        "Incorrect insideTriangles");

    indices = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2 };
    UTAssert(
        output.insideNodeIndices == indices,
        "Incorrect insideNodeIndices");

    triangulation =
    {
        {1,3,5}, {0,5,4}
    };
    Equal(triangulation, output.outsideTriangles,
        "Incorrect outsideTriangles");

    // All triangles are CCW ordered, so the exterior triangle orderings
    // (triangles in holes) are reversed from their original CW orderings.
    triangulation =
    {
        {0,6,9},    // node 0 (interior)
        {0,11,6},   // node 0 (interior)
        {6,10,9},   // node 1 (exterior)
        {6,7,10},   // node 1 (exterior)
        {0,1,11},   // node 0 (interior)
        {5,9,10},   // node 0 (interior)
        {6,11,7},   // node 0 (interior)
        {0,9,5},    // node 0 (interior)
        {5,10,8},   // node 0 (interior)
        {7,11,8},   // node 2 (exterior)
        {10,7,8},   // node 0 (interior)
        {1,8,11},   // node 0 (interior)
        {1,5,8},    // node 0 (interior)
        {5,3,4},    // node 0 (interior)
        {1,3,5},    // outside
        {0,5,4}     // outside
    };
    Equal(triangulation, output.allTriangles,
        "Incorrect allTriangles");
}

void UnitTestTriangulateCDT::Test2()
{
    // Vertices 0, 5, 8 and 11 are the same. There are 3 triangles
    // embedded in rectangle that share this vertex, but they are
    // actually outside the outer polygon (and not part of the
    // hull-attached triangles for which there are none). See Test2.png
    // upper-left object.
    std::vector<Vector2<double>> points(14);
    points[0] = { 2.0, 0.0 };
    points[1] = { 2.0, 4.0 };
    points[2] = { 0.0, 4.0 };
    points[3] = { 0.0, -4.0 };
    points[4] = { 2.0, -4.0 };
    points[5] = points[0];
    points[6] = { 1.0, -3.0 };
    points[7] = { 1.0, -2.0 };
    points[8] = points[0];
    points[9] = { 1.0, -1.0 };
    points[10] = { 1.0, 1.0 };
    points[11] = points[0];
    points[12] = { 1.0, 2.0 };
    points[13] = { 1.0, 3.0 };

    auto tree = std::make_shared<PolygonTree>();
    tree->polygon = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };

    TriangulateCDT<double> triangulator;
    PolygonTreeEx output;
    triangulator(points, tree, output);

    std::vector<std::size_t> indices = { 11, 1, 2, 3, 4, 11, 6, 7, 11, 9, 10, 11, 12, 13 };
    UTAssert(
        output.nodes[0].polygon == indices,
        "Incorrect nodes[0].polygon");

    std::vector<std::array<std::size_t, 3>> triangulation =
    {
        {11,1,13}, {11,6,4}, {11,9,7}, {11,12,10}, {1,2,13}, {2,3,10},
        {2,10,12}, {2,12,13}, {3,4,6}, {3,6,7}, {3,7,9}, {3,9,10}
    };
    Equal(triangulation, output.nodes[0].triangulation,
        "Incorrect nodes[0].triangulation");

    triangulation =
    {
        {11,1,13}, {11,6,4}, {11,9,7}, {11,12,10}, {1,2,13}, {2,3,10},
        {2,10,12}, {2,12,13}, {3,4,6}, {3,6,7}, {3,7,9}, {3,9,10}
    };
    Equal(triangulation, output.interiorTriangles,
        "Incorrect interiorTriangles");

    indices = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    UTAssert(
        output.interiorNodeIndices == indices,
        "Incorrect interiorNodeIndices");

    UTAssert(
        output.exteriorTriangles.size() == 0,
        "Incorrect exteriorTriangles");

    UTAssert(
        output.exteriorNodeIndices.size() == 0,
        "Incorrect exteriorNodeIndices");

    triangulation =
    {
        {11,1,13}, {11,6,4}, {11,9,7}, {11,12,10}, {1,2,13}, {2,3,10},
        {2,10,12}, {2,12,13}, {3,4,6}, {3,6,7}, {3,7,9}, {3,9,10}
    };
    Equal(triangulation, output.insideTriangles,
        "Incorrect insideTriangles");

    indices = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    UTAssert(
        output.insideNodeIndices == indices,
        "Incorrect insideNodeIndices");

    triangulation =
    {
        {11,10,9}, {11,13,12}, {11,7,6}
    };
    Equal(triangulation, output.outsideTriangles,
        "Incorrect outsideTriangles");

    // All triangles are CCW ordered, so the exterior triangle orderings
    // (triangles in holes) are reversed from their original CW orderings.
    triangulation =
    {
        {2,10,12},  // node 0 (interior)
        {3,6,7},    // node 0 (interior)
        {2,3,10},   // node 0 (interior)
        {3,7,9},    // node 0 (interior)
        {2,12,13},  // node 0 (interior)
        {3,9,10},   // node 0 (interior)
        {3,4,6},    // node 0 (interior)
        {11,9,7},   // node 0 (interior)
        {11,10,9},  // outside
        {11,12,10}, // node 0 (interior)
        {1,2,13},   // node 0 (interior)
        {11,1,13},  // node 0 (interior)
        {11,13,12}, // outside
        {11,6,4},   // node 0 (interior)
        {11,7,6}    // outside
    };
    Equal(triangulation, output.allTriangles,
        "Incorrect insideTriangles");
}

void UnitTestTriangulateCDT::Test3()
{
    // Same dataset as Test2 but with a small rectangle as an inner child.
    // See Test2.png right-most object. No vertices are duplicated in this
    // example.
    std::vector<Vector2<double>> points(15);
    points[0] = { 2.0, 0.0 };
    points[1] = { 2.0, 4.0 };
    points[2] = { 0.0, 4.0 };
    points[3] = { 0.0, -4.0 };
    points[4] = { 2.0, -4.0 };
    points[5] = { 1.0, -3.0 };
    points[6] = { 1.0, -2.0 };
    points[7] = { 1.0, -1.0 };
    points[8] = { 1.0, 1.0 };
    points[9] = { 1.0, 2.0 };
    points[10] = { 1.0, 3.0 };
    points[11] = { 0.25, -0.25 };
    points[12] = { 0.25, 0.25 };
    points[13] = { 0.75, 0.25 };
    points[14] = { 0.75, -0.25 };

    auto tree = std::make_shared<PolygonTree>();
    tree->polygon = { 0, 1, 2, 3, 4, 0, 5, 6, 0, 7, 8, 0, 9, 10 };
    tree->child.resize(1);
    tree->child[0] = std::make_shared<PolygonTree>();
    tree->child[0]->polygon = { 11, 12, 13, 14 };

    TriangulateCDT<double> triangulator;
    PolygonTreeEx output;
    triangulator(points, tree, output);

    std::vector<std::size_t> indices = { 0, 1, 2, 3, 4, 0, 5, 6, 0, 7, 8, 0, 9, 10 };
    UTAssert(
        output.nodes[0].polygon == indices,
        "Incorrect nodes[0].polygon");

    std::vector<std::array<std::size_t, 3>> triangulation =
    {
        {0,1,10}, {0,5,4}, {0,7,6}, {0,9,8}, {1,2,10}, {2,3,12}, {2,9,10},
        {2,12,9}, {3,4,5}, {3,5,6}, {3,6,11}, {3,11,12}, {6,7,11}, {7,8,13},
        {7,13,14}, {7,14,11}, {8,9,12}, {8,12,13}
    };
    Equal(triangulation, output.nodes[0].triangulation,
        "Incorrect nodes[0].triangulation");

    indices = { 11, 12, 13, 14 };
    UTAssert(
        output.nodes[1].polygon == indices,
        "Incorrect nodes[1].polygon");

    triangulation =
    {
        {11,12,13}, {11,13,14}
    };
    Equal(triangulation, output.nodes[1].triangulation,
        "Incorrect nodes[1].triangulation");

    triangulation =
    {
        {0,1,10}, {0,5,4}, {0,7,6}, {0,9,8}, {1,2,10}, {2,3,12}, {2,9,10},
        {2,12,9}, {3,4,5}, {3,5,6}, {3,6,11}, {3,11,12}, {6,7,11}, {7,8,13},
        {7,13,14}, {7,14,11}, {8,9,12}, {8,12,13}
    };
    Equal(triangulation, output.interiorTriangles,
        "Incorrect interiorTriangles");

    indices = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    UTAssert(
        output.interiorNodeIndices == indices,
        "Incorrect interiorNodeIndices");

    triangulation =
    {
        {11,12,13}, {11,13,14}
    };
    Equal(triangulation, output.exteriorTriangles,
        "Incorrect exteriorTriangles");

    indices = { 1, 1 };
    UTAssert(
        output.exteriorNodeIndices == indices,
        "Incorrect exteriorNodeIndices");

    triangulation =
    {
        {0,1,10}, {0,5,4}, {0,7,6}, {0,9,8}, {1,2,10}, {2,3,12}, {2,9,10},
        {2,12,9}, {3,4,5}, {3,5,6}, {3,6,11}, {3,11,12}, {6,7,11}, {7,8,13},
        {7,13,14}, {7,14,11}, {8,9,12}, {8,12,13},{11,12,13}, {11,13,14}
    };
    Equal(triangulation, output.insideTriangles,
        "Incorrect insideTriangles");

    indices = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 };
    UTAssert(
        output.insideNodeIndices == indices,
        "Incorrect insideNodeIndices");

    triangulation =
    {
        {0,8,7}, {0,10,9}, {0,6,5}
    };
    Equal(triangulation, output.outsideTriangles,
        "Incorrect outsideTriangles");

    // All triangles are CCW ordered, so the exterior triangle orderings
    // (triangles in holes) are reversed from their original CW orderings.
    triangulation =
    {
        {0,8,7},    // node 0 (interior)
        {2,3,12},   // node 0 (interior)
        {3,6,11},   // node 0 (interior)
        {3,11,12},  // node 0 (interior)
        {11,13,12}, // node 1 (exterior)
        {11,14,13}, // node 1 (exterior)
        {3,5,6},    // node 0 (interior)
        {7,14,11},  // node 0 (interior)
        {6,7,11},   // node 0 (interior)
        {8,12,13},  // node 0 (interior)
        {2,12,9},   // node 0 (interior)
        {0,10,9},   // node 0 (interior)
        {8,9,12},   // node 0 (interior)
        {2,9,10},   // node 0 (interior)
        {3,4,5},    // node 0 (interior)
        {0,7,6},    // node 0 (interior)
        {0,9,8},    // node 0 (interior)
        {1,2,10},   // node 0 (interior)
        {0,1,10},   // node 0 (interior)
        {0,5,4},    // node 0 (interior)
        {0,6,5},    // node 0 (interior)
        {7,8,13},   // node 0 (interior)
        {7,13,14}   // node 0 (interior)
    };
    Equal(triangulation, output.allTriangles,
        "Incorrect allTriangles");
}

void UnitTestTriangulateCDT::Test4()
{
    // Same dataset as Test3 but with a star-shaped polygon as an outer
    // grandchild nested in an inner child. Vertices are shared between the
    // small rectangle and its nested polygon.
    std::vector<Vector2<double>> points(19);
    points[0] = { 2.0, 0.0 };
    points[1] = { 2.0, 4.0 };
    points[2] = { 0.0, 4.0 };
    points[3] = { 0.0, -4.0 };
    points[4] = { 2.0, -4.0 };
    points[5] = { 1.0, -3.0 };
    points[6] = { 1.0, -2.0 };
    points[7] = { 1.0, -1.0 };
    points[8] = { 1.0, 1.0 };
    points[9] = { 1.0, 2.0 };
    points[10] = { 1.0, 3.0 };
    points[11] = { 0.25, -0.25 };
    points[12] = { 0.25, 0.25 };
    points[13] = { 0.75, 0.25 };
    points[14] = { 0.75, -0.25 };
    points[15] = { 0.375, 0.0 };
    points[16] = { 0.5, -0.125 };
    points[17] = { 0.625, 0.0 };
    points[18] = { 0.625, 0.125 };

    auto tree = std::make_shared<PolygonTree>();
    tree->polygon = { 0, 1, 2, 3, 4, 0, 5, 6, 0, 7, 8, 0, 9, 10 };
    tree->child.resize(1);
    tree->child[0] = std::make_shared<PolygonTree>();
    tree->child[0]->polygon = { 11, 12, 13, 14 };
    tree->child[0]->child.resize(1);
    tree->child[0]->child[0] = std::make_shared<PolygonTree>();
    tree->child[0]->child[0]->polygon = { 11, 16, 14, 17, 13, 18, 12, 15 };

    TriangulateCDT<double> triangulator;
    PolygonTreeEx output;
    triangulator(points, tree, output);

    std::vector<std::size_t> indices = { 0, 1, 2, 3, 4, 0, 5, 6, 0, 7, 8, 0, 9, 10 };
    UTAssert(
        output.nodes[0].polygon == indices,
        "Incorrect nodes[0].polygon");

    std::vector<std::array<std::size_t, 3>> triangulation =
    {
        {0,1,10}, {0,5,4}, {0,7,6}, {0,9,8}, {1,2,10}, {2,3,12}, {2,9,10},
        {2,12,9}, {3,4,5}, {3,5,6}, {3,6,11}, {3,11,12}, {6,7,11}, {7,8,13},
        {7,13,14}, {7,14,11}, {8,9,12}, {8,12,13}
    };
    Equal(triangulation, output.nodes[0].triangulation,
        "Incorrect nodes[0].triangulation");

    indices = { 11, 12, 13, 14 };
    UTAssert(
        output.nodes[1].polygon == indices,
        "Incorrect nodes[1].polygon");

    triangulation =
    {
        {11,16,14}, {11,12,15}, {12,13,18}, {13,14,17}
    };
    Equal(triangulation, output.nodes[1].triangulation,
        "Incorrect nodes[1].triangulation");

    indices = { 11, 16, 14, 17, 13, 18, 12, 15 };
    UTAssert(
        output.nodes[2].polygon == indices,
        "Incorrect nodes[2].polygon");

    triangulation =
    {
        {11,16,15}, {12,15,18}, {13,18,17}, {14,17,16}, {16,17,15}, {17,18,15}
    };
    Equal(triangulation, output.nodes[2].triangulation,
        "Incorrect nodes[2].triangulation");

    triangulation =
    {
        {0,1,10}, {0,5,4}, {0,7,6}, {0,9,8}, {1,2,10}, {2,3,12}, {2,9,10},
        {2,12,9}, {3,4,5}, {3,5,6}, {3,6,11}, {3,11,12}, {6,7,11}, {7,8,13},
        {7,13,14}, {7,14,11}, {8,9,12}, {8,12,13}, {11,16,15}, {12,15,18},
        {13,18,17}, {14,17,16}, {16,17,15}, {17,18,15}
    };
    Equal(triangulation, output.interiorTriangles,
        "Incorrect interiorTriangles");

    indices = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2
    };
    UTAssert(
        output.interiorNodeIndices == indices,
        "Incorrect interiorNodeIndices");

    triangulation =
    {
        {11,16,14}, {11,12,15}, {12,13,18}, {13,14,17}
    };
    Equal(triangulation, output.exteriorTriangles,
        "Incorrect exteriorTriangles");

    indices = { 1, 1, 1, 1 };
    UTAssert(
        output.exteriorNodeIndices == indices,
        "Incorrect exteriorNodeIndices");

    triangulation =
    {
        {0,1,10}, {0,5,4}, {0,7,6}, {0,9,8}, {1,2,10}, {2,3,12}, {2,9,10},
        {2,12,9}, {3,4,5}, {3,5,6}, {3,6,11}, {3,11,12}, {6,7,11}, {7,8,13},
        {7,13,14}, {7,14,11}, {8,9,12}, {8,12,13},
        {11,16,14}, {11,12,15}, {12,13,18}, {13,14,17},
        {11,16,15}, {12,15,18}, {13,18,17}, {14,17,16}, {16,17,15}, {17,18,15}
    };
    Equal(triangulation, output.insideTriangles,
        "Incorrect insideTriangles");

    triangulation =
    {
        {0,8,7}, {0,10,9}, {0,6,5}
    };
    Equal(triangulation, output.outsideTriangles,
        "Incorrect outsideTriangles");

    // All triangles are CCW ordered, so the exterior triangle orderings
    // (triangles in holes) are reversed from their original CW orderings.
    triangulation =
    {
        {0,8,7},    // outside
        {3,6,11},   // node 0 (interior)
        {2,3,12},   // node 0 (interior)
        {0,6,5},    // outside
        {11,15,12}, // node 1 (exterior)
        {3,11,12},  // node 0 (interior)
        {11,16,15}, // node 2 (interior)
        {2,9,10},   // node 0 (interior)
        {16,17,15}, // node 2 (interior)
        {17,18,15}, // node 2 (interior)
        {12,15,18}, // node 2 (interior)
        {14,17,16}, // node 2 (interior)
        {11,14,16}, // node 1 (exterior)
        {13,18,17}, // node 2 (interior)
        {6,7,11},   // node 0 (interior)
        {13,17,14}, // node 1 (exterior)
        {12,18,13}, // node 1 (exterior)
        {3,5,6},    // node 0 (interior)
        {7,14,11},  // node 0 (interior)
        {8,12,13},  // node 0 (interior)
        {2,12,9},   // node 0 (interior)
        {0,10,9},   // outside
        {8,9,12},   // node 0 (interior)
        {3,4,5},    // node 0 (interior)
        {0,7,6},    // node 0 (interior)
        {0,9,8},    // node 0 (interior)
        {1,2,10},   // node 0 (interior)
        {0,1,10},   // node 0 (interior)
        {0,5,4},    // node 0 (interior)
        {7,8,13},   // node 0 (interior)
        {7,13,14}   // node 0 (interior)
    };
    Equal(triangulation, output.allTriangles,
        "Incorrect allTriangles");
}

void UnitTestTriangulateCDT::Test5()
{
    // An outer polygon (rectangle) with two inner polygons (triangles).
    // A vertex of the second inner polygon is coincident with an edge
    // of the first inner polygon, but that vertex is not part of the
    // polygon indices for the first inner polygon.
    std::vector<Vector2<double>> points(10);
    points[0] = { 0.0, 0.0 };
    points[1] = { 4.0, 0.0 };
    points[2] = { 4.0, 4.0 };
    points[3] = { 0.0, 4.0 };
    points[4] = { 1.0, 2.0 };
    points[5] = { 2.0, 3.0 };
    points[6] = { 2.0, 1.0 };
    points[7] = { 2.0, 2.0 };
    points[8] = { 3.0, 3.0 };
    points[9] = { 3.0, 1.0 };

    auto tree = std::make_shared<PolygonTree>();
    tree->polygon = { 0, 1, 2, 3 };
    tree->child.resize(2);
    tree->child[0] = std::make_shared<PolygonTree>();
    tree->child[0]->polygon = { 4, 5, 6 };
    tree->child[1] = std::make_shared<PolygonTree>();
    tree->child[1]->polygon = { 7, 8, 9 };

    TriangulateCDT<double> triangulator;
    PolygonTreeEx output;
    triangulator(points, tree, output);

    std::vector<std::size_t> indices = { 0, 1, 2, 3 };
    UTAssert(
        output.nodes[0].polygon == indices,
        "Incorrect nodes[0].polygon");

    std::vector<std::array<std::size_t, 3>> triangulation =
    {
        {0,1,6}, {0,4,3}, {0,6,4}, {1,2,9}, {1,9,6}, {2,3,5}, {2,5,8},
        {2,8,9}, {3,4,5}, {5,7,8}, {6,9,7}
    };
    Equal(triangulation, output.nodes[0].triangulation,
        "Incorrect nodes[0].triangulation");

    indices = { 4, 5, 7, 6 };  // 7 was inserted into child[0]->polygon
    UTAssert(
        output.nodes[1].polygon == indices,
        "Incorrect nodes[1].polygon");

    triangulation =
    {
        {4,7,6}, {4,5,7}
    };
    Equal(triangulation, output.nodes[1].triangulation,
        "Incorrect nodes[1].triangulation");

    indices = { 7, 8, 9 };
    UTAssert(
        output.nodes[2].polygon == indices,
        "Incorrect nodes[2].polygon");

    triangulation =
    {
        {7,8,9}
    };
    Equal(triangulation, output.nodes[2].triangulation,
        "Incorrect nodes[2].triangulation");

    triangulation =
    {
        {0,1,6}, {0,4,3}, {0,6,4}, {1,2,9}, {1,9,6}, {2,3,5}, {2,5,8},
        {2,8,9}, {3,4,5}, {5,7,8}, {6,9,7}
    };
    Equal(triangulation, output.interiorTriangles,
        "Incorrect interiorTriangles");

    indices = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    UTAssert(
        output.interiorNodeIndices == indices,
        "Incorrect interiorNodeIndices");

    triangulation =
    {
        {4,7,6}, {4,5,7}, {7,8,9}
    };
    Equal(triangulation, output.exteriorTriangles,
        "Incorrect exteriorTriangles");

    indices = { 1, 1, 2 };
    UTAssert(
        output.exteriorNodeIndices == indices,
        "Incorrect exteriorNodeIndices");

    triangulation =
    {
        {0,1,6}, {0,4,3}, {0,6,4}, {1,2,9}, {1,9,6}, {2,3,5}, {2,5,8},
        {2,8,9}, {3,4,5}, {5,7,8}, {6,9,7}, {4,7,6}, {4,5,7}, {7,8,9}
    };
    Equal(triangulation, output.insideTriangles,
        "Incorrect insideTriangles");

    indices = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2 };
    UTAssert(
        output.insideNodeIndices == indices,
        "Incorrect insideNodeIndices");

    UTAssert(
        output.outsideTriangles.size() == 0,
        "Incorrect outsideTriangles");

    // All triangles are CCW ordered, so the exterior triangle orderings
    // (triangles in holes) are reversed from their original CW orderings.
    triangulation =
    {
        {0,4,3},    // node 0 (interior)
        {3,4,5},    // node 0 (interior)
        {0,6,4},    // node 0 (interior)
        {4,6,7},    // node 1 (exterior)
        {7,9,8},    // node 1 (exterior)
        {4,7,5},    // node 1 (exterior)
        {6,9,7},    // node 0 (interior)
        {5,7,8},    // node 0 (interior)
        {1,9,6},    // node 0 (interior)
        {0,1,6},    // node 0 (interior)
        {2,8,9},    // node 0 (interior)
        {1,2,9},    // node 0 (interior)
        {2,3,5},    // node 0 (interior)
        {2,5,8}     // node 0 (interior)
    };
    Equal(triangulation, output.allTriangles,
        "Incorrect allTriangles");
}

void UnitTestTriangulateCDT::Test6()
{
    // An outer polygon (rectangle) with two inner polygons (triangles).
    // An edge of the second inner polygon is coincident with an edge
    // of the first inner polygon, but that edge is not part of the
    // polygon indices for the first inner polygon.
    std::vector<Vector2<double>> points(10);
    points[0] = { 0.0, 0.0 };
    points[1] = { 4.0, 0.0 };
    points[2] = { 4.0, 4.0 };
    points[3] = { 0.0, 4.0 };
    points[4] = { 1.0, 2.0 };
    points[5] = { 2.0, 3.0 };
    points[6] = { 2.0, 1.0 };
    points[7] = { 2.0, 2.5 };
    points[8] = { 3.0, 2.0 };
    points[9] = { 2.0, 1.5 };

    auto tree = std::make_shared<PolygonTree>();
    tree->polygon = { 0, 1, 2, 3 };
    tree->child.resize(2);
    tree->child[0] = std::make_shared<PolygonTree>();
    tree->child[0]->polygon = { 4, 5, 6 };
    tree->child[1] = std::make_shared<PolygonTree>();
    tree->child[1]->polygon = { 7, 8, 9 };

    TriangulateCDT<double> triangulator;
    PolygonTreeEx output;
    triangulator(points, tree, output);

    std::vector<std::size_t> indices = { 0, 1, 2, 3 };
    UTAssert(
        output.nodes[0].polygon == indices,
        "Incorrect nodes[0].polygon");

    std::vector<std::array<std::size_t, 3>> triangulation =
    {
        {0,1,6}, {0,4,3}, {0,6,4}, {1,2,8}, {1,8,6}, {2,3,5}, {2,5,8},
        {3,4,5}, {5,7,8}, {6,8,9}
    };
    Equal(triangulation, output.nodes[0].triangulation,
        "Incorrect nodes[0].triangulation");

    indices = { 4, 5, 7, 9, 6 };
    UTAssert(
        output.nodes[1].polygon == indices,
        "Incorrect nodes[1].polygon");

    triangulation =
    {
        {4,9,6}, {4,5,7}, {4,7,9}
    };
    Equal(triangulation, output.nodes[1].triangulation,
        "Incorrect nodes[1].triangulation");

    indices = { 7, 8, 9 };
    UTAssert(
        output.nodes[2].polygon == indices,
        "Incorrect nodes[2].polygon");

    triangulation =
    {
        {7,8,9}
    };
    Equal(triangulation, output.nodes[2].triangulation,
        "Incorrect nodes[2].triangulation");

    triangulation =
    {
        {0,1,6}, {0,4,3}, {0,6,4}, {1,2,8}, {1,8,6}, {2,3,5}, {2,5,8},
        {3,4,5}, {5,7,8}, {6,8,9}
    };
    Equal(triangulation, output.interiorTriangles,
        "Incorrect interiorTriangles");

    indices = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    UTAssert(
        output.interiorNodeIndices == indices,
        "Incorrect interiorNodeIndices");

    triangulation =
    {
        {4,9,6}, {4,5,7}, {4,7,9}, {7,8,9}
    };
    Equal(triangulation, output.exteriorTriangles,
        "Incorrect exteriorTriangles");

    indices = { 1, 1, 1, 2 };
    UTAssert(
        output.exteriorNodeIndices == indices,
        "Incorrect exteriorNodeIndices");

    triangulation =
    {
        {0,1,6}, {0,4,3}, {0,6,4}, {1,2,8}, {1,8,6}, {2,3,5}, {2,5,8},
        {3,4,5}, {5,7,8}, {6,8,9},
        {4,9,6}, {4,5,7}, {4,7,9}, {7,8,9}
    };
    Equal(triangulation, output.insideTriangles,
        "Incorrect insideTriangles");

    indices = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2 };
    UTAssert(
        output.insideNodeIndices == indices,
        "Incorrect insideNodeIndices");

    UTAssert(
        output.outsideTriangles.size() == 0,
        "Incorrect outsideTriangles");

    // All triangles are CCW ordered, so the exterior triangle orderings
    // (triangles in holes) are reversed from their original CW orderings.
    triangulation =
    {
        {0,4,3},    // node 0 (interior)
        {3,4,5},    // node 0 (interior)
        {6,8,9},    // node 0 (interior)
        {0,6,4},    // node 0 (interior)
        {4,6,9},    // node 1 (exterior)
        {5,7,8},    // node 0 (interior)
        {4,9,7},    // node 1 (exterior)
        {7,9,8},    // node 1 (exterior)
        {4,7,5},    // node 1 (exterior)
        {1,8,6},    // node 0 (interior)
        {0,1,6},    // node 0 (interior)
        {1,2,8},    // node 0 (interior)
        {2,3,5},    // node 0 (interior)
        {2,5,8}     // node 0 (interior)
    };
    Equal(triangulation, output.allTriangles,
        "Incorrect allTriangles");
}

void UnitTestTriangulateCDT::Test7()
{
    std::vector<Vector2<double>> points(22);
    points[0] = { 0.20258949138391005, 0.8396238654719022 };
    points[1] = { 0.20258918996554820, 0.8396250329226389 };
    points[2] = { 0.20258951843651343, 0.8396250711453495 };
    points[3] = { 0.20258919589966767, 0.8396263194048305 };
    points[4] = { 0.20258754935605133, 0.8396261286403432 };
    points[5] = { 0.20258751322773583, 0.8396262680921505 };
    points[6] = { 0.20258671875386030, 0.8396261759387660 };
    points[7] = { 0.20258675924549896, 0.8396260199063308 };
    points[8] = { 0.20258550225937170, 0.8396258743458712 };
    points[9] = { 0.20258555933163822, 0.8396256535617209 };
    points[10] = { 0.20258406864592410, 0.8396254809486579 };
    points[11] = { 0.20258412327472970, 0.8396252353808321 };
    points[12] = { 0.20258415972346505, 0.8396250711453495 };
    points[13] = { 0.20258416820622066, 0.8396250329226389 };
    points[14] = { 0.20258435106964215, 0.8396242089526990 };
    points[15] = { 0.20258442729824064, 0.8396238654719022 };
    points[16] = { 0.20258446169407165, 0.8396237104866646 };
    points[17] = { 0.20258455419652200, 0.8396232933529734 };
    points[18] = { 0.20258588466101080, 0.8396254106118890 };
    points[19] = { 0.20258585149975500, 0.8396256478021342 };
    points[20] = { 0.20258831834811977, 0.8396259335125329 };
    points[21] = { 0.20258876393067780, 0.8396242089526990 };

    auto tree = std::make_shared<PolygonTree>();
    tree->polygon = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
    tree->child.resize(1);
    tree->child[0] = std::make_shared<PolygonTree>();
    tree->child[0]->polygon = { 15, 14, 13, 12, 11, 18, 19, 20, 21, 16 };

    TriangulateCDT<double> triangulator;
    PolygonTreeEx output;
    triangulator(points, tree, output);

    std::vector<std::size_t> indices = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
    UTAssert(
        output.nodes[0].polygon == indices,
        "Incorrect nodes[0].polygon");

    std::vector<std::array<std::size_t, 3>> triangulation =
    {
        {0,1,21},
        {0,21,17},
        {1,2,3},
        {1,3,20},
        {1,20,21},
        {3,4,20},
        {4,5,7},
        {4,7,20},
        {5,6,7},
        {7,8,19},
        {7,19,20},
        {8,9,19},
        {9,10,11},
        {9,11,18},
        {9,18,19},
        {16,17,21}
    };
    Equal(triangulation, output.nodes[0].triangulation,
        "Incorrect nodes[0].triangulation");

    indices = { 15, 14, 13, 12, 11, 18, 19, 20, 21, 16 };
    UTAssert(
        output.nodes[1].polygon == indices,
        "Incorrect nodes[1].polygon");

    triangulation =
    {
        {11,18,12}, {12,18,13}, {13,18,14}, {14,18,15}, {15,18,16},
        {16,18,21}, {18,19,20}, {18,20,21}
    };
    Equal(triangulation, output.nodes[1].triangulation,
        "Incorrect nodes[1].triangulation");

    triangulation =
    {
        {0,1,21},
        {0,21,17},
        {1,2,3},
        {1,3,20},
        {1,20,21},
        {3,4,20},
        {4,5,7},
        {4,7,20},
        {5,6,7},
        {7,8,19},
        {7,19,20},
        {8,9,19},
        {9,10,11},
        {9,11,18},
        {9,18,19},
        {16,17,21}
    };
    Equal(triangulation, output.interiorTriangles,
        "Incorrect interiorTriangles");

    indices = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    UTAssert(
        output.interiorNodeIndices == indices,
        "Incorrect interiorNodeIndices");

    triangulation =
    {
        {11,18,12}, {12,18,13}, {13,18,14}, {14,18,15}, {15,18,16},
        {16,18,21}, {18,19,20}, {18,20,21}
    };
    Equal(triangulation, output.exteriorTriangles,
        "Incorrect exteriorTriangles");

    indices = { 1, 1, 1, 1, 1, 1, 1, 1 };
    UTAssert(
        output.exteriorNodeIndices == indices,
        "Incorrect exteriorNodeIndices");

    triangulation =
    {
        {0,1,21},
        {0,21,17},
        {1,2,3},
        {1,3,20},
        {1,20,21},
        {3,4,20},
        {4,5,7},
        {4,7,20},
        {5,6,7},
        {7,8,19},
        {7,19,20},
        {8,9,19},
        {9,10,11},
        {9,11,18},
        {9,18,19},
        {11,18,12},
        {12,18,13},
        {13,18,14},
        {14,18,15},
        {15,18,16},
        {16,17,21},
        {16,18,21},
        {18,19,20},
        {18,20,21}
    }; 
    Equal(triangulation, output.insideTriangles,
        "Incorrect insideTriangles");

    indices = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1
    };
    UTAssert(
        output.insideNodeIndices == indices,
        "Incorrect insideNodeIndices");

    triangulation =
    {
        {10,12,11}, {14,17,15}, {10,13,12}, {10,17,14}, {10,14,13},
        {15,17,16}, {8,10,9}, {6,8,7}, {0,2,1}, {3,5,4}
    };
    Equal(triangulation, output.outsideTriangles,
        "Incorrect outsideTriangles");

    // All triangles are CCW ordered, so the exterior triangle orderings
    // (triangles in holes) are reversed from their original CW orderings.
    triangulation =
    {
        {0,1,21},   // node 0 (interior)
        {0,2,1},    // outside
        {0,21,17},  // node 0 (interior)
        {1,2,3},    // node 0 (interior)
        {1,3,20},   // node 0 (interior)
        {1,20,21},  // node 0 (interior)
        {3,4,20},   // node 0 (interior)
        {3,5,4},    // outside
        {4,5,7},    // node 0 (interior)
        {4,7,20},   // node 0 (interior)
        {5,6,7},    // node 0 (interior)
        {6,8,7},    // outside
        {7,8,19},   // node 0 (interior)
        {7,19,20},  //
        {8,9,19},   //
        {8,10,9},   //
        {9,10,11},  //
        {9,11,18},  //
        {9,18,19},  //
        {10,12,11}, // outside
        {10,13,12}, // outside
        {10,14,13}, // outside
        {10,17,14}, // outside
        {11,12,18}, // node 1 (exterior)
        {12,13,18}, // node 1 (exterior)
        {13,14,18}, // node 1 (exterior)
        {14,15,18}, // node 1 (exterior)
        {14,17,15}, // outside
        {15,16,18}, // node 1 (exterior)
        {15,17,16}, // outside
        {16,17,21}, // node 0 (interior)
        {16,21,18}, // node 1 (exterior)
        {18,20,19}, // node 1 (exterior)
        {18,21,20}  // node 1 (exterior)
    };
    Equal(triangulation, output.allTriangles,
        "Incorrect allTriangles");
}

void UnitTestTriangulateCDT::Test8()
{
    std::vector<Vector2<double>> points(24);
    points[0] = { 129.9250909375382, 1.0 };
    points[1] = { 112.5477434107041, 1.0 };
    points[2] = { 112.5477434107041, 0.0 };
    points[3] = { 129.9250909375382, 0.0 };
    points[4] = { 113.74060251435496, 0.8387271627767082 };
    points[5] = { 113.74060251435496, 0.909818344592056 };
    points[6] = { 128.4709579922158, 0.909818344592056 };
    points[7] = { 128.4709579922158, 0.8387271627767082 };
    points[8] = { 113.52997782186625, 0.22320245720476972 };
    points[9] = { 113.52997782186625, 0.7526325008314533 };
    points[10] = { 128.68158268470452, 0.7526325008314533 };
    points[11] = { 128.68158268470452, 0.22320245720476972 };
    points[12] = { 123.55972508323318, 0.032896905937090194 };
    points[13] = { 123.55972508323318, 0.1814887038764954 };
    points[14] = { 128.68158268470452, 0.1814887038764954 };
    points[15] = { 128.68158268470452, 0.032896905937090194 };
    points[16] = { 113.51246911826932, 0.032896905937090194 };
    points[17] = { 113.51246911826932, 0.18148660359038984 };
    points[18] = { 118.65183542333759, 0.18148660359038984 };
    points[19] = { 118.65183542333759, 0.032896905937090194 };
    points[20] = { 119.22520834295486, 0.032896905937090194 };
    points[21] = { 119.22520834295486, 0.18128867426191317 };
    points[22] = { 123.07064041675179, 0.18128867426191317 };
    points[23] = { 123.07064041675179, 0.032896905937090194 };

    auto tree = std::make_shared<PolygonTree>();
    tree->polygon = { 0, 1, 2, 3 };
    tree->child.resize(5);
    tree->child[0] = std::make_shared<PolygonTree>();
    tree->child[0]->polygon = { 4, 5, 6, 7 };
    tree->child[1] = std::make_shared<PolygonTree>();
    tree->child[1]->polygon = { 8, 9, 10, 11 };
    tree->child[2] = std::make_shared<PolygonTree>();
    tree->child[2]->polygon = { 12, 13, 14, 15 };
    tree->child[3] = std::make_shared<PolygonTree>();
    tree->child[3]->polygon = { 16, 17, 18, 19 };
    tree->child[4] = std::make_shared<PolygonTree>();
    tree->child[4]->polygon = { 20, 21, 22, 23 };

    TriangulateCDT<double> triangulator;
    PolygonTreeEx output;
    triangulator(points, tree, output);

    std::vector<std::size_t> indices = { 0, 1, 2, 3 };
    UTAssert(
        output.nodes[0].polygon == indices,
        "Incorrect nodes[0].polygon");

    std::vector<std::array<std::size_t, 3>> triangulation =
    {
        {0,1,6},
        {0,6,10},
        {0,10,3},
        {1,2,9},
        {1,5,6},
        {1,9,5},
        {2,3,23},
        {2,8,9},
        {2,16,17},
        {2,17,8},
        {2,19,16},
        {2,20,19},
        {2,23,20},
        {3,10,11},
        {3,11,14},
        {3,12,23},
        {3,14,15},
        {3,15,12},
        {4,5,9},
        {4,9,7},
        {6,7,10},
        {7,9,10},
        {8,13,11},
        {8,17,18},
        {8,18,21},
        {8,21,22},
        {8,22,13},
        {11,13,14},
        {12,13,22},
        {12,22,23},
        {18,19,21},
        {19,20,21}
    };
    Equal(triangulation, output.nodes[0].triangulation,
        "Incorrect nodes[0].triangulation");

    indices = { 4, 5, 6, 7 };
    UTAssert(
        output.nodes[1].polygon == indices,
        "Incorrect nodes[1].polygon");

    triangulation =
    {
        {4,5,6},
        {4,6,7}
    };
    Equal(triangulation, output.nodes[1].triangulation,
        "Incorrect nodes[1].triangulation");

    indices = { 8, 9, 10, 11 };
    UTAssert(
        output.nodes[2].polygon == indices,
        "Incorrect nodes[2].polygon");

    triangulation =
    {
        {8,9,10},
        {8,10,11}
    };
    Equal(triangulation, output.nodes[2].triangulation,
        "Incorrect nodes[2].triangulation");

    indices = { 12, 13, 14, 15 };
    UTAssert(
        output.nodes[3].polygon == indices,
        "Incorrect nodes[3].polygon");

    triangulation =
    {
        {12,13,14},
        {12,14,15}
    };
    Equal(triangulation, output.nodes[3].triangulation,
        "Incorrect nodes[3].triangulation");

    indices = { 16, 17, 18, 19 };
    UTAssert(
        output.nodes[4].polygon == indices,
        "Incorrect nodes[4].polygon");

    triangulation =
    {
        {16,17,18},
        {16,18,19}
    };
    Equal(triangulation, output.nodes[4].triangulation,
        "Incorrect nodes[4].triangulation");

    indices = { 20, 21, 22, 23 };
    UTAssert(
        output.nodes[5].polygon == indices,
        "Incorrect nodes[5].polygon");

    triangulation =
    {
        {20,21,22},
        {20,22,23}
    };
    Equal(triangulation, output.nodes[5].triangulation,
        "Incorrect nodes[5].triangulation");

    triangulation =
    {
        {0,1,6},
        {0,6,10},
        {0,10,3},
        {1,2,9},
        {1,5,6},
        {1,9,5},
        {2,3,23},
        {2,8,9},
        {2,16,17},
        {2,17,8},
        {2,19,16},
        {2,20,19},
        {2,23,20},
        {3,10,11},
        {3,11,14},
        {3,12,23},
        {3,14,15},
        {3,15,12},
        {4,5,9},
        {4,9,7},
        {6,7,10},
        {7,9,10},
        {8,13,11},
        {8,17,18},
        {8,18,21},
        {8,21,22},
        {8,22,13},
        {11,13,14},
        {12,13,22},
        {12,22,23},
        {18,19,21},
        {19,20,21}
    };
    Equal(triangulation, output.interiorTriangles,
        "Incorrect interiorTriangles");

    indices = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    UTAssert(
        output.interiorNodeIndices == indices,
        "Incorrect interiorNodeIndices");

    triangulation =
    {
        {4,5,6},
        {4,6,7},
        {8,9,10},
        {8,10,11},
        {12,13,14},
        {12,14,15},
        {16,17,18},
        {16,18,19},
        {20,21,22},
        {20,22,23}
    };
    Equal(triangulation, output.exteriorTriangles,
        "Incorrect exteriorTriangles");

    indices = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
    UTAssert(
        output.exteriorNodeIndices == indices,
        "Incorrect exteriorNodeIndices");

    triangulation =
    {
        {0,1,6},
        {0,6,10},
        {0,10,3},
        {1,2,9},
        {1,5,6},
        {1,9,5},
        {2,3,23},
        {2,8,9},
        {2,16,17},
        {2,17,8},
        {2,19,16},
        {2,20,19},
        {2,23,20},
        {3,10,11},
        {3,11,14},
        {3,12,23},
        {3,14,15},
        {3,15,12},
        {4,5,6},
        {4,5,9},
        {4,6,7},
        {4,9,7},
        {6,7,10},
        {7,9,10},
        {8,9,10},
        {8,10,11},
        {8,13,11},
        {8,17,18},
        {8,18,21},
        {8,21,22},
        {8,22,13},
        {11,13,14},
        {12,13,14},
        {12,13,22},
        {12,14,15},
        {12,22,23},
        {16,17,18},
        {16,18,19},
        {18,19,21},
        {19,20,21},
        {20,21,22},
        {20,22,23}
    };
    Equal(triangulation, output.insideTriangles,
        "Incorrect insideTriangles");

    indices = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 2, 2, 3, 3, 4, 4, 5, 5
    };
    UTAssert(
        output.insideNodeIndices == indices,
        "Incorrect insideNodeIndices");

    UTAssert(
        output.outsideTriangles.size() == 0,
        "Incorrect insideNodeIndices");

    // All triangles are CCW ordered, so the exterior triangle orderings
    // (triangles in holes) are reversed from their original CW orderings.
    triangulation =
    {
        {0,1,6},    // node 0 (interior)
        {0,6,10},   // node 0 (interior)
        {0,10,3},   // node 0 (interior)
        {1,2,9},    // node 0 (interior)
        {1,5,6},    // node 0 (interior)
        {1,9,5},    // node 0 (interior)
        {2,3,23},   // node 0 (interior)
        {2,8,9},    // node 0 (interior)
        {2,16,17},  // node 0 (interior)
        {2,17,8},   // node 0 (interior)
        {2,19,16},  // node 0 (interior)
        {2,20,19},  // node 0 (interior)
        {2,23,20},  // node 0 (interior)
        {3,10,11},  // node 0 (interior)
        {3,11,14},  // node 0 (interior)
        {3,12,23},  // node 0 (interior)
        {3,14,15},  // node 0 (interior)
        {3,15,12},  // node 0 (interior)
        {4,5,9},    // node 0 (interior)
        {4,6,5},    // node 1 (exterior)
        {4,7,6},    // node 1 (exterior)
        {4,9,7},    // node 0 (interior)
        {6,7,10},   // node 0 (interior)
        {7,9,10},   // node 0 (interior)
        {8,10,9},   // node 2 (exterior)
        {8,11,10},  // node 2 (exterior)
        {8,13,11},  // node 0 (interior)
        {8,17,18}, // node 0 (interior)
        {8,18,21}, // node 0 (interior)
        {8,21,22}, // node 0 (interior)
        {8,22,13}, // node 0 (interior)
        {11,13,14}, // node 0 (interior)
        {12,13,22}, // node 0 (interior)
        {12,14,13}, // node 3 (exterior)
        {12,15,14}, // node 3 (exterior)
        {12,22,23}, // node 0 (interior)
        {16,18,17}, // node 4 (exterior)
        {16,19,18}, // node 4 (exterior)
        {18,19,21}, // node 0 (interior)
        {19,20,21}, // node 0 (interior)
        {20,22,21}, // node 5 (exterior)
        {20,23,22}  // node 5 (exterior)
    };
    Equal(triangulation, output.allTriangles,
        "Incorrect allTriangles");
}

void UnitTestTriangulateCDT::Profile()
{
    std::default_random_engine dre;
    std::uniform_real_distribution<double> rnd(0.999, 1.001);

    std::size_t constexpr numOuterPoints = 6000;
    std::size_t constexpr numInnerPoints = 1000;
    std::size_t constexpr numPoints = numOuterPoints + 4 * numInnerPoints;
    std::vector<Vector2<double>> points(numPoints);

    Vector2<double> center;
    double radius, angle, cs, sn;

    std::vector<Vector2<double>> outerPoints(numOuterPoints);
    center = { 0.0, 0.0 };
    radius = 1.0;
    for (std::size_t i = 0; i < numOuterPoints; ++i)
    {
        angle = C_TWO_PI<double> * i / (double)numOuterPoints;
        cs = std::cos(angle);
        sn = std::sin(angle);
        outerPoints[i] = center + rnd(dre) * radius * Vector2<double>{ cs, sn };
    }
    std::copy(outerPoints.begin(), outerPoints.end(), points.begin());

    std::array<std::vector<Vector2<double>>, 4> innerPoints;
    for (std::size_t i = 0; i < 4; ++i)
    {
        innerPoints[i].resize(numInnerPoints);
    }

    radius = 0.25;

    center = { 0.5, 0.5 };
    for (std::size_t i = 0; i < numInnerPoints; ++i)
    {
        angle = C_TWO_PI<double> * i / (double)numInnerPoints;
        cs = std::cos(angle);
        sn = std::sin(angle);
        innerPoints[0][i] = center + rnd(dre) * radius * Vector2<double>{ cs, sn };
    }
    std::copy(innerPoints[0].begin(), innerPoints[0].end(),
        points.begin() + numOuterPoints);

    center = { -0.5, 0.5 };
    for (std::size_t i = 0; i < numInnerPoints; ++i)
    {
        angle = C_TWO_PI<double> * i / (double)numInnerPoints;
        cs = std::cos(angle);
        sn = std::sin(angle);
        innerPoints[1][i] = center + rnd(dre) * radius * Vector2<double>{ cs, sn };
    }
    std::copy(innerPoints[1].begin(), innerPoints[1].end(),
        points.begin() + numOuterPoints + numInnerPoints);

    center = { 0.5, -0.5 };
    for (std::size_t i = 0; i < numInnerPoints; ++i)
    {
        angle = C_TWO_PI<double> * i / (double)numInnerPoints;
        cs = std::cos(angle);
        sn = std::sin(angle);
        innerPoints[2][i] = center + rnd(dre) * radius * Vector2<double>{ cs, sn };
    }
    std::copy(innerPoints[2].begin(), innerPoints[2].end(),
        points.begin() + numOuterPoints + 2 * numInnerPoints);

    center = { -0.5, -0.5 };
    for (std::size_t i = 0; i < numInnerPoints; ++i)
    {
        angle = C_TWO_PI<double> * i / (double)numInnerPoints;
        cs = std::cos(angle);
        sn = std::sin(angle);
        innerPoints[3][i] = center + rnd(dre) * radius * Vector2<double>{ cs, sn };
    }
    std::copy(innerPoints[3].begin(), innerPoints[3].end(),
        points.begin() + numOuterPoints + 3 * numInnerPoints);

    auto tree = std::make_shared<PolygonTree>();
    tree->polygon.resize(numOuterPoints);
    std::iota(tree->polygon.begin(), tree->polygon.end(), 0);

    tree->child.resize(4);
    for (std::size_t i = 0; i < 4; ++i)
    {
        tree->child[i] = std::make_shared<PolygonTree>();
        tree->child[i]->polygon.resize(numInnerPoints);
        std::iota(tree->child[i]->polygon.begin(), tree->child[i]->polygon.end(),
            numOuterPoints + i * numInnerPoints);
        std::reverse(tree->child[i]->polygon.begin(), tree->child[i]->polygon.end());
    }

    TriangulateCDT<double> triangulator;
    PolygonTreeEx output;
    Timer timer;
    triangulator(points, tree, output);
    auto microsecs = timer.GetMicroseconds();
    UTInformation("time = " + std::to_string(microsecs) + " microseconds");
    // 264855 microseconds
}

#else

#include <GTL/Mathematics/Geometry/2D/TriangulateCDT.h>

namespace gtl
{
    template class TriangulateCDT<float>;
    template class TriangulateCDT<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(TriangulateCDT)
