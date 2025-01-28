#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Curves/BasisFunction.h>
using namespace gtl;

namespace gtl
{
    class UnitTestBasisFunction
    {
    public:
        UnitTestBasisFunction();
    };
}

UnitTestBasisFunction::UnitTestBasisFunction()
{
    UTInformation("Mathematics/Curves/BasisFunction [tested by GTL/VisualTests/Curves/BSplineCurveFitter]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Curves/BasisFunction.h>

namespace gtl
{
    template class BasisFunction<float>;
    template class BasisFunction<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class BasisFunction<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(BasisFunction)
