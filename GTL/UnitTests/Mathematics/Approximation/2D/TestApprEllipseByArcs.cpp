#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/2D/ApprEllipseByArcs.h>
using namespace gtl;

namespace gtl
{
    class UnitTestApprEllipseByArcs
    {
    public:
        UnitTestApprEllipseByArcs();
    };
}

UnitTestApprEllipseByArcs::UnitTestApprEllipseByArcs()
{
    UTInformation("Mathematics/Approximation/2D/ApprEllipseByArcs [tested by GTL/VisualTests/Approximation/ApproximationEllipseByArcs]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Approximation/2D/ApprEllipseByArcs.h>

namespace gtl
{
    template class ApprEllipseByArcs<float>;
    template class ApprEllipseByArcs<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ApprEllipseByArcs<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprEllipseByArcs)
