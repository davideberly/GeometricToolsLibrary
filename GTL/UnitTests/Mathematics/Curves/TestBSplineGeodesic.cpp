#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Curves/BSplineGeodesic.h>
using namespace gtl;

namespace gtl
{
    class UnitTestBSplineGeodesic
    {
    public:
        UnitTestBSplineGeodesic();

    private:
        // Add test function declarations here.
    };
}

UnitTestBSplineGeodesic::UnitTestBSplineGeodesic()
{
    UTInformation("Mathematics/Curves/BSplineGeodesic [tested by GTL/VisualTests/Curves/GeodesicHeightField]");

    // Add member test function calls here.
}

// Add member test function implementations here.

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Curves/BSplineGeodesic.h>

namespace gtl
{
    template class BSplineGeodesic<float>;
    template class BSplineGeodesic<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class BSplineGeodesic<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(BSplineGeodesic)
