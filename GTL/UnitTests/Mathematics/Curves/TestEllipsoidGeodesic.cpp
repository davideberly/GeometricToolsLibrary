#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Curves/EllipsoidGeodesic.h>
using namespace gtl;

namespace gtl
{
    class UnitTestEllipsoidGeodesic
    {
    public:
        UnitTestEllipsoidGeodesic();

    private:
        // Add test function declarations here.
    };
}

UnitTestEllipsoidGeodesic::UnitTestEllipsoidGeodesic()
{
    UTInformation("Mathematics/Curves/EllipsoidGeodesic [tested by GTL/VisualTests/Curves/GeodesicEllipsoid]");

    // Add member test function calls here.
}

// Add member test function implementations here.

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Curves/EllipsoidGeodesic.h>

namespace gtl
{
    template class EllipsoidGeodesic<float>;
    template class EllipsoidGeodesic<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class EllipsoidGeodesic<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(EllipsoidGeodesic)
