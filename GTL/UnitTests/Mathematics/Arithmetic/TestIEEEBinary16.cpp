// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/IEEEBinary16.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIEEEBinary16
    {
    public:
        UnitTestIEEEBinary16();

    private:
        void TestConstructorsAndAssignment();
        void TestConvert32to16();
        void TestConvert16to32();
        void TestComparisons();
        void TestArithmetic();
        void TestMathFunctions();
        void TestExtendedMathFunctions();
    };
}

UnitTestIEEEBinary16::UnitTestIEEEBinary16()
{
    UTInformation("Mathematics/Arithmetic/IEEEBinary16");

    TestConstructorsAndAssignment();
    TestConvert32to16();
    TestConvert16to32();
    TestComparisons();
    TestArithmetic();
    TestMathFunctions();
    TestExtendedMathFunctions();
}

void UnitTestIEEEBinary16::TestConstructorsAndAssignment()
{
    IEEEBinary16 x = 1.2345f;
    float fconvert = static_cast<float>(x);  // rounding error occurs
    IEEEBinary16 y = fconvert;
    UTAssert(x == y, "Conversion failed.");

    IEEEBinary16 z = 1.2345;
    double dconvert = static_cast<double>(x);  // rounding error occurs
    IEEEBinary16 w = dconvert;
    UTAssert(z == w, "Conversion failed.");

    x = IEEEBinary16(w.encoding);
    UTAssert(x == y, "Conversion failed.");

}

void UnitTestIEEEBinary16::TestConvert32to16()
{
    IEEEBinary16 x;
    IEEEBinary32 y;

    // Test block "if (biased32 == 0)"
    x = 0.0f;
    UTAssert(x.encoding == 0, "Invalid conversion.");
    x = -0.0f;
    UTAssert(x.encoding == 0x8000u, "Invalid conversion.");
 
    // Test block "if (biased32 < F32::MAX_BIASED_EXPONENT)",
    // subblock "if (nonneg32 <= F16_AVR_MIN_SUB_ZER)"
    x = std::ldexp(1.0f, -26);
    UTAssert(x.encoding == 0x0000u, "Invalid conversion.");
    x = -std::ldexp(1.0f, -26);
    UTAssert(x.encoding == 0x8000u, "Invalid conversion.");
    x = std::ldexp(1.0f, -25);
    UTAssert(x.encoding == 0x0000u, "Invalid conversion.");
    x = -std::ldexp(1.0f, -25);
    UTAssert(x.encoding == 0x8000u, "Invalid conversion.");

    // Test block "if (biased32 < F32::MAX_BIASED_EXPONENT)",
    // subblock "if (nonneg32 <= F16_MIN_SUB)"
    x = std::ldexp(1.5f, -25);
    UTAssert(x.encoding == 0x0001u, "Invalid conversion.");
    x = -std::ldexp(1.5f, -25);
    UTAssert(x.encoding == 0x8001u, "Invalid conversion.");
    x = std::ldexp(1.0f, -24);
    UTAssert(x.encoding == 0x0001u, "Invalid conversion.");
    x = -std::ldexp(1.0f, -24);
    UTAssert(x.encoding == 0x8001u, "Invalid conversion.");

    // Test block "if (biased32 < F32::MAX_BIASED_EXPONENT)",
    // subblock "if (nonneg32 <= F16_MIN_NOR)". The first two
    // tests do not get into the subsubblock where round-up
    // occurs.
    x = std::ldexp(1.0f, -20);
    UTAssert(x.encoding == 0x0010u, "Invalid conversion.");
    x = -std::ldexp(1.0f, -20);
    UTAssert(x.encoding == 0x8010u, "Invalid conversion.");

    // This shows that the two 16-bit floating-point numbers that
    // bound 'number' are 1.13248825e-6 and 1.19209290e-6, and that
    // 'number' was rounded to the nearest 16-bit number 1.19209290e-6.
    // Also, this code does get into the subsubblock where
    // round-up occurs.
    float number = std::ldexp(1.2345f, -20);    // 1.17731099e-6
    x = number;
    UTAssert(x.encoding == 0x0014u, "Invalid conversion.");
    y.number = (float)x;                        // 1.19209290e-6
    x.encoding = x.GetNextDown();
    UTAssert(x.encoding == 0x0013u, "Invalid conversion.");
    y.number = (float)x;                        // 1.13248825e-6
    y.SetEncoding(0, 15 + 127, 0x007FE000);  // y = 1.1^{10}*2^{15}
    x = y.number;  // x = 1.1^{10}*2^{15} = 16-max-normal
    UTAssert(x.encoding == 0x7BFFu, "Invalid conversion.");
    y.SetEncoding(0, 15 + 127, 0x007FE001);  // y = 1.1^{10}0^{12}1*2^{15}
    x = y.number;  // x = 1.1^{10}*2^{15} = 16-max-normal (y rounded down)
    UTAssert(x.encoding == 0x7BFFu, "Invalid conversion.");

    // Test block "if (biased32 < F32::MAX_BIASED_EXPONENT)",
    // subblock "if (nonneg32 < F16_AVR_MAX_NOR_INF)"
    y.SetEncoding(0, 15 + 127, 0x007FE000);
    y.number += std::ldexp(1.0f, 16);
    y.number *= 0.5f;
    y.encoding -= 0x00000001u; // y = (16-max-normal+16-infinity)/2 - epsilon
    x = y.number;  // x = +infinity (2^{16})
    UTAssert(x.encoding == 0x7BFFu, "Invalid conversion.");

    // Test block "if (biased32 < F32::MAX_BIASED_EXPONENT)",
    // final subblock where nonneg32 >= (MAX_NORMAL+INFINITY)/2".
    // "trailing32 == 0".
    y.SetEncoding(0, 15 + 127, 0x007FE000);
    y.number += std::ldexp(1.0f, 16);
    y.number *= 0.5f;  // y = (16-max-normal+16-infinity)/2
    x = y.number;  // x = +infinity (2^{16})
    UTAssert(x.encoding == 0x7C00u, "Invalid conversion.");
    y.SetEncoding(0, 15 + 127, 0x007FE001);
    y.number += std::ldexp(1.0f, 16);
    y.number *= 0.5f;  // y = (16-max-normal+16-infinity)/2 + epsilon
    x = y.number;  // x = +infinity (2^{16})
    UTAssert(x.encoding == 0x7C00u, "Invalid conversion.");

    // Test block when biased32 == F32::MAX_BIASED_EXPONENT,
    // subblock "if (trailing32 == 0)".
    y.encoding = 0x7F800000;
    x = y.number;
    UTAssert(x.encoding == 0x7C00u, "Invalid conversion.");
    y.encoding = 0xFF800000;
    x = y.number;
    UTAssert(x.encoding == 0xFC00u, "Invalid conversion.");

    // Test block when biased32 == F32::MAX_BIASED_EXPONENT,
    // final subblock when trailing32 != 0.
    y.encoding = 0x7FC00000;  // quiet NaN, no payload
    x = y.number;
    UTAssert(x.encoding == 0x7E00u, "Invalid conversion.");
    y.encoding = 0xFFC12300;  // quiet NaN, payload
    x = y.number;
    UTAssert(x.encoding == 0xFE09u, "Invalid conversion.");
    y.encoding = 0x7F812345;  // signaling NaN (must have payload)
    x = y.number;
    UTAssert(x.encoding == 0x7C09u, "Invalid conversion.");
}

void UnitTestIEEEBinary16::TestConvert16to32()
{
    IEEEBinary16 x;
    IEEEBinary32 y;

    // Test block "if (biased16 == 0)", subblock "if (trailing16 == 0)".
    x.SetEncoding(0, 0, 0);
    y.number = (float)x;  // +0.0f
    UTAssert(y.encoding == 0x00000000u, "Invalid conversion.");
    x.SetEncoding(1, 0, 0);
    y.number = (float)x;  // -0.0f
    UTAssert(y.encoding == 0x80000000u, "Invalid conversion.");

    // Test block "if (biased16 == 0)", subblock when trailing16 != 0.
    x.encoding = IEEEBinary16::MIN_SUBNORMAL;
    y.number = (float)x;  // 2^{-24}
    UTAssert(y.number == std::ldexp(1.0f, -24), "Invalid conversion.");
    x.encoding = IEEEBinary16::SIGN_MASK | IEEEBinary16::MAX_SUBNORMAL;
    y.number = (float)x;  // -(2^{-14} - 2^{-24})
    float expected = -(std::ldexp(1.0f, -14) - std::ldexp(1.0f, -24));
    UTAssert(y.number == expected, "Invalid conversion.");

    // Test block "if (biased16 < F16::MAX_BIASED_EXPONENT)".
    x.encoding = IEEEBinary16::MIN_NORMAL;
    y.number = (float)x;  // 2^{-24}
    UTAssert(y.number == std::ldexp(1.0f, -14), "Invalid conversion.");
    x.encoding = IEEEBinary16::SIGN_MASK | IEEEBinary16::MAX_NORMAL;
    y.number = (float)x;  // 1.1111111111*2^{15}
    expected = 0.0f;
    for (std::int32_t i = 0; i <= 10; ++i)
    {
        expected += std::ldexp(1.0f, -i);
    }
    expected *= std::ldexp(1.0f, 15);
    expected = -expected;
    UTAssert(y.number == expected, "Invalid conversion.");

    // Test block when biased16 == F16::MAX_BIASED_EXPONENT, subblock
    // "if (trailing16 == 0)".
    x.encoding = IEEEBinary16::POS_INFINITY;
    y.number = (float)x;
    UTAssert(y.number == std::numeric_limits<float>::infinity(), "Invalid conversion.");

    // Test block when biased16 == F16::MAX_BIASED_EXPONENT, subblock
    // when trailing16 != 0.
    IEEEBinary32 nan;
    x.encoding = 0x7E00u;  // quiet NaN, no payload
    y.number = (float)x;
    nan.number = std::numeric_limits<float>::quiet_NaN();
    UTAssert(y.encoding == nan.encoding, "Invalid conversion.");
    x.encoding = 0x7E01u;  // quiet NaN, payload
    y.number = (float)x;
    nan.number = std::numeric_limits<float>::quiet_NaN();
    nan.encoding |= 0x00002000u;
    UTAssert(y.encoding == nan.encoding, "Invalid conversion.");

    x.encoding = 0x7C01u;  // signaling NaN (must have payload)
    y.number = (float)x;

    // NOTE: When using floating-point arithmetic in hardware, a
    // signaling NaN when assigned to a floating-point number
    // is converted to a quiet NaN. The floating-point environment
    // usually provides a mechanism for the programmer to hook up
    // a function that is called when the signaling NaN is generated
    // (which is why it is called "signaling"). In the following line
    // of code, the right-hand side generates a signaling NaN, but
    // then the assignment to nan.number leads to quiet Nan with the
    // same payload. The right-hand side is 0x7F800001, but after the
    // conversion to quiet NaN, the left-hand side is 0x7FC00001.
    nan.number = std::numeric_limits<float>::signaling_NaN();

    // Let's just compare the bit patterns (without invoking the
    // floating-point hardware) to ensure the conversion is correct.
    // NOTE: The correct result involves the payload that was assigned
    // to x.encoding. The C++ implementations of quiet_NaN() and
    // signaling_NaN() appears to ignore the fact that there are
    // many such NaNs and that the payload is *user-defined*.
    nan.encoding = 0x7F802000u;
    UTAssert(y.encoding == nan.encoding, "Invalid conversion.");
}

void UnitTestIEEEBinary16::TestComparisons()
{
    IEEEBinary16 x = 1.2345f;
    IEEEBinary16 y = 0.6789f;
    bool result;

    result = (x == y);
    UTAssert(result == false, "Incorrect comparision.");
    result = (x != y);
    UTAssert(result == true, "Incorrect comparision.");
    result = (x < y);
    UTAssert(result == false, "Incorrect comparision.");
    result = (x <= y);
    UTAssert(result == false, "Incorrect comparision.");
    result = (x > y);
    UTAssert(result == true, "Incorrect comparision.");
    result = (x >= y);
    UTAssert(result == true, "Incorrect comparision.");
}

void UnitTestIEEEBinary16::TestArithmetic()
{
    IEEEBinary16 x, y, z, w;
    float fx, fy;

    x = 1.2345f;
    y = -x;
    UTAssert(y == IEEEBinary16(-1.2345f), "Negation failed.");

    fx = 1.2345f;
    fy = 6.7891f;
    x = fx;
    y = fy;

    z = x + y;
    w = fx + fy;
    UTAssert(z == w, "Addition failed.");

    z = x - y;
    w = fx - fy;
    UTAssert(z == w, "Subtraction failed.");

    z = x * y;
    w = fx * fy;
    UTAssert(z == w, "Multiplication failed.");

    z = x / y;
    w = IEEEBinary16(fx) / IEEEBinary16(fy);
    UTAssert(z == w, "Division failed.");

    z = x + fy;
    w = x + y;
    UTAssert(z == w, "Addition failed.");

    z = x - fy;
    w = x - y;
    UTAssert(z == w, "Subtraction failed.");

    z = x * fy;
    w = x * y;
    UTAssert(z == w, "Multiplication failed.");

    z = x / fy;
    w = IEEEBinary16(fx) / IEEEBinary16(fy);
    UTAssert(z == w, "Division failed.");

    z = fx + y;
    w = x + y;
    UTAssert(z == w, "Addition failed.");

    z = fx - y;
    w = x - y;
    UTAssert(z == w, "Subtraction failed.");

    z = fx * y;
    w = x * y;
    UTAssert(z == w, "Multiplication failed.");

    // z is computed as fx/(float)y. w is computed as (float)x/(float)y.
    // These are generally two different numbers (but close to each other).
    z = fx / y;
    w = x / y;
    UTAssert(z != w, "Division failed.");

    x = fx;
    z = x + y;
    x += y;
    UTAssert(z == x, "Addition failed.");

    x = fx;
    z = x - y;
    x -= y;
    UTAssert(z == x, "Subtraction failed.");

    x = fx;
    z = x * y;
    x *= y;
    UTAssert(z == x, "Multiplication failed.");

    x = fx;
    z = x / y;
    x /= y;
    UTAssert(z == x, "Division failed.");

    x = fx;
    z = x + y;
    x += fy;
    UTAssert(z == x, "Addition failed.");

    x = fx;
    z = x - y;
    x -= fy;
    UTAssert(z == x, "Subtraction failed.");

    x = fx;
    z = x * y;
    x *= fy;
    UTAssert(z == x, "Multiplication failed.");

    // z is computed as (float)x/(float)y. x is computed as (float)x/y. The
    // latter invokes operator/(float,IEEEBinary16), so x is computed as
    // (float)x/(float)y, which implies z == x.
    x = fx;
    z = x / y;
    x /= fy;
    UTAssert(z == x, "Division failed.");
}

void UnitTestIEEEBinary16::TestMathFunctions()
{
    IEEEBinary16 x, y, z, w;

    x = 0.9876f;
    z = std::acos((float)x);
    w = std::acos(x);
    UTAssert(z == w, "Incorrect acos.");

    x = 1.2345f;
    z = std::acosh((float)x);
    w = std::acosh(x);
    UTAssert(z == w, "Incorrect acosh.");

    x = 0.9876f;
    z = std::asin((float)x);
    w = std::asin(x);
    UTAssert(z == w, "Incorrect asin.");

    x = 0.9876f;
    z = std::asinh((float)x);
    w = std::asinh(x);
    UTAssert(z == w, "Incorrect asinh.");

    x = 0.9876f;
    z = std::atan((float)x);
    w = std::atan(x);
    UTAssert(z == w, "Incorrect atan.");

    x = 0.9876f;
    z = std::atanh((float)x);
    w = std::atanh(x);
    UTAssert(z == w, "Incorrect atanh.");

    x = 0.9876f;
    y = 2.3579f;
    z = std::atan2((float)y, (float)x);
    w = std::atan2(y, x);
    UTAssert(z == w, "Incorrect atan2.");

    x = 0.9876f;
    z = std::ceil((float)x);
    w = std::ceil(x);
    UTAssert(z == w, "Incorrect ceil.");

    x = 0.9876f;
    z = std::cos((float)x);
    w = std::cos(x);
    UTAssert(z == w, "Incorrect cos.");

    x = 0.9876f;
    z = std::cosh((float)x);
    w = std::cosh(x);
    UTAssert(z == w, "Incorrect cosh.");

    x = 0.9876f;
    z = std::exp((float)x);
    w = std::exp(x);
    UTAssert(z == w, "Incorrect exp.");

    x = 0.9876f;
    z = std::exp2((float)x);
    w = std::exp2(x);
    UTAssert(z == w, "Incorrect exp2.");

    x = -0.9876f;
    z = std::fabs((float)x);
    w = std::fabs(x);
    UTAssert(z == w, "Incorrect fabs.");

    x = 0.9876f;
    z = std::floor((float)x);
    w = std::floor(x);
    UTAssert(z == w, "Incorrect floor.");

    x = 2.3579f;
    y = 0.9876f;
    z = std::fmod((float)x, (float)y);
    w = std::fmod(x, y);
    UTAssert(z == w, "Incorrect fmod.");

    std::int32_t exponent0, exponent1;
    x = 2.3579f;
    z = std::frexp((float)x, &exponent0);
    w = std::frexp(x, &exponent1);
    UTAssert(z == w && exponent0 == exponent1, "Incorrect frexp.");

    x = 2.3579f;
    exponent0 = 2;
    z = std::ldexp((float)x, exponent0);
    w = std::ldexp(x, exponent0);
    UTAssert(z == w, "Incorrect ldexp.");

    x = 1.2345f;
    z = std::log((float)x);
    w = std::log(x);
    UTAssert(z == w, "Incorrect log.");

    x = 1.2345f;
    z = std::log2((float)x);
    w = std::log2(x);
    UTAssert(z == w, "Incorrect log2.");

    x = 1.2345f;
    z = std::log10((float)x);
    w = std::log10(x);
    UTAssert(z == w, "Incorrect log10.");

    x = 2.3579f;
    y = 0.9876f;
    z = std::pow((float)x, (float)y);
    w = std::pow(x, y);
    UTAssert(z == w, "Incorrect pow.");

    x = 1.2345f;
    z = std::sin((float)x);
    w = std::sin(x);
    UTAssert(z == w, "Incorrect sin.");

    x = 1.2345f;
    z = std::sinh((float)x);
    w = std::sinh(x);
    UTAssert(z == w, "Incorrect sinh.");

    x = 1.2345f;
    z = std::sqrt((float)x);
    w = std::sqrt(x);
    UTAssert(z == w, "Incorrect sqrt.");

    x = 1.2345f;
    z = std::tan((float)x);
    w = std::tan(x);
    UTAssert(z == w, "Incorrect tan.");

    x = 1.2345f;
    z = std::tanh((float)x);
    w = std::tanh(x);
    UTAssert(z == w, "Incorrect tanh.");
}

void UnitTestIEEEBinary16::TestExtendedMathFunctions()
{
    IEEEBinary16 x, y, z, w;

    x = 0.9876f;
    z = atandivpi((float)x);
    w = atandivpi(x);
    UTAssert(z == w, "Incorrect atandivpi.");

    x = 0.9876f;
    y = 2.3579f;
    z = atan2divpi((float)y, (float)x);
    w = atan2divpi(y, x);
    UTAssert(z == w, "Incorrect atan2divpi.");

    x = 0.9876f;
    z = clamp((float)x, 0.0f, 0.5f);
    w = clamp(x, IEEEBinary16(0.0f), IEEEBinary16(0.5f));
    UTAssert(z == w, "Incorrect clamp.");

    x = 0.9876f;
    z = cospi((float)x);
    w = cospi(x);
    UTAssert(z == w, "Incorrect cospi.");

    x = 0.9876f;
    z = exp10((float)x);
    w = exp10(x);
    UTAssert(z == w, "Incorrect exp10.");

    x = 0.9876f;
    z = invsqrt((float)x);
    w = invsqrt(x);
    UTAssert(z == w, "Incorrect invsqrt.");

    x = -0.9876f;
    std::int32_t sign0 = isign((float)x);
    std::int32_t sign1 = isign(x);
    UTAssert(sign0 == sign1, "Incorrect isign.");

    x = 0.9876f;
    z = saturate((float)x);
    w = saturate(x);
    UTAssert(z == w, "Incorrect saturate.");

    x = -0.9876f;
    z = sign((float)x);
    w = sign(x);
    UTAssert(sign0 == sign1, "Incorrect sign.");

    x = 0.9876f;
    z = sinpi((float)x);
    w = sinpi(x);
    UTAssert(z == w, "Incorrect sinpi.");

    x = 0.9876f;
    z = sqr((float)x);
    w = sqr(x);
    UTAssert(z == w, "Incorrect sqr.");
}

#else

#include <GTL/Mathematics/Arithmetic/IEEEBinary16.h>

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IEEEBinary16)
