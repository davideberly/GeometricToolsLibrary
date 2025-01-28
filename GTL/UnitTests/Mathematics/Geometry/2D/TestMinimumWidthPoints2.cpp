#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/2D/MinimumWidthPoints2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestMinimumWidthPoints2
    {
    public:
        UnitTestMinimumWidthPoints2();

    private:
    };
}

UnitTestMinimumWidthPoints2::UnitTestMinimumWidthPoints2()
{
    UTInformation("Mathematics/Geometry/2D/MinimumWidthPoints2 [tested by GTL/VisualTests/Geometry/MinimumAreaBox2D]");
}

#else

#include <GTL/Mathematics/Geometry/2D/MinimumWidthPoints2.h>

namespace gtl
{
    template class MinimumWidthPoints2<float>;
    template class MinimumWidthPoints2<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(MinimumWidthPoints2)
