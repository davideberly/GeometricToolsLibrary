#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/2D/ExactToCircumcircle2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestExactToCircumcircle2
    {
    public:
        UnitTestExactToCircumcircle2();

    private:
        using IRat = ExactToCircumcircle2<float>::Rational;
        using CRat = ExactToCircumcircle2<float>::CRational;

        void Test0();
        void Test1();
        void Test2();
    };
}

UnitTestExactToCircumcircle2::UnitTestExactToCircumcircle2()
{
    UTInformation("Mathematics/Geometry/2D/ExactToCircumcircle2");

    Test0();
    Test1();
    Test2();
}

void UnitTestExactToCircumcircle2::Test0()
{
    ExactToCircumcircle2<float> query;
    Vector2<float> P, V0, V1, V2;
    Vector2<IRat> rP, rV0, rV1, rV2;

    V0 = { 0.0f, 0.0f };
    V1 = { 1.0f, 0.0f };
    V2 = { 0.0f, 1.0f };
    P = { 0.999999f, 0.999999f };  // inside the circumcircle
    rP = { P[0], P[1] };
    rV0 = { V0[0], V0[1] };
    rV1 = { V1[0], V1[1] };
    rV2 = { V2[0], V2[1] };
    std::int32_t isign = query(P, V0, V1, V2);
    UTAssert(isign == -1 && query.mISign == -1,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == -3.87430237e-06f &&
        query.mIDet[1] == -1.78813920e-07f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2]()
    {
        return std::array<Vector2<IRat> const*, 4>{ &rP, &rV0, &rV1, &rV2 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, GetRPoints);
    UTAssert(rsign == -1 && query.mISign == -1,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2);
    auto const& rdet = query.mNode[query.detNode];
    UTAssert(
        rdet.GetSign() == -1 &&
        rdet.GetBiasedExponent() == -47 &&
        rdet.GetUInteger().GetNumBits() == 29 &&
        rdet.GetUInteger().GetBits()[0] == 0x10fffedf,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(query.mIDet[0]) <= rdet &&
        rdet <= CRat(query.mIDet[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactToCircumcircle2::Test1()
{
    ExactToCircumcircle2<float> query;
    Vector2<float> P, V0, V1, V2;
    Vector2<IRat> rP, rV0, rV1, rV2;

    V0 = { 0.0f, 0.0f };
    V1 = { 1.0f, 0.0f };
    V2 = { 0.0f, 1.0f };
    P = { 1.00001f, 1.00001f };  // outside the circumcircle
    rP = { P[0], P[1] };
    rV0 = { V0[0], V0[1] };
    rV1 = { V1[0], V1[1] };
    rV2 = { V2[0], V2[1] };
    std::int32_t isign = query(P, V0, V1, V2);
    UTAssert(isign == 1 && query.mISign == 1,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == 1.63316708e-05f &&
        query.mIDet[1] == 2.37226504e-05f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2]()
    {
        return std::array<Vector2<IRat> const*, 4>{ &rP, &rV0, &rV1, &rV2 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, GetRPoints);
    UTAssert(rsign == 1 && query.mISign == 1,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2);
    auto const& rdet = query.mNode[query.detNode];
    UTAssert(
        rdet.GetSign() == 1 &&
        rdet.GetBiasedExponent() == -41 &&
        rdet.GetUInteger().GetNumBits() == 26 &&
        rdet.GetUInteger().GetBits()[0] == 0x02a001b9,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(query.mIDet[0]) <= rdet &&
        rdet <= CRat(query.mIDet[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactToCircumcircle2::Test2()
{
    ExactToCircumcircle2<float> query;
    Vector2<float> P, V0, V1, V2;
    Vector2<IRat> rP, rV0, rV1, rV2;

    union { std::uint32_t encoding; float number; } onePlus = { 0x3F800001u };

    V0 = { 0.0f, 0.0f };
    V1 = { 1.0f, 0.0f };
    V2 = { 0.0f, 1.0f };

    // Outside the circumcircle, but floating-point arithmetic cannot
    // determine this.
    P = { onePlus.number, onePlus.number };
    rP = { P[0], P[1] };
    rV0 = { V0[0], V0[1] };
    rV1 = { V1[0], V1[1] };
    rV2 = { V2[0], V2[1] };

    std::int32_t isign = query(P, V0, V1, V2);
    UTAssert(isign == 1 && query.mISign == query.invalidSign,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == -3.09944176e-06f &&
        query.mIDet[1] == 3.45706962e-06f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1, &rV2]()
    {
        return std::array<Vector2<IRat> const*, 4>{ &rP, &rV0, &rV1, &rV2 };
    };

    std::int32_t rsign = query(P, V0, V1, V2, GetRPoints);
    UTAssert(rsign == 1 && query.mISign == query.invalidSign,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1, rV2);
    auto const& rdet = query.mNode[query.detNode];
    UTAssert(
        rdet.GetSign() == 1 &&
        rdet.GetBiasedExponent() == -45 &&
        rdet.GetUInteger().GetNumBits() == 24 &&
        rdet.GetUInteger().GetBits()[0] == 0x00800001,
        "Mismatched rational determinants."
    );
    UTAssert(
        CRat(query.mIDet[0]) <= rdet &&
        rdet <= CRat(query.mIDet[1]),
        "Interval does not contain the determinant.");
}

#else

#include <GTL/Mathematics/Geometry/2D/ExactToCircumcircle2.h>

namespace gtl
{
    template class ExactToCircumcircle2<float>;
    template class ExactToCircumcircle2<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ExactToCircumcircle2)
