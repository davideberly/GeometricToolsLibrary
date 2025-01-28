#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/2D/ApprEllipse2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestApprEllipse2
    {
    public:
        UnitTestApprEllipse2();
    };
}

UnitTestApprEllipse2::UnitTestApprEllipse2()
{
    UTInformation("Mathematics/Approximation/2D/ApprEllipse2 [tested by GTL/VisualTests/Approximation/ApproximationEllipse2]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Approximation/2D/ApprEllipse2.h>

namespace gtl
{
    template class ApprEllipse2<float>;
    template class ApprEllipse2<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ApprEllipse2<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprEllipse2)
