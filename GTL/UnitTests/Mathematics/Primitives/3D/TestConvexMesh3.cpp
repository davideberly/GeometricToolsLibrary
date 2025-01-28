#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/3D/ConvexMesh3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestConvexMesh3
    {
    public:
        UnitTestConvexMesh3();

    private:
        void Test();
    };
}

UnitTestConvexMesh3::UnitTestConvexMesh3()
{
    UTInformation("Mathematics/Primitives/3D/ConvexMesh3");

    Test();
}

void UnitTestConvexMesh3::Test()
{
    ConvexMesh3<double> mesh{};
    UTAssert(mesh.configuration == ConvexMesh3<double>::CFG_EMPTY, "Incorrect configuration.");
    UTAssert(mesh.vertices.size() == 0, "Vertices should be empty.");
    UTAssert(mesh.triangles.size() == 0, "Triangles should be empty.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/3D/ConvexMesh3.h>

namespace gtl
{
    template class ConvexMesh3<float>;
    template class ConvexMesh3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ConvexMesh3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ConvexMesh3)
