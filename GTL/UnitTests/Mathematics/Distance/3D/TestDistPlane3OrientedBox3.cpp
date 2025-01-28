#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistPlane3OrientedBox3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistPlane3OrientedBox3
    {
    public:
        UnitTestDistPlane3OrientedBox3();

    private:
        using PBQuery = DCPQuery<double, Plane3<double>, OrientedBox3<double>>;
        using PBOutput = PBQuery::Output;

        void Test();
    };
}

UnitTestDistPlane3OrientedBox3::UnitTestDistPlane3OrientedBox3()
{
    UTInformation("Mathematics/Distance/3D/DistPlane3OrientedBox3");

    Test();
}

void UnitTestDistPlane3OrientedBox3::Test()
{
    PBQuery query{};
    PBOutput output{};
    Vector3<double> planeOrigin{}, planeNormal{};
    Plane3<double> plane{};
    OrientedBox3<double> box{};
    double const maxError = 1e-14;

    box.center = { 1.5, 1.0, 0.5 };
    box.axis[0] = { 1.0, 0.0, 0.0 };
    box.axis[1] = { 0.0, 1.0, 0.0 };
    box.axis[2] = { 0.0, 0.0, 1.0 };
    box.extent = { 1.5, 1.0, 0.5 };

    planeOrigin = { -1.0, -1.0, -1.0 };
    planeNormal = { 1.0, 2.0, 3.0 };
    Normalize(planeNormal);
    plane = Plane3<double>(planeNormal, planeOrigin);
    output = query(plane, box);
    UTAssert(std::fabs(output.distance - 1.6035674514745470) <= maxError,
        "Invalid distance.");

    Vector<double, 3> translate{ 0.1234, 5.6789, -1.9735 };
    Matrix3x3<double> rotate{};
    Quaternion<double> q(1.0, 2.0, 3.0, 4.0);
    Normalize(q);
    RigidMotion<double>::Convert(q, rotate);
    box.center = rotate * box.center + translate;
    box.axis[0] = rotate * box.axis[0];
    box.axis[1] = rotate * box.axis[1];
    box.axis[2] = rotate * box.axis[2];
    planeOrigin = rotate * planeOrigin + translate;
    planeNormal = rotate * planeNormal;
    plane = Plane3<double>(planeNormal, planeOrigin);
    output = query(plane, box);
    UTAssert(std::fabs(output.distance - 1.6035674514745490) <= maxError,
        "Invalid distance.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistPlane3OrientedBox3.h>

namespace gtl
{
    template class DCPQuery<float, Plane3<float>, OrientedBox<float, 3>>;
    template class DCPQuery<double, Plane3<double>, OrientedBox<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Plane3<Rational>, OrientedBox<Rational, 3>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPlane3OrientedBox3)
