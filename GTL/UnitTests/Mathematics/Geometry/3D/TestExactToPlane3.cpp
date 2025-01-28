#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/3D/ExactToPlane3.h>
#include <GTL/Mathematics/Algebra/Vector.h>
using namespace gtl;

namespace gtl
{
    class UnitTestExactToPlane3
    {
    public:
        UnitTestExactToPlane3();

    private:
        using IRat = ExactToPlane3<float>::Rational;
        using CRat = ExactToPlane3<float>::CRational;

        void Test0();
        void Test1();
    };
}

UnitTestExactToPlane3::UnitTestExactToPlane3()
{
    UTInformation("Mathematics/Geometry/3D/ExactToPlane3");

    Test0();
    Test1();
}

void UnitTestExactToPlane3::Test0()
{
    ExactToPlane3<float> query;
    Vector3<float> origin = { -0.729045987f, 0.811583877f, 0.123454321f };
    Vector3<float> U0 = { 1.0f, 2.0f, 3.0f }, U1, U2;
    ComputeOrthonormalBasis(1, U0, U1, U2);
    Vector3<float> V0 = origin;
    Vector3<float> V1 = origin + U0;
    Vector3<float> V2 = origin + U1;
    Vector3<float> P = origin + 2.0f * U0 + 3.0f * U1 + 1.234f * U2;
    Vector3<IRat> rP = { P[0], P[1], P[2] };
    Vector3<IRat> rV0 = { V0[0], V0[1], V0[2] };
    Vector3<IRat> rV1 = { V1[0], V1[1], V1[2] };
    Vector3<IRat> rV2 = { V2[0], V2[1], V2[2] };

    std::int32_t isign = query(P, V0, V1, V2);
    UTAssert(isign == +1 && query.mISign == +1,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == 1.23399901f &&
        query.mIDet[1] == 1.23400092f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2]()
    {
        return std::array<Vector3<IRat> const*, 4>{ &rP, &rV0, &rV1, &rV2 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, GetRPoints);
    UTAssert(rsign == 1 && query.mISign == 1,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2);
    auto const& rdet = query.mNode[query.detNode];
    UTAssert(
        rdet.GetSign() == 1 &&
        rdet.GetBiasedExponent() == -73 &&
        rdet.GetUInteger().GetNumBits() == 74 &&
        rdet.GetUInteger().GetBits()[0] == 0xe9ede8ad &&
        rdet.GetUInteger().GetBits()[1] == 0xced6ff33 &&
        rdet.GetUInteger().GetBits()[2] == 0x00000277,
        "Mismatched rational determinants."
    );

    UTAssert(
        CRat(query.mIDet[0]) <= rdet &&
        rdet <= CRat(query.mIDet[1]),
        "Interval does not contain the determinant.");

    P = origin + 2.0f * U0 + 3.0f * U1 - 1.234f * U2;
    rP = { P[0], P[1], P[2] };
    isign = query(P, V0, V1, V2);
    UTAssert(isign == -1 && query.mISign == -1,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == -1.23400116f &&
        query.mIDet[1] == -1.23399854f,
        "Incorrect interval for sign.");

    rsign = query(P, V0, V1, V2, GetRPoints);
    UTAssert(rsign == -1 && query.mISign == -1,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2);
    UTAssert(
        rdet.GetSign() == -1 &&
        rdet.GetBiasedExponent() == -73 &&
        rdet.GetUInteger().GetNumBits() == 74 &&
        rdet.GetUInteger().GetBits()[0] == 0xf92b80d3 &&
        rdet.GetUInteger().GetBits()[1] == 0xced47a04 &&
        rdet.GetUInteger().GetBits()[2] == 0x00000277,
        "Mismatched rational determinants."
    );

    UTAssert(
        CRat(query.mIDet[0]) <= rdet &&
        rdet <= CRat(query.mIDet[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactToPlane3::Test1()
{
    ExactToPlane3<float> query;
    float const epsilon = std::ldexp(1.225f, -75);
    Vector3<float> V0{ 0.0f, 0.0f, 0.0f };
    Vector3<float> V1{ epsilon, 0.0f, 0.0f };
    Vector3<float> V2{ 0.0f, epsilon, 0.0f };
    Vector3<float> P{ 0.0f, 0.0f, epsilon };
    Vector3<IRat> rP = { P[0], P[1], P[2] };
    Vector3<IRat> rV0 = { V0[0], V0[1], V0[2] };
    Vector3<IRat> rV1 = { V1[0], V1[1], V1[2] };
    Vector3<IRat> rV2 = { V2[0], V2[1], V2[2] };

    std::int32_t isign = query(P, V0, V1, V2);
    UTAssert(isign == +1 && query.mISign == query.invalidSign,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == -7.006e-45f &&
        query.mIDet[1] == +7.006e-45f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2]()
    {
        return std::array<Vector3<IRat> const*, 4>{ &rP, &rV0, &rV1, &rV2 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, GetRPoints);
    UTAssert(rsign == 1 && query.mISign == query.invalidSign,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2);
    auto const& rdet = query.mNode[query.detNode];
    UTAssert(
        rdet.GetSign() == 1 &&
        rdet.GetBiasedExponent() == -294 &&
        rdet.GetUInteger().GetNumBits() == 70 &&
        rdet.GetUInteger().GetBits()[0] == 0x4bd978d5 &&
        rdet.GetUInteger().GetBits()[1] == 0xd312a837 &&
        rdet.GetUInteger().GetBits()[2] == 0x0000003a,
        "Mismatched rational determinants."
    );

    UTAssert(
        CRat(query.mIDet[0]) <= rdet &&
        rdet <= CRat(query.mIDet[1]),
        "Interval does not contain the determinant.");

    P = { 0.0f, 0.0f, -epsilon };
    rP = { P[0], P[1], P[2] };

    isign = query(P, V0, V1, V2);
    UTAssert(isign == -1 && query.mISign == query.invalidSign,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == -7.006e-45f &&
        query.mIDet[1] == +7.006e-45f,
        "Incorrect interval for sign.");

    rsign = query(P, V0, V1, V2, GetRPoints);
    UTAssert(rsign == -1 && query.mISign == query.invalidSign,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2);
    UTAssert(
        rdet.GetSign() == -1 &&
        rdet.GetBiasedExponent() == -294 &&
        rdet.GetUInteger().GetNumBits() == 70 &&
        rdet.GetUInteger().GetBits()[0] == 0x4bd978d5 &&
        rdet.GetUInteger().GetBits()[1] == 0xd312a837 &&
        rdet.GetUInteger().GetBits()[2] == 0x0000003a,
        "Mismatched rational determinants."
    );

    UTAssert(
        CRat(query.mIDet[0]) <= rdet &&
        rdet <= CRat(query.mIDet[1]),
        "Interval does not contain the determinant.");
}

#else

#include <GTL/Mathematics/Geometry/3D/ExactToPlane3.h>

namespace gtl
{
    template class ExactToPlane3<float>;
    template class ExactToPlane3<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ExactToPlane3)
