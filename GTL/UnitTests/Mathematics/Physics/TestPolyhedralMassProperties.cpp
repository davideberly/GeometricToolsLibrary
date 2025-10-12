#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Physics/PolyhedralMassProperties.h>
using namespace gtl;

namespace gtl
{
    class UnitTestPolyhedralMassProperties
    {
    public:
        UnitTestPolyhedralMassProperties();

    private:
    };
}

UnitTestPolyhedralMassProperties::UnitTestPolyhedralMassProperties()
{
    UTInformation("Mathematics/Physics/PolyhedralMassProperties [tested by GTL/VisualTests/Mathematics/Physics/BouncingTetrahedra]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Physics/PolyhedralMassProperties.h>

namespace gtl
{
    template class PolyhedralMassProperties<float>;
    template class PolyhedralMassProperties<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class PolyhedralMassProperties<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(PolyhedralMassProperties)
