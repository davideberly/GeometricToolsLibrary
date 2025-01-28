#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Meshes/VETFeaturesKS.h>
using namespace gtl;

namespace gtl
{
    class UnitTestVETFeaturesKS
    {
    public:
        UnitTestVETFeaturesKS();
    };
}

UnitTestVETFeaturesKS::UnitTestVETFeaturesKS()
{
    UTInformation("Mathematics/Meshes/VETFeaturesKS (tested by ConvexHull3 and Delaunay2)");
}

#else

#include <GTL/Mathematics/Meshes/VETFeaturesKS.h>

namespace gtl
{
    template class VETFeaturesKS<1>;
    template class VETFeaturesKS<2>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(VETFeaturesKS)
