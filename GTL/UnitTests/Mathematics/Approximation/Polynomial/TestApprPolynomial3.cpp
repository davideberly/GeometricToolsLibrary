#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/Polynomial/ApprPolynomial3.h>
#include <fstream>
using namespace gtl;

//#define INTERNAL_GENERATE_DATA

namespace gtl
{
    class UnitTestApprPolynomial3
    {
    public:
        UnitTestApprPolynomial3();

    private:
        void Test();
    };
}

UnitTestApprPolynomial3::UnitTestApprPolynomial3()
{
    UTInformation("Mathematics/Approximation/Polynomial/ApprPolynomial3");

    Test();
}

void UnitTestApprPolynomial3::Test()
{
    std::vector<std::array<double, 4>> observations(1024);
    std::ifstream inFile("Mathematics/Approximation/ND/Input/RandomUnitPoints4D_Double_1024.binary", std::ios::binary);
    UTAssert(inFile, "Failed to open input file.");
    inFile.read((char*)observations.data(), observations.size() * sizeof(observations[0]));
    inFile.close();
#if defined(INTERNAL_GENERATE_DATA)
    std::ofstream outFile("Mathematics/Approximation/Polynomial/Input/ApprPolynomial3Input.txt");
    for (auto const& p : observations)
    {
        outFile << std::setprecision(17) << p[0] << "," << p[1] << "," << p[2] << "," << p[3] << std::endl;
    }
    outFile.close();
#endif

    std::size_t constexpr xDegree = 3, yDegree = 2, zDegree = 1;
    Polynomial<double, 3> polynomial{};
    std::array<double, 2> xExtreme{}, yExtreme{}, zExtreme{}, wExtreme{};
    bool success = ApprPolynomial3<double>::Fit(xDegree, yDegree, zDegree, observations,
        polynomial, xExtreme, yExtreme, zExtreme, wExtreme);
    UTAssert(success, "The fit failed.");
    // coefficients of polynomial
    //   {1, x, x^2, x^3} [polynomial.mCoefficient[0].mCoefficient[0].mCoefficient[0..3]]
    //   {-0.014284589238581596, -0.26965168367314085, 0.017179112803374141, 0.26714751715803409}
    //   {y, y*x y*x^2, y*x^3} [polynomial.mCoefficient[0].mCoefficient[1].mCoefficient[0..3]]
    //   {-0.017659624992757965, 0.025837391113287506, -0.069583432491744895, 0.055370744899336161}
    //   {y^2, y^2*x, y^2*x^2, y^2*x^3} [polynomial.mCoefficient[0].mCoefficient[2].mCoefficient[0..3]]
    //   {0.070144835749038500, 0.64024994614447428, -0.16968056818489388, -0.58151760455370993}
    //   {z, z*x, z*x^2, z*x^3} [polynomial.mCoefficient[1].mCoefficient[0].mCoefficient[0..3]]
    //   {-0.039457187852054681, 0.11086782757404533, 0.16763135888251046, 0.053119980975631975}
    //   {z*y, z*y*x, z*y*x^2, z*y*x^3} [polynomial.mCoefficient[1].mCoefficient[1].mCoefficient[0..3]]
    //   {-0.13990048083846299, -0.14700160470157686, 0.35066334044067543, 0.11027116248641451}
    //   {z*y^2, z*y^2*x, z*y^2*x^2, z*y^2*x^3} [polynomial.mCoefficient[1].mCoefficient[2].mCoefficient[0..3]]
    //   {-0.0079261803145761148, -0.28109860505322548, 0.13176597647613700, -0.011491032406311705}

    // From Mathematica's "Fit" function
    // basis = {1, x, x^2, x^3, y, y*x y*x^2, y*x^3, y^2, y^2*x, y^2*x^2, y^2*x^3, 
    //     z, z*x, z*x^2, z*x^3, z*y, z*y*x, z*y*x^2, z*y*x^3, z*y^2, z*y^2*x, z*y^2*x^2, z*y^2*x^3}
    // Fit[SetPrecision[points, 17], basis, {x,y}, WorkingPrecision -> 64]
    Polynomial<double, 3> expectedPolynomial
    {
        {{-0.014284589238581833, -0.26965168367313349, 0.017179112803375081, 0.26714751715802264},
        {-0.017659624992757659, 0.025837391113289479, -0.06958343249174566, 0.05537074489933345},
        { 0.07014483574903946, 0.6402499461444537, -0.16968056818489662, -0.5815176045536783}},
        {{-0.03945718785205475, 0.11086782757404236, 0.16763135888251034, 0.05311998097563626},
        {-0.13990048083846388, -0.14700160470157331, 0.3506633404406768, 0.11027116248640820},
        {-0.007926180314575905, -0.28109860505322219, 0.13176597647613738, -0.011491032406316581}}
    };

    Polynomial<double, 3> diff = polynomial - expectedPolynomial;
    // coefficients of diff
    //   {2.3765711620882257e-16, -7.3829831137572910e-15, -9.4022012397942945e-16, 1.1435297153639112e-14}
    //   {-3.0531133177191805e-16, -1.9741153156616065e-15, 7.6327832942979512e-16, 2.7131075164277263e-15}
    //   {-9.5756735873919752e-16, 2.0539125955565396e-14, 2.7478019859472624e-15, -3.1641356201816961e-14}
    //   {6.9388939039072284e-17, 2.9698465908722937e-15, 1.1102230246251565e-16, -4.2882364326146671e-15}
    //   {8.8817841970012523e-16, -3.5527136788005009e-15, -1.3877787807814457e-15, 6.3143934525555778e-15}
    //   {-2.0990154059319366e-16, -3.2751579226442118e-15, -3.8857805861880479e-16, 4.8763076909708047e-15}
    double constexpr maxError = 1.0e-13;
    double error{};
    for (std::size_t s = 0; s <= zDegree; ++s)
    {
        for (std::size_t r = 0; r <= yDegree; ++r)
        {
            for (std::size_t c = 0; c <= xDegree; ++c)
            {
                error = std::fabs(diff[s][r][c]);
                UTAssert(error <= maxError, "Inaccurate result diff[" + std::to_string(s)
                    + "][" + std::to_string(r) + "][" + std::to_string(c) + "]");
            }
        }
    }

    std::array<double, 2> expectedXExtreme{ -0.99651681129409786, 0.99988315531559957 };
    std::array<double, 2> expectedYExtreme{ -0.99606591039127690, 0.99972259312224510 };
    std::array<double, 2> expectedZExtreme{ -0.99997571560185605, 0.99608215171706327 };
    std::array<double, 2> expectedWExtreme{ -0.99874379726483786, 0.99838581631689394 };

    for (std::size_t i = 0; i < 2; ++i)
    {
        error = std::fabs(xExtreme[i] - expectedXExtreme[i]);
        UTAssert(error <= maxError, "The x-extreme value is incorrect.");
        error = std::fabs(yExtreme[i] - expectedYExtreme[i]);
        UTAssert(error <= maxError, "The y-extreme value is incorrect.");
        error = std::fabs(zExtreme[i] - expectedZExtreme[i]);
        UTAssert(error <= maxError, "The z-extreme value is incorrect.");
        error = std::fabs(wExtreme[i] - expectedWExtreme[i]);
        UTAssert(error <= maxError, "The w-extreme value is incorrect.");
    }

    std::array<double, 3> x = { 0.0, 0.0, 0.0 };
    double w = polynomial(x.data());
    double expectedW = -0.014284589238581596;
    error = std::fabs(w - expectedW);
    UTAssert(error <= maxError, "The w-value is incorrect.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Approximation/Polynomial/ApprPolynomial3.h>

namespace gtl
{
    template class ApprPolynomial3<float>;
    template class ApprPolynomial3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ApprPolynomial3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprPolynomial3)
