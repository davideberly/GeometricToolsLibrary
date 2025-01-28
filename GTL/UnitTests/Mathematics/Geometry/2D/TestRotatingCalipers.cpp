#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/2D/RotatingCalipers.h>
using namespace gtl;

namespace gtl
{
    class UnitTestRotatingCalipers
    {
    public:
        UnitTestRotatingCalipers();

    private:
    };
}

UnitTestRotatingCalipers::UnitTestRotatingCalipers()
{
    UTInformation("Mathematics/Geometry/2D/RotatingCalipers [tested by GTL/VisualTests/Geometry/MinimumAreaBox2D]");
}

#else

#include <GTL/Mathematics/Geometry/2D/RotatingCalipers.h>

namespace gtl
{
    template class RotatingCalipers<float>;
    template class RotatingCalipers<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(RotatingCalipers)
