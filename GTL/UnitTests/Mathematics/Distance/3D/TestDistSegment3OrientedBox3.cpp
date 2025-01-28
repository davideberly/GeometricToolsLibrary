#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistSegment3OrientedBox3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistSegment3OrientedBox3
    {
    public:
        UnitTestDistSegment3OrientedBox3();

    private:
        using SBQuery = DCPQuery<double, Segment3<double>, OrientedBox3<double>>;
        using SBOutput = SBQuery::Output;

        void Test();
    };
}

UnitTestDistSegment3OrientedBox3::UnitTestDistSegment3OrientedBox3()
{
    UTInformation("Mathematics/Distance/3D/DistSegment3OrientedBox3");

    Test();
}

void UnitTestDistSegment3OrientedBox3::Test()
{
    SBQuery query{};
    SBOutput output{};
    Segment3<double> segment{};
    OrientedBox3<double> box{};
    double const maxError = 1e-14;
    Vector3<double> direction{ -0.13375998748853216, -0.49589068532333880, 0.85802138315814536 };

    box.center = { 2.0, 3.5, 5.0 };
    box.extent = { 1.0, 1.5, 2.0 };
    Quaternion<double> q(1.0, 2.0, 3.0, 4.0);
    Normalize(q);
    Matrix3x3<double> rotate{};
    RigidMotion<double>::Convert(q, rotate);
    box.axis[0] = rotate.GetCol(0);
    box.axis[1] = rotate.GetCol(1);
    box.axis[2] = rotate.GetCol(2);

    segment.p[0] = {-1.0, -1.0, -1.0};
    segment.p[1] = segment.p[0] + 10.0 * direction;
    output = query(segment, box);
    UTAssert(std::fabs(output.distance - 5.1606938208625763) <= maxError,
        "Invalid distance");
    UTAssert(0.0 <= output.parameter && output.parameter <= 1.0, "Invalid parameter.");

    // Verified with Mathematica.
    segment.p[0] += 6.0 * direction;
    segment.p[1] += 6.0 * direction;
    output = query(segment, box);
    UTAssert(std::fabs(output.distance - 6.6315829285508334) <= maxError,
        "Invalid distance");
    UTAssert(output.parameter == 0.0, "Invalid parameter.");

    segment.p[0] -= 15.0 * direction;
    segment.p[1] -= 15.0 * direction;
    output = query(segment, box);
    UTAssert(std::fabs(output.distance - 5.2110329794092145) <= maxError,
        "Invalid distance");
    UTAssert(output.parameter == 1.0, "Invalid parameter.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistSegment3OrientedBox3.h>

namespace gtl
{
    template class DCPQuery<float, Segment<float, 3>, OrientedBox<float, 3>>;
    template class DCPQuery<double, Segment3<double>, OrientedBox3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Segment<Rational, 3>, OrientedBox<Rational, 3>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistSegment3OrientedBox3)
