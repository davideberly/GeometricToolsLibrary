#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Containment/3D/ContTetrahedron3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestContTetrahedron3
    {
    public:
        UnitTestContTetrahedron3();

    private:
        void Test();
    };
}

UnitTestContTetrahedron3::UnitTestContTetrahedron3()
{
    UTInformation("Mathematics/Containment/3D/ContTetrahedron3");

    Test();
}

void UnitTestContTetrahedron3::Test()
{
    Tetrahedron3<double> tetra{};
    Vector3<double> point{};
    bool contained{};

    tetra.v[0] = { 0.0, 0.0, 0.0 };
    tetra.v[1] = { 1.0, 0.0, 0.0 };
    tetra.v[2] = { 0.0, 1.0, 0.0 };
    tetra.v[3] = { 0.0, 0.0, 1.0 };

    point = { 0.2, 0.3, 0.4 };
    contained = InContainer(point, tetra);
    UTAssert(contained, "Invalid result.");

    point = { -0.1, 0.25, 0.25 };
    contained = InContainer(point, tetra);
    UTAssert(!contained, "Invalid result.");

    point = { 0.25, -0.1, 0.25 };
    contained = InContainer(point, tetra);
    UTAssert(!contained, "Invalid result.");

    point = { 0.25, 0.25, -0.1 };
    contained = InContainer(point, tetra);
    UTAssert(!contained, "Invalid result.");

    point = { 0.5, 0.5, 0.5 };
    contained = InContainer(point, tetra);
    UTAssert(!contained, "Invalid result.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Containment/3D/ContTetrahedron3.h>

namespace gtl
{
    template bool InContainer(Vector3<float> const&, Tetrahedron3<float> const&);

    template bool InContainer(Vector3<double> const&, Tetrahedron3<double> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template bool InContainer(Vector3<Rational> const&, Tetrahedron3<Rational> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ContTetrahedron3)
