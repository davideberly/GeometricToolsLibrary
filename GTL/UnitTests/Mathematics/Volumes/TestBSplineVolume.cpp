#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Volumes/BSplineVolume.h>
using namespace gtl;

namespace gtl
{
    class UnitTestBSplineVolume
    {
    public:
        UnitTestBSplineVolume();

    private:
    };
}

UnitTestBSplineVolume::UnitTestBSplineVolume()
{
    UTInformation("Mathematics/Volumes/BSplineVolume [tested by GTL/VisualTests/Mathematics/Physics/GelatinCube]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Volumes/BSplineVolume.h>

namespace gtl
{
    template class BSplineVolume<float, 3>;
    template class BSplineVolume<double, 4>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class BSplineVolume<Rational, 2>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(BSplineVolume)
