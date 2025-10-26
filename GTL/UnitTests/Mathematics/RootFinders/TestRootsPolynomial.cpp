#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/RootFinders/RootsPolynomial.h>
#include <GTL/Utility/Timer.h>
#include <fstream>
using namespace gtl;

namespace gtl
{
    class UnitTestRootsPolynomial
    {
    public:
        UnitTestRootsPolynomial();

        void TestPascal();
        void TestConstant();
        void TestLinear();
        void TestRepeatedReal();
        void TestRandomRealLargeDegree();
        void TestPseudoExtendedGCD();
        void TestPseudoSquareFreeFactors();
        void TestRandomNonsimple();

    private:
        using BSN = BSNumber<UIntegerAP32>;
        using BSR = BSRational<UIntegerAP32>;

        std::size_t mMaxBisections;
        std::size_t mPrecision;
    };

    extern std::atomic<std::size_t> gsUIntegerAP32MaxBlocks;
}

UnitTestRootsPolynomial::UnitTestRootsPolynomial()
    :
    mMaxBisections(128),
    mPrecision(64)
{
    UTInformation("Mathematics/RootFinders/RootsPolynomial");

    // The test is by visual inspection of the mPascal[] member.
    TestPascal();

    TestConstant();
    TestLinear();
    TestRepeatedReal();
    TestRandomRealLargeDegree();
    TestPseudoExtendedGCD();
    TestPseudoSquareFreeFactors();
    TestRandomNonsimple();
}

void UnitTestRootsPolynomial::TestPascal()
{
    RootsPolynomial finder(mMaxBisections, mPrecision);
    finder.AddRowsToPascalsTriangle(17);
}

void UnitTestRootsPolynomial::TestConstant()
{
    RootsPolynomial finder(mMaxBisections, mPrecision);
    std::vector<BSN> roots;

    Polynomial1<float> p{ 0.0f };
    finder(p, roots, true);

    UTAssert(
        roots.size() == 0 && finder.GetRootStatus() == finder.zeroConstant,
        "Expecting zero-constant status.");

    auto const& rootIntervals = finder.GetRootIntervals();
    UTAssert(
        rootIntervals.size() == 0,
        "Expecting no root intervals.");

    p[0] = 1.0f;
    finder(p, roots);

    UTAssert(
        roots.size() == 0 && finder.GetRootStatus() == finder.nonzeroConstant,
        "Expecting zero-constant status.");

    UTAssert(
        rootIntervals.size() == 0,
        "Expecting no root intervals.");
}

void UnitTestRootsPolynomial::TestLinear()
{
    RootsPolynomial finder(mMaxBisections, mPrecision);
    std::vector<BSN> roots;

    Polynomial1<float> p{ 4.0f, 3.0f };
    finder(p, roots, true);

    UTAssert(
        roots.size() == 1 && finder.GetRootStatus() == finder.unknownRootStatus,
        "Incorrect number of roots.");

    double error = std::fabs((-4.0 / 3.0) - (double)roots[0]);
    UTAssert(
        error == 0.0,
        "Incorrect root value.");

    auto const& rootIntervals = finder.GetRootIntervals();
    UTAssert(
        rootIntervals.size() == 1,
        "Incorrect number of root intervals.");

    bool contained = (rootIntervals[0][0] <= roots[0] && roots[0] <= rootIntervals[0][1]);
    UTAssert(
        contained,
        "Expecting root to be in the root interval.");
}

void UnitTestRootsPolynomial::TestRepeatedReal()
{
    RootsPolynomial finder(mMaxBisections, mPrecision);
    std::vector<BSN> roots;

    // The polynomial has root -4/3 of multiplicity 2 and a root 5/7 of
    // multiplicity 1.
    Polynomial1<float> poly0{ 4.0f, 3.0f };
    Polynomial1<float> poly1{ -5.0f, 7.0f };
    Polynomial1<float> p = poly0 * poly0 * poly1;

    // Numerical rounding errors lead to non-detection of root -4/3.
    finder(p, roots, false);

    UTAssert(
        roots.size() == 1 && finder.GetRootStatus() == finder.unknownRootStatus,
        "Expecting to find only the simple root.");

    double error = std::fabs(5.0 / 7.0 - (double)roots[0]);
    UTAssert(
        error == 0.0,
        "Incorrect root value.");

    auto const& rootIntervals = finder.GetRootIntervals();
    UTAssert(
        rootIntervals.size() == 0,
        "Root intervals were not requested.");

    // Exact arithmetic leads to detection of -4/3 and 5/7.
    Polynomial1<BSN> rp{ p[0], p[1], p[2], p[3] };
    finder(rp, roots, true);

    UTAssert(
        roots.size() == 2 && finder.GetRootStatus() == finder.hasRepeatedRoots,
        "Expecting to find both roots.");

    error = std::fabs(-4.0 / 3.0 - (double)roots[0]);
    UTAssert(
        error == 0.0,
        "Incorrect root value.");
    error = std::fabs(5.0 / 7.0 - (double)roots[1]);
    UTAssert(
        error == 0.0,
        "Incorrect root value.");

    UTAssert(
        rootIntervals.size() == 2,
        "Incorrect number of root intervals.");

    bool contained = (rootIntervals[0][0] <= roots[0] && roots[0] <= rootIntervals[0][1]);
    UTAssert(
        contained,
        "Expecting root to be in the root interval.");

    contained = (rootIntervals[1][0] <= roots[1] && roots[1] <= rootIntervals[1][1]);
    UTAssert(
        contained,
        "Expecting root to be in the root interval.");
}

void UnitTestRootsPolynomial::TestRandomRealLargeDegree()
{
    std::size_t const degree = 33;
    Polynomial1<float> p(degree);
    std::ifstream inFile("Mathematics/RootFinders/Input/RandomRealLargeDegree.binary", std::ios::binary);
    UTAssert(inFile, "Failed to open input file.");
    inFile.read((char*)p.GetCoefficients().data(),
        p.GetCoefficients().size() * sizeof(float));
    inFile.close();

    RootsPolynomial finder(mMaxBisections, mPrecision);
    std::vector<BSN> roots{};
    finder(p, roots, true);
    UTAssert(
        roots.size() == 6 && finder.GetRootStatus() == finder.hasRepeatedRoots,
        "Expecting 6 roots, root 0 is nonsimple (multiplicity 2).");

    std::vector<double> computedRoots =
    {
        (double)roots[0],
        (double)roots[1],
        (double)roots[2],
        (double)roots[3],
        (double)roots[4],
        (double)roots[5]
    };

    std::vector<double> expectedRoots =
    {
        -0.98483003931491264,
        -0.77823652794774534,
        -0.59512492336817469,
        0.0,
        0.82755556327703006,
        1.1454895060232844
    };

    auto const& rootIntervals = finder.GetRootIntervals();
    UTAssert(
        rootIntervals.size() == 6,
        "Expecting 6 root intervals.");

    Polynomial1<BSN> rp(p.GetDegree());
    for (std::size_t i = 0; i <= p.GetDegree(); ++i)
    {
        rp[i] = p[i];
    }

    double error, pValue;
    BSN rpValue;
    for (std::size_t i = 0; i < roots.size(); ++i)
    {
        error = std::fabs(expectedRoots[i] - (double)roots[i]);
        UTAssert(
            error == 0.0,
            "Incorrect root value.");

        bool contained = (rootIntervals[i][0] <= roots[i] && roots[i] <= rootIntervals[i][1]);
        UTAssert(
            contained,
            "Expecting root to be in the root interval.");

        pValue = (double)p((float)roots[i]);
        // 0.0
        // -4.3357468371141294e-07
        // -1.8049837180456052e-08
        // 0.0
        // -8.1640266103022441e-08
        // 4.4814332795795053e-05
        UTAssert(
            std::fabs(pValue) <= 4.5e-5,
            "Expecting zero-valued polynomial (within rounding errors).");

        rpValue = rp(roots[i]);
        pValue = rpValue;
        // 0.0
        // -2.5210731924887027e-19
        // 1.1806629653528413e-20
        // -5.8320986986193833e-21
        // 2.6272221759616945e-20
        // 3.5111175405137769e-17
        UTAssert(
            std::fabs(pValue) <= 4.0e-15f,
            "Expecting zero-valued polynomial (within rounding errors).");
    }
}

void UnitTestRootsPolynomial::TestPseudoExtendedGCD()
{
    Polynomial1<float> poly0{ 4.0f, 3.0f };
    Polynomial1<float> poly1{ -5.0f, 7.0f };
    Polynomial1<float> p = poly0 * poly0 * poly1;

    Polynomial1<BSN> p0(p.GetDegree());
    for (std::size_t i = 0; i <= p.GetDegree(); ++i)
    {
        p0[i] = p[i];
    }

    Polynomial1<BSN> p1 = GetDerivative(p0);
    BSN a0{}, a1{};
    Polynomial1<BSN> g, q0, q1, test;
    GetPseudoExtendedGCD(p0, p1, g, a0, a1, q0, q1);

    // p0 = (4 + 3 * x)^2 * (-5 + 7 * x) = -80 - 8 * x + 123 * x^2 + 63 * x^3
    // p1 = -8 + 246 * x + 189 * x^2 = (4 + 3 * x) * (-2 + 63 * x)
    // g  = -2795688 - 2096766 * x = -698922 * (4 + 3 * x)
    // a0 = -9.2182800363391826e+18
    // a1 = 4396427658756.0000
    // q0 = -263785659525360.00 + 171460678691484.00 * x + 276974942501628.00 * x^2
    // q1 = 12580596.000000000 - 396288774.00000000 * x
    // a0 * p(x) = q0(x) * g(x)
    // a1 * p'(x) = q1(x) * g(x)

    UTAssert(
        p0.GetDegree() == 3 &&
        p0[0] == BSN(-80) &&
        p0[1] == BSN(-8) &&
        p0[2] == BSN(123) &&
        p0[3] == BSN(63),
        "Incorrect p0.");

    UTAssert(
        p1.GetDegree() == 2 &&
        p1[0] == BSN(-8) &&
        p1[1] == BSN(246) &&
        p1[2] == BSN(189),
        "Incorrect p1.");

    UTAssert(
        g.GetDegree() == 1 &&
        g[0] == BSN(-2795688) &&
        g[1] == BSN(-2096766),
        "Incorrect GCD.");

    // The debugger value for a0 is a rounded result of the rational a0.
    UTAssert(
        a0.GetSign() == -1 &&
        a0.GetBiasedExponent() == 3 &&
        a0.GetUInteger().GetNumBits() == 60 &&
        a0.GetUInteger().GetBits()[0] == 0x47c24dbf &&
        a0.GetUInteger().GetBits()[1] == 0x0ffdbd1b &&
        a1 == BSN(4396427658756),
        "Incorrect a0 or a1.");

    UTAssert(
        q0.GetDegree() == 2 &&
        q0[0] == BSN(-263785659525360) &&
        q0[1] == BSN(171460678691484) &&
        q0[2] == BSN(276974942501628),
        "Incorrect q0.");

    UTAssert(
        q1.GetDegree() == 1 &&
        q1[0] == BSN(12580596) &&
        q1[1] == BSN(-396288774),
        "Incorrect q1.");

    test = a0 * p0 - q0 * g;  // = 0
    UTAssert(
        IsZero(test),
        "Incorrect extended GCD.");

    test = a1 * p1 - q1 * g;  // = 0
    UTAssert(
        IsZero(test),
        "Incorrect extended GCD.");
}

void UnitTestRootsPolynomial::TestPseudoSquareFreeFactors()
{
    Polynomial1<float> poly0{ 4.0f, 3.0f };
    Polynomial1<float> poly1{ -5.0f, 7.0f };
    Polynomial1<float> poly2{ 1.0f, 1.0f, 2.0f };
    Polynomial1<float> p = poly0 * poly0 * poly1 * poly1 * poly1 * poly2;

    Polynomial1<BSN> p0(p.GetDegree());
    for (std::size_t i = 0; i <= p.GetDegree(); ++i)
    {
        p0[i] = p[i];
    }

    p0 = p0 * p0;
    p0 = p0 * p0;

    // p0 = (1 + x + 2 * x^2)^4 * (4 + 3 * x)^8 * (-5 + 7 * x)^{12}

    std::vector<Polynomial1<BSN>> factors;
    Timer timer;
    GetPseudoSquareFreeFactors(p0, factors);
    auto seconds = timer.GetSeconds();
    std::cout << "time = " << seconds << " seconds" << std::endl;
    // time = 1.881 seconds (Release, no debugger attached)

    UTAssert(
        factors.size() == 13,
        "Expecting 13 factors.");

    for (std::size_t i = 0; i < factors.size(); ++i)
    {
        Polynomial1<BSR> test(factors[i].GetDegree());
        BSN const& leading = factors[i][test.GetDegree()];
        for (std::size_t j = 0; j < test.GetDegree(); ++j)
        {
            test[j] = BSR(factors[i][j], leading);
        }
        test[test.GetDegree()] = BSR(1);

        std::cout << std::endl;
        std::cout << "factor " << i << std::endl;
        for (std::size_t j = 0; j <= test.GetDegree(); ++j)
        {
            std::cout << "f[" << j << "] = " << (double)test[j] << std::endl;
        }

        if (i != 4 && i != 8 && i != 12)
        {
            UTAssert(
                factors[i].GetDegree() == 0 && factors[i][0] == BSN(1),
                "Expecting the factor to be the constant-1 polynomial.");
        }
        else if (i == 4)
        {
            UTAssert(
                factors[i].GetDegree() == 2,
                "Expecting the factor to be degree 2.");

            BSR ratio0(factors[i][0], factors[i][2]);
            UTAssert(
                (double)ratio0 == 0.5,
                "Unexpected coefficient.");

            BSR ratio1(factors[i][1], factors[i][2]);
            UTAssert(
                (double)ratio1 == 0.5,
                "Unexpected coefficient.");
        }
        else if (i == 8)
        {
            UTAssert(
                factors[i].GetDegree() == 1,
                "Expecting the factor to be degree 1.");

            BSR ratio0(factors[i][0], factors[i][1]);
            UTAssert(
                ratio0 == BSR(4, 3),
                "Unexpected coefficient.");
        }
        else if (i == 12)
        {
            UTAssert(
                factors[i].GetDegree() == 1,
                "Expecting the factor to be degree 1.");

            BSR ratio0(factors[i][0], factors[i][1]);
            UTAssert(
                ratio0 == BSR(-5, 7),
                "Unexpected coefficient.");
        }
    }
}

void UnitTestRootsPolynomial::TestRandomNonsimple()
{
    RootsPolynomial finder(mMaxBisections, mPrecision);
    std::vector<BSN> roots{};
    double root{}, expectedRoot{};
    double const maxError = 1e-15;

    // poly[0] = {0.629447341, -0.729045987}
    //   roots = {0.86338496072636883}
    // poly[1] = {0.811583877, 0.670017123, -0.746026397}
    //   roots = {-0.68651659818395772, 1.5846310967324841}
    // poly[2] = {0.937735558, 0.826751709, -0.557931900, 0.264718533}
    //   roots = {-0.69740448443985226}

    // The polynomial is (linear)*(quadratic)^2*(cubic)^3, which has
    // degree 14.
    std::size_t constexpr numFactors = 3;
    std::vector<Polynomial1<BSN>> poly(numFactors);
    Polynomial1<BSN> p{ 1 };

    poly[0].SetDegree(1);
    poly[0][0] = 0.629447341f;
    poly[0][1] = -0.729045987f;
    finder(poly[0], roots);
    UTAssert(roots.size() == 1,
        "Incorrect number of roots for poly[0].");
    root = (double)roots[0];
    expectedRoot = 0.86338496072636883;
    UTAssert(std::fabs(root - expectedRoot) <= maxError,
        "Incorrect root[0] for poly[0].");
    std::cout << "poly[0] root[0] = " << root << std::endl;
    p *= poly[0];

    poly[1].SetDegree(2);
    poly[1][0] = 0.811583877f;
    poly[1][1] = 0.670017123f;
    poly[1][2] = -0.746026397f;
    finder(poly[1], roots);
    UTAssert(roots.size() == 2,
        "Incorrect number of roots for poly[1].");
    root = (double)roots[0];
    expectedRoot = -0.68651659818395772;
    UTAssert(std::fabs(root - expectedRoot) <= maxError,
        "Incorrect root[0] for poly[1].");
    std::cout << "poly[1] root[0] = " << root << std::endl;
    root = (double)roots[1];
    expectedRoot = 1.5846310967324841;
    UTAssert(std::fabs(root - expectedRoot) <= maxError,
        "Incorrect root[1] for poly[1].");
    std::cout << "poly[1] root[1] = " << root << std::endl;
    p *= poly[1] * poly[1];

    poly[2].SetDegree(3);
    poly[2][0] = 0.937735558f;
    poly[2][1] = 0.826751709f;
    poly[2][2] = -0.557931900f;
    poly[2][3] = 0.264718533f;
    finder(poly[2], roots);
    UTAssert(roots.size() == 1,
        "Incorrect number of roots for poly[2].");
    root = (double)roots[0];
    expectedRoot = -0.69740448443985226;
    UTAssert(std::fabs(root - expectedRoot) <= maxError,
        "Incorrect root[0] for poly[2].");
    std::cout << "poly[2] root[0] = " << root << std::endl;
    p *= poly[2] * poly[2] * poly[2];

    // The coefficients of p are written to a text file to be read by
    // Mathematica for experiments and verification.
    Polynomial1<double> temp(p.GetDegree());
    std::ofstream outFile("Mathematics/RootFinders/Output/Coefficients.txt");
    UTAssert(outFile, "Failed to open output file.");
    for (std::size_t i = 0; i <= p.GetDegree(); ++i)
    {
        temp[i] = (double)p[i];
        outFile << temp[i] << std::endl;
    }
    outFile.close();

    Timer timer{};
    std::int64_t msecs{};

    // Compute the roots using double-precision coefficients.
    finder(temp, roots);
    msecs = timer.GetMicroseconds();
    std::cout << std::endl;
    std::cout << "microseconds = " << msecs << std::endl;
    std::cout << "max blocks = " << gsUIntegerAP32MaxBlocks << std::endl;
    std::cout << "root status = " << finder.GetRootStatus() << std::endl;
    for (std::size_t i = 0; i < roots.size(); ++i)
    {
        BSN pAtRoot = p(roots[i]);
        std::cout << "root[" << i << "] = " << (double)roots[i]
            << ", value = " << (double)pAtRoot << std::endl;
    }
    // microseconds = 14134
    // max blocks = 30
    // root status = unknownRootStatus
    // root[0] = -0.697432, p(root[0]) = -6.73852e-17
    // root[1] =  0.863385, p(root[1]) =  1.27356e-16

    // The Mathematica experiment is found in
    // UnitTests/Mathematics/NumericalMethods/RootFinders/TestRandomNonsimple.nb
    // The notebook reads in Coefficients.txt. NSolve is used to compute the
    // polynomial roots (using Coefficients.txt numbers). The output to 6
    // decimal places is
    //
    // root[0] = -0.664240, p(root[0]) = -5.55112e-17
    // root[1] =  0.863386, p(root[1]) = -1.11022e-16
    //
    // The root[0] values between the unit test and Mathematica differ in the
    // second decimal place. The next experiment uses rational p(x) rather
    // than double-precision temp[]. This shows that Mathematica is not doing
    // what I am doing regarding arbitrary-precision arithmetic for computing
    // the roots.

    timer.Reset();
    finder(p, roots);
    msecs = timer.GetMicroseconds();
    std::cout << std::endl;
    std::cout << "microseconds = " << msecs << std::endl;
    std::cout << "max blocks = " << gsUIntegerAP32MaxBlocks << std::endl;
    std::cout << "root status = " << finder.GetRootStatus() << std::endl;
    for (std::size_t i = 0; i < roots.size(); ++i)
    {
        BSN pAtRoot = p(roots[i]);
        std::cout << "root[" << i << "] = " << (double)roots[i]
            << ", value = " << (double)pAtRoot << std::endl;
    }
    // microseconds = 205058
    // max blocks = 2756
    // root status = hasRepeatedRoots
    // root[0] = -0.697404, p(root[0]) =  3.96203e-68
    // root[1] = -0.686517, p(root[1]) =  4.66572e-48
    // root[2] =  0.863385, p(root[2]) = -7.63715e-20
    // root[3] =  1.58463,  p(root[3]) = -1.58575e-38

    for (std::size_t i = 0; i <= p.GetDegree(); ++i)
    {
        // temp[i] is original p[i] rounded to double-precision. Now
        // use the rounded number as "truth".
        p[i] = temp[i];
    }
    timer.Reset();
    finder(p, roots);
    msecs = timer.GetMicroseconds();
    std::cout << std::endl;
    std::cout << "microseconds = " << msecs << std::endl;
    std::cout << "max blocks = " << gsUIntegerAP32MaxBlocks << std::endl;
    std::cout << "root status = " << finder.GetRootStatus() << std::endl;
    for (std::size_t i = 0; i < roots.size(); ++i)
    {
        BSN pAtRoot = p(roots[i]);
        std::cout << "root[" << i << "] = " << (double)roots[i]
            << ", value = " << (double)pAtRoot << std::endl;
    }
    // microseconds = 129170144
    // max blocks = 365512
    // root status = hasOnlySimpleRoots
    // root[0] = -0.697432, p(root[0]) = -1.2548e-31
    // root[1] =  0.863385, p(root[1]) = -3.6534e-20
    //
    // The compute time and maximum blocks are a result of rounding errors
    // causing p to have all simple roots, and the GetPseudoSquareFreeFactors
    // computations have significant cost.
    //
    // The root[0] value from finder(temp, roots) is -0.697432 and from
    // finder(p, roots) is -0.697432. The rational evaluation of p(root[0])
    // is -6.73852e-17 from finder(temp, roots) but is -1.2548e-31 from
    // p(root[0]).
    //
    // The Mathematica graph of temp(x) is very flat near root[0], so an
    // algorithm mixing rational and floating-point arithmetic might very
    // well choose a root relatively far from -0.697432.
}

// Here is a possible variation for computing roots and multiplicities.
// However, it requires a fast integer multiplication.
// 
// p(x) = g(x) * q0(x) / m0
// p'(x) = g(x) * q1(x) / m1
// q0(x) = 0 has simple roots
// g(x) = 0 has nonsimple roots (multiplicity > 1)
// g(x) = 0 and q0(x) = 0 have some common roots
// h(x)  = 0 are the common roots of g(x) = 0 and q0(x) = 0
// q0(x) = h(x) * w0(x) / n0
// g(x)  = h(x) * w1(x) / n1
// p(x)  = g(x) * q0(x) / m0
//       = g(x) * h(x) * w0(x) / (n0 * m0)
// But since the roots of h(x) = 0 are already a subset of the
// roots of g(x) = 0, we can compute the simple roots of p(x) = 0
// by solving w0(x) = 0 and the nonsimple roots of p(x) = 0 by
// solving g(x) = 0.

#else

#include <GTL/Mathematics/RootFinders/RootsPolynomial.h>

namespace gtl
{
    using BSN = BSNumber<UIntegerAP32>;
    template void RootsPolynomial::operator()(Polynomial1<float> const&, std::vector<BSN>&, bool);
    template void RootsPolynomial::operator()(Polynomial1<double> const&, std::vector<BSN>&, bool);
    template void RootsPolynomial::operator()(Polynomial1<BSN> const&, std::vector<BSN>&, bool);
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(RootsPolynomial)
