#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Meshes/UniqueVerticesSimplices.h>
using namespace gtl;

namespace gtl
{
    class UnitTestUniqueVerticesSimplices
    {
    public:
        UnitTestUniqueVerticesSimplices();
    };
}

UnitTestUniqueVerticesSimplices::UnitTestUniqueVerticesSimplices()
{
    UTInformation("Mathematics/Meshes/UniqueVerticesSimplices [tested by GTL/VisualTests/Mathematics/Distance/DistanceOrientedBoxCone]");
}

#else

#include <array>
#include <vector>
#include <GTL/Mathematics/Meshes/UniqueVerticesSimplices.h>

namespace gtl
{
    template class UniqueVerticesSimplices<std::array<float, 3>, std::size_t, 3>;
    template class UniqueVerticesSimplices<std::vector<double>, std::int32_t, 2>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(UniqueVerticesSimplices)
