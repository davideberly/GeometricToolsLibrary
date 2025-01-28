#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/2D/IntpHermiteCubic2.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestIntpHermiteCubic2
    {
    public:
        UnitTestIntpHermiteCubic2();

    private:
        using Interpolator = IntpHermiteCubic2<double>;
        using Sample = Interpolator::Sample;

        void Test();
    };
}

UnitTestIntpHermiteCubic2::UnitTestIntpHermiteCubic2()
{
    UTInformation("Mathematics/Interpolation/2D/IntpHermiteCubic2");

    Test();
}

void UnitTestIntpHermiteCubic2::Test()
{
    double const maxError = 1e-14;
    double g{}, gx{}, gy{}, gxy{};

    std::default_random_engine dre{};
    std::uniform_real_distribution<double> urd(-1.0, 1.0);
    std::array<std::array<Sample, 2>, 2> samples{};  // (F, Fx, Fy, Fxy)
    for (std::size_t i = 0; i < 2; ++i)
    {
        for (std::size_t j = 0; j < 2; ++j)
        {
            samples[i][j] = Sample(urd(dre), urd(dre), urd(dre), urd(dre));
        }
    }

    Interpolator hermite(samples);

    for (std::size_t i = 0; i < 2; ++i)
    {
        double x = static_cast<double>(i);
        std::string suffixi = std::to_string(i);
        for (std::size_t j = 0; j < 2; ++j)
        {
            double y = static_cast<double>(j);
            std::string suffix = suffixi + std::to_string(j) + ".";
            g = hermite(0, 0, x, y);
            UTAssert(std::fabs(g - samples[i][j].F) <= maxError,
                "Incorrect g" + suffix);
            gx = hermite(1, 0, x, y);
            UTAssert(std::fabs(gx - samples[i][j].Fx) <= maxError,
                "Incorrect gx" + suffix);
            gy = hermite(0, 1, x, y);
            UTAssert(std::fabs(gy - samples[i][j].Fy) <= maxError,
                "Incorrect gy" + suffix);
            gxy = hermite(1, 1, x, y);
            UTAssert(std::fabs(gxy - samples[i][j].Fxy) <= maxError,
                "Incorrect gxy" + suffix);
        }
    }
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Interpolation/2D/IntpHermiteCubic2.h>

namespace gtl
{
    template class IntpHermiteCubic2<float>;
    template class IntpHermiteCubic2<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class IntpHermiteCubic2<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpHermiteCubic2)
