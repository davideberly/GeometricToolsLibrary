#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/3D/IntpCubic3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntpCubic3
    {
    public:
        UnitTestIntpCubic3();

    private:
        // Add test function declarations here.
    };
}

UnitTestIntpCubic3::UnitTestIntpCubic3()
{
    UTInformation("Mathematics/Interpolation/3D/IntpCubic3 [NEEDS UNIT TESTS]");

    // Add member test function calls here.
}

// Add member test function implementations here.

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Interpolation/3D/IntpCubic3.h>

namespace gtl
{
    template class IntpCubic3<float>;
    template class IntpCubic3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class IntpCubic3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpCubic3)
