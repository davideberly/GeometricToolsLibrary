#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Mathematics/Integration/IntgTrapezoidRule.h>
#include <GTL/Mathematics/Algebra/Vector.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntgTrapezoidRule
    {
    public:
        UnitTestIntgTrapezoidRule();

    private:
        void Test();
    };
}

UnitTestIntgTrapezoidRule::UnitTestIntgTrapezoidRule()
{
    UTInformation("Mathematics/Integration/IntgTrapezoidRule");

    // Integral(sin(x);0,pi/2) = 1
    Test();
}

void UnitTestIntgTrapezoidRule::Test()
{
    std::function<double(double)> integrand = [](double x) { return sin(x); };

    std::size_t const numSamples = 1024;
    double const a = 0.0, b = C_PI_DIV_2<double>;
    double value = IntgTrapezoidRule<double>::Integrate(numSamples, a, b, integrand);
    double error = std::fabs(value - 1.0);
    UTAssert(
        error <= 1e-6,
        "TrapezoidRule failed."
    );
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Integration/IntgTrapezoidRule.h>

namespace gtl
{
    template class IntgTrapezoidRule<float>;
    template class IntgTrapezoidRule<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class IntgTrapezoidRule<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntgTrapezoidRule)
