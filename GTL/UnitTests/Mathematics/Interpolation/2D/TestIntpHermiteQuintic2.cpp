#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/2D/IntpHermiteQuintic2.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestIntpHermiteQuintic2
    {
    public:
        UnitTestIntpHermiteQuintic2();

    private:
        using Interpolator = IntpHermiteQuintic2<double>;
        using Sample = Interpolator::Sample;

        void Test();
    };
}

UnitTestIntpHermiteQuintic2::UnitTestIntpHermiteQuintic2()
{
    UTInformation("Mathematics/Interpolation/2D/IntpHermiteQuintic2");

    Test();
}

void UnitTestIntpHermiteQuintic2::Test()
{
    double const maxError = 1e-12;
    double g{}, gx{}, gy{}, gxx{}, gxy{}, gyy{}, gxxy{}, gxyy{}, gxxyy{};

    std::default_random_engine dre{};
    std::uniform_real_distribution<double> urd(-1.0, 1.0);
    std::array<std::array<Sample, 2>, 2> samples{};  // (F,Fx,Fy,Fxx,Fxy,Fyy,Fxxy,Fxyy,Fxxyy)
    for (std::size_t i = 0; i < 2; ++i)
    {
        for (std::size_t j = 0; j < 2; ++j)
        {
            samples[i][j] = Sample(
                urd(dre), urd(dre), urd(dre),
                urd(dre), urd(dre), urd(dre),
                urd(dre), urd(dre), urd(dre)
            );
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
            gxx = hermite(2, 0, x, y);
            UTAssert(std::fabs(gxx - samples[i][j].Fxx) <= maxError,
                "Incorrect gxx" + suffix);
            gxy = hermite(1, 1, x, y);
            UTAssert(std::fabs(gxy - samples[i][j].Fxy) <= maxError,
                "Incorrect gxy" + suffix);
            gyy = hermite(0, 2, x, y);
            UTAssert(std::fabs(gyy - samples[i][j].Fyy) <= maxError,
                "Incorrect gyy" + suffix);
            gxxy = hermite(2, 1, x, y);
            UTAssert(std::fabs(gxxy - samples[i][j].Fxxy) <= maxError,
                "Incorrect gxxy" + suffix);
            gxyy = hermite(1, 2, x, y);
            UTAssert(std::fabs(gxyy - samples[i][j].Fxyy) <= maxError,
                "Incorrect gxyy" + suffix);
            gxxyy = hermite(2, 2, x, y);
            UTAssert(std::fabs(gxxyy - samples[i][j].Fxxyy) <= maxError,
                "Incorrect gxxyy" + suffix);
        }
    }
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Interpolation/2D/IntpHermiteQuintic2.h>

namespace gtl
{
    template class IntpHermiteQuintic2<float>;
    template class IntpHermiteQuintic2<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class IntpHermiteQuintic2<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpHermiteQuintic2)
