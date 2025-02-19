#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/Polynomial/ApprPolynomial2.h>
#include <fstream>
using namespace gtl;

namespace gtl
{
    class UnitTestApprPolynomial2
    {
    public:
        UnitTestApprPolynomial2();

    private:
        void Test();
    };
}

UnitTestApprPolynomial2::UnitTestApprPolynomial2()
{
    UTInformation("Mathematics/Approximation/Polynomial/ApprPolynomial2");

    Test();
}

void UnitTestApprPolynomial2::Test()
{
    std::vector<std::array<double, 2>> observations(1024);
    std::ifstream input("Mathematics/Approximation/Input/RandomUnitPoints2D_Double_1024.binary", std::ios::binary);
    input.read((char*)observations.data(), observations.size() * sizeof(observations[0]));
    input.close();

    std::size_t degree = 3;
    Polynomial<double, 1> polynomial{};
    std::array<double, 2> xExtreme{}, wExtreme{};
    bool success = ApprPolynomial2<double>::Fit(degree, observations,
        polynomial, xExtreme, wExtreme);
    UTAssert(
        success,
        "The fit failed.");
    std::array<double, 2> expectedXExtreme{ -2.8077941722892099, 4.8079669163559924 };
    std::array<double, 2> expectedWExtreme{ -1.8078876442533560, 5.8078827166748379 };
    double error = std::fabs(xExtreme[0] - expectedXExtreme[0]);
    UTAssert(
        error <= 1e-15,
        "The xmin value is incorrect.");
    error = std::fabs(xExtreme[1] - expectedXExtreme[1]);
    UTAssert(
        error <= 1e-15,
        "The xmax value is incorrect.");
    error = std::fabs(wExtreme[0] - expectedWExtreme[0]);
    UTAssert(
        error <= 1e-15,
        "The wmin value is incorrect.");
    error = std::fabs(wExtreme[1] - expectedWExtreme[1]);
    UTAssert(
        error <= 1e-15,
        "The wmax value is incorrect.");

    double x = 0.0;
    double w = polynomial(x);
    double expectedW = 2.3417976564982710;
    error = std::fabs(w - expectedW);
    UTAssert(
        error <= 1e-15,
        "The evaluation failed.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Approximation/Polynomial/ApprPolynomial2.h>

namespace gtl
{
    template class ApprPolynomial2<float>;
    template class ApprPolynomial2<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ApprPolynomial2<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprPolynomial2)
