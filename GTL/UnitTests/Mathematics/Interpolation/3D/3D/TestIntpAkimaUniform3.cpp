#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/3D/IntpAkimaUniform3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntpAkimaUniform3
    {
    public:
        UnitTestIntpAkimaUniform3();

    private:
        // Add test function declarations here.
    };
}

UnitTestIntpAkimaUniform3::UnitTestIntpAkimaUniform3()
{
    UTInformation("Mathematics/Interpolation/3D/IntpAkimaUniform3 [NEEDS UNIT TESTS]");

    // Add member test function calls here.
}

// Add member test function implementations here.

#else

#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#include <GTL/Mathematics/Interpolation/3D/IntpAkimaUniform3.h>

namespace gtl
{
    template class IntpAkimaUniform3<float>;
    template class IntpAkimaUniform3<double>;

    using Rational = BSRational<UIntegerAP32>;
    template class IntpAkimaUniform3<Rational>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpAkimaUniform3)
