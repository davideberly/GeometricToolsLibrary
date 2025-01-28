#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Meshes/TriangleKey.h>
using namespace gtl;

namespace gtl
{
    class UnitTestTriangleKey
    {
    public:
        UnitTestTriangleKey();

    private:
        // Add test function declarations here.
    };
}

UnitTestTriangleKey::UnitTestTriangleKey()
{
    UTInformation("Mathematics/Meshes/TriangleKey (tested by DynamicETManifoldMesh)");
}

#else

#include <GTL/Mathematics/Meshes/TriangleKey.h>

namespace gtl
{
    template class TriangleKey<false>;
    template class TriangleKey<true>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(TriangleKey)
