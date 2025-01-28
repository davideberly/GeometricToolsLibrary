#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Meshes/FeatureKey.h>
using namespace gtl;

namespace gtl
{
    class UnitTestFeatureKey
    {
    public:
        UnitTestFeatureKey();
    };
}

UnitTestFeatureKey::UnitTestFeatureKey()
{
    UTInformation("Mathematics/Meshes/FeatureKey (tested by derived *Key classes");
}

#else

#include <GTL/Mathematics/Meshes/FeatureKey.h>

namespace gtl
{
    template class FeatureKey<2, false>;
    template class FeatureKey<4, true>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(FeatureKey)
