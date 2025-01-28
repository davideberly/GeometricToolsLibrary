#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/RootFinders/RootsBisection1.h>
using namespace gtl;

namespace gtl
{
    class UnitTestRootsBisection1
    {
    public:
        UnitTestRootsBisection1();

    private:
        using Rational = BSRational<UIntegerAP32>;

        void TestFPType();
        void TestAPType();
    };
}

UnitTestRootsBisection1::UnitTestRootsBisection1()
{
    UTInformation("Mathematics/RootFinders/RootsBisection1");

    TestFPType();
    TestAPType();
}

void UnitTestRootsBisection1::TestFPType()
{
    auto F = [](double const& t)
    {
        return std::exp(-t) * std::sin(t) / t - 0.1;
    };

    std::size_t constexpr maxIterations = 1024;
    RootsBisection1<double> bisector(maxIterations);

    double tMin = 1.0, tMax = 2.0;
    double tRoot, fRoot;
    bool hasRoot = bisector(F, tMin, tMax, tRoot, fRoot);

    UTAssert(
        hasRoot &&
        bisector.GetNumIterations() == 53 &&
        tRoot == 1.7367417146411115 &&
        fRoot == 1.3877787807814457e-17,
        "Bisector failed to find root.");


    auto G = [](double const& t)
    {
        return 1.0 / t - 1.1;
    };

    tMin = 0.0;
    tMax = 2.0;
    double fMin = 1.0, fMax = G(tMax);
    hasRoot = bisector(G, tMin, tMax, fMin, fMax, tRoot, fRoot);

    UTAssert(
        hasRoot &&
        bisector.GetNumIterations() == 53 &&
        tRoot == 0.90909090909090895 &&
        fRoot == 0.0,
        "Bisector failed to find root.");
}

void UnitTestRootsBisection1::TestAPType()
{
    Rational const oneTenth(0.1);
    auto F = [&oneTenth](Rational const& t)
    {
        return std::exp(-t) * std::sin(t) / t - oneTenth;
    };

    std::size_t constexpr precision = 64;
    std::size_t constexpr maxIterations = 1024;
    RootsBisection1<Rational> bisector(maxIterations, precision);

    Rational tMin(1.0), tMax(2.0);
    Rational tRoot, fRoot;
    bool hasRoot = bisector(F, tMin, tMax, tRoot, fRoot);
    double dTRoot = tRoot;
    double dFRoot = fRoot;
    double expectedTRoot = 1.7367417146411115;
    double dTError = std::fabs(dTRoot - expectedTRoot);
    double dFError = std::fabs(dFRoot);
    UTAssert(
        hasRoot &&
        bisector.GetNumIterations() == 64 &&
        dTError <= 1e-15 &&
        dFError <= 1e-15,
        "Bisector failed to find root.");


    Rational const one(1.0), number(1.1);
    auto G = [&one, &number](Rational const& t)
    {
        return one / t - number;
    };

    tMin = 0;
    tMax = 2;
    Rational fMin(1), fMax = G(tMax);
    hasRoot = bisector(G, tMin, tMax, fMin, fMax, tRoot, fRoot);
    dTRoot = tRoot;
    dFRoot = fRoot;
    expectedTRoot = 0.90909090909090906;
    dTError = std::fabs(dTRoot - expectedTRoot);
    dFError = std::fabs(dFRoot);
    UTAssert(
        hasRoot &&
        bisector.GetNumIterations() == 66 &&
        dTError <= 1e-15 &&
        dFError <= 1e-15,
        "Bisector failed to find root.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/RootFinders/RootsBisection1.h>

namespace gtl
{
    template class RootsBisection1<float>;
    template class RootsBisection1<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class RootsBisection1<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(RootsBisection1)
