#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Minimizers/BrentsMinimizer.h>
#include <limits>
#include <vector>
using namespace gtl;

namespace gtl
{
    class UnitTestBrentsMinimizer
    {
    public:
        UnitTestBrentsMinimizer();

    private:
        void TestParabolas();
        void TestSinusoidals();
    };
}

UnitTestBrentsMinimizer::UnitTestBrentsMinimizer()
{
    UTInformation("Mathematics/Minimizers/BrentsMinimizer");

    TestParabolas();
    TestSinusoidals();
}

void UnitTestBrentsMinimizer::TestParabolas()
{
    std::vector<std::pair<double, double>> evaluations;

    auto F = [&evaluations](double const& x)
    {
        double result = x * x;
        evaluations.push_back(std::make_pair(x, result));
        return result;
    };

    auto G = [&evaluations](double const& x)
    {
        double result = (x - 1e-06f) * x;
        evaluations.push_back(std::make_pair(x, result));
        return result;
    };

    std::size_t maxSubdivisions = 8;
    std::size_t maxBisections = 8;
    double epsilon = 1e-08;
    double tolerance = 1e-04;
    BrentsMinimizer<double> minimizer(maxSubdivisions, maxBisections, epsilon, tolerance);

    double tMin = std::numeric_limits<double>::max();
    double fMin = std::numeric_limits<double>::max();
    evaluations.clear();
    minimizer(F, -1.0, 1.0, 0.0, tMin, fMin);
    UTAssert( // evaluations.size() == 27
        tMin == 0.0 && fMin == 0.0,
        "Brents minimization failed.");

    tMin = std::numeric_limits<double>::max();
    fMin = std::numeric_limits<double>::max();
    evaluations.clear();
    minimizer(F, -1.0, 1.0, 0.01, tMin, fMin);
    UTAssert( // evaluations.size() == 5
        tMin <= 1e-18 && std::fabs(fMin) <= 1e-37,
        "Brents minimization failed.");

    tMin = std::numeric_limits<double>::max();
    fMin = std::numeric_limits<double>::max();
    evaluations.clear();
    minimizer(G, -1.0, 1.0, 0.0, tMin, fMin);
    UTAssert( // evaluations.size() == 10
        tMin <= 1e-6 && std::fabs(fMin) <= 1e-12,
        "Brents minimization failed.");

    tMin = std::numeric_limits<double>::max();
    fMin = std::numeric_limits<double>::max();
    evaluations.clear();
    minimizer(G, -1.0, 1.0, 0.01, tMin, fMin);
    UTAssert( // evaluations.size() == 5
        tMin <= 1e-6 && std::fabs(fMin) <= 1e-12,
        "Brents minimization failed.");
}

void UnitTestBrentsMinimizer::TestSinusoidals()
{
    std::vector<std::pair<double, double>> evaluations;

    auto F = [&evaluations](double const& x)
    {
        double result = std::sin(x) + 0.0625 * std::cos(7.1 * x);
        evaluations.push_back(std::make_pair(x, result));
        return result;
    };

    auto G = [&evaluations](double const& x)
    {
        double result = std::sin(x) + 0.0625 * std::cos(7.1 * x) + 0.0375 * std::cos(10.2 * x);
        evaluations.push_back(std::make_pair(x, result));
        return result;
    };

    std::size_t maxSubdivisions = 8;
    std::size_t maxBisections = 8;
    double epsilon = 1e-08;
    double tolerance = 1e-04;
    BrentsMinimizer<double> minimizer(maxSubdivisions, maxBisections, epsilon, tolerance);

    double tMin = std::numeric_limits<double>::max();
    double fMin = std::numeric_limits<double>::max();
    evaluations.clear();
    minimizer(F, 0.0, C_TWO_PI<double>, tMin, fMin);
    UTAssert( // evaluations.size() == 11
        std::fabs(tMin - 4.8297107695939632) <= 1e-08
        && std::fabs(fMin + 1.0534181516413925) <= 1e-08,
        "Brents minimization failed.");

    tMin = std::numeric_limits<double>::max();
    fMin = std::numeric_limits<double>::max();
    evaluations.clear();
    minimizer(G, 4.0, 6.0, tMin, fMin);
    UTAssert( // evaluations.size() == 11
        std::fabs(tMin - 4.7311447093965171) <= 1e-08
        && std::fabs(fMin + 1.0512123478754345) <= 1e-08,
        "Brents minimization failed.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Minimizers/BrentsMinimizer.h>

namespace gtl
{
    template class BrentsMinimizer<float>;
    template class BrentsMinimizer<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class BrentsMinimizer<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(BrentsMinimizer)
