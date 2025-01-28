#if defined(GTL_UNIT_TESTS)

#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/RootFinders/RootsCubic.h>
#include <GTL/Mathematics/Algebra/Polynomial.h>
using namespace gtl;

namespace gtl
{
    class UnitTestRootsCubic
    {
    public:
        UnitTestRootsCubic();

    private:
        using Rational = BSRational<UIntegerAP32>;

        template <std::size_t Degree, std::size_t NumCoefficients>
        void Solve(bool, double const*, std::size_t, PolynomialRoot<double>*, PolynomialRoot<Rational>*) {}

        template <std::size_t Degree>
        double DEvaluate(double const*, double const&) {}

        template <std::size_t Degree>
        Rational REvaluate(double const*, Rational const&) {}

        template<> void Solve<3, 4>(bool useBisection, double const* p, std::size_t numExpectedRoots,
            PolynomialRoot<double>* dRoots, PolynomialRoot<Rational>* rRoots)
        {
            std::size_t numRoots = RootsCubic<double>::Solve(useBisection, p[0], p[1], p[2], p[3], dRoots);
            UTAssert(numRoots == numExpectedRoots, "Invalid number of roots (double).");
            numRoots = RootsCubic<Rational>::Solve(useBisection, p[0], p[1], p[2], p[3], rRoots);
            UTAssert(numRoots == numExpectedRoots, "Invalid number of roots (Rational).");
        }

        template<> void Solve<3, 3>(bool useBisection, double const* p, std::size_t numExpectedRoots,
            PolynomialRoot<double>* dRoots, PolynomialRoot<Rational>* rRoots)
        {
            std::size_t numRoots = RootsCubic<double>::Solve(useBisection, p[0], p[1], p[2], dRoots);
            UTAssert(numRoots == numExpectedRoots, "Invalid number of roots (double).");
            numRoots = RootsCubic<Rational>::Solve(useBisection, p[0], p[1], p[2], rRoots);
            UTAssert(numRoots == numExpectedRoots, "Invalid number of roots (Rational).");
        }

        template<> void Solve<3, 2>(bool useBisection, double const* p, std::size_t numExpectedRoots,
            PolynomialRoot<double>* dRoots, PolynomialRoot<Rational>* rRoots)
        {
            std::size_t numRoots = RootsCubic<double>::Solve(useBisection, p[0], p[1], dRoots);
            UTAssert(numRoots == numExpectedRoots, "Invalid number of roots (double).");
            numRoots = RootsCubic<Rational>::Solve(useBisection, p[0], p[1], rRoots);
            UTAssert(numRoots == numExpectedRoots, "Invalid number of roots (Rational).");
        }

        template<> double DEvaluate<3>(double const* p, double const& x)
        {
            return std::fma(x, std::fma(x, std::fma(x, p[3], p[2]), p[1]), p[0]);
        }

        template<> Rational REvaluate<3>(double const* p, Rational const& rX)
        {
            Rational rP0(p[0]), rP1(p[1]), rP2(p[2]), rP3(p[3]);
            return rP0 + rX * (rP1 + rX * (rP2 + rX * rP3));
        }

        template <std::size_t Degree, std::size_t NumCoefficients>
        void SingleTest(bool useBisection, double dMaxXError, double dMaxFError,
            std::array<double, Degree + 1> const& p,
            std::size_t numExpectedRoots,
            std::array<PolynomialRoot<double>, Degree> const& dXExpecteds,
            std::array<PolynomialRoot<double>, Degree>& dRoots,
            std::array<double, Degree>& dFValues,
            std::array<PolynomialRoot<Rational>, Degree>& rRoots,
            std::array<Rational, Degree>& rFValues)
        {
            Rational rMaxXError(dMaxXError);
            Rational rMaxFError(dMaxFError);
            dRoots.fill(PolynomialRoot<double>{});
            dFValues.fill(0.0);
            rRoots.fill(PolynomialRoot<Rational>{});
            rFValues.fill(Rational(0));

            Solve<Degree, NumCoefficients>(useBisection, p.data(),
                numExpectedRoots, dRoots.data(), rRoots.data());

            for (std::size_t i = 0; i < numExpectedRoots; ++i)
            {
                std::string label = "[" + std::to_string(i) + "].";

                double dXError = std::fabs(dRoots[i].x - dXExpecteds[i].x);
                UTAssert(dXError <= dMaxXError,
                    "Invalid x-error (double) " + label);

                std::size_t multiplicity = dXExpecteds[i].m;
                UTAssert(dRoots[i].m == multiplicity,
                    "Invalid multiplicity (double) " + label);

                dFValues[i] = DEvaluate<Degree>(p.data(), dRoots[i].x);
                UTAssert(std::fabs(dFValues[i]) <= dMaxFError,
                    "Invalid f-error (double) " + label);

                Rational rXExpected(dXExpecteds[i].x);
                Rational rXError = std::fabs(rRoots[i].x - rXExpected);
                UTAssert(rXError <= rMaxXError,
                    "Invalid x-error (Rational) " + label);

                UTAssert(rRoots[i].m == multiplicity,
                    "Invalid multiplicity (Rational) " + label);

                rFValues[i] = REvaluate<Degree>(p.data(), rRoots[i].x);
                UTAssert(std::fabs(rFValues[i]) <= rMaxFError,
                    "Invalid f-error (Rational) " + label);
            }
        }

        void TestCubicSolveGeneralBisection();
        void TestCubicSolveMonicBisection();
        void TestCubicSolveDepressedBisection();
        void TestCubicSolveGeneralClosedForm();
        void TestCubicSolveMonicClosedForm();
        void TestCubicSolveDepressedClosedForm();
    };
}

UnitTestRootsCubic::UnitTestRootsCubic()
{
    UTInformation("Mathematics/RootFinders/RootsCubic");

    TestCubicSolveGeneralBisection();
    TestCubicSolveMonicBisection();
    TestCubicSolveDepressedBisection();
    TestCubicSolveGeneralClosedForm();
    TestCubicSolveMonicClosedForm();
    TestCubicSolveDepressedClosedForm();
}

void UnitTestRootsCubic::TestCubicSolveGeneralBisection()
{
    bool useBisection = true;
    double const dMaxXError = 1e-15;
    double const dMaxFError = 1e-12;
    std::array<double, 4> p{};
    std::size_t numRoots{};
    std::array<PolynomialRoot<double>, 3> dXExpecteds{};
    std::array<PolynomialRoot<double>, 3> dRoots{};
    std::array<double, 3> dFValues{};
    std::array<PolynomialRoot<Rational>, 3> rRoots{};
    std::array<Rational, 3> rFValues{};

    // c0 = 0, c1 = 0
    p[0] = -1.0 / 256.0;
    p[1] = 3.0 / 32.0;
    p[2] = -3.0 / 4.0;
    p[3] = 2.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.125, 3 };
    SingleTest<3, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0.125, df0 = 0.0, m0 = 3
    // rx0 = 0.125, rf0 = 0.0, m0 = 3

    // c0 = 0, c1 < 0
    p[0] = -2.0 * (63.0 / 64.0);
    p[1] = -2.0 * (2.0 + 63.0 / 64.0);
    p[2] = -6.0;
    p[3] = -2.0;
    numRoots = 3;
    dXExpecteds[0] = { -1.125, 1 };
    dXExpecteds[1] = { -1.0, 1 };
    dXExpecteds[2] = { -0.875, 1 };
    SingleTest<3, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.125, rf0 = 0, m0 = 1
    // dx1 = -1.0, df1 = 0, m1 = 1
    // dx2 = -0.875, df2 = 0, m2 = 1
    // rx0 = -1.125, rf0 = 0, m0 = 1
    // rx1 = -1.0, rf1 = 0, m1 = 1
    // rx2 = -0.875, rf2 = 0, m2 = 1

    // c0 = 0, c1 > 0
    p[0] = -2.0 * (65.0 / 64.0);
    p[1] = -2.0 * (2.0 + 65.0 / 64.0);
    p[2] = -6.0;
    p[3] = -2.0;
    numRoots = 1;
    dXExpecteds[0] = { -1.0, 1 };
    SingleTest<3, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1, df0 = 0, m0 = 1
    // rx0 = -1, rf0 = 0, m0 = 1

    // c0 > 0, c1 = 0
    double a = 0.0625;
    double b = 0.875;
    double mult = std::ldexp(3.0, 10);
    p[0] = mult * (a * a * a + b);
    p[1] = mult * (3.0 * a * a);
    p[2] = mult * (3.0 * a);
    p[3] = mult;
    numRoots = 1;
    dXExpecteds[0] = { -1.0189655913861946, 1 };
    SingleTest<3, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.0189655913861946, df0 = -3.0875973647339034e-15, m0 = 1
    // rx0 = -1.0189655913861946, rf0 = 2.1861216841217654e-13, m0 = 1

    // c0 < 0, c1 = 0
    p[0] = 2.0 * (a * a * a - b);
    p[1] = 2.0 * (3.0 * a * a);
    p[2] = 2.0 * (3.0 * a);
    p[3] = 2.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.89396559138619458, 1 };
    SingleTest<3, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0.89396559138619458, df0 = 2.0364093718712812e-16, m0 = 1
    // rx0 = 0.89396559138619458, rf0 = -1.4232563047667745e-16, m0 = 1

    // c0 != 0, c1 != 0, sign(delta) > 0
    p[0] = 6.0;
    p[1] = 0.0;
    p[2] = -6.0;
    p[3] = 2.0;
    numRoots = 3;
    dXExpecteds[0] = { -0.87938524157181674, 1 };
    dXExpecteds[1] = { 1.3472963553338606, 1 };
    dXExpecteds[2] = { 2.5320888862379562, 1 };
    SingleTest<3, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.87938524157181674, df0 = 4.7190119747269912e-16, m0 = 1
    // dx1 = 1.3472963553338606, df1 = 4.1300891138986383e-16, m1 = 1
    // dx2 = 2.5320888862379562, df2 = 1.5861358196701874e-15, m2 = 1
    // rx0 = -0.87938524157181674, rf0 = 3.5429651279471601e-16, m0 = 1
    // rx1 = 1.3472963553338606, rf1 = 3.9965890211362694e-17, m1 = 1
    // rx2 = 2.5320888862379562, rf2 = 5.4582380493884678e-16, m2 = 1

    // c0 != 0, c1 != 0, sign(delta) < 0
    p[0] = -2.0;
    p[1] = 8.0;
    p[2] = -6.0;
    p[3] = 2.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.31767219617198067, 1 };
    SingleTest<3, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0.31767219617198067, df0 = -1.9846369836984043e-17, m0 = 1
    // rx0 = 0.31767219617198067, rf0 = -1.3946171710388160e-17, m0 = 1

    // c0 != 0, c1 != 0, sign(delta) = 0
    p[0] = 8.0;
    p[1] = 0.0;
    p[2] = -6.0;
    p[3] = 2.0;
    numRoots = 2;
    dXExpecteds[0] = { -1.0, 1 };
    dXExpecteds[1] = { 2.0, 2 };
    SingleTest<3, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1, df0 = 0, m0 = 1
    // dx1 = 2, df1 = 0, m1 = 2
    // rx0 = -1, rf0 = 0, m0 = 1
    // rx1 = 2, rf1 = 0, m1 = 2
}

void UnitTestRootsCubic::TestCubicSolveMonicBisection()
{
    bool useBisection = true;
    double const dMaxXError = 1e-15;
    double const dMaxFError = 1e-12;
    std::array<double, 4> p{};
    std::size_t numRoots{};
    std::array<PolynomialRoot<double>, 3> dXExpecteds{};
    std::array<PolynomialRoot<double>, 3> dRoots{};
    std::array<double, 3> dFValues{};
    std::array<PolynomialRoot<Rational>, 3> rRoots{};
    std::array<Rational, 3> rFValues{};

    // c0 = 0, c1 = 0
    p[0] = -1.0 / 512.0;
    p[1] = 3.0 / 64.0;
    p[2] = -3.0 / 8.0;
    p[3] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.125, 3 };
    SingleTest<3, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0.125, df0 = 0.0, m0 = 3
    // rx0 = 0.125, rf0 = 0.0, m0 = 3

    // c0 = 0, c1 < 0
    p[0] = 63.0 / 64.0;
    p[1] = 2.0 + 63.0 / 64.0;
    p[2] = 3.0;
    p[3] = 1.0;
    numRoots = 3;
    dXExpecteds[0] = { -1.125, 1 };
    dXExpecteds[1] = { -1.0, 1 };
    dXExpecteds[2] = { -0.875, 1 };
    SingleTest<3, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.125, rf0 = 0, m0 = 1
    // dx1 = -1.0, df1 = 0, m1 = 1
    // dx2 = -0.875, df2 = 0, m2 = 1
    // rx0 = -1.125, rf0 = 0, m0 = 1
    // rx1 = -1.0, rf1 = 0, m1 = 1
    // rx2 = -0.875, rf2 = 0, m2 = 1

    // c0 = 0, c1 > 0
    p[0] = 65.0 / 64.0;
    p[1] = 2.0 + 65.0 / 64.0;
    p[2] = 3.0;
    p[3] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { -1.0, 1 };
    SingleTest<3, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1, df0 = 0, m0 = 1
    // rx0 = -1, rf0 = 0, m0 = 1

    // c0 > 0, c1 = 0
    double a = 0.0625;
    double b = 0.875;
    p[0] = a * a * a + b;
    p[1] = 3.0 * a * a;
    p[2] = 3.0 * a;
    p[3] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { -1.0189655913861946, 1 };
    SingleTest<3, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.0189655913861946, df0 = -7.6423681324348804e-17, m0 = 1
    // rx0 = -1.0189655913861946, rf0 = 7.1162815238338723e-17, m0 = 1

    // c0 < 0, c1 = 0
    p[0] = a * a * a - b;
    p[1] = 3.0 * a * a;
    p[2] = 3.0 * a;
    p[3] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.89396559138619458, 1 };
    SingleTest<3, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0.89396559138619458, df0 = 1.0182046859356406e-16, m0 = 1
    // rx0 = 0.89396559138619458, rf0 = -7.1162815238338723e-17, m0 = 1

    // c0 != 0, c1 != 0, sign(delta) > 0
    p[0] = 3.0;
    p[1] = 0.0;
    p[2] = -3.0;
    p[3] = 1.0;
    numRoots = 3;
    dXExpecteds[0] = { -0.87938524157181674, 1 };
    dXExpecteds[1] = { 1.3472963553338606, 1 };
    dXExpecteds[2] = { 2.5320888862379562, 1 };
    SingleTest<3, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.87938524157181674, df0 = 2.3595059873634956e-16, m0 = 1
    // dx1 = 1.3472963553338606, df1 = 2.0650445569493192e-16, m1 = 1
    // dx2 = 2.5320888862379562, df2 = 7.9306790983509372e-16, m2 = 1
    // rx0 = -0.87938524157181674, rf0 = 1.7714825639735801e-16, m0 = 1
    // rx1 = 1.3472963553338606, rf1 = 1.9982945105681347e-17, m1 = 1
    // rx2 = 2.5320888862379562, rf2 = 2.7291190246942339e-16, m2 = 1

    // c0 != 0, c1 != 0, sign(delta) < 0
    p[0] = -1.0;
    p[1] = 4.0;
    p[2] = -3.0;
    p[3] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.31767219617198067, 1 };
    SingleTest<3, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // x0 = 0.31767219617198067, f0 = -9.9231849184920217e-18, m0 = 1
    // x0 = 0.31767219617198067, f0 = -6.9730858551940801e-18, m0 = 1

    // c0 != 0, c1 != 0, sign(delta) = 0
    p[0] = 4.0;
    p[1] = 0.0;
    p[2] = -3.0;
    p[3] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { -1.0, 1 };
    dXExpecteds[1] = { 2.0, 2 };
    SingleTest<3, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1, df0 = 0, m0 = 1
    // dx1 = 2, df1 = 0, m1 = 2
    // rx0 = -1, rf0 = 0, m0 = 1
    // rx1 = 2, rf1 = 0, m1 = 2
}

void UnitTestRootsCubic::TestCubicSolveDepressedBisection()
{
    bool useBisection = true;
    double const dMaxXError = 1e-15;
    double const dMaxFError = 1e-16;
    std::array<double, 4> p{};
    std::size_t numRoots{};
    std::array<PolynomialRoot<double>, 3> dXExpecteds{};
    std::array<PolynomialRoot<double>, 3> dRoots{};
    std::array<double, 3> dFValues{};
    std::array<PolynomialRoot<Rational>, 3> rRoots{};
    std::array<Rational, 3> rFValues{};

    // c0 = 0, c1 = 0
    p[0] = 0.0;
    p[1] = 0.0;
    p[2] = 0.0;
    p[3] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.0, 3 };
    SingleTest<3, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0, df0 = 0, m0 = 3
    // rx0 = 0, rf0 = 0, m0 = 3

    // c0 = 0, c1 < 0
    p[0] = 0.0;
    p[1] = -4.0;
    p[2] = 0.0;
    p[3] = 1.0;
    numRoots = 3;
    dXExpecteds[0] = { -2.0, 1 };
    dXExpecteds[1] = { 0.0, 1 };
    dXExpecteds[2] = { 2.0, 1 };
    SingleTest<3, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -2, df0 = 0, m0 = 1
    // dx1 = 0, df1 = 0, m1 = 1
    // dx2 = 2, df2 = 0, m2 = 1
    // rx0 = -2, rf0 = 0, m0 = 1
    // rx1 = 0, rf1 = 0, m1 = 1
    // rx2 = 2, rf2 = 0, m2 = 1

    // c0 = 0, c1 > 0
    p[0] = 0.0;
    p[1] = 1.0;
    p[2] = 0.0;
    p[3] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.0, 1 };
    SingleTest<3, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0, df0 = 0, m0 = 1
    // rx0 = 0, rf0 = 0, m0 = 1

    // c0 > 0, c1 = 0
    p[0] = 8.0;
    p[1] = 0.0;
    p[2] = 0.0;
    p[3] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { -2.0, 1 };
    SingleTest<3, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -2, df0 = 0, m0 = 1
    // rx0 = -2, rf0 = 0, m0 = 1

    // c0 < 0, c1 = 0
    p[0] = -8.0;
    p[1] = 0.0;
    p[2] = 0.0;
    p[3] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 2.0, 1 };
    SingleTest<3, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 2, df0 = 0, m0 = 1
    // rx0 = 2, rf0 = 0, m0 = 1

    // c0 != 0, c1 != 0, sign(delta) > 0
    p[0] = 0.001;
    p[1] = -0.12345;
    p[2] = 0.0;
    p[3] = 1.0;
    numRoots = 3;
    dXExpecteds[0] = { -0.35533678611407293, 1 };
    dXExpecteds[1] = { 0.0081047580240754084, 1 };
    dXExpecteds[2] = { 0.34723202808999754, 1 };
    SingleTest<3, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.35533678611407293, df0 = -9.4276538005796305e-18, m0 = 1
    // dx1 = 0.0081047580240754084, df1 = -1.5321200755508989e-20, m1 = 1
    // dx2 = 0.34723202808999754, df2 = 1.1213437012842635e-17, m2 = 1
    // rx0 = -0.35533678611407293, rf0 = -2.3207466365379852e-18, m0 = 1
    // rx1 = 0.0081047580240754084, rf1 = 7.3580033952895871e-20, m1 = 1
    // rx2 = 0.34723202808999754, rf2 = 4.5809620337002540e-18, m2 = 1

    // c0 != 0, c1 != 0, sign(delta) < 0
    p[0] = 0.001;
    p[1] = 0.12345;
    p[2] = 0.0;
    p[3] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { -0.0080961467563645841, 1 };
    SingleTest<3, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.0080961467563645841, df0 = 1.0187472980468244e-19, m0 = 1
    // rx0 = -0.0080961467563645841, rf0 = 2.5518638379977936e-20, m0 = 1

    // c0 != 0, c1 != 0, sign(delta) = 0
    Polynomial1<Rational> linear0 = Polynomial1<Rational>{ 0.125, 1.0 };
    Polynomial1<Rational> linear1 = Polynomial1<Rational>{ -0.25, 1.0 };
    auto f = linear0 * linear0 * linear1;
    p[0] = f[0];
    p[1] = f[1];
    p[2] = 0.0;
    p[3] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { -0.125, 2 };
    dXExpecteds[1] = { 0.25, 1 };
    SingleTest<3, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.125, df0 = 0, m0 = 2
    // dx1 = 0.25, df1 = 0, m1 = 1
    // rx0 = -0.125, rf1 = 0, m0 = 2
    // rx1 = 0.25, rf1 = 0, m1 = 1

    // Perturb the previous polynomial so that the double root
    // becomes 2 separate roots.
    p[0] = f[0] + Rational(1e-16);
    p[1] = f[1];
    p[2] = 0.0;
    p[3] = 1.0;
    numRoots = 3;
    dXExpecteds[0] = { -0.12500001634464430, 1 };
    dXExpecteds[1] = { -0.12499998365535514, 1 };
    dXExpecteds[2] = { 0.24999999999999928, 1 };
    SingleTest<3, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.12500001634464430, df0 = 9.7159648175885620e-26, m0 = 1
    // dx1 = -0.12499998365535514, df1 = -4.3367651881875314e-19, m1 = 1
    // dx2 = 0.24999999999999928, df2 = -1.3010426069823449e-18, m2 = 1
    // rx0 = -0.12500001634464430, rf0 = 2.1588338637999728e-24, m0 = 1
    // rx1 = -0.12499998365535514, rf1 = 4.3420483579902291e-24, m1 = 1
    // rx2 = 0.24999999999999928, rf2 = 6.5052130349167831e-19, m2 = 1
}

void UnitTestRootsCubic::TestCubicSolveGeneralClosedForm()
{
    bool useBisection = false;
    double const dMaxXError = 1e-15;
    double const dMaxFError = 1e-12;
    std::array<double, 4> p{};
    std::size_t numRoots{};
    std::array<PolynomialRoot<double>, 3> dXExpecteds{};
    std::array<PolynomialRoot<double>, 3> dRoots{};
    std::array<double, 3> dFValues{};
    std::array<PolynomialRoot<Rational>, 3> rRoots{};
    std::array<Rational, 3> rFValues{};

    // c0 = 0, c1 = 0
    p[0] = -1.0 / 256.0;
    p[1] = 3.0 / 32.0;
    p[2] = -3.0 / 4.0;
    p[3] = 2.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.125, 3 };
    SingleTest<3, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0.125, df0 = 0.0, m0 = 3
    // rx0 = 0.125, rf0 = 0.0, m0 = 3

    // c0 = 0, c1 < 0
    p[0] = -2.0 * (63.0 / 64.0);
    p[1] = -2.0 * (2.0 + 63.0 / 64.0);
    p[2] = -6.0;
    p[3] = -2.0;
    numRoots = 3;
    dXExpecteds[0] = { -1.125, 1 };
    dXExpecteds[1] = { -1.0, 1 };
    dXExpecteds[2] = { -0.875, 1 };
    SingleTest<3, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.125, rf0 = 0, m0 = 1
    // dx1 = -1.0, df1 = 0, m1 = 1
    // dx2 = -0.875, df2 = 0, m2 = 1
    // rx0 = -1.125, rf0 = 0, m0 = 1
    // rx1 = -1.0, rf1 = 0, m1 = 1
    // rx2 = -0.875, rf2 = 0, m2 = 1

    // c0 = 0, c1 > 0
    p[0] = -2.0 * (65.0 / 64.0);
    p[1] = -2.0 * (2.0 + 65.0 / 64.0);
    p[2] = -6.0;
    p[3] = -2.0;
    numRoots = 1;
    dXExpecteds[0] = { -1.0, 1 };
    SingleTest<3, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1, df0 = 0, m0 = 1
    // rx0 = -1, rf0 = 0, m0 = 1

    // c0 > 0, c1 = 0
    double a = 0.0625;
    double b = 0.875;
    double mult = std::ldexp(3.0, 10);
    p[0] = mult * (a * a * a + b);
    p[1] = mult * (3.0 * a * a);
    p[2] = mult * (3.0 * a);
    p[3] = mult;
    numRoots = 1;
    dXExpecteds[0] = { -1.0189655913861946, 1 };
    SingleTest<3, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.0189655913861946, df0 = -3.0875973647339034e-15, m0 = 1
    // rx0 = -1.0189655913861946, rf0 = -2.4940458782063351e-13, m0 = 1

    // c0 < 0, c1 = 0
    p[0] = 2.0 * (a * a * a - b);
    p[1] = 2.0 * (3.0 * a * a);
    p[2] = 2.0 * (3.0 * a);
    p[3] = 2.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.89396559138619458, 1 };
    SingleTest<3, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0.89396559138619458, df0 = 2.0364093718712812e-16, m0 = 1
    // rx0 = 0.89396559138619458, rf0 = 1.6237277852905826e-16, m0 = 1

    // c0 != 0, c1 != 0, sign(delta) > 0
    p[0] = 6.0;
    p[1] = 0.0;
    p[2] = -6.0;
    p[3] = 2.0;
    numRoots = 3;
    dXExpecteds[0] = { -0.87938524157181674, 1 };
    dXExpecteds[1] = { 1.3472963553338606, 1 };
    dXExpecteds[2] = { 2.5320888862379558, 1 };
    SingleTest<3, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.87938524157181674, df0 = 4.7190119747269912e-16, m0 = 1
    // dx1 = 1.3472963553338608, df1 = -5.7583635956765410e-16, m1 = 1
    // dx2 = 2.5320888862379558, df2 = -1.8594504335970572e-15, m2 = 1
    // rx0 = -0.87938524157181674, rf0 = 3.4743381395321418e-16, m0 = 1
    // rx1 = 1.3472963553338608, rf1 = -1.0886456149033276e-16, m1 = 1
    // rx2 = 2.5320888862379558, rf2 = -3.5165585587227835e-16, m2 = 1

    // c0 != 0, c1 != 0, sign(delta) < 0
    p[0] = -2.0;
    p[1] = 8.0;
    p[2] = -6.0;
    p[3] = 2.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.31767219617198073, 1 };
    SingleTest<3, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0.31767219617198073, df0 = 3.2964062902649422e-16, m0 = 1
    // rx0 = 0.31767219617198073, rf0 = 3.0825470400054791e-16, m0 = 1

    // c0 != 0, c1 != 0, sign(delta) = 0
    p[0] = 8.0;
    p[1] = 0.0;
    p[2] = -6.0;
    p[3] = 2.0;
    numRoots = 2;
    dXExpecteds[0] = { -1.0, 1 };
    dXExpecteds[1] = { 2.0, 2 };
    SingleTest<3, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1, df0 = 0, m0 = 1
    // dx1 = 2, df1 = 0, m1 = 2
    // rx0 = -1, rf0 = 0, m0 = 1
    // rx1 = 2, rf1 = 0, m1 = 2
}

void UnitTestRootsCubic::TestCubicSolveMonicClosedForm()
{
    bool useBisection = true;
    double const dMaxXError = 1e-15;
    double const dMaxFError = 1e-12;
    std::array<double, 4> p{};
    std::size_t numRoots{};
    std::array<PolynomialRoot<double>, 3> dXExpecteds{};
    std::array<PolynomialRoot<double>, 3> dRoots{};
    std::array<double, 3> dFValues{};
    std::array<PolynomialRoot<Rational>, 3> rRoots{};
    std::array<Rational, 3> rFValues{};

    // c0 = 0, c1 = 0
    p[0] = -1.0 / 512.0;
    p[1] = 3.0 / 64.0;
    p[2] = -3.0 / 8.0;
    p[3] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.125, 3 };
    SingleTest<3, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0.125, df0 = 0.0, m0 = 3
    // rx0 = 0.125, rf0 = 0.0, m0 = 3

    // c0 = 0, c1 < 0
    p[0] = 63.0 / 64.0;
    p[1] = 2.0 + 63.0 / 64.0;
    p[2] = 3.0;
    p[3] = 1.0;
    numRoots = 3;
    dXExpecteds[0] = { -1.125, 1 };
    dXExpecteds[1] = { -1.0, 1 };
    dXExpecteds[2] = { -0.875, 1 };
    SingleTest<3, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.125, rf0 = 0, m0 = 1
    // dx1 = -1.0, df1 = 0, m1 = 1
    // dx2 = -0.875, df2 = 0, m2 = 1
    // rx0 = -1.125, rf0 = 0, m0 = 1
    // rx1 = -1.0, rf1 = 0, m1 = 1
    // rx2 = -0.875, rf2 = 0, m2 = 1

    // c0 = 0, c1 > 0
    p[0] = 65.0 / 64.0;
    p[1] = 2.0 + 65.0 / 64.0;
    p[2] = 3.0;
    p[3] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { -1.0, 1 };
    SingleTest<3, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1, df0 = 0, m0 = 1
    // rx0 = -1, rf0 = 0, m0 = 1

    // c0 > 0, c1 = 0
    double a = 0.0625;
    double b = 0.875;
    p[0] = a * a * a + b;
    p[1] = 3.0 * a * a;
    p[2] = 3.0 * a;
    p[3] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { -1.0189655913861946, 1 };
    SingleTest<3, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.0189655913861946, df0 = -7.6423681324348804e-17, m0 = 1
    // rx0 = -1.0189655913861946, rf0 = 7.1162815238338723e-17, m0 = 1

    // c0 < 0, c1 = 0
    p[0] = a * a * a - b;
    p[1] = 3.0 * a * a;
    p[2] = 3.0 * a;
    p[3] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.89396559138619458, 1 };
    SingleTest<3, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0.89396559138619458, df0 = 1.0182046859356406e-16, m0 = 1
    // rx0 = 0.89396559138619458, rf0 = -7.1162815238338723e-17, m0 = 1

    // c0 != 0, c1 != 0, sign(delta) > 0
    p[0] = 3.0;
    p[1] = 0.0;
    p[2] = -3.0;
    p[3] = 1.0;
    numRoots = 3;
    dXExpecteds[0] = { -0.87938524157181674, 1 };
    dXExpecteds[1] = { 1.3472963553338606, 1 };
    dXExpecteds[2] = { 2.5320888862379562, 1 };
    SingleTest<3, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.87938524157181674, df0 = 2.3595059873634956e-16, m0 = 1
    // dx1 = 1.3472963553338606, df1 = 2.0650445569493192e-16, m1 = 1
    // dx2 = 2.5320888862379562, df2 = 7.9306790983509372e-16, m2 = 1
    // rx0 = -0.87938524157181674, rf0 = 1.7714825639735801e-16, m0 = 1
    // rx1 = 1.3472963553338606, rf1 = 1.9982945105681347e-17, m1 = 1
    // rx2 = 2.5320888862379562, rf2 = 2.7291190246942339e-16, m2 = 1

    // c0 != 0, c1 != 0, sign(delta) < 0
    p[0] = -1.0;
    p[1] = 4.0;
    p[2] = -3.0;
    p[3] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.31767219617198067, 1 };
    SingleTest<3, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // x0 = 0.31767219617198067, f0 = -9.9231849184920217e-18, m0 = 1
    // x0 = 0.31767219617198067, f0 = -6.9730858551940801e-18, m0 = 1

    // c0 != 0, c1 != 0, sign(delta) = 0
    p[0] = 4.0;
    p[1] = 0.0;
    p[2] = -3.0;
    p[3] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { -1.0, 1 };
    dXExpecteds[1] = { 2.0, 2 };
    SingleTest<3, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1, df0 = 0, m0 = 1
    // dx1 = 2, df1 = 0, m1 = 2
    // rx0 = -1, rf0 = 0, m0 = 1
    // rx1 = 2, rf1 = 0, m1 = 2
}

void UnitTestRootsCubic::TestCubicSolveDepressedClosedForm()
{
    bool useBisection = true;
    double const dMaxXError = 1e-15;
    double const dMaxFError = 1e-16;
    std::array<double, 4> p{};
    std::size_t numRoots{};
    std::array<PolynomialRoot<double>, 3> dXExpecteds{};
    std::array<PolynomialRoot<double>, 3> dRoots{};
    std::array<double, 3> dFValues{};
    std::array<PolynomialRoot<Rational>, 3> rRoots{};
    std::array<Rational, 3> rFValues{};

    // c0 = 0, c1 = 0
    p[0] = 0.0;
    p[1] = 0.0;
    p[2] = 0.0;
    p[3] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.0, 3 };
    SingleTest<3, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0, df0 = 0, m0 = 3
    // rx0 = 0, rf0 = 0, m0 = 3

    // c0 = 0, c1 < 0
    p[0] = 0.0;
    p[1] = -4.0;
    p[2] = 0.0;
    p[3] = 1.0;
    numRoots = 3;
    dXExpecteds[0] = { -2.0, 1 };
    dXExpecteds[1] = { 0.0, 1 };
    dXExpecteds[2] = { 2.0, 1 };
    SingleTest<3, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -2, df0 = 0, m0 = 1
    // dx1 = 0, df1 = 0, m1 = 1
    // dx2 = 2, df2 = 0, m2 = 1
    // rx0 = -2, rf0 = 0, m0 = 1
    // rx1 = 0, rf1 = 0, m1 = 1
    // rx2 = 2, rf2 = 0, m2 = 1

    // c0 = 0, c1 > 0
    p[0] = 0.0;
    p[1] = 1.0;
    p[2] = 0.0;
    p[3] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.0, 1 };
    SingleTest<3, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0, df0 = 0, m0 = 1
    // rx0 = 0, rf0 = 0, m0 = 1

    // c0 > 0, c1 = 0
    p[0] = 8.0;
    p[1] = 0.0;
    p[2] = 0.0;
    p[3] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { -2.0, 1 };
    SingleTest<3, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -2, df0 = 0, m0 = 1
    // rx0 = -2, rf0 = 0, m0 = 1

    // c0 < 0, c1 = 0
    p[0] = -8.0;
    p[1] = 0.0;
    p[2] = 0.0;
    p[3] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 2.0, 1 };
    SingleTest<3, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 2, df0 = 0, m0 = 1
    // rx0 = 2, rf0 = 0, m0 = 1

    // c0 != 0, c1 != 0, sign(delta) > 0
    p[0] = 0.001;
    p[1] = -0.12345;
    p[2] = 0.0;
    p[3] = 1.0;
    numRoots = 3;
    dXExpecteds[0] = { -0.35533678611407293, 1 };
    dXExpecteds[1] = { 0.0081047580240754084, 1 };
    dXExpecteds[2] = { 0.34723202808999754, 1 };
    SingleTest<3, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.35533678611407293, df0 = -9.4276538005796305e-18, m0 = 1
    // dx1 = 0.0081047580240754084, df1 = -1.5321200755508989e-20, m1 = 1
    // dx2 = 0.34723202808999754, df2 = 1.1213437012842635e-17, m2 = 1
    // rx0 = -0.35533678611407293, rf0 = -2.3207466365379852e-18, m0 = 1
    // rx1 = 0.0081047580240754084, rf1 = 7.3580033952895871e-20, m1 = 1
    // rx2 = 0.34723202808999754, rf2 = 4.5809620337002540e-18, m2 = 1

    // c0 != 0, c1 != 0, sign(delta) < 0
    p[0] = 0.001;
    p[1] = 0.12345;
    p[2] = 0.0;
    p[3] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { -0.0080961467563645841, 1 };
    SingleTest<3, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.0080961467563645841, df0 = 1.0187472980468244e-19, m0 = 1
    // rx0 = -0.0080961467563645841, rf0 = 2.5518638379977936e-20, m0 = 1

    // c0 != 0, c1 != 0, sign(delta) = 0
    Polynomial1<Rational> linear0 = Polynomial1<Rational>{ 0.125, 1.0 };
    Polynomial1<Rational> linear1 = Polynomial1<Rational>{ -0.25, 1.0 };
    auto f = linear0 * linear0 * linear1;
    p[0] = f[0];
    p[1] = f[1];
    p[2] = 0.0;
    p[3] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { -0.125, 2 };
    dXExpecteds[1] = { 0.25, 1 };
    SingleTest<3, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.125, df0 = 0, m0 = 2
    // dx1 = 0.25, df1 = 0, m1 = 1
    // rx0 = -0.125, rf1 = 0, m0 = 2
    // rx1 = 0.25, rf1 = 0, m1 = 1

    // Perturb the previous polynomial so that the double root
    // becomes 2 separate roots.
    p[0] = f[0] + Rational(1e-16);
    p[1] = f[1];
    p[2] = 0.0;
    p[3] = 1.0;
    numRoots = 3;
    dXExpecteds[0] = { -0.12500001634464430, 1 };
    dXExpecteds[1] = { -0.12499998365535514, 1 };
    dXExpecteds[2] = { 0.24999999999999928, 1 };
    SingleTest<3, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.12500001634464430, df0 = 9.7159648175885620e-26, m0 = 1
    // dx1 = -0.12499998365535514, df1 = -4.3367651881875314e-19, m1 = 1
    // dx2 = 0.24999999999999928, df2 = -1.3010426069823449e-18, m2 = 1
    // rx0 = -0.12500001634464430, rf0 = 2.1588338637999728e-24, m0 = 1
    // rx1 = -0.12499998365535514, rf1 = 4.3420483579902291e-24, m1 = 1
    // rx2 = 0.24999999999999928, rf2 = 6.5052130349167831e-19, m2 = 1
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/RootFinders/RootsCubic.h>

namespace gtl
{
    template class RootsCubic<float>;
    template class RootsCubic<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class RootsCubic<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(RootsCubic)
