// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Algebra/LieGroupsAlgebras.h>
#include <GTL/Mathematics/Algebra/Vector.h>
using namespace gtl;

namespace gtl
{
    class UnitTestLieGroupsAlgebras
    {
    public:
        UnitTestLieGroupsAlgebras();

    private:
        void TestLieSO2();
        void TestLieSE2();
        void TestLieSO3();
        void TestLieSE3();

        void TestLieSO2Instance(
            double c, double maxError,
            Matrix<double, 2, 2> const& expectedM,
            Matrix<double, 2, 2> const& expectedP0,
            Matrix<double, 2, 2> const& expectedP1,
            Matrix<double, 2, 2> const& expectedP2);

        void TestLieSE2Instance(
            Vector<double, 3> const& c, double maxError,
            Matrix<double, 3, 3> const& expectedM,
            Matrix<double, 3, 3> const& expectedA,
            Matrix<double, 3, 3> const& expectedP0,
            Matrix<double, 3, 3> const& expectedP1,
            Matrix<double, 3, 3> const& expectedP2);

        void TestLieSO3Instance(
            Vector<double, 3> const& c, double maxError,
            Matrix<double, 3, 3> const& expectedM,
            Matrix<double, 3, 3> const& expectedP0,
            Matrix<double, 3, 3> const& expectedP1,
            Matrix<double, 3, 3> const& expectedP2);

        void TestLieSE3Instance(
            Vector<double, 6> const& c, double maxError,
            Matrix<double, 4, 4> const& expectedM,
            Matrix<double, 6, 6> const& expectedA,
            Matrix<double, 4, 4> const& expectedP0,
            Matrix<double, 4, 4> const& expectedP1,
            Matrix<double, 4, 4> const& expectedP2);
    };
}

UnitTestLieGroupsAlgebras::UnitTestLieGroupsAlgebras()
{
    UTInformation("Mathematics/Algebra/LieGroupsAlgebras");

    TestLieSO2();
    TestLieSE2();
    TestLieSO3();
    TestLieSE3();
}

void UnitTestLieGroupsAlgebras::TestLieSO2()
{
    double c{};
    double maxError{};
    Matrix<double, 2, 2> expectedM{};
    Matrix<double, 2, 2> expectedP0{};
    Matrix<double, 2, 2> expectedP1{};
    Matrix<double, 2, 2> expectedP2{};

    c = 1.0;
    maxError = 1e-13;
    expectedM = {
        { 0.54030230586813977, -0.84147098480789650 },
        { 0.84147098480789650, 0.54030230586813977 }
    };
    expectedP0 = {
        { 0.87758256189037276, -0.47942553860420301 },
        { 0.47942553860420301, 0.87758256189037276 }
    };
    expectedP1 = {
        { 0.99999950000004167, -0.00099999983333334168 },
        { 0.00099999983333334168, 0.99999950000004167 }
    };
    expectedP2 = {
        { 0.54114350656157206, -0.84093026185662145 },
        { 0.84093026185662145, 0.54114350656157206 }
    };
    TestLieSO2Instance(c, maxError, expectedM, expectedP0, expectedP1, expectedP2);
}

void UnitTestLieGroupsAlgebras::TestLieSE2()
{
    Vector<double, 3> c{};
    double maxError{};
    Matrix<double, 3, 3> expectedM{};
    Matrix<double, 3, 3> expectedA{};
    Matrix<double, 3, 3> expectedP0{};
    Matrix<double, 3, 3> expectedP1{};
    Matrix<double, 3, 3> expectedP2{};

    c = { 1.0, 2.0, 3.0 };
    maxError = 1e-13;
    expectedM = {
        { 0.54030230586813977, -0.84147098480789650, 0.30384888722021230 },
        { 0.84147098480789650, 0.54030230586813977, 3.4438083426874098 },
        { 0.0, 0.0, 1.0}
    };
    expectedA = {
        { 1.0, 0.0, 0.0 },
        { 3.4438083426874098, 0.54030230586813977, -0.84147098480789650 },
        { -0.30384888722021230, 0.84147098480789650, 0.54030230586813977 }
    };
    expectedP0 = {
        { 0.87758256189037276, -0.47942553860420301, 0.59159876287952429 },
        { 0.47942553860420301, 0.87758256189037276, 1.6831114920318633 },
        { 0.0, 0.0, 1.0 }
    };
    expectedP1 = {
        { 0.99999950000004167, -0.00099999983333334168, 0.0019984996667916834 },
        { 0.00099999983333334168, 0.99999950000004167, 0.0030009994999166912 },
        { 0.0, 0.0, 1.0 }
    };
    expectedP2 = {
        { 0.54114350656157206, -0.84093026185662145, 0.30529104339795943 },
        { 0.84093026185662145, 0.54114350656157206, 3.4405037724467196 },
        { 0.0, 0.0, 1.0 }
    };
    TestLieSE2Instance(c, maxError, expectedM, expectedA, expectedP0, expectedP1, expectedP2);
}

void UnitTestLieGroupsAlgebras::TestLieSO3()
{
    // The logarithm extracts an angle theta that is in [0,pi]. In the Eade
    // PDFs, theta = Length(c), which can be larger than pi. As a consequence,
    // expectedC will have length in [0,pi], so we cannot recapture the
    // original c. However, c and expectedC are parallel.

    Vector<double, 3> c{};
    double maxError{};
    Matrix<double, 3, 3> expectedM{};
    Matrix<double, 3, 3> expectedP0{};
    Matrix<double, 3, 3> expectedP1{};
    Matrix<double, 3, 3> expectedP2{};

    c = { 1.0, 2.0, 3.0 };
    maxError = 1e-13;
    expectedM = {
        { -0.69492055764131155, 0.71352099052778750, 0.089292858861912183 },
        { -0.19200697279199935, -0.30378504433947051, 0.93319235382364685 },
        { 0.69297816774177012, 0.63134969938371777, 0.34810747783026474 }
    };
    expectedP0 = {
        { 0.34586890410062221, 0.86660113335108180, -0.35969039026759531 },
        { -0.66533002692050391, 0.49682223392355551, 0.55722851969113107 },
        { 0.66159704991346213, 0.046584799600602300, 0.74841111696177764 }
    };
    expectedP1 = {
        { 0.99999700101110667, 0.0020382149142277929, -0.0013578102798540964 },
        { -0.0020372921484144708, 0.99999769308546671, 0.00068063532582702700 },
        { 0.0013591944285740794, -0.00067786702838706133, 0.99999884654273330 }
    };
    expectedP2 = {
        { -0.69358540721944983, 0.71499587656578434, 0.087864551362627219 },
        { -0.19389267434441482, -0.30275800555342314, 0.93313622848375377 },
        { 0.69379025196942679, 0.63017337818035390, 0.34862099722328821 }
    };
    TestLieSO3Instance(c, maxError, expectedM, expectedP0, expectedP1, expectedP2);

    c = { 0.001, 0.002, 0.003 };
    expectedM = {
        { 0.99999350000758336, -0.0029989930011715660, 0.0020014953315866009 },
        { 0.0030009929988382340, 0.99999500000583330, -0.00099699767016829836 },
        { -0.0019984953350865994, 0.0010029976631683018, 0.99999750000291665 }
    };
    expectedP0 = {
        { 0.99999837500047395, -0.0014997491250730696, 0.0010003744165573937 },
        { 0.0015002491249272362, 0.99999875000036453, -0.00049924970855213429},
        { -0.00099962441677614372, 0.00050074970811463441, 0.99999937500018232 }
    };
    expectedP1 = {
        { 0.99999999999349998, -2.9999989999930000e-06, 2.0000014999953331e-06 },
        { 3.0000009999929996e-06, 0.99999999999500000, -9.9999699999766651e-07 },
        { -1.9999984999953332e-06, 1.0000029999976667e-06, 0.99999999999750000 }
    };
    expectedP2 = {
        { 0.99999351300105299, -0.0029959950211458889, 0.0019994923470795789 },
        { 0.0029979910208218761, 0.99999501000081004, -0.00099600367414731357 },
        { -0.0019964983475655980, 0.0010019916731752749, 0.99999750500040496 }
    };
    TestLieSO3Instance(c, maxError, expectedM, expectedP0, expectedP1, expectedP2);
}

void UnitTestLieGroupsAlgebras::TestLieSE3()
{
    Vector<double, 6> c{};
    double maxError{};
    Matrix<double, 4, 4> expectedM{};
    Matrix<double, 6, 6> expectedA{};
    Matrix<double, 4, 4> expectedP0{};
    Matrix<double, 4, 4> expectedP1{};
    Matrix<double, 4, 4> expectedP2{};

    c = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
    maxError = 1e-13;
    expectedM = {
        { -0.69492055764131155, 0.71352099052778750, 0.089292858861912183, 1.6358564971782192 },
        { -0.19200697279199935, -0.30378504433947051, 0.93319235382364685, 5.2890190292236978 },
        { 0.69297816774177012, 0.63134969938371777, 0.34810747783026474, 6.5953684814581290 },
        { 0.0, 0.0, 0.0, 1.0 }
    };
    Matrix<double, 3, 3> R = {
        { expectedM(0, 0), expectedM(0, 1), expectedM(0, 2) },
        { expectedM(1, 0), expectedM(1, 1), expectedM(1, 2) },
        { expectedM(2, 0), expectedM(2, 1), expectedM(2, 2) }
    };
    Vector<double, 3> T = { expectedM(0, 3), expectedM(1, 3), expectedM(2, 3) };
    Matrix<double, 3, 3> skewT = {
        { 0.0, -T[2], +T[1] },
        { +T[2], 0.0, -T[0] },
        { -T[1], +T[0], 0.0 }
    };
    Matrix<double, 3, 3> skewRT = skewT * R;
    expectedA = {
        { R(0, 0), R(0, 1), R(0, 2), 0.0, 0.0, 0.0},
        { R(1, 0), R(1, 1), R(1, 2), 0.0, 0.0, 0.0},
        { R(2, 0), R(2, 1), R(2, 2), 0.0, 0.0, 0.0},
        { skewRT(0, 0), skewRT(0, 1), skewRT(0, 2), R(0, 0), R(0, 1), R(0, 2) },
        { skewRT(1, 0), skewRT(1, 1), skewRT(1, 2), R(1, 0), R(1, 1), R(1, 2) },
        { skewRT(2, 0), skewRT(2, 1), skewRT(2, 2), R(2, 0), R(2, 1), R(2, 2) }
    };
    expectedP0 = {
        { 0.34586890410062221, 0.86660113335108180, -0.35969039026759531, 0.55420919581375683 },
        { -0.66533002692050391, 0.49682223392355551, 0.55722851969113107, 2.4781972910550385 },
        { 0.66159704991346213, 0.046584799600602300, 0.74841111696177776, 3.4964654073587229 },
        { 0.0, 0.0, 0.0, 1.0 }
    };
    expectedP1 = {
        { 0.99999700101110667, 0.0020382149142277929, -0.0013578102798540964, 0.0011205916220279357 },
        { -0.0020372921484144708, 0.99999769308546671, 0.00068063532582702700, 0.0042817050728169628 },
        { 0.0013591944285740794, -0.00067786702838706133, 0.99999884654273330, 0.0074386660774460470 },
        { 0.0, 0.0, 0.0, 1.0 }
    };
    expectedP2 = {
        { -0.69358540721944983, 0.71499587656578434, 0.087864551362627219, 1.6329187266453442 },
        { -0.19389267434441482, -0.30275800555342314, 0.93313622848375377, 5.2835913354098967 },
        { 0.69379025196942679, 0.63017337818035390, 0.34862099722328821, 6.5892995341782887 },
        { 0.0, 0.0, 0.0, 1.0 }
    };
    TestLieSE3Instance(c, maxError, expectedM, expectedA, expectedP0, expectedP1, expectedP2);
}

void UnitTestLieGroupsAlgebras::TestLieSO2Instance(
    double c, double maxError,
    Matrix<double, 2, 2> const& expectedM,
    Matrix<double, 2, 2> const& expectedP0,
    Matrix<double, 2, 2> const& expectedP1,
    Matrix<double, 2, 2> const& expectedP2)
{
    Matrix<double, 2, 2> diff{};
    double error{};

    // ToGroup
    Matrix<double, 2, 2> G = LieSO2<double>::ToGroup(c);
    Matrix<double, 2, 2> expectedG = { { 0.0, -c }, { c, 0.0 }};
    UTAssert(
        G == expectedG,
        "Invalid generator from algebra."
    );

    // ToAlgebra
    auto expectedC = LieSO2<double>::ToAlgebra(G);
    UTAssert(
        c == expectedC,
        "Invalid algebra from generator.");

    // Exp
    auto M = LieSO2<double>::Exp(c);
    diff = M - expectedM;
    error = L1Norm(diff);
    UTAssert(
        error <= maxError,
        "Invalid exp from algebra.");

    // Log
    expectedC = LieSO2<double>::Log(M);
    error = std::fabs(c - expectedC);
    UTAssert(
        error <= maxError,
        "Invalid algebra from group.");

    // Adjoint
    auto A = LieSO2<double>::Adjoint(M);
    UTAssert(
        A == 1.0,
        "Invalid adjoint from group.");

    // GeodesicPath 0
    Matrix<double, 2, 2> M0{}, M1{};
    MakeIdentity(M0);
    M1 = M;
    auto P = LieSO2<double>::GeodesicPath(0.5, M0, M1);
    diff = P - expectedP0;
    error = L1Norm(diff);
    UTAssert(
        error <= maxError,
        "Invalid geodesic path 0.");

    // GeodesicPath 1
    P = LieSO2<double>::GeodesicPath(0.001, M0, M1);
    diff = P - expectedP1;
    error = L1Norm(diff);
    UTAssert(
        error <= maxError,
        "Invalid geodesic path 1.");

    // GeodesicPath 2
    P = LieSO2<double>::GeodesicPath(0.999, M0, M1);
    diff = P - expectedP2;
    error = L1Norm(diff);
    UTAssert(
        error <= maxError,
        "Invalid geodesic path 2.");
}

void UnitTestLieGroupsAlgebras::TestLieSE2Instance(
    Vector<double, 3> const& c, double maxError,
    Matrix<double, 3, 3> const& expectedM,
    Matrix<double, 3, 3> const& expectedA,
    Matrix<double, 3, 3> const& expectedP0,
    Matrix<double, 3, 3> const& expectedP1,
    Matrix<double, 3, 3> const& expectedP2)
{
    Matrix<double, 3, 3> diff{};
    double error{};

    // ToGroup
    Matrix<double, 3, 3> G = LieSE2<double>::ToGroup(c);
    Matrix<double, 3, 3> expectedG = {
        { 0.0, -c[0], c[1] },
        { +c[0], 0.0, c[2] },
        { 0.0, 0.0, 0.0 }
    };
    UTAssert(
        G == expectedG,
        "Invalid generator from algebra.");

    // ToAlgebra
    auto expectedC = LieSE2<double>::ToAlgebra(G);
    UTAssert(
        c == expectedC,
        "Invalid algebra from generator.");

    // Exp
    auto M = LieSE2<double>::Exp(c);
    diff = M - expectedM;
    error = L1Norm(diff);
    UTAssert(
        error <= maxError,
        "Invalid exp from algebra.");

    // Log
    expectedC = LieSE2<double>::Log(M);
    Vector<double, 3> cross = Cross(c, expectedC);
    error = Length(cross);
    UTAssert(
        error <= maxError,
        "Invalid cross from group.");

    // Adjoint
    auto A = LieSE2<double>::Adjoint(M);
    diff = A - expectedA;
    error = L1Norm(diff);
    UTAssert(
        error <= maxError,
        "Invalid adjoint from group.");

    // GeodesicPath 0
    Matrix<double, 3, 3> M0{}, M1{};
    MakeIdentity(M0);
    M1 = M;
    auto P = LieSE2<double>::GeodesicPath(0.5, M0, M1);
    diff = P - expectedP0;
    error = L1Norm(diff);
    UTAssert(
        error <= maxError,
        "Invalid geodesic path 0.");

    // GeodesicPath 1
    P = LieSE2<double>::GeodesicPath(0.001, M0, M1);
    diff = P - expectedP1;
    error = L1Norm(diff);
    UTAssert(
        error <= maxError,
        "Invalid geodesic path 1.");

    // GeodesicPath 2
    P = LieSE2<double>::GeodesicPath(0.999, M0, M1);
    diff = P - expectedP2;
    error = L1Norm(diff);
    UTAssert(
        error <= maxError,
        "Invalid geodesic path 2.");
}

void UnitTestLieGroupsAlgebras::TestLieSO3Instance(
    Vector<double, 3> const& c, double maxError,
    Matrix<double, 3, 3> const& expectedM,
    Matrix<double, 3, 3> const& expectedP0,
    Matrix<double, 3, 3> const& expectedP1,
    Matrix<double, 3, 3> const& expectedP2)
{
    Matrix<double, 3, 3> diff{};
    double error{};

    // ToGroup
    Matrix<double, 3, 3> G = LieSO3<double>::ToGroup(c);
    Matrix<double, 3, 3> expectedG = {
        { 0.0, -c[2], +c[1] },
        { +c[2], 0.0, -c[0] },
        { -c[1], +c[0], 0.0 }
    };
    UTAssert(
        G == expectedG,
        "Invalid generator from algebra.");

    // ToAlgebra
    auto expectedC = LieSO3<double>::ToAlgebra(G);
    UTAssert(
        c == expectedC,
        "Invalid algebra from generator.");

    // Exp
    auto M = LieSO3<double>::Exp(c);
    diff = M - expectedM;
    error = L1Norm(diff);
    UTAssert(
        error <= maxError,
        "Invalid exp from algebra.");

    // Log
    expectedC = LieSO3<double>::Log(M);
    Vector<double, 3> cross = Cross(c, expectedC);
    error = Length(cross);
    UTAssert(
        error <= maxError,
        "Invalid cross from group.");

    // Adjoint
    auto A = LieSO3<double>::Adjoint(M);
    auto const& expectedA = M;
    diff = A - expectedA;
    error = L1Norm(diff);
    UTAssert(
        error <= maxError,
        "Invalid adjoint from group.");

    // GeodesicPath 0
    Matrix<double, 3, 3> M0{}, M1{};
    MakeIdentity(M0);
    M1 = M;
    auto P = LieSO3<double>::GeodesicPath(0.5, M0, M1);
    diff = P - expectedP0;
    error = L1Norm(diff);
    UTAssert(
        error <= maxError,
        "Invalid geodesic path 0.");

    // GeodesicPath 1
    P = LieSO3<double>::GeodesicPath(0.001, M0, M1);
    diff = P - expectedP1;
    error = L1Norm(diff);
    UTAssert(
        error <= maxError,
        "Invalid geodesic path 1.");

    // GeodesicPath 2
    P = LieSO3<double>::GeodesicPath(0.999, M0, M1);
    diff = P - expectedP2;
    error = L1Norm(diff);
    UTAssert(
        error <= maxError,
        "Invalid geodesic path 2.");
}

void UnitTestLieGroupsAlgebras::TestLieSE3Instance(
    Vector<double, 6> const& c, double maxError,
    Matrix<double, 4, 4> const& expectedM,
    Matrix<double, 6, 6> const& expectedA,
    Matrix<double, 4, 4> const& expectedP0,
    Matrix<double, 4, 4> const& expectedP1,
    Matrix<double, 4, 4> const& expectedP2)
{
    // The logarithm extracts (c[0],c[1],c[2]) whose length is in [0,pi]. In
    // the Eade PDFs, theta = Length(c), which can be larger than pi. As a
    // consequence, expectedC will have length in [0,pi], so we cannot
    // recapture the original c. However, c and expectedC are parallel.

    Matrix<double, 4, 4> diff{};
    double error{};

    // ToGroup
    Matrix<double, 4, 4> G = LieSE3<double>::ToGroup(c);
    Matrix<double, 4, 4> expectedG = {
        { 0.0, -c[2], +c[1], c[3]},
        { +c[2], 0.0, -c[0], c[4]},
        { -c[1], +c[0], 0.0, c[5]},
        { 0.0, 0.0, 0.0, 0.0 }
    };
    UTAssert(
        G == expectedG,
        "Invalid generator from algebra.");

    // ToAlgebra
    auto expectedC = LieSE3<double>::ToAlgebra(G);
    UTAssert(
        c == expectedC,
        "Invalid algebra from generator.");

    // Exp
    auto M = LieSE3<double>::Exp(c);
    diff = M - expectedM;
    error = L1Norm(diff);
    UTAssert(
        error <= maxError,
        "Invalid exp from algebra.");

    // Log
    expectedC = LieSE3<double>::Log(M);
    Matrix<double, 4, 4> inverted = LieSE3<double>::Exp(expectedC);
    diff = M - inverted;
    error = L1Norm(diff);
    UTAssert(
        error <= maxError,
        "Invalid log from group.");

    // Adjoint
    auto A = LieSE3<double>::Adjoint(M);
    auto adiff = A - expectedA;
    error = L1Norm(adiff);
    UTAssert(
        error <= maxError,
        "Invalid algebra from group.");

    // GeodesicPath 0
    Matrix<double, 4, 4> M0{}, M1{};
    MakeIdentity(M0);
    M1 = LieSE3<double>::Exp(c);
    auto P = LieSE3<double>::GeodesicPath(0.5, M0, M1);
    diff = P - expectedP0;
    error = L1Norm(diff);
    UTAssert(
        error <= maxError,
        "Invalid geodesic path 0.");

    // GeodesicPath 1
    P = LieSE3<double>::GeodesicPath(0.001, M0, M1);
    diff = P - expectedP1;
    error = L1Norm(diff);
    UTAssert(
        error <= maxError,
        "Invalid geodesic path 1.");

    // GeodesicPath 2
    P = LieSE3<double>::GeodesicPath(0.999, M0, M1);
    diff = P - expectedP2;
    error = L1Norm(diff);
    UTAssert(
        error <= maxError,
        "Invalid geodesic path 2.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Algebra/LieGroupsAlgebras.h>

namespace gtl
{
    template class LieSO2<float>;
    template class LieSE2<float>;
    template class LieSO3<float>;
    template class LieSE3<float>;

    template class LieSO2<double>;
    template class LieSE2<double>;
    template class LieSO3<double>;
    template class LieSE3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class LieSO2<Rational>;
    template class LieSE2<Rational>;
    template class LieSO3<Rational>;
    template class LieSE3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(LieGroupsAlgebras)
