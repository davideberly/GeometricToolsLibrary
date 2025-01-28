#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Surfaces/NURBSSphere.h>
using namespace gtl;

namespace gtl
{
    class UnitTestNURBSSphere
    {
    public:
        UnitTestNURBSSphere();
    };
}

UnitTestNURBSSphere::UnitTestNURBSSphere()
{
    UTInformation("Mathematics/Surfaces/NURBSSphere [tested by GTL/VisualTests/Surfaces/NURBSSphere]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Surfaces/NURBSSphere.h>

namespace gtl
{
    template class NURBSEighthSphereDegree4<float>;
    template class NURBSHalfSphereDegree3<float>;
    template class NURBSFullSphereDegree3<float>;

    template class NURBSEighthSphereDegree4<double>;
    template class NURBSHalfSphereDegree3<double>;
    template class NURBSFullSphereDegree3<double>;
    
#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class NURBSEighthSphereDegree4<Rational>;
    template class NURBSHalfSphereDegree3<Rational>;
    template class NURBSFullSphereDegree3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(NURBSSphere)
