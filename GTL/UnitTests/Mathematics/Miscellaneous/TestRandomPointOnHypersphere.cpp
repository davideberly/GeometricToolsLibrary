#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Miscellaneous/RandomPointOnHypersphere.h>
#include <algorithm>
using namespace gtl;

namespace gtl
{
    class UnitTestRandomPointOnHypersphere
    {
    public:
        UnitTestRandomPointOnHypersphere();

    private:
        void Test2();
        void Test3();
        void Test4();
    };
}

UnitTestRandomPointOnHypersphere::UnitTestRandomPointOnHypersphere()
{
    UTInformation("Mathematics/Miscellaneous/RandomPointOnHypersphere");

    Test2();
    Test3();
    Test4();
}

void UnitTestRandomPointOnHypersphere::Test2()
{
    RandomPointOnHypersphere<double, 2> rph;

    std::vector<std::array<double, 2>> points(8192);
    for (auto& point : points)
    {
        rph.Generate(point);
    }

    double constexpr maxError = 1e-08;
    for (std::size_t i = 0; i < points.size(); ++i)
    {
        auto const& point = points[i];
        double error = std::fabs(std::sqrt(point[0] * point[0] + point[1] * point[1]) - 1.0);
        UTAssert(error <= maxError, "Point is not numerically unit length.");
    }

#if defined(GTL_USE_MSWINDOWS)
    double const angle = C_TWO_PI<double> / 256.0;
    std::vector<std::size_t> histogram;
    rph.Histogram(points, angle, histogram);
    auto extreme = std::minmax_element(histogram.begin(), histogram.end());
    UTAssert(
        *extreme.first == 42 && *extreme.second == 88,
        "Unexpected histogram extremes.");

    std::nth_element(histogram.begin(), histogram.begin() + histogram.size() / 2, histogram.end());
    UTAssert(
        histogram[histogram.size() / 2] == 64,
        "Unexpected histogram median.");
#endif
}

void UnitTestRandomPointOnHypersphere::Test3()
{
    RandomPointOnHypersphere<double, 3> rph;

    std::vector<std::array<double, 3>> points(8192);
    for (auto& point : points)
    {
        rph.Generate(point);
    }

    double constexpr maxError = 1e-08;
    for (std::size_t i = 0; i < points.size(); ++i)
    {
        auto const& point = points[i];
        double error = std::fabs(std::sqrt(point[0] * point[0] + point[1] * point[1]
            + point[2] * point[2]) - 1.0);
        UTAssert(error <= maxError, "Point is not numerically unit length.");
    }

#if defined(GTL_USE_MSWINDOWS)
    double const angle = C_TWO_PI<double> / 64.0;
    std::vector<std::size_t> histogram;
    rph.Histogram(points, angle, histogram);
    auto extreme = std::minmax_element(histogram.begin(), histogram.end());
    UTAssert(
        *extreme.first == 7 && *extreme.second == 43,
        "Unexpected histogram extremes.");

    std::nth_element(histogram.begin(), histogram.begin() + histogram.size() / 2, histogram.end());
    UTAssert(
        histogram[histogram.size() / 2] == 21,
        "Unexpected histogram median.");
#endif
}

void UnitTestRandomPointOnHypersphere::Test4()
{
    RandomPointOnHypersphere<double, 4> rph;

    std::vector<std::array<double, 4>> points(8192);
    for (auto& point : points)
    {
        rph.Generate(point);
    }

    double constexpr maxError = 1e-08;
    for (std::size_t i = 0; i < points.size(); ++i)
    {
        auto const& point = points[i];
        double error = std::fabs(std::sqrt(point[0] * point[0] + point[1] * point[1]
            + point[2] * point[2] + point[3] * point[3]) - 1.0);
        UTAssert(error <= maxError, "Point is not numerically unit length.");
    }

#if defined(GTL_USE_MSWINDOWS)
    double const angle = C_TWO_PI<double> / 16.0;
    std::vector<std::size_t> histogram;
    rph.Histogram(points, angle, histogram);
    auto extreme = std::minmax_element(histogram.begin(), histogram.end());
    UTAssert(
        *extreme.first == 48 && *extreme.second == 244,
        "Unexpected histogram extremes.");

    std::nth_element(histogram.begin(), histogram.begin() + histogram.size() / 2, histogram.end());
    UTAssert(
        histogram[histogram.size() / 2] == 112,
        "Unexpected histogram median.");
#endif
}

#else

#include <GTL/Mathematics/Miscellaneous/RandomPointOnHypersphere.h>

namespace gtl
{
    template class RandomPointOnHypersphere<float, 2>;
    template class RandomPointOnHypersphere<double, 3>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(RandomPointOnHypersphere)
