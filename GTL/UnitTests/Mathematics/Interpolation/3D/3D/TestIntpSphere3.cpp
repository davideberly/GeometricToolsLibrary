#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/3D/IntpSphere3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntpSphere3
    {
    public:
        UnitTestIntpSphere3();

    private:
        // Add test function declarations here.
    };
}

UnitTestIntpSphere3::UnitTestIntpSphere3()
{
    UTInformation("Mathematics/Interpolation/3D/IntpSphere3 [NEEDS UNIT TESTS]");

    // Add member test function calls here.
}

// Add member test function implementations here.

#else

#include <GTL/Mathematics/Interpolation/3D/IntpSphere3.h>

namespace gtl
{
    template class IntpSphere3<float>;
    template class IntpSphere3<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpSphere3)
