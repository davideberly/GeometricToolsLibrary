#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Surfaces/NURBSSurface.h>
using namespace gtl;

namespace gtl
{
    class UnitTestNURBSSurface
    {
    public:
        UnitTestNURBSSurface();
    };
}

UnitTestNURBSSurface::UnitTestNURBSSurface()
{
    UTInformation("Mathematics/Surfaces/NURBSSurface [tested by GTL/VisualTests/Surfaces/NURBSSphere]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Surfaces/NURBSSurface.h>

namespace gtl
{
    template class NURBSSurface<float, 3>;
    template class NURBSSurface<double, 4>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class NURBSSurface<Rational, 2>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(NURBSSurface)
