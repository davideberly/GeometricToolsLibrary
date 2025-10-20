#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Volumes/ParametricVolume.h>
using namespace gtl;

namespace gtl
{
    class UnitTestParametricVolume
    {
    public:
        UnitTestParametricVolume();

    private:
    };
}

UnitTestParametricVolume::UnitTestParametricVolume()
{
    UTInformation("Mathematics/Volumes/ParametricVolume [tested by GTL/VisualTests/Mathematics/Physics/GelatinCube]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Volumes/ParametricVolume.h>

namespace gtl
{
    template class ParametricVolume<float, 3>;
    template class ParametricVolume<double, 4>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ParametricVolume<Rational, 2>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ParametricVolume)
