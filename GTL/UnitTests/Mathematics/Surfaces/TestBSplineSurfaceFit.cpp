#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Surfaces/BSplineSurfaceFit.h>
using namespace gtl;

namespace gtl
{
    class UnitTestBSplineSurfaceFit
    {
    public:
        UnitTestBSplineSurfaceFit();
    };
}

UnitTestBSplineSurfaceFit::UnitTestBSplineSurfaceFit()
{
    UTInformation("Mathematics/Surfaces/BSplineSurfaceFit [tested by GTL/VisualTests/Surfaces/BSplineSurfaceFitter]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Surfaces/BSplineSurfaceFit.h>

namespace gtl
{
    template class BSplineSurfaceFit<float, 2>;
    template class BSplineSurfaceFit<float, 3>;

    template class BSplineSurfaceFit<double, 2>;
    template class BSplineSurfaceFit<double, 3>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class BSplineSurfaceFit<Rational, 2>;
    template class BSplineSurfaceFit<Rational, 3>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(BSplineSurfaceFit)
