#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Meshes/Mesh.h>
using namespace gtl;

namespace gtl
{
    class UnitTestMesh
    {
    public:
        UnitTestMesh();

    private:
    };
}

UnitTestMesh::UnitTestMesh()
{
    UTInformation("Mathematics/Meshes/Mesh [tested by GTL/VisualTests/Mathematics/Physics/Rope]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Meshes/Mesh.h>

namespace gtl
{
    template class Mesh<float>;
    template class Mesh<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Mesh<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Mesh)
