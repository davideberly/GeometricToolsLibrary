#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistPlane3AlignedBox3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistPlane3AlignedBox3
    {
    public:
        UnitTestDistPlane3AlignedBox3();

    private:
        using PBQuery = DCPQuery<double, Plane3<double>, AlignedBox3<double>>;
        using PBOutput = PBQuery::Output;

        void Test();
    };
}

UnitTestDistPlane3AlignedBox3::UnitTestDistPlane3AlignedBox3()
{
    UTInformation("Mathematics/Distance/3D/DistPlane3AlignedBox3");

    Test();
}

void UnitTestDistPlane3AlignedBox3::Test()
{
    PBQuery query{};
    PBOutput output{};
    Vector3<double> planeOrigin{}, planeNormal{};
    Plane3<double> plane{};
    AlignedBox3<double> box{};
    double const maxError = 1e-14;

    box.min = { 0.0, 0.0, 0.0 };
    box.max = { 3.0, 2.0, 1.0 };

    planeOrigin = { -1.0, -1.0, -1.0 };
    planeNormal = { 0.84799830400508802, 0.42399915200254401, 0.31799936400190804 };
    plane = Plane3<double>(planeNormal, planeOrigin);
    output = query(plane, box);
    UTAssert(std::fabs(output.sqrDistance - 2.5280898876404501) <= maxError,
        "Invalid sqrDistance.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistPlane3AlignedBox3.h>

namespace gtl
{
    template class DCPQuery<float, Plane3<float>, AlignedBox<float, 3>>;
    template class DCPQuery<double, Plane3<double>, AlignedBox<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Plane3<Rational>, AlignedBox<Rational, 3>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPlane3AlignedBox3)
