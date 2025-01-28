#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/3D/IntpHermiteCubic3.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestIntpHermiteCubic3
    {
    public:
        UnitTestIntpHermiteCubic3();

    private:
        using Interpolator = IntpHermiteCubic3<double>;
        using Sample = Interpolator::Sample;

        void Test();
    };
}

UnitTestIntpHermiteCubic3::UnitTestIntpHermiteCubic3()
{
    UTInformation("Mathematics/Interpolation/3D/IntpHermiteCubic3");

    Test();
}

void UnitTestIntpHermiteCubic3::Test()
{
    double const maxError = 1e-14;
    double g{}, gx{}, gy{}, gz{}, gxy{}, gxz{}, gyz{}, gxyz{};

    std::default_random_engine dre{};
    std::uniform_real_distribution<double> urd(-1.0, 1.0);
    std::array<std::array<std::array<Sample, 2>, 2>, 2> samples{};
    for (std::size_t i = 0; i < 2; ++i)
    {
        for (std::size_t j = 0; j < 2; ++j)
        {
            for (std::size_t k = 0; k < 2; ++k)
            {
                samples[i][j][k] = Sample(
                    urd(dre), urd(dre), urd(dre), urd(dre),
                    urd(dre), urd(dre), urd(dre), urd(dre));
            }
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
            std::string suffixj = suffixi + std::to_string(j);
            for (std::size_t k = 0; k < 2; ++k)
            {
                double z = static_cast<double>(k);
                std::string suffix = suffixj + std::to_string(k) + ".";
                g = hermite(0, 0, 0, x, y, z);
                UTAssert(std::fabs(g - samples[i][j][k].F) <= maxError,
                    "Incorrect g" + suffix);
                gx = hermite(1, 0, 0, x, y, z);
                UTAssert(std::fabs(gx - samples[i][j][k].Fx) <= maxError,
                    "Incorrect gx" + suffix);
                gy = hermite(0, 1, 0, x, y, z);
                UTAssert(std::fabs(gy - samples[i][j][k].Fy) <= maxError,
                    "Incorrect gy" + suffix);
                gz = hermite(0, 0, 1, x, y, z);
                UTAssert(std::fabs(gz - samples[i][j][k].Fz) <= maxError,
                    "Incorrect gz" + suffix);
                gxy = hermite(1, 1, 0, x, y, z);
                UTAssert(std::fabs(gxy - samples[i][j][k].Fxy) <= maxError,
                    "Incorrect gxy" + suffix);
                gxz = hermite(1, 0, 1, x, y, z);
                UTAssert(std::fabs(gxz - samples[i][j][k].Fxz) <= maxError,
                    "Incorrect gxz" + suffix);
                gyz = hermite(0, 1, 1, x, y, z);
                UTAssert(std::fabs(gyz - samples[i][j][k].Fyz) <= maxError,
                    "Incorrect gyz" + suffix);
                gxyz = hermite(1, 1, 1, x, y, z);
                UTAssert(std::fabs(gxyz - samples[i][j][k].Fxyz) <= maxError,
                    "Incorrect gxyz" + suffix);
            }
        }
    }
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Interpolation/3D/IntpHermiteCubic3.h>

namespace gtl
{
    template class IntpHermiteCubic3<float>;
    template class IntpHermiteCubic3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class IntpHermiteCubic3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpHermiteCubic3)
