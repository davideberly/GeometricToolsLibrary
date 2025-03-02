#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/2D/RotatingCalipers.h>
using namespace gtl;

namespace gtl
{
    class UnitTestRotatingCalipers
    {
    public:
        UnitTestRotatingCalipers();

    private:
        void Test();
    };
}

UnitTestRotatingCalipers::UnitTestRotatingCalipers()
{
    UTInformation("Mathematics/Geometry/2D/RotatingCalipers [tested by GTL/VisualTests/Geometry/MinimumAreaBox2D]");
}

void UnitTestRotatingCalipers::Test()
{
    RotatingCalipers<double> rc{};
    std::vector<RotatingCalipers<double>::Antipode> antipodes{};
    std::vector<Vector2<double>> vertices(9);
    vertices[0] = { 1.5, 0.0 };
    vertices[1] = { 1.75, 0.125 };
    vertices[2] = { 2.0, 0.5 };
    vertices[3] = { 2.0, 0.75 };
    vertices[4] = { 0.5, 1.0 };
    vertices[5] = { 0.125, 0.875 };
    vertices[6] = { 0.0, 0.5 };
    vertices[7] = { 0.25, 0.125 };
    vertices[8] = { 1.0, 0.0 };
    rc.ComputeAntipodes(vertices, antipodes);
    UTAssert(
        antipodes[0].vertex == 4 &&
        antipodes[0].edge[0] == 8 &&
        antipodes[0].edge[1] == 0,
        "Incorrect output.");

    UTAssert(
        antipodes[1].vertex == 0 &&
        antipodes[1].edge[0] == 4 &&
        antipodes[1].edge[1] == 5,
        "Incorrect output.");

    UTAssert(
        antipodes[2].vertex == 5 &&
        antipodes[2].edge[0] == 0 &&
        antipodes[2].edge[1] == 1,
        "Incorrect output.");

    UTAssert(
        antipodes[3].vertex == 5 &&
        antipodes[3].edge[0] == 1 &&
        antipodes[3].edge[1] == 2,
        "Incorrect output.");

    UTAssert(
        antipodes[4].vertex == 2 &&
        antipodes[4].edge[0] == 5 &&
        antipodes[4].edge[1] == 6,
        "Incorrect output.");

    UTAssert(
        antipodes[5].vertex == 6 &&
        antipodes[5].edge[0] == 2 &&
        antipodes[5].edge[1] == 3,
        "Incorrect output.");

    UTAssert(
        antipodes[6].vertex == 3 &&
        antipodes[6].edge[0] == 6 &&
        antipodes[6].edge[1] == 7,
        "Incorrect output.");

    UTAssert(
        antipodes[7].vertex == 3 &&
        antipodes[7].edge[0] == 7 &&
        antipodes[7].edge[1] == 8,
        "Incorrect output.");

    UTAssert(
        antipodes[8].vertex == 8 &&
        antipodes[8].edge[0] == 3 &&
        antipodes[8].edge[1] == 4,
        "Incorrect output.");


    vertices[1] = 0.5 * (vertices[0] + vertices[2]);
    rc.ComputeAntipodes(vertices, antipodes);
    UTAssert(
        antipodes[0].vertex == 4 &&
        antipodes[0].edge[0] == 8 &&
        antipodes[0].edge[1] == 0,
        "Incorrect output.");

    UTAssert(
        antipodes[1].vertex == 0 &&
        antipodes[1].edge[0] == 4 &&
        antipodes[1].edge[1] == 5,
        "Incorrect output.");

    UTAssert(
        antipodes[2].vertex == 5 &&
        antipodes[2].edge[0] == 0 &&
        antipodes[2].edge[1] == 2,
        "Incorrect output.");

    UTAssert(
        antipodes[3].vertex == 2 &&
        antipodes[3].edge[0] == 5 &&
        antipodes[3].edge[1] == 6,
        "Incorrect output.");

    UTAssert(
        antipodes[4].vertex == 6 &&
        antipodes[4].edge[0] == 2 &&
        antipodes[4].edge[1] == 3,
        "Incorrect output.");

    UTAssert(
        antipodes[5].vertex == 3 &&
        antipodes[5].edge[0] == 6 &&
        antipodes[5].edge[1] == 7,
        "Incorrect output.");

    UTAssert(
        antipodes[6].vertex == 3 &&
        antipodes[6].edge[0] == 7 &&
        antipodes[6].edge[1] == 8,
        "Incorrect output.");

    UTAssert(
        antipodes[7].vertex == 8 &&
        antipodes[7].edge[0] == 3 &&
        antipodes[7].edge[1] == 4,
        "Incorrect output.");

    vertices[7][1] = 0.0;
    rc.ComputeAntipodes(vertices, antipodes);
    UTAssert(
        antipodes[0].vertex == 4 &&
        antipodes[0].edge[0] == 7 &&
        antipodes[0].edge[1] == 0,
        "Incorrect output.");

    UTAssert(
        antipodes[1].vertex == 0 &&
        antipodes[1].edge[0] == 4 &&
        antipodes[1].edge[1] == 5,
        "Incorrect output.");

    UTAssert(
        antipodes[2].vertex == 5 &&
        antipodes[2].edge[0] == 0 &&
        antipodes[2].edge[1] == 2,
        "Incorrect output.");

    UTAssert(
        antipodes[3].vertex == 2 &&
        antipodes[3].edge[0] == 5 &&
        antipodes[3].edge[1] == 6,
        "Incorrect output.");

    UTAssert(
        antipodes[4].vertex == 6 &&
        antipodes[4].edge[0] == 2 &&
        antipodes[4].edge[1] == 3,
        "Incorrect output.");

    UTAssert(
        antipodes[5].vertex == 3 &&
        antipodes[5].edge[0] == 6 &&
        antipodes[5].edge[1] == 7,
        "Incorrect output.");

    UTAssert(
        antipodes[6].vertex == 7 &&
        antipodes[6].edge[0] == 3 &&
        antipodes[6].edge[1] == 4,
        "Incorrect output.");

    vertices.resize(3);
    vertices[0] = { 0.0, 0.0 };
    vertices[1] = { 4.0, 0.0 };
    vertices[2] = { 1.0, 1.0 };
    rc.ComputeAntipodes(vertices, antipodes);
    UTAssert(
        antipodes[0].vertex == 1 &&
        antipodes[0].edge[0] == 2 &&
        antipodes[0].edge[1] == 0,
        "Incorrect output.");

    UTAssert(
        antipodes[1].vertex == 0 &&
        antipodes[1].edge[0] == 1 &&
        antipodes[1].edge[1] == 2,
        "Incorrect output.");

    UTAssert(
        antipodes[2].vertex == 2 &&
        antipodes[2].edge[0] == 0 &&
        antipodes[2].edge[1] == 1,
        "Incorrect output.");
}

#else

#include <GTL/Mathematics/Geometry/2D/RotatingCalipers.h>

namespace gtl
{
    template class RotatingCalipers<float>;
    template class RotatingCalipers<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(RotatingCalipers)
