#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/3D/ExactToCircumsphere3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestExactToCircumsphere3
    {
    public:
        UnitTestExactToCircumsphere3();

    private:
        using IRat = ExactToCircumsphere3<float>::Rational;
        using CRat = ExactToCircumsphere3<float>::CRational;

        void Test0();
        void Test1();
        void Test2();
    };
}

UnitTestExactToCircumsphere3::UnitTestExactToCircumsphere3()
{
    UTInformation("Mathematics/Geometry/3D/ExactToCircumsphere3");

    Test0();
    Test1();
    Test2();
}

void UnitTestExactToCircumsphere3::Test0()
{
    ExactToCircumsphere3<float> query;
    Vector3<float> P, V0, V1, V2, V3;
    Vector3<IRat> rP, rV0, rV1, rV2, rV3;

    V0 = { 0.0f, 0.0f, 0.0f };
    V1 = { 1.0f, 0.0f, 0.0f };
    V2 = { 0.0f, 1.0f, 0.0f };
    V3 = { 0.0f, 0.0f, 1.0f };
    P = { 0.99999f, 0.99999f, 0.99999f };  // inside the circumsphere
    rP = { P[0], P[1], P[2] };
    rV0 = { V0[0], V0[1], V0[2] };
    rV1 = { V1[0], V1[1], V1[2] };
    rV2 = { V2[0], V2[1], V2[2] };
    rV3 = { V3[0], V3[1], V3[2] };

    std::int32_t isign = query(P, V0, V1, V2, V3);
    UTAssert(isign == -1 && query.mISign == -1,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == -4.05907667e-05f &&
        query.mIDet[1] == -1.94907170e-05f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2, &rV3]()
    {
        return std::array<Vector3<IRat> const*, 5>{ &rP, &rV0, &rV1, &rV2, &rV3 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, V3, GetRPoints);
    UTAssert(rsign == -1 && query.mISign == -1,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2, rV3);
    auto const& rdet = query.mNode[query.detNode];
    UTAssert(
        rdet.GetSign() == -1 &&
        rdet.GetBiasedExponent() == -42 &&
        rdet.GetUInteger().GetNumBits() == 27 &&
        rdet.GetUInteger().GetBits()[0] == 0x07dffad5,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(query.mIDet[0]) <= rdet &&
        rdet <= CRat(query.mIDet[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactToCircumsphere3::Test1()
{
    ExactToCircumsphere3<float> query;
    Vector3<float> P, V0, V1, V2, V3;
    Vector3<IRat> rP, rV0, rV1, rV2, rV3;

    V0 = { 0.0f, 0.0f, 0.0f };
    V1 = { 1.0f, 0.0f, 0.0f };
    V2 = { 0.0f, 1.0f, 0.0f };
    V3 = { 0.0f, 0.0f, 1.0f };
    P = { 1.00001f, 1.00001f, 1.00001f };  // outside the circumsphere
    rP = { P[0], P[1], P[2] };
    rV0 = { V0[0], V0[1], V0[2] };
    rV1 = { V1[0], V1[1], V1[2] };
    rV2 = { V2[0], V2[1], V2[2] };
    rV3 = { V3[0], V3[1], V3[2] };

    std::int32_t isign = query(P, V0, V1, V2, V3);
    UTAssert(isign == 1 && query.mISign == 1,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == 9.89437012e-06f &&
        query.mIDet[1] == 4.97102774e-05f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2, &rV3]()
    {
        return std::array<Vector3<IRat> const*, 5>{ &rP, &rV0, &rV1, &rV2, &rV3 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, V3, GetRPoints);
    UTAssert(rsign == 1 && query.mISign == 1,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2, rV3);
    auto const& rdet = query.mNode[query.detNode];
    UTAssert(
        rdet.GetSign() == 1 &&
        rdet.GetBiasedExponent() == -42 &&
        rdet.GetUInteger().GetNumBits() == 27 &&
        rdet.GetUInteger().GetBits()[0] == 0x07e0052b,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(query.mIDet[0]) <= rdet &&
        rdet <= CRat(query.mIDet[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactToCircumsphere3::Test2()
{
    ExactToCircumsphere3<float> query;
    Vector3<float> P, V0, V1, V2, V3;
    Vector3<IRat> rP, rV0, rV1, rV2, rV3;

    union { std::uint32_t encoding; float number; } onePlus = { 0x3F800001u };

    V0 = { 0.0f, 0.0f, 0.0f };
    V1 = { 1.0f, 0.0f, 0.0f };
    V2 = { 0.0f, 1.0f, 0.0f };
    V3 = { 0.0f, 0.0f, 1.0f };
    // Outside the circumcircle, but floating-point arithmetic cannot
    // determine this.
    P = { onePlus.number, onePlus.number, onePlus.number };
    rP = { P[0], P[1], P[2] };
    rV0 = { V0[0], V0[1], V0[2] };
    rV1 = { V1[0], V1[1], V1[2] };
    rV2 = { V2[0], V2[1], V2[2] };
    rV3 = { V3[0], V3[1], V3[2] };

    std::int32_t isign = query(P, V0, V1, V2, V3);
    UTAssert(isign == 1 && query.mISign == query.invalidSign,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == -1.56760234e-05f &&
        query.mIDet[1] == 1.72853488e-05f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2, &rV3]()
    {
        return std::array<Vector3<IRat> const*, 5>{ &rP, &rV0, &rV1, &rV2, &rV3 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, V3, GetRPoints);
    UTAssert(rsign == 1 && query.mISign == query.invalidSign,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2, rV3);
    auto const& rdet = query.mNode[query.detNode];
    UTAssert(
        rdet.GetSign() == 1 &&
        rdet.GetBiasedExponent() == -46 &&
        rdet.GetUInteger().GetNumBits() == 25 &&
        rdet.GetUInteger().GetBits()[0] == 0x01800003,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(query.mIDet[0]) <= rdet &&
        rdet <= CRat(query.mIDet[1]),
        "Interval does not contain the determinant.");
}

#else

#include <GTL/Mathematics/Geometry/3D/ExactToCircumsphere3.h>

namespace gtl
{
    template class ExactToCircumsphere3<float>;
    template class ExactToCircumsphere3<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ExactToCircumsphere3)
