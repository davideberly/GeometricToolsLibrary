#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Meshes/IndexAttribute.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIndexAttribute
    {
    public:
        UnitTestIndexAttribute();

    private:
    };
}

UnitTestIndexAttribute::UnitTestIndexAttribute()
{
    UTInformation("Mathematics/Meshes/IndexAttribute [tested by GTL/VisualTests/Mathematics/Physics/Rope]");
}

#else

#include <GTL/Mathematics/Meshes/IndexAttribute.h>

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IndexAttribute)
