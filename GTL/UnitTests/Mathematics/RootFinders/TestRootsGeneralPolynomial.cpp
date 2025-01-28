#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/RootFinders/RootsGeneralPolynomial.h>
#include <GTL/Utility/Timer.h>
#include <fstream>
using namespace gtl;

namespace gtl
{
    class UnitTestRootsGeneralPolynomial
    {
    public:
        UnitTestRootsGeneralPolynomial();
    };
}

UnitTestRootsGeneralPolynomial::UnitTestRootsGeneralPolynomial()
{
    UTInformation("Mathematics/RootFinders/RootsGeneralPolynomial (tested by DistCircle3Circle3)");
}

#else

#include <GTL/Mathematics/RootFinders/RootsGeneralPolynomial.h>

namespace gtl
{
    using Rational = BSRational<UIntegerAP32>;
    template void RootsGeneralPolynomial<float>::Solve(std::vector<float> const&, bool, std::vector<float>&);
    template void RootsGeneralPolynomial<double>::Solve(std::vector<Rational> const&, bool, std::vector<Rational>&);
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(RootsGeneralPolynomial)
