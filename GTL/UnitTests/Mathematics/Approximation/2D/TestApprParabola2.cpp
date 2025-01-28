#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/2D/ApprParabola2.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestApprParabola2
    {
    public:
        UnitTestApprParabola2();

    private:
        void Test();
    };
}

UnitTestApprParabola2::UnitTestApprParabola2()
{
    UTInformation("Mathematics/Approximation/2D/ApprParabola2");

    Test();
}

void UnitTestApprParabola2::Test()
{
    double const maxError = 1e-03;

    // Theoretically, y = 3.0 * x^2 - 2.0 * x + 1.0.
    double const u0 = 3.0, u1 = -2.0, u2 = 1.0;
    std::default_random_engine dre{};
    std::uniform_real_distribution<double> urd(-1.0e-03, +1.0e-03);
    std::vector<Vector2<double>> points(1024);
    for (std::size_t i = 0; i < points.size(); ++i)
    {
        points[i][0] = static_cast<double>(i) / static_cast<double>(points.size());
        points[i][1] = u2 + points[i][0] * (u1 + points[i][0] * u0) + urd(dre);
    }

    std::array<double, 3> u{};
    double meanSquareError0 = 0.0;
    ApprParabola2<double> fitter{};
    fitter.Fit(points, u, &meanSquareError0);
    UTAssert(std::fabs(u[0] - 3.0) <= maxError, "Invalid u[0].");
    UTAssert(std::fabs(u[1] + 2.0) <= maxError, "Invalid u[1].");
    UTAssert(std::fabs(u[2] - 1.0) <= maxError, "Invalid u[2].");
    UTAssert(meanSquareError0 <= 1e-06, "Invalid meanSquareError0.");
    // u = (3.0003017725897152, -2.0003060150785359, 1.0000576324162669)
    // meanSquareError0 = 3.4353850571308352e-07

    std::array<double, 3> v{};
    Vector2<double> average{};
    double meanSquareError1 = 0.0;
    fitter.FitRobust(points, average, v, &meanSquareError1);
    double uu0 = v[0];
    double uu1 = -2.0 * v[0] * average[0] + v[1];
    double uu2 = v[0] * average[0] * average[0] - v[1] * average[0] + v[2] + average[1];
    UTAssert(std::fabs(uu0 - 3.0) <= maxError, "Invalid uu[0].");
    UTAssert(std::fabs(uu1 + 2.0) <= maxError, "Invalid uu[1].");
    UTAssert(std::fabs(uu2 - 1.0) <= maxError, "Invalid uu[2].");
    UTAssert(meanSquareError1 <= 1e-06, "Invalid meanSquareError1.");
}

#else

#include <GTL/Mathematics/Approximation/2D/ApprParabola2.h>

namespace gtl
{
    template class ApprParabola2<float>;
    template class ApprParabola2<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprParabola2)
