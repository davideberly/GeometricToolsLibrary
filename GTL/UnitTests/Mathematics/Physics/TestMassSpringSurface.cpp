#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Physics/MassSpringSurface.h>
using namespace gtl;

namespace gtl
{
    class UnitTestMassSpringSurface
    {
    public:
        UnitTestMassSpringSurface();

    private:
    };
}

UnitTestMassSpringSurface::UnitTestMassSpringSurface()
{
    UTInformation("Mathematics/Physics/MassSpringSurface [tested by GTL/VisualTests/Mathematics/Physics/Cloth]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Physics/MassSpringSurface.h>

namespace gtl
{
    template class MassSpringSurface<float, 2>;
    template class MassSpringSurface<double, 3>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class MassSpringSurface<Rational, 4>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(MassSpringSurface)
