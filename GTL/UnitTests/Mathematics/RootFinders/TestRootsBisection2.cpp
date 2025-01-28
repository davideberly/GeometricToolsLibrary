#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/RootFinders/RootsBisection2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestRootsBisection2
    {
    public:
        UnitTestRootsBisection2();

    private:
        using Rational = BSRational<UIntegerAP32>;

        void TestFPType();
        void TestAPType();
    };
}

UnitTestRootsBisection2::UnitTestRootsBisection2()
{
    UTInformation("Mathematics/RootFinders/RootsBisection2");

    TestFPType();
    TestAPType();
}

void UnitTestRootsBisection2::TestFPType()
{
    auto F = [](double const& x, double const& y)
    {
        return std::tan(x) - y;
    };

    auto G = [](double const& x, double const& y)
    {
        return std::log(y + 0.5) + x;
    };

    std::size_t constexpr maxIterations = 1024;
    RootsBisection2<double> bisector(maxIterations, maxIterations);

    double xMin = -C_PI_DIV_4<double>, xMax = C_PI_DIV_4<double>;
    double yMin = 0.0, yMax = 1.0;
    double xRoot, yRoot, fRoot, gRoot;
    bool hasRoot = bisector(F, G, xMin, xMax, yMin, yMax, xRoot, yRoot, fRoot, gRoot);

    UTAssert(
        hasRoot &&
        xRoot == 0.26273142129218852 &&
        yRoot == 0.26894839274547777 &&
        fRoot == 5.5511151231257827e-17 &&
        gRoot == -1.1102230246251565e-16,
        "Bisector failed to find root.");
}

void UnitTestRootsBisection2::TestAPType()
{
    auto F = [](Rational const& x, Rational const& y)
    {
        return std::tan(x) - y;
    };

    Rational const half(0.5);
    auto G = [&half](Rational const& x, Rational const& y)
    {
        return std::log(y + half) + x;
    };

    std::size_t constexpr precision = 64;
    std::size_t constexpr maxIterations = 1024;
    RootsBisection2<Rational> bisector(maxIterations, maxIterations, precision);

    Rational xMin(-C_PI_DIV_4<Rational>), xMax(C_PI_DIV_4<Rational>);
    Rational yMin(0), yMax(1);
    Rational xRoot, yRoot, fRoot, gRoot;
    bool hasRoot = bisector(F, G, xMin, xMax, yMin, yMax, xRoot, yRoot, fRoot, gRoot);

    double dXRoot = xRoot, dYRoot = yRoot, dFRoot = fRoot, dGRoot = gRoot;
    double expectedXRoot = 0.26273142129218852;
    double expectedYRoot = 0.26894839274547783;
    double dXError = std::fabs(dXRoot - expectedXRoot);
    double dYError = std::fabs(dYRoot - expectedYRoot);
    double dFError = std::fabs(dFRoot);
    double dGError = std::fabs(dGRoot);

    UTAssert(
        hasRoot &&
        dXError <= 1e-15 &&
        dYError <= 1e-15 &&
        dFError <= 1e-15 &&
        dGError <= 1e-15,
        "Bisector failed to find root.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/RootFinders/RootsBisection2.h>

namespace gtl
{
    template class RootsBisection2<float>;
    template class RootsBisection2<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class RootsBisection2<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(RootsBisection2)
