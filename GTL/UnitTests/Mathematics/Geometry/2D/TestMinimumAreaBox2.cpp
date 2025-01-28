#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/2D/MinimumAreaBox2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestMinimumAreaBox2
    {
    public:
        UnitTestMinimumAreaBox2();

    private:
    };
}

UnitTestMinimumAreaBox2::UnitTestMinimumAreaBox2()
{
    UTInformation("Mathematics/Geometry/2D/MinimumAreaBox2 [tested by GTL/VisualTests/Geometry/MinimumAreaBox2D]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Geometry/2D/MinimumAreaBox2.h>

namespace gtl
{
    template class MinimumAreaBox2<float, double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class MinimumAreaBox2<double, Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(MinimumAreaBox2)
