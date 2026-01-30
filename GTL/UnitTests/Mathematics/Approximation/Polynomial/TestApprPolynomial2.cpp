#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/Polynomial/ApprPolynomial2.h>
#include <fstream>
using namespace gtl;

//#define INTERNAL_GENERATE_DATA

namespace gtl
{
    class UnitTestApprPolynomial2
    {
    public:
        UnitTestApprPolynomial2();

    private:
        void TestSameDegree();
        void TestDifferentDegrees();
    };
}

UnitTestApprPolynomial2::UnitTestApprPolynomial2()
{
    UTInformation("Mathematics/Approximation/Polynomial/ApprPolynomial2");

    TestSameDegree();
    TestDifferentDegrees();
}

void UnitTestApprPolynomial2::TestSameDegree()
{
    std::vector<std::array<double, 3>> observations(1024);
    std::ifstream inFile("Mathematics/Approximation/3D/Input/RandomUnitPoints3D_Double_1024.binary", std::ios::binary);
    UTAssert(inFile, "Failed to open input file.");
    inFile.read((char*)observations.data(), observations.size() * sizeof(observations[0]));
    inFile.close();
#if defined(INTERNAL_GENERATE_DATA)
    std::ofstream outFile("Mathematics/Approximation/Polynomial/Input/ApprPolynomial2Input.txt");
    for (auto const& p : observations)
    {
        outFile << std::setprecision(17) << p[0] << "," << p[1] << "," << p[2] << std::endl;
    }
    outFile.close();
#endif

    std::size_t constexpr xDegree = 3, yDegree = 3;
    Polynomial<double, 2> polynomial{};
    bool success = ApprPolynomial2<double>::Fit(xDegree, yDegree, observations, polynomial);
    UTAssert(success, "The fit failed.");
    // coefficients of polynomial
    //    {1, y, y^2, y^3}
    //    {-0.036601514495418062, -0.1236171215319241400,  0.081462419748683748,  0.1400882814972888300},
    //    {x, x*y, x*y^2, x*y^3}
    //    { 0.010262828175322626,  0.0099693797919490068, -0.057945503187400153,  0.0025206172798712401},
    //    {x^2, x^2*y, x^2*y^2, x^2*y^3}
    //    { 0.029618145735493043,  0.4456248169916167700,  0.041707525455515607, -0.5489697567179854200},
    //    {x^3, x^3*y, x^3*y^2, x^3*y^3}
    //    {-0.061908945779295350, -0.5346376935903833800,  0.207103186261772490,  0.5396477671579489300}
    //};

    // From Mathematica's "Fit" function
    // basis = {1, y, y^2, y^3, x, x*y, x*y^2, x*y^3, x^2, x^2*y, x^2*y^2, x^2*y^3, x^3, x^3*y, x^3*y^2, x^3*y^3}.
    // Fit[SetPrecision[points, 17], basis, {x,y}, WorkingPrecision -> 64]
    Polynomial<double, 2> expectedPolynomial{
        {-0.036601514495418090, -0.1236171215319271200,  0.081462419748683550,  0.1400882814972936800},
        { 0.010262828175322223,  0.0099693797919746670, -0.057945503187398970,  0.0025206172798354416},
        { 0.029618145735493291,  0.4456248169916201000,  0.041707525455515640, -0.5489697567179909000},
        {-0.061908945779294740, -0.5346376935904212000,  0.207103186261771190,  0.5396477671580017000}
    };

    Polynomial<double, 2> diff = polynomial - expectedPolynomial;
    // coefficients of diff
    // { 2.7755575615628914e-17,  2.9837243786801082e-15,  1.9428902930940239e-16, -4.8572257327350599e-15}
    // { 4.0245584642661925e-16, -2.5660029656648931e-14, -1.1865508575681361e-15,  3.5798621011995380e-14}
    // {-2.4633073358870661e-16, -3.3306690738754696e-15, -3.4694469519536142e-17,  5.4400928206632670e-15}
    // {-6.1062266354383610e-16,  3.7858605139717838e-14,  1.3045120539345589e-15, -5.2735593669694936e-14}
    double constexpr maxError = 1.0e-13;
    double error{};
    for (std::size_t r = 0; r <= yDegree; ++r)
    {
        for (std::size_t c = 0; c <= xDegree; ++c)
        {
            error = std::fabs(diff[r][c]);
            UTAssert(error <= maxError, "Inaccurate result diff[" + std::to_string(r) + "][" + std::to_string(c) + "]");
        }
    }

    std::array<double, 2> x = { 0.0, 0.0 };
    double w = polynomial(x.data());
    double expectedW = -0.036601514495418062;
    error = std::fabs(w - expectedW);
    UTAssert(error <= maxError, "The w-value is incorrect.");
}

void UnitTestApprPolynomial2::TestDifferentDegrees()
{
    std::vector<std::array<double, 3>> observations(1024);
    std::ifstream inFile("Mathematics/Approximation/3D/Input/RandomUnitPoints3D_Double_1024.binary", std::ios::binary);
    UTAssert(inFile, "Failed to open input file.");
    inFile.read((char*)observations.data(), observations.size() * sizeof(observations[0]));
    inFile.close();
#if defined(INTERNAL_GENERATE_DATA)
    std::ofstream outFile("Mathematics/Approximation/Polynomial/Input/ApprPolynomial2Input.txt");
    for (auto const& p : observations)
    {
        outFile << std::setprecision(17) << p[0] << "," << p[1] << "," << p[2] << std::endl;
    }
    outFile.close();
#endif

    std::size_t constexpr xDegree = 3, yDegree = 1;
    Polynomial<double, 2> polynomial{};
    std::array<double, 2> xExtreme{}, yExtreme{}, wExtreme{};
    bool success = ApprPolynomial2<double>::Fit(xDegree, yDegree, observations, polynomial);
    UTAssert(success, "The fit failed.");
    // coefficients of polynomial
    //   {1, x, x^2, x^3} [polynomial.mCoefficient[0].mCoefficient[0..3]]
    //   {-0.027108499479626003, 0.020124308366052661, 0.095671538383850022, -0.032941084969050828}
    //   {y, y*x, y*x^2, y*x^3} [polynomial.mCoefficient[1].mCoefficient[0..3]]
    //   {-0.023285091002197961, -0.28264297391735627, 0.066689188592509746, 0.28836057459675163}

    // From Mathematica's "Fit" function
    // basis = {1, x, x^2, x^3, y, y*x, y*x^2, y*x^3}
    // Fit[SetPrecision[points, 17], basis, {x,y}, WorkingPrecision -> 64]
    Polynomial<double, 2> expectedPolynomial{
        {-0.027108499479625883, 0.020124308366053196, 0.09567153838384973, -0.032941084969051591},
        {-0.023285091002198019, -0.28264297391735397, 0.06668918859251004, 0.28836057459674822}
    };

    Polynomial<double, 2> diff = polynomial - expectedPolynomial;
    // coefficients of diff
    // { 2.7755575615628914e-17,  2.9837243786801082e-15, 4.0245584642661925e-16, -2.5660029656648931e-14}
    // {-2.4633073358870661e-16, -3.3306690738754696e-15, -6.1062266354383610e-16,  3.7858605139717838e-14}
    double constexpr maxError = 1.0e-13;
    double error{};
    for (std::size_t r = 0; r <= yDegree; ++r)
    {
        for (std::size_t c = 0; c <= xDegree; ++c)
        {
            error = std::fabs(diff[r][c]);
            UTAssert(error <= maxError, "Inaccurate result diff[" + std::to_string(r) + "][" + std::to_string(c) + "]");
        }
    }

    std::array<double, 2> expectedXExtreme{ -0.99874379726483786, 0.99988315531559957 };
    std::array<double, 2> expectedYExtreme{ -0.99680419189214831, 0.99972259312224510 };
    std::array<double, 2> expectedWExtreme{ -0.99697032390997875, 0.99608215171706327 };

    for (std::size_t i = 0; i < 2; ++i)
    {
        error = std::fabs(xExtreme[i] - expectedXExtreme[i]);
        UTAssert(error <= maxError, "The x-extreme value is incorrect.");
        error = std::fabs(yExtreme[i] - expectedYExtreme[i]);
        UTAssert(error <= maxError, "The y-extreme value is incorrect.");
        error = std::fabs(wExtreme[i] - expectedWExtreme[i]);
        UTAssert(error <= maxError, "The w-extreme value is incorrect.");
    }

    std::array<double, 2> x = { 0.0, 0.0 };
    double w = polynomial(x.data());
    double expectedW = -0.027108499479626003;
    error = std::fabs(w - expectedW);
    UTAssert(error <= maxError, "The w-value is incorrect.");
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
