#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Volumes/NURBSVolume.h>
using namespace gtl;

namespace gtl
{
    class UnitTestNURBSVolume
    {
    public:
        UnitTestNURBSVolume();

    private:
    };
}

UnitTestNURBSVolume::UnitTestNURBSVolume()
{
    UTInformation("Mathematics/Volumes/NURBSVolume [tested by GTL/VisualTests/Mathematics/Physics/FreeFormDeformation]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Volumes/NURBSVolume.h>

namespace gtl
{
    template class NURBSVolume<float, 3>;
    template class NURBSVolume<double, 4>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class NURBSVolume<Rational, 2>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(NURBSVolume)
