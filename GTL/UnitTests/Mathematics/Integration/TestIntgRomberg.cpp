#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Mathematics/Integration/IntgRomberg.h>
#include <GTL/Mathematics/Algebra/Vector.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntgRomberg
    {
    public:
        UnitTestIntgRomberg();

    private:
        void Test();
    };
}

UnitTestIntgRomberg::UnitTestIntgRomberg()
{
    UTInformation("Mathematics/Integration/IntgRomberg");

    // Integral(sin(x);0,pi/2) = 1
    Test();
}

void UnitTestIntgRomberg::Test()
{
    std::function<double(double)> integrand = [](double x) { return sin(x); };

    std::size_t const order = 5;
    double const a = 0.0, b = C_PI_DIV_2<double>;
    double value = IntgRomberg<double>::Integrate(order, a, b, integrand);
    double error = std::fabs(value - 1.0);
    UTAssert(
        error <= 1e-11,
        "Romberg failed."
    );
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Integration/IntgRomberg.h>

namespace gtl
{
    template class IntgRomberg<float>;
    template class IntgRomberg<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class IntgRomberg<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntgRomberg)
