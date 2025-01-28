#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/RootFinders/RootsBrentsMethod.h>
using namespace gtl;

namespace gtl
{
    class UnitTestRootsBrentsMethod
    {
    public:
        UnitTestRootsBrentsMethod();

    private:
        void Test();
    };
}

UnitTestRootsBrentsMethod::UnitTestRootsBrentsMethod()
{
    UTInformation("Mathematics/RootFinders/RootsBrentsMethod");

    Test();
}

void UnitTestRootsBrentsMethod::Test()
{
    auto F = [](double const& t)
    {
        return std::exp(-t) * std::sin(t) / t - 0.1;
    };

    std::size_t constexpr maxIterations = 1024;
    double constexpr negFTolerance = 0.0;
    double constexpr posFTolerance = 0.0;
    double constexpr stepTTolerance = 0.0;
    double constexpr convTTolerance = 0.0;
    RootsBrentsMethod<double> bisector(maxIterations, negFTolerance,
        posFTolerance, stepTTolerance, convTTolerance);

    double root, fAtRoot;
    std::size_t iterations = bisector(F, 1.0, 2.0, root, fAtRoot);

    UTAssert(
        iterations == 27 &&
        root == 1.7367417146411115 &&
        fAtRoot == 1.3877787807814457e-17,
        "Bisector failed to find root.");

    auto G = [](double const& t)
    {
        return 1.0 / t - 1.1;
    };

    double tmin = 0.0, tmax = 2.0, fmin = 1.0, fmax = G(tmax);
    iterations = bisector(G, tmin, tmax, fmin, fmax, root, fAtRoot);

    UTAssert(
        iterations == 10 &&
        root == 0.90909090909090906 &&
        fAtRoot == 0.0,
        "Bisector failed to find root.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/RootFinders/RootsBrentsMethod.h>

namespace gtl
{
    template class RootsBrentsMethod<float>;
    template class RootsBrentsMethod<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class RootsBrentsMethod<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(RootsBrentsMethod)
