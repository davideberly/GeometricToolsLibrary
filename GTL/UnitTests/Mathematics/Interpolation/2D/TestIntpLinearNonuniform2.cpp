#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/2D/IntpLinearNonuniform2.h>
#include <fstream>
using namespace gtl;

namespace gtl
{
    class UnitTestIntpLinearNonuniform2
    {
    public:
        UnitTestIntpLinearNonuniform2();

    private:
        void Test();
    };
}

UnitTestIntpLinearNonuniform2::UnitTestIntpLinearNonuniform2()
{
    UTInformation("Mathematics/Interpolation/2D/IntpLinearNonuniform2 [tested by GTL/VisualTests/Mathematics/Interpolation/Interpolation2D]");
}

#else

#include <GTL/Mathematics/Interpolation/2D/IntpLinearNonuniform2.h>

namespace gtl
{
    template class IntpLinearNonuniform2<float>;
    template class IntpLinearNonuniform2<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpLinearNonuniform2)
