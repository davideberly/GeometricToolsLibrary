#if defined(GTL_UNIT_TESTS)

#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/RootFinders/RootsQuartic.h>
using namespace gtl;

namespace gtl
{
    class UnitTestRootsQuartic
    {
    public:
        UnitTestRootsQuartic();

    private:
        using Rational = BSRational<UIntegerAP32>;

        template <std::size_t Degree, std::size_t NumCoefficients>
        void Solve(bool, double const*, std::size_t, PolynomialRoot<double>*, PolynomialRoot<Rational>*) {}

        template <std::size_t Degree>
        double DEvaluate(double const*, double const&) {}

        template <std::size_t Degree>
        Rational REvaluate(double const*, Rational const&) {}

        template<> void Solve<4, 5>(bool useBisection, double const* p, std::size_t numExpectedRoots,
            PolynomialRoot<double>* dRoots, PolynomialRoot<Rational>* rRoots)
        {
            std::size_t numRoots = RootsQuartic<double>::Solve(useBisection, p[0], p[1], p[2], p[3], p[4], dRoots);
            UTAssert(numRoots == numExpectedRoots, "Invalid number of roots (double).");
            numRoots = RootsQuartic<Rational>::Solve(useBisection, p[0], p[1], p[2], p[3], p[4], rRoots);
            UTAssert(numRoots == numExpectedRoots, "Invalid number of roots (Rational).");
        }

        template<> void Solve<4, 4>(bool useBisection, double const* p, std::size_t numExpectedRoots,
            PolynomialRoot<double>* dRoots, PolynomialRoot<Rational>* rRoots)
        {
            std::size_t numRoots = RootsQuartic<double>::Solve(useBisection, p[0], p[1], p[2], p[3], dRoots);
            UTAssert(numRoots == numExpectedRoots, "Invalid number of roots (double).");
            numRoots = RootsQuartic<Rational>::Solve(useBisection, p[0], p[1], p[2], p[3], rRoots);
            UTAssert(numRoots == numExpectedRoots, "Invalid number of roots (Rational).");
        }

        template<> void Solve<4, 3>(bool useBisection, double const* p, std::size_t numExpectedRoots,
            PolynomialRoot<double>* dRoots, PolynomialRoot<Rational>* rRoots)
        {
            std::size_t numRoots = RootsQuartic<double>::Solve(useBisection, p[0], p[1], p[2], dRoots);
            UTAssert(numRoots == numExpectedRoots, "Invalid number of roots (double).");
            numRoots = RootsQuartic<Rational>::Solve(useBisection, p[0], p[1], p[2], rRoots);
            UTAssert(numRoots == numExpectedRoots, "Invalid number of roots (Rational).");
        }

        template<> double DEvaluate<4>(double const* p, double const& x)
        {
            return std::fma(x, std::fma(x, std::fma(x, std::fma(x, p[4], p[3]), p[2]), p[1]), p[0]);
        }

        template<> Rational REvaluate<4>(double const* p, Rational const& rX)
        {
            Rational rP0(p[0]), rP1(p[1]), rP2(p[2]), rP3(p[3]), rP4(p[4]);
            return rP0 + rX * (rP1 + rX * (rP2 + rX * (rP3 + rX * rP4)));
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

        void TestQuarticSolveGeneralBisection();
        void TestQuarticSolveMonicBisection();
        void TestQuarticSolveDepressedBisection();
        void TestQuarticSolveGeneralClosedForm();
        void TestQuarticSolveMonicClosedForm();
        void TestQuarticSolveDepressedClosedForm();
    };
}

UnitTestRootsQuartic::UnitTestRootsQuartic()
{
    UTInformation("Mathematics/RootFinders/RootsQuartic");

    TestQuarticSolveGeneralBisection();
    TestQuarticSolveMonicBisection();
    TestQuarticSolveDepressedBisection();
    TestQuarticSolveGeneralClosedForm();
    TestQuarticSolveMonicClosedForm();
    TestQuarticSolveDepressedClosedForm();
}

void UnitTestRootsQuartic::TestQuarticSolveGeneralBisection()
{
    // These are the TestQuarticSolveDepressed tests with x = y - 1.
    bool useBisection = true;
    double const dMaxXError = 1e-15;
    double const dMaxFError = 1e-13;
    double const multiplier = 3.0;
    std::array<double, 5> p{};
    std::size_t numRoots{};
    std::array<PolynomialRoot<double>, 4> dXExpecteds{};
    std::array<PolynomialRoot<double>, 4> dRoots{};
    std::array<double, 4> dFValues{};
    std::array<PolynomialRoot<Rational>, 4> rRoots{};
    std::array<Rational, 4> rFValues{};

    // d0 = 0, d1 = 0, d2 = 0
    p[0] = 1.0;
    p[1] = -4.0;
    p[2] = 6.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 1;
    dXExpecteds[0] = { 1.0, 4 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 1, df0 = 0, m0 = 4
    // rx0 = 1, rf0 = 0, m0 = 4

    // d0 = 0, d1 = 0, d2 > 0
    p[0] = 2.0;
    p[1] = -6.0;
    p[2] = 7.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 1;
    dXExpecteds[0] = { 1.0, 2 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 1, df0 = 0, m0 = 2
    // rx0 = 1, rf0 = 0, m0 = 2

    // d0 = 0, d1 = 0, d2 < 0
    p[0] = -1.0;
    p[1] = 0.0;
    p[2] = 4.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 3;
    dXExpecteds[0] = { 1.0 - std::sqrt(2.0), 1 };
    dXExpecteds[1] = { 1.0, 2 };
    dXExpecteds[2] = { 1.0 + std::sqrt(2.0), 1 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.41421356237309503, df0 = 1.0817628073847497e-16, m0 = 1
    // dx1 = 1, df1 = 0, m1 = 2
    // dx2 = 2.4142135623730949, df2 = -2.1359401048246719e-15, m2 = 1
    // rx0 = -0.41421356237309503, rf0 = -2.4351687258166853e-16, m0 = 1
    // rx1 = 1, rf1 = 0, m1 = 2
    // rx2 = 2.4142135623730949, rf2 = -2.4351687258166853e-16, m2 = 1

    // d0 = 0, d1 != 0
    p[0] = -3.0;
    p[1] = 3.0;
    p[2] = 3.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 4;
    dXExpecteds[0] = { -0.87938524157181674, 1 };
    dXExpecteds[1] = { 1.0, 1 };
    dXExpecteds[2] = { 1.3472963553338606, 1 };
    dXExpecteds[3] = { 2.5320888862379562, 1 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.87938524157181674, df0 = -1.4889027903759177e-15, m0 = 1
    // dx1 = 1, df1 = 0, m1 = 1
    // dx2 = 1.3472963553338606, df2 = 1.1754459545374543e-15, m2 = 1
    // dx3 = 2.5320888862379562, df3 = 1.0552239827229418e-14, m3 = 1
    // rx0 = -0.87938524157181674, rf0 = -9.9878945593012450e-16, m0 = 1
    // rx1 = 1, rf1 = 0, m1 = 1
    // rx2 = 1.3472963553338606, rf2 = 2.0820012012119223e-17, m2 = 1
    // rx3 = 2.5320888862379562, rf3 = 1.2543758780863816e-15, m3 = 1

    // biquadratic (4 real roots)
    p[0] = -1.0;
    p[1] = 2.0;
    p[2] = 3.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 4;
    dXExpecteds[0] = { -0.61803398874989479, 1 };
    dXExpecteds[1] = { 0.38196601125010515, 1 };
    dXExpecteds[2] = { 1.6180339887498949, 1 };
    dXExpecteds[3] = { 2.6180339887498949, 1 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.61803398874989479, df0 = -1.2723651138672315e-15, m0 = 1
    // dx1 = 0.38196601125010515, df1 = 7.1592876322879724e-17, m1 = 1
    // dx2 = 1.6180339887498949, df2 = -3.5415406986315995e-16, m2 = 1
    // dx3 = 2.6180339887498949, df3 = 1.9608839251377711e-15, m3 = 1
    // rx0 = -0.61803398874989479, rf0 = -1.2308801366482148e-15, m0 = 1
    // rx1 = 0.38196601125010515, rf1 = 9.8672306407265462e-18, m1 = 1
    // rx2 = 1.6180339887498949, rf2 = 9.8672306407265462e-18, m2 = 1
    // rx3 = 2.6180339887498949, rf3 = -1.2308801366482148e-15, m3 = 1

    // biquadratic (2 complex-conjugate pairs)
    p[0] = 3.0;
    p[1] = -6.0;
    p[2] = 7.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 0;
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // biquadratic (2 real roots, 1 complex-conjugate pair)
    p[0] = -4.0;
    p[1] = -6.0;
    p[2] = 7.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 2;
    dXExpecteds[0] = { -0.41421356237309503, 1 };
    dXExpecteds[1] = { 2.4142135623730949, 1 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.41421356237309503, df0 = -1.0388770660336793e-15, m0 = 1
    // dx1 = 2.4142135623730949, df1 = -8.5437604192986874e-15, m1 = 1
    // rx0 = -0.41421356237309503, rf0 = -6.0879218145417138e-16, m0 = 1
    // rx1 = 2.4142135623730949, rf1 = -6.0879218145417138e-16, m1 = 1

    // biquadratic (2 complex-conjugate pairs)
    p[0] = 8.0;
    p[1] = -6.0;
    p[2] = 7.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 0;
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // biquadratic (2 real roots, each of multiplicity 2)
    p[0] = 4.0;
    p[1] = 8.0;
    p[2] = 0.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 2;
    dXExpecteds[0] = { 1.0 - std::sqrt(3.0), 2 };
    dXExpecteds[1] = { 1.0 + std::sqrt(3.0), 2 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.73205080756887730, df0 = -3.8435006836367688e-16, m0 = 2
    // dx1 = 2.7320508075688772, df1 = 2.0857530876482813e-15, m1 = 2
    // rx0 = -0.73205080756887730, rf0 = 4.0996822949569752e-33, m0 = 2
    // rx1 = 2.7320508075688772, rf1 = 4.0996822949569752e-33, m1 = 2

    // biquadratic (1 complex-conjugate pair, multiplicity 2)
    p[0] = 4.0;
    p[1] = -8.0;
    p[2] = 8.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 0;
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // (x-r0)^3*(x-r1), r1 < r0 [delta = 0]
    p[0] = -16.0;
    p[1] = 16.0;
    p[2] = 0.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 2;
    dXExpecteds[0] = { -2.0, 1 };
    dXExpecteds[1] = { 2.0, 3 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -2, df0 = 0, m0 = 1
    // dx1 = 2, df1 = 0, m1 = 3
    // rx0 = -2, rf0 = 0, m0 = 1
    // rx1 = 2, rf1 = 0, m1 = 3

    // (x-r0)^3*(x-r1), r0 < r1 [delta = 0]: transform by x = y-2 (avoids m0 = 0)
    p[0] = 5.0;
    p[1] = -16.0;
    p[2] = 18.0;
    p[3] = -8.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 2;
    dXExpecteds[0] = { 1.0, 3 };
    dXExpecteds[1] = { 5.0, 1 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 1, df0 = 0, m0 = 3
    // dx1 = 5, df1 = 0, m1 = 1
    // rx0 = 1, rf0 = 0, m0 = 3
    // rx1 = 5, rf1 = 0, m1 = 1

    // (x-r0)^2 * (x-z0) * (x-z0c) [delta = 0, d2 != 0, qDiscriminant < 0]
    p[0] = 36.0;
    p[1] = -36.0;
    p[2] = 13.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 1;
    dXExpecteds[0] = { 2.0, 2 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 2, df0 = 0, m0 = 2
    // rx0 = 2, rf0 = 0, m0 = 2

    // (x-r0)^2 * (x-z0) * (x-z0c) [delta = 0, d2 == 0, qDiscriminant < 0]
    p[0] = 8.0;
    p[1] = -8.0;
    p[2] = 6.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 1;
    dXExpecteds[0] = { 2.0, 2 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 2, df0 = 0, m0 = 2
    // rx0 = 2, rf0 = 0, m0 = 2

    // (x-r0)^2 * (x-r1) * (x-r2) [delta = 0, qDiscriminant > 0]
    p[0] = -36.0;
    p[1] = 36.0;
    p[2] = -5.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 3;
    dXExpecteds[0] = { -3.0, 1 };
    dXExpecteds[1] = { 2.0, 2 };
    dXExpecteds[2] = { 3.0, 1 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -3, df0 = 0, m0 = 1
    // dx1 = 2, df1 = 0, m1 = 2
    // dx2 = 3, df2 = 0, m2 = 1
    // rx0 = -3, rf0 = 0, m0 = 1
    // rx1 = 2, rf1 = 0, m1 = 2
    // rx2 = 3, rf2 = 0, m2 = 1

    // (x-z0) * (x-z0c) * (x-z1) * (x-z1x) [delta > 0, d2 > 0]
    p[0] = 72.0;
    p[1] = -36.0;
    p[2] = 17.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 0;
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // (x-r0) * (x-r1) * (x-r2) * (x-r3) [delta > 0, d2 < 0]
    p[0] = -120.0;
    p[1] = 106.0;
    p[2] = -19.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 4;
    dXExpecteds[0] = { -5.0, 1 };
    dXExpecteds[1] = { 2.0, 1 };
    dXExpecteds[2] = { 3.0, 1 };
    dXExpecteds[3] = { 4.0, 1 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -5, df0 = 0, m0 = 1
    // dx1 = 2, df1 = 0, m1 = 1
    // dx2 = 3, df2 = 0, m2 = 1
    // dx3 = 4, df3 = 0, m3 = 1
    // rx0 = -5, rf0 = 0, m0 = 1
    // rx1 = 2, rf1 = 0, m1 = 1
    // rx2 = 3, rf2 = 0, m2 = 1
    // rx3 = 4, rf3 = 0, m3 = 1

    // (x-r0) * (x-r1) * (x-z0) * (x-z0c) [delta < 0]
    p[0] = 40.0;
    p[1] = -14.0;
    p[2] = 1.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 2;
    dXExpecteds[0] = { 2.0, 1 };
    dXExpecteds[1] = { 4.0, 1 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 2, df0 = 0, m0 = 1
    // dx1 = 4, df1 = 0, m1 = 1
    // rx0 = 2, rf0 = 0, m0 = 1
    // rx1 = 4, rf1 = 0, m1 = 1
}

void UnitTestRootsQuartic::TestQuarticSolveMonicBisection()
{
    // These are the TestQuarticSolveDepressed tests with x = y - 1.
    bool useBisection = true;
    double const dMaxXError = 1e-15;
    double const dMaxFError = 1e-13;
    double const multiplier = 1.0;
    std::array<double, 5> p{};
    std::size_t numRoots{};
    std::array<PolynomialRoot<double>, 4> dXExpecteds{};
    std::array<PolynomialRoot<double>, 4> dRoots{};
    std::array<double, 4> dFValues{};
    std::array<PolynomialRoot<Rational>, 4> rRoots{};
    std::array<Rational, 4> rFValues{};

    // d0 = 0, d1 = 0, d2 = 0
    p[0] = 1.0;
    p[1] = -4.0;
    p[2] = 6.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 1;
    dXExpecteds[0] = { 1.0, 4 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 1, df0 = 0, m0 = 4
    // rx0 = 1, rf0 = 0, m0 = 4

    // d0 = 0, d1 = 0, d2 > 0
    p[0] = 2.0;
    p[1] = -6.0;
    p[2] = 7.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 1;
    dXExpecteds[0] = { 1.0, 2 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 1, df0 = 0, m0 = 2
    // rx0 = 1, rf0 = 0, m0 = 2

    // d0 = 0, d1 = 0, d2 < 0
    p[0] = -1.0;
    p[1] = 0.0;
    p[2] = 4.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 3;
    dXExpecteds[0] = { 1.0 - std::sqrt(2.0), 1 };
    dXExpecteds[1] = { 1.0, 2 };
    dXExpecteds[2] = { 1.0 + std::sqrt(2.0), 1 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.41421356237309503, df0 = -8.6573088836139943e-17, m0 = 1
    // dx1 = 1, df1 = 0, m1 = 2
    // dx2 = 2.4142135623730949, df2 = -7.5665195966337276e-16, m2 = 1
    // rx0 = -0.41421356237309503, rf0 = -8.1172290860556186e-17, m0 = 1
    // rx1 = 1, rf1 = 0, m1 = 2
    // rx2 = 2.4142135623730949, rf2 = -8.1172290860556186e-17, m2 = 1

    // d0 = 0, d1 != 0
    p[0] = -3.0;
    p[1] = 3.0;
    p[2] = 3.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 4;
    dXExpecteds[0] = { -0.87938524157181674, 1 };
    dXExpecteds[1] = { 1.0, 1 };
    dXExpecteds[2] = { 1.3472963553338606, 1 };
    dXExpecteds[3] = { 2.5320888862379562, 1 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.87938524157181674, df0 = -2.3595059873634956e-16, m0 = 1
    // dx1 = 1, df1 = 0, m1 = 1
    // dx2 = 1.3472963553338606, df2 = -2.0650445569493192e-16, m2 = 1
    // dx3 = 2.5320888862379562, df3 = 1.4558787956839843e-15, m3 = 1
    // rx0 = -0.87938524157181674, rf0 = -3.3292981864337480e-16, m0 = 1
    // rx1 = 1, rf1 = 0, m1 = 1
    // rx2 = 1.3472963553338606, rf2 = 6.9400040040397409e-18, m2 = 1
    // rx3 = 2.5320888862379562, rf3 = 4.1812529269546056e-16, m3 = 1

    // biquadratic (4 real roots)
    p[0] = -1.0;
    p[1] = 2.0;
    p[2] = 3.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 4;
    dXExpecteds[0] = { -0.61803398874989479, 1 };
    dXExpecteds[1] = { 0.38196601125010515, 1 };
    dXExpecteds[2] = { 1.6180339887498949, 1 };
    dXExpecteds[3] = { 2.6180339887498949, 1 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.61803398874989479, df0 = -1.9540318318539112e-16, m0 = 1
    // dx1 = 0.38196601125010515, df1 = 2.3864292107626575e-17, m1 = 1
    // dx2 = 1.6180339887498949, df2 = -2.3780992921680097e-16, m2 = 1
    // dx3 = 2.6180339887498949, df3 = 3.1452445346376371e-16, m3 = 1
    // rx0 = -0.61803398874989479, rf0 = -4.1029337888273826e-16, m0 = 1
    // rx1 = 0.38196601125010515, rf1 = 3.2890768802421818e-18, m1 = 1
    // rx2 = 1.6180339887498949, rf2 = 3.2890768802421818e-18, m2 = 1
    // rx3 = 2.6180339887498949, rf3 = -4.1029337888273826e-16, m3 = 1

    // biquadratic (2 complex-conjugate pairs)
    p[0] = 3.0;
    p[1] = -6.0;
    p[2] = 7.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 0;
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // biquadratic (2 real roots, 1 complex-conjugate pair)
    p[0] = -4.0;
    p[1] = -6.0;
    p[2] = 7.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 2;
    dXExpecteds[0] = { -0.41421356237309503, 1 };
    dXExpecteds[1] = { 2.4142135623730949, 1 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.41421356237309503, df0 = -3.4629235534455977e-16, m0 = 1
    // dx1 = 2.4142135623730949, df1 = -2.4905447419917049e-15, m1 = 1
    // rx0 = -0.41421356237309503, rf0 = -2.0293072715139045e-16, m0 = 1
    // rx1 = 2.4142135623730949, rf1 = -2.0293072715139045e-16, m1 = 1

    // biquadratic (2 complex-conjugate pairs)
    p[0] = 8.0;
    p[1] = -6.0;
    p[2] = 7.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 0;
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // biquadratic (2 real roots, each of multiplicity 2)
    p[0] = 4.0;
    p[1] = 8.0;
    p[2] = 0.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 2;
    dXExpecteds[0] = { 1.0 - std::sqrt(3.0), 2 };
    dXExpecteds[1] = { 1.0 + std::sqrt(3.0), 2 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.73205080756887730, df0 = 8.8613887014349680e-17, m0 = 2
    // dx1 = 2.7320508075688772, df1 = -5.1802325518739439e-16, m1 = 2
    // rx0 = -0.73205080756887730, rf0 = 1.3665607649856586e-33, m0 = 2
    // rx1 = 2.7320508075688772, rf1 = 1.3665607649856586e-33, m1 = 2

    // biquadratic (1 complex-conjugate pair, multiplicity 2)
    p[0] = 4.0;
    p[1] = -8.0;
    p[2] = 8.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 0;
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // (x-r0)^3*(x-r1), r1 < r0 [delta = 0]
    p[0] = -16.0;
    p[1] = 16.0;
    p[2] = 0.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 2;
    dXExpecteds[0] = { -2.0, 1 };
    dXExpecteds[1] = { 2.0, 3 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -2, df0 = 0, m0 = 1
    // dx1 = 2, df1 = 0, m1 = 3
    // rx0 = -2, rf0 = 0, m0 = 1
    // rx1 = 2, rf1 = 0, m1 = 3

    // (x-r0)^3*(x-r1), r0 < r1 [delta = 0]: transform by x = y-2 (avoids m0 = 0)
    p[0] = 5.0;
    p[1] = -16.0;
    p[2] = 18.0;
    p[3] = -8.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 2;
    dXExpecteds[0] = { 1.0, 3 };
    dXExpecteds[1] = { 5.0, 1 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 1, df0 = 0, m0 = 3
    // dx1 = 5, df1 = 0, m1 = 1
    // rx0 = 1, rf0 = 0, m0 = 3
    // rx1 = 5, rf1 = 0, m1 = 1

    // (x-r0)^2 * (x-z0) * (x-z0c) [delta = 0, d2 != 0, qDiscriminant < 0]
    p[0] = 36.0;
    p[1] = -36.0;
    p[2] = 13.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 1;
    dXExpecteds[0] = { 2.0, 2 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 2, df0 = 0, m0 = 2
    // rx0 = 2, rf0 = 0, m0 = 2

    // (x-r0)^2 * (x-z0) * (x-z0c) [delta = 0, d2 == 0, qDiscriminant < 0]
    p[0] = 8.0;
    p[1] = -8.0;
    p[2] = 6.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 1;
    dXExpecteds[0] = { 2.0, 2 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 2, df0 = 0, m0 = 2
    // rx0 = 2, rf0 = 0, m0 = 2

    // (x-r0)^2 * (x-r1) * (x-r2) [delta = 0, qDiscriminant > 0]
    p[0] = -36.0;
    p[1] = 36.0;
    p[2] = -5.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 3;
    dXExpecteds[0] = { -3.0, 1 };
    dXExpecteds[1] = { 2.0, 2 };
    dXExpecteds[2] = { 3.0, 1 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -3, df0 = 0, m0 = 1
    // dx1 = 2, df1 = 0, m1 = 2
    // dx2 = 3, df2 = 0, m2 = 1
    // rx0 = -3, rf0 = 0, m0 = 1
    // rx1 = 2, rf1 = 0, m1 = 2
    // rx2 = 3, rf2 = 0, m2 = 1

    // (x-z0) * (x-z0c) * (x-z1) * (x-z1x) [delta > 0, d2 > 0]
    p[0] = 72.0;
    p[1] = -36.0;
    p[2] = 17.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 0;
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // (x-r0) * (x-r1) * (x-r2) * (x-r3) [delta > 0, d2 < 0]
    p[0] = -120.0;
    p[1] = 106.0;
    p[2] = -19.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 4;
    dXExpecteds[0] = { -5.0, 1 };
    dXExpecteds[1] = { 2.0, 1 };
    dXExpecteds[2] = { 3.0, 1 };
    dXExpecteds[3] = { 4.0, 1 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -5, df0 = 0, m0 = 1
    // dx1 = 2, df1 = 0, m1 = 1
    // dx2 = 3, df2 = 0, m2 = 1
    // dx3 = 4, df3 = 0, m3 = 1
    // rx0 = -5, rf0 = 0, m0 = 1
    // rx1 = 2, rf1 = 0, m1 = 1
    // rx2 = 3, rf2 = 0, m2 = 1
    // rx3 = 4, rf3 = 0, m3 = 1

    // (x-r0) * (x-r1) * (x-z0) * (x-z0c) [delta < 0]
    p[0] = 40.0;
    p[1] = -14.0;
    p[2] = 1.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 2;
    dXExpecteds[0] = { 2.0, 1 };
    dXExpecteds[1] = { 4.0, 1 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 2, df0 = 0, m0 = 1
    // dx1 = 4, df1 = 0, m1 = 1
    // rx0 = 2, rf0 = 0, m0 = 1
    // rx1 = 4, rf1 = 0, m1 = 1
}

void UnitTestRootsQuartic::TestQuarticSolveDepressedBisection()
{
    bool useBisection = true;
    double const dMaxXError = 1e-15;
    double const dMaxFError = 1e-13;
    std::array<double, 5> p{};
    std::size_t numRoots{};
    std::array<PolynomialRoot<double>, 4> dXExpecteds{};
    std::array<PolynomialRoot<double>, 4> dRoots{};
    std::array<double, 4> dFValues{};
    std::array<PolynomialRoot<Rational>, 4> rRoots{};
    std::array<Rational, 4> rFValues{};

    // d0 = 0, d1 = 0, d2 = 0
    p[0] = 0.0;
    p[1] = 0.0;
    p[2] = 0.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.0, 4 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0, df0 = 0, m0 = 4
    // rx0 = 0, rf0 = 0, m0 = 4

    // d0 = 0, d1 = 0, d2 > 0
    p[0] = 0.0;
    p[1] = 0.0;
    p[2] = 1.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.0, 2 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0, df0 = 0, m0 = 2
    // rx0 = 0, r0 = 0, m0 = 2

    // d0 = 0, d1 = 0, d2 < 0
    p[0] = 0.0;
    p[1] = 0.0;
    p[2] = -2.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 3;
    dXExpecteds[0] = { -std::sqrt(2.0), 1 };
    dXExpecteds[1] = { 0.0, 2 };
    dXExpecteds[2] = { std::sqrt(2.0), 1 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.4142135623730949, df0 = -7.0920927433406598e-16, m0 = 1
    // dx1 = 0, df1 = 0, m1 = 2
    // dx2 = +1.4142135623730949, df2 = -7.0920927433406598e-16, m2 = 1
    // rx0 = -1.4142135623730949, rf0 = -8.1172290860556186e-17, m0 = 1
    // rx1 = 0, rf1 = 0, m1 = 2
    // rx2 = +1.4142135623730949, rf2 = -8.1172290860556186e-17, m2 = 1

    // d0 = 0, d1 != 0
    p[0] = 0.0;
    p[1] = 1.0;
    p[2] = -3.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 4;
    dXExpecteds[0] = { -1.8793852415718169, 1 };
    dXExpecteds[1] = { 0.0, 1 };
    dXExpecteds[2] = { 0.34729635533386072, 1 };
    dXExpecteds[3] = { 1.5320888862379562, 1 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.8793852415718169, df0 = 1.1104728932133024e-15, m0 = 1
    // dx1 = 0, df1 = 0, m1 = 1
    // dx2 = 0.34729635533386072, df2 = -3.0759126044386366e-17, m2 = 1
    // dx3 = 1.5320888862379562, df3 = 1.1718388047462252e-15, m3 = 1
    // rx0 = -1.8793852415718169, rf0 = -3.3292981864337480e-16, m0 = 1
    // rx1 = 0, rf1 = 0, m1 = 1
    // rx2 = 0.34729635533386072, rf2 = 6.9400040040397409e-18, m2 = 1
    // rx3 = 1.5320888862379562, rf3 = 4.1812529269546056e-16, m3 = 1

    // biquadratic (4 real roots)
    p[0] = 1.0;
    p[1] = 0.0;
    p[2] = -3.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 4;
    dXExpecteds[0] = { -1.6180339887498949, 1 };
    dXExpecteds[1] = { -0.61803398874989490, 1 };
    dXExpecteds[2] = { 0.61803398874989490, 1 };
    dXExpecteds[3] = { 1.6180339887498949, 1 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.6180339887498949, df0 = 4.1744778811042245e-16, m0 = 1
    // dx1 = -0.61803398874989490, df1 = -2.5869690143265365e-16, m1 = 1
    // dx2 = 0.61803398874989490, df2 = -2.5869690143265365e-16, m2 = 1
    // dx3 = 1.6180339887498949, df3 = 4.1744778811042245e-16, m3 = 1
    // rx0 = -1.6180339887498949, rf0 = -4.1029337888273826e-16, m0 = 1
    // rx1 = -0.61803398874989490, rf1 = 3.2890768802421818e-18, m1 = 1
    // rx2 = 0.61803398874989490, rf2 = 3.2890768802421818e-18, m2 = 1
    // rx3 = 1.6180339887498949, rf3 = -4.1029337888273826e-16, m3 = 1

    // biquadratic (2 complex-conjugate pairs)
    p[0] = 1.0;
    p[1] = 0.0;
    p[2] = 1.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 0;
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // biquadratic (2 real roots, 1 complex-conjugate pair)
    p[0] = -6.0;
    p[1] = 0.0;
    p[2] = 1.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { -std::sqrt(2.0), 1 };
    dXExpecteds[1] = { std::sqrt(2.0), 1 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.4142135623730949, df0 = -2.3198878784481192e-15, m0 = 1
    // dx1 = 1.4142135623730949, df1 = -2.3198878784481192e-15, m1 = 1
    // rx0 = -1.4142135623730949, rf0 = -2.0293072715139045e-16, m0 = 1
    // rx1 = 1.4142135623730949, rf1 = -2.0293072715139045e-16, m1 = 1

    // biquadratic (2 complex-conjugate pairs)
    p[0] = 6.0;
    p[1] = 0.0;
    p[2] = 1.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 0;
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // biquadratic (2 real roots, each of multiplicity 2)
    p[0] = 9.0;
    p[1] = 0.0;
    p[2] = -6.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { -std::sqrt(3.0), 2 };
    dXExpecteds[1] = { std::sqrt(3.0), 2 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // x0 = -1.7320508075688772, f0 = 2.7369146927071510e-16, m0 = 2
    // x1 = +1.7320508075688772, f1 = 2.7369146927071510e-16, m1 = 2
    // x0 = -1.7320508075688772, f0 = 1.3665607649856586e-33, m0 = 2
    // x1 = +1.7320508075688772, f1 = 1.3665607649856586e-33, m1 = 2

    // biquadratic (1 complex-conjugate pair, multiplicity 2)
    p[0] = 1.0;
    p[1] = 0.0;
    p[2] = 2.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 0;
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // (x-r0)^3*(x-r1), r1 < r0 [delta = 0]
    p[0] = -3.0;
    p[1] = 8.0;
    p[2] = -6.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { -3.0, 1 };
    dXExpecteds[1] = { 1.0, 3 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -3, df0 = 0, m0 = 1
    // dx1 = 1, df1 = 0, m1 = 3
    // rx0 = -3, rf0 = 0, m0 = 1
    // rx1 = 1, rf1 = 0, m1 = 3

    // (x-r0)^3*(x-r1), r0 < r1 [delta = 0]
    p[0] = -3.0;
    p[1] = -8.0;
    p[2] = -6.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { -1.0, 3 };
    dXExpecteds[1] = { 3.0, 1 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1, df0 = 0, m0 = 3
    // dx1 = 3, df1 = 0, m1 = 1
    // rx0 = -1, rf0 = 0, m0 = 3
    // rx1 = 3, rf1 = 0, m1 = 1

    // (x-r0)^2 * (x-z0) * (x-z0c) [delta = 0, d2 != 0, qDiscriminant < 0]
    p[0] = 10.0;
    p[1] = -18.0;
    p[2] = 7.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 1.0, 2 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 1, df0 = 0, m0 = 2
    // rx0 = 1, rf0 = 0, m0 = 2

    // (x-r0)^2 * (x-z0) * (x-z0c) [delta = 0, d2 == 0, qDiscriminant < 0]
    p[0] = 3.0;
    p[1] = -4.0;
    p[2] = 0.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 1.0, 2 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 1, df0 = 0, m0 = 2
    // rx0 = 1, rf0 = 0, m0 = 2

    // (x-r0)^2 * (x-r1) * (x-r2) [delta = 0, qDiscriminant > 0]
    p[0] = -8.0;
    p[1] = 18.0;
    p[2] = -11.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 3;
    dXExpecteds[0] = { -4.0, 1 };
    dXExpecteds[1] = { 1.0, 2 };
    dXExpecteds[2] = { 2.0, 1 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -4, df0 = 0, m0 = 1
    // dx1 = 1, df1 = 0, m1 = 2
    // dx2 = 2, df2 = 0, m2 = 1
    // rx0 = -4, rf0 = 0, m0 = 1
    // rx1 = 1, rf1 = 0, m1 = 2
    // rx2 = 2, rf2 = 0, m2 = 1

    // (x-z0) * (x-z0c) * (x-z1) * (x-z1x) [delta > 0, d2 > 0]
    p[0] = 50.0;
    p[1] = -10.0;
    p[2] = 11.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 0;
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // (x-r0) * (x-r1) * (x-r2) * (x-r3) [delta > 0, d2 < 0]
    p[0] = -36.0;
    p[1] = 60.0;
    p[2] = -25.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 4;
    dXExpecteds[0] = { -6.0, 1 };
    dXExpecteds[1] = { 1.0, 1 };
    dXExpecteds[2] = { 2.0, 1 };
    dXExpecteds[3] = { 3.0, 1 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -6, df0 = 0, m0 = 1
    // dx1 = 1, df1 = 0, m1 = 1
    // dx2 = 2, df2 = 0, m2 = 1
    // dx3 = 3, df3 = 0, m3 = 1
    // rx0 = -6, rf0 = 0, m0 = 1
    // rx1 = 1, rf1 = 0, m1 = 1
    // rx2 = 2, rf2 = 0, m2 = 1
    // rx3 = 3, rf3 = 0, m3 = 1

    // (x-r0) * (x-r1) * (x-z0) * (x-z0c) [delta < 0]
    p[0] = 24.0;
    p[1] = -20.0;
    p[2] = -5.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { 1.0, 1 };
    dXExpecteds[1] = { 3.0, 1 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 1, df0 = 0, m0 = 1
    // dx1 = 3, df1 = 0, m1 = 1
    // rx0 = 1, rf0 = 0, m0 = 1
    // rx1 = 3, rf1 = 0, m1 = 1
}

void UnitTestRootsQuartic::TestQuarticSolveGeneralClosedForm()
{
    // These are the TestQuarticSolveDepressed tests with x = y - 1.
    bool useBisection = false;
    double const dMaxXError = 1e-15;
    double const dMaxFError = 1e-12;
    double const multiplier = 3.0;
    std::array<double, 5> p{};
    std::size_t numRoots{};
    std::array<PolynomialRoot<double>, 4> dXExpecteds{};
    std::array<PolynomialRoot<double>, 4> dRoots{};
    std::array<double, 4> dFValues{};
    std::array<PolynomialRoot<Rational>, 4> rRoots{};
    std::array<Rational, 4> rFValues{};

    // d0 = 0, d1 = 0, d2 = 0
    p[0] = 1.0;
    p[1] = -4.0;
    p[2] = 6.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 1;
    dXExpecteds[0] = { 1.0, 4 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 1, df0 = 0, m0 = 4
    // rx0 = 1, rf0 = 0, m0 = 4

    // d0 = 0, d1 = 0, d2 > 0
    p[0] = 2.0;
    p[1] = -6.0;
    p[2] = 7.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 1;
    dXExpecteds[0] = { 1.0, 2 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 1, df0 = 0, m0 = 2
    // rx0 = 1, rf0 = 0, m0 = 2

    // d0 = 0, d1 = 0, d2 < 0
    p[0] = -1.0;
    p[1] = 0.0;
    p[2] = 4.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 3;
    dXExpecteds[0] = { 1.0 - std::sqrt(2.0), 1 };
    dXExpecteds[1] = { 1.0, 2 };
    dXExpecteds[2] = { 1.0 + std::sqrt(2.0), 1 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.41421356237309515, df0 = 1.6480620202249443e-15, m0 = 1
    // dx1 = 1, df1 = 0, m1 = 2
    // dx2 = 2.4142135623730949, df2 = -2.1359401048246719e-15, m2 = 1
    // rx0 = -0.41421356237309515, rf0 = 1.6405940778388619e-15, m0 = 1
    // rx1 = 1, rf1 = 0, m1 = 2
    // rx2 = 2.4142135623730949, rf2 = 1.6405940778388619e-15, m2 = 1

    // d0 = 0, d1 != 0
    p[0] = -3.0;
    p[1] = 3.0;
    p[2] = 3.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 4;
    dXExpecteds[0] = { -0.87938524157181674, 1 };
    dXExpecteds[1] = { 1.0, 1 };
    dXExpecteds[2] = { 1.3472963553338608, 1 };
    dXExpecteds[3] = { 2.5320888862379558, 1 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.87938524157181674, df0 = -1.4889027903759177e-15, m0 = 1
    // dx1 = 1, df1 = 0, m1 = 1
    // dx2 = 1.3472963553338608, df2 = -9.3120478227076920e-16, m2 = 1
    // dx3 = 2.5320888862379558, df3 = -5.6443744953009558e-15, m3 = 1
    // rx0 = -0.87938524157181674, rf0 = -9.7944297355001869e-16, m0 = 1
    // rx1 = 1, rf1 = 0, m1 = 1
    // rx2 = 1.3472963553338608, rf2 = -5.6712398145917301e-17, m2 = 1
    // rx3 = 2.5320888862379558, rf3 = -8.0815204284362126e-16, m3 = 1

    // biquadratic (4 real roots)
    p[0] = -1.0;
    p[1] = 2.0;
    p[2] = 3.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 4;
    dXExpecteds[0] = { -0.61803398874989490, 1 };
    dXExpecteds[1] = { 0.38196601125010510, 1 };
    dXExpecteds[2] = { 1.6180339887498949, 1 };
    dXExpecteds[3] = { 2.6180339887498949, 1 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.61803398874989490, df0 = 1.4622462686090192e-15, m0 = 1
    // dx1 = 0.38196601125010510, df1 = -3.6439736571132598e-16, m1 = 1
    // dx2 = 1.6180339887498949, df2 = -3.5415406986315995e-16, m2 = 1
    // dx3 = 2.6180339887498949, df3 = 1.9608839251377711e-15, m3 = 1
    // rx0 = -0.61803398874989490, rf0 = 1.1792146462637020e-15, m0 = 1
    // rx1 = 0.38196601125010510, rf1 = -4.5041991484104990e-16, m1 = 1
    // rx2 = 1.6180339887498949, rf2 = -4.5041991484104990e-16, m2 = 1
    // rx3 = 2.6180339887498949, rf3 = 1.1792146462637020e-15, m3 = 1

    // biquadratic (2 complex-conjugate pairs)
    p[0] = 3.0;
    p[1] = -6.0;
    p[2] = 7.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 0;
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // biquadratic (2 real roots, 1 complex-conjugate pair)
    p[0] = -4.0;
    p[1] = -6.0;
    p[2] = 7.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 2;
    dXExpecteds[0] = { -0.41421356237309515, 1 };
    dXExpecteds[1] = { 2.4142135623730949, 1 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.41421356237309515, df0 = 3.6490837029246180e-15, m0 = 1
    // dx1 = 2.4142135623730949, df1 = -8.5437604192986874e-15, m1 = 1
    // rx0 = -0.41421356237309515, rf0 = 4.1014851945971544e-15, m0 = 1
    // rx1 = 2.4142135623730949, rf1 = 4.1014851945971544e-15, m1 = 1

    // biquadratic (2 complex-conjugate pairs)
    p[0] = 8.0;
    p[1] = -6.0;
    p[2] = 7.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 0;
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // biquadratic (2 real roots, each of multiplicity 2)
    p[0] = 4.0;
    p[1] = 8.0;
    p[2] = 0.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 2;
    dXExpecteds[0] = { 1.0 - std::sqrt(3.0), 2 };
    dXExpecteds[1] = { 1.0 + std::sqrt(3.0), 2 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.73205080756887719, df0 = -1.1652055593853478e-15, m0 = 2
    // dx1 = 2.7320508075688772, df1 = 2.0857530876482813e-15, m1 = 2
    // rx0 = -0.73205080756887719, rf0 = 3.6253049521952823e-31, m0 = 2
    // rx1 = 2.7320508075688772, rf1 = 3.6253049521952823e-31, m1 = 2

    // biquadratic (1 complex-conjugate pair, multiplicity 2)
    p[0] = 4.0;
    p[1] = -8.0;
    p[2] = 8.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 0;
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // (x-r0)^3*(x-r1), r1 < r0 [delta = 0]
    p[0] = -16.0;
    p[1] = 16.0;
    p[2] = 0.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 2;
    dXExpecteds[0] = { -2.0, 1 };
    dXExpecteds[1] = { 2.0, 3 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -2, df0 = 0, m0 = 1
    // dx1 = 2, df1 = 0, m1 = 3
    // rx0 = -2, rf0 = 0, m0 = 1
    // rx1 = 2, rf1 = 0, m1 = 3

    // (x-r0)^3*(x-r1), r0 < r1 [delta = 0]: transform by x = y-2 (avoids m0 = 0)
    p[0] = 5.0;
    p[1] = -16.0;
    p[2] = 18.0;
    p[3] = -8.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 2;
    dXExpecteds[0] = { 1.0, 3 };
    dXExpecteds[1] = { 5.0, 1 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 1, df0 = 0, m0 = 3
    // dx1 = 5, df1 = 0, m1 = 1
    // rx0 = 1, rf0 = 0, m0 = 3
    // rx1 = 5, rf1 = 0, m1 = 1

    // (x-r0)^2 * (x-z0) * (x-z0c) [delta = 0, d2 != 0, qDiscriminant < 0]
    p[0] = 36.0;
    p[1] = -36.0;
    p[2] = 13.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 1;
    dXExpecteds[0] = { 2.0, 2 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 2, df0 = 0, m0 = 2
    // rx0 = 2, rf0 = 0, m0 = 2

    // (x-r0)^2 * (x-z0) * (x-z0c) [delta = 0, d2 == 0, qDiscriminant < 0]
    p[0] = 8.0;
    p[1] = -8.0;
    p[2] = 6.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 1;
    dXExpecteds[0] = { 2.0, 2 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 2, df0 = 0, m0 = 2
    // rx0 = 2, rf0 = 0, m0 = 2

    // (x-r0)^2 * (x-r1) * (x-r2) [delta = 0, qDiscriminant > 0]
    p[0] = -36.0;
    p[1] = 36.0;
    p[2] = -5.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 3;
    dXExpecteds[0] = { -3.0, 1 };
    dXExpecteds[1] = { 2.0, 2 };
    dXExpecteds[2] = { 3.0, 1 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -3, df0 = 0, m0 = 1
    // dx1 = 2, df1 = 0, m1 = 2
    // dx2 = 3, df2 = 0, m2 = 1
    // rx0 = -3, rf0 = 0, m0 = 1
    // rx1 = 2, rf1 = 0, m1 = 2
    // rx2 = 3, rf2 = 0, m2 = 1

    // (x-z0) * (x-z0c) * (x-z1) * (x-z1x) [delta > 0, d2 > 0]
    p[0] = 72.0;
    p[1] = -36.0;
    p[2] = 17.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 0;
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // (x-r0) * (x-r1) * (x-r2) * (x-r3) [delta > 0, d2 < 0]
    p[0] = -120.0;
    p[1] = 106.0;
    p[2] = -19.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 4;
    dXExpecteds[0] = { -5.0, 1 };
    dXExpecteds[1] = { 2.0, 1 };
    dXExpecteds[2] = { 3.0, 1 };
    dXExpecteds[3] = { 4.0, 1 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -5, df0 = 0, m0 = 1
    // dx1 = 2.0000000000000004, df1 = 2.3092638912203243e-14, m1 = 1
    // dx2 = 3, df2 = 0, m2 = 1
    // dx3 = 4, df3 = 0, m3 = 1
    // rx0 = -5, rf0 = 3.3573144264664739e-13, m0 = 1
    // rx1 = 2.0000000000000004, rf1 = 2.5646151868841094e-14, m1 = 1
    // rx2 = 3, rf2 = 3.9968028886505635e-15, m2 = 1
    // rx3 = 4, rf3 = -1.1990408665951686e-14, m3 = 1

    // (x-r0) * (x-r1) * (x-z0) * (x-z0c) [delta < 0]
    p[0] = 40.0;
    p[1] = -14.0;
    p[2] = 1.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 2;
    dXExpecteds[0] = { 2.0, 1 };
    dXExpecteds[1] = { 4.0, 1 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 2.0000000000000004, df0 = -2.6645352591003757e-14, m0 = 1
    // dx1 = 3.9999999999999996, df1 = -7.1942451995710131e-14, m1 = 1
    // rx0 = 2.0000000000000004, rf0 = -2.5979218776228663e-14, m0 = 1
    // rx1 = 3.9999999999999996, rf1 = -5.7953641885433159e-14, m1 = 1
}

void UnitTestRootsQuartic::TestQuarticSolveMonicClosedForm()
{
    // These are the TestQuarticSolveDepressed tests with x = y - 1.
    bool useBisection = false;
    double const dMaxXError = 1e-15;
    double const dMaxFError = 1e-12;
    double const multiplier = 1.0;
    std::array<double, 5> p{};
    std::size_t numRoots{};
    std::array<PolynomialRoot<double>, 4> dXExpecteds{};
    std::array<PolynomialRoot<double>, 4> dRoots{};
    std::array<double, 4> dFValues{};
    std::array<PolynomialRoot<Rational>, 4> rRoots{};
    std::array<Rational, 4> rFValues{};

    // d0 = 0, d1 = 0, d2 = 0
    p[0] = 1.0;
    p[1] = -4.0;
    p[2] = 6.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 1;
    dXExpecteds[0] = { 1.0, 4 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 1, df0 = 0, m0 = 4
    // rx0 = 1, rf0 = 0, m0 = 4

    // d0 = 0, d1 = 0, d2 > 0
    p[0] = 2.0;
    p[1] = -6.0;
    p[2] = 7.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 1;
    dXExpecteds[0] = { 1.0, 2 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 1, df0 = 0, m0 = 2
    // rx0 = 1, rf0 = 0, m0 = 2

    // d0 = 0, d1 = 0, d2 < 0
    p[0] = -1.0;
    p[1] = 0.0;
    p[2] = 4.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 3;
    dXExpecteds[0] = { 1.0 - std::sqrt(2.0), 1 };
    dXExpecteds[1] = { 1.0, 2 };
    dXExpecteds[2] = { 1.0 + std::sqrt(2.0), 1 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.41421356237309515, df0 = 5.4935400674164810e-16, m0 = 1
    // dx1 = 1, df1 = 0, m1 = 2
    // dx2 = 2.4142135623730949, df2 = -7.5665195966337276e-16, m2 = 1
    // rx0 = -0.41421356237309515, rf0 = 5.4686469261295395e-16, m0 = 1
    // rx1 = 1, rf1 = 0, m1 = 2
    // rx2 = 2.4142135623730949, rf2 = 5.4686469261295395e-16, m2 = 1

    // d0 = 0, d1 != 0
    p[0] = -3.0;
    p[1] = 3.0;
    p[2] = 3.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 4;
    dXExpecteds[0] = { -0.87938524157181674, 1 };
    dXExpecteds[1] = { 1.0, 1 };
    dXExpecteds[2] = { 1.3472963553338608, 1 };
    dXExpecteds[3] = { 2.5320888862379558, 1 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.87938524157181674, df0 = -2.3595059873634956e-16, m0 = 1
    // dx1 = 1, df1 = 0, m1 = 1
    // dx2 = 1.3472963553338608, df2 = 2.8791817978382705e-16, m2 = 1
    // dx3 = 2.5320888862379558, df3 = -1.8814581651003186e-15, m3 = 1
    // rx0 = -0.87938524157181674, rf0 = -3.2648099118333953e-16, m0 = 1
    // rx1 = 1, rf1 = 0, m1 = 1
    // rx2 = 1.3472963553338608, rf2 = -1.8904132715305768e-17, m2 = 1
    // rx3 = 2.5320888862379558, rf3 = -2.6938401428120707e-16, m3 = 1

    // biquadratic (4 real roots)
    p[0] = -1.0;
    p[1] = 2.0;
    p[2] = 3.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 4;
    dXExpecteds[0] = { -0.61803398874989490, 1 };
    dXExpecteds[1] = { 0.38196601125010510, 1 };
    dXExpecteds[2] = { 1.6180339887498949, 1 };
    dXExpecteds[3] = { 2.6180339887498949, 1 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.61803398874989490, df0 = 3.9592801429486531e-16, m0 = 1
    // dx1 = 0.38196601125010510, df1 = -1.2146578857044199e-16, m1 = 1
    // dx2 = 1.6180339887498949, df2 = -2.3780992921680097e-16, m2 = 1
    // dx3 = 2.6180339887498949, df3 = 3.1452445346376371e-16, m3 = 1
    // rx0 = -0.61803398874989490, rf0 = 3.9307154875456732e-16, m0 = 1
    // rx1 = 0.38196601125010510, rf1 = -1.5013997161368331e-16, m1 = 1
    // rx2 = 1.6180339887498949, rf2 = -1.5013997161368331e-16, m2 = 1
    // rx3 = 2.6180339887498949, rf3 = 3.9307154875456732e-16, m3 = 1

    // biquadratic (2 complex-conjugate pairs)
    p[0] = 3.0;
    p[1] = -6.0;
    p[2] = 7.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 0;
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // biquadratic (2 real roots, 1 complex-conjugate pair)
    p[0] = -4.0;
    p[1] = -6.0;
    p[2] = 7.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 2;
    dXExpecteds[0] = { -0.41421356237309515, 1 };
    dXExpecteds[1] = { 2.4142135623730949, 1 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.41421356237309515, df0 = 1.4616249324728026e-15, m0 = 1
    // dx1 = 2.4142135623730949, df1 = -2.4905447419917049e-15, m1 = 1
    // rx0 = -0.41421356237309515, rf0 = 1.3671617315323848e-15, m0 = 1
    // rx1 = 2.4142135623730949, rf1 = 1.3671617315323848e-15, m1 = 1

    // biquadratic (2 complex-conjugate pairs)
    p[0] = 8.0;
    p[1] = -6.0;
    p[2] = 7.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 0;
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // biquadratic (2 real roots, each of multiplicity 2)
    p[0] = 4.0;
    p[1] = 8.0;
    p[2] = 0.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 2;
    dXExpecteds[0] = { 1.0 - std::sqrt(3.0), 2 };
    dXExpecteds[1] = { 1.0 + std::sqrt(3.0), 2 };
    SingleTest<4, 5>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.73205080756887719, df0 = -6.0513242959735789e-16, m0 = 2
    // dx1 = 2.7320508075688772, df1 = -5.1802325518739439e-16, m1 = 2
    // rx0 = -0.73205080756887719, rf0 = 1.2084349840650942e-31, m0 = 2
    // rx1 = 2.7320508075688772, rf1 = 1.2084349840650942e-31, m1 = 2

    // biquadratic (1 complex-conjugate pair, multiplicity 2)
    p[0] = 4.0;
    p[1] = -8.0;
    p[2] = 8.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 0;
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // (x-r0)^3*(x-r1), r1 < r0 [delta = 0]
    p[0] = -16.0;
    p[1] = 16.0;
    p[2] = 0.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 2;
    dXExpecteds[0] = { -2.0, 1 };
    dXExpecteds[1] = { 2.0, 3 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -2, df0 = 0, m0 = 1
    // dx1 = 2, df1 = 0, m1 = 3
    // rx0 = -2, rf0 = 0, m0 = 1
    // rx1 = 2, rf1 = 0, m1 = 3

    // (x-r0)^3*(x-r1), r0 < r1 [delta = 0]: transform by x = y-2 (avoids m0 = 0)
    p[0] = 5.0;
    p[1] = -16.0;
    p[2] = 18.0;
    p[3] = -8.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 2;
    dXExpecteds[0] = { 1.0, 3 };
    dXExpecteds[1] = { 5.0, 1 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 1, df0 = 0, m0 = 3
    // dx1 = 5, df1 = 0, m1 = 1
    // rx0 = 1, rf0 = 0, m0 = 3
    // rx1 = 5, rf1 = 0, m1 = 1

    // (x-r0)^2 * (x-z0) * (x-z0c) [delta = 0, d2 != 0, qDiscriminant < 0]
    p[0] = 36.0;
    p[1] = -36.0;
    p[2] = 13.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 1;
    dXExpecteds[0] = { 2.0, 2 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 2, df0 = 0, m0 = 2
    // rx0 = 2, rf0 = 0, m0 = 2

    // (x-r0)^2 * (x-z0) * (x-z0c) [delta = 0, d2 == 0, qDiscriminant < 0]
    p[0] = 8.0;
    p[1] = -8.0;
    p[2] = 6.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 1;
    dXExpecteds[0] = { 2.0, 2 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 2, df0 = 0, m0 = 2
    // rx0 = 2, rf0 = 0, m0 = 2

    // (x-r0)^2 * (x-r1) * (x-r2) [delta = 0, qDiscriminant > 0]
    p[0] = -36.0;
    p[1] = 36.0;
    p[2] = -5.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 3;
    dXExpecteds[0] = { -3.0, 1 };
    dXExpecteds[1] = { 2.0, 2 };
    dXExpecteds[2] = { 3.0, 1 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -3, df0 = 0, m0 = 1
    // dx1 = 2, df1 = 0, m1 = 2
    // dx2 = 3, df2 = 0, m2 = 1
    // rx0 = -3, rf0 = 0, m0 = 1
    // rx1 = 2, rf1 = 0, m1 = 2
    // rx2 = 3, rf2 = 0, m2 = 1

    // (x-z0) * (x-z0c) * (x-z1) * (x-z1x) [delta > 0, d2 > 0]
    p[0] = 72.0;
    p[1] = -36.0;
    p[2] = 17.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 0;
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // (x-r0) * (x-r1) * (x-r2) * (x-r3) [delta > 0, d2 < 0]
    p[0] = -120.0;
    p[1] = 106.0;
    p[2] = -19.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 4;
    dXExpecteds[0] = { -5.0, 1 };
    dXExpecteds[1] = { 2.0, 1 };
    dXExpecteds[2] = { 3.0, 1 };
    dXExpecteds[3] = { 4.0, 1 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -5, df0 = 0, m0 = 1
    // dx1 = 2.0000000000000004, df1 = 1.2434497875801750e-14, m1 = 1
    // dx2 = 3, df2 = 0, m2 = 1
    // dx3 = 4, df3 = 0, m3 = 1
    // rx0 = -5, rf0 = 1.1191048088221579e-13, m0 = 1
    // rx1 = 2.0000000000000004, rf1 = 8.5487172896136991e-15, m1 = 1
    // rx2 = 3, rf2 = 1.3322676295501878e-15, m2 = 1
    // rx3 = 4, rf3 = -3.9968028886505620e-15, m3 = 1

    // (x-r0) * (x-r1) * (x-z0) * (x-z0c) [delta < 0]
    p[0] = 40.0;
    p[1] = -14.0;
    p[2] = 1.0;
    p[3] = -4.0;
    p[4] = 1.0;
    std::transform(p.cbegin(), p.cend(), p.begin(), [&multiplier](double v) { return multiplier * v; });
    numRoots = 2;
    dXExpecteds[0] = { 2.0, 1 };
    dXExpecteds[1] = { 4.0, 1 };
    SingleTest<4, 4>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 2, df0 = -8.8817841970012523e-15, m0 = 1
    // dx1 = 3.9999999999999996, df1 = -2.3980817331903378e-14, m1 = 1
    // rx0 = 2, rf0 = -8.6597395920762210e-15, m0 = 1
    // rx1 = 3.9999999999999996, rf1 = -1.9317880628477717e-14, m1 = 1
}

void UnitTestRootsQuartic::TestQuarticSolveDepressedClosedForm()
{
    bool useBisection = false;
    double const dMaxXError = 1e-15;
    double const dMaxFError = 1e-12;
    std::array<double, 5> p{};
    std::size_t numRoots{};
    std::array<PolynomialRoot<double>, 4> dXExpecteds{};
    std::array<PolynomialRoot<double>, 4> dRoots{};
    std::array<double, 4> dFValues{};
    std::array<PolynomialRoot<Rational>, 4> rRoots{};
    std::array<Rational, 4> rFValues{};

    // d0 = 0, d1 = 0, d2 = 0
    p[0] = 0.0;
    p[1] = 0.0;
    p[2] = 0.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.0, 4 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0, df0 = 0, m0 = 4
    // rx0 = 0, rf0 = 0, m0 = 4

    // d0 = 0, d1 = 0, d2 > 0
    p[0] = 0.0;
    p[1] = 0.0;
    p[2] = 1.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.0, 2 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0, df0 = 0, m0 = 2
    // rx0 = 0, r0 = 0, m0 = 2

    // d0 = 0, d1 = 0, d2 < 0
    p[0] = 0.0;
    p[1] = 0.0;
    p[2] = -2.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 3;
    dXExpecteds[0] = { -std::sqrt(2.0), 1 };
    dXExpecteds[1] = { 0.0, 2 };
    dXExpecteds[2] = { std::sqrt(2.0), 1 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.4142135623730951, df0 = 5.4686469261295386e-16, m0 = 1
    // dx1 = 0, df1 = 0, m1 = 2
    // dx2 = +1.4142135623730951, df2 = 5.4686469261295386e-16, m2 = 1
    // rx0 = -1.4142135623730951, rf0 = 5.4686469261295395e-16, m0 = 1
    // rx1 = 0, rf1 = 0, m1 = 2
    // rx2 = +1.4142135623730951, rf2 = 5.4686469261295395e-16, m2 = 1

    // d0 = 0, d1 != 0
    p[0] = 0.0;
    p[1] = 1.0;
    p[2] = -3.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 4;
    dXExpecteds[0] = { -1.8793852415718169, 1 };
    dXExpecteds[1] = { 0.0, 1 };
    dXExpecteds[2] = { 0.34729635533386072, 1 };
    dXExpecteds[3] = { 1.5320888862379560, 1 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.8793852415718169, df0 = 1.1104728932133024e-15, m0 = 1
    // dx1 = 0, df1 = 0, m1 = 1
    // dx2 = 0.34729635533386072, df2 = -3.0759126044386366e-17, m2 = 1
    // dx3 = 1.5320888862379560, df3 = -1.6973006591496277e-16, m3 = 1
    // rx0 = -1.8793852415718169, rf0 = -3.2648099118333953e-16, m0 = 1
    // rx1 = 0, rf1 = 0, m1 = 1
    // rx2 = 0.34729635533386072, rf2 = -1.8904132715305768e-17, m2 = 1
    // rx3 = 1.5320888862379560, rf3 = -1.8904132715305768e-17, m3 = 1

    // biquadratic (4 real roots)
    p[0] = 1.0;
    p[1] = 0.0;
    p[2] = -3.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 4;
    dXExpecteds[0] = { -1.6180339887498949, 1 };
    dXExpecteds[1] = { -0.61803398874989490, 1 };
    dXExpecteds[2] = { 0.61803398874989490, 1 };
    dXExpecteds[3] = { 1.6180339887498949, 1 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.6180339887498949, df0 = 4.1744778811042245e-16, m0 = 1
    // dx1 = -0.61803398874989490, df1 = -2.5869690143265365e-16, m1 = 1
    // dx2 = 0.61803398874989490, df2 = -2.5869690143265365e-16, m2 = 1
    // dx3 = 1.6180339887498949, df3 = 4.1744778811042245e-16, m3 = 1
    // rx0 = -1.6180339887498949, rf0 = 3.9307154875456732e-16, m0 = 1
    // rx1 = -0.61803398874989490, rf1 = -1.5013997161368331e-16, m1 = 1
    // rx2 = 0.61803398874989490, rf2 = -1.5013997161368331e-16, m2 = 1
    // rx3 = 1.6180339887498949, rf3 = 3.9307154875456732e-16, m3 = 1

    // biquadratic (2 complex-conjugate pairs)
    p[0] = 1.0;
    p[1] = 0.0;
    p[2] = 1.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 0;
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // biquadratic (2 real roots, 1 complex-conjugate pair)
    p[0] = -6.0;
    p[1] = 0.0;
    p[2] = 1.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { -std::sqrt(2.0), 1 };
    dXExpecteds[1] = { std::sqrt(2.0), 1 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.4142135623730951, df0 = -2.3198878784481192e-15, m0 = 1
    // dx1 = 1.4142135623730951, df1 = -2.3198878784481192e-15, m1 = 1
    // rx0 = -1.4142135623730951, rf0 = 1.3671617315323848e-15, m0 = 1
    // rx1 = 1.4142135623730951, rf1 = 1.3671617315323848e-15, m1 = 1

    // biquadratic (2 complex-conjugate pairs)
    p[0] = 6.0;
    p[1] = 0.0;
    p[2] = 1.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 0;
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // biquadratic (2 real roots, each of multiplicity 2)
    p[0] = 9.0;
    p[1] = 0.0;
    p[2] = -6.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { -std::sqrt(3.0), 2 };
    dXExpecteds[1] = { std::sqrt(3.0), 2 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // x0 = -1.7320508075688772, f0 = 2.7369146927071510e-16, m0 = 2
    // x1 = +1.7320508075688772, f1 = 2.7369146927071510e-16, m1 = 2
    // x0 = -1.7320508075688772, f0 = 1.2084349840650942e-31, m0 = 2
    // x1 = +1.7320508075688772, f1 = 1.2084349840650942e-31, m1 = 2

    // biquadratic (1 complex-conjugate pair, multiplicity 2)
    p[0] = 1.0;
    p[1] = 0.0;
    p[2] = 2.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 0;
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // (x-r0)^3*(x-r1), r1 < r0 [delta = 0]
    p[0] = -3.0;
    p[1] = 8.0;
    p[2] = -6.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { -3.0, 1 };
    dXExpecteds[1] = { 1.0, 3 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -3, df0 = 0, m0 = 1
    // dx1 = 1, df1 = 0, m1 = 3
    // rx0 = -3, rf0 = 0, m0 = 1
    // rx1 = 1, rf1 = 0, m1 = 3

    // (x-r0)^3*(x-r1), r0 < r1 [delta = 0]
    p[0] = -3.0;
    p[1] = -8.0;
    p[2] = -6.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { -1.0, 3 };
    dXExpecteds[1] = { 3.0, 1 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1, df0 = 0, m0 = 3
    // dx1 = 3, df1 = 0, m1 = 1
    // rx0 = -1, rf0 = 0, m0 = 3
    // rx1 = 3, rf1 = 0, m1 = 1

    // (x-r0)^2 * (x-z0) * (x-z0c) [delta = 0, d2 != 0, qDiscriminant < 0]
    p[0] = 10.0;
    p[1] = -18.0;
    p[2] = 7.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 1.0, 2 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 1, df0 = 0, m0 = 2
    // rx0 = 1, rf0 = 0, m0 = 2

    // (x-r0)^2 * (x-z0) * (x-z0c) [delta = 0, d2 == 0, qDiscriminant < 0]
    p[0] = 3.0;
    p[1] = -4.0;
    p[2] = 0.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 1.0, 2 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 1, df0 = 0, m0 = 2
    // rx0 = 1, rf0 = 0, m0 = 2

    // (x-r0)^2 * (x-r1) * (x-r2) [delta = 0, qDiscriminant > 0]
    p[0] = -8.0;
    p[1] = 18.0;
    p[2] = -11.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 3;
    dXExpecteds[0] = { -4.0, 1 };
    dXExpecteds[1] = { 1.0, 2 };
    dXExpecteds[2] = { 2.0, 1 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -4, df0 = 0, m0 = 1
    // dx1 = 1, df1 = 0, m1 = 2
    // dx2 = 2, df2 = 0, m2 = 1
    // rx0 = -4, rf0 = 0, m0 = 1
    // rx1 = 1, rf1 = 0, m1 = 2
    // rx2 = 2, rf2 = 0, m2 = 1

    // (x-z0) * (x-z0c) * (x-z1) * (x-z1x) [delta > 0, d2 > 0]
    p[0] = 50.0;
    p[1] = -10.0;
    p[2] = 11.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 0;
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // (x-r0) * (x-r1) * (x-r2) * (x-r3) [delta > 0, d2 < 0]
    p[0] = -36.0;
    p[1] = 60.0;
    p[2] = -25.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 4;
    dXExpecteds[0] = { -6.0, 1 };
    dXExpecteds[1] = { 1.0, 1 };
    dXExpecteds[2] = { 2.0, 1 };
    dXExpecteds[3] = { 3.0, 1 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -6, df0 = 0, m0 = 1
    // dx1 = 1, df1 = 9.7699626167013681e-15, m1 = 1
    // dx2 = 1.9999999999999998, df2 = 3.1086244689504375e-15, m2 = 1
    // dx3 = 3, df3 = 0, m3 = 1
    // rx0 = -6, rf0 = 1.1191048088221579e-13, m0 = 1
    // rx1 = 1, rf1 = 8.5487172896136991e-15, m1 = 1
    // rx2 = 1.9999999999999998, rf2 = 1.3322676295501878e-15, m2 = 1
    // rx3 = 3, rf3 = -3.9968028886505620e-15, m3 = 1

    // (x-r0) * (x-r1) * (x-z0) * (x-z0c) [delta < 0]
    p[0] = 24.0;
    p[1] = -20.0;
    p[2] = -5.0;
    p[3] = 0.0;
    p[4] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { 1.0, 1 };
    dXExpecteds[1] = { 3.0, 1 };
    SingleTest<4, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 1.0, df0 = -1.0658141036401503e-14, m0 = 1
    // dx1 = 2.9999999999999996, df1 = -2.3092638912203253e-14, m1 = 1
    // rx0 = 1.0, rf0 = -8.6597395920762210e-15, m0 = 1
    // rx1 = 2.9999999999999996, rf1 = -1.9317880628477717e-14, m1 = 1
}

#else

#include <GTL/Mathematics/RootFinders/RootsQuartic.h>

namespace gtl
{
    template class RootsQuartic<float>;
    template class RootsQuartic<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class RootsQuartic<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(RootsQuartic)
