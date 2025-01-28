#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/3D/IntpThinPlateSpline3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntpThinPlateSpline3
    {
    public:
        UnitTestIntpThinPlateSpline3();

    private:
        // Add test function declarations here.
    };
}

UnitTestIntpThinPlateSpline3::UnitTestIntpThinPlateSpline3()
{
    UTInformation("Mathematics/Interpolation/3D/IntpThinPlateSpline3 [NEEDS UNIT TESTS]");

    // Add member test function calls here.
}

// Add member test function implementations here.

#else

#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#include <GTL/Mathematics/Interpolation/3D/IntpThinPlateSpline3.h>

namespace gtl
{
    template class IntpThinPlateSpline3<float>;
    template class IntpThinPlateSpline3<double>;

    using Rational = BSRational<UIntegerAP32>;
    template class IntpThinPlateSpline3<Rational>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpThinPlateSpline3)
