#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/ND/IntpBSplineUniformShared.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntpBSplineUniformShared
    {
    public:
        UnitTestIntpBSplineUniformShared();
    };
}

UnitTestIntpBSplineUniformShared::UnitTestIntpBSplineUniformShared()
{
    // Tested by derived classes IntpBSplineUniform1, IntpBSplineUniform2,
    // IntpBSplineUniform3, and IntpBSplineUniform.
    UTInformation("Mathematics/Interpolation/ND/IntpBSplineUniformShared");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Interpolation/ND/IntpBSplineUniformShared.h>

namespace gtl
{
    template class IntpBSplineUniformShared<float>;
    template class IntpBSplineUniformShared<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class IntpBSplineUniformShared<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpBSplineUniformShared)
