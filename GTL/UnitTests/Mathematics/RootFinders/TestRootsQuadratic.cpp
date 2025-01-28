#if defined(GTL_UNIT_TESTS)

#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/RootFinders/RootsQuadratic.h>
using namespace gtl;

namespace gtl
{
    class UnitTestRootsQuadratic
    {
    public:
        UnitTestRootsQuadratic();

    private:
        using Rational = BSRational<UIntegerAP32>;

        template <std::size_t Degree, std::size_t NumCoefficients>
        void Solve(bool, double const*, std::size_t, PolynomialRoot<double>*, PolynomialRoot<Rational>*) {}

        template <std::size_t Degree>
        double DEvaluate(double const*, double const&) {}

        template <std::size_t Degree>
        Rational REvaluate(double const*, Rational const&) {}

        template<> void Solve<2, 3>(bool useBisection, double const* p, std::size_t numExpectedRoots,
            PolynomialRoot<double>* dRoots, PolynomialRoot<Rational>* rRoots)
        {
            std::size_t numRoots = RootsQuadratic<double>::Solve(useBisection, p[0], p[1], p[2], dRoots);
            UTAssert(numRoots == numExpectedRoots, "Invalid number of roots (double).");
            numRoots = RootsQuadratic<Rational>::Solve(useBisection, p[0], p[1], p[2], rRoots);
            UTAssert(numRoots == numExpectedRoots, "Invalid number of roots (Rational).");
        }

        template<> void Solve<2, 2>(bool useBisection, double const* p, std::size_t numExpectedRoots,
            PolynomialRoot<double>* dRoots, PolynomialRoot<Rational>* rRoots)
        {
            std::size_t numRoots = RootsQuadratic<double>::Solve(useBisection, p[0], p[1], dRoots);
            UTAssert(numRoots == numExpectedRoots, "Invalid number of roots (double).");
            numRoots = RootsQuadratic<Rational>::Solve(useBisection, p[0], p[1], rRoots);
            UTAssert(numRoots == numExpectedRoots, "Invalid number of roots (Rational).");
        }

        template<> void Solve<2, 1>(bool useBisection, double const* p, std::size_t numExpectedRoots,
            PolynomialRoot<double>* dRoots, PolynomialRoot<Rational>* rRoots)
        {
            std::size_t numRoots = RootsQuadratic<double>::Solve(useBisection, p[0], dRoots);
            UTAssert(numRoots == numExpectedRoots, "Invalid number of roots (double).");
            numRoots = RootsQuadratic<Rational>::Solve(useBisection, p[0], p[1], p[2], rRoots);
            UTAssert(numRoots == numExpectedRoots, "Invalid number of roots (Rational).");
        }

        template<> double DEvaluate<2>(double const* p, double const& x)
        {
            return std::fma(x, std::fma(x, p[2], p[1]), p[0]);
        }

        template<> Rational REvaluate<2>(double const* p, Rational const& rX)
        {
            Rational rP0(p[0]), rP1(p[1]), rP2(p[2]);
            return rP0 + rX * (rP1 + rX * rP2);
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

        void TestQuadraticSolveGeneralBisection();
        void TestQuadraticSolveMonicBisection();
        void TestQuadraticSolveDepressedBisection();
        void TestQuadraticSolveGeneralClosedForm();
        void TestQuadraticSolveMonicClosedForm();
        void TestQuadraticSolveDepressedClosedForm();
    };
}

UnitTestRootsQuadratic::UnitTestRootsQuadratic()
{
    UTInformation("Mathematics/RootFinders/RootsQuadratic");

    TestQuadraticSolveGeneralBisection();
    TestQuadraticSolveMonicBisection();
    TestQuadraticSolveDepressedBisection();
    TestQuadraticSolveGeneralClosedForm();
    TestQuadraticSolveMonicClosedForm();
    TestQuadraticSolveDepressedClosedForm();
}

void UnitTestRootsQuadratic::TestQuadraticSolveGeneralBisection()
{
    bool useBisection = true;
    double const dMaxXError = 1e-15;
    double const dMaxFError = 1e-15;
    std::array<double, 3> p{};
    std::size_t numRoots{};
    std::array<PolynomialRoot<double>, 2> dXExpecteds{};
    std::array<PolynomialRoot<double>, 2> dRoots{};
    std::array<double, 2> dFValues{};
    std::array<PolynomialRoot<Rational>, 2> rRoots{};
    std::array<Rational, 2> rFValues{};

    // Two real roots.
    p[0] = 11.664735862503024;
    p[1] = -10.720274879826544;
    p[2] = -15.003768675975405;
    numRoots = 2;
    dXExpecteds[0] = { -1.3086113690716892, 1 };
    dXExpecteds[1] = { 0.59410589305803607, 1 };
    SingleTest<2, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.3086113690716892, df0 = -8.7267603117833390e-16, m0 = 1
    // dx1 = 0.59410589305803607, df1 = -5.7245145026569683e-16, m1 = 1
    // rx0 = -1.3086113690716892, rf0 = 8.0637954712921642e-16, m0 = 1
    // rx1 = 0.59410589305803607, rf1 = 8.0637954712921642e-16, m1 = 1

    // Two real roots, small coefficients.
    p[0] = -0.00055793191403459021;
    p[1] = -0.00038366589898599344;
    p[2] = 9.4441192735703834e-05;
    numRoots = 2;
    dXExpecteds[0] = { -1.1363534715798598, 1 };
    dXExpecteds[1] = { 5.1988381551404812, 1 };
    SingleTest<2, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.1363534715798598, df0 = -4.6610910918178370e-20, m0 = 1
    // dx1 = 5.1988381551404812, df1 = 2.9800114226410557e-20, m1 = 1
    // rx0 = -1.1363534715798598, rf0 = 8.9233770876771518e-21, m0 = 1
    // rx1 = 5.1988381551404812, rf1 = 8.9233770876771518e-21, m1 = 1

    // Two complex roots.
    double root = -0.72904599140643900;
    p[0] = root * root;
    p[1] = -2.0 * root;
    p[2] = 1.0;
    p[0] += 1e-16;
    numRoots = 0;
    SingleTest<2, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // Two complex roots. The problem is that root * root has rounding errors,
    // so p(x) != (x - root)^2 exactly.
    p[0] = root * root;
    p[1] = -2.0 * root;
    p[2] = 1.0;
    numRoots = 0;
    SingleTest<2, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // One real root, multiplicity 2.
    p[0] = 12.0;
    p[1] = -12.0;
    p[2] = 3.0;
    numRoots = 1;
    dXExpecteds[0] = { 2.0, 2 };
    SingleTest<2, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 2.0, df0 = 0.0, m0 = 2
    // rx0 = 2.0, rf0 = 0.0, m0 = 2

    // Two real roots, slightly perturbed leading coefficient.
    p[0] = root * root;
    p[1] = -2.0 * root;
    p[2] = 1.0;
    p[0] -= 1e-16;
    numRoots = 2;
    dXExpecteds[0] = { -0.72904600039938472, 1 };
    dXExpecteds[1] = { -0.72904598241349328, 1 };
    SingleTest<2, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.72904600039938472, df0 = -5.1593043945409682e-26, m0 = 1
    // dx1 = -0.72904598241349328, df1 = -5.1593043945409682e-26, m1 = 1
    // rx0 = -0.72904600039938472, rf0 = 1.0643459599496777e-32, m0 = 1
    // rx1 = -0.72904598241349328, rf1 = 1.0643459599496777e-32, m1 = 1

    // One real root, multiplicity 2, large leading coefficient.
    p[0] = 1.0;
    p[1] = 200.0;
    p[2] = 10000.0;
    numRoots = 1;
    dXExpecteds[0] = { -0.01, 2 };
    SingleTest<2, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.01, df0 = 0.0, m0 = 2
    // rx0 = -0.01, rf0 = 0.0, m0 = 2

    // Two real roots, one exactly zero.
    p[0] = 0.0;
    p[1] = 10.720274879826544;
    p[2] = -15.003768675975405;
    numRoots = 2;
    dXExpecteds[0] = { 0.0, 1 };
    dXExpecteds[1] = { 0.71450547601365311, 1 };
    SingleTest<2, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0.0, df0 = 0.0, m0 = 1
    // dx1 = 0.71450547601365311, df1 = -2.2519066432726706e-16, m1 = 1
    // rx0 = 0.0, rf0 = 0.0, m0 = 1
    // rx1 = 0.71450547601365311, rf1 = 0.0, m1 = 1

    // Two real roots, one exactly zero (negated coefficients).
    p[0] = 0.0;
    p[1] = -10.720274879826544;
    p[2] = +15.003768675975405;
    numRoots = 2;
    dXExpecteds[0] = { 0.0, 1 };
    dXExpecteds[1] = { 0.71450547601365311, 1 };
    SingleTest<2, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0.0, df0 = 0.0, m0 = 1
    // dx1 = 0.71450547601365311, df1 = 2.2519066432726706e-16, m1 = 1
    // rx0 = 0.0, rf0 = 0.0, m0 = 1
    // rx1 = 0.71450547601365311, rf1 = 0.0, m1 = 1
}

void UnitTestRootsQuadratic::TestQuadraticSolveMonicBisection()
{
    bool useBisection = true;
    double const dMaxXError = 1e-15;
    double const dMaxFError = 1e-14;
    std::array<double, 3> p{};
    std::size_t numRoots{};
    std::array<PolynomialRoot<double>, 2> dXExpecteds{};
    std::array<PolynomialRoot<double>, 2> dRoots{};
    std::array<double, 2> dFValues{};
    std::array<PolynomialRoot<Rational>, 2> rRoots{};
    std::array<Rational, 2> rFValues{};

    // Two real roots.
    p[0] = -11.664735862503024;
    p[1] = 10.720274879826544;
    p[2] = 15.003768675975405;
    p[0] /= p[2];
    p[1] /= p[2];
    p[2] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { -1.3086113690716892, 1 };
    dXExpecteds[1] = { 0.59410589305803607, 1 };
    SingleTest<2, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.3086113690716892, df0 = -8.8192859062923606e-18, m0 = 1
    // dx1 = 0.59410589305803607, df1 = -8.8192859062923606e-18, m1 = 1
    // rx0 = -1.3086113690716892, rf0 = 9.6802739782115659e-17, m0 = 1
    // rx1 = 0.59410589305803607, rf1 = 9.6802739782115659e-17, m1 = 1

    // Two real roots, small coefficients and then monic-ized.
    p[0] = -0.00055793191403459021;
    p[1] = -0.00038366589898599344;
    p[2] = 9.4441192735703834e-05;
    p[0] /= p[2];
    p[1] /= p[2];
    p[2] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { -1.1363534715798596, 1 };
    dXExpecteds[1] = { 5.1988381551404803, 1 };
    SingleTest<2, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.1363534715798596, df0 = -1.7809234183479331e-15, m0 = 1
    // dx1 = 5.1988381551404803, df1 = -2.9352973825752797e-15, m1 = 1
    // rx0 = -1.1363534715798596, rf0 = -1.5286022606957955e-15, m0 = 1
    // rx1 = 5.1988381551404803, rf1 = -1.5286022606957955e-15, m1 = 1

    // Two complex roots.
    double root = -0.72904599140643900;
    p[0] = root * root;
    p[1] = -2.0 * root;
    p[2] = 1.0;
    p[0] += 1e-16;
    numRoots = 0;
    SingleTest<2, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // Two complex roots. The problem is that root * root has rounding errors,
    // so p(x) != (x - root)^2 exactly.
    p[0] = root * root;
    p[1] = -2.0 * root;
    p[2] = 1.0;
    numRoots = 0;
    SingleTest<2, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // One real root, multiplicity 2.
    p[0] = 2.0 * 2.0;
    p[1] = -4.0;
    p[2] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 2.0, 2 };
    SingleTest<2, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 2.0, df0 = 0.0, m0 = 2
    // rx0 = 2.0, rf0 = 0.0, m0 = 2

    // Two real roots, slightly perturbed leading coefficient.
    p[0] = root * root;
    p[1] = -2.0 * root;
    p[2] = 1.0;
    p[0] -= 1e-16;
    numRoots = 2;
    dXExpecteds[0] = { -0.72904600039938472, 1 };
    dXExpecteds[1] = { -0.72904598241349328, 1 };
    SingleTest<2, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.72904600039938472, df0 = -5.1593043945409682e-26, m0 = 1
    // dx1 = -0.72904598241349328, df1 = -5.1593043945409682e-26, m1 = 1
    // rx0 = -0.72904600039938472, rf0 = 1.0643459599496777e-32, m0 = 1
    // rx1 = -0.72904598241349328, rf1 = 1.0643459599496777e-32, m1 = 1

    // One real root, multiplicity 2, large leading coefficient and then
    // monic-ized. However, we get two complex roots. Unlike the original
    // case (p[i] are integers), the floating-point divisions by p[2] cause
    // rounding errors that lead to a discriminant approximately -6*10^{-22}.
    p[0] = 1.0;
    p[1] = 200.0;
    p[2] = 10000.0;
    p[0] /= p[2];
    p[1] /= p[2];
    p[2] = 1.0;
    numRoots = 0;
    SingleTest<2, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // Two real roots, one exactly zero.
    p[0] = 0.0;
    p[1] = 10.720274879826544;
    p[2] = -15.003768675975405;
    p[0] /= p[2];
    p[1] /= p[2];
    p[2] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { 0.0, 1 };
    dXExpecteds[1] = { 0.71450547601365311, 1 };
    SingleTest<2, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0, df0 = 0, m0 = 1
    // dx1 = 0.71450547601365311, rf1 = 0, m1 = 1
    // rx0 = 0, rf0 = 0, m0 = 1
    // rx1 = 0.71450547601365311, rf1 = 0, m1 = 1

    // Two real roots, one exactly zero (negated coefficients).
    p[0] = 0.0;
    p[1] = -10.720274879826544;
    p[2] = +15.003768675975405;
    p[0] /= p[2];
    p[1] /= p[2];
    p[2] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { 0.0, 1 };
    dXExpecteds[1] = { 0.71450547601365311, 1 };
    SingleTest<2, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0, df0 = 0, m0 = 1
    // dx1 = 0.71450547601365311, rf1 = 0, m1 = 1
    // rx0 = 0, df0 = 0, m0 = 1
    // rx1 = 0.71450547601365311, rf1 = 0, m1 = 1
}

void UnitTestRootsQuadratic::TestQuadraticSolveDepressedBisection()
{
    bool useBisection = true;
    double const dMaxXError = 1e-15;
    double const dMaxFError = 1e-14;
    std::array<double, 3> p{};
    std::size_t numRoots{};
    std::array<PolynomialRoot<double>, 2> dXExpecteds{};
    std::array<PolynomialRoot<double>, 2> dRoots{};
    std::array<double, 2> dFValues{};
    std::array<PolynomialRoot<Rational>, 2> rRoots{};
    std::array<Rational, 2> rFValues{};

    // Two complex roots.
    p[0] = 1.0;
    p[1] = 0.0;
    p[2] = 1.0;
    numRoots = 0;
    SingleTest<2, 1>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // Two real roots.
    p[0] = -17.123456;
    p[1] = 0.0;
    p[2] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { -4.1380497822041722, 1 };
    dXExpecteds[1] = { 4.1380497822041722, 1 };
    SingleTest<2, 1>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -4.1380497822041722, df0 = -4.0105574152098984e-15, m0 = 1
    // dx1 = 4.1380497822041722, df1 = -4.0105574152098984e-15, m1 = 1
    // rx0 = -4.1380497822041722, rf0 = -3.3523089901134919e-16, m0 = 1
    // rx1 = 4.1380497822041722, rf1 = -3.3523089901134919e-16, m1 = 1

    // Two real roots, constant coefficient nearly zero.
    p[0] = -1.0e-16;
    p[1] = 0.0;
    p[2] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { -1.0000000000000000e-08, 1 };
    dXExpecteds[1] = { 1.0000000000000000e-08, 1 };
    SingleTest<2, 1>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.0000000000000000e-08, df0 = 6.2747334936222342e-33, m0 = 1
    // dx1 = 1.0000000000000000e-08, df1 = 6.2747334936222342e-33, m1 = 1
    // rx0 = -1.0000000000000000e-08, rf0 = -1.0268878757438320e-32, m0 = 1
    // rx1 = 1.0000000000000000e-08, rf1 = -1.0268878757438320e-32, m1 = 1

    // One real root, multiplicity 2 (both roots zero).
    p[0] = 0.0;
    p[1] = 0.0;
    p[2] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.0, 2 };
    SingleTest<2, 1>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0.0, df0 = 0.0, m0 = 2
    // rx0 = 0.0, rf0 = 0.0, m0 = 2
}

void UnitTestRootsQuadratic::TestQuadraticSolveGeneralClosedForm()
{
    bool useBisection = false;
    double const dMaxXError = 1e-15;
    double const dMaxFError = 1e-15;
    std::array<double, 3> p{};
    std::size_t numRoots{};
    std::array<PolynomialRoot<double>, 2> dXExpecteds{};
    std::array<PolynomialRoot<double>, 2> dRoots{};
    std::array<double, 2> dFValues{};
    std::array<PolynomialRoot<Rational>, 2> rRoots{};
    std::array<Rational, 2> rFValues{};

    // Two real roots.
    p[0] = 11.664735862503024;
    p[1] = -10.720274879826544;
    p[2] = -15.003768675975405;
    numRoots = 2;
    dXExpecteds[0] = { -1.3086113690716892, 1 };
    dXExpecteds[1] = { 0.59410589305803607, 1 };
    SingleTest<2, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.3086113690716892, df0 = -8.7267603117833390e-16, m0 = 1
    // dx1 = 0.59410589305803607, df1 = -5.7245145026569683e-16, m1 = 1
    // rx0 = -1.3086113690716892, rf0 = -7.7834889338758933e-16, m0 = 1
    // rx1 = 0.59410589305803607, rf1 = -7.7834889338758933e-16, m1 = 1

    // Two real roots, small coefficients.
    p[0] = -0.00055793191403459021;
    p[1] = -0.00038366589898599344;
    p[2] = 9.4441192735703834e-05;
    numRoots = 2;
    dXExpecteds[0] = { -1.1363534715798596, 1 };
    dXExpecteds[1] = { 5.1988381551404812, 1 };
    SingleTest<2, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.1363534715798596, df0 = -1.5563136496285170e-19, m0 = 1
    // dx1 = 5.1988381551404812, df1 = 2.9800114226410557e-20, m1 = 1
    // rx0 = -1.1363534715798596, rf0 = -1.2392658803811765e-19, m0 = 1
    // rx1 = 5.1988381551404812, rf1 = -1.2392658803811765e-19, m1 = 1

    // Two complex roots.
    double root = -0.72904599140643900;
    p[0] = root * root;
    p[1] = -2.0 * root;
    p[2] = 1.0;
    p[0] += 1e-16;
    numRoots = 0;
    SingleTest<2, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // Two complex roots. The problem is that root * root has rounding errors,
    // so p(x) != (x - root)^2 exactly.
    p[0] = root * root;
    p[1] = -2.0 * root;
    p[2] = 1.0;
    numRoots = 0;
    SingleTest<2, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // One real root, multiplicity 2.
    p[0] = 12.0;
    p[1] = -12.0;
    p[2] = 3.0;
    numRoots = 1;
    dXExpecteds[0] = { 2.0, 2 };
    SingleTest<2, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 2.0, df0 = 0.0, m0 = 2
    // rx0 = 2.0, rf0 = 0.0, m0 = 2

    // Two real roots, slightly perturbed leading coefficient.
    p[0] = root * root;
    p[1] = -2.0 * root;
    p[2] = 1.0;
    p[0] -= 1e-16;
    numRoots = 2;
    dXExpecteds[0] = { -0.72904600039938472, 1 };
    dXExpecteds[1] = { -0.72904598241349328, 1 };
    SingleTest<2, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.72904600039938472, df0 = -5.1593043945409682e-26, m0 = 1
    // dx1 = -0.72904598241349328, df1 = -5.1593043945409682e-26, m1 = 1
    // rx0 = -0.72904600039938472, rf0 = -4.2341211062677455e-33, m0 = 1
    // rx1 = -0.72904598241349328, rf1 = -4.2341211062677455e-33, m1 = 1

    // One real root, multiplicity 2, large leading coefficient.
    p[0] = 1.0;
    p[1] = 200.0;
    p[2] = 10000.0;
    numRoots = 1;
    dXExpecteds[0] = { -0.01, 2 };
    SingleTest<2, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.01, df0 = 0.0, m0 = 2
    // rx0 = -0.01, rf0 = 0.0, m0 = 2

    // Two real roots, one exactly zero.
    p[0] = 0.0;
    p[1] = 10.720274879826544;
    p[2] = -15.003768675975405;
    numRoots = 2;
    dXExpecteds[0] = { 0.0, 1 };
    dXExpecteds[1] = { 0.71450547601365311, 1 };
    SingleTest<2, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0.0, df0 = 0.0, m0 = 1
    // dx1 = 0.71450547601365311, df1 = -2.2519066432726706e-16, m1 = 1
    // rx0 = 0.0, rf0 = 0.0, m0 = 1
    // rx1 = 0.71450547601365311, rf1 = 0.0, m1 = 1

    // Two real roots, one exactly zero (negated coefficients).
    p[0] = 0.0;
    p[1] = -10.720274879826544;
    p[2] = +15.003768675975405;
    numRoots = 2;
    dXExpecteds[0] = { 0.0, 1 };
    dXExpecteds[1] = { 0.71450547601365311, 1 };
    SingleTest<2, 3>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0.0, df0 = 0.0, m0 = 1
    // dx1 = 0.71450547601365311, df1 = 2.2519066432726706e-16, m1 = 1
    // rx0 = 0.0, rf0 = 0.0, m0 = 1
    // rx1 = 0.71450547601365311, rf1 = 0.0, m1 = 1
}

void UnitTestRootsQuadratic::TestQuadraticSolveMonicClosedForm()
{
    bool useBisection = false;
    double const dMaxXError = 1e-15;
    double const dMaxFError = 1e-14;
    std::array<double, 3> p{};
    std::size_t numRoots{};
    std::array<PolynomialRoot<double>, 2> dXExpecteds{};
    std::array<PolynomialRoot<double>, 2> dRoots{};
    std::array<double, 2> dFValues{};
    std::array<PolynomialRoot<Rational>, 2> rRoots{};
    std::array<Rational, 2> rFValues{};

    // Two real roots.
    p[0] = -11.664735862503024;
    p[1] = 10.720274879826544;
    p[2] = 15.003768675975405;
    p[0] /= p[2];
    p[1] /= p[2];
    p[2] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { -1.3086113690716892, 1 };
    dXExpecteds[1] = { 0.59410589305803607, 1 };
    SingleTest<2, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.3086113690716892, df0 = -8.8192859062923606e-18, m0 = 1
    // dx1 = 0.59410589305803607, df1 = -8.8192859062923606e-18, m1 = 1
    // rx0 = -1.3086113690716892, rf0 = -8.8192859062923606e-18, m0 = 1
    // rx1 = 0.59410589305803607, rf1 = -8.8192859062923606e-18, m1 = 1

    // Two real roots, small coefficients and then monic-ized.
    p[0] = -0.00055793191403459021;
    p[1] = -0.00038366589898599344;
    p[2] = 9.4441192735703834e-05;
    p[0] /= p[2];
    p[1] /= p[2];
    p[2] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { -1.1363534715798598, 1 };
    dXExpecteds[1] = { 5.1988381551404803, 1 };
    SingleTest<2, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.1363534715798598, df0 = -6.2654945412058653e-16, m0 = 1
    // dx1 = 5.1988381551404803, df1 = -2.9352973825752797e-15, m1 = 1
    // rx0 = -1.1363534715798598, rf0 = -1.2190713881631102e-16, m0 = 1
    // rx1 = 5.1988381551404803, rf1 = -1.2190713881631102e-16, m1 = 1

    // Two complex roots.
    double root = -0.72904599140643900;
    p[0] = root * root;
    p[1] = -2.0 * root;
    p[2] = 1.0;
    p[0] += 1e-16;
    numRoots = 0;
    SingleTest<2, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // Two complex roots. The problem is that root * root has rounding errors,
    // so p(x) != (x - root)^2 exactly.
    p[0] = root * root;
    p[1] = -2.0 * root;
    p[2] = 1.0;
    numRoots = 0;
    SingleTest<2, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // One real root, multiplicity 2.
    p[0] = 2.0 * 2.0;
    p[1] = -4.0;
    p[2] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 2.0, 2 };
    SingleTest<2, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 2.0, df0 = 0.0, m0 = 2
    // rx0 = 2.0, rf0 = 0.0, m0 = 2

    // Two real roots, slightly perturbed leading coefficient.
    p[0] = root * root;
    p[1] = -2.0 * root;
    p[2] = 1.0;
    p[0] -= 1e-16;
    numRoots = 2;
    dXExpecteds[0] = { -0.72904600039938472, 1 };
    dXExpecteds[1] = { -0.72904598241349328, 1 };
    SingleTest<2, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -0.72904600039938472, df0 = -5.1593043945409682e-26, m0 = 1
    // dx1 = -0.72904598241349328, df1 = -5.1593043945409682e-26, m1 = 1
    // rx0 = -0.72904600039938472, rf0 = -4.2341211062677455e-33, m0 = 1
    // rx1 = -0.72904598241349328, rf1 = -4.2341211062677455e-33, m1 = 1

    // One real root, multiplicity 2, large leading coefficient and then
    // monic-ized. However, we get two complex roots. Unlike the original
    // case (p[i] are integers), the floating-point divisions by p[2] cause
    // rounding errors that lead to a discriminant approximately -6*10^{-22}.
    p[0] = 1.0;
    p[1] = 200.0;
    p[2] = 10000.0;
    p[0] /= p[2];
    p[1] /= p[2];
    p[2] = 1.0;
    numRoots = 0;
    SingleTest<2, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // Two real roots, one exactly zero.
    p[0] = 0.0;
    p[1] = 10.720274879826544;
    p[2] = -15.003768675975405;
    p[0] /= p[2];
    p[1] /= p[2];
    p[2] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { 0.0, 1 };
    dXExpecteds[1] = { 0.71450547601365311, 1 };
    SingleTest<2, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0, df0 = 0, m0 = 1
    // dx1 = 0.71450547601365311, rf1 = 0, m1 = 1
    // rx0 = 0, rf0 = 0, m0 = 1
    // rx1 = 0.71450547601365311, rf1 = 0, m1 = 1

    // Two real roots, one exactly zero (negated coefficients).
    p[0] = 0.0;
    p[1] = -10.720274879826544;
    p[2] = +15.003768675975405;
    p[0] /= p[2];
    p[1] /= p[2];
    p[2] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { 0.0, 1 };
    dXExpecteds[1] = { 0.71450547601365311, 1 };
    SingleTest<2, 2>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0, df0 = 0, m0 = 1
    // dx1 = 0.71450547601365311, rf1 = 0, m1 = 1
    // rx0 = 0, df0 = 0, m0 = 1
    // rx1 = 0.71450547601365311, rf1 = 0, m1 = 1
}

void UnitTestRootsQuadratic::TestQuadraticSolveDepressedClosedForm()
{
    bool useBisection = false;
    double const dMaxXError = 1e-15;
    double const dMaxFError = 1e-14;
    std::array<double, 3> p{};
    std::size_t numRoots{};
    std::array<PolynomialRoot<double>, 2> dXExpecteds{};
    std::array<PolynomialRoot<double>, 2> dRoots{};
    std::array<double, 2> dFValues{};
    std::array<PolynomialRoot<Rational>, 2> rRoots{};
    std::array<Rational, 2> rFValues{};

    // Two complex roots.
    p[0] = 1.0;
    p[1] = 0.0;
    p[2] = 1.0;
    numRoots = 0;
    SingleTest<2, 1>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);

    // Two real roots.
    p[0] = -17.123456;
    p[1] = 0.0;
    p[2] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { -4.1380497822041731, 1 };
    dXExpecteds[1] = { 4.1380497822041731, 1 };
    SingleTest<2, 1>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -4.1380497822041731, df0 = 3.3400956171872005e-15, m0 = 1
    // dx1 = 4.1380497822041731, df1 = 3.3400956171872005e-15, m1 = 1
    // rx0 = -4.1380497822041731, rf0 = 3.3400956171872005e-15, m0 = 1
    // rx1 = 4.1380497822041731, rf1 = 3.3400956171872005e-15, m1 = 1

    // Two real roots, constant coefficient nearly zero.
    p[0] = -1.0e-16;
    p[1] = 0.0;
    p[2] = 1.0;
    numRoots = 2;
    dXExpecteds[0] = { -1.0000000000000000e-08, 1 };
    dXExpecteds[1] = { 1.0000000000000000e-08, 1 };
    SingleTest<2, 1>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = -1.0000000000000000e-08, df0 = 6.2747334936222342e-33, m0 = 1
    // dx1 = 1.0000000000000000e-08, df1 = 6.2747334936222342e-33, m1 = 1
    // rx0 = -1.0000000000000000e-08, rf0 = 6.2747334936222342e-33, m0 = 1
    // rx1 = 1.0000000000000000e-08, rf1 = 6.2747334936222342e-33, m1 = 1

    // One real root, multiplicity 2 (both roots zero).
    p[0] = 0.0;
    p[1] = 0.0;
    p[2] = 1.0;
    numRoots = 1;
    dXExpecteds[0] = { 0.0, 2 };
    SingleTest<2, 1>(useBisection, dMaxXError, dMaxFError,
        p, numRoots, dXExpecteds, dRoots, dFValues, rRoots, rFValues);
    // dx0 = 0.0, df0 = 0.0, m0 = 2
    // rx0 = 0.0, rf0 = 0.0, m0 = 2
}

#else

#include <GTL/Mathematics/RootFinders/RootsQuadratic.h>

namespace gtl
{
    template class RootsQuadratic<float>;
    template class RootsQuadratic<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class RootsQuadratic<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(RootsQuadratic)
