#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Interpolation/3D/IntpHermiteQuintic3.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestIntpHermiteQuintic3
    {
    public:
        UnitTestIntpHermiteQuintic3();

    private:
        using Interpolator = IntpHermiteQuintic3<double>;
        using Sample = Interpolator::Sample;

        void Test();
    };
}

UnitTestIntpHermiteQuintic3::UnitTestIntpHermiteQuintic3()
{
    UTInformation("Mathematics/Interpolation/3D/IntpHermiteQuintic3");

    Test();
}

void UnitTestIntpHermiteQuintic3::Test()
{
    double const maxError = 1e-10;
    double g{};
    double gx{}, gy{}, gz{};
    double gxx{}, gxy{}, gxz{}, gyy{}, gyz{}, gzz{};
    double gxxy{}, gxxz{}, gxyy{}, gxyz{}, gxzz{}, gyyz{}, gyzz{};
    double gxxyy{}, gxxyz{}, gxxzz{}, gxyyz{}, gxyzz{}, gyyzz{};
    double gxxyyz{}, gxxyzz{}, gxyyzz{};
    double gxxyyzz{};

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
                    urd(dre), urd(dre), urd(dre), urd(dre),
                    urd(dre), urd(dre), urd(dre), urd(dre),
                    urd(dre), urd(dre), urd(dre), urd(dre),
                    urd(dre), urd(dre), urd(dre), urd(dre),
                    urd(dre), urd(dre), urd(dre), urd(dre),
                    urd(dre), urd(dre), urd(dre));
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
                gxx = hermite(2, 0, 0, x, y, z);
                UTAssert(std::fabs(gxx - samples[i][j][k].Fxx) <= maxError,
                    "Incorrect gxx" + suffix);
                gxy = hermite(1, 1, 0, x, y, z);
                UTAssert(std::fabs(gxy - samples[i][j][k].Fxy) <= maxError,
                    "Incorrect gxy" + suffix);
                gxz = hermite(1, 0, 1, x, y, z);
                UTAssert(std::fabs(gxz - samples[i][j][k].Fxz) <= maxError,
                    "Incorrect gxz" + suffix);
                gyy = hermite(0, 2, 0, x, y, z);
                UTAssert(std::fabs(gyy - samples[i][j][k].Fyy) <= maxError,
                    "Incorrect gyy" + suffix);
                gyz = hermite(0, 1, 1, x, y, z);
                UTAssert(std::fabs(gyz - samples[i][j][k].Fyz) <= maxError,
                    "Incorrect gyz" + suffix);
                gzz = hermite(0, 0, 2, x, y, z);
                UTAssert(std::fabs(gzz - samples[i][j][k].Fzz) <= maxError,
                    "Incorrect gzz" + suffix);
                gxxy = hermite(2, 1, 0, x, y, z);
                UTAssert(std::fabs(gxxy - samples[i][j][k].Fxxy) <= maxError,
                    "Incorrect gxxy" + suffix);
                gxxz = hermite(2, 0, 1, x, y, z);
                UTAssert(std::fabs(gxxz - samples[i][j][k].Fxxz) <= maxError,
                    "Incorrect gxxz" + suffix);
                gxyy = hermite(1, 2, 0, x, y, z);
                UTAssert(std::fabs(gxyy - samples[i][j][k].Fxyy) <= maxError,
                    "Incorrect gxyy" + suffix);
                gxyz = hermite(1, 1, 1, x, y, z);
                UTAssert(std::fabs(gxyz - samples[i][j][k].Fxyz) <= maxError,
                    "Incorrect gxyz" + suffix);
                gxzz = hermite(1, 0, 2, x, y, z);
                UTAssert(std::fabs(gxzz - samples[i][j][k].Fxzz) <= maxError,
                    "Incorrect gxzz" + suffix);
                gyyz = hermite(0, 2, 1, x, y, z);
                UTAssert(std::fabs(gyyz - samples[i][j][k].Fyyz) <= maxError,
                    "Incorrect gyyz" + suffix);
                gyzz = hermite(0, 1, 2, x, y, z);
                UTAssert(std::fabs(gyzz - samples[i][j][k].Fyzz) <= maxError,
                    "Incorrect gyzz" + suffix);
                gxxyy = hermite(2, 2, 0, x, y, z);
                UTAssert(std::fabs(gxxyy - samples[i][j][k].Fxxyy) <= maxError,
                    "Incorrect gxxyy" + suffix);
                gxxyz = hermite(2, 1, 1, x, y, z);
                UTAssert(std::fabs(gxxyz - samples[i][j][k].Fxxyz) <= maxError,
                    "Incorrect gxxyz" + suffix);
                gxxzz = hermite(2, 0, 2, x, y, z);
                UTAssert(std::fabs(gxxzz - samples[i][j][k].Fxxzz) <= maxError,
                    "Incorrect gxxzz" + suffix);
                gxyyz = hermite(1, 2, 1, x, y, z);
                UTAssert(std::fabs(gxyyz - samples[i][j][k].Fxyyz) <= maxError,
                    "Incorrect gxyyz" + suffix);
                gxyzz = hermite(1, 1, 2, x, y, z);
                UTAssert(std::fabs(gxyzz - samples[i][j][k].Fxyzz) <= maxError,
                    "Incorrect gxyzz" + suffix);
                gyyzz = hermite(0, 2, 2, x, y, z);
                UTAssert(std::fabs(gyyzz - samples[i][j][k].Fyyzz) <= maxError,
                    "Incorrect gyyzz" + suffix);
                gxxyyz = hermite(2, 2, 1, x, y, z);
                UTAssert(std::fabs(gxxyyz - samples[i][j][k].Fxxyyz) <= maxError,
                    "Incorrect gxxyyz" + suffix);
                gxxyzz = hermite(2, 1, 2, x, y, z);
                UTAssert(std::fabs(gxxyzz - samples[i][j][k].Fxxyzz) <= maxError,
                    "Incorrect gxxyzz" + suffix);
                gxyyzz = hermite(1, 2, 2, x, y, z);
                UTAssert(std::fabs(gxyyzz - samples[i][j][k].Fxyyzz) <= maxError,
                    "Incorrect gxyyzz" + suffix);
                gxxyyzz = hermite(2, 2, 2, x, y, z);
                UTAssert(std::fabs(gxxyyzz - samples[i][j][k].Fxxyyzz) <= maxError,
                    "Incorrect gxxyyzz" + suffix);
            }
        }
    }
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Interpolation/3D/IntpHermiteQuintic3.h>

namespace gtl
{
    template class IntpHermiteQuintic3<float>;
    template class IntpHermiteQuintic3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class IntpHermiteQuintic3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntpHermiteQuintic3)
