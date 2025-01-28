#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistRay3OrientedBox3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistRay3OrientedBox3
    {
    public:
        UnitTestDistRay3OrientedBox3();

    private:
        using RBQuery = DCPQuery<double, Ray3<double>, OrientedBox3<double>>;
        using RBOutput = RBQuery::Output;

        void Test();
    };
}

UnitTestDistRay3OrientedBox3::UnitTestDistRay3OrientedBox3()
{
    UTInformation("Mathematics/Distance/3D/DistRay3OrientedBox3");

    Test();
}

void UnitTestDistRay3OrientedBox3::Test()
{
    RBQuery query{};
    RBOutput output{};
    Ray3<double> ray{};
    OrientedBox3<double> box{};
    double const maxError = 1e-14;

    box.center = { 2.0, 3.5, 5.0 };
    box.extent = { 1.0, 1.5, 2.0 };
    Quaternion<double> q(1.0, 2.0, 3.0, 4.0);
    Normalize(q);
    Matrix3x3<double> rotate{};
    RigidMotion<double>::Convert(q, rotate);
    box.axis[0] = rotate.GetCol(0);
    box.axis[1] = rotate.GetCol(1);
    box.axis[2] = rotate.GetCol(2);

    ray.origin = { -1.0, -1.0, -1.0 };
    ray.direction = { -0.13375998748853216, -0.49589068532333880, 0.85802138315814536 };
    output = query(ray, box);
    UTAssert(std::fabs(output.distance - 5.1606938208625763) <= maxError,
        "Invalid distance");
    UTAssert(output.parameter > 0.0, "Invalid parameter.");

    // Verified with Mathematica.
    ray.origin += 6.0 * ray.direction;
    output = query(ray, box);
    UTAssert(std::fabs(output.distance - 6.6315829285508334) <= maxError,
        "Invalid distance");
    UTAssert(output.parameter == 0.0, "Invalid parameter.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistRay3OrientedBox3.h>

namespace gtl
{
    template class DCPQuery<float, Ray3<float>, OrientedBox3<float>>;
    template class DCPQuery<double, Ray3<double>, OrientedBox3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Ray3<Rational>, OrientedBox3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistRay3OrientedBox3)
