#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Meshes/RectanglePatchMesh.h>
using namespace gtl;

namespace gtl
{
    class UnitTestRectanglePatchMesh
    {
    public:
        UnitTestRectanglePatchMesh();

    private:
    };
}

UnitTestRectanglePatchMesh::UnitTestRectanglePatchMesh()
{
    UTInformation("Mathematics/Meshes/RectanglePatchMesh [tested by GTL/VisualTests/Mathematics/Physics/Cloth]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Meshes/RectanglePatchMesh.h>

namespace gtl
{
    template class RectanglePatchMesh<float>;
    template class RectanglePatchMesh<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class RectanglePatchMesh<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(RectanglePatchMesh)
