#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/3D/IntpLinearNonuniform3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntpLinearNonuniform3
    {
    public:
        UnitTestIntpLinearNonuniform3();

    private:
        // Add test function declarations here.
    };
}

UnitTestIntpLinearNonuniform3::UnitTestIntpLinearNonuniform3()
{
    UTInformation("Mathematics/Interpolation/3D/IntpLinearNonuniform3 [NEEDS UNIT TESTS]");

    // Add member test function calls here.
}

// Add member test function implementations here.

#else

#include <GTL/Mathematics/Interpolation/3D/IntpLinearNonuniform3.h>

namespace gtl
{
    template class IntpLinearNonuniform3<float>;
    template class IntpLinearNonuniform3<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpLinearNonuniform3)
