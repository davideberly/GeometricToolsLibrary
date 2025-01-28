#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/3D/ExactColinear3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestExactColinear3
    {
    public:
        UnitTestExactColinear3();

    private:
        using IRat = BSNumber<UIntegerFP32<2>>;

        void Test();
    };
}

UnitTestExactColinear3::UnitTestExactColinear3()
{
    UTInformation("Mathematics/Geometry/3D/ExactColinear3");

    Test();
}

void UnitTestExactColinear3::Test()
{
    ExactColinear3<float> query;
    Vector3<float> P0, P1, P2;
    Vector3<IRat> rP0, rP1, rP2;

    auto GetRPoints = [&rP0, &rP1, &rP2]()
    {
        return std::array<Vector3<IRat> const*, 3>{ &rP0, &rP1, &rP2 };
    };

    P0 = { 1.0f, 2.0f, 3.0f };
    P1 = { 2.0f, 4.0f, 6.0f };
    P2 = { 5.0f, 10.0f, 15.0f };
    bool colinear = query(P0, P1, P2);
    UTAssert(colinear, "The points are expected to be colinear.");

    colinear = query(P0, P1, P2, GetRPoints);
    UTAssert(colinear, "The points are expected to be colinear.");

    P0 = { 1.0f, 2.0f, 3.0f };
    P1 = { 2.0f, 4.0f, 6.0f };
    P2 = { 5.00001f, 10.0f, 15.0f };
    colinear = query(P0, P1, P2);
    UTAssert(!colinear, "The points are expected to be non-colinear.");
}

#else

#include <GTL/Mathematics/Geometry/3D/ExactColinear3.h>

namespace gtl
{
    template class ExactColinear3<float>;
    template class ExactColinear3<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ExactColinear3)
