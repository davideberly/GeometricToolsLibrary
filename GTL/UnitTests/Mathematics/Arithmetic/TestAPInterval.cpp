// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/UIntegerAP32.h>
#include <GTL/Mathematics/Arithmetic/BSRational.h>
#include <GTL/Mathematics/Arithmetic/APInterval.h>
using namespace gtl;

namespace gtl
{
    class UnitTestAPInterval
    {
    public:
        UnitTestAPInterval();

    private:
        using BSN = BSNumber<UIntegerAP32>;
        using BSR = BSRational<UIntegerAP32>;
        using BSNInterval = APInterval<BSN>;

        template <typename T>
        T DhDu(T const& u, T const& v, T const& w)
        {
            //dhdu/(-2u) = -1 - 2 u^2 + 3 u^4 - v^2 + 12 u^2 v^2 - 3 u^4 v^2 - 2 u^2 v^4 -
            //3 u^4 v^4 + 3 u^4 v^6 + 4 v w + 8 u^2 v w + 8 u^2 v^3 w + 8 w^2 -
            //4 u^2 w^2 + 8 v^2 w^2 + 16 u^2 v^2 w^2 - 4 u^2 v^4 w^2 + 12 v w^3 +
            //w^4 + v^2 w^4
            //=
            //(-(1 + v^2) + u^2 (3 u^2 (-1 + v^2)^2 (1 + v^2) - 2 (1 + v^2 (v^2 - 6))))
            //+ w(4 v(1 + 2 u^2 (1 + v^2)))
            //+ w^2 (8 (1 + v^2) - 4 u^2 (1 + v^2 (v^2 - 4)))
            //+ w^3 (12 v)
            //+ w^4 (1 + v^2)

            T t1(1), t2(2), t3(3), t4(4), t6(6), t8(8), t12(12);

            T u2 = u * u, v2 = v * v, w2 = w * w;
            T onepv2 = t1 + v2, onemv2 = t1 - v2;
            T v2m4 = v2 - t4, v2m6 = v2 - t6;

            T dhdu =
                (-onepv2 + u2 * (t3 * u2 * onemv2 * onemv2 * onepv2 - t12 * (t1 + v2 * v2m6))) +
                w * ((v * (t4 + t8 * u2 * onepv2)) + w2 * (t12 * v)) +
                w2 * ((t8 * onepv2 - t4 * u2 * (t1 + v2 * v2m4)) + w2 * onepv2);
            return dhdu;
        }

        template <typename T>
        T DhDv(T const& u, T const& v, T const& w)
        {
            //dhdv/(-2u^2) = -v + 6 u^2 v - u^4 v - 2 u^2 v^3 - 2 u^4 v^3 + 3 u^4 v^5 +
            //2 w + 2 u^2 w + 6 u^2 v^2 w + 8 v w^2 + 8 u^2 v w^2 -
            //4 u^2 v^3 w^2 + 6 w^3 + v w^4
            //=
            //(v(-1 - 2 u^2 (-3 + v^2) + u^4 (-1 - 2 v^2 + 3 v^4)))
            //+ w(2 + u^2 (2 + 6 v^2))
            //+ w^2 (4 v(2 + u^2(2 - v^2)))
            //+ w^3 (6)
            //+ w^4 (v)

            T t1(1), t2(2), t3(3), t4(4), t6(6);

            T u2 = u * u, v2 = v * v, w2 = w * w;

            T dhdv =
                (v * (-t1 + u2 * (u2 * (-t1 + v2 * (t3 * v2 - t2)) - t2 * (v2 - t3))))
                + w * ((t2 + u2 * (t2 + t6 * v2)) + t6 * w2)
                + w2 * ((t4 * v * (t2 + u2 * (t2 - v2))) + w2 * v);
            return dhdv;
        }

        template <typename T>
        T DhDw(T const& u, T const& v, T const& w)
        {
            //dhdw/4 = -u^2 v - u^4 v - u^4 v^3 + w - 4 u^2 w + u^4 w - 4 u^2 v^2 w -
            //4 u^4 v^2 w + u^4 v^4 w - 9 u^2 v w^2 - w^3 - u^2 w^3 -
            //u^2 v^2 w^3
            //=
            //(-u^2 v(1 + u^2 (1 + v^2))
            //+ w(1 - 4 u^2 (1 + v^2) + u^4 (1 - 4 v^2 + v^4))
            //+ w^2 (-9 u^2 v)
            //+ w^3 (-1 - u^2 (1 + v^2))

            T t1(1), t4(4), t9(9);

            T u2 = u * u, v2 = v * v, w2 = w * w;
            T onepv2 = t1 + v2, onepu2tonepv2 = t1 + u2 * onepv2;
            T v2m4 = v2 - t4;

            T dhdw =
                (-u2 * v * onepu2tonepv2)
                + w * (t1 + u2 * (u2 * (t1 + v2 * v2m4) - t4 * onepv2))
                - w2 * (t9 * u2 * v + w * onepu2tonepv2);
            return dhdw;
        }

        // This code was used for mathematical proofs in RAEFGC, Section 5.5.4.1
        // "Two Nonzero Radical Coefficients." In the PDF draft on page 113, there
        // is a polynomial delta(u) = 2 - 10u^2 + u^4 + 4u^6, which is cubic in
        // u^2. There is a polynomial epsilon(v) = -11 + 52v^2 + 61v^4 + 4v^6,
        // which is cubic in v^2. It is shown that delta(u) = 0 has a single root
        // in (0,1), call it bar(u) and epsilon(v) has a single root in 
        // (0,+infinity), call it bar(v). I needed to prove that h(bar(u),bar(v))
        // is negative, which I did using arbitrary-precision interval arithmetic
        // for the domain [ubound0,ubound1]x[vbound0,vbound1].
        void VerifyTwoRootLocalMaximum();

        // This code was used for mathematical proofs in RAEFGC, Section 5.5.4.2
        // "Three Nonzero Radical Coefficients." In the PDF draft on page 121,
        // there is a polynomial h(u,v,w) in equation (5.42). Page 123 contains
        // a proof that the gradient of h(u,v,w) cannot be (0,0,0) on its domain.
        // It uses arbitrary-precision interval arithmetic. The string numbers
        // in the Get*RootBounds() functions were copied from Mathematica's
        // polynomials whose integer-valued coefficients are computed exactly.
        void GetURootBounds();
        void GetVRootBounds();
        void GetWRootBounds();
        void VerifyGradientValues();
    };
}

UnitTestAPInterval::UnitTestAPInterval()
{
    UTInformation("Mathematics/Arithmetic/APInterval");

    VerifyTwoRootLocalMaximum();
    VerifyGradientValues();
}


void UnitTestAPInterval::VerifyTwoRootLocalMaximum()
{
    double result;
    BSN ru, rusqr, rv, rvsqr, rvalue;
    IEEEBinary64 nbr;

    // delta(u) = 2 - 10 u^2 + u^4 + 4u^6
    double u = 0.4559806241221404;  // estimate from Mathematica
    nbr.number = u;
    std::array<BSN, 4> rcu =
    {
        BSN(2), BSN(-10), BSN(1), BSN(4)
    };

    // I iterated over j to find a floating-point bounding interval for the
    // root. It turns out that 2 iterations was sufficient.
    //for (std::size_t j = 0; j < 16; ++j)
    //{
    //    ru = BSN(nbr.number);
    //    rusqr = ru * ru;
    //    rvalue = rcu[3];
    //    for (std::int32_t i = 2; i >= 0; --i)
    //    {
    //        rvalue = rusqr * rvalue + rcu[i];
    //    }
    //    result = rvalue;
    //    // j = 0, u = 0.45598062412214041 (0x3fdd2ec95b0db677), s = 2.1786639694774773e-16
    //    // j = 1, u = 0.45598062412214047 (0x3fdd2ec95b0db678), s = -2.4106073262015346e-16
    //    nbr.encoding = nbr.GetNextUp();
    //}

    std::array<IEEEBinary64, 2> ubound;
    ubound[0].encoding = 0x3fdd2ec95b0db677ull;  // 0.45598062412214041
    // delta(ubound[0].number) = 2.1786639694774773e-16
    ubound[1].encoding = 0x3fdd2ec95b0db678ull;  // 0.45598062412214047
    // delta(ubound[1].number) = -2.4106073262015346e-16

    // epsilon(v) = -11 + 52 v^2 + 61 v^4 + 4 v^6
    double v = 0.41849918836555849;  // estimate from Mathematica
    nbr.number = v;
    std::array<BSN, 4> rcv =
    {
        BSN(-11), BSN(52), BSN(61), BSN(4)
    };

    // I iterated over j to find a floating-point bounding interval for the
    // root. It turns out that 2 iterations was sufficient.
    //for (std::size_t j = 0; j < 16; ++j)
    //{
    //    rv = BSN(nbr.number);
    //    rvsqr = rv * rv;
    //    rvalue = rcv[3];
    //    for (std::int32_t i = 2; i >= 0; --i)
    //    {
    //        rvalue = rvsqr * rvalue + rcv[i];
    //    }
    //    result = rvalue;
    //    // j = 0, v = 0.41849918836555849 (0x3fdac8b0d1dbb001), s = -3.0502019361243936e-16
    //    // j = 1, v = 0.41849918836555855 (0x3fdac8b0d1dbb002), s = 3.1209272228747110e-15
    //    nbr.encoding = nbr.GetNextUp();
    //}

    std::array<IEEEBinary64, 2> vbound;
    vbound[0].encoding = 0x3fdac8b0d1dbb001ull;  // 0.41849918836555849
    // delta(vbound[0].number) = -3.0502019361243936e-16
    vbound[1].encoding = 0x3fdac8b0d1dbb002ull;  // 0.41849918836555855
    // delta(vbound[1].number) = 3.1209272228747110e-15

    BSNInterval iu(ubound[0].number, ubound[1].number);
    BSNInterval iv(vbound[0].number, vbound[1].number);

    // h = -(1 - u^2)(1 - u^4) + 2(1 - 4u^2 + u^4)v^2 - (1 + u^2)v^4
    BSNInterval t1(1), t2(2), t4(4);
    BSNInterval u2 = iu * iu, v2 = iv * iv;
    BSNInterval ih = (-t1 + u2) * (t1 - u2 * u2) + (t2 * (t1 + u2 * (u2 - t4)) - (t1 + u2) * v2) * v2;
    UTAssert(
        ih[0].GetSign() == -1 &&
        ih[0].GetBiasedExponent() == -324 &&
        ih[0].GetUInteger().GetNumBits() == 324 &&
        ih[0].GetUInteger().GetBits()[0] == 0xa3200571u &&
        ih[0].GetUInteger().GetBits()[1] == 0x24394657u &&
        ih[0].GetUInteger().GetBits()[2] == 0x2346bba1u &&
        ih[0].GetUInteger().GetBits()[3] == 0x82d7ccdau &&
        ih[0].GetUInteger().GetBits()[4] == 0xf57c7dfdu &&
        ih[0].GetUInteger().GetBits()[5] == 0x192d25ebu &&
        ih[0].GetUInteger().GetBits()[6] == 0xe2e7243cu &&
        ih[0].GetUInteger().GetBits()[7] == 0x86877c14u &&
        ih[0].GetUInteger().GetBits()[8] == 0x6cf112d5u &&
        ih[0].GetUInteger().GetBits()[9] == 0x88588e99u &&
        ih[0].GetUInteger().GetBits()[10] == 0x0000000bu,
        "Invalid ih[0]"
    );
    UTAssert(
        ih[1].GetSign() == -1 &&
        ih[1].GetBiasedExponent() == -322 &&
        ih[1].GetUInteger().GetNumBits() == 322 &&
        ih[1].GetUInteger().GetBits()[0] == 0x7a5a22d1u &&
        ih[1].GetUInteger().GetBits()[1] == 0xc9d85f25u &&
        ih[1].GetUInteger().GetBits()[2] == 0x450a2e40u &&
        ih[1].GetUInteger().GetBits()[3] == 0xaf062742u &&
        ih[1].GetUInteger().GetBits()[4] == 0x0c99947du &&
        ih[1].GetUInteger().GetBits()[5] == 0xa4faab67u &&
        ih[1].GetUInteger().GetBits()[6] == 0xa1cd284fu &&
        ih[1].GetUInteger().GetBits()[7] == 0x2d2cb215u &&
        ih[1].GetUInteger().GetBits()[8] == 0x5b3c1762u &&
        ih[1].GetUInteger().GetBits()[9] == 0xe21623a6u &&
        ih[1].GetUInteger().GetBits()[10] == 0x00000002u,
        "Invalid ih[1]"
    );

    // ih interval signs [-,-]
    result = ih[0];
    UTAssert(result == -0.72078757956370176, "Invalid result.");
    result = ih[1];
    UTAssert(result == -0.72078757956370165, "Invalid result.");
}

void UnitTestAPInterval::GetURootBounds()
{
    // s2(u) = sum_{i=0}^{20} k_i u^{2i}
    std::array<BSN, 21> rc =
    {
        "-37207192050",
        "295857979002",
        "9378948799503",
        "-183918665085777",
        "1631553649888447",
        "-5537713445825708",
        "-7183396914975630",
        "70866669687699740",
        "27721799343665642",
        "-317692906318962288",
        "-189673047448949692",
        "553489794922507154",
        "501052800597819744",
        "-133786966364736740",
        "-201135784306801994",
        "-55349780456996148",
        "-18037439531354256",
        "984836331197766",
        "-756197827652475",
        "31513177182231",
        "1189903543929"
    };

    double u1 = 0.2805011059039313;
    BSN ru, rusqr, rvalue;
    IEEEBinary64 nbr;
    nbr.number = u1;

    for (std::size_t j = 0; j < 16; ++j)
    {
        ru = BSN(nbr.number);
        rusqr = ru * ru;
        rvalue = rc[20];
        for (std::int32_t i = 19; i >= 0; --i)
        {
            rvalue = rusqr * rvalue + rc[i];
        }

        // j = 0, u = 0.28050110590393129 (0x3fd1f3bae9165994), s = -6.5974228245723879e-06
        // j = 1, u = 0.28050110590393135 (0x3fd1f3bae9165995), s = 7.7652524599805936e-06
        nbr.encoding = nbr.GetNextUp();
    }

    double u0 = 0.2458553651861940;
    nbr.number = u0;
    rc[0] = -1;
    rc[1] = 16;
    rc[2] = 9;
    for (std::size_t j = 0; j < 16; ++j)
    {
        ru = BSN(nbr.number);
        rusqr = ru * ru;
        rvalue = rc[0] + (rc[1] + rc[2] * rusqr) * rusqr;

        // j = 0, u = 0.24585536518619397 (0x3fcf78304882aae3), s = 1.9873956435515848e-17
        // j = 1, u = 0.24585536518619394 (0x3fcf78304882aae2), s = -2.1333828874176199e-16
        nbr.encoding = nbr.GetNextDown();
    }

    double u2 = 0.4559806241221404;
    nbr.number = u2;
    rc[0] = 2;
    rc[1] = -10;
    rc[2] = 1;
    rc[3] = 4;
    for (std::size_t j = 0; j < 16; ++j)
    {
        ru = BSN(nbr.number);
        rusqr = ru * ru;
        rvalue = rc[0] + rc[1] * rusqr + rc[2] * rusqr * rusqr + rc[3] * rusqr * rusqr * rusqr;

        // j = 0, u = 0.45598062412214041 (0x3fdd2ec95b0db677), s = 2.1786639694774773e-16
        // j = 1, u = 0.45598062412214047 (0x3fdd2ec95b0db678), s = -2.4106073262015346e-16
        nbr.encoding = nbr.GetNextUp();
    }

    double w4 = 0.1813345817725104;  // -3 + 16w + 3w^2
    double w1 = 5.514667915105844;   // -3 - 16w + 3w^2

    std::array<BSN, 31> rd =
    {
        "+1391560839045",
        "-3983111121265731",
        "+4745336658054607269",
        "+479194431809955841692",
        "+12809700737843563377468",
        "-96976610634748943327964",
        "-27865601615770607911434154",
        "-910845339635475353308187978",
        "-14787704363974503341297382197",
        "-146934721443480754361541353615",
        "-928327412190498159192435696797",
        "-3243988552551817526843258581866",
        "-1510007604308936622254213882294",
        "+21581324320977844715472611373008",
        "+50842058578360484029963603772404",
        "-82699842189925900531663228570324",
        "-300026352169897193341095454659221",
        "+117228474853643630390207758570047",
        "+739084359177408279003857676742763",
        "+45598733061414256740620393432888",
        "-832978966645207643096480387351064",
        "-246488304251518598247876561546532",
        "+238398686796573452394409140795222",
        "-57867673246248900945946861201026",
        "-23743773876794114453384688581691",
        "+67295775343736377537981464083731",
        "-6054993176196893821079946205491",
        "+176490069674387646836473845814",
        "+1196530284796341605301618450",
        "-122802199917586439728766544",
        "+212123835947363329094688"
    };

    // wbound[4], wbound[7] of GetVerifyGradientValues().
    nbr.number = w4;
    rd[0] = -3;
    rd[1] = 16;
    rd[2] = 3;
    for (std::size_t j = 0; j < 16; ++j)
    {
        ru = BSN(nbr.number);
        rvalue = rd[0] + (rd[1] + rd[2] * ru) * ru;

        // j = 0, u = 0.24585536518619397 (0x3fcf78304882aae3), s = 1.9873956435515848e-17
        // j = 1, u = 0.24585536518619394 (0x3fcf78304882aae2), s = -2.1333828874176199e-16
        nbr.encoding = nbr.GetNextDown();
    }

    // wbound[1], wbound[10] of GetVerifyGradientValues().
    nbr.number = w1;
    rd[0] = -3;
    rd[1] = -16;
    rd[2] = 3;
    for (std::size_t j = 0; j < 16; ++j)
    {
        ru = BSN(nbr.number);
        rvalue = rd[0] + (rd[1] + rd[2] * ru) * ru;

        // j = 0, u = 0.24585536518619397 (0x3fcf78304882aae3), s = 1.9873956435515848e-17
        // j = 1, u = 0.24585536518619394 (0x3fcf78304882aae2), s = -2.1333828874176199e-16
        nbr.encoding = nbr.GetNextDown();
    }

    BSN send0, send1;

    // wbound[0], wbound[11] of GetVerifyGradientValues().
    ru = 23.80;
    rusqr = ru * ru;
    send0 = rd[30];
    for (std::int32_t i = 29; i >= 0; --i)
    {
        send0 = rusqr * send0 + rd[i];
    }
    ru = 23.81;
    rusqr = ru * ru;
    send1 = rd[30];
    for (std::int32_t i = 29; i >= 0; --i)
    {
        send1 = rusqr * send1 + rd[i];
    }
    UTAssert(send0.GetSign() == -send1.GetSign(), "invalid");

    // wbound[2], wbound[9] of GetVerifyGradientValues().
    ru = 4.90;
    rusqr = ru * ru;
    send0 = rd[30];
    for (std::int32_t i = 29; i >= 0; --i)
    {
        send0 = rusqr * send0 + rd[i];
    }
    ru = 4.91;
    rusqr = ru * ru;
    send1 = rd[30];
    for (std::int32_t i = 29; i >= 0; --i)
    {
        send1 = rusqr * send1 + rd[i];
    }
    UTAssert(send0.GetSign() == -send1.GetSign(), "invalid");

    // wbound[3], wbound[8] of GetVerifyGradientValues().
    ru = 1.294;
    rusqr = ru * ru;
    send0 = rd[30];
    for (std::int32_t i = 29; i >= 0; --i)
    {
        send0 = rusqr * send0 + rd[i];
    }
    ru = 1.295;
    rusqr = ru * ru;
    send1 = rd[30];
    for (std::int32_t i = 29; i >= 0; --i)
    {
        send1 = rusqr * send1 + rd[i];
    }
    UTAssert(send0.GetSign() == -send1.GetSign(), "invalid");

    // wbound[5], wbound[6] of GetVerifyGradientValues().
    ru = 0.15;
    rusqr = ru * ru;
    send0 = rd[30];
    for (std::int32_t i = 29; i >= 0; --i)
    {
        send0 = rusqr * send0 + rd[i];
    }
    ru = 0.16;
    rusqr = ru * ru;
    send1 = rd[30];
    for (std::int32_t i = 29; i >= 0; --i)
    {
        send1 = rusqr * send1 + rd[i];
    }
    UTAssert(send0.GetSign() == -send1.GetSign(), "invalid");
}

void UnitTestAPInterval::GetVRootBounds()
{
    std::array<BSN, 28> rc =
    {
        "+74414384100",
        "+155643494868",
        "-65205940914288",
        "+97872501823587",
        "+859664766067351",
        "-83644731626888",
        "-8378892121350483",
        "+41139441484229580",
        "-106686021507997833",
        "-87245585284253496",
        "+501896793300448449",
        "+509120382666076287",
        "-2364120025411535826",
        "-1404525036987932844",
        "+11335389399076804146",
        "-7992137346092838492",
        "-16104631965666965046",
        "+26027399972887452696",
        "-7035729700775046702",
        "-17534821017911090187",
        "+25062244171077410235",
        "-5967023472855418524",
        "-8907156672054874111",
        "+6549045902462192096",
        "-1539298998923024805",
        "+19474334318491164",
        "+23826574792273149",
        "+445744566598617"
    };

    //double v = 0.1907179139182551;
    double v = 0.5124662216418173;
    BSN rv, rvsqr, rvalue;
    IEEEBinary64 nbr;
    nbr.number = v;
    // nbr.encoding = 0x3fc86971d18526df, 0.19071791391825507 (vs 15.x)
    // nbr.encoding = 0x3fc86971d18526e0, 0.19071791391825510 (vs 16.3.2)

    for (std::size_t j = 0; j < 16; ++j)
    {
        rv = BSN(nbr.number);
        rvsqr = rv * rv;
        rvalue = rc[27];
        for (std::int32_t i = 26; i >= 0; --i)
        {
            rvalue = rvsqr * rvalue + rc[i];
        }

        //j = 0, v = 0.19071791391825510 (0x3fc86971d18526e0), s = -7.3303052262220213e-05
        //j = 1, v = 0.19071791391825507 (0x3fc86971d18526df), s = -3.0560581156926929e-05
        //j = 2, v = 0.19071791391825504 (0x3fc86971d18526de), s = 1.2181889948366342e-05
        //nbr.encoding = nbr.GetNextDown();

        // j = 0, v = 0.51246622164181732 (0x3fe0661f8fc8338d), s = 0.00044564026100726384
        // j = 1, v = 0.51246622164181721 (0x3fe0661f8fc8338c), s = -0.0012715849999477022
        nbr.encoding = nbr.GetNextDown();
    }
}

void UnitTestAPInterval::GetWRootBounds()
{
    std::array<BSN, 25> rc =
    {
        "-256091138580607926399",
        "+1093308221197288596786264",
        "+7280663482591251009855723",
        "-121853951727202026255063447",
        "-503572897510857164461540896",
        "+10877800946546215966445453097",
        "+80725539761353277481926975076",
        "-137601295890337351078993376380",
        "-2599739927351509316631801762107",
        "+619595751289636341922295358858",
        "+112263628006214429022327361352139",
        "+741260790963741209215156011348689",
        "+2740976954154853195735429081541350",
        "+6977299805749267464892406295722693",
        "+13291416116930569656259582262979718",
        "+19909147954328449889308630539439538",
        "+24178278006113349122210541526422244",
        "+23820758385158671809950489619265472",
        "+18171166392240090153842906980985968",
        "+9890213980206875315968290791053824",
        "+3587295685308401289864664764948480",
        "+894955765635425721184564365688832",
        "+184074965659535748250232368660480",
        "+32116511091610982150684371058688",
        "+456168605333707820786144772096"
    };

    double w = 0.01529288293808486;
    BSN rw, rwsqr, rvalue;
    IEEEBinary64 nbr;
    nbr.number = w;

    for (std::size_t j = 0; j < 16; ++j)
    {
        rw = BSN(nbr.number);
        rwsqr = rw * rw;
        rvalue = rc[24];
        for (std::int32_t i = 23; i >= 0; --i)
        {
            rvalue = rwsqr * rvalue + rc[i];
        }

        //j = 0, w = 0.015292882938084860 (0x3f8f51e000a521cc), s = 173553.67446702754
        //j = 1, w = 0.015292882938084859 (0x3f8f51e000a521cb), s = 115365.46937912672
        //j = 2, w = 0.015292882938084857 (0x3f8f51e000a521ca), s = 57177.264291225911
        //j = 3, w = 0.015292882938084855 (0x3f8f51e000a521c9), s = -1010.9407966748904
        nbr.encoding = nbr.GetNextDown();
    }
}

void UnitTestAPInterval::VerifyGradientValues()
{
    // The u0bound, u1bound and u2bound intervals are computed in
    // GetURootBounds().
    std::array<double, 2> u0bound =
    {
        0.24585536518619394, 0.24585536518619397
    };

    std::array<double, 2> u1bound =
    {
        0.28050110590393129, 0.28050110590393135
    };

    std::array<double, 2> u2bound =
    {
        0.45598062412214041, 0.45598062412214047
    };

    std::array<std::array<double, 2>, 12> wbound =
    { {
        { -23.81, -23.80 },
        { -5.515, -5.514 },
        { -4.91, -4.90 },
        { -1.295, -1.294 },
        { -0.1814, -0.1813 },
        { -0.16, -0.15 },
        { 0.15, 0.16 },
        { 0.1813, 0.1814 },
        { 1.294, 1.295 },
        { 4.90, 4.91 },
        { 5.514, 5.515 },
        { 23.80, 23.81 }
    } };

    BSNInterval iu, iv, iw;
    BSNInterval dhdu, dhdv, dhdw;

    std::array<std::array<std::int32_t, 3>, 12> signs0 =
    { {
        { +1, -1 , +1 },
        { +1, -1 , +1 },
        { +1, -1 , +1 },
        { +1, -1 , +1 },
        { -1, -1 , -1 },
        { -1, -1 , -1 },
        { -1, +1 , +1 },
        { -1, +1 , +1 },
        { +1, +1 , -1 },
        { +1, +1 , -1 },
        { +1, +1 , -1 },
        { +1, +1 , -1 }
    } };
    iu = BSNInterval(u1bound[0], u1bound[1]);
    iv = BSNInterval(u0bound[0], u0bound[1]);
    for (std::size_t i = 0; i < 12; ++i)
    {
        iw = BSNInterval(wbound[i][0], wbound[i][1]);
        dhdu = DhDu(iu, iv, iw);  // + + + + - - - - + + + +
        dhdv = DhDv(iu, iv, iw);  // - - - - - - + + + + + +
        dhdw = DhDw(iu, iv, iw);  // + + + + - - + + - - - -
        UTAssert(
            dhdu[0].GetSign() == signs0[i][0] && dhdu[1].GetSign() == signs0[i][0] &&
            dhdv[0].GetSign() == signs0[i][1] && dhdv[1].GetSign() == signs0[i][1] &&
            dhdw[0].GetSign() == signs0[i][2] && dhdw[1].GetSign() == signs0[i][2],
            "Incorrect signs.");
    }

    std::array<std::array<std::int32_t, 3>, 12> signs1 =
    { {
        { +1, -1 , +1 },
        { +1, -1 , +1 },
        { +1, -1 , +1 },
        { +1, -1 , +1 },
        { -1, -1 , -1 },
        { -1, -1 , -1 },
        { -1, +1 , -1 },
        { -1, +1 , -1 },
        { +1, +1 , -1 },
        { +1, +1 , -1 },
        { +1, +1 , -1 },
        { +1, +1 , -1 }
    } };
    iu = BSNInterval(u2bound[0], u2bound[1]);
    iv = BSNInterval(u0bound[0], u0bound[1]);
    for (std::size_t i = 0; i < 12; ++i)
    {
        iw = BSNInterval(wbound[i][0], wbound[i][1]);
        dhdu = DhDu(iu, iv, iw);  // + + + + - - - - + + + +
        dhdv = DhDv(iu, iv, iw);  // - - - - - - + + + + + +
        dhdw = DhDw(iu, iv, iw);  // + + + + - - - - - - - -
        UTAssert(
            dhdu[0].GetSign() == signs1[i][0] && dhdu[1].GetSign() == signs1[i][0] &&
            dhdv[0].GetSign() == signs1[i][1] && dhdv[1].GetSign() == signs1[i][1] &&
            dhdw[0].GetSign() == signs1[i][2] && dhdw[1].GetSign() == signs1[i][2],
            "Incorrect signs.");
    }

    std::array<std::array<std::int32_t, 3>, 12> signs2 =
    { {
        { +1, +1 , +1 },
        { +1, -1 , +1 },
        { +1, -1 , +1 },
        { +1, -1 , +1 },
        { -1, -1 , -1 },
        { -1, -1 , -1 },
        { -1, +1 , -1 },
        { -1, +1 , -1 },
        { +1, +1 , -1 },
        { +1, +1 , -1 },
        { +1, +1 , -1 },
        { +1, +1 , -1 }
    } };
    iu = BSNInterval(u2bound[0], u2bound[1]);
    iv = BSNInterval(u1bound[0], u1bound[1]);
    for (std::size_t i = 0; i < 12; ++i)
    {
        iw = BSNInterval(wbound[i][0], wbound[i][1]);
        dhdu = DhDu(iu, iv, iw);  // + + + + - - - - + + + +
        dhdv = DhDv(iu, iv, iw);  // + - - - - - + + + + + +
        dhdw = DhDw(iu, iv, iw);  // + + + + - - - - - - - -
        UTAssert(
            dhdu[0].GetSign() == signs2[i][0] && dhdu[1].GetSign() == signs2[i][0] &&
            dhdv[0].GetSign() == signs2[i][1] && dhdv[1].GetSign() == signs2[i][1] &&
            dhdw[0].GetSign() == signs2[i][2] && dhdw[1].GetSign() == signs2[i][2],
            "Incorrect signs.");
    }
}

#else

#include <GTL/Mathematics/Arithmetic/APInterval.h>

namespace gtl
{
    using BSN = BSNumber<UIntegerAP32>;
    template class APInterval<BSN>;
    template APInterval<BSN> operator+(APInterval<BSN> const&);
    template APInterval<BSN> operator-(APInterval<BSN> const&);
    template APInterval<BSN> operator+(BSN const&, APInterval<BSN> const&);
    template APInterval<BSN> operator+(APInterval<BSN> const&, BSN const&);
    template APInterval<BSN> operator+(APInterval<BSN> const&, APInterval<BSN> const&);
    template APInterval<BSN> operator-(BSN const&, APInterval<BSN> const&);
    template APInterval<BSN> operator-(APInterval<BSN> const&, BSN const&);
    template APInterval<BSN> operator-(APInterval<BSN> const&, APInterval<BSN> const&);
    template APInterval<BSN> operator*(BSN const&, APInterval<BSN> const&);
    template APInterval<BSN> operator*(APInterval<BSN> const&, BSN const&);
    template APInterval<BSN> operator*(APInterval<BSN> const&, APInterval<BSN> const&);

    using BSR = BSRational<UIntegerAP32>;
    template class APInterval<BSR>;
    template APInterval<BSR> operator+(APInterval<BSR> const&);
    template APInterval<BSR> operator-(APInterval<BSR> const&);
    template APInterval<BSR> operator+(BSR const&, APInterval<BSR> const&);
    template APInterval<BSR> operator+(APInterval<BSR> const&, BSR const&);
    template APInterval<BSR> operator+(APInterval<BSR> const&, APInterval<BSR> const&);
    template APInterval<BSR> operator-(BSR const&, APInterval<BSR> const&);
    template APInterval<BSR> operator-(APInterval<BSR> const&, BSR const&);
    template APInterval<BSR> operator-(APInterval<BSR> const&, APInterval<BSR> const&);
    template APInterval<BSR> operator*(BSR const&, APInterval<BSR> const&);
    template APInterval<BSR> operator*(APInterval<BSR> const&, BSR const&);
    template APInterval<BSR> operator*(APInterval<BSR> const&, APInterval<BSR> const&);
    template APInterval<BSR> operator/(BSR const&, APInterval<BSR> const&);
    template APInterval<BSR> operator/(APInterval<BSR> const&, BSR const&);
    template APInterval<BSR> operator/(APInterval<BSR> const&, APInterval<BSR> const&);
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(APInterval)
