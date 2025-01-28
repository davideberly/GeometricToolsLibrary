#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/3D/Polyhedron3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestPolyhedron3
    {
    public:
        UnitTestPolyhedron3();

    private:
        void Test();
    };
}

UnitTestPolyhedron3::UnitTestPolyhedron3()
{
    UTInformation("Mathematics/Primitives/3D/Polyhedron3");

    Test();
}

void UnitTestPolyhedron3::Test()
{
    double const maxError = 1e-12;

    double const a = 1.0 / std::sqrt(3.0);
    double const b = std::sqrt((3.0 - std::sqrt(5.0)) / 6.0);
    double const c = std::sqrt((3.0 + std::sqrt(5.0)) / 6.0);
    std::size_t const numVertices = 20;
    std::vector<Vector3<double>> vertices(numVertices);
    vertices[0] = { a, a, a };
    vertices[1] = { a, a, -a };
    vertices[2] = { a, -a, a };
    vertices[3] = { a, -a, -a };
    vertices[4] = { -a, a, a };
    vertices[5] = { -a, a, -a };
    vertices[6] = { -a, -a, a };
    vertices[7] = { -a, -a, -a };
    vertices[8] = { b, c, 0.0f };
    vertices[9] = { -b, c, 0.0f };
    vertices[10] = { b, -c, 0.0f };
    vertices[11] = { -b, -c, 0.0f };
    vertices[12] = { c, 0.0f, b };
    vertices[13] = { c, 0.0f, -b };
    vertices[14] = { -c, 0.0f, b };
    vertices[15] = { -c, 0.0f, -b };
    vertices[16] = { 0.0f, b, c };
    vertices[17] = { 0.0f, -b, c };
    vertices[18] = { 0.0f, b, -c };
    vertices[19] = { 0.0f, -b, -c };

    std::size_t const numTriangles = 36;
    std::vector<std::size_t> indices(3 * numTriangles);
    auto triangles = reinterpret_cast<std::array<std::size_t, 3>*>(indices.data());
    triangles[0] = { 0, 8, 9 };
    triangles[1] = { 0, 9, 4 };
    triangles[2] = { 0, 4, 16 };
    triangles[3] = { 0, 12, 13 };
    triangles[4] = { 0, 13, 1 };
    triangles[5] = { 0, 1, 8 };
    triangles[6] = { 0, 16, 17 };
    triangles[7] = { 0, 17, 2 };
    triangles[8] = { 0, 2, 12 };
    triangles[9] = { 8, 1, 18 };
    triangles[10] = { 8, 18, 5 };
    triangles[11] = { 8, 5, 9 };
    triangles[12] = { 12, 2, 10 };
    triangles[13] = { 12, 10, 3 };
    triangles[14] = { 12, 3, 13 };
    triangles[15] = { 16, 4, 14 };
    triangles[16] = { 16, 14, 6 };
    triangles[17] = { 16, 6, 17 };
    triangles[18] = { 9, 5, 15 };
    triangles[19] = { 9, 15, 14 };
    triangles[20] = { 9, 14, 4 };
    triangles[21] = { 6, 11, 10 };
    triangles[22] = { 6, 10, 2 };
    triangles[23] = { 6, 2, 17 };
    triangles[24] = { 3, 19, 18 };
    triangles[25] = { 3, 18, 1 };
    triangles[26] = { 3, 1, 13 };
    triangles[27] = { 7, 15, 5 };
    triangles[28] = { 7, 5, 18 };
    triangles[29] = { 7, 18, 19 };
    triangles[30] = { 7, 11, 6 };
    triangles[31] = { 7, 6, 14 };
    triangles[32] = { 7, 14, 15 };
    triangles[33] = { 7, 19, 3 };
    triangles[34] = { 7, 3, 10 };
    triangles[35] = { 7, 10, 11 };

    Vector3<double> trueAverage{ 0.0, 0.0, 0.0 };
    Vector3<double> edge = vertices[triangles[0][0]] - vertices[triangles[0][1]];
    double lengthEdge = Length(edge);
    double trueSurfaceArea = 3.0 * std::sqrt(25.0 + 10.0 * std::sqrt(5.0)) * lengthEdge * lengthEdge;
    double trueVolume = 0.25 * (15.0 + 7.0 * std::sqrt(5)) * std::pow(lengthEdge, 3.0);

    Polyhedron3<double> poly(vertices, indices, true);
    Vector3<double> average = poly.ComputeVertexAverage();
    double surfaceArea = poly.ComputeSurfaceArea();
    double volume = poly.ComputeVolume();
    UTAssert(Length(average - trueAverage) <= maxError, "Invalid average.");
    UTAssert(std::fabs(surfaceArea - trueSurfaceArea) <= maxError, "Invalid surface area.");
    UTAssert(std::fabs(volume - trueVolume) <= maxError, "Invalid volume.");

    std::vector<Vector3<double>> vertexPool(2 * numVertices);
    for (std::size_t i = 0; i < numVertices; ++i)
    {
        vertexPool[2 * i] = vertices[i];
        vertexPool[2 * i + 1] = { 0.0, 0.0, 0.0 };
    }

    for (std::size_t i = 0; i < numTriangles; ++i)
    {
        triangles[i][0] *= 2;
        triangles[i][1] *= 2;
        triangles[i][2] *= 2;
    }

    Polyhedron3<double> poly1(vertexPool, indices, true);
    average = poly1.ComputeVertexAverage();
    surfaceArea = poly1.ComputeSurfaceArea();
    volume = poly1.ComputeVolume();
    UTAssert(Length(average - trueAverage) <= maxError, "Invalid average.");
    UTAssert(std::fabs(surfaceArea - trueSurfaceArea) <= maxError, "Invalid surface area.");
    UTAssert(std::fabs(volume - trueVolume) <= maxError, "Invalid volume.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/3D/Polyhedron3.h>

namespace gtl
{
    template class Polyhedron3<float>;
    template class Polyhedron3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Polyhedron3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Polyhedron3)
