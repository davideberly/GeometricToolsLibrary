#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Meshes/VETManifoldMeshKS.h>
using namespace gtl;

namespace gtl
{
    class UnitTestVETManifoldMeshKS
    {
    public:
        UnitTestVETManifoldMeshKS();
    };
}

UnitTestVETManifoldMeshKS::UnitTestVETManifoldMeshKS()
{
    UTInformation("Mathematics/Meshes/VETManifoldMeshKS (tested by ConvexHull3 and Delaunay2)");
}

#else

#include <GTL/Mathematics/Meshes/VETManifoldMeshKS.h>

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(VETManifoldMeshKS)
