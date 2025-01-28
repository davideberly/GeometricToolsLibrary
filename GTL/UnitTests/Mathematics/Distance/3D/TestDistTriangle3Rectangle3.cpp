#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistTriangle3Rectangle3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistTriangle3Rectangle3
    {
    public:
        UnitTestDistTriangle3Rectangle3();

    private:
        using TRQuery = DCPQuery<double, Triangle<double, 3>, Rectangle<double, 3>>;
        using TROutput = TRQuery::Output;

        void Test();

        // The expected values are passed to this function.
        void Validate(TROutput const& output,
            double sqrDistance,
            Vector<double, 3> const& barycentric,
            Vector<double, 2> const& cartesian,
            Vector<double, 3> const& closest0,
            Vector<double, 3> const& closest1);
    };
}

UnitTestDistTriangle3Rectangle3::UnitTestDistTriangle3Rectangle3()
{
    UTInformation("Mathematics/Distance/3D/DistTriangle3Rectangle3");

    Test();
}

void UnitTestDistTriangle3Rectangle3::Validate(TROutput const& output,
    double sqrDistance,
    Vector<double, 3> const& barycentric,
    Vector<double, 2> const& cartesian,
    Vector<double, 3> const& closest0,
    Vector<double, 3> const& closest1)
{
    double constexpr maxError = 1e-14;

    double error = std::fabs(output.sqrDistance - sqrDistance);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(sqrDistance));
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.barycentric[0] - barycentric[0]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.barycentric[1] - barycentric[1]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.barycentric[2] - barycentric[2]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[0] - cartesian[0]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian[1] - cartesian[1]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - closest0);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = Length(output.closest[1] - closest1);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
}

void UnitTestDistTriangle3Rectangle3::Test()
{
    TRQuery query{};
    TROutput output{};
    Triangle<double, 3> triangle{};
    Rectangle<double, 3> rectangle{};

    // Coverage of second loop in DistTriangle3Rectangle3 where
    // output.closet[i] = stOutput.closest[1-i];
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    Vector3<double> axis{ -1.0, 1.0, 0.0 };
    Normalize(axis);
    AxisAngle<double> aa(axis, 0.001);
    rectangle.center = { 0.0, 0.0, 0.01 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 0.25, 0.125 };
    Matrix3x3<double> rotAA{};
    RigidMotion<double>::Convert(aa, rotAA);
    rectangle.axis[0] = rotAA * rectangle.axis[0];
    rectangle.axis[1] = rotAA * rectangle.axis[1];
    output = query(triangle, rectangle);
    Validate(output, 9.4767012501546831e-05,
        { 0.62500018749998443, 0.24999990625000781, 0.12499990625000781 },
        { 0.25, 0.125 },
        { 0.24999990625000781, 0.12499990625000781, 0.0 },
        { 0.24999990625000781, 0.12499990625000781, 0.0097348350012492167 });

    Vector<double, 3> translate{ 0.1234, 5.6789, -1.9735 };
    Matrix3x3<double> rotate{};
    Quaternion<double> q(1.0, 2.0, 3.0, 4.0);
    Normalize(q);
    RigidMotion<double>::Convert(q, rotate);

    // coincident and overlapping
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    rectangle.center = { 1.0, 0.0625, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 0.5, 0.25 };
    output = query(triangle, rectangle);
    Validate(output, 0.0,
        { 0.5, 0.5, 0.0 },
        { -0.5, -0.0625 },
        { 0.5, 0.0, 0.0 },
        { 0.5, 0.0, 0.0 });

    // coincident and overlapping
    // arbitrary rigid motion first
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    rectangle.center = { 1.0, 0.0625, 0.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 0.5, 0.25 };
    triangle.v[0] = rotate * triangle.v[0] + translate;
    triangle.v[1] = rotate * triangle.v[1] + translate;
    triangle.v[2] = rotate * triangle.v[2] + translate;
    rectangle.center = rotate * rectangle.center + translate;
    rectangle.axis[0] = rotate * rectangle.axis[0];
    rectangle.axis[1] = rotate * rectangle.axis[1];
    output = query(triangle, rectangle);
    Validate(output, 0.0,
        { 0.1875, 0.5, 0.3125 },
        { -0.5, 0.25 },
        { -0.018266666666666667, 6.2497333333333333, -1.9318333333333333 },
        { -0.018266666666666667, 6.2497333333333333, -1.9318333333333333 });

    // triangles are parallel but not coincident, projections overlap
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    rectangle.center = { 1.0, 0.0625, 1.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 0.5, 0.25 };
    output = query(triangle, rectangle);
    Validate(output, 1.0,
        { 0.5, 0.5, 0.0 },
        { -0.5, -0.0625 },
        { 0.5, 0.0, 0.0 },
        { 0.5, 0.0, 1.0 });

    // triangles are parallel but not coincident, projections overlap
    // arbitrary rigid motion first
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    rectangle.center = { 1.0, 0.0625, 1.0 };
    rectangle.axis[0] = { 1.0, 0.0, 0.0 };
    rectangle.axis[1] = { 0.0, 1.0, 0.0 };
    rectangle.extent = { 0.5, 0.25 };
    triangle.v[0] = rotate * triangle.v[0] + translate;
    triangle.v[1] = rotate * triangle.v[1] + translate;
    triangle.v[2] = rotate * triangle.v[2] + translate;
    rectangle.center = rotate * rectangle.center + translate;
    rectangle.axis[0] = rotate * rectangle.axis[0];
    rectangle.axis[1] = rotate * rectangle.axis[1];
    rectangle.extent = { 0.5, 0.25 };
    output = query(triangle, rectangle);
    Validate(output, 1.0,
        { 0.5, 0.5, 0.0 },
        { -0.5, -0.0625 },
        { 0.19006666666666672, 6.1455666666666664, -2.1401666666666666 },
        { 0.92339999999999989, 6.2788999999999993, -1.4735000000000000 });

    Vector<double, 3> trn0{ 0.1234, 5.6789, -1.9735 };
    Matrix3x3<double> rot0{};
    Quaternion<double> q0(1.0, 2.0, 3.0, 4.0);
    Normalize(q0);
    RigidMotion<double>::Convert(q0, rot0);
    Vector<double, 3> trn1{ -1.0, -2.0, 3.0 };
    Matrix3x3<double> rot1{};
    Quaternion<double> q1(2.0, 0.0, 5.0, 1.0);
    Normalize(q1);
    RigidMotion<double>::Convert(q1, rot1);

    // random objects
    triangle.v[0] = { 0.0, 0.0, 0.0 };
    triangle.v[1] = { 1.0, 0.0, 0.0 };
    triangle.v[2] = { 0.0, 1.0, 0.0 };
    rectangle.center = { 0.25, 0.25, 1.0 };
    rectangle.axis[0] = { 1.0, 2.0, 3.0 };
    rectangle.axis[1] = { 0.0, -3.0, 2.0 };
    Normalize(rectangle.axis[0]);
    Normalize(rectangle.axis[1]);
    rectangle.extent = { 0.5, 0.25 };
    triangle.v[0] = rot0 * triangle.v[0] + trn0;
    triangle.v[1] = rot0 * triangle.v[1] + trn0;
    triangle.v[2] = rot0 * triangle.v[2] + trn0;
    rectangle.center = rot1 * rectangle.center + trn1;
    rectangle.axis[0] = rot1 * rectangle.axis[0];
    rectangle.axis[1] = rot1 * rectangle.axis[1];
    output = query(triangle, rectangle);
    Validate(output, 85.770287643009596,
        { 0.0, 0.0, 1.0 },
        { -0.5, 0.25 },
        { -0.54326666666666668, 6.0122333333333327, -1.3068333333333335 },
        { -0.51063285673844960, -1.8493257373466672, 3.5885840805005658 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistTriangle3Rectangle3.h>

namespace gtl
{
    template class DCPQuery<float, Triangle<float, 3>, Rectangle<float, 3>>;
    template class DCPQuery<double, Triangle<double, 3>, Rectangle<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Triangle<Rational, 3>, Rectangle<Rational, 3>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistTriangle3Rectangle3)
