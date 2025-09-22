#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/3D/ApprCylinder3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestApprCylinder3
    {
    public:
        UnitTestApprCylinder3();
    };
}

UnitTestApprCylinder3::UnitTestApprCylinder3()
{
    UTInformation("Mathematics/Approximation/3D/ApprCylinder3 [tested by GTL/VisualTests/Mathematics/Approximation/ApproximationCylinder3]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Approximation/3D/ApprCylinder3.h>

namespace gtl
{
    template class ApprCylinder3<float>;
    template class ApprCylinder3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ApprCylinder3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprCylinder3)
