// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Utility/Timer.h>
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#include <GTL/Mathematics/Algebra/Polynomial.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestPolynomial
    {
    public:
        UnitTestPolynomial();

    private:
        using BSR = BSRational<UIntegerAP32>;
        using BSN = BSNumber<UIntegerAP32>;

        void Constructors();
        void Accessors();

        void DoComparisons(Polynomial<float, 1> const& p0, Polynomial<float, 1> const& p1,
            bool equal, bool lessThan)
        {
            bool result = (p0 == p1);
            UTAssert(result == equal, "Incorrect operator==");
            result = (p0 < p1);
            UTAssert(result == lessThan, "Incorrect operator<");
            UTAssert((p0 != p1) == !equal, "Incorrect operator!=");
            UTAssert((p0 <= p1) == equal || lessThan, "Incorrect operator<=");
            UTAssert((p0 > p1) == (!equal && !lessThan), "Incorrect operator>");
            UTAssert((p0 >= p1) == !lessThan, "Incorrect operator>=");
        }

        void Comparisons();
        void AlgebraicOperations();
        void BSROperations();
        void BSNOperations();

        void TestPolynomial2();

        void TestPerformanceBSR(std::size_t degree);
        void TestPerformanceBSN(std::size_t degree);
        void TestPerformanceRepeatedBSR(std::size_t numFactors);
        void TestPerformanceRepeatedBSN(std::size_t numFactors);
    };

    extern std::atomic<std::size_t> gsUIntegerAP32MaxBlocks;
}

UnitTestPolynomial::UnitTestPolynomial()
{
    UTInformation("Mathematics/Algebra/Polynomial");

    Constructors();
    Accessors();
    Comparisons();
    AlgebraicOperations();
    BSROperations();
    BSNOperations();
    TestPolynomial2();

    // These all turn out to have numFactors of 1.
    //for (std::size_t degree = 2; degree <= 8; ++degree)
    //{
    //    TestPerformanceBSR(degree);
    //}

    // These all turn out to have numFactors of 1.
    //for (std::size_t degree = 5; degree <= 16; ++degree)
    //{
    //    TestPerformanceBSN(degree);
    //}

    //TestPerformanceRepeatedBSR(2);

    // Terminated the call after 10 minutes when numFactors is 3.
    // The degree of the test polynomial is 14, which the call to
    // TestPerformanceBSR is extremely slow (that test stops at
    // degree 8).
    //TestPerformanceRepeatedBSR(3);

    //TestPerformanceRepeatedBSN(2);

    // This call also takes too long (terminated/gave-up).
    //TestPerformanceRepeatedBSN(3);
}

void UnitTestPolynomial::Constructors()
{
    Polynomial<float, 1> pzero;
    UTAssert(pzero.GetDegree() == 0 && pzero[0] == 0.0f, "Default constructor failed");

    Polynomial<float, 1> pzero2(2);
    UTAssert(pzero2.GetDegree() == 2
        && pzero2[0] == 0.0f && pzero2[1] == 0.0f && pzero2[2] == 0.0f,
        "Degree constructor failed");

    Polynomial<float, 1> plist{ 1.0f, 2.0f, 3.0f };
    UTAssert(plist.GetDegree() == 2
        && plist[0] == 1.0f && plist[1] == 2.0f && plist[2] == 3.0f,
        "Initializer constructor failed");

    plist.SetDegree(3);
    plist[3] = 4.0f;
    UTAssert(plist.GetDegree() == 3
        && plist[0] == 1.0f && plist[1] == 2.0f && plist[2] == 3.0f && plist[3] == 4.0f,
        "SetDegree failed");

    plist.SetDegree(1);
    UTAssert(plist.GetDegree() == 1
        && plist[0] == 1.0f && plist[1] == 2.0f,
        "SetDegree failed");

    plist.SetDegree(3);
    plist[2] = 3.0f;
    plist[3] = 4.0f;
    float value = plist(-2.0f);
    UTAssert(value == -23.0f, "operator()(T const&) failed");
}

void UnitTestPolynomial::Accessors()
{
    Polynomial<float, 1> p{ 1.0f, 2.0f, 3.0f };
}

void UnitTestPolynomial::Comparisons()
{
    Polynomial<float, 1> p0 = { 1.0f, 2.0f };
    Polynomial<float, 1> p1 = { 3.0f, 4.0f };
    Polynomial<float, 1> p2 = { 2.0f, 5.0f };
    Polynomial<float, 1> p3 = { 1.0f, 2.0f, 3.0f };

    DoComparisons(p0, p0, true, false);
    DoComparisons(p0, p1, false, true);
    DoComparisons(p1, p2, false, false);
    DoComparisons(p0, p3, false, true);
    DoComparisons(p3, p0, false, false);
}

void UnitTestPolynomial::AlgebraicOperations()
{
    Polynomial<float, 1> p = { 1.0f, 2.0f };
    p = +p;
    UTAssert(p[0] == 1.0f && p[1] == 2.0f, "unary positive failed");
    p = -p;
    UTAssert(p[0] == -1.0f && p[1] == -2.0f, "unary negation failed");

    Polynomial<float, 1> p0 = { 1.0f, 2.0f, 3.0f };
    Polynomial<float, 1> p1 = { 2.0f, -1.0f, -4.0f, 1.0f };
    Polynomial<float, 1> p2, p3;

    p2 = p0 + p1;
    UTAssert(p2.GetDegree() == 3
        && p2[0] == 3.0f && p2[1] == 1.0f && p2[2] == -1.0f && p2[3] == 1.0f,
        "p0 + p1 failed");

    p2 = p1 + p0;
    UTAssert(p2.GetDegree() == 3
        && p2[0] == 3.0f && p2[1] == 1.0f && p2[2] == -1.0f && p2[3] == 1.0f,
        "p1 + p0 failed");

    p2 = p0 - p1;
    UTAssert(p2.GetDegree() == 3
        && p2[0] == -1.0f && p2[1] == 3.0f && p2[2] == 7.0f && p2[3] == -1.0f,
        "p0 - p1 failed");

    p2 = p1 + 1.0f;
    UTAssert(p2.GetDegree() == 3
        && p2[0] == 3.0f && p2[1] == -1.0f && p2[2] == -4.0f && p2[3] == 1.0f,
        "p + s failed");

    p2 = 1.0f + p1;
    UTAssert(p2.GetDegree() == 3
        && p2[0] == 3.0f && p2[1] == -1.0f && p2[2] == -4.0f && p2[3] == 1.0f,
        "s + p failed");

    p2 = p1 - 1.0f;
    UTAssert(p2.GetDegree() == 3
        && p2[0] == 1.0f && p2[1] == -1.0f && p2[2] == -4.0f && p2[3] == 1.0f,
        "p - s failed");

    p2 = 1.0f - p1;
    UTAssert(p2.GetDegree() == 3
        && p2[0] == -1.0f && p2[1] == 1.0f && p2[2] == 4.0f && p2[3] == -1.0f,
        "s - p failed");

    p2 = p1 * 2.0f;
    UTAssert(p2.GetDegree() == 3
        && p2[0] == 4.0f && p2[1] == -2.0f && p2[2] == -8.0f && p2[3] == 2.0f,
        "p * s failed");

    p2 = 2.0f * p1;
    UTAssert(p2.GetDegree() == 3
        && p2[0] == 4.0f && p2[1] == -2.0f && p2[2] == -8.0f && p2[3] == 2.0f,
        "s * p failed");

    p2 = p1 / 2.0f;
    UTAssert(p2.GetDegree() == 3
        && p2[0] == 1.0f && p2[1] == -0.5f && p2[2] == -2.0f && p2[3] == 0.5f,
        "p / s failed");

    p2 = p0;
    p2 += p1;
    UTAssert(p2.GetDegree() == 3
        && p2[0] == 3.0f && p2[1] == 1.0f && p2[2] == -1.0f && p2[3] == 1.0f,
        "p0 += p1 failed");

    p2 = p0;
    p2 -= p1;
    UTAssert(p2.GetDegree() == 3
        && p2[0] == -1.0f && p2[1] == 3.0f && p2[2] == 7.0f && p2[3] == -1.0f,
        "p0 -= p1 failed");

    p2 = p1;
    p2 += 1.0f;
    UTAssert(p2.GetDegree() == 3
        && p2[0] == 3.0f && p2[1] == -1.0f && p2[2] == -4.0f && p2[3] == 1.0f,
        "p += s failed");

    p2 = p1;
    p2 -= 1.0f;
    UTAssert(p2.GetDegree() == 3
        && p2[0] == 1.0f && p2[1] == -1.0f && p2[2] == -4.0f && p2[3] == 1.0f,
        "p -= s failed");

    p2 = p1;
    p2 *= 2.0f;
    UTAssert(p2.GetDegree() == 3
        && p2[0] == 4.0f && p2[1] == -2.0f && p2[2] == -8.0f && p2[3] == 2.0f,
        "p *= s failed");

    p2 = p1;
    p2 /= 2.0f;
    UTAssert(p2.GetDegree() == 3
        && p2[0] == 1.0f && p2[1] == -0.5f && p2[2] == -2.0f && p2[3] == 0.5f,
        "p /= s failed");

    p2 = p0 * p1;
    UTAssert(p2.GetDegree() == 5
        && p2[0] == 2.0f && p2[1] == 3.0f && p2[2] == 0.0f && p2[3] == -10.0f
        && p2[4] == -10.0f && p2[5] == 3.0f,
        "p0 * p1 failed");

    UTAssert(p2.GetDegree() == 5
        && p2[0] == 2.0f && p2[1] == 3.0f && p2[2] == 0.0f && p2[3] == -10.0f
        && p2[4] == -10.0f && p2[5] == 3.0f,
        "p0 * p1 failed");

    p3 = p0;
    p3 *= p1;
    UTAssert(p2.GetDegree() == 5
        && p3[0] == 2.0f && p3[1] == 3.0f && p3[2] == 0.0f && p3[3] == -10.0f
        && p3[4] == -10.0f && p3[5] == 3.0f,
        "p3 *= p1 failed");

    Fill(p, -1.0f);
    UTAssert(p[0] == -1.0f && p[1] == -1.0f, "Fill failed");

    p2[5] = 0.0f;
    p2[4] = 0.0f;
    p2[3] = 0.0f;
    EliminateLeadingZeros(p2);
    UTAssert(p2.GetDegree() == 1
        && p2[0] == 2.0f && p2[1] == 3.0f,
        "EliminateLeadingZeros failed");

    float const epsilon = 1e-7f;
    p2 = p0 * p1;
    MakeMonic(p2);
    UTAssert(p2.GetDegree() == 5 &&
        std::fabs(p2[0] - 2.0f / 3.0f) <= epsilon &&
        std::fabs(p2[1] - 1.0f) <= epsilon &&
        std::fabs(p2[2] - 0.0f) <= epsilon &&
        std::fabs(p2[3] + 10.0f / 3.0f) <= epsilon &&
        std::fabs(p2[4] + 10.0f / 3.0f) <= epsilon &&
        std::fabs(p2[5] - 1.0f) <= 1e-8f,
        "MakeMonic failed");

    p2.SetDegree(3);
    p2[0] = 1.0f;
    p2[1] = 2.0f;
    p2[2] = 3.0f;
    p2[3] = 4.0f;
    p = GetDerivative(p2);
    UTAssert(p.GetDegree() == 2 && p[0] == 2.0f && p[1] == 6.0f && p[2] == 12.0f,
        "GetDerivative failed");

    p2[0] = 0.0f;
    p2[1] = 1.0f;
    p2[2] = 2.0f;
    p2[3] = 3.0f;
    p = GetInversion(p2);
    UTAssert(p.GetDegree() == 2 && p[0] == 3.0f && p[1] == 2.0f && p[2] == 1.0f,
        "GetInversion failed");

    p = GetTranslation(p2, 2.0f);
    UTAssert(p.GetDegree() == 3 && p[0] == -18.0f && p[1] == 29.0f && p[2] == -16.0f
        && p[3] == 3.0f,
        "GetTranslation failed");
}

void UnitTestPolynomial::BSROperations()
{
    Polynomial1<BSR> poly0{ -4.0f, 3.0f };
    Polynomial1<BSR> poly1{ 1.0f, 1.0f, 2.0f };
    Polynomial1<BSR> p0 = poly0 * poly0 * poly1;
    Polynomial1<BSR> p1{ 2.0, 3.0, 4.0 };
    Polynomial1<BSR> q, r, test, g, q0, q1;
    bool isZero;

    // p0 = 16 - 8 * x + 17 * x^2 - 39 * x^3 + 18 * x^4
    // p1 = 2 + 3 * x + 4 * x^2
    // q  = 11.84375 - 13.125 * x + 4.5 * x^2
    //    = (379/32) - (105/8) * x + (9/2) * x^2
    // r  = -7.6875 - 17.28125 * x
    //    = -(123/16) - (553/32) * x
    // p0 = q * p1 + r

    GetQR(p0, p1, q, r);
    test = p0 - q * p1 - r;
    isZero = IsZero(test);
    UTAssert(isZero,
        "Expecting the zero polynomial.");

    GetQR(p0, poly0, q, r);
    test = p0 - q * poly0 - r;
    isZero = (test == Polynomial1<BSR>{});
    UTAssert(isZero,
        "Expecting the zero polynomial.");

    GetGCD(p0, p1, g);
    UTAssert(g.GetDegree() == 0 && g[0] == BSR(1),
        "Expecting a GCD of 1.");

    GetGCD(p0, poly0, g);
    UTAssert(g == poly0,
        "Expecting g to be poly0.");

    // g = 1, q0 = p0, q1 = p1
    GetExtendedGCD(p0, p1, g, q0, q1);
    UTAssert(g.GetDegree() == 0 && g[0] == BSR(1),
        "Expecting a GCD of 1.");
    test = p0 - q0 * g;
    isZero = (test == Polynomial1<BSR>{});
    UTAssert(isZero,
        "Expecting the zero polynomial.");
    test = p1 - q1 * g;
    isZero = (test == Polynomial1<BSR>{});
    UTAssert(isZero,
        "Expecting the zero polynomial.");

    GetExtendedGCD(p0, poly0, g, q0, q1);
    UTAssert(g == poly0,
        "Expecting g to be poly0.");
    test = p0 - q0 * g;
    isZero = (test == Polynomial1<BSR>{});
    UTAssert(isZero,
        "Expecting the zero polynomial.");
    test = poly0 - q1 * g;
    isZero = (test == Polynomial1<BSR>{});
    UTAssert(isZero,
        "Expecting the zero polynomial.");

    std::default_random_engine dre;
    std::uniform_real_distribution<float> urd(-1.0f, 1.0f);
    Polynomial1<BSR> input;
    input.SetDegree(6);
    for (std::size_t i = 0; i <= input.GetDegree(); ++i)
    {
        input[i] = urd(dre);
    }

    std::vector<Polynomial1<BSR>> factors;
    GetSquareFreeFactors(input, factors);
    UTAssert(factors.size() == 1 && factors[0] == input,
        "Expecting input to be square free.");

    p0 = Polynomial1<BSR>{ -1, 2 };
    p1 = Polynomial1<BSR>{ 6, 5, 4, 3 };
    Polynomial1<BSR> u = p0 * p0 * p1;
    GetSquareFreeFactors(u, factors);
    UTAssert(factors.size() == 3 &&
        factors[0].GetDegree() == 0 &&
        factors[1].GetDegree() == 3 &&
        factors[2].GetDegree() == 1,
        "Unexpected degrees for the factors.");
    test = factors[1] * factors[2] * factors[2];
    test *= u[0] / test[0];
    test -= u;
    isZero = (test == Polynomial1<BSR>{});
    UTAssert(isZero,
        "Expecting the zero polynomial.");
}

void UnitTestPolynomial::BSNOperations()
{
    Polynomial1<BSN> poly0{ -4.0f, 3.0f };
    Polynomial1<BSN> poly1{ 1.0f, 1.0f, 2.0f };
    Polynomial1<BSN> p0 = poly0 * poly0 * poly1;
    Polynomial1<BSN> p1{ 2.0, 3.0, 4.0 };
    Polynomial1<BSN> q, r, test, g, q0, q1, u, w;
    BSN d, d2, d3, d4, a0, a1;
    bool isZero;

    // p0 = 16 - 8 * x + 17 * x^2 - 39 * x^3 + 18 * x^4
    // p1 = 2 + 3 * x + 4 * x^2
    // a0 = 64
    // q  = 758 - 840 * x + 288 * x^2
    // r  = -492 - 1106 * x
    // a0 * p0 = q * p1 + r
    GetPseudoQR(p0, p1, a0, q, r);
    test = a0 * p0 - q * p1 - r;
    isZero = (test == Polynomial1<BSN>{});
    UTAssert(isZero,
        "Expecting the zero polynomial.");

    // p0 = 16 - 8 * x + 17 * x^2 - 39 * x^3 + 18 * x^4
    // poly0 = -4 + 3 * x
    // a0 = 81
    // q = -324 - 81 * x - 405 * x^2 + 486 * x^3
    // r = 0
    // a0 * p0 = q * p1 + r
    GetPseudoQR(p0, poly0, a0, q, r);
    test = a0 * p0 - q * poly0 - r;
    isZero = (test == Polynomial1<BSN>{});
    UTAssert(isZero,
        "Expecting the zero polynomial.");

    // u0 = 1 + x + x^2 + x^3
    // u1 = 3 - 2*x - 2*x^2
    // a0 = -2
    // q = 0 + 1 * x
    // r = -2 - 5 * x
    // a0 * p0 = q * p1 + r
    Polynomial1<BSN> u0{ 1, 1, 1, 1 };
    Polynomial1<BSN> u1{ 3, -2, -2 };
    GetPseudoQR(u0, u1, a0, q, r);
    test = a0 * u0 - q * u1 - r;
    isZero = (test == Polynomial1<BSN>{});
    UTAssert(isZero,
        "Expecting the zero polynomial.");

    // u0 = 1 + x + x^2 + x^3
    // u1 = 3 - 2*x^2 - 2*x^3
    // a0 = -2
    // q = 1
    // r = -5 - 2 * x
    // a0 * p0 = q * p1 + r
    u0 = Polynomial1<BSN>{ 1, 1, 1, 1 };
    u1 = Polynomial1<BSN>{ 3, 0, -2, -2 };
    GetPseudoQR(u0, u1, a0, q, r);
    test = a0 * u0 - q * u1 - r;
    isZero = (test == Polynomial1<BSN>{});
    UTAssert(isZero,
        "Expecting the zero polynomial.");

    // u0 = 2 + x + x^2 + 2*x^3 + 2*x^5
    // u1 = 1 + 2*x^3
    // a0 = 4
    // q = 4 + 4*x^2
    // r = 4 + 4*x
    // a0 * p0 = q * p1 + r
    u0 = Polynomial1<BSN>{ 2, 1, 1, 2, 0, 2 };
    u1 = Polynomial1<BSN>{ 1, 0, 0, 2 };
    GetPseudoQR(u0, u1, a0, q, r);
    test = a0 * u0 - q * u1 - r;
    isZero = (test == Polynomial1<BSN>{});
    UTAssert(isZero,
        "Expecting the zero polynomial.");

    GetPseudoGCD(p0, p1, g);
    UTAssert(g.GetDegree() == 0 && g[0] == BSN(1),
        "Expecting a GCD of 1.");

    GetPseudoGCD(p0, poly0, g);
    UTAssert(g == poly0,
        "Expecting g to be poly0.");

    // g = -4 + 3*x
    // a0 = 81
    // a1 = 3
    // q0 = -324 - 81 * x - 405 * x^2 + 486 * x^3
    // q1 = 3
    GetPseudoExtendedGCD(p0, poly0, g, a0, a1, q0, q1);
    UTAssert(g == poly0,
        "Expecting g to be poly0.");
    test = a0 * p0 - q0 * g;
    isZero = (test == Polynomial1<BSN>{});
    UTAssert(isZero,
        "Expecting the zero polynomial.");
    test = a1 * poly0 - q1 * g;
    isZero = (test == Polynomial1<BSN>{});
    UTAssert(isZero,
        "Expecting the zero polynomial.");

    // g = 1
    // a0 = 1
    // a1 = 1
    // q0 = 16 - 8 * x + 17 * x^2 - 39 * x^3 + 18 * x^4
    // q1 = 2 + 3 * x + 4 * x^2
    GetPseudoExtendedGCD(p0, p1, g, a0, a1, q0, q1);
    isZero = (g == Polynomial1<BSN>{});
    UTAssert(!isZero,
        "Expecting a nonzero constant GCD.");
    test = a0 * p0 - q0 * g;
    isZero = (test == Polynomial1<BSN>{});
    UTAssert(isZero,
        "Expecting the zero polynomial.");
    test = a1 * p1 - q1 * g;
    isZero = (test == Polynomial1<BSN>{});
    UTAssert(isZero,
        "Expecting the zero polynomial.");

#if 1
    std::default_random_engine dre;
    std::uniform_real_distribution<float> urd(-1.0f, 1.0f);
    Polynomial1<BSN> input;
    input.SetDegree(6);
    for (std::size_t i = 0; i <= input.GetDegree(); ++i)
    {
        input[i] = urd(dre);
    }

    std::vector<Polynomial1<BSN>> factors;
    GetPseudoSquareFreeFactors(input, factors);
    UTAssert(factors.size() == 1 && factors[0] == input,
        "Expecting input to be square free.");

    p0 = Polynomial1<BSN>{ -1, 2 };
    p1 = Polynomial1<BSN>{ 6, 5, 4, 3 };
    u = p0 * p0 * p1;  // degree 5, { 6, -19, 8, 7, 4, 12 }
    GetPseudoSquareFreeFactors(u, factors);
    UTAssert(factors.size() == 3 &&
        factors[0].GetDegree() == 0 &&
        factors[1].GetDegree() == 3 &&
        factors[2].GetDegree() == 1,
        "Unexpected degrees for the factors.");
    test = factors[1] * factors[2] * factors[2];

    BSR rtemp = BSR(factors[2][0], factors[2][1]);  // -1/2 = -0.5
    rtemp = BSR(factors[1][0], factors[1][3]);  // 6/3 = 2.0
    rtemp = BSR(factors[1][1], factors[1][3]);  // 5/3 = 1.66...
    rtemp = BSR(factors[1][2], factors[1][3]);  // 4/3 = 1.33...
    rtemp = BSR(factors[1][3], factors[1][3]);  // 3/3 = 1.0

    Polynomial1<BSR> rtest{
        BSR(test[0], test[5]),
        BSR(test[1], test[5]),
        BSR(test[2], test[5]),
        BSR(test[3], test[5]),
        BSR(test[4], test[5]),
        BSR(1)
    };
    rtest *= BSR(12);  // degree 5, { 6, -19, 8, 7, 4, 12 }

    test = test[0] * u - u[0] * test;
    isZero = IsZero(test);
    UTAssert(isZero,
        "Expecting the zero polynomial.");
#endif
}

void UnitTestPolynomial::TestPerformanceBSR(std::size_t degree)
{
    // degree : microseconds : blocks (using BSR)
    // ------------------------------------------
    //  5 :          424 :    432
    //  6 :         7586 :   2228
    //  7 :       412960 :  17019
    //  8 :     15322541 : 105104

    gsUIntegerAP32MaxBlocks = 0;

    std::default_random_engine dre;
    std::uniform_real_distribution<float> urd(-1.0f, 1.0f);
    Polynomial1<BSR> input;
    input.SetDegree(degree);
    for (std::size_t i = 0; i <= degree; ++i)
    {
        input[i] = urd(dre);
    }
    if (input[degree].GetSign() < 0)
    {
        input[degree] = -input[degree];
    }

    Timer timer;
    std::vector<Polynomial1<BSR>> factors;
    GetSquareFreeFactors(input, factors);
    auto msecs = timer.GetMicroseconds();
    std::cout << std::endl;
    std::cout << "degree = " << degree << std::endl;
    std::cout << "microseconds = " << msecs << std::endl;
    std::cout << "num factors = " << factors.size() << std::endl;
    std::cout << "max blocks = " << gsUIntegerAP32MaxBlocks << std::endl;
}

void UnitTestPolynomial::TestPerformanceBSN(std::size_t degree)
{
    // The table suggests that an increase in degree of 1 leads to 
    // 6 times the computation time and 2.5 times the block size.
    //
    // degree : microseconds : blocks (using BSN)
    // ------------------------------------------
    //  5 :           54 :      58
    //  6 :           68 :     122
    //  7 :          167 :     313
    //  8 :          612 :     727
    //  9 :         3967 :    1973
    // 10 :        27451 :    5087
    // 11 :       135793 :   11534
    // 12 :       786286 :   28432
    // 13 :      4905542 :   70804
    // 14 :     25758253 :  162931
    // 15 :    161046099 :  405729
    // 16 :    900747666 :  962860

    gsUIntegerAP32MaxBlocks = 0;

    std::default_random_engine dre;
    std::uniform_real_distribution<float> urd(-1.0f, 1.0f);
    Polynomial1<BSN> input;
    input.SetDegree(degree);
    for (std::size_t i = 0; i <= degree; ++i)
    {
        input[i] = urd(dre);
    }
    if (input[degree].GetSign() < 0)
    {
        input[degree] = -input[degree];
    }

    Timer timer;
    std::vector<Polynomial1<BSN>> factors;
    GetPseudoSquareFreeFactors(input, factors);
    auto msecs = timer.GetMicroseconds();
    std::cout << std::endl;
    std::cout << "degree = " << degree << std::endl;
    std::cout << "microseconds = " << msecs << std::endl;
    std::cout << "num factors = " << factors.size() << std::endl;
    std::cout << "max blocks = " << gsUIntegerAP32MaxBlocks << std::endl;
}

void UnitTestPolynomial::TestPerformanceRepeatedBSR(std::size_t numFactors)
{
    // factors : microseconds : blocks (using BSR)
    // ------------------------------------------
    //  2 :           543901 :    12543

    gsUIntegerAP32MaxBlocks = 0;

    std::default_random_engine dre;
    std::uniform_real_distribution<float> urd(-1.0f, 1.0f);

    std::vector<Polynomial1<BSR>> poly(numFactors);
    Polynomial1<BSR> input{ 1 };
    for (std::size_t i = 0; i < numFactors; ++i)
    {
        poly[i].SetDegree(i + 1);
        for (std::size_t j = 0; j <= poly[i].GetDegree(); ++j)
        {
            poly[i][j] = urd(dre);
        }

        for (std::size_t k = 0; k <= i; ++k)
        {
            input *= poly[i];
        }
    }

    Timer timer;
    std::vector<Polynomial1<BSR>> factors;
    GetSquareFreeFactors(input, factors);
    auto msecs = timer.GetMicroseconds();
    std::cout << std::endl;
    std::cout << "num specified factors = " << numFactors << std::endl;
    std::cout << "microseconds = " << msecs << std::endl;
    std::cout << "num factors = " << factors.size() << std::endl;
    std::cout << "max blocks = " << gsUIntegerAP32MaxBlocks << std::endl;
}

void UnitTestPolynomial::TestPerformanceRepeatedBSN(std::size_t numFactors)
{
    // The factors.size() is numFactors + 1, because factors[0]
    // is always the polynomial 1.
    //
    // numFactors : microseconds : blocks (using BSN)
    // ------------------------------------------
    //  2 :           5146 :    1201

    gsUIntegerAP32MaxBlocks = 0;

    std::default_random_engine dre;
    std::uniform_real_distribution<float> urd(-1.0f, 1.0f);

    std::vector<Polynomial1<BSN>> poly(numFactors);
    Polynomial1<BSN> p{ 1 };
    for (std::size_t i = 0; i < numFactors; ++i)
    {
        poly[i].SetDegree(i + 1);
        for (std::size_t j = 0; j <= poly[i].GetDegree(); ++j)
        {
            poly[i][j] = urd(dre);
        }

        for (std::size_t k = 0; k <= i; ++k)
        {
            p *= poly[i];
        }
    }

    Timer timer;
    std::vector<Polynomial1<BSN>> factors;
    GetPseudoSquareFreeFactors(p, factors);
    auto msecs = timer.GetMicroseconds();
    std::cout << std::endl;
    std::cout << "microseconds = " << msecs << std::endl;
    std::cout << "num factors = " << factors.size() << std::endl;
    std::cout << "max blocks = " << gsUIntegerAP32MaxBlocks << std::endl;
}

void UnitTestPolynomial::TestPolynomial2()
{
    Polynomial<double, 2> p0{};
    UTAssert(
        p0.GetDegree() == 0,
        "Constructor failed.");

    Polynomial<double, 1> const& c0 = p0[0];
    UTAssert(
        c0.GetDegree() == 0 && c0[0] == 0.0,
        "Constructor failed.");

    Polynomial<double, 2> p2(2);
    UTAssert(
        p2.GetDegree() == 2,
        "Constructor failed.");
    for (std::size_t i = 0; i <= p2.GetDegree(); ++i)
    {
        Polynomial<double, 1> const& c2 = p2[i];
        UTAssert(
            c2.GetDegree() == 0 && c2[0] == 0.0,
            "Constructor failed.");
    }

    p2[0] = Polynomial<double, 1>{ 1.0, 2.0 };
    p2[1] = Polynomial<double, 1>{ 3.0 };
    p2[2] = Polynomial<double, 1>{ 4.0, 5.0, 6.0 };
    UTAssert(
        p2[0].GetDegree() == 1 &&
        p2[0][0] == 1.0 && p2[0][1] == 2.0,
        "operator[] failed.");
    UTAssert(
        p2[1].GetDegree() == 0 &&
        p2[1][0] == 3.0,
        "operator[] failed.");
    UTAssert(
        p2[2].GetDegree() == 2 &&
        p2[2][0] == 4.0 && p2[2][1] == 5.0 && p2[2][2] == 6.0,
        "operator[] failed.");

    Polynomial<double, 2> p3 = p2;
    UTAssert(
        p3.GetDegree() == 2,
        "Copy constructor failed.");
    p3.SetDegree(3);
    UTAssert(
        p3.GetDegree() == 3,
        "SetDegree failed.");
    UTAssert(
        p3[0].GetDegree() == 1 &&
        p3[0][0] == 1.0 && p3[0][1] == 2.0,
        "operator[] failed.");
    UTAssert(
        p3[1].GetDegree() == 0 &&
        p3[1][0] == 3.0,
        "operator[] failed.");
    UTAssert(
        p3[2].GetDegree() == 2 &&
        p3[2][0] == 4.0 && p3[2][1] == 5.0 && p3[2][2] == 6.0,
        "operator[] failed.");
    UTAssert(
        p3[3].GetDegree() == 0 &&
        p3[3][0] == 0.0,
        "SetDegree failed.");

    EliminateLeadingZeros(p3);
    UTAssert(
        p3.GetDegree() == 2,
        "Copy constructor failed.");
    UTAssert(
        p3[0].GetDegree() == 1 &&
        p3[0][0] == 1.0 && p3[0][1] == 2.0,
        "operator[] failed.");
    UTAssert(
        p3[1].GetDegree() == 0 &&
        p3[1][0] == 3.0,
        "operator[] failed.");
    UTAssert(
        p3[2].GetDegree() == 2 &&
        p3[2][0] == 4.0 && p3[2][1] == 5.0 && p3[2][2] == 6.0,
        "operator[] failed.");

    Fill(p3, 4.0);
    UTAssert(
        p3.GetDegree() == 2,
        "Fill failed.");
    UTAssert(
        p3[0].GetDegree() == 0 &&
        p3[0][0] == 4.0,
        "Fill failed.");
    UTAssert(
        p3[1].GetDegree() == 0 &&
        p3[1][0] == 4.0,
        "Fill failed.");
    UTAssert(
        p3[2].GetDegree() == 0 &&
        p3[2][0] == 4.0,
        "Fill failed.");

    Polynomial<double, 2> p4{ p2[0], p2[1], p2[2] };
    bool equal = (p4 == p2);
    UTAssert(
        equal,
        "Initializer list constructor failed.");

    // p4(x,y) = (1 + 2 * x) + (3) * y + (4 + 5 * x + 6 * x^2) * y^2
    // p4(1,y) = 3 + 3 * y + 15 * y^2
    // p4(1,2) = 3 + 6 + 60 = 69
    std::array<double, 2> input{ 1.0, 2.0 };
    double value = p4(input.data());
    UTAssert(
        value == 69.0,
        "Polynomial evaluation failed.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Algebra/Polynomial.h>

namespace gtl
{
    template class Polynomial<float, 1>;
    template class Polynomial<float, 2>;
    using PF1 = Polynomial<float, 1>;
    using PF2 = Polynomial<float, 2>;
    template PF1 operator+(PF1 const&);
    template PF1 operator-(PF1 const&);
    template PF1 operator+(PF1 const&, PF1 const&);
    template PF1 operator+(PF1 const&, float const&);
    template PF1 operator+(float const&, PF1 const&);
    template PF1& operator+=(PF1&, PF1 const&);
    template PF1& operator+=(PF1&, float const&);
    template PF1 operator-(PF1 const&, PF1 const&);
    template PF1 operator-(PF1 const&, float const&);
    template PF1 operator-(float const&, PF1 const&);
    template PF1& operator-=(PF1&, PF1 const&);
    template PF1& operator-=(PF1&, float const&);
    template PF1 operator*(PF1 const&, float const&);
    template PF1 operator*(float const&, PF1 const&);
    template PF1& operator*=(PF1&, float const&);
    template PF1 operator/(PF1 const&, float const&);
    template PF1& operator/=(PF1&, float const&);
    template PF1 operator*(PF1 const&, PF1 const&);
    template PF1& operator*=(PF1&, PF1 const&);
    template void Fill(PF1&, float const&);
    template void EliminateLeadingZeros(PF1&);
    template void MakeMonic(PF1&);
    template PF1 MultiplyByXToPower(std::size_t, PF1 const&);
    template PF1 GetDerivative(PF1 const&);
    template PF1 GetInversion(PF1 const&);
    template PF1 GetTranslation(PF1 const&, float const&);
    template PF2 operator+(PF2 const&);
    template PF2 operator-(PF2 const&);
    template PF2 operator+(PF2 const&, PF2 const&);
    template PF2 operator+(PF2 const&, float const&);
    template PF2 operator+(float const&, PF2 const&);
    template PF2& operator+=(PF2&, PF2 const&);
    template PF2& operator+=(PF2&, float const&);
    template PF2 operator-(PF2 const&, PF2 const&);
    template PF2 operator-(PF2 const&, float const&);
    template PF2 operator-(float const&, PF2 const&);
    template PF2& operator-=(PF2&, PF2 const&);
    template PF2& operator-=(PF2&, float const&);
    template PF2 operator*(PF2 const&, float const&);
    template PF2 operator*(float const&, PF2 const&);
    template PF2& operator*=(PF2&, float const&);
    template PF2 operator/(PF2 const&, float const&);
    template PF2& operator/=(PF2&, float const&);
    template PF2 operator*(PF2 const&, PF2 const&);
    template PF2& operator*=(PF2&, PF2 const&);
    template void Fill(PF2&, float const&);
    template void EliminateLeadingZeros(PF2&);

    template class Polynomial<double, 1>;
    template class Polynomial<double, 2>;
    using PD1 = Polynomial<double, 1>;
    using PD2 = Polynomial<double, 2>;
    template PD1 operator+(PD1 const&);
    template PD1 operator-(PD1 const&);
    template PD1 operator+(PD1 const&, PD1 const&);
    template PD1 operator+(PD1 const&, double const&);
    template PD1 operator+(double const&, PD1 const&);
    template PD1& operator+=(PD1&, PD1 const&);
    template PD1& operator+=(PD1&, double const&);
    template PD1 operator-(PD1 const&, PD1 const&);
    template PD1 operator-(PD1 const&, double const&);
    template PD1 operator-(double const&, PD1 const&);
    template PD1& operator-=(PD1&, PD1 const&);
    template PD1& operator-=(PD1&, double const&);
    template PD1 operator*(PD1 const&, double const&);
    template PD1 operator*(double const&, PD1 const&);
    template PD1& operator*=(PD1&, double const&);
    template PD1 operator/(PD1 const&, double const&);
    template PD1& operator/=(PD1&, double const&);
    template PD1 operator*(PD1 const&, PD1 const&);
    template PD1& operator*=(PD1&, PD1 const&);
    template void Fill(PD1&, double const&);
    template void EliminateLeadingZeros(PD1&);
    template void MakeMonic(PD1&);
    template PD1 MultiplyByXToPower(std::size_t, PD1 const&);
    template PD1 GetDerivative(PD1 const&);
    template PD1 GetInversion(PD1 const&);
    template PD1 GetTranslation(PD1 const&, double const&);
    template PD2 operator+(PD2 const&);
    template PD2 operator-(PD2 const&);
    template PD2 operator+(PD2 const&, PD2 const&);
    template PD2 operator+(PD2 const&, double const&);
    template PD2 operator+(double const&, PD2 const&);
    template PD2& operator+=(PD2&, PD2 const&);
    template PD2& operator+=(PD2&, double const&);
    template PD2 operator-(PD2 const&, PD2 const&);
    template PD2 operator-(PD2 const&, double const&);
    template PD2 operator-(double const&, PD2 const&);
    template PD2& operator-=(PD2&, PD2 const&);
    template PD2& operator-=(PD2&, double const&);
    template PD2 operator*(PD2 const&, double const&);
    template PD2 operator*(double const&, PD2 const&);
    template PD2& operator*=(PD2&, double const&);
    template PD2 operator/(PD2 const&, double const&);
    template PD2& operator/=(PD2&, double const&);
    template PD2 operator*(PD2 const&, PD2 const&);
    template PD2& operator*=(PD2&, PD2 const&);
    template void Fill(PD2&, double const&);
    template void EliminateLeadingZeros(PD2&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Polynomial<Rational, 1>;
    template class Polynomial<Rational, 2>;
    using PR1 = Polynomial<Rational, 1>;
    using PR2 = Polynomial<Rational, 2>;
    template PR1 operator+(PR1 const&);
    template PR1 operator-(PR1 const&);
    template PR1 operator+(PR1 const&, PR1 const&);
    template PR1 operator+(PR1 const&, Rational const&);
    template PR1 operator+(Rational const&, PR1 const&);
    template PR1& operator+=(PR1&, PR1 const&);
    template PR1& operator+=(PR1&, Rational const&);
    template PR1 operator-(PR1 const&, PR1 const&);
    template PR1 operator-(PR1 const&, Rational const&);
    template PR1 operator-(Rational const&, PR1 const&);
    template PR1& operator-=(PR1&, PR1 const&);
    template PR1& operator-=(PR1&, Rational const&);
    template PR1 operator*(PR1 const&, Rational const&);
    template PR1 operator*(Rational const&, PR1 const&);
    template PR1& operator*=(PR1&, Rational const&);
    template PR1 operator/(PR1 const&, Rational const&);
    template PR1& operator/=(PR1&, Rational const&);
    template PR1 operator*(PR1 const&, PR1 const&);
    template PR1& operator*=(PR1&, PR1 const&);
    template void Fill(PR1&, Rational const&);
    template void EliminateLeadingZeros(PR1&);
    template void MakeMonic(PR1&);
    template PR1 MultiplyByXToPower(std::size_t, PR1 const&);
    template PR1 GetDerivative(PR1 const&);
    template PR1 GetInversion(PR1 const&);
    template PR1 GetTranslation(PR1 const&, Rational const&);
    template PR2 operator+(PR2 const&);
    template PR2 operator-(PR2 const&);
    template PR2 operator+(PR2 const&, PR2 const&);
    template PR2 operator+(PR2 const&, Rational const&);
    template PR2 operator+(Rational const&, PR2 const&);
    template PR2& operator+=(PR2&, PR2 const&);
    template PR2& operator+=(PR2&, Rational const&);
    template PR2 operator-(PR2 const&, PR2 const&);
    template PR2 operator-(PR2 const&, Rational const&);
    template PR2 operator-(Rational const&, PR2 const&);
    template PR2& operator-=(PR2&, PR2 const&);
    template PR2& operator-=(PR2&, Rational const&);
    template PR2 operator*(PR2 const&, Rational const&);
    template PR2 operator*(Rational const&, PR2 const&);
    template PR2& operator*=(PR2&, Rational const&);
    template PR2 operator/(PR2 const&, Rational const&);
    template PR2& operator/=(PR2&, Rational const&);
    template PR2 operator*(PR2 const&, PR2 const&);
    template PR2& operator*=(PR2&, PR2 const&);
    template void Fill(PR2&, Rational const&);
    template void EliminateLeadingZeros(PR2&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Polynomial)
