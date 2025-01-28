#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Curves/RiemannianGeodesic.h>
using namespace gtl;

namespace gtl
{
    class UnitTestRiemannianGeodesic
    {
    public:
        UnitTestRiemannianGeodesic();

    private:
        // Add test function declarations here.
    };
}

UnitTestRiemannianGeodesic::UnitTestRiemannianGeodesic()
{
    UTInformation("Mathematics/Curves/RiemannianGeodesic [tested by GTL/VisualTests/Curves/GeodesicHeightField]");

    // Add member test function calls here.
}

// Add member test function implementations here.

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Curves/RiemannianGeodesic.h>

namespace gtl
{
    template class RiemannianGeodesic<float>;
    template class RiemannianGeodesic<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class RiemannianGeodesic<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(RiemannianGeodesic)
