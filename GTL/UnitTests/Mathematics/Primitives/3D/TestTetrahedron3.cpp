#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/3D/Tetrahedron3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestTetrahedron3
    {
    public:
        UnitTestTetrahedron3();

    private:
        void Test();
    };
}

UnitTestTetrahedron3::UnitTestTetrahedron3()
{
    UTInformation("Mathematics/Primitives/3D/Tetrahedron3");

    Test();
}

void UnitTestTetrahedron3::Test()
{
    double const maxError = 1e-15;

    Tetrahedron3<double> tetra{};
    tetra.v[0] = { 0.0, 0.0, 0.0 };
    tetra.v[1] = { 1.0, 0.0, 0.0 };
    tetra.v[2] = { 0.0, 1.0, 0.0 };
    tetra.v[3] = { 0.0, 0.0, 1.0 };
    Vector3<double> normal{}, expectedNormal{};

    normal = tetra.ComputeFaceNormal(0);  // (0,0,-1)
    expectedNormal = { 0.0, 0.0, -1.0 };
    UTAssert(Length(normal - expectedNormal) <= maxError, "Invalid face normal 0.");

    normal = tetra.ComputeFaceNormal(1);  // (0,-1,0)
    expectedNormal = { 0.0, -1.0, 0.0 };
    UTAssert(Length(normal - expectedNormal) <= maxError, "Invalid face normal 1.");

    normal = tetra.ComputeFaceNormal(2);  // (-1,0,0)
    expectedNormal = { -1.0, 0.0, 0.0 };
    UTAssert(Length(normal - expectedNormal) <= maxError, "Invalid face normal 2.");

    normal = tetra.ComputeFaceNormal(3);  // (1,1,1)/sqrt(3)
    expectedNormal = { 1.0, 1.0, 1.0 };
    Normalize(expectedNormal);
    UTAssert(Length(normal - expectedNormal) <= maxError, "Invalid face normal 3.");

    normal = tetra.ComputeEdgeNormal(0);  // (0,-1,-1)/sqrt(2)
    expectedNormal = { 0.0, -1.0, -1.0 };
    Normalize(expectedNormal);
    UTAssert(Length(normal - expectedNormal) <= maxError, "Invalid edge normal 0.");

    normal = tetra.ComputeEdgeNormal(1);  // (-1,0,-1)/sqrt(2)
    expectedNormal = { -1.0, 0.0, -1.0 };
    Normalize(expectedNormal);
    UTAssert(Length(normal - expectedNormal) <= maxError, "Invalid edge normal 1.");

    normal = tetra.ComputeEdgeNormal(2);  // (-1,-1,0)/sqrt(2)
    expectedNormal = { -1.0, -1.0, 0.0 };
    Normalize(expectedNormal);
    UTAssert(Length(normal - expectedNormal) <= maxError, "Invalid edge normal 2.");

    normal = tetra.ComputeEdgeNormal(3);  // (1,1,0)/sqrt(2)
    expectedNormal = { 1.0, 1.0, 0.0 };
    Normalize(expectedNormal);
    UTAssert(Length(normal - expectedNormal) <= maxError, "Invalid edge normal 3.");

    normal = tetra.ComputeEdgeNormal(4);  // (1,0,1)/sqrt(2)
    expectedNormal = { 1.0, 0.0, 1.0 };
    Normalize(expectedNormal);
    UTAssert(Length(normal - expectedNormal) <= maxError, "Invalid edge normal 4.");

    normal = tetra.ComputeEdgeNormal(5);  // (0,1,1)/sqrt(2)
    expectedNormal = { 0.0, 1.0, 1.0 };
    Normalize(expectedNormal);
    UTAssert(Length(normal - expectedNormal) <= maxError, "Invalid edge normal 5.");

    normal = tetra.ComputeVertexNormal(0);  // (-1,-1,-1)/sqrt(3)
    expectedNormal = { -1.0, -1.0, -1.0 };
    Normalize(expectedNormal);
    UTAssert(Length(normal - expectedNormal) <= maxError, "Invalid vertex normal 0.");

    normal = tetra.ComputeVertexNormal(1);  // (1,0,0)
    expectedNormal = { 1.0, 0.0, 0.0 };
    UTAssert(Length(normal - expectedNormal) <= maxError, "Invalid vertex normal 1.");

    normal = tetra.ComputeVertexNormal(2);  // (0,1,0)
    expectedNormal = { 0.0, 1.0, 0.0 };
    UTAssert(Length(normal - expectedNormal) <= maxError, "Invalid vertex normal 2.");

    normal = tetra.ComputeVertexNormal(3);  // (0,0,1)
    expectedNormal = { 0.0, 0.0, 1.0 };
    UTAssert(Length(normal - expectedNormal) <= maxError, "Invalid vertex normal 3.");

    std::array<Plane3<double>, 4> planes{};
    tetra.GetPlanes(planes);
    std::array<Plane3<double>, 4> expectedPlanes =
    {
        Plane3<double>(Vector3<double>{ 0.0, 0.0, -1.0}, 0.0),
        Plane3<double>(Vector3<double>{ 0.0, -1.0, 0.0}, 0.0),
        Plane3<double>(Vector3<double>{ -1.0, 0.0, 0.0}, 0.0),
        Plane3<double>(Vector3<double>{ 0.57735026918962584, 0.57735026918962584, 0.57735026918962584 }, 0.57735026918962584)
    };

    for (std::size_t i = 0; i < 4; ++i)
    {
        double error = Length(planes[i].normal - expectedPlanes[i].normal);
        UTAssert(error <= maxError, "Invalid plane normal " + std::to_string(i));
        error = std::fabs(planes[i].constant - expectedPlanes[i].constant);
        UTAssert(error <= maxError, "Invalid plane constant " + std::to_string(i));
    }

    Vector3<double> centroid = tetra.ComputeCentroid();
    Vector3<double> expectedCentroid{ 0.25, 0.25, 0.25 };
    UTAssert(Length(centroid - expectedCentroid) <= maxError, "Invalid centroid.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/3D/Tetrahedron3.h>

namespace gtl
{
    template class Tetrahedron3<float>;
    template bool operator==(Tetrahedron3<float> const&, Tetrahedron3<float> const&);
    template bool operator!=(Tetrahedron3<float> const&, Tetrahedron3<float> const&);
    template bool operator< (Tetrahedron3<float> const&, Tetrahedron3<float> const&);
    template bool operator<=(Tetrahedron3<float> const&, Tetrahedron3<float> const&);
    template bool operator> (Tetrahedron3<float> const&, Tetrahedron3<float> const&);
    template bool operator>=(Tetrahedron3<float> const&, Tetrahedron3<float> const&);

    template class Tetrahedron3<double>;
    template bool operator==(Tetrahedron3<double> const&, Tetrahedron3<double> const&);
    template bool operator!=(Tetrahedron3<double> const&, Tetrahedron3<double> const&);
    template bool operator< (Tetrahedron3<double> const&, Tetrahedron3<double> const&);
    template bool operator<=(Tetrahedron3<double> const&, Tetrahedron3<double> const&);
    template bool operator> (Tetrahedron3<double> const&, Tetrahedron3<double> const&);
    template bool operator>=(Tetrahedron3<double> const&, Tetrahedron3<double> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Tetrahedron3<Rational>;
    template bool operator==(Tetrahedron3<Rational> const&, Tetrahedron3<Rational> const&);
    template bool operator!=(Tetrahedron3<Rational> const&, Tetrahedron3<Rational> const&);
    template bool operator< (Tetrahedron3<Rational> const&, Tetrahedron3<Rational> const&);
    template bool operator<=(Tetrahedron3<Rational> const&, Tetrahedron3<Rational> const&);
    template bool operator> (Tetrahedron3<Rational> const&, Tetrahedron3<Rational> const&);
    template bool operator>=(Tetrahedron3<Rational> const&, Tetrahedron3<Rational> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Tetrahedron3)
