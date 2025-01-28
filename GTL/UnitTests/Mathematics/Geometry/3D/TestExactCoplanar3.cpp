#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/3D/ExactCoplanar3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestExactCoplanar3
    {
    public:
        UnitTestExactCoplanar3();

    private:
        using IRat = BSNumber<UIntegerFP32<2>>;

        void Test();
    };
}

UnitTestExactCoplanar3::UnitTestExactCoplanar3()
{
    UTInformation("Mathematics/Geometry/3D/ExactCoplanar3");

    Test();
}

void UnitTestExactCoplanar3::Test()
{
    ExactCoplanar3<float> query;
    Vector3<float> P0, P1, P2, P3;
    Vector3<IRat> rP0, rP1, rP2, rP3;

    auto GetRPoints = [&rP0, &rP1, &rP2, &rP3]()
    {
        return std::array<Vector3<IRat> const*, 4>{ &rP0, &rP1, &rP2, &rP3 };
    };

    P0 = { 1.0f, 2.0f, 3.0f };
    P1 = { 2.0f, 4.0f, 6.0f };
    P2 = { 5.0f, 10.0f, 15.0f };
    P3 = { -1.0f, 0.0f, 2.0f };
    bool coplanar = query(P0, P1, P2, P3);
    UTAssert(coplanar, "The points are expected to be coplanar.");

    coplanar = query(P0, P1, P2, P3, GetRPoints);
    UTAssert(coplanar, "The points are expected to be coplanar.");

    P0 = { 1.0f, 2.0f, 3.0f };
    P1 = { 2.0f, 4.0f, 6.0f };
    P2 = { 5.00001f, 10.0f, 15.0f };
    P3 = { -1.0f, 0.0f, 2.0f };
    coplanar = query(P0, P1, P2, P3);
    UTAssert(!coplanar, "The points are expected to be non-colinear.");

    P0 = { 1.0f, 2.0f, 3.0f };
    P1 = { 2.0f, 4.0f, 0.0f };
    P2 = { 5.0f, 10.0f, 15.0f };
    P3 = { -1.0f, 0.0f, 2.0f };
    coplanar = query(P0, P1, P2, P3);
    UTAssert(!coplanar, "The points are expected to be non-colinear.");
}

#else

#include <GTL/Mathematics/Geometry/3D/ExactCoplanar3.h>

namespace gtl
{
    template class ExactCoplanar3<float>;
    template class ExactCoplanar3<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ExactCoplanar3)
