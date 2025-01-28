#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistPoint3ConvexPolyhedron3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistPoint3ConvexPolyhedron3
    {
    public:
        UnitTestDistPoint3ConvexPolyhedron3();

    private:
        using PCQuery = DCPQuery<double, Vector3<double>, ConvexPolyhedron3<double>>;
        using PCOutput = typename PCQuery::Output;

        void Test();

        void Validate(PCOutput const& output,
            double sqrDistance, std::array<Vector3<double>, 2> const& closest);
    };
}

UnitTestDistPoint3ConvexPolyhedron3::UnitTestDistPoint3ConvexPolyhedron3()
{
    UTInformation("Mathematics/Distance/3D/DistPoint3ConvexPolyhedron3");

    Test();
}

void UnitTestDistPoint3ConvexPolyhedron3::Validate(PCOutput const& output,
    double sqrDistance, std::array<Vector3<double>, 2> const& closest)
{
    double const maxError = 1e-14;

    double error = std::fabs(output.sqrDistance - sqrDistance);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(sqrDistance));
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - closest[0]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = Length(output.closest[1] - closest[1]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
}

void UnitTestDistPoint3ConvexPolyhedron3::Test()
{
    PCQuery query{};
    PCOutput output{};

    // Create a dodecahedron.

    double const a = 1.0 / std::sqrt(3.0);
    double const b = std::sqrt((3.0 - std::sqrt(5.0)) / 6.0);
    double const c = std::sqrt((3.0 + std::sqrt(5.0)) / 6.0);
    std::size_t constexpr numVertices = 20;
    std::vector<Vector3<double>> vertices(numVertices);

    vertices[0] = { a, a, a };
    vertices[1] = { a, a, -a };
    vertices[2] = { a, -a, a };
    vertices[3] = { a, -a, -a };
    vertices[4] = { -a, a, a };
    vertices[5] = { -a, a, -a };
    vertices[6] = { -a, -a, a };
    vertices[7] = { -a, -a, -a };
    vertices[8] = { b, c, 0.0 };
    vertices[9] = { -b, c, 0.0 };
    vertices[10] = { b, -c, 0.0 };
    vertices[11] = { -b, -c, 0.0 };
    vertices[12] = { c, 0.0, b };
    vertices[13] = { c, 0.0, -b };
    vertices[14] = { -c, 0.0, b };
    vertices[15] = { -c, 0.0, -b };
    vertices[16] = { 0.0, b, c };
    vertices[17] = { 0.0, -b, c };
    vertices[18] = { 0.0, b, -c };
    vertices[19] = { 0.0, -b, -c };

    std::size_t constexpr numTriangles = 36;
    std::vector<std::size_t> indices(3 * numTriangles);
    std::array<std::size_t, 3>* triangles =
        reinterpret_cast<std::array<std::size_t, 3>*>(indices.data());
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

    ConvexPolyhedron3<double> dodecahedron(
        std::move(vertices), std::move(indices), true, true);

    // The point is inside the polyhedron.
    Vector3<double> point{ 0.1, 0.2, 0.3 };
    output = query(point, dodecahedron);
    // distance = 1.1443916996305594e-16, sqrDistance = 1.3096323621833204e-32
    Validate(output, 0.0, { point, point });

    // The point is on the polyhedron.
    point = dodecahedron.vertices[0];
    output = query(point, dodecahedron);
    // distance = 9.6148134319178206e-16, sqrDistance = 9.2444637330587321e-31
    Validate(output, 0.0, { point, point });

    // The point is outside the polyhedron, closest is vertex.
    point = { 2.0, 3.0, 4.0 };
    output = query(point, dodecahedron);
    // distance = 4.4280577180731004, sqrDistance = 19.607695154586754
    Validate(output, 19.607695154586754, { point, dodecahedron.vertices[0] });

    // The point is outside the polyhedron, closest is on triangle face.
    point = { a, a + 2.0 * c, a };
    Normalize(point);
    output = query(point, dodecahedron);
    // distance = 0.12993963402678882, sqrDistance = 0.016884308491015815
    Vector3<double> closest{ 0.22391897979451336, 0.83800276514010408, 0.15560567148925641 };
    // Mathematica: closest = (0.223919, 0.838003, 0.155606)
    Validate(output, 0.016884308491015815, { point, closest });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistPoint3ConvexPolyhedron3.h>

namespace gtl
{
    template class DCPQuery<float, Vector3<float>, ConvexPolyhedron3<float>>;
    template class DCPQuery<double, Vector3<double>, ConvexPolyhedron3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Vector3<Rational>, ConvexPolyhedron3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPoint3ConvexPolyhedron3)
