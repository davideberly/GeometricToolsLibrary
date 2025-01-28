#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/2D/ExactToLine2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestExactToLine2
    {
    public:
        UnitTestExactToLine2();

    private:
        using IRat = ExactToLine2<float>::Rational;
        using CRat = ExactToLine2<float>::CRational;

        void Test0();
        void Test1();
    };
}

UnitTestExactToLine2::UnitTestExactToLine2()
{
    UTInformation("Mathematics/Geometry/2D/ExactToLine2");

    Test0();
    Test1();
}

void UnitTestExactToLine2::Test0()
{
    ExactToLine2<float> query;
    Vector2<float> origin = { -0.729045987f, 0.811583877f };
    Vector2<float> U = { 0.808353007f, 0.588698089f };
    Vector2<float> Uperp = Perp(U); // (0.588698089f, -0.808353007f)
    Vector2<float> V0 = origin;
    Vector2<float> V1 = origin + U;
    Vector2<float> P = origin + 2.0f * U + 1.234f * Uperp;
    Vector2<IRat> rP = { P[0], P[1] };
    Vector2<IRat> rV0 = { V0[0], V0[1] };
    Vector2<IRat> rV1 = { V1[0], V1[1] };

    std::int32_t isign = query(P, V0, V1);
    UTAssert(isign == +1 && query.mISign == +1,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == 1.23399925f &&
        query.mIDet[1] == 1.23400033f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1]()
    {
        return std::array<Vector2<IRat> const*, 3>{ &rP, &rV0, &rV1 };
    };

    std::int32_t rsign = query(P, V0, V1, GetRPoints);
    UTAssert(rsign == 1 && query.mISign == 1,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1);
    auto const& rdet = query.mNode[query.detNode];
    UTAssert(
        rdet.GetSign() == 1 &&
        rdet.GetBiasedExponent() == -47 &&
        rdet.GetUInteger().GetNumBits() == 48 &&
        rdet.GetUInteger().GetBits()[0] == 0xb4ff001d &&
        rdet.GetUInteger().GetBits()[1] == 0x00009df3,
        "Mismatched rational determinants."
    );

    UTAssert(
        CRat(query.mIDet[0]) <= rdet &&
        rdet <= CRat(query.mIDet[1]),
        "Interval does not contain the determinant.");

    P = origin + 2.0f * U - 1.234f * Uperp;
    rP = { P[0], P[1] };
    isign = query(P, V0, V1);
    UTAssert(isign == -1 && query.mISign == -1,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == -1.23400080f &&
        query.mIDet[1] == -1.23399937f,
        "Incorrect interval for sign.");

    rsign = query(P, V0, V1, GetRPoints);
    UTAssert(rsign == -1 && query.mISign == -1,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1);
    UTAssert(
        rdet.GetSign() == -1 &&
        rdet.GetBiasedExponent() == -47 &&
        rdet.GetUInteger().GetNumBits() == 48 &&
        rdet.GetUInteger().GetBits()[0] == 0xb69ce08f &&
        rdet.GetUInteger().GetBits()[1] == 0x00009df3,
        "Mismatched rational determinants."
    );

    UTAssert(
        CRat(query.mIDet[0]) <= rdet &&
        rdet <= CRat(query.mIDet[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactToLine2::Test1()
{
    ExactToLine2<float> query;
    float const epsilon = std::ldexp(1.225f, -75);
    Vector2<float> V0{ 0.0f, 0.0f };
    Vector2<float> V1{ epsilon, 0.0f };
    Vector2<float> P{ 1.0f, -epsilon };  // right-of <V0,V1>
    Vector2<IRat> rP = { P[0], P[1] };
    Vector2<IRat> rV0 = { V0[0], V0[1] };
    Vector2<IRat> rV1 = { V1[0], V1[1] };

    std::int32_t isign = query(P, V0, V1);
    UTAssert(isign == +1 && query.mISign == query.invalidSign,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == -4.20389e-45f &&
        query.mIDet[1] == 7.00649e-45f,
        "Incorrect interval for sign.");

    auto GetRPoints = [&rP, &rV0, &rV1]()
    {
        return std::array<Vector2<IRat> const*, 3>{ &rP, &rV0, &rV1 };
    };

    std::int32_t rsign = query(P, V0, V1, GetRPoints);
    UTAssert(rsign == 1 && query.mISign == query.invalidSign,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1);
    auto const& rdet = query.mNode[query.detNode];
    UTAssert(
        rdet.GetSign() == 1 &&
        rdet.GetBiasedExponent() == -196 &&
        rdet.GetUInteger().GetNumBits() == 47 &&
        rdet.GetUInteger().GetBits()[0] == 0x3daf5c29 &&
        rdet.GetUInteger().GetBits()[1] == 0x0000600a,
        "Mismatched rational determinants."
    );

    UTAssert(
        CRat(query.mIDet[0]) <= rdet &&
        rdet <= CRat(query.mIDet[1]),
        "Interval does not contain the determinant.");

    P = { 1.0f, epsilon };  // left-of
    rP = { P[0], P[1] };

    isign = query(P, V0, V1);
    UTAssert(isign == -1 && query.mISign == query.invalidSign,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == -7.00649e-45f &&
        query.mIDet[1] == 4.20389e-45f,
        "Incorrect interval for sign.");

    rsign = query(P, V0, V1, GetRPoints);
    UTAssert(rsign == -1 && query.mISign == query.invalidSign,
        "Incorrect sign.");

    query.ComputeRational(rP, rV0, rV1);
    UTAssert(
        rdet.GetSign() == -1 &&
        rdet.GetBiasedExponent() == -196 &&
        rdet.GetUInteger().GetNumBits() == 47 &&
        rdet.GetUInteger().GetBits()[0] == 0x3daf5c29 &&
        rdet.GetUInteger().GetBits()[1] == 0x0000600a,
        "Mismatched rational determinants."
    );

    UTAssert(
        CRat(query.mIDet[0]) <= rdet &&
        rdet <= CRat(query.mIDet[1]),
        "Interval does not contain the determinant.");
}

#else

#include <GTL/Mathematics/Geometry/2D/ExactToLine2.h>

namespace gtl
{
    template class ExactToLine2<float>;
    template class ExactToLine2<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ExactToLine2)
