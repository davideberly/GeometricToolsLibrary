// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/BSPrecision.h>
using namespace gtl;

namespace gtl
{
    class UnitTestBSPrecision
    {
    public:
        UnitTestBSPrecision();

    private:
        // Code used in RAEFGC in Listing 3.9.
        void SumsProductsRatios();

        // Code used in RAEFGC in Listing 3.10.
        void Determinants();

        // Code used in RAEFGC in Section 4.3 "Primal Queries".
        void ToLine();
        void ToCircumcircle();
        void ToPlane();
        void ToCircumsphere();
    };
}

UnitTestBSPrecision::UnitTestBSPrecision()
{
    UTInformation("Mathematics/Arithmetic/BSPrecision");

    SumsProductsRatios();
    Determinants();
    ToLine();
    ToCircumcircle();
    ToPlane();
    ToCircumsphere();
}

void UnitTestBSPrecision::SumsProductsRatios()
{
    BSPrecision fx(BSPrecision::Type::IS_FLOAT);
    BSPrecision fy(BSPrecision::Type::IS_FLOAT);
    UTAssert(
        fx.bsn.minExponent == -149 &&
        fx.bsn.maxExponent == 127 &&
        fx.bsn.maxBits == 24 &&
        fx.bsn.maxWords == 1 &&
        fx.bsr == fx.bsn,
        "Incorrect fx precision results.");

    BSPrecision dx(BSPrecision::Type::IS_DOUBLE);
    BSPrecision dy(BSPrecision::Type::IS_DOUBLE);
    UTAssert(
        dx.bsn.minExponent == -1074 &&
        dx.bsn.maxExponent == 1023 &&
        dx.bsn.maxBits == 53 &&
        dx.bsn.maxWords == 2 &&
        dx.bsr == dx.bsn,
        "Incorrect dx precision results.");

    BSPrecision ix(BSPrecision::Type::IS_INT32);
    BSPrecision iy(BSPrecision::Type::IS_INT32);
    UTAssert(
        ix.bsn.minExponent == 0 &&
        ix.bsn.maxExponent == 30 &&
        ix.bsn.maxBits == 31 &&
        ix.bsn.maxWords == 1 &&
        ix.bsr == ix.bsn,
        "Incorrect ix precision results.");

    BSPrecision sum, product, ratio;

    // sum and product of 'float' type
    sum = fx + fy;
    UTAssert(
        sum.bsn.minExponent == -149 &&
        sum.bsn.maxExponent == 128 &&
        sum.bsn.maxBits == 277 &&
        sum.bsn.maxWords == 9,
        "Incorrect sum.bsn results.");
    UTAssert(
        sum.bsr.minExponent == -298 &&
        sum.bsr.maxExponent == 256 &&
        sum.bsr.maxBits == 554 &&
        sum.bsr.maxWords == 18,
        "Incorrect sum.bsr results.");

    product = fx * fy;
    UTAssert(
        product.bsn.minExponent == -298 &&
        product.bsn.maxExponent == 255 &&
        product.bsn.maxBits == 48 &&
        product.bsn.maxWords == 2,
        "Incorrect product.bsn results.");
    UTAssert(
        product.bsr.minExponent == -298 &&
        product.bsr.maxExponent == 255 &&
        product.bsr.maxBits == 48 &&
        product.bsr.maxWords == 2,
        "Incorrect product.bsr results.");

    // sum and product of 'double' type
    sum = dx + dy;
    UTAssert(
        sum.bsn.minExponent == -1074 &&
        sum.bsn.maxExponent == 1024 &&
        sum.bsn.maxBits == 2098 &&
        sum.bsn.maxWords == 66,
        "Incorrect sum.bsn results.");
    UTAssert(
        sum.bsr.minExponent == -2148 &&
        sum.bsr.maxExponent == 2048 &&
        sum.bsr.maxBits == 4196 &&
        sum.bsr.maxWords == 132,
        "Incorrect sum.bsr results.");

    product = dx * dy;
    UTAssert(
        product.bsn.minExponent == -2148 &&
        product.bsn.maxExponent == 2047 &&
        product.bsn.maxBits == 106 &&
        product.bsn.maxWords == 4,
        "Incorrect product.bsn results.");
    UTAssert(
        product.bsr.minExponent == -2148 &&
        product.bsr.maxExponent == 2047 &&
        product.bsr.maxBits == 106 &&
        product.bsr.maxWords == 4,
        "Incorrect product.bsr results.");

    // sum and product of 'std::int32_t' type
    sum = ix + iy;
    UTAssert(
        sum.bsn.minExponent == 0 &&
        sum.bsn.maxExponent == 31 &&
        sum.bsn.maxBits == 32 &&
        sum.bsn.maxWords == 1,
        "Incorrect sum.bsn results.");
    UTAssert(
        sum.bsr.minExponent == 0 &&
        sum.bsr.maxExponent == 62 &&
        sum.bsr.maxBits == 63 &&
        sum.bsr.maxWords == 2,
        "Incorrect sum.bsr results.");

    product = ix * iy;
    UTAssert(
        product.bsn.minExponent == 0 &&
        product.bsn.maxExponent == 61 &&
        product.bsn.maxBits == 62 &&
        product.bsn.maxWords == 2,
        "Incorrect product.bsn results.");
    UTAssert(
        product.bsr.minExponent == 0 &&
        product.bsr.maxExponent == 61 &&
        product.bsr.maxBits == 62 &&
        product.bsr.maxWords == 2,
        "Incorrect product.bsr results.");

    // sum and product of mixed types 'float' and 'double'
    sum = dx + fy;  // same for fx + dy
    UTAssert(
        sum.bsn.minExponent == -1074 &&
        sum.bsn.maxExponent == 1023 &&
        sum.bsn.maxBits == 1173 &&
        sum.bsn.maxWords == 37,
        "Incorrect sum.bsn results.");
    UTAssert(
        sum.bsr.minExponent == -1223 &&
        sum.bsr.maxExponent == 1152 &&
        sum.bsr.maxBits == 2375 &&
        sum.bsr.maxWords == 75,
        "Incorrect sum.bsr results.");

    product = dx * fy;  // same for fx * dy
    UTAssert(
        product.bsn.minExponent == -1223 &&
        product.bsn.maxExponent == 1151 &&
        product.bsn.maxBits == 77 &&
        product.bsn.maxWords == 3,
        "Incorrect product.bsn results.");
    UTAssert(
        product.bsr.minExponent == -1223 &&
        product.bsr.maxExponent == 1151 &&
        product.bsr.maxBits == 77 &&
        product.bsr.maxWords == 3,
        "Incorrect product.bsr results.");

    // sum and product of mixed types 'std::int32_t' and 'float'
    sum = ix + fy;  // same for fx + iy
    UTAssert(
        sum.bsn.minExponent == -149 &&
        sum.bsn.maxExponent == 127 &&
        sum.bsn.maxBits == 128 &&
        sum.bsn.maxWords == 4,
        "Incorrect sum.bsn results.");
    UTAssert(
        sum.bsr.minExponent == -149 &&
        sum.bsr.maxExponent == 159 &&
        sum.bsr.maxBits == 308 &&
        sum.bsr.maxWords == 10,
        "Incorrect sum.bsr results.");

    product = ix * fy;  // same for fx * iy
    UTAssert(
        product.bsn.minExponent == -149 &&
        product.bsn.maxExponent == 158 &&
        product.bsn.maxBits == 55 &&
        product.bsn.maxWords == 2,
        "Incorrect product.bsn results.");
    UTAssert(
        product.bsr.minExponent == -149 &&
        product.bsr.maxExponent == 158 &&
        product.bsr.maxBits == 55 &&
        product.bsr.maxWords == 2,
        "Incorrect product.bsr results.");

    ratio = fx / fy;
    UTAssert(
        ratio.bsr.minExponent == -298 &&
        ratio.bsr.maxExponent == 255 &&
        ratio.bsr.maxBits == 48 &&
        ratio.bsr.maxWords == 2,
        "Incorrect ratio.bsr results.");

    ratio = dx / dy;
    UTAssert(
        ratio.bsr.minExponent == -2148 &&
        ratio.bsr.maxExponent == 2047 &&
        ratio.bsr.maxBits == 106 &&
        ratio.bsr.maxWords == 4,
        "Incorrect ratio.bsr results.");

    ratio = ix / iy;
    UTAssert(
        ratio.bsr.minExponent == 0 &&
        ratio.bsr.maxExponent == 61 &&
        ratio.bsr.maxBits == 62 &&
        ratio.bsr.maxWords == 2,
        "Incorrect ratio.bsr results.");

    ratio = fx / iy;  // same for ix / fy
    UTAssert(
        ratio.bsr.minExponent == -149 &&
        ratio.bsr.maxExponent == 158 &&
        ratio.bsr.maxBits == 55 &&
        ratio.bsr.maxWords == 2,
        "Incorrect ratio.bsr results.");
}

void UnitTestBSPrecision::Determinants()
{
    BSPrecision fx(BSPrecision::Type::IS_FLOAT);
    BSPrecision dx(BSPrecision::Type::IS_DOUBLE);

    auto fdet2 = fx * fx - fx * fx;
    UTAssert(
        fdet2.bsn.minExponent == -298 &&
        fdet2.bsn.maxExponent == 256 &&
        fdet2.bsn.maxBits == 554 &&
        fdet2.bsn.maxWords == 18,
        "Incorrect fdet2.bsn results.");
    UTAssert(
        fdet2.bsr.minExponent == -596 &&
        fdet2.bsr.maxExponent == 512 &&
        fdet2.bsr.maxBits == 1108 &&
        fdet2.bsr.maxWords == 35,
        "Incorrect fdet2.bsr results.");

    auto ddet2 = dx * dx - dx * dx;
    UTAssert(
        ddet2.bsn.minExponent == -2148 &&
        ddet2.bsn.maxExponent == 2048 &&
        ddet2.bsn.maxBits == 4196 &&
        ddet2.bsn.maxWords == 132,
        "Incorrect ddet2.bsn results.");
    UTAssert(
        ddet2.bsr.minExponent == -4296 &&
        ddet2.bsr.maxExponent == 4096 &&
        ddet2.bsr.maxBits == 8392 &&
        ddet2.bsr.maxWords == 263,
        "Incorrect ddet2.bsr results.");

    auto fdet3 = fx * fdet2 - fx * fdet2 + fx * fdet2;
    UTAssert(
        fdet3.bsn.minExponent == -447 &&
        fdet3.bsn.maxExponent == 386 &&
        fdet3.bsn.maxBits == 834 &&
        fdet3.bsn.maxWords == 27,
        "Incorrect fdet3.bsn results.");
    UTAssert(
        fdet3.bsr.minExponent == -2235 &&
        fdet3.bsr.maxExponent == 1924 &&
        fdet3.bsr.maxBits == 4160 &&
        fdet3.bsr.maxWords == 130,
        "Incorrect fdet3.bsr results.");

    auto ddet3 = dx * ddet2 - dx * ddet2 + dx * ddet2;
    UTAssert(
        ddet3.bsn.minExponent == -3222 &&
        ddet3.bsn.maxExponent == 3074 &&
        ddet3.bsn.maxBits == 6297 &&
        ddet3.bsn.maxWords == 197,
        "Incorrect ddet3.bsn results.");
    UTAssert(
        ddet3.bsr.minExponent == -16110 &&
        ddet3.bsr.maxExponent == 15364 &&
        ddet3.bsr.maxBits == 31475 &&
        ddet3.bsr.maxWords == 984,
        "Incorrect ddet3.bsr results.");

    // The determinant is computed using 3 different expressions that lead
    // to 3 different BSPrecision results. The last expression generates
    // smaller numbers by quite a bit. TODO: Figure out what is going on here.
    // The first expression uses the Laplace expansion theorem and obtains
    // minimum operation count. The last expression uses a cofactor expansion
    // by a row (or column), but leads to a smaller number of words. This does
    // not make sense to me.
    auto fdet4 = fdet2 * fdet2 - fdet2 * fdet2 + fdet2 * fdet2 + fdet2 * fdet2 - fdet2 * fdet2 + fdet2 * fdet2;
    UTAssert(
        fdet4.bsn.minExponent == -596 &&
        fdet4.bsn.maxExponent == 518 &&
        fdet4.bsn.maxBits == 1115 &&
        fdet4.bsn.maxWords == 35,
        "Incorrect fdet4.bsn (expression0) results.");
    UTAssert(
        fdet4.bsr.minExponent == -7152 &&
        fdet4.bsr.maxExponent == 6160 &&
        fdet4.bsr.maxBits == 13313 &&
        fdet4.bsr.maxWords == 417,
        "Incorrect fdet4.bsr (expression0) results.");

    auto temp0 = fdet2 * fdet2;
    auto temp1 = temp0 + temp0;
    auto temp2 = temp1 + temp1;
    fdet4 = temp2 + temp1;
    UTAssert(
        fdet4.bsn.minExponent == -596 &&
        fdet4.bsn.maxExponent == 516 &&
        fdet4.bsn.maxBits == 1113 &&
        fdet4.bsn.maxWords == 35,
        "Incorrect fdet4.bsn (expression1) results.");
    UTAssert(
        fdet4.bsr.minExponent == -7152 &&
        fdet4.bsr.maxExponent == 6160 &&
        fdet4.bsr.maxBits == 13313 &&
        fdet4.bsr.maxWords == 417,
        "Incorrect fdet4.bsr (expression1) results.");

    fdet4 = fx * fdet3 - fx * fdet3 + fx * fdet3 - fx * fdet3;
    UTAssert(
        fdet4.bsn.minExponent == -596 &&
        fdet4.bsn.maxExponent == 517 &&
        fdet4.bsn.maxBits == 1114 &&
        fdet4.bsn.maxWords == 35,
        "Incorrect fdet4.bsn (expression2) results.");
    UTAssert(
        fdet4.bsr.minExponent == -9536 &&
        fdet4.bsr.maxExponent == 8214 &&
        fdet4.bsr.maxBits == 17751 &&
        fdet4.bsr.maxWords == 555,
        "Incorrect fdet4.bsr (expression2) results.");

    // The determinant is computed using 3 different expressions that lead
    // to 3 different BSPrecision results. The last expression generates
    // smaller numbers by quite a bit. TODO: Figure out what is going on here.
    // The first expression uses the Laplace expansion theorem and obtains
    // minimum operation count. The last expression uses a cofactor expansion
    // by a row (or column), but leads to a smaller number of words. This does
    // not make sense to me.
    auto ddet4 = ddet2 * ddet2 - ddet2 * ddet2 + ddet2 * ddet2 + ddet2 * ddet2 - ddet2 * ddet2 + ddet2 * ddet2;
    UTAssert(
        ddet4.bsn.minExponent == -4296 &&
        ddet4.bsn.maxExponent == 4102 &&
        ddet4.bsn.maxBits == 8399 &&
        ddet4.bsn.maxWords == 263,
        "Incorrect ddet4.bsn (expression0) results.");
    UTAssert(
        ddet4.bsr.minExponent == -51552 &&
        ddet4.bsr.maxExponent == 49168 &&
        ddet4.bsr.maxBits == 100721 &&
        ddet4.bsr.maxWords == 3148,
        "Incorrect ddet4.bsr (expression0) results.");

    temp0 = ddet2 * ddet2;
    temp1 = temp0 + temp0;
    temp2 = temp1 + temp1;
    ddet4 = temp2 + temp1;
    UTAssert(
        ddet4.bsn.minExponent == -4296 &&
        ddet4.bsn.maxExponent == 4100 &&
        ddet4.bsn.maxBits == 8397 &&
        ddet4.bsn.maxWords == 263,
        "Incorrect ddet4.bsn (expression1) results.");
    UTAssert(
        ddet4.bsr.minExponent == -51552 &&
        ddet4.bsr.maxExponent == 49168 &&
        ddet4.bsr.maxBits == 100721 &&
        ddet4.bsr.maxWords == 3148,
        "Incorrect ddet4.bsr (expression1) results.");

    ddet4 = dx * ddet3 - dx * ddet3 + dx * ddet3 - dx * ddet3;
    UTAssert(
        ddet4.bsn.minExponent == -4296 &&
        ddet4.bsn.maxExponent == 4101 &&
        ddet4.bsn.maxBits == 8398 &&
        ddet4.bsn.maxWords == 263,
        "Incorrect ddet4.bsn (expression2) results.");
    UTAssert(
        ddet4.bsr.minExponent == -68736 &&
        ddet4.bsr.maxExponent == 65558 &&
        ddet4.bsr.maxBits == 134295 &&
        ddet4.bsr.maxWords == 4197,
        "Incorrect ddet4.bsr (expression2) results.");
}

void UnitTestBSPrecision::ToLine()
{
    BSPrecision fx(BSPrecision::Type::IS_FLOAT);
    auto fdiff = fx - fx;
    auto fdet = fdiff * fdiff - fdiff * fdiff;
    UTAssert(
        fdet.bsn.minExponent == -298 &&
        fdet.bsn.maxExponent == 258 &&
        fdet.bsn.maxBits == 557 &&
        fdet.bsn.maxWords == 18,
        "Incorrect fdet.bsn results.");
    UTAssert(
        fdet.bsr.minExponent == -1192 &&
        fdet.bsr.maxExponent == 1028 &&
        fdet.bsr.maxBits == 2221 &&
        fdet.bsr.maxWords == 70,
        "Incorrect fdet.bsr results.");

    BSPrecision dx(BSPrecision::Type::IS_DOUBLE);
    auto ddiff = dx - dx;
    auto ddet = ddiff * ddiff - ddiff * ddiff;
    UTAssert(
        ddet.bsn.minExponent == -2148 &&
        ddet.bsn.maxExponent == 2050 &&
        ddet.bsn.maxBits == 4199 &&
        ddet.bsn.maxWords == 132,
        "Incorrect ddet.bsn results.");
    UTAssert(
        ddet.bsr.minExponent == -8592 &&
        ddet.bsr.maxExponent == 8196 &&
        ddet.bsr.maxBits == 16789 &&
        ddet.bsr.maxWords == 525,
        "Incorrect ddet.bsr results.");
}

void UnitTestBSPrecision::ToCircumcircle()
{
    BSPrecision det;
    {
        BSPrecision v0x(BSPrecision::Type::IS_FLOAT), v0y(BSPrecision::Type::IS_FLOAT);
        BSPrecision v1x(BSPrecision::Type::IS_FLOAT), v1y(BSPrecision::Type::IS_FLOAT);
        BSPrecision v2x(BSPrecision::Type::IS_FLOAT), v2y(BSPrecision::Type::IS_FLOAT);
        BSPrecision px(BSPrecision::Type::IS_FLOAT), py(BSPrecision::Type::IS_FLOAT);
        auto x0 = v0x - px;
        auto y0 = v0y - py;
        auto s00 = v0x + px;
        auto s01 = v0y + py;
        auto t00 = s00 * x0;
        auto t01 = s01 * y0;
        auto z0 = t00 + t01;

        auto x1 = v1x - px;
        auto y1 = v1y - py;
        auto s10 = v1x + px;
        auto s11 = v1y + py;
        auto t10 = s10 * x1;
        auto t11 = s11 * y1;
        auto z1 = t10 + t11;

        auto x2 = v2x - px;
        auto y2 = v2y - py;
        auto s20 = v2x + px;
        auto s21 = v2y + py;
        auto t20 = s20 * x2;
        auto t21 = s21 * y2;
        auto z2 = t20 + t21;

        auto y0z1 = y0 * z1;
        auto y0z2 = y0 * z2;
        auto y1z0 = y1 * z0;
        auto y1z2 = y1 * z2;
        auto y2z0 = y2 * z0;
        auto y2z1 = y2 * z1;
        auto c0 = y1z2 - y2z1;
        auto c1 = y2z0 - y0z2;
        auto c2 = y0z1 - y1z0;
        auto x0c0 = x0 * c0;
        auto x1c1 = x1 * c1;
        auto x2c2 = x2 * c2;
        auto term = x0c0 + x1c1;
        det = term + x2c2;
        UTAssert(
            det.bsn.minExponent == -596 &&
            det.bsn.maxExponent == 519 &&
            det.bsn.maxBits == 1116 &&
            det.bsn.maxWords == 35,
            "Incorrect det.bsn results.");
        UTAssert(
            det.bsr.minExponent == -9834 &&
            det.bsr.maxExponent == 8491 &&
            det.bsr.maxBits == 18326 &&
            det.bsr.maxWords == 573,
            "Incorrect det.bsr results.");
    }
    {
        BSPrecision v0x(BSPrecision::Type::IS_DOUBLE), v0y(BSPrecision::Type::IS_DOUBLE);
        BSPrecision v1x(BSPrecision::Type::IS_DOUBLE), v1y(BSPrecision::Type::IS_DOUBLE);
        BSPrecision v2x(BSPrecision::Type::IS_DOUBLE), v2y(BSPrecision::Type::IS_DOUBLE);
        BSPrecision px(BSPrecision::Type::IS_DOUBLE), py(BSPrecision::Type::IS_DOUBLE);
        auto x0 = v0x - px;
        auto y0 = v0y - py;
        auto s00 = v0x + px;
        auto s01 = v0y + py;
        auto t00 = s00 * x0;
        auto t01 = s01 * y0;
        auto z0 = t00 + t01;

        auto x1 = v1x - px;
        auto y1 = v1y - py;
        auto s10 = v1x + px;
        auto s11 = v1y + py;
        auto t10 = s10 * x1;
        auto t11 = s11 * y1;
        auto z1 = t10 + t11;

        auto x2 = v2x - px;
        auto y2 = v2y - py;
        auto s20 = v2x + px;
        auto s21 = v2y + py;
        auto t20 = s20 * x2;
        auto t21 = s21 * y2;
        auto z2 = t20 + t21;

        auto y0z1 = y0 * z1;
        auto y0z2 = y0 * z2;
        auto y1z0 = y1 * z0;
        auto y1z2 = y1 * z2;
        auto y2z0 = y2 * z0;
        auto y2z1 = y2 * z1;
        auto c0 = y1z2 - y2z1;
        auto c1 = y2z0 - y0z2;
        auto c2 = y0z1 - y1z0;
        auto x0c0 = x0 * c0;
        auto x1c1 = x1 * c1;
        auto x2c2 = x2 * c2;
        auto term = x0c0 + x1c1;
        det = term + x2c2;
        UTAssert(
            det.bsn.minExponent == -4296 &&
            det.bsn.maxExponent == 4103 &&
            det.bsn.maxBits == 8400 &&
            det.bsn.maxWords == 263,
            "Incorrect det.bsn results.");
        UTAssert(
            det.bsr.minExponent == -70884 &&
            det.bsr.maxExponent == 67627 &&
            det.bsr.maxBits == 138512 &&
            det.bsr.maxWords == 4329,
            "Incorrect det.bsr results.");
    }
}

void UnitTestBSPrecision::ToPlane()
{
    BSPrecision fx(BSPrecision::Type::IS_FLOAT);
    auto fdiff = fx - fx;
    auto fterm = fdiff * fdiff - fdiff * fdiff;
    auto fminor = fdiff * fterm;
    auto fdet = fminor + fminor + fminor;
    UTAssert(
        fdet.bsn.minExponent == -447 &&
        fdet.bsn.maxExponent == 389 &&
        fdet.bsn.maxBits == 837 &&
        fdet.bsn.maxWords == 27,
        "Incorrect fdet.bsn results.");
    UTAssert(
        fdet.bsr.minExponent == -4470 &&
        fdet.bsr.maxExponent == 3859 &&
        fdet.bsr.maxBits == 8330 &&
        fdet.bsr.maxWords == 261,
        "Incorrect fdet.bsr results.");

    BSPrecision dx(BSPrecision::Type::IS_DOUBLE);
    auto ddiff = dx - dx;
    auto dterm = ddiff * ddiff - ddiff * ddiff;
    auto dminor = ddiff * dterm;
    auto ddet = dminor + dminor + dminor;
    // -3222, 3077, 6300, 197
    UTAssert(
        ddet.bsn.minExponent == -3222 &&
        ddet.bsn.maxExponent == 3077 &&
        ddet.bsn.maxBits == 6300 &&
        ddet.bsn.maxWords == 197,
        "Incorrect ddet.bsn results.");
    UTAssert(
        ddet.bsr.minExponent == -32220 &&
        ddet.bsr.maxExponent == 30739 &&
        ddet.bsr.maxBits == 62960 &&
        ddet.bsr.maxWords == 1968,
        "Incorrect ddet.bsr results.");
}

void UnitTestBSPrecision::ToCircumsphere()
{
    BSPrecision det;
    {
        BSPrecision v00(BSPrecision::Type::IS_FLOAT), v01(BSPrecision::Type::IS_FLOAT), v02(BSPrecision::Type::IS_FLOAT);
        BSPrecision v10(BSPrecision::Type::IS_FLOAT), v11(BSPrecision::Type::IS_FLOAT), v12(BSPrecision::Type::IS_FLOAT);
        BSPrecision v20(BSPrecision::Type::IS_FLOAT), v21(BSPrecision::Type::IS_FLOAT), v22(BSPrecision::Type::IS_FLOAT);
        BSPrecision v30(BSPrecision::Type::IS_FLOAT), v31(BSPrecision::Type::IS_FLOAT), v32(BSPrecision::Type::IS_FLOAT);
        BSPrecision p0(BSPrecision::Type::IS_FLOAT), p1(BSPrecision::Type::IS_FLOAT), p2(BSPrecision::Type::IS_FLOAT);
        auto x0 = v00 - p0;
        auto y0 = v01 - p1;
        auto z0 = v02 - p2;
        auto s00 = v00 + p0;
        auto s01 = v01 + p1;
        auto s02 = v02 + p2;
        auto t00 = s00 * x0;
        auto t01 = s01 * y0;
        auto t02 = s02 * z0;
        auto t00pt01 = t00 + t01;
        auto w0 = t00pt01 + t02;

        auto x1 = v10 - p0;
        auto y1 = v11 - p1;
        auto z1 = v12 - p2;
        auto s10 = v10 + p0;
        auto s11 = v11 + p1;
        auto s12 = v12 + p2;
        auto t10 = s10 * x1;
        auto t11 = s11 * y1;
        auto t12 = s12 * z1;
        auto t10pt11 = t10 + t11;
        auto w1 = t10pt11 + t12;

        auto x2 = v20 - p0;
        auto y2 = v21 - p1;
        auto z2 = v22 - p2;
        auto s20 = v20 + p0;
        auto s21 = v21 + p1;
        auto s22 = v22 + p2;
        auto t20 = s20 * x2;
        auto t21 = s21 * y2;
        auto t22 = s22 * z2;
        auto t20pt21 = t20 + t21;
        auto w2 = t20pt21 + t22;

        auto x3 = v30 - p0;
        auto y3 = v31 - p1;
        auto z3 = v32 - p2;
        auto s30 = v30 + p0;
        auto s31 = v31 + p1;
        auto s32 = v32 + p2;
        auto t30 = s30 * x3;
        auto t31 = s31 * y3;
        auto t32 = s32 * z3;
        auto t30pt31 = t30 + t31;
        auto w3 = t30pt31 + t32;

        auto x0y1 = x0 * y1;
        auto x0y2 = x0 * y2;
        auto x0y3 = x0 * y3;
        auto x1y0 = x1 * y0;
        auto x1y2 = x1 * y2;
        auto x1y3 = x1 * y3;
        auto x2y0 = x2 * y0;
        auto x2y1 = x2 * y1;
        auto x2y3 = x2 * y3;
        auto x3y0 = x3 * y0;
        auto x3y1 = x3 * y1;
        auto x3y2 = x3 * y2;
        auto a0 = x0y1 - x1y0;
        auto a1 = x0y2 - x2y0;
        auto a2 = x0y3 - x3y0;
        auto a3 = x1y2 - x2y1;
        auto a4 = x1y3 - x3y1;
        auto a5 = x2y3 - x3y2;

        auto z0w1 = z0 * w1;
        auto z0w2 = z0 * w2;
        auto z0w3 = z0 * w3;
        auto z1w0 = z1 * w0;
        auto z1w2 = z1 * w2;
        auto z1w3 = z1 * w3;
        auto z2w0 = z2 * w0;
        auto z2w1 = z2 * w1;
        auto z2w3 = z2 * w3;
        auto z3w0 = z3 * w0;
        auto z3w1 = z3 * w1;
        auto z3w2 = z3 * w2;
        auto b0 = z0w1 - z1w0;
        auto b1 = z0w2 - z2w0;
        auto b2 = z0w3 - z3w0;
        auto b3 = z1w2 - z2w1;
        auto b4 = z1w3 - z3w1;
        auto b5 = z2w3 - z3w2;
        auto a0b5 = a0 * b5;
        auto a1b4 = a1 * b4;
        auto a2b3 = a2 * b3;
        auto a3b2 = a3 * b2;
        auto a4b1 = a4 * b1;
        auto a5b0 = a5 * b0;
        auto term0 = a0b5 - a1b4;
        auto term1 = term0 + a2b3;
        auto term2 = term1 + a3b2;
        auto term3 = term2 - a4b1;
        det = term3 + a5b0;
        UTAssert(
            det.bsn.minExponent == -745 &&
            det.bsn.maxExponent == 653 &&
            det.bsn.maxBits == 1399 &&
            det.bsn.maxWords == 44,
            "Incorrect det.bsn results.");
        UTAssert(
            det.bsr.minExponent == -32184 &&
            det.bsr.maxExponent == 27796 &&
            det.bsr.maxBits == 59981 &&
            det.bsr.maxWords == 1875,
            "Incorrect det.bsr results.");
    }
    {
        BSPrecision v00(BSPrecision::Type::IS_DOUBLE), v01(BSPrecision::Type::IS_DOUBLE), v02(BSPrecision::Type::IS_DOUBLE);
        BSPrecision v10(BSPrecision::Type::IS_DOUBLE), v11(BSPrecision::Type::IS_DOUBLE), v12(BSPrecision::Type::IS_DOUBLE);
        BSPrecision v20(BSPrecision::Type::IS_DOUBLE), v21(BSPrecision::Type::IS_DOUBLE), v22(BSPrecision::Type::IS_DOUBLE);
        BSPrecision v30(BSPrecision::Type::IS_DOUBLE), v31(BSPrecision::Type::IS_DOUBLE), v32(BSPrecision::Type::IS_DOUBLE);
        BSPrecision p0(BSPrecision::Type::IS_DOUBLE), p1(BSPrecision::Type::IS_DOUBLE), p2(BSPrecision::Type::IS_DOUBLE);
        auto x0 = v00 - p0;
        auto y0 = v01 - p1;
        auto z0 = v02 - p2;
        auto s00 = v00 + p0;
        auto s01 = v01 + p1;
        auto s02 = v02 + p2;
        auto t00 = s00 * x0;
        auto t01 = s01 * y0;
        auto t02 = s02 * z0;
        auto t00pt01 = t00 + t01;
        auto w0 = t00pt01 + t02;

        auto x1 = v10 - p0;
        auto y1 = v11 - p1;
        auto z1 = v12 - p2;
        auto s10 = v10 + p0;
        auto s11 = v11 + p1;
        auto s12 = v12 + p2;
        auto t10 = s10 * x1;
        auto t11 = s11 * y1;
        auto t12 = s12 * z1;
        auto t10pt11 = t10 + t11;
        auto w1 = t10pt11 + t12;

        auto x2 = v20 - p0;
        auto y2 = v21 - p1;
        auto z2 = v22 - p2;
        auto s20 = v20 + p0;
        auto s21 = v21 + p1;
        auto s22 = v22 + p2;
        auto t20 = s20 * x2;
        auto t21 = s21 * y2;
        auto t22 = s22 * z2;
        auto t20pt21 = t20 + t21;
        auto w2 = t20pt21 + t22;

        auto x3 = v30 - p0;
        auto y3 = v31 - p1;
        auto z3 = v32 - p2;
        auto s30 = v30 + p0;
        auto s31 = v31 + p1;
        auto s32 = v32 + p2;
        auto t30 = s30 * x3;
        auto t31 = s31 * y3;
        auto t32 = s32 * z3;
        auto t30pt31 = t30 + t31;
        auto w3 = t30pt31 + t32;

        auto x0y1 = x0 * y1;
        auto x0y2 = x0 * y2;
        auto x0y3 = x0 * y3;
        auto x1y0 = x1 * y0;
        auto x1y2 = x1 * y2;
        auto x1y3 = x1 * y3;
        auto x2y0 = x2 * y0;
        auto x2y1 = x2 * y1;
        auto x2y3 = x2 * y3;
        auto x3y0 = x3 * y0;
        auto x3y1 = x3 * y1;
        auto x3y2 = x3 * y2;
        auto a0 = x0y1 - x1y0;
        auto a1 = x0y2 - x2y0;
        auto a2 = x0y3 - x3y0;
        auto a3 = x1y2 - x2y1;
        auto a4 = x1y3 - x3y1;
        auto a5 = x2y3 - x3y2;

        auto z0w1 = z0 * w1;
        auto z0w2 = z0 * w2;
        auto z0w3 = z0 * w3;
        auto z1w0 = z1 * w0;
        auto z1w2 = z1 * w2;
        auto z1w3 = z1 * w3;
        auto z2w0 = z2 * w0;
        auto z2w1 = z2 * w1;
        auto z2w3 = z2 * w3;
        auto z3w0 = z3 * w0;
        auto z3w1 = z3 * w1;
        auto z3w2 = z3 * w2;
        auto b0 = z0w1 - z1w0;
        auto b1 = z0w2 - z2w0;
        auto b2 = z0w3 - z3w0;
        auto b3 = z1w2 - z2w1;
        auto b4 = z1w3 - z3w1;
        auto b5 = z2w3 - z3w2;
        auto a0b5 = a0 * b5;
        auto a1b4 = a1 * b4;
        auto a2b3 = a2 * b3;
        auto a3b2 = a3 * b2;
        auto a4b1 = a4 * b1;
        auto a5b0 = a5 * b0;
        auto term0 = a0b5 - a1b4;
        auto term1 = term0 + a2b3;
        auto term2 = term1 + a3b2;
        auto term3 = term2 - a4b1;
        det = term3 + a5b0;
        UTAssert(
            det.bsn.minExponent == -5370 &&
            det.bsn.maxExponent == 5133 &&
            det.bsn.maxBits == 10504 &&
            det.bsn.maxWords == 329,
            "Incorrect det.bsn results.");
        UTAssert(
            det.bsr.minExponent == -231984 &&
            det.bsr.maxExponent == 221332 &&
            det.bsr.maxBits == 453317 &&
            det.bsr.maxWords == 14167,
            "Incorrect det.bsr results.");
    }
}

#else

#include <GTL/Mathematics/Arithmetic/BSPrecision.h>

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(BSPrecision)
