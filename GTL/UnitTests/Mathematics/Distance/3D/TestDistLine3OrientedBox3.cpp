#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistLine3OrientedBox3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistLine3OrientedBox3
    {
    public:
        UnitTestDistLine3OrientedBox3();

    private:
        using LBQuery = DCPQuery<double, Line<double, 3>, OrientedBox<double, 3>>;
        using LBOutput = LBQuery::Output;

        void Test();
    };
}

UnitTestDistLine3OrientedBox3::UnitTestDistLine3OrientedBox3()
{
    UTInformation("Mathematics/Distance/3D/DistLine3OrientedBox3");

    Test();
}

void UnitTestDistLine3OrientedBox3::Test()
{
    LBQuery query{};
    LBOutput output{};
    Line3<double> line{};
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

    line.origin = { -1.0, -1.0, -1.0 };
    line.direction = { -0.13375998748853216, -0.49589068532333880, 0.85802138315814536 };

    output = query(line, box);
    UTAssert(std::fabs(output.distance - 5.1606938208625763) <= maxError, "Invalid distance");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistLine3OrientedBox3.h>

namespace gtl
{
    template class DCPQuery<float, Line<float, 3>, OrientedBox<float, 3>>;
    template class DCPQuery<double, Line<double, 3>, OrientedBox<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Line<Rational, 3>, OrientedBox<Rational, 3>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistLine3OrientedBox3)
