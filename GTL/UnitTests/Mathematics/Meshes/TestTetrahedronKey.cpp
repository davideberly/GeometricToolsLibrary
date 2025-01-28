#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Meshes/TetrahedronKey.h>
using namespace gtl;

namespace gtl
{
    class UnitTestTetrahedronKey
    {
    public:
        UnitTestTetrahedronKey();

    private:
        // Add test function declarations here.
    };
}

UnitTestTetrahedronKey::UnitTestTetrahedronKey()
{
    UTInformation("Mathematics/Meshes/TetrahedronKey (tested by DynamicTSManifoldMesh)");
}

// Add member test function implementations here.

#else

#include <GTL/Mathematics/Meshes/TetrahedronKey.h>

namespace gtl
{
    template class TetrahedronKey<false>;
    template class TetrahedronKey<true>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(TetrahedronKey)
