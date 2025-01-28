#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/3D/ConvexPolyhedron3.h>
#include <utility>
using namespace gtl;

namespace gtl
{
    class UnitTestConvexPolyhedron3
    {
    public:
        UnitTestConvexPolyhedron3();

    private:
        void Test();
    };
}

UnitTestConvexPolyhedron3::UnitTestConvexPolyhedron3()
{
    UTInformation("Mathematics/Primitives/3D/ConvexPolyhedron3");

    Test();
}

void UnitTestConvexPolyhedron3::Test()
{
    double const maxError = 1e-15;

    ConvexPolyhedron3<double> poly0{};
    UTAssert(poly0.vertices.size() == 0, "Vertices should be empty.");
    UTAssert(poly0.indices.size() == 0, "Vertices should be empty.");
    UTAssert(poly0.planes.size() == 0, "Vertices should be empty.");
    UTAssert(IsZero(poly0.alignedBox.min), "Expecting zero box min.");
    UTAssert(IsZero(poly0.alignedBox.max), "Expecting zero box max.");

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

    ConvexPolyhedron3<double> poly1(vertices, indices, true, true);
    UTAssert(poly1.vertices == vertices, "Invalid  vertices.");
    UTAssert(poly1.indices == indices, "Invalid indices.");
    UTAssert(poly1.planes.size() == indices.size() / 3, "Invalid number of planes.");
    Vector3<double> expectedBound{ 0.93417235896271578, 0.93417235896271578, 0.93417235896271578 };
    UTAssert(Length(poly1.alignedBox.min + expectedBound) <= maxError, "Invalid box min.");
    UTAssert(Length(poly1.alignedBox.max - expectedBound) <= maxError, "Invalid box max.");

    std::vector<Vector4<double>> expectedPlanes =
    {
        Vector4<double>{ 0.0000000000000000, 0.4120226591665966, 0.2546440075000701, -0.3849001794597506 },
        Vector4<double>{ 0.0000000000000000, 0.6666666666666669, 0.4120226591665966, -0.6227815726418107 },
        Vector4<double>{ 0.0000000000000000, 0.4120226591665966, 0.2546440075000703, -0.3849001794597507 },
        Vector4<double>{ 0.4120226591665966, 0.2546440075000701, 0.0000000000000000, -0.3849001794597506 },
        Vector4<double>{ 0.6666666666666669, 0.4120226591665966, 0.0000000000000000, -0.6227815726418107 },
        Vector4<double>{ 0.4120226591665966, 0.2546440075000703, 0.0000000000000000, -0.3849001794597507 },
        Vector4<double>{ 0.2546440075000701, 0.0000000000000000, 0.4120226591665966, -0.3849001794597506 },
        Vector4<double>{ 0.4120226591665966, 0.0000000000000000, 0.6666666666666669, -0.6227815726418107 },
        Vector4<double>{ 0.2546440075000703, 0.0000000000000000, 0.4120226591665966, -0.3849001794597507 },
        Vector4<double>{ -0.0000000000000001, 0.4120226591665967, -0.2546440075000702, -0.3849001794597506 },
        Vector4<double>{ 0.0000000000000001, 0.6666666666666669, -0.4120226591665969, -0.6227815726418107 },
        Vector4<double>{ 0.0000000000000000, 0.4120226591665966, -0.2546440075000700, -0.3849001794597506 },
        Vector4<double>{ 0.4120226591665967, -0.2546440075000702, -0.0000000000000001, -0.3849001794597506 },
        Vector4<double>{ 0.6666666666666669, -0.4120226591665969, 0.0000000000000001, -0.6227815726418107 },
        Vector4<double>{ 0.4120226591665966, -0.2546440075000700, 0.0000000000000000, -0.3849001794597506 },
        Vector4<double>{ -0.2546440075000702, -0.0000000000000001, 0.4120226591665967, -0.3849001794597506 },
        Vector4<double>{ -0.4120226591665969, 0.0000000000000001, 0.6666666666666669, -0.6227815726418107 },
        Vector4<double>{ -0.2546440075000700, 0.0000000000000000, 0.4120226591665966, -0.3849001794597506 },
        Vector4<double>{ -0.4120226591665967, 0.2546440075000702, 0.0000000000000001, -0.3849001794597507 },
        Vector4<double>{ -0.6666666666666666, 0.4120226591665966, 0.0000000000000000, -0.6227815726418106 },
        Vector4<double>{ -0.4120226591665967, 0.2546440075000702, -0.0000000000000001, -0.3849001794597507 },
        Vector4<double>{ 0.0000000000000000, -0.4120226591665966, 0.2546440075000701, -0.3849001794597506 },
        Vector4<double>{ 0.0000000000000000, -0.6666666666666669, 0.4120226591665966, -0.6227815726418107 },
        Vector4<double>{ 0.0000000000000000, -0.4120226591665966, 0.2546440075000703, -0.3849001794597507 },
        Vector4<double>{ 0.2546440075000701, 0.0000000000000000, -0.4120226591665966, -0.3849001794597506 },
        Vector4<double>{ 0.4120226591665966, 0.0000000000000000, -0.6666666666666669, -0.6227815726418107 },
        Vector4<double>{ 0.2546440075000703, 0.0000000000000000, -0.4120226591665966, -0.3849001794597507 },
        Vector4<double>{ -0.2546440075000703, 0.0000000000000000, -0.4120226591665966, -0.3849001794597507 },
        Vector4<double>{ -0.4120226591665966, 0.0000000000000000, -0.6666666666666669, -0.6227815726418107 },
        Vector4<double>{ -0.2546440075000701, 0.0000000000000000, -0.4120226591665966, -0.3849001794597506 },
        Vector4<double>{ -0.4120226591665966, -0.2546440075000703, 0.0000000000000000, -0.3849001794597507 },
        Vector4<double>{ -0.6666666666666669, -0.4120226591665966, 0.0000000000000000, -0.6227815726418107 },
        Vector4<double>{ -0.4120226591665966, -0.2546440075000701, 0.0000000000000000, -0.3849001794597506 },
        Vector4<double>{ 0.0000000000000000, -0.4120226591665966, -0.2546440075000703, -0.3849001794597507 },
        Vector4<double>{ 0.0000000000000000, -0.6666666666666669, -0.4120226591665966, -0.6227815726418107 },
        Vector4<double>{ 0.0000000000000000, -0.4120226591665966, -0.2546440075000701, -0.3849001794597506 }
    };

    for (std::size_t i = 0; i < poly1.planes.size(); ++i)
    {
        UTAssert(Length(poly1.planes[i] - expectedPlanes[i]) <= maxError,
            "Invalid plane " + std::to_string(i));
    }
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/3D/ConvexPolyhedron3.h>

namespace gtl
{
    template class ConvexPolyhedron3<float>;
    template class ConvexPolyhedron3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ConvexPolyhedron3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ConvexPolyhedron3)
