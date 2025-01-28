#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistRectangle3Rectangle3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistRectangle3Rectangle3
    {
    public:
        UnitTestDistRectangle3Rectangle3();

    private:
        using RRQuery = DCPQuery<double, Rectangle<double, 3>, Rectangle<double, 3>>;
        using RROutput = RRQuery::Output;

        void Test();

        // The expected values are passed to this function.
        void Validate(RROutput const& output,
            double sqrDistance,
            Vector<double, 2> const& cartesian0,
            Vector<double, 2> const& cartesian1,
            Vector<double, 3> const& closest0,
            Vector<double, 3> const& closest1);
    };
}

UnitTestDistRectangle3Rectangle3::UnitTestDistRectangle3Rectangle3()
{
    UTInformation("Mathematics/Distance/3D/DistRectangle3Rectangle3");

    Test();
}

void UnitTestDistRectangle3Rectangle3::Validate(RROutput const& output,
    double sqrDistance,
    Vector<double, 2> const& cartesian0,
    Vector<double, 2> const& cartesian1,
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
    error = std::fabs(output.cartesian0[0] - cartesian0[0]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian0[1] - cartesian0[1]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian1[0] - cartesian1[0]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.cartesian1[1] - cartesian1[1]);
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

void UnitTestDistRectangle3Rectangle3::Test()
{
    RRQuery query{};
    RROutput output{};
    std::array<Rectangle<double, 3>, 2> rectangle{};

    // Coverage of second loop in DistRectangle3Rectangle3 where
    // output.closet[i] = stOutput.closest[1-i];
    rectangle[0].center = { 0.5, 0.5, 0.0 };
    rectangle[0].axis[0] = { 1.0, 0.0, 0.0 };
    rectangle[0].axis[1] = { 0.0, 1.0, 0.0 };
    rectangle[0].extent = { 0.5, 0.5 };
    Vector3<double> axis{ -1.0, 1.0, 0.0 };
    Normalize(axis);
    AxisAngle<double> aa(axis, 0.001);
    rectangle[1].center = { 0.0, 0.0, 0.01 };
    rectangle[1].axis[0] = { 1.0, 0.0, 0.0 };
    rectangle[1].axis[1] = { 0.0, 1.0, 0.0 };
    rectangle[1].extent = { 0.25, 0.125 };
    Matrix3x3<double> rotAA{};
    RigidMotion<double>::Convert(aa, rotAA);
    rectangle[1].axis[0] = rotAA * rectangle[1].axis[0];
    rectangle[1].axis[1] = rotAA * rectangle[1].axis[1];
    output = query(rectangle[0], rectangle[1]);
    Validate(output, 9.4767012501546831e-05,
        { -0.25000009374999221, -0.37500009374999221 },
        { 0.25, 0.125 },
        { 0.24999990625000779, 0.12499990625000779, 0.0 },
        { 0.24999990625000781, 0.12499990625000781, 0.0097348350012492167 });

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
    rectangle[0].center = { 3.0, 2.0, 1.0 };
    rectangle[0].axis[0] = { 1.0, 1.0, 1.0 };
    rectangle[0].axis[1] = { 1.0, -1.0, 0.0 };
    rectangle[0].extent = { 1.0, 3.0 };
    Normalize(rectangle[0].axis[0]);
    Normalize(rectangle[0].axis[1]);
    rectangle[1].extent = { 0.5, 0.25 };
    rectangle[1].center = { -0.25, -0.25, -1.0 };
    rectangle[1].axis[0] = { 1.0, 2.0, 3.0 };
    rectangle[1].axis[1] = { 0.0, -3.0, 2.0 };
    rectangle[1].extent = { 0.5, 0.25 };
    Normalize(rectangle[1].axis[0]);
    Normalize(rectangle[1].axis[1]);
    rectangle[1].extent = { 0.5, 0.25 };
    rectangle[0].center = rot1 * rectangle[0].center + trn1;
    rectangle[0].axis[0] = rot1 * rectangle[0].axis[0];
    rectangle[0].axis[1] = rot1 * rectangle[0].axis[1];
    rectangle[1].center = rot1 * rectangle[1].center + trn1;
    rectangle[1].axis[0] = rot1 * rectangle[1].axis[0];
    rectangle[1].axis[1] = rot1 * rectangle[1].axis[1];
    output = query(rectangle[0], rectangle[1]);
    Validate(output, 8.3269284469013751,
        { -1.0, -0.94868500079249252 },
        { 0.5, -0.25 },
        { -2.5839427111800126, -3.4263171591367008, 4.7569580686983937 },
        { -1.4893671432615503, -2.1506742626533328, 2.4114159194994342 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistRectangle3Rectangle3.h>

namespace gtl
{
    template class DCPQuery<float, Rectangle<float, 3>, Rectangle<float, 3>>;
    template class DCPQuery<double, Rectangle<double, 3>, Rectangle<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Rectangle<Rational, 3>, Rectangle<Rational, 3>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistRectangle3Rectangle3)
