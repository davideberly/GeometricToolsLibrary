#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/2D/ExactColinear2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestExactColinear2
    {
    public:
        UnitTestExactColinear2();

    private:
        using IRat = BSNumber<UIntegerFP32<2>>;

        void Test();
    };
}

UnitTestExactColinear2::UnitTestExactColinear2()
{
    UTInformation("Mathematics/Geometry/2D/ExactColinear2");

    Test();
}

void UnitTestExactColinear2::Test()
{
    ExactColinear2<float> query;
    Vector2<float> P0, P1, P2;
    Vector2<IRat> rP0, rP1, rP2;

    auto GetRPoints = [&P0, &P1, &P2, &rP0, &rP1, &rP2]()
    {
        rP0 = { P0[0], P0[1] };
        rP1 = { P1[0], P1[1] };
        rP2 = { P2[0], P2[1] };
        return std::array<Vector2<IRat> const*, 3>{ &rP0, & rP1, &rP2 };
    };

    P0 = { 1.0f, 2.0f };
    P1 = { 2.0f, 4.0f };
    P2 = { 5.0f, 10.0f };
    bool colinear = query(P0, P1, P2);
    UTAssert(colinear, "The points are expected to be colinear.");

    colinear = query(P0, P1, P2, GetRPoints);
    UTAssert(colinear, "The points are expected to be colinear.");

    P0 = { 1.0f, 2.0f };
    P1 = { 2.0f, 4.0f };
    P2 = { 5.00001f, 10.0f };
    colinear = query(P0, P1, P2);
    UTAssert(!colinear, "The points are expected to be non-colinear.");
}

#else

#include <GTL/Mathematics/Geometry/2D/ExactColinear2.h>

namespace gtl
{
    template class ExactColinear2<float>;
    template class ExactColinear2<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ExactColinear2)
