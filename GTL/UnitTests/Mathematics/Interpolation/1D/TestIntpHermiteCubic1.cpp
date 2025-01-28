#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/1D/IntpHermiteCubic1.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestIntpHermiteCubic1
    {
    public:
        UnitTestIntpHermiteCubic1();

    private:
        using Interpolator = IntpHermiteCubic1<double>;
        using Sample = Interpolator::Sample;

        void Test();
    };
}

UnitTestIntpHermiteCubic1::UnitTestIntpHermiteCubic1()
{
    UTInformation("Mathematics/Interpolation/1D/IntpHermiteCubic1");

    Test();
}

void UnitTestIntpHermiteCubic1::Test()
{
    double const maxError = 1e-14;
    double g{}, gx{};

    std::default_random_engine dre{};
    std::uniform_real_distribution<double> urd(-1.0, 1.0);
    std::array<Sample, 2> samples{};  // (F,Fx)
    for (std::size_t i = 0; i < 2; ++i)
    {
        samples[i] = Sample(urd(dre), urd(dre));
    }

    Interpolator hermite(samples);

    for (std::size_t i = 0; i < 2; ++i)
    {
        double x = static_cast<double>(i);
        std::string suffix = std::to_string(i) + ".";
        g = hermite(0, x);
        UTAssert(std::fabs(g - samples[i].F) <= maxError,
            "Incorrect g" + suffix);
        gx = hermite(1, x);
        UTAssert(std::fabs(gx - samples[i].Fx) <= maxError,
            "Incorrect gx" + suffix);
    }
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Interpolation/1D/IntpHermiteCubic1.h>

namespace gtl
{
    template class IntpHermiteCubic1<float>;
    template class IntpHermiteCubic1<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class IntpHermiteCubic1<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpHermiteCubic1)
