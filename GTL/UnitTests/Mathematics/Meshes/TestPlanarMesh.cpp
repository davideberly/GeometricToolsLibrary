#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Meshes/PlanarMesh.h>
using namespace gtl;

namespace gtl
{
    class UnitTestPlanarMesh
    {
    public:
        UnitTestPlanarMesh();
    };
}

UnitTestPlanarMesh::UnitTestPlanarMesh()
{
    UTInformation("Mathematics/Meshes/PlanarMesh [tested by TestIntpQuadraticNonuniform]");
}

#else

#include <GTL/Mathematics/Meshes/PlanarMesh.h>

namespace gtl
{
    template class PlanarMesh<float>;
    template class PlanarMesh<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(PlanarMesh)
