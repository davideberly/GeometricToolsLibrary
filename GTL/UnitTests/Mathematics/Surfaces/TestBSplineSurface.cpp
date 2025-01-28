#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Surfaces/BSplineSurface.h>
using namespace gtl;

namespace gtl
{
    class UnitTestBSplineSurface
    {
    public:
        UnitTestBSplineSurface();
    };
}

UnitTestBSplineSurface::UnitTestBSplineSurface()
{
    UTInformation("Mathematics/Surfaces/BSplineSurface [tested by GTL/VisualTests/Surfaces/BSplineSurfaceFitter]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Surfaces/BSplineSurface.h>

namespace gtl
{
    template class BSplineSurface<float, 3>;
    template class BSplineSurface<double, 4>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class BSplineSurface<Rational, 2>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(BSplineSurface)
