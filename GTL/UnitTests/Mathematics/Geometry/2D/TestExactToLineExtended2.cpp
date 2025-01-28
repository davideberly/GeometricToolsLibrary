#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/2D/ExactToLineExtended2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestExactToLineExtended2
    {
    public:
        UnitTestExactToLineExtended2();

    private:
        using QueryType = ExactToLineExtended2<float>;
        using OrderType = ExactToLineExtended2<float>::OrderType;
        using IRat = ExactToLineExtended2<float>::Rational;
        using CRat = ExactToLineExtended2<float>::CRational;

        void TestEqualsEndpoint();
        void TestRightOrLeftOf();
        void TestCollinear();
    };
}

UnitTestExactToLineExtended2::UnitTestExactToLineExtended2()
{
    UTInformation("Mathematics/Geometry/2D/ExactToLineExtended2");

    TestEqualsEndpoint();
    TestRightOrLeftOf();
    TestCollinear();
}

void UnitTestExactToLineExtended2::TestEqualsEndpoint()
{
    QueryType query;
    OrderType order = OrderType::UNKNOWN;
    Vector2<float> P, V0, V1;

    V0 = { 0.12345f, -9.87654f };
    V1 = { 3.579248f, 1.1f };
    P = V0;
    order = query(P, V0, V1);
    UTAssert(order == OrderType::P_EQUALS_V0,
        "Incorrect order.");

    P = V1;
    order = query(P, V0, V1);
    UTAssert(order == OrderType::P_EQUALS_V1,
        "Incorrect order.");

    P = { 1.0f, 2.0f };
    V0 = V1;
    order = query(P, V0, V1);
    UTAssert(order == OrderType::V0_EQUALS_V1,
        "Incorrect order.");

    Vector2<IRat> rP, rV0, rV1;
    auto GetRPoints = [&P, &V0, &V1, &rP, &rV0, &rV1]()
    {
        rP = { P[0], P[1] };
        rV0 = { V0[0], V0[1] };
        rV1 = { V1[0], V1[1] };
        return std::array<Vector2<IRat> const*, 3>{ &rP, &rV0, &rV1 };
    };

    order = query(P, V0, V1, GetRPoints);
    UTAssert(order == OrderType::V0_EQUALS_V1,
        "Incorrect order.");
}

void UnitTestExactToLineExtended2::TestRightOrLeftOf()
{
    QueryType query;
    OrderType order = OrderType::UNKNOWN;
    Vector2<float> P, V0, V1;

    V0 = { 1.0f, 2.0f };
    V1 = { 2.0f, 2.0f };
    P = { 3.0f, 1.999999f };
    order = query(P, V0, V1);
    UTAssert(order == OrderType::P_RIGHT_OF_V0_V1,
        "Incorrect order.");

    P = { 0.9f, 2.000001f };
    order = query(P, V0, V1);
    UTAssert(order == OrderType::P_LEFT_OF_V0_V1,
        "Incorrect order.");
}

void UnitTestExactToLineExtended2::TestCollinear()
{
    QueryType query;
    OrderType order = OrderType::UNKNOWN;
    Vector2<float> P, V0, V1;

    V0 = { 0.0f, 0.0f };
    V1 = { 1.0f, 0.0f };
    P = { 2.0f, 0.0f };
    order = query(P, V0, V1);
    UTAssert(order == OrderType::COLLINEAR_RIGHT,
        "Incorrect order.");

    P = { -1.0f, 0.0f };
    order = query(P, V0, V1);
    UTAssert(order == OrderType::COLLINEAR_LEFT,
        "Incorrect order.");

    P = { 0.25f, 0.0f };
    order = query(P, V0, V1);
    UTAssert(order == OrderType::COLLINEAR_CONTAIN,
        "Incorrect order.");
}

#else

#include <GTL/Mathematics/Geometry/2D/ExactToLineExtended2.h>

namespace gtl
{
    template class ExactToLineExtended2<float>;
    template class ExactToLineExtended2<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ExactToLineExtended2)
