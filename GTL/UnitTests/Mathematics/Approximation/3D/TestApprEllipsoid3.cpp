#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/3D/ApprEllipsoid3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestApprEllipsoid3
    {
    public:
        UnitTestApprEllipsoid3();
    };
}

UnitTestApprEllipsoid3::UnitTestApprEllipsoid3()
{
    UTInformation("Mathematics/Approximation/3D/ApprEllipsoid3 [tested by GTL/VisualTests/Mathematics/Approximation/ApproximationEllipsoid3]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Approximation/3D/ApprEllipsoid3.h>

namespace gtl
{
    template class ApprEllipsoid3<float>;
    template class ApprEllipsoid3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ApprEllipsoid3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprEllipsoid3)
