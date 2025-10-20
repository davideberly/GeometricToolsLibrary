#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Physics/MassSpringVolume.h>
using namespace gtl;

namespace gtl
{
    class UnitTestMassSpringVolume
    {
    public:
        UnitTestMassSpringVolume();

    private:
    };
}

UnitTestMassSpringVolume::UnitTestMassSpringVolume()
{
    UTInformation("Mathematics/Physics/MassSpringVolume [tested by GTL/VisualTests/Mathematics/Physics/GelatinCube]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Physics/MassSpringVolume.h>

namespace gtl
{
    template class MassSpringVolume<float, 2>;
    template class MassSpringVolume<double, 3>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class MassSpringVolume<Rational, 4>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(MassSpringVolume)
