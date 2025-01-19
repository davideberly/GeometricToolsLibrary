// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/UIntegerAP32.h>
#include <vector>
#include <random>
#include <fstream>
using namespace gtl;

namespace gtl
{
    class UnitTestUIntegerAP32
    {
    public:
        UnitTestUIntegerAP32();

    private:
        void TestAdd();
        void TestSub();
        void TestMul();
        void TestShift();
        void TestComparison();
        void TestRoundUp();

        void TestCopyAndMove();
        void TestWriteAndRead();

        using ALU = UIntegerALU32<UIntegerAP32>;
    };
}

UnitTestUIntegerAP32::UnitTestUIntegerAP32()
{
    UTInformation("Mathematics/Arithmetic/UIntegerAP32");

    TestShift();
    TestAdd();
    TestSub();
    TestMul();
    TestRoundUp();
    TestComparison();
    TestCopyAndMove();
    TestWriteAndRead();
}

void UnitTestUIntegerAP32::TestShift()
{
    UIntegerAP32 n0, n1, n2, actual;
    std::uint64_t u0, uactual;
    std::int32_t shift;

    // The constructor for UInteger types shifts the bits right to
    // enforce the invariant that UInteger is zero or odd. The bits
    // for this test need to be set directly to avoid that shift.
    u0 = 0xFEDCBA9876523218ull;
    uactual = (u0 >> 3);
    n0.SetNumBits(64);
    n0.GetBits()[0] = 0x76523218u;
    n0.GetBits()[1] = 0xFEDCBA98u;
    shift = ALU::ShiftRightToOdd(n0, n1);
    UTAssert(shift == 3, "Invalid ShiftRightToOdd.");
    actual = UIntegerAP32(uactual);
    UTAssert(actual == n1, "Incorrect ShiftRightToOdd.");
    ALU::ShiftLeft(n1, shift, n2);
    UTAssert(n2 == n0, "Incorrect ShiftLeft.");

    u0 = 0xFEDCBA9100000000ull;
    uactual = (u0 >> 32);
    n0.SetNumBits(64);
    n0.GetBits()[0] = 0x00000000u;
    n0.GetBits()[1] = 0xFEDCBA91u;
    shift = ALU::ShiftRightToOdd(n0, n1);
    UTAssert(shift == 32, "Invalid ShiftRightToOdd.");
    actual = UIntegerAP32(uactual);
    UTAssert(actual == n1, "Incorrect ShiftRightToOdd.");
    ALU::ShiftLeft(n1, shift, n2);
    UTAssert(n2 == n0, "Incorrect ShiftLeft.");

    // "if (lshift > 0)" block
    u0 = 0xFEDCBA987654321Full;
    n0 = UIntegerAP32(u0);
    ALU::ShiftLeft(n0, 35, n1);
    ALU::ShiftRightToOdd(n1, n2);
    UTAssert(n0 == n2, "Invalid shifts.");

    // The "else" block for "if (lshift > 0)".
    ALU::ShiftLeft(n0, 32, n1);
    ALU::ShiftRightToOdd(n1, n2);
    UTAssert(n0 == n2, "Invalid shifts.");
}

void UnitTestUIntegerAP32::TestAdd()
{
    UIntegerAP32 n0, n1, n2, n3, actual;
    std::uint64_t u0, u1, uactual;
    std::size_t shift;

    // no carry-out:
    // if(carry > 0){...}else{active}
    // if((mask&getback)==0){active}
    u0 = 1234567u;
    u1 = 876543218901235ull;
    uactual = u0 + u1;
    n0 = UIntegerAP32(u0);
    n1 = UIntegerAP32(u1);
    ALU::Add(n0, n1, n2);
    shift = ALU::ShiftRightToOdd(n2, n3);
    UTAssert(shift == 1, "Invalid ShiftRightToOdd.");
    actual = UIntegerAP32(uactual);
    UTAssert(actual == n3, "Incorrect Add.");

    // carry-out first: if(carry > 0){for{..})if(carry > 0){active}}else{...}
    // if((mask&getback)==0){...} // code does not enter this branch
    u0 = 0xF000000087654321ull;
    u1 = 0xF7FFFFFFABCDEFFFull;
    uactual = u0 + u1;
    uactual >>= 5;  // the shift amount below
    uactual |= 0x0800000000000000ull;  // there is a carry-out, shifted right by 5
    n0 = UIntegerAP32(u0);
    n1 = UIntegerAP32(u1);
    ALU::Add(n0, n1, n2);
    shift = ALU::ShiftRightToOdd(n2, n3);
    UTAssert(shift == 5, "Invalid ShiftRightToOdd.");
    actual = UIntegerAP32(uactual);
    UTAssert(actual == n3, "Incorrect Add.");

    // carry-out first: if(carry > 0){for{active})if(carry > 0){active}}else{...}
    // if((mask&getback)==0){...} // code does not enter this branch
    u0 = 0x0000000087654321ull;
    u1 = 0xFFFFFFFFABCDEFFFull;
    uactual = u0 + u1;
    uactual >>= 5;  // the shift amount below
    uactual |= 0x0800000000000000ull;  // there is a carry-out, shifted right by 5
    n0 = UIntegerAP32(u0);
    n1 = UIntegerAP32(u1);
    ALU::Add(n0, n1, n2);
    shift = ALU::ShiftRightToOdd(n2, n3);
    UTAssert(shift == 5, "Invalid ShiftRightToOdd.");
    actual = UIntegerAP32(uactual);
    UTAssert(actual == n3, "Incorrect Add.");
}

void UnitTestUIntegerAP32::TestSub()
{
    UIntegerAP32 n0, n1, n2, n3, actual;
    std::uint64_t u0, u1, uactual;
    std::size_t shift;

    u0 = 876543218901235ull;
    u1 = 1234567ull;
    uactual = u0 - u1;
    n0 = UIntegerAP32(u0);
    n1 = UIntegerAP32(u1);
    ALU::Sub(n0, n1, n2);
    shift = ALU::ShiftRightToOdd(n2, n3);
    UTAssert(shift == 2, "Invalid ShiftRightToOdd.");
    actual = UIntegerAP32(uactual);
    UTAssert(actual == n3, "Incorrect Sub.");
}

void UnitTestUIntegerAP32::TestMul()
{
    UIntegerAP32 n0, n1, n2, n3, n4, n5;

    std::uint64_t u0 = 0xD091BB5C22AE9EF7ull;
    std::uint64_t u1 = 0x73F0FD776AE18FBDull;

    // branch at 377 (carry > 0)
    // branch at 393 (carry > 0)
    // branch at 407 (carry = 0)
    // did not reach line 417
    n0 = UIntegerAP32(u0);
    n1 = UIntegerAP32(u1);
    ALU::Mul(n0, n1, n2);

    std::uint64_t v0 = u0 & 0x00000000FFFFFFFFull;
    std::uint64_t v1 = (u0 & 0xFFFFFFFF00000000ull) >> 32;
    std::uint64_t w0 = u1 & 0x00000000FFFFFFFFull;
    std::uint64_t w1 = (u1 & 0xFFFFFFFF00000000ull) >> 32;
    // Compute (v0 + v1 * 2^{32}) * (w0 + w1 * 2^{32})
    // = (v0 * w0) + (v0 * w1 + v1 * w0) * 2^{32} + (v1 * w1) * 2^{64}
    // = p00 + (p01 + p10) * 2^{32} + p11 * 2^{64}
    // = p00 + s0 * 2^{32} + p11 * 2^{64}
    // = p00 + (t0 + t1 * 2^{32}) * 2^{32} + p11 * 2^{64}
    // = p00 + t0 * 2^{32} + (t1 + p11) * 2^{64}
    // = p00 + t0 * 2^{32} + s1 * 2^{64}
    // = u2 + s1 * 2^{64}
    std::uint64_t p00 = v0 * w0;
    std::uint64_t p01 = v0 * w1;
    std::uint64_t p10 = v1 * w0;
    std::uint64_t p11 = v1 * w1;
    std::uint64_t s0 = p01 + p10;
    std::uint64_t t0 = s0 & 0x00000000FFFFFFFFull;
    std::uint64_t t1 = (s0 & 0xFFFFFFFF00000000ull) >> 32;
    std::uint64_t s1 = t1 + p11;
    std::uint64_t u2 = p00 + (t0 << 32);
    std::uint32_t block0 = u2 & 0x00000000FFFFFFFFull;
    std::uint32_t block1 = (u2 & 0xFFFFFFFF00000000ull) >> 32;
    std::uint32_t block2 = s1 & 0x00000000FFFFFFFFull;
    std::uint32_t block3 = (s1 & 0xFFFFFFFF00000000ull) >> 32;
    auto const& n2bits = n2.GetBits();
    UTAssert(block0 == n2bits[0] && block1 == n2bits[1]
        && block2 == n2bits[2] && block3 == n2bits[3],
        "Incorrect result from Mul.");

    // In the last ALU::MUL,
    // branch at 377 (carry = 0)
    // branch at 393 (carry > 0 and carry = 0)
    // branch at 407 (carry > 0 and carry = 0)
    // did not reach line 417
    ALU::ShiftLeft(n0, 98, n3);
    ALU::Mul(n3, n1, n4);
    std::size_t shift = ALU::ShiftRightToOdd(n4, n5); (void)shift;
    UTAssert(shift == 98, "Incorrect shift from ShiftRightToOdd.");
    auto const& n5bits = n5.GetBits();
    UTAssert(block0 == n5bits[0] && block1 == n5bits[1]
        && block2 == n5bits[2] && block3 == n5bits[3],
        "Incorrect result from Mul.");

    // branch at 377 (carry > 0)
    // branch at 393 (carry > 0)
    // branch at 407 (carry = 0)
    // reached line 417
    u0 = 0x0091BB5C22AE9EF7ull;
    u1 = 0x000000001AB863EFull;
    u1 >>= 1;
    n0 = UIntegerAP32(u0);
    n1 = UIntegerAP32(u1);
    ALU::Mul(n0, n1, n2);

    v0 = u0 & 0x00000000FFFFFFFFull;
    v1 = (u0 & 0xFFFFFFFF00000000ull) >> 32;
    w0 = u1 & 0x00000000FFFFFFFFull;
    // Compute (v0 + v1 * 2^{32}) * (w0 + 0 * 2^{32})
    // = (v0 * w0) + (v1 * w0) * 2^{32}
    // = p00 + p10 * 2^{32}
    // = p00 + (t0 + t1 * 2^{32}) * 2^{32}
    // = p00 + t0 * 2^{32} + t1 * 2^{64}
    // = u2 + t1 * 2^{64}
    p00 = v0 * w0;
    p10 = v1 * w0;
    t0 = p10 & 0x00000000FFFFFFFFull;
    t1 = (p10 & 0xFFFFFFFF00000000ull) >> 32;
    u2 = p00 + (t0 << 32);
    block0 = u2 & 0x00000000FFFFFFFFull;
    block1 = (u2 & 0xFFFFFFFF00000000ull) >> 32;
    block2 = t1 & 0x00000000FFFFFFFFull;
    auto const& nbits = n2.GetBits();
    UTAssert(block0 == nbits[0] && block1 == nbits[1]
        && block2 == nbits[2], "Incorrect result from Mul.");
}

void UnitTestUIntegerAP32::TestRoundUp()
{
    std::uint64_t u0 = 0x0091BB5C22AE9EF7ull;
    std::uint64_t u1 = (u0 >> 3) + 1ull;
    UIntegerAP32 n0(u0), n1(u1);
    std::size_t shift = UIntegerALU32<UIntegerAP32>::RoundUp(n0);
    UTAssert(shift == 3, "Incorrect RoundUp.");
    UTAssert(n0 == n1, "Incorrect RoundUp.");

    u0 = 0xFFFFFFFFFFFFFFFFull;
    n0 = UIntegerAP32(u0);
    n1 = UIntegerAP32(1u);
    shift = UIntegerALU32<UIntegerAP32>::RoundUp(n0);
    UTAssert(shift == 64, "Incorrect RoundUp.");
    UTAssert(n0 == n1, "Incorrect RoundUp.");
}

void UnitTestUIntegerAP32::TestComparison()
{
    std::uint64_t u0 = 0x1291BB5C22AE9EF7ull;
    std::uint64_t u1 = 0x129200000000FFFFull;
    UIntegerAP32 n0(u0), n1(u1);
    bool result;
    result = (n0 == n1);
    UTAssert(result == false, "Incorrect Equal.");
    result = (n0 != n1);
    UTAssert(result == true, "Incorrect NotEqual.");
    result = (n0 < n1);
    UTAssert(result == true, "Incorrect LessThan.");
    result = (n0 <= n1);
    UTAssert(result == true, "Incorrect LessThanOrEqual.");
    result = (n0 > n1);
    UTAssert(result == false, "Incorrect GreaterThan.");
    result = (n0 >= n1);
    UTAssert(result == false, "Incorrect GreaterThanOrEqual.");

    n1 = UIntegerAP32(static_cast<std::uint64_t>(0));
    result = (n0 < n1);
    UTAssert(result == false, "Incorrect LessThan.");
    result = (n1 < n0);
    UTAssert(result == true, "Incorrect LessThan.");

    n0 = UIntegerAP32(static_cast<std::uint64_t>(0));
    result = (n0 < n1);
    UTAssert(result == false, "Incorrect LessThan.");
    result = (n1 < n0);
    UTAssert(result == false, "Incorrect LessThan.");
}

void UnitTestUIntegerAP32::TestCopyAndMove()
{
    std::uint64_t u0 = 0x1291BB5C22AE9EF7ull;
    UIntegerAP32 n0(u0);
    UIntegerAP32 n1 = n0;
    bool equal = (n0 == n1);
    UTAssert(equal == true, "Copy failed.");

    UIntegerAP32 n2 = std::move(n1);
    equal = (n0 == n2);
    UTAssert(equal == true, "Move failed.");

    n2.SetAllBitsToZero();
    for (auto block : n2.GetBits())
    {
        UTAssert(block == 0, "SetAllBitsToZero failed.");
    }
}

void UnitTestUIntegerAP32::TestWriteAndRead()
{
    std::uint64_t u0 = 0x1291BB5C22AE9EF7ull;
    UIntegerAP32 n0(u0);
    std::ofstream output("Mathematics/Arithmetic/UIntegerAP32Number.binary", std::ios::binary);
    bool success = n0.Write(output);
    output.close();
    UTAssert(success == true, "Write failed.");

    UIntegerAP32 n1;
    std::ifstream input("Mathematics/Arithmetic/UIntegerAP32Number.binary", std::ios::binary);
    success = n1.Read(input);
    input.close();
    UTAssert(success == true, "Read failed.");

    success = (n0 == n1);
    UTAssert(success == true, "Write or Read failed.");
}

#else

#include <GTL/Mathematics/Arithmetic/UIntegerAP32.h>

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(UIntegerAP32)
