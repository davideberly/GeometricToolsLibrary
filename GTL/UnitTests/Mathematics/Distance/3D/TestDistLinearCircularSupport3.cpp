#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistLinearCircularSupport3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistLinearCircularSupport3
    {
    public:
        UnitTestDistLinearCircularSupport3();

    private:
        // Add test function declarations here.
    };
}

UnitTestDistLinearCircularSupport3::UnitTestDistLinearCircularSupport3()
{
    // Tested by DistLine3Arc3, DistRay3Arc3, and DistSegment3Arc3.
    UTInformation("Mathematics/Distance/3D/DistLinearCircularSupport3");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistLinearCircularSupport3.h>

namespace gtl
{
    template class DCPLinearCircularSupport3<float>;
    template class DCPLinearCircularSupport3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPLinearCircularSupport3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistLinearCircularSupport3)
