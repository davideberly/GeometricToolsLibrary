#if defined(GTL_UNIT_TESTS)

#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/RootFinders/PolynomialRoot.h>
using namespace gtl;

namespace gtl
{
    class UnitTestPolynomialRoot
    {
    public:
        UnitTestPolynomialRoot();

    private:
        void Test();
    };
}

UnitTestPolynomialRoot::UnitTestPolynomialRoot()
{
    UTInformation("Mathematics/RootFinders/PolynomialRoot");

    Test();
}

void UnitTestPolynomialRoot::Test()
{
    PolynomialRoot<double> root0{}, root1{}, root2{};
    root0 = { 1.0, 1 };
    root1 = { 2.0, 2 };
    root2 = { 1.0, 3 };

    UTAssert(root0 < root1, "Invalid less-than result.");
    UTAssert(root0 == root2, "Invalid equality result.");

    double c0 = -0.12345, c1 = -0.67891, c2 = 2.34567;
    auto F = [&, c0, c1, c2](double x)
    {
        return std::fma(x, std::fma(x, c2, c1), c0);
    };

    double xMin = -0.5, xMax = 0.0;
    PolynomialRootBisect<double>(F, +1, -1, xMin, xMax);
    UTAssert(xMin == -0.12652506024738283, "Invalid xMin.");
    UTAssert(xMax == -0.12652506024738280, "Invalid xMax.");

    xMin = 0.0;
    xMax = 1.0;
    PolynomialRootBisect<double>(F, -1, +1, xMin, xMax);
    UTAssert(xMin == 0.41595622490396278, "Invalid xMin.");
    UTAssert(xMax == 0.41595622490396283, "Invalid xMin.");
}

#else

#include <GTL/Mathematics/RootFinders/PolynomialRoot.h>

namespace gtl
{
    template struct PolynomialRoot<float>;
    template void PolynomialRootBisect<float>(std::function<float(float)> const&,
        std::int32_t, std::int32_t, float&, float&);

    template struct PolynomialRoot<double>;
    template void PolynomialRootBisect<double>(std::function<double(double)> const&,
        std::int32_t, std::int32_t, double&, double&);
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(PolynomialRoot)
