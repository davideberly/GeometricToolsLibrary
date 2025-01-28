#if defined(GTL_UNIT_TESTS)

#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/RootFinders/RootsLinear.h>
using namespace gtl;

namespace gtl
{
    class UnitTestRootsLinear
    {
    public:
        UnitTestRootsLinear();

    private:
        void Test();
    };
}

UnitTestRootsLinear::UnitTestRootsLinear()
{
    UTInformation("Mathematics/RootFinders/RootsLinear");

    Test();
}

void UnitTestRootsLinear::Test()
{
    PolynomialRoot<double> root{};
    std::size_t numRoots{};
    double g0{}, g1{};

    g0 = 2.0;
    g1 = 3.0;
    numRoots = RootsLinear<double>::Solve(g0, g1, &root);
    UTAssert(numRoots == 1, "Invalid number of roots.");
    UTAssert(root.x == -g0 / g1, "Invalid root.");
    UTAssert(root.m == 1, "Invalid multiplicity.");

    g0 = 2.0;
    g1 = 0.0;
    numRoots = RootsLinear<double>::Solve(g0, g1, &root);
    UTAssert(numRoots == 0, "Invalid number of roots.");

    g0 = 0.0;
    g1 = 3.0;
    numRoots = RootsLinear<double>::Solve(g0, g1, &root);
    UTAssert(numRoots == 1, "Invalid number of roots.");
    UTAssert(root.x == 0.0, "Invalid root.");
    UTAssert(root.m == 1, "Invalid multiplicity.");

    g0 = -3.0;
    numRoots = RootsLinear<double>::Solve(g0, &root);
    UTAssert(numRoots == 1, "Invalid number of roots.");
    UTAssert(root.x == -g0, "Invalid root.");
    UTAssert(root.m == 1, "Invalid multiplicity.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/RootFinders/RootsLinear.h>

namespace gtl
{
    template class RootsLinear<float>;
    template class RootsLinear<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class RootsLinear<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(RootsLinear)
