#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/3D/ApprParaboloid3.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestApprParaboloid3
    {
    public:
        UnitTestApprParaboloid3();

    private:
        void Test();
    };
}

UnitTestApprParaboloid3::UnitTestApprParaboloid3()
{
    UTInformation("Mathematics/Approximation/3D/ApprParaboloid3");

    Test();
}

void UnitTestApprParaboloid3::Test()
{
    // Generate points approximately on the surface
    // z = k0*x^2 + k1*x*y + k2*y^2 + k3*x + k4*y + k5
    double k0 = 1.0;
    double k1 = 0.001;
    double k2 = 2.0;
    double k3 = -1.12345;
    double k4 = +0.67891;
    double k5 = -0.03053;

    std::default_random_engine dre{};
    std::uniform_real_distribution<double> urd(-1.0e-4, +1.0e-4);
    std::vector<Vector3<double>> points(1024);
    for (std::size_t y = 0, i = 0; y < 32; ++y)
    {
        for (std::size_t x = 0; x < 32; ++x, ++i)
        {
            auto& q = points[i];
            q[0] = -1.0 + 2.0 * static_cast<double>((x + urd(dre)) / 32.0);
            q[1] = -1.0 + 2.0 * static_cast<double>((y + urd(dre)) / 32.0);
            q[2] = k0 * q[0] * q[0] + k1 * q[0] * q[1] + k2 * q[1] * q[1] + k3 * q[0] + k4 * q[1] + k5 + urd(dre);
        }
    }

    Vector3<double> trn{ 1.0, 2.0, 3.0 };
    for (auto& point : points)
    {
        point += trn;
    }
    std::array<double, 6> u{};
    double rootMeanSquareError0 = 0.0;
    ApprParaboloid3<double>::Fit(points, u, &rootMeanSquareError0);
    // u[0] = 0.99999914987265359
    // u[1] = 0.00099612417638750284
    // u[2] = 1.9999955794968700
    // u[3] = -3.1254401968966223
    // u[4] = -7.3220722936320817
    // u[5] = 11.737083342303684
    // rootMeanSquareError0 = 3.3034706267422605e-09

    Vector3<double> average{};
    std::array<double, 6> v{};
    double rootMeanSquareError1 = 0.0;
    ApprParaboloid3<double>::FitRobust(points, average, v, &rootMeanSquareError1);
    // v[0] = 0.99999914987325556
    // v[1] = 0.00099612417737204991
    // v[2] = 1.9999955794982056
    // v[3] = -1.1859807998516487
    // v[4] = 0.55387604486124276
    // v[5] = -0.99902200413833886
    // average[0] = 0.96874996225398768
    // average[1] = 1.9687501872419999
    // average[2] = 3.9853170664821125
    // rootMeanSquareError1 = 3.3034706267463402e-09

    std::array<double, 6> d{};
    d[0] = u[0];
    d[1] = u[1];
    d[2] = u[2];
    d[3] = u[3] + 2.0 * average[0] * u[0] + average[1] * u[1];
    d[4] = u[4] + average[0] * u[1] + 2.0 * average[1] * u[2];
    d[5] = u[5]
        + u[0] * average[0] * average[0]
        + u[1] * average[0] * average[1]
        + u[2] * average[1] * average[1]
        + average[0] * u[3]
        + average[1] * u[4]
        - average[2];
    // d[0] = 0.99999914987265359
    // d[1] = 0.00099612417638750284
    // d[2] = 1.9999955794968700
    // d[3] = -1.1859807998515373
    // d[4] = 0.55387604486146369
    // d[5] = -0.99902200413772313
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Approximation/3D/ApprParaboloid3.h>

namespace gtl
{
    template class ApprParaboloid3<float>;
    template class ApprParaboloid3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ApprParaboloid3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprParaboloid3)
