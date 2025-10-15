#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Meshes/TubeMesh.h>
using namespace gtl;

namespace gtl
{
    class UnitTestTubeMesh
    {
    public:
        UnitTestTubeMesh();

    private:
    };
}

UnitTestTubeMesh::UnitTestTubeMesh()
{
    UTInformation("Mathematics/Meshes/TubeMesh [tested by GTL/VisualTests/Mathematics/Physics/Rope]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Meshes/TubeMesh.h>

namespace gtl
{
    template class TubeMesh<float>;
    template class TubeMesh<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TubeMesh<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(TubeMesh)
