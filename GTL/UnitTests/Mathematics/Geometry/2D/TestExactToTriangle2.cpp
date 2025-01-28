#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/2D/ExactToTriangle2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestExactToTriangle2
    {
    public:
        UnitTestExactToTriangle2();

    private:
        using IRat = ExactToTriangle2<float>::Rational;
        using CRat = ExactToTriangle2<float>::CRational;

        // The comments are the signs of the barycentric coordinates of
        // P = b0 * V0 + b1 * V1 + b2 * V2 for triangle vertices V0, V1
        // and V2.
        void Test0();  // (+,+,+)
        void Test1();  // (+,+,-)
        void Test2();  // (-,+,+)
        void Test3();  // (+,-,+)
        void Test4();  // (+,+,0)
        void Test5();  // (+,0,+)
        void Test6();  // (0,+,+)
    };
}

UnitTestExactToTriangle2::UnitTestExactToTriangle2()
{
    UTInformation("Mathematics/Geometry/2D/ExactToTriangle2");

    Test0();
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    Test6();
}

void UnitTestExactToTriangle2::Test0()
{
    // (b0,b1,b2) = (+,+,+)
    ExactToTriangle2<float> query;
    Vector2<float> V0 = { -0.729045987f, -0.811583877f };
    Vector2<float> V1 = { 0.808353007f, 0.588698089f };
    Vector2<float> V2 = { -0.5f, 1.0f };
    Vector2<float> P = (V0 + V1 + V2) / 3.0f;
    Vector2<IRat> rP = { P[0], P[1] };
    Vector2<IRat> rV0 = { V0[0], V0[1] };
    Vector2<IRat> rV1 = { V1[0], V1[1] };
    Vector2<IRat> rV2 = { V2[0], V2[1] };

    std::int32_t isign = query(P, V0, V1, V2);
    std::int32_t isign01 = query.mISign01;
    std::int32_t isign12 = query.mISign12;
    std::int32_t isign20 = query.mISign20;
    auto const& idet01 = query.mIDet01;
    auto const& idet12 = query.mIDet12;
    auto const& idet20 = query.mIDet20;
    UTAssert(isign == -1 && isign01 == -1 && isign12 == -1 && isign20 == -1,
        "Incorrect sign.");
    UTAssert(
        idet01[0] == -0.821466982f && idet01[1] == -0.821465492f &&
        idet12[0] == -0.821466386f && idet12[1] == -0.821465790f &&
        idet20[0] == -0.821466327f && idet20[1] == -0.821465790f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2]()
    {
        return std::array<Vector2<IRat> const*, 4>{ &rP, &rV0, &rV1, &rV2 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, GetRPoints);
    UTAssert(rsign == -1 && isign01 == -1 && isign12 == -1 && isign20 == -1,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2);

    auto const& rdet01 = query.mNode[query.det01Node];
    double ddet01 = rdet01; (void)ddet01;
    UTAssert(
        rdet01.GetSign() == -1 &&
        rdet01.GetBiasedExponent() == -50 &&
        rdet01.GetUInteger().GetNumBits() == 50 &&
        rdet01.GetUInteger().GetBits()[0] == 0x676db339 &&
        rdet01.GetUInteger().GetBits()[1] == 0x0003492e,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet01[0]) <= rdet01 && rdet01 <= CRat(idet01[1]),
        "Interval does not contain the determinant.");

    auto const& rdet12 = query.mNode[query.det12Node];
    double ddet12 = rdet12; (void)ddet12;
    UTAssert(
        rdet12.GetSign() == -1 &&
        rdet12.GetBiasedExponent() == -50 &&
        rdet12.GetUInteger().GetNumBits() == 50 &&
        rdet12.GetUInteger().GetBits()[0] == 0x68fefb6d &&
        rdet12.GetUInteger().GetBits()[1] == 0x0003492e,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet12[0]) <= rdet12 && rdet12 <= CRat(idet12[1]),
        "Interval does not contain the determinant.");

    auto const& rdet20 = query.mNode[query.det20Node];
    double ddet20 = rdet20; (void)ddet20;
    UTAssert(
        rdet20.GetSign() == -1 &&
        rdet20.GetBiasedExponent() == -49 &&
        rdet20.GetUInteger().GetNumBits() == 49 &&
        rdet20.GetUInteger().GetBits()[0] == 0x33c48c0d &&
        rdet20.GetUInteger().GetBits()[1] == 0x0001a497,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet20[0]) <= rdet20 && rdet20 <= CRat(idet20[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactToTriangle2::Test1()
{
    // (b0,b1,b2) = (+,+,-)
    ExactToTriangle2<float> query;
    Vector2<float> V0 = { -0.729045987f, -0.811583877f };
    Vector2<float> V1 = { 0.808353007f, 0.588698089f };
    Vector2<float> V2 = { -0.5f, 1.0f };
    float b0 = 1.234f, b1 = 0.135f, b2 = 1.0f - b0 - b1;
    Vector2<float> P = b0 * V0 + b1 * V1 + b2 * V2;
    Vector2<IRat> rP = { P[0], P[1] };
    Vector2<IRat> rV0 = { V0[0], V0[1] };
    Vector2<IRat> rV1 = { V1[0], V1[1] };
    Vector2<IRat> rV2 = { V2[0], V2[1] };

    std::int32_t isign = query(P, V0, V1, V2);
    std::int32_t isign01 = query.mISign01;
    std::int32_t isign12 = query.mISign12;
    std::int32_t isign20 = query.mISign20;
    auto const& idet01 = query.mIDet01;
    auto const& idet12 = query.mIDet12;
    auto const& idet20 = query.mIDet20;
    UTAssert(
        isign == 1 && isign01 == 1 && isign12 == query.invalidSign && isign20 == query.invalidSign,
        "Incorrect sign.");
    UTAssert(
        idet01[0] == 0.909362614f && idet01[1] == 0.909363091f &&
        idet12[0] == 0.0f && idet12[1] == 0.0f &&
        idet20[0] == 0.0f && idet20[1] == 0.0f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2]()
    {
        return std::array<Vector2<IRat> const*, 4>{ &rP, &rV0, &rV1, &rV2 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, GetRPoints);
    UTAssert(
        rsign == 1 && isign01 == 1 && isign12 == query.invalidSign && isign20 == query.invalidSign,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2);

    auto const& rdet01 = query.mNode[query.det01Node];
    double ddet01 = rdet01; (void)ddet01;
    UTAssert(
        rdet01.GetSign() == 1 &&
        rdet01.GetBiasedExponent() == -47 &&
        rdet01.GetUInteger().GetNumBits() == 47 &&
        rdet01.GetUInteger().GetBits()[0] == 0x00175463 &&
        rdet01.GetUInteger().GetBits()[1] == 0x00007466,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet01[0]) <= rdet01 && rdet01 <= CRat(idet01[1]),
        "Interval does not contain the determinant.");

    auto const& rdet12 = query.mNode[query.det12Node];
    double ddet12 = rdet12; (void)ddet12;
    UTAssert(
        rdet12.GetSign() == 0 &&
        rdet12.GetBiasedExponent() == 0 &&
        rdet12.GetUInteger().GetNumBits() == 0,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet12[0]) <= rdet12 && rdet12 <= CRat(idet12[1]),
        "Interval does not contain the determinant.");

    auto const& rdet20 = query.mNode[query.det20Node];
    double ddet20 = rdet20; (void)ddet20;
    UTAssert(
        rdet20.GetSign() == 0 &&
        rdet20.GetBiasedExponent() == 0 &&
        rdet20.GetUInteger().GetNumBits() == 0,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet20[0]) <= rdet20 && rdet20 <= CRat(idet20[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactToTriangle2::Test2()
{
    // (b0,b1,b2) = (-,+,+)
    ExactToTriangle2<float> query;
    Vector2<float> V0 = { -0.729045987f, -0.811583877f };
    Vector2<float> V1 = { 0.808353007f, 0.588698089f };
    Vector2<float> V2 = { -0.5f, 1.0f };
    float b0 = -1.234f, b1 = 0.135f, b2 = 1.0f - b0 - b1;
    Vector2<float> P = b0 * V0 + b1 * V1 + b2 * V2;
    Vector2<IRat> rP = { P[0], P[1] };
    Vector2<IRat> rV0 = { V0[0], V0[1] };
    Vector2<IRat> rV1 = { V1[0], V1[1] };
    Vector2<IRat> rV2 = { V2[0], V2[1] };

    std::int32_t isign = query(P, V0, V1, V2);
    std::int32_t isign01 = query.mISign01;
    std::int32_t isign12 = query.mISign12;
    std::int32_t isign20 = query.mISign20;
    auto const& idet01 = query.mIDet01;
    auto const& idet12 = query.mIDet12;
    auto const& idet20 = query.mIDet20;
    UTAssert(isign == 1 && isign01 == -1 && isign12 == 1 && isign20 == query.invalidSign,
        "Incorrect sign.");
    UTAssert(
        idet01[0] == -5.17277479f && idet01[1] == -5.17277050f &&
        idet12[0] == 3.04106617f && idet12[1] == 3.04106832f &&
        idet20[0] == 0.0f && idet20[1] == 0.0f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2]()
    {
        return std::array<Vector2<IRat> const*, 4>{ &rP, &rV0, &rV1, &rV2 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, GetRPoints);
    UTAssert(rsign == 1 && isign01 == -1 && isign12 == 1 && isign20 == query.invalidSign,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2);

    auto const& rdet01 = query.mNode[query.det01Node];
    double ddet01 = rdet01; (void)ddet01;
    UTAssert(
        rdet01.GetSign() == -1 &&
        rdet01.GetBiasedExponent() == -47 &&
        rdet01.GetUInteger().GetNumBits() == 50 &&
        rdet01.GetUInteger().GetBits()[0] == 0x65e52e41 &&
        rdet01.GetUInteger().GetBits()[1] == 0x0002961d,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet01[0]) <= rdet01 && rdet01 <= CRat(idet01[1]),
        "Interval does not contain the determinant.");

    auto const& rdet12 = query.mNode[query.det12Node];
    double ddet12 = rdet12; (void)ddet12;
    UTAssert(
        rdet12.GetSign() == 1 &&
        rdet12.GetBiasedExponent() == -47 &&
        rdet12.GetUInteger().GetNumBits() == 49 &&
        rdet12.GetUInteger().GetBits()[0] == 0xb39de743 &&
        rdet12.GetUInteger().GetBits()[1] == 0x00018541,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet12[0]) <= rdet12 && rdet12 <= CRat(idet12[1]),
        "Interval does not contain the determinant.");

    auto const& rdet20 = query.mNode[query.det20Node];
    double ddet20 = rdet20; (void)ddet20;
    UTAssert(
        rdet20.GetSign() == 0 &&
        rdet20.GetBiasedExponent() == 0 &&
        rdet20.GetUInteger().GetNumBits() == 0,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet20[0]) <= rdet20 && rdet20 <= CRat(idet20[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactToTriangle2::Test3()
{
    // (b0,b1,b2) = (+,-,+)
    ExactToTriangle2<float> query;
    Vector2<float> V0 = { -0.729045987f, -0.811583877f };
    Vector2<float> V1 = { 0.808353007f, 0.588698089f };
    Vector2<float> V2 = { -0.5f, 1.0f };
    float b0 = 0.234f, b1 = -0.135f, b2 = 1.0f - b0 - b1;
    Vector2<float> P = b0 * V0 + b1 * V1 + b2 * V2;
    Vector2<IRat> rP = { P[0], P[1] };
    Vector2<IRat> rV0 = { V0[0], V0[1] };
    Vector2<IRat> rV1 = { V1[0], V1[1] };
    Vector2<IRat> rV2 = { V2[0], V2[1] };

    std::int32_t isign = query(P, V0, V1, V2);
    std::int32_t isign01 = query.mISign01;
    std::int32_t isign12 = query.mISign12;
    std::int32_t isign20 = query.mISign20;
    auto const& idet01 = query.mIDet01;
    auto const& idet12 = query.mIDet12;
    auto const& idet20 = query.mIDet20;
    UTAssert(isign == 1 && isign01 == -1 && isign12 == -1 && isign20 == 1,
        "Incorrect sign.");
    UTAssert(
        idet01[0] == -2.22042370f && idet01[1] == -2.22042203f &&
        idet12[0] == -0.576669395f && idet12[1] == -0.576668978f &&
        idet20[0] == 0.332693666f && idet20[1] == 0.332693934f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2]()
    {
        return std::array<Vector2<IRat> const*, 4>{ &rP, &rV0, &rV1, &rV2 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, GetRPoints);
    UTAssert(rsign == 1 && isign01 == -1 && isign12 == -1 && isign20 == 1,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2);

    auto const& rdet01 = query.mNode[query.det01Node];
    double ddet01 = rdet01; (void)ddet01;
    UTAssert(
        rdet01.GetSign() == -1 &&
        rdet01.GetBiasedExponent() == -48 &&
        rdet01.GetUInteger().GetNumBits() == 50 &&
        rdet01.GetUInteger().GetBits()[0] == 0xa2a79765 &&
        rdet01.GetUInteger().GetBits()[1] == 0x0002386d,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet01[0]) <= rdet01 && rdet01 <= CRat(idet01[1]),
        "Interval does not contain the determinant.");

    auto const& rdet12 = query.mNode[query.det12Node];
    double ddet12 = rdet12; (void)ddet12;
    UTAssert(
        rdet12.GetSign() == -1 &&
        rdet12.GetBiasedExponent() == -48 &&
        rdet12.GetUInteger().GetNumBits() == 48 &&
        rdet12.GetUInteger().GetBits()[0] == 0x9709c779 &&
        rdet12.GetUInteger().GetBits()[1] == 0x000093a0,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet12[0]) <= rdet12 && rdet12 <= CRat(idet12[1]),
        "Interval does not contain the determinant.");

    auto const& rdet20 = query.mNode[query.det20Node];
    double ddet20 = rdet20; (void)ddet20;
    UTAssert(
        rdet20.GetSign() == 1 &&
        rdet20.GetBiasedExponent() == -47 &&
        rdet20.GetUInteger().GetNumBits() == 46 &&
        rdet20.GetUInteger().GetBits()[0] == 0xb5d9f697 &&
        rdet20.GetUInteger().GetBits()[1] == 0x00002a95,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet20[0]) <= rdet20 && rdet20 <= CRat(idet20[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactToTriangle2::Test4()
{
    // (b0,b1,b2) = (+,+,0)
    ExactToTriangle2<float> query;
    Vector2<float> V0 = { 0.0f, 0.0f };
    Vector2<float> V1 = { 1.0f, 0.0f };
    Vector2<float> V2 = { 0.0f, 2.0f };
    float b0 = 0.5f, b1 = 0.5f, b2 = 0.0f;
    Vector2<float> P = b0 * V0 + b1 * V1 + b2 * V2;
    Vector2<IRat> rP = { P[0], P[1] };
    Vector2<IRat> rV0 = { V0[0], V0[1] };
    Vector2<IRat> rV1 = { V1[0], V1[1] };
    Vector2<IRat> rV2 = { V2[0], V2[1] };

    std::int32_t isign = query(P, V0, V1, V2);
    std::int32_t isign01 = query.mISign01;
    std::int32_t isign12 = query.mISign12;
    std::int32_t isign20 = query.mISign20;
    auto const& idet01 = query.mIDet01;
    auto const& idet12 = query.mIDet12;
    auto const& idet20 = query.mIDet20;
    UTAssert(isign == 0 && isign01 == query.invalidSign && isign12 == -1 && isign20 == -1,
        "Incorrect sign.");
    UTAssert(
        idet01[0] == -7.006e-45f && idet01[1] == +7.006e-45f &&
        idet12[0] == -1.00000048f && idet12[1] == -0.999999762f &&
        idet20[0] == -1.00000048f && idet20[1] == -0.999999762f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2]()
    {
        return std::array<Vector2<IRat> const*, 4>{ &rP, &rV0, &rV1, &rV2 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, GetRPoints);
    UTAssert(rsign == 0 && isign01 == query.invalidSign && isign12 == -1 && isign20 == -1,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2);

    auto const& rdet01 = query.mNode[query.det01Node];
    double ddet01 = rdet01; (void)ddet01;
    UTAssert(
        rdet01.GetSign() == 0 &&
        rdet01.GetBiasedExponent() == 0 &&
        rdet01.GetUInteger().GetNumBits() == 0,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet01[0]) <= rdet01 && rdet01 <= CRat(idet01[1]),
        "Interval does not contain the determinant.");

    auto const& rdet12 = query.mNode[query.det12Node];
    double ddet12 = rdet12; (void)ddet12;
    UTAssert(
        rdet12.GetSign() == -1 &&
        rdet12.GetBiasedExponent() == 0 &&
        rdet12.GetUInteger().GetNumBits() == 1 &&
        rdet12.GetUInteger().GetBits()[0] == 0x00000001,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet12[0]) <= rdet12 && rdet12 <= CRat(idet12[1]),
        "Interval does not contain the determinant.");

    auto const& rdet20 = query.mNode[query.det20Node];
    double ddet20 = rdet20; (void)ddet20;
    UTAssert(
        rdet20.GetSign() == -1 &&
        rdet20.GetBiasedExponent() == 0 &&
        rdet20.GetUInteger().GetNumBits() == 1 &&
        rdet20.GetUInteger().GetBits()[0] == 0x00000001,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet20[0]) <= rdet20 && rdet20 <= CRat(idet20[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactToTriangle2::Test5()
{
    // (b0,b1,b2) = (+,0,+)
    ExactToTriangle2<float> query;
    Vector2<float> V0 = { 0.0f, 0.0f };
    Vector2<float> V1 = { 1.0f, 0.0f };
    Vector2<float> V2 = { 0.0f, 2.0f };
    float b0 = 0.5f, b1 = 0.0f, b2 = 0.5f;
    Vector2<float> P = b0 * V0 + b1 * V1 + b2 * V2;
    Vector2<IRat> rP = { P[0], P[1] };
    Vector2<IRat> rV0 = { V0[0], V0[1] };
    Vector2<IRat> rV1 = { V1[0], V1[1] };
    Vector2<IRat> rV2 = { V2[0], V2[1] };

    std::int32_t isign = query(P, V0, V1, V2);
    std::int32_t isign01 = query.mISign01;
    std::int32_t isign12 = query.mISign12;
    std::int32_t isign20 = query.mISign20;
    auto const& idet01 = query.mIDet01;
    auto const& idet12 = query.mIDet12;
    auto const& idet20 = query.mIDet20;
    UTAssert(isign == 0 && isign01 == -1 && isign12 == -1 && isign20 == query.invalidSign,
        "Incorrect sign.");
    UTAssert(
        idet01[0] == -1.00000048f && idet01[1] == -0.999999762f &&
        idet12[0] == -1.00000107f && idet12[1] == -0.999999225f &&
        idet20[0] == -8.40779e-45f && idet20[1] == 8.40779e-45f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2]()
    {
        return std::array<Vector2<IRat> const*, 4>{ &rP, &rV0, &rV1, &rV2 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, GetRPoints);
    UTAssert(rsign == 0 && isign01 == -1 && isign12 == -1 && isign20 == query.invalidSign,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2);

    auto const& rdet01 = query.mNode[query.det01Node];
    double ddet01 = rdet01; (void)ddet01;
    UTAssert(
        rdet01.GetSign() == -1 &&
        rdet01.GetBiasedExponent() == 0 &&
        rdet01.GetUInteger().GetNumBits() == 1 &&
        rdet01.GetUInteger().GetBits()[0] == 0x00000001,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet01[0]) <= rdet01 && rdet01 <= CRat(idet01[1]),
        "Interval does not contain the determinant.");

    auto const& rdet12 = query.mNode[query.det12Node];
    double ddet12 = rdet12; (void)ddet12;
    UTAssert(
        rdet12.GetSign() == -1 &&
        rdet12.GetBiasedExponent() == 0 &&
        rdet12.GetUInteger().GetNumBits() == 1 &&
        rdet12.GetUInteger().GetBits()[0] == 0x00000001,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet12[0]) <= rdet12 && rdet12 <= CRat(idet12[1]),
        "Interval does not contain the determinant.");

    auto const& rdet20 = query.mNode[query.det20Node];
    double ddet20 = rdet20; (void)ddet20;
    UTAssert(
        rdet20.GetSign() == 0 &&
        rdet20.GetBiasedExponent() == 0 &&
        rdet20.GetUInteger().GetNumBits() == 0,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet20[0]) <= rdet20 && rdet20 <= CRat(idet20[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactToTriangle2::Test6()
{
    // (b0,b1,b2) = (0,+,+)
    ExactToTriangle2<float> query;
    Vector2<float> V0 = { 0.0f, 0.0f };
    Vector2<float> V1 = { 1.0f, 0.0f };
    Vector2<float> V2 = { 0.0f, 2.0f };
    float b0 = 0.0f, b1 = 0.5f, b2 = 0.5f;
    Vector2<float> P = b0 * V0 + b1 * V1 + b2 * V2;
    Vector2<IRat> rP = { P[0], P[1] };
    Vector2<IRat> rV0 = { V0[0], V0[1] };
    Vector2<IRat> rV1 = { V1[0], V1[1] };
    Vector2<IRat> rV2 = { V2[0], V2[1] };

    std::int32_t isign = query(P, V0, V1, V2);
    std::int32_t isign01 = query.mISign01;
    std::int32_t isign12 = query.mISign12;
    std::int32_t isign20 = query.mISign20;
    auto const& idet01 = query.mIDet01;
    auto const& idet12 = query.mIDet12;
    auto const& idet20 = query.mIDet20;
    UTAssert(isign == 0 && isign01 == -1 && isign12 == query.invalidSign && isign20 == -1,
        "Incorrect sign.");
    UTAssert(
        idet01[0] == -1.00000048f && idet01[1] == -0.999999762f &&
        idet12[0] == -5.36441860e-07f && idet12[1] == 5.36441860e-07f &&
        idet20[0] == -1.00000048f && idet20[1] == -0.999999762f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2]()
    {
        return std::array<Vector2<IRat> const*, 4>{ &rP, &rV0, &rV1, &rV2 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, GetRPoints);
    UTAssert(rsign == 0 && isign01 == -1 && isign12 == query.invalidSign && isign20 == -1,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2);

    auto const& rdet01 = query.mNode[query.det01Node];
    double ddet01 = rdet01; (void)ddet01;
    UTAssert(
        rdet01.GetSign() == -1 &&
        rdet01.GetBiasedExponent() == 0 &&
        rdet01.GetUInteger().GetNumBits() == 1 &&
        rdet01.GetUInteger().GetBits()[0] == 0x00000001,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet01[0]) <= rdet01 && rdet01 <= CRat(idet01[1]),
        "Interval does not contain the determinant.");

    auto const& rdet12 = query.mNode[query.det12Node];
    double ddet12 = rdet12; (void)ddet12;
    UTAssert(
        rdet12.GetSign() == 0 &&
        rdet12.GetBiasedExponent() == 0 &&
        rdet12.GetUInteger().GetNumBits() == 0,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet12[0]) <= rdet12 && rdet12 <= CRat(idet12[1]),
        "Interval does not contain the determinant.");

    auto const& rdet20 = query.mNode[query.det20Node];
    double ddet20 = rdet20; (void)ddet20;
    UTAssert(
        rdet20.GetSign() == -1 &&
        rdet20.GetBiasedExponent() == 0 &&
        rdet20.GetUInteger().GetNumBits() == 1 &&
        rdet20.GetUInteger().GetBits()[0] == 0x00000001,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet20[0]) <= rdet20 && rdet20 <= CRat(idet20[1]),
        "Interval does not contain the determinant.");
}

#else

#include <GTL/Mathematics/Geometry/2D/ExactToTriangle2.h>

namespace gtl
{
    template class ExactToTriangle2<float>;
    template class ExactToTriangle2<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ExactToTriangle2)
