#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Meshes/EdgeKey.h>
using namespace gtl;

namespace gtl
{
    class UnitTestEdgeKey
    {
    public:
        UnitTestEdgeKey();

    private:
        // Add test function declarations here.
    };
}

UnitTestEdgeKey::UnitTestEdgeKey()
{
    UTInformation("Mathematics/Meshes/EdgeKey (tested by DynamicETManifoldMesh)");
}

#else

#include <GTL/Mathematics/Meshes/EdgeKey.h>

namespace gtl
{
    template class EdgeKey<false>;
    template class EdgeKey<true>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(EdgeKey)
