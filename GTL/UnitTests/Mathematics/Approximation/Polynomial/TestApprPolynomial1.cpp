#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/Polynomial/ApprPolynomial1.h>
#include <fstream>
using namespace gtl;

//#define INTERNAL_GENERATE_DATA

namespace gtl
{
    class UnitTestApprPolynomial1
    {
    public:
        UnitTestApprPolynomial1();

    private:
        void Test();
    };
}

UnitTestApprPolynomial1::UnitTestApprPolynomial1()
{
    UTInformation("Mathematics/Approximation/Polynomial/ApprPolynomial1");

    Test();
}

void UnitTestApprPolynomial1::Test()
{
    std::vector<std::array<double, 2>> observations(1024);
    std::ifstream inFile("Mathematics/Approximation/2D/Input/RandomUnitPoints2D_Double_1024.binary", std::ios::binary);
    UTAssert(inFile, "Failed to open input file.");
    inFile.read((char*)observations.data(), observations.size() * sizeof(observations[0]));
    inFile.close();
#if defined(INTERNAL_GENERATE_DATA)
    std::ofstream outFile("Mathematics/Approximation/Polynomial/Input/ApprPolynomial1Input.txt");
    for (auto const& p : observations)
    {
        outFile << std::setprecision(17) << p[0] << "," << p[1] << std::endl;
    }
    outFile.close();
#endif

    std::size_t constexpr xDegree = 3;
    Polynomial<double, 1> polynomial{};
    bool success = ApprPolynomial1<double>::Fit(xDegree, observations, polynomial);
    UTAssert(success, "The fit failed.");
    // coefficients of polynomial
    // {1, x, x^2, x^3}
    // {2.3417976564982710, -0.76376353669192054, 0.033299763850360320, -0.0011838348907506763}

    // From Mathematica's "Fit" function
    // basis = {1,x,x^2,x^3}
    // Fit[SetPrecision[points, 17], basis, {x}, WorkingPrecision -> 64]
    Polynomial<double, 1> expectedPolynomial{
       2.3417976564982810,
      -0.7637635366919154,
       0.033299763850358672,
      -0.0011838348907507455
    };

    Polynomial<double, 1> diff = polynomial - expectedPolynomial;
    // coefficients of diff
    // -1.0214051826551440e-14
    // -5.1070259132757201e-15
    //  1.6445178552260131e-15
    //  6.9172098604575183e-17
    double constexpr maxError = 1.0e-13;
    double error{};
    for (std::size_t i = 0; i <= xDegree; ++i)
    {
        error = std::fabs(diff[i]);
        UTAssert(error <= maxError, "Inaccurate result diff[" + std::to_string(i) + "].");;
    }

    double x = 0.0;
    double w = polynomial(x);
    double expectedW = 2.3417976564982710;
    error = std::fabs(w - expectedW);
    UTAssert(error <= maxError, "The w-value is incorrect.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Approximation/Polynomial/ApprPolynomial1.h>

namespace gtl
{
    template class ApprPolynomial1<float>;
    template class ApprPolynomial1<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ApprPolynomial1<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprPolynomial1)
