#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Curves/BSplineCurveFit.h>
using namespace gtl;

namespace gtl
{
    class UnitTestBSplineCurveFit
    {
    public:
        UnitTestBSplineCurveFit();
    };
}

UnitTestBSplineCurveFit::UnitTestBSplineCurveFit()
{
    UTInformation("Mathematics/Curves/BSplineCurveFit [tested by GTL/VisualTests/Curves/BSplineCurveFitter]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Curves/BSplineCurveFit.h>

namespace gtl
{
    template class BSplineCurveFit<float, 2>;
    template class BSplineCurveFit<float, 3>;

    template class BSplineCurveFit<double, 2>;
    template class BSplineCurveFit<double, 3>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class BSplineCurveFit<Rational, 2>;
    template class BSplineCurveFit<Rational, 3>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(BSplineCurveFit)
