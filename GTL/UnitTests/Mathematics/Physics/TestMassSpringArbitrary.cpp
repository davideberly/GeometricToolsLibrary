#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Physics/MassSpringArbitrary.h>
using namespace gtl;

namespace gtl
{
    class UnitTestMassSpringArbitrary
    {
    public:
        UnitTestMassSpringArbitrary();

    private:
    };
}

UnitTestMassSpringArbitrary::UnitTestMassSpringArbitrary()
{
    UTInformation("Mathematics/Physics/MassSpringArbitrary [tested by GTL/VisualTests/Mathematics/Physics/GelatinBlob]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Physics/MassSpringArbitrary.h>

namespace gtl
{
    template class MassSpringArbitrary<float, 2>;
    template class MassSpringArbitrary<double, 3>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class MassSpringArbitrary<Rational, 4>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(MassSpringArbitrary)
