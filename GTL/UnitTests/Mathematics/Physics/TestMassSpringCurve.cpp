#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Physics/MassSpringCurve.h>
using namespace gtl;

namespace gtl
{
    class UnitTestMassSpringCurve
    {
    public:
        UnitTestMassSpringCurve();

    private:
    };
}

UnitTestMassSpringCurve::UnitTestMassSpringCurve()
{
    UTInformation("Mathematics/Physics/MassSpringCurve [tested by GTL/VisualTests/Mathematics/Physics/Rope]");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Physics/MassSpringCurve.h>

namespace gtl
{
    template class MassSpringCurve<float, 2>;
    template class MassSpringCurve<double, 3>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class MassSpringCurve<Rational, 4>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(MassSpringCurve)
