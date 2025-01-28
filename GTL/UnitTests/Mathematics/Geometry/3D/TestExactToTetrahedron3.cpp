#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/3D/ExactToTetrahedron3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestExactToTetrahedron3
    {
    public:
        UnitTestExactToTetrahedron3();

    private:
        using IRat = ExactToTetrahedron3<float>::Rational;
        using CRat = ExactToTetrahedron3<float>::CRational;

        // The comments are the signs of the barycentric coordinates of
        // P = b0 * V0 + b1 * V1 + b2 * V2 + b3 * V3 for tetrahedron vertices
        // V0, V1, V2 and V3.
        void Test0();  // (+,+,+,+)
        void Test1();  // (+,+,+,-)
        void Test2();  // (-,+,+,+)
        void Test3();  // (+,-,+,+)
        void Test4();  // (+,+,-,+)
        void Test5();  // (+,+,+,0)
        void Test6();  // (+,+,0,+)
        void Test7();  // (+,0,+,+)
        void Test8();  // (0,+,+,+)
    };
}

UnitTestExactToTetrahedron3::UnitTestExactToTetrahedron3()
{
    UTInformation("Mathematics/Geometry/3D/ExactToTetrahedron3");

    Test0();
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    Test6();
    Test7();
    Test8();
}

void UnitTestExactToTetrahedron3::Test0()
{
    // (b0,b1,b2,b3) = (+,+,+,+)
    ExactToTetrahedron3<float> query;
    Vector3<float> V0 = { -0.729045987f, -0.811583877f, 0.0f };
    Vector3<float> V1 = { 0.808353007f, 0.588698089f, 0.0f };
    Vector3<float> V2 = { -0.5f, 1.0f, 0.0f };
    Vector3<float> V3 = { 0.0f, 0.0f, 1.0f };
    float b0 = 0.25f, b1 = 0.25f, b2 = 0.25f, b3 = 0.25f;
    Vector3<float> P = b0 * V0 + b1 * V1 + b2 * V2 + b3 * V3;
    Vector3<IRat> rP = { P[0], P[1], P[2] };
    Vector3<IRat> rV0 = { V0[0], V0[1], V0[2] };
    Vector3<IRat> rV1 = { V1[0], V1[1], V1[2] };
    Vector3<IRat> rV2 = { V2[0], V2[1], V2[2] };
    Vector3<IRat> rV3 = { V3[0], V3[1], V3[2] };

    std::int32_t isign = query(P, V0, V1, V2, V3);
    std::int32_t isign021 = query.mISign021;
    std::int32_t isign013 = query.mISign013;
    std::int32_t isign032 = query.mISign032;
    std::int32_t isign123 = query.mISign123;
    auto const& idet021 = query.mIDet021;
    auto const& idet013 = query.mIDet013;
    auto const& idet032 = query.mIDet032;
    auto const& idet123 = query.mIDet123;
    UTAssert(isign == -1 &&
        isign021 == -1 && isign013 == -1 &&
        isign032 == -1 && isign123 == -1,
        "Incorrect sign.");
    UTAssert(
        idet021[0] == -0.616100013f && idet021[1] == -0.616099179f &&
        idet013[0] == -0.616101325f && idet013[1] == -0.616098166f &&
        idet032[0] == -0.616100609f && idet032[1] == -0.616098583f &&
        idet123[0] == -0.616100252f && idet123[1] == -0.616098940f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2, &rV3]()
    {
        return std::array<Vector3<IRat> const*, 5>{ &rP, &rV0, &rV1, & rV2, &rV3 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, V3, GetRPoints);
    UTAssert(rsign == -1 &&
        isign021 == -1 && isign013 == -1 &&
        isign032 == -1 && isign123 == -1,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2, rV3);

    auto const& rdet021 = query.mNode[query.det021Node];
    double ddet021 = rdet021; (void)ddet021;
    UTAssert(
        rdet021.GetSign() == -1 &&
        rdet021.GetBiasedExponent() == -46 &&
        rdet021.GetUInteger().GetNumBits() == 46 &&
        rdet021.GetUInteger().GetBits()[0] == 0x2cdfd71b &&
        rdet021.GetUInteger().GetBits()[1] == 0x0000276e,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet021[0]) <= rdet021 && rdet021 <= CRat(idet021[1]),
        "Interval does not contain the determinant.");

    auto const& rdet013 = query.mNode[query.det013Node];
    double ddet013 = rdet013; (void)ddet013;
    UTAssert(
        rdet013.GetSign() == -1 &&
        rdet013.GetBiasedExponent() == -46 &&
        rdet013.GetUInteger().GetNumBits() == 46 &&
        rdet013.GetUInteger().GetBits()[0] == 0x2cdfd71b &&
        rdet013.GetUInteger().GetBits()[1] == 0x0000276e,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet013[0]) <= rdet013 && rdet013 <= CRat(idet013[1]),
        "Interval does not contain the determinant.");

    auto const& rdet032 = query.mNode[query.det032Node];
    double ddet032 = rdet032; (void)ddet032;
    UTAssert(
        rdet032.GetSign() == -1 &&
        rdet032.GetBiasedExponent() == -46 &&
        rdet032.GetUInteger().GetNumBits() == 46 &&
        rdet032.GetUInteger().GetBits()[0] == 0x2cdfd71b &&
        rdet032.GetUInteger().GetBits()[1] == 0x0000276e,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet032[0]) <= rdet032 && rdet032 <= CRat(idet032[1]),
        "Interval does not contain the determinant.");

    auto const& rdet123 = query.mNode[query.det123Node];
    double ddet123 = rdet123; (void)ddet123;
    UTAssert(
        rdet123.GetSign() == -1 &&
        rdet123.GetBiasedExponent() == -46 &&
        rdet123.GetUInteger().GetNumBits() == 46 &&
        rdet123.GetUInteger().GetBits()[0] == 0x2cdfd71b &&
        rdet123.GetUInteger().GetBits()[1] == 0x0000276e,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet123[0]) <= rdet123 && rdet123 <= CRat(idet123[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactToTetrahedron3::Test1()
{
    // (b0,b1,b2,b3) = (+,+,+,-)
    ExactToTetrahedron3<float> query;
    Vector3<float> V0 = { -0.729045987f, -0.811583877f, 0.0f };
    Vector3<float> V1 = { 0.808353007f, 0.588698089f, 0.0f };
    Vector3<float> V2 = { -0.5f, 1.0f, 0.0f };
    Vector3<float> V3 = { 0.0f, 0.0f, 1.0f };
    float b0 = 0.12345f, b1 = 0.67890f, b2 = 0.35791f, b3 = -0.24680f;
    float bsum = b0 + b1 + b2 + b3;
    b0 /= bsum;
    b1 /= bsum;
    b2 /= bsum;
    b3 /= bsum;
    Vector3<float> P = b0 * V0 + b1 * V1 + b2 * V2 + b3 * V3;
    Vector3<IRat> rP = { P[0], P[1], P[2] };
    Vector3<IRat> rV0 = { V0[0], V0[1], V0[2] };
    Vector3<IRat> rV1 = { V1[0], V1[1], V1[2] };
    Vector3<IRat> rV2 = { V2[0], V2[1], V2[2] };
    Vector3<IRat> rV3 = { V3[0], V3[1], V3[2] };

    std::int32_t isign = query(P, V0, V1, V2, V3);
    std::int32_t isign021 = query.mISign021;
    std::int32_t isign013 = query.mISign013;
    std::int32_t isign032 = query.mISign032;
    std::int32_t isign123 = query.mISign123;
    auto const& idet021 = query.mIDet021;
    auto const& idet013 = query.mIDet013;
    auto const& idet032 = query.mIDet032;
    auto const& idet123 = query.mIDet123;
    UTAssert(isign == 1 &&
        isign021 == 1 && isign013 == query.invalidSign &&
        isign032 == query.invalidSign && isign123 == query.invalidSign,
        "Incorrect sign.");
    UTAssert(
        idet021[0] == 0.665834486f && idet021[1] == 0.665835440f &&
        idet013[0] == 0.0f && idet013[1] == 0.0f &&
        idet032[0] == 0.0f && idet032[1] == 0.0f &&
        idet123[0] == 0.0f && idet123[1] == 0.0f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2, &rV3]()
    {
        return std::array<Vector3<IRat> const*, 5>{ &rP, &rV0, &rV1, &rV2, &rV3 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, V3, GetRPoints);
    UTAssert(rsign == 1 &&
        isign021 == 1 && isign013 == query.invalidSign &&
        isign032 == query.invalidSign && isign123 == query.invalidSign,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2, rV3);

    auto const& rdet021 = query.mNode[query.det021Node];
    double ddet021 = rdet021; (void)ddet021;
    UTAssert(
        rdet021.GetSign() == 1 &&
        rdet021.GetBiasedExponent() == -67 &&
        rdet021.GetUInteger().GetNumBits() == 67 &&
        rdet021.GetUInteger().GetBits()[0] == 0xb2dc1855 &&
        rdet021.GetUInteger().GetBits()[1] == 0x53a140d8 &&
        rdet021.GetUInteger().GetBits()[2] == 0x00000005,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet021[0]) <= rdet021 && rdet021 <= CRat(idet021[1]),
        "Interval does not contain the determinant.");

    auto const& rdet013 = query.mNode[query.det013Node];
    double ddet013 = rdet013; (void)ddet013;
    UTAssert(
        rdet013.GetSign() == 0 &&
        rdet013.GetBiasedExponent() == 0 &&
        rdet013.GetUInteger().GetNumBits() == 0,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet013[0]) <= rdet013 && rdet013 <= CRat(idet013[1]),
        "Interval does not contain the determinant.");

    auto const& rdet032 = query.mNode[query.det032Node];
    double ddet032 = rdet032; (void)ddet032;
    UTAssert(
        rdet032.GetSign() == 0 &&
        rdet032.GetBiasedExponent() == 0 &&
        rdet032.GetUInteger().GetNumBits() == 0,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet032[0]) <= rdet032 && rdet032 <= CRat(idet032[1]),
        "Interval does not contain the determinant.");

    auto const& rdet123 = query.mNode[query.det123Node];
    double ddet123 = rdet123; (void)ddet123;
    UTAssert(
        rdet123.GetSign() == 0 &&
        rdet123.GetBiasedExponent() == 0 &&
        rdet123.GetUInteger().GetNumBits() == 0,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet123[0]) <= rdet123 && rdet123 <= CRat(idet123[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactToTetrahedron3::Test2()
{
    // (b0,b1,b2,b3) = (-,+,+,+)
    ExactToTetrahedron3<float> query;
    Vector3<float> V0 = { -0.729045987f, -0.811583877f, 0.0f };
    Vector3<float> V1 = { 0.808353007f, 0.588698089f, 0.0f };
    Vector3<float> V2 = { -0.5f, 1.0f, 0.0f };
    Vector3<float> V3 = { 0.0f, 0.0f, 1.0f };
    float b0 = -0.12345f, b1 = 0.67890f, b2 = 0.35791f, b3 = 0.24680f;
    float bsum = b0 + b1 + b2 + b3;
    b0 /= bsum;
    b1 /= bsum;
    b2 /= bsum;
    b3 /= bsum;
    Vector3<float> P = b0 * V0 + b1 * V1 + b2 * V2 + b3 * V3;
    Vector3<IRat> rP = { P[0], P[1], P[2] };
    Vector3<IRat> rV0 = { V0[0], V0[1], V0[2] };
    Vector3<IRat> rV1 = { V1[0], V1[1], V1[2] };
    Vector3<IRat> rV2 = { V2[0], V2[1], V2[2] };
    Vector3<IRat> rV3 = { V3[0], V3[1], V3[2] };

    std::int32_t isign = query(P, V0, V1, V2, V3);
    std::int32_t isign021 = query.mISign021;
    std::int32_t isign013 = query.mISign013;
    std::int32_t isign032 = query.mISign032;
    std::int32_t isign123 = query.mISign123;
    auto const& idet021 = query.mIDet021;
    auto const& idet013 = query.mIDet013;
    auto const& idet032 = query.mIDet032;
    auto const& idet123 = query.mIDet123;
    UTAssert(isign == 1 &&
        isign021 == -1 && isign013 == -1 &&
        isign032 == -1 && isign123 == 1,
        "Incorrect sign.");
    UTAssert(
        idet021[0] == -0.524250090f && idet021[1] == -0.524249315f &&
        idet013[0] == -0.760270715f && idet013[1] == -0.760266066f &&
        idet032[0] == -1.44211316f && idet032[1] == -1.44210982f &&
        idet123[0] == 0.262230664f && idet123[1] == 0.262231380f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2, &rV3]()
    {
        return std::array<Vector3<IRat> const*, 5>{ &rP, &rV0, &rV1, &rV2, &rV3 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, V3, GetRPoints);
    UTAssert(rsign == 1 &&
        isign021 == -1 && isign013 == -1 &&
        isign032 == -1 && isign123 == 1,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2, rV3);

    auto const& rdet021 = query.mNode[query.det021Node];
    double ddet021 = rdet021; (void)ddet021;
    UTAssert(
        rdet021.GetSign() == -1 &&
        rdet021.GetBiasedExponent() == -68 &&
        rdet021.GetUInteger().GetNumBits() == 68 &&
        rdet021.GetUInteger().GetBits()[0] == 0xc84fed7f &&
        rdet021.GetUInteger().GetBits()[1] == 0x63539ba2 &&
        rdet021.GetUInteger().GetBits()[2] == 0x00000008,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet021[0]) <= rdet021 && rdet021 <= CRat(idet021[1]),
        "Interval does not contain the determinant.");

    auto const& rdet013 = query.mNode[query.det013Node];
    double ddet013 = rdet013; (void)ddet013;
    UTAssert(
        rdet013.GetSign() == -1 &&
        rdet013.GetBiasedExponent() == -68 &&
        rdet013.GetUInteger().GetNumBits() == 68 &&
        rdet013.GetUInteger().GetBits()[0] == 0xef381281 &&
        rdet013.GetUInteger().GetBits()[1] == 0x2a0f05e5 &&
        rdet013.GetUInteger().GetBits()[2] == 0x0000000c,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet013[0]) <= rdet013 && rdet013 <= CRat(idet013[1]),
        "Interval does not contain the determinant.");

    auto const& rdet032 = query.mNode[query.det032Node];
    double ddet032 = rdet032; (void)ddet032;
    UTAssert(
        rdet032.GetSign() == -1 &&
        rdet032.GetBiasedExponent() == -48 &&
        rdet032.GetUInteger().GetNumBits() == 49 &&
        rdet032.GetUInteger().GetBits()[0] == 0x36727725 &&
        rdet032.GetUInteger().GetBits()[1] == 0x0001712e,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet032[0]) <= rdet032 && rdet032 <= CRat(idet032[1]),
        "Interval does not contain the determinant.");

    auto const& rdet123 = query.mNode[query.det123Node];
    double ddet123 = rdet123; (void)ddet123;
    UTAssert(
        rdet123.GetSign() == 1 &&
        rdet123.GetBiasedExponent() == -49 &&
        rdet123.GetUInteger().GetNumBits() == 48 &&
        rdet123.GetUInteger().GetBits()[0] == 0x251b21db &&
        rdet123.GetUInteger().GetBits()[1] == 0x00008643,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet123[0]) <= rdet123 && rdet123 <= CRat(idet123[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactToTetrahedron3::Test3()
{
    // (b0,b1,b2,b3) = (+,-,+,+)
    ExactToTetrahedron3<float> query;
    Vector3<float> V0 = { -0.729045987f, -0.811583877f, 0.0f };
    Vector3<float> V1 = { 0.808353007f, 0.588698089f, 0.0f };
    Vector3<float> V2 = { -0.5f, 1.0f, 0.0f };
    Vector3<float> V3 = { 0.0f, 0.0f, 1.0f };
    float b0 = 0.12345f, b1 = -0.67890f, b2 = 0.35791f, b3 = 0.24680f;
    float bsum = b0 + b1 + b2 + b3;
    b0 /= bsum;
    b1 /= bsum;
    b2 /= bsum;
    b3 /= bsum;
    Vector3<float> P = b0 * V0 + b1 * V1 + b2 * V2 + b3 * V3;
    Vector3<IRat> rP = { P[0], P[1], P[2] };
    Vector3<IRat> rV0 = { V0[0], V0[1], V0[2] };
    Vector3<IRat> rV1 = { V1[0], V1[1], V1[2] };
    Vector3<IRat> rV2 = { V2[0], V2[1], V2[2] };
    Vector3<IRat> rV3 = { V3[0], V3[1], V3[2] };

    std::int32_t isign = query(P, V0, V1, V2, V3);
    std::int32_t isign021 = query.mISign021;
    std::int32_t isign013 = query.mISign013;
    std::int32_t isign032 = query.mISign032;
    std::int32_t isign123 = query.mISign123;
    auto const& idet021 = query.mIDet021;
    auto const& idet013 = query.mIDet013;
    auto const& idet032 = query.mIDet032;
    auto const& idet123 = query.mIDet123;
    UTAssert(isign == 1 &&
        isign021 == -1 && isign013 == -1 &&
        isign032 == 1 && isign123 == query.invalidSign,
        "Incorrect sign.");
    UTAssert(
        idet021[0] == -12.3470182f && idet021[1] == -12.3470011f &&
        idet013[0] == -17.9056835f && idet013[1] == -17.9056396f &&
        idet032[0] == 33.9642563f && idet032[1] == 33.9643021f &&
        idet123[0] == 0.0f && idet123[1] == 0.0f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2, &rV3]()
    {
        return std::array<Vector3<IRat> const*, 5>{ &rP, &rV0, &rV1, &rV2, &rV3 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, V3, GetRPoints);
    UTAssert(rsign == 1 &&
        isign021 == -1 && isign013 == -1 &&
        isign032 == 1 && isign123 == query.invalidSign,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2, rV3);

    auto const& rdet021 = query.mNode[query.det021Node];
    double ddet021 = rdet021; (void)ddet021;
    UTAssert(
        rdet021.GetSign() == -1 &&
        rdet021.GetBiasedExponent() == -65 &&
        rdet021.GetUInteger().GetNumBits() == 69 &&
        rdet021.GetUInteger().GetBits()[0] == 0x33773453 &&
        rdet021.GetUInteger().GetBits()[1] == 0xb1ab1518 &&
        rdet021.GetUInteger().GetBits()[2] == 0x00000018,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet021[0]) <= rdet021 && rdet021 <= CRat(idet021[1]),
        "Interval does not contain the determinant.");

    auto const& rdet013 = query.mNode[query.det013Node];
    double ddet013 = rdet013; (void)ddet013;
    UTAssert(
        rdet013.GetSign() == -1 &&
        rdet013.GetBiasedExponent() == -65 &&
        rdet013.GetUInteger().GetNumBits() == 70 &&
        rdet013.GetUInteger().GetBits()[0] == 0xd010cbad &&
        rdet013.GetUInteger().GetBits()[1] == 0xcfb30a9d &&
        rdet013.GetUInteger().GetBits()[2] == 0x00000023,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet013[0]) <= rdet013 && rdet013 <= CRat(idet013[1]),
        "Interval does not contain the determinant.");

    auto const& rdet032 = query.mNode[query.det032Node];
    double ddet032 = rdet032; (void)ddet032;
    UTAssert(
        rdet032.GetSign() == 1 &&
        rdet032.GetBiasedExponent() == -45 &&
        rdet032.GetUInteger().GetNumBits() == 51 &&
        rdet032.GetUInteger().GetBits()[0] == 0x5d4b4ce7 &&
        rdet032.GetUInteger().GetBits()[1] == 0x00043edb,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet032[0]) <= rdet032 && rdet032 <= CRat(idet032[1]),
        "Interval does not contain the determinant.");

    auto const& rdet123 = query.mNode[query.det123Node];
    double ddet123 = rdet123; (void)ddet123;
    UTAssert(
        rdet123.GetSign() == 0 &&
        rdet123.GetBiasedExponent() == 0 &&
        rdet123.GetUInteger().GetNumBits() == 0,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet123[0]) <= rdet123 && rdet123 <= CRat(idet123[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactToTetrahedron3::Test4()
{
    // (b0,b1,b2,b3) = (+,+,-,+)
    ExactToTetrahedron3<float> query;
    Vector3<float> V0 = { -0.729045987f, -0.811583877f, 0.0f };
    Vector3<float> V1 = { 0.808353007f, 0.588698089f, 0.0f };
    Vector3<float> V2 = { -0.5f, 1.0f, 0.0f };
    Vector3<float> V3 = { 0.0f, 0.0f, 1.0f };
    float b0 = 0.12345f, b1 = 0.67890f, b2 = -0.35791f, b3 = 0.24680f;
    float bsum = b0 + b1 + b2 + b3;
    b0 /= bsum;
    b1 /= bsum;
    b2 /= bsum;
    b3 /= bsum;
    Vector3<float> P = b0 * V0 + b1 * V1 + b2 * V2 + b3 * V3;
    Vector3<IRat> rP = { P[0], P[1], P[2] };
    Vector3<IRat> rV0 = { V0[0], V0[1], V0[2] };
    Vector3<IRat> rV1 = { V1[0], V1[1], V1[2] };
    Vector3<IRat> rV2 = { V2[0], V2[1], V2[2] };
    Vector3<IRat> rV3 = { V3[0], V3[1], V3[2] };

    std::int32_t isign = query(P, V0, V1, V2, V3);
    std::int32_t isign021 = query.mISign021;
    std::int32_t isign013 = query.mISign013;
    std::int32_t isign032 = query.mISign032;
    std::int32_t isign123 = query.mISign123;
    auto const& idet021 = query.mIDet021;
    auto const& idet013 = query.mIDet013;
    auto const& idet032 = query.mIDet032;
    auto const& idet123 = query.mIDet123;
    UTAssert(isign == 1 &&
        isign021 == -1 && isign013 == 1 &&
        isign032 == query.invalidSign && isign123 == query.invalidSign,
        "Incorrect sign.");
    UTAssert(
        idet021[0] == -0.879888296f && idet021[1] == -0.879887044f &&
        idet013[0] == 1.27601326f && idet013[1] == 1.27601767f &&
        idet032[0] == 0.0f && idet032[1] == 0.0f &&
        idet123[0] == 0.0f && idet123[1] == 0.0f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2, &rV3]()
    {
        return std::array<Vector3<IRat> const*, 5>{ &rP, &rV0, &rV1, &rV2, &rV3 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, V3, GetRPoints);
    UTAssert(rsign == 1 &&
        isign021 == -1 && isign013 == 1 &&
        isign032 == query.invalidSign && isign123 == query.invalidSign,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2, rV3);

    auto const& rdet021 = query.mNode[query.det021Node];
    double ddet021 = rdet021; (void)ddet021;
    UTAssert(
        rdet021.GetSign() == -1 &&
        rdet021.GetBiasedExponent() == -69 &&
        rdet021.GetUInteger().GetNumBits() == 69 &&
        rdet021.GetUInteger().GetBits()[0] == 0xf6ee5bf1 &&
        rdet021.GetUInteger().GetBits()[1] == 0x280a17c3 &&
        rdet021.GetUInteger().GetBits()[2] == 0x0000001c,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet021[0]) <= rdet021 && rdet021 <= CRat(idet021[1]),
        "Interval does not contain the determinant.");

    auto const& rdet013 = query.mNode[query.det013Node];
    double ddet013 = rdet013; (void)ddet013;
    UTAssert(
        rdet013.GetSign() == 1 &&
        rdet013.GetBiasedExponent() == -69 &&
        rdet013.GetUInteger().GetNumBits() == 70 &&
        rdet013.GetUInteger().GetBits()[0] == 0x08665bf1 &&
        rdet013.GetUInteger().GetBits()[1] == 0xd51e2a90 &&
        rdet013.GetUInteger().GetBits()[2] == 0x00000028,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet013[0]) <= rdet013 && rdet013 <= CRat(idet013[1]),
        "Interval does not contain the determinant.");

    auto const& rdet032 = query.mNode[query.det032Node];
    double ddet032 = rdet032; (void)ddet032;
    UTAssert(
        rdet032.GetSign() == 0 &&
        rdet032.GetBiasedExponent() == 0 &&
        rdet032.GetUInteger().GetNumBits() == 0,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet032[0]) <= rdet032 && rdet032 <= CRat(idet032[1]),
        "Interval does not contain the determinant.");

    auto const& rdet123 = query.mNode[query.det123Node];
    double ddet123 = rdet123; (void)ddet123;
    UTAssert(
        rdet123.GetSign() == 0 &&
        rdet123.GetBiasedExponent() == 0 &&
        rdet123.GetUInteger().GetNumBits() == 0,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet123[0]) <= rdet123 && rdet123 <= CRat(idet123[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactToTetrahedron3::Test5()
{
    // (b0,b1,b2,b3) = (+,+,+,0)
    ExactToTetrahedron3<float> query;
    Vector3<float> V0 = { 0.0f, 0.0f, 0.0f };
    Vector3<float> V1 = { 1.0f, 0.0f, 0.0f };
    Vector3<float> V2 = { 0.0f, 2.0f, 0.0f };
    Vector3<float> V3 = { 0.0f, 0.0f, 4.0f };
    float b0 = 0.25f, b1 = 0.5f, b2 = 0.25f, b3 = 0.0f;
    Vector3<float> P = b0 * V0 + b1 * V1 + b2 * V2 + b3 * V3;
    Vector3<IRat> rP = { P[0], P[1], P[2] };
    Vector3<IRat> rV0 = { V0[0], V0[1], V0[2] };
    Vector3<IRat> rV1 = { V1[0], V1[1], V1[2] };
    Vector3<IRat> rV2 = { V2[0], V2[1], V2[2] };
    Vector3<IRat> rV3 = { V3[0], V3[1], V3[2] };

    std::int32_t isign = query(P, V0, V1, V2, V3);
    std::int32_t isign021 = query.mISign021;
    std::int32_t isign013 = query.mISign013;
    std::int32_t isign032 = query.mISign032;
    std::int32_t isign123 = query.mISign123;
    auto const& idet021 = query.mIDet021;
    auto const& idet013 = query.mIDet013;
    auto const& idet032 = query.mIDet032;
    auto const& idet123 = query.mIDet123;
    UTAssert(isign == 0 &&
        isign021 == query.invalidSign && isign013 == -1 &&
        isign032 == -1 && isign123 == -1,
        "Incorrect sign.");
    UTAssert(
        idet021[0] == -1.96181e-44f && idet021[1] == +1.96181e-44f &&
        idet013[0] == -2.00000191f && idet013[1] == -1.99999905f &&
        idet032[0] == -4.00000381f && idet032[1] == -3.99999809f &&
        idet123[0] == -2.00000405f && idet123[1] == -1.99999690f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2, &rV3]()
    {
        return std::array<Vector3<IRat> const*, 5>{ &rP, &rV0, &rV1, &rV2, &rV3 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, V3, GetRPoints);
    UTAssert(rsign == 0 &&
        isign021 == query.invalidSign && isign013 == -1 &&
        isign032 == -1 && isign123 == -1,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2, rV3);

    auto const& rdet021 = query.mNode[query.det021Node];
    double ddet021 = rdet021; (void)ddet021;
    UTAssert(
        rdet021.GetSign() == 0 &&
        rdet021.GetBiasedExponent() == 0 &&
        rdet021.GetUInteger().GetNumBits() == 0,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet021[0]) <= rdet021 && rdet021 <= CRat(idet021[1]),
        "Interval does not contain the determinant.");

    auto const& rdet013 = query.mNode[query.det013Node];
    double ddet013 = rdet013; (void)ddet013;
    UTAssert(
        rdet013.GetSign() == -1 &&
        rdet013.GetBiasedExponent() == 1 &&
        rdet013.GetUInteger().GetNumBits() == 1 &&
        rdet013.GetUInteger().GetBits()[0] == 0x00000001,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet013[0]) <= rdet013 && rdet013 <= CRat(idet013[1]),
        "Interval does not contain the determinant.");

    auto const& rdet032 = query.mNode[query.det032Node];
    double ddet032 = rdet032; (void)ddet032;
    UTAssert(
        rdet032.GetSign() == -1 &&
        rdet032.GetBiasedExponent() == 2 &&
        rdet032.GetUInteger().GetNumBits() == 1 &&
        rdet032.GetUInteger().GetBits()[0] == 0x00000001,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet032[0]) <= rdet032 && rdet032 <= CRat(idet032[1]),
        "Interval does not contain the determinant.");

    auto const& rdet123 = query.mNode[query.det123Node];
    double ddet123 = rdet123; (void)ddet123;
    UTAssert(
        rdet123.GetSign() == -1 &&
        rdet123.GetBiasedExponent() == 1 &&
        rdet123.GetUInteger().GetNumBits() == 1 &&
        rdet123.GetUInteger().GetBits()[0] == 0x00000001,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet123[0]) <= rdet123 && rdet123 <= CRat(idet123[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactToTetrahedron3::Test6()
{
    // (b0,b1,b2,b3) = (+,+,0,+)
    ExactToTetrahedron3<float> query;
    Vector3<float> V0 = { 0.0f, 0.0f, 0.0f };
    Vector3<float> V1 = { 1.0f, 0.0f, 0.0f };
    Vector3<float> V2 = { 0.0f, 2.0f, 0.0f };
    Vector3<float> V3 = { 0.0f, 0.0f, 4.0f };
    float b0 = 0.25f, b1 = 0.5f, b2 = 0.0f, b3 = 0.25f;
    Vector3<float> P = b0 * V0 + b1 * V1 + b2 * V2 + b3 * V3;
    Vector3<IRat> rP = { P[0], P[1], P[2] };
    Vector3<IRat> rV0 = { V0[0], V0[1], V0[2] };
    Vector3<IRat> rV1 = { V1[0], V1[1], V1[2] };
    Vector3<IRat> rV2 = { V2[0], V2[1], V2[2] };
    Vector3<IRat> rV3 = { V3[0], V3[1], V3[2] };

    std::int32_t isign = query(P, V0, V1, V2, V3);
    std::int32_t isign021 = query.mISign021;
    std::int32_t isign013 = query.mISign013;
    std::int32_t isign032 = query.mISign032;
    std::int32_t isign123 = query.mISign123;
    auto const& idet021 = query.mIDet021;
    auto const& idet013 = query.mIDet013;
    auto const& idet032 = query.mIDet032;
    auto const& idet123 = query.mIDet123;
    UTAssert(isign == 0 &&
        isign021 == -1 && isign013 == query.invalidSign &&
        isign032 == -1 && isign123 == -1,
        "Incorrect sign.");
    UTAssert(
        idet021[0] == -2.00000167f && idet021[1] == -1.99999917f &&
        idet013[0] == -2.38220e-44f && idet013[1] == 2.38220e-44f &&
        idet032[0] == -4.00000381f && idet032[1] == -3.99999809f &&
        idet123[0] == -2.00000429f && idet123[1] == -1.99999678f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2, &rV3]()
    {
        return std::array<Vector3<IRat> const*, 5>{ &rP, &rV0, &rV1, &rV2, &rV3 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, V3, GetRPoints);
    UTAssert(rsign == 0 &&
        isign021 == -1 && isign013 == query.invalidSign &&
        isign032 == -1 && isign123 == -1,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2, rV3);

    auto const& rdet021 = query.mNode[query.det021Node];
    double ddet021 = rdet021; (void)ddet021;
    UTAssert(
        rdet021.GetSign() == -1 &&
        rdet021.GetBiasedExponent() == 1 &&
        rdet021.GetUInteger().GetNumBits() == 1 &&
        rdet021.GetUInteger().GetBits()[0] == 0x0000001,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet021[0]) <= rdet021 && rdet021 <= CRat(idet021[1]),
        "Interval does not contain the determinant.");

    auto const& rdet013 = query.mNode[query.det013Node];
    double ddet013 = rdet013; (void)ddet013;
    UTAssert(
        rdet013.GetSign() == 0 &&
        rdet013.GetBiasedExponent() == 0 &&
        rdet013.GetUInteger().GetNumBits() == 0,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet013[0]) <= rdet013 && rdet013 <= CRat(idet013[1]),
        "Interval does not contain the determinant.");

    auto const& rdet032 = query.mNode[query.det032Node];
    double ddet032 = rdet032; (void)ddet032;
    UTAssert(
        rdet032.GetSign() == -1 &&
        rdet032.GetBiasedExponent() == 2 &&
        rdet032.GetUInteger().GetNumBits() == 1 &&
        rdet032.GetUInteger().GetBits()[0] == 0x00000001,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet032[0]) <= rdet032 && rdet032 <= CRat(idet032[1]),
        "Interval does not contain the determinant.");

    auto const& rdet123 = query.mNode[query.det123Node];
    double ddet123 = rdet123; (void)ddet123;
    UTAssert(
        rdet123.GetSign() == -1 &&
        rdet123.GetBiasedExponent() == 1 &&
        rdet123.GetUInteger().GetNumBits() == 1 &&
        rdet123.GetUInteger().GetBits()[0] == 0x00000001,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet123[0]) <= rdet123 && rdet123 <= CRat(idet123[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactToTetrahedron3::Test7()
{
    // (b0,b1,b2,b3) = (+,0,+,+)
    ExactToTetrahedron3<float> query;
    Vector3<float> V0 = { 0.0f, 0.0f, 0.0f };
    Vector3<float> V1 = { 1.0f, 0.0f, 0.0f };
    Vector3<float> V2 = { 0.0f, 2.0f, 0.0f };
    Vector3<float> V3 = { 0.0f, 0.0f, 4.0f };
    float b0 = 0.25f, b1 = 0.0f, b2 = 0.5f, b3 = 0.25f;
    Vector3<float> P = b0 * V0 + b1 * V1 + b2 * V2 + b3 * V3;
    Vector3<IRat> rP = { P[0], P[1], P[2] };
    Vector3<IRat> rV0 = { V0[0], V0[1], V0[2] };
    Vector3<IRat> rV1 = { V1[0], V1[1], V1[2] };
    Vector3<IRat> rV2 = { V2[0], V2[1], V2[2] };
    Vector3<IRat> rV3 = { V3[0], V3[1], V3[2] };

    std::int32_t isign = query(P, V0, V1, V2, V3);
    std::int32_t isign021 = query.mISign021;
    std::int32_t isign013 = query.mISign013;
    std::int32_t isign032 = query.mISign032;
    std::int32_t isign123 = query.mISign123;
    auto const& idet021 = query.mIDet021;
    auto const& idet013 = query.mIDet013;
    auto const& idet032 = query.mIDet032;
    auto const& idet123 = query.mIDet123;
    UTAssert(isign == 0 &&
        isign021 == -1 && isign013 == -1 &&
        isign032 == query.invalidSign && isign123 == -1,
        "Incorrect sign.");
    UTAssert(
        idet021[0] == -2.00000167f && idet021[1] == -1.99999917f &&
        idet013[0] == -4.00000381f && idet013[1] == -3.99999809f &&
        idet032[0] == -2.66246e-44f && idet032[1] == 2.66246e-44f &&
        idet123[0] == -2.00000858f && idet123[1] == -1.99999249f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2, &rV3]()
    {
        return std::array<Vector3<IRat> const*, 5>{ &rP, &rV0, &rV1, &rV2, &rV3 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, V3, GetRPoints);
    UTAssert(rsign == 0 &&
        isign021 == -1 && isign013 == -1 &&
        isign032 == query.invalidSign && isign123 == -1,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2, rV3);

    auto const& rdet021 = query.mNode[query.det021Node];
    double ddet021 = rdet021; (void)ddet021;
    UTAssert(
        rdet021.GetSign() == -1 &&
        rdet021.GetBiasedExponent() == 1 &&
        rdet021.GetUInteger().GetNumBits() == 1 &&
        rdet021.GetUInteger().GetBits()[0] == 0x0000001,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet021[0]) <= rdet021 && rdet021 <= CRat(idet021[1]),
        "Interval does not contain the determinant.");

    auto const& rdet013 = query.mNode[query.det013Node];
    double ddet013 = rdet013; (void)ddet013;
    UTAssert(
        rdet013.GetSign() == -1 &&
        rdet013.GetBiasedExponent() == 2 &&
        rdet013.GetUInteger().GetNumBits() == 1 &&
        rdet013.GetUInteger().GetBits()[0] == 0x0000001,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet013[0]) <= rdet013 && rdet013 <= CRat(idet013[1]),
        "Interval does not contain the determinant.");

    auto const& rdet032 = query.mNode[query.det032Node];
    double ddet032 = rdet032; (void)ddet032;
    UTAssert(
        rdet032.GetSign() == 0 &&
        rdet032.GetBiasedExponent() == 0 &&
        rdet032.GetUInteger().GetNumBits() == 0,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet032[0]) <= rdet032 && rdet032 <= CRat(idet032[1]),
        "Interval does not contain the determinant.");

    auto const& rdet123 = query.mNode[query.det123Node];
    double ddet123 = rdet123; (void)ddet123;
    UTAssert(
        rdet123.GetSign() == -1 &&
        rdet123.GetBiasedExponent() == 1 &&
        rdet123.GetUInteger().GetNumBits() == 1 &&
        rdet123.GetUInteger().GetBits()[0] == 0x00000001,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet123[0]) <= rdet123 && rdet123 <= CRat(idet123[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactToTetrahedron3::Test8()
{
    // (b0,b1,b2,b3) = (0,+,+,+)
    ExactToTetrahedron3<float> query;
    Vector3<float> V0 = { 0.0f, 0.0f, 0.0f };
    Vector3<float> V1 = { 1.0f, 0.0f, 0.0f };
    Vector3<float> V2 = { 0.0f, 2.0f, 0.0f };
    Vector3<float> V3 = { 0.0f, 0.0f, 4.0f };
    float b0 = 0.0f, b1 = 0.25f, b2 = 0.5f, b3 = 0.25f;
    Vector3<float> P = b0 * V0 + b1 * V1 + b2 * V2 + b3 * V3;
    Vector3<IRat> rP = { P[0], P[1], P[2] };
    Vector3<IRat> rV0 = { V0[0], V0[1], V0[2] };
    Vector3<IRat> rV1 = { V1[0], V1[1], V1[2] };
    Vector3<IRat> rV2 = { V2[0], V2[1], V2[2] };
    Vector3<IRat> rV3 = { V3[0], V3[1], V3[2] };

    std::int32_t isign = query(P, V0, V1, V2, V3);
    std::int32_t isign021 = query.mISign021;
    std::int32_t isign013 = query.mISign013;
    std::int32_t isign032 = query.mISign032;
    std::int32_t isign123 = query.mISign123;
    auto const& idet021 = query.mIDet021;
    auto const& idet013 = query.mIDet013;
    auto const& idet032 = query.mIDet032;
    auto const& idet123 = query.mIDet123;
    UTAssert(isign == 0 &&
        isign021 == -1 && isign013 == -1 &&
        isign032 == -1 && isign123 == query.invalidSign,
        "Incorrect sign.");
    UTAssert(
        idet021[0] == -2.00000167f && idet021[1] == -1.99999917f &&
        idet013[0] == -4.00000381f && idet013[1] == -3.99999809f &&
        idet032[0] == -2.00000191f && idet032[1] == -1.99999905f &&
        idet123[0] == -6.19888351e-06f && idet123[1] == 6.79492996e-06f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2, &rV3]()
    {
        return std::array<Vector3<IRat> const*, 5>{ &rP, &rV0, &rV1, &rV2, &rV3 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, V3, GetRPoints);
    UTAssert(rsign == 0 &&
        isign021 == -1 && isign013 == -1 &&
        isign032 == -1 && isign123 == query.invalidSign,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2, rV3);

    auto const& rdet021 = query.mNode[query.det021Node];
    double ddet021 = rdet021; (void)ddet021;
    UTAssert(
        rdet021.GetSign() == -1 &&
        rdet021.GetBiasedExponent() == 1 &&
        rdet021.GetUInteger().GetNumBits() == 1 &&
        rdet021.GetUInteger().GetBits()[0] == 0x0000001,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet021[0]) <= rdet021 && rdet021 <= CRat(idet021[1]),
        "Interval does not contain the determinant.");

    auto const& rdet013 = query.mNode[query.det013Node];
    double ddet013 = rdet013; (void)ddet013;
    UTAssert(
        rdet013.GetSign() == -1 &&
        rdet013.GetBiasedExponent() == 2 &&
        rdet013.GetUInteger().GetNumBits() == 1 &&
        rdet013.GetUInteger().GetBits()[0] == 0x0000001,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet013[0]) <= rdet013 && rdet013 <= CRat(idet013[1]),
        "Interval does not contain the determinant.");

    auto const& rdet032 = query.mNode[query.det032Node];
    double ddet032 = rdet032; (void)ddet032;
    UTAssert(
        rdet032.GetSign() == -1 &&
        rdet032.GetBiasedExponent() == 1 &&
        rdet032.GetUInteger().GetNumBits() == 1 &&
        rdet032.GetUInteger().GetBits()[0] == 0x0000001,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet032[0]) <= rdet032 && rdet032 <= CRat(idet032[1]),
        "Interval does not contain the determinant.");

    auto const& rdet123 = query.mNode[query.det123Node];
    double ddet123 = rdet123; (void)ddet123;
    UTAssert(
        rdet123.GetSign() == 0 &&
        rdet123.GetBiasedExponent() == 0 &&
        rdet123.GetUInteger().GetNumBits() == 0,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(idet123[0]) <= rdet123 && rdet123 <= CRat(idet123[1]),
        "Interval does not contain the determinant.");
}

#else

#include <GTL/Mathematics/Geometry/3D/ExactToTetrahedron3.h>

namespace gtl
{
    template class ExactToTetrahedron3<float>;
    template class ExactToTetrahedron3<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ExactToTetrahedron3)
