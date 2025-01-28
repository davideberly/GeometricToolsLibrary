#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistRectangle3OrientedBox3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistRectangle3OrientedBox3
    {
    public:
        UnitTestDistRectangle3OrientedBox3();

    private:
        using RBQuery = DCPQuery<double, Rectangle3<double>, OrientedBox3<double>>;
        using RBOutput = RBQuery::Output;

        void Test();

        // The expected values are passed to this function.
        void Validate(RBOutput const& output,
            double sqrDistance,
            std::array<double, 2> const& cartesian,
            Vector<double, 3> const& closest0,
            Vector<double, 3> const& closest1);
    };
}

UnitTestDistRectangle3OrientedBox3::UnitTestDistRectangle3OrientedBox3()
{
    UTInformation("Mathematics/Distance/3D/DistRectangle3OrientedBox3");

    Test();
}

void UnitTestDistRectangle3OrientedBox3::Validate(RBOutput const& output,
    double sqrDistance,
    std::array<double, 2> const& cartesian,
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

void UnitTestDistRectangle3OrientedBox3::Test()
{
    RBQuery query{};
    RBOutput output{};
    Rectangle3<double> rectangle{};
    OrientedBox3<double> box{};
    Vector3<double> normal{};

    box.center = { 1.5, 1.0, 0.5 };
    box.axis[0] = { 1.0, 0.0, 0.0 };
    box.axis[1] = { 0.0, 1.0, 0.0 };
    box.axis[2] = { 0.0, 0.0, 1.0 };
    box.extent = { 1.5, 1.0, 0.5 };
    rectangle.center = { -1.0, -1.0, -1.0 };
    rectangle.extent = { 5.0, 7.0 };
    normal = { 4.0, 2.0, 1.5 };
    ComputeOrthonormalBasis(1, normal, rectangle.axis[0], rectangle.axis[1]);
    output = query(rectangle,  box);
    Validate(output, 2.5280898876404518,
        { 0.58520573598065284, -0.35978376691409109 },
        { -1.3483146067415737, -0.67415730337078683, -0.50561797752809001 },
        { 0.0, 0.0, 0.0 });

    Vector<double, 3> translate{ 0.1234, 5.6789, -1.9735 };
    Matrix3x3<double> rotate{};
    Quaternion<double> q(1.0, 2.0, 3.0, 4.0);
    Normalize(q);
    RigidMotion<double>::Convert(q, rotate);

    box.center = { 1.5, 1.0, 0.5 };
    box.axis[0] = { 1.0, 0.0, 0.0 };
    box.axis[1] = { 0.0, 1.0, 0.0 };
    box.axis[2] = { 0.0, 0.0, 1.0 };
    box.extent = { 1.5, 1.0, 0.5 };
    rectangle.center = { -1.0, -1.0, -1.0 };
    rectangle.extent = { 5.0, 7.0 };
    normal = { 4.0, 2.0, 1.5 };
    ComputeOrthonormalBasis(1, normal, rectangle.axis[0], rectangle.axis[1]);
    box.center = rotate * box.center + translate;
    box.axis[0] = rotate * box.axis[0];
    box.axis[1] = rotate * box.axis[1];
    box.axis[2] = rotate * box.axis[2];
    rectangle.center = rotate * rectangle.center + translate;
    rectangle.axis[0] = rotate * rectangle.axis[0];
    rectangle.axis[1] = rotate * rectangle.axis[1];
    output = query(rectangle, box);
    Validate(output, 2.5280898876404363,
        { 0.58520573598065362, -0.35978376691409131 },
        { 0.022276404494382285, 4.1283382022471953, -2.3105786516853923 },
        { 0.1234, 5.6789, -1.9735 });

    box.center = { 1.5, 1.0, 0.5 };
    box.axis[0] = { 1.0, 0.0, 0.0 };
    box.axis[1] = { 0.0, 1.0, 0.0 };
    box.axis[2] = { 0.0, 0.0, 1.0 };
    box.extent = { 1.5, 1.0, 0.5 };
    rectangle.center = { -10.0, -10.0, -10.0 };
    rectangle.extent = { 0.5, 0.7 };
    normal = { 13.0, 11.0, 10.5 };
    ComputeOrthonormalBasis(1, normal, rectangle.axis[0], rectangle.axis[1]);
    output = query(rectangle, box);
    Validate(output, 298.36873673838159,
        { 0.5, -0.62066297654797387 },
        { -10.579647213087945, -9.4815740162398612, -9.8254544810684017 },
        { 0.0, 0.0, 0.0 });

    box.center = { 1.5, 1.0, 0.5 };
    box.axis[0] = { 1.0, 0.0, 0.0 };
    box.axis[1] = { 0.0, 1.0, 0.0 };
    box.axis[2] = { 0.0, 0.0, 1.0 };
    box.extent = { 1.5, 1.0, 0.5 };
    rectangle.center = { -10.0, -10.0, -10.0 };
    rectangle.extent = { 0.5, 0.7 };
    normal = { 13.0, 11.0, 10.5 };
    ComputeOrthonormalBasis(1, normal, rectangle.axis[0], rectangle.axis[1]);
    box.center = rotate * box.center + translate;
    box.axis[0] = rotate * box.axis[0];
    box.axis[1] = rotate * box.axis[1];
    box.axis[2] = rotate * box.axis[2];
    rectangle.center = rotate * rectangle.center + translate;
    rectangle.axis[0] = rotate * rectangle.axis[0];
    rectangle.axis[1] = rotate * rectangle.axis[1];
    output = query(rectangle, box);
    Validate(output, 298.36873673838164,
        { 0.5, -0.62066297654797264 },
        { -2.1715035703686487, -8.6660226684378241, -11.318303260509531 },
        { 0.1234, 5.6789, -1.9735 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistRectangle3OrientedBox3.h>

namespace gtl
{
    template class DCPQuery<float, Rectangle3<float>, OrientedBox3<float>>;
    template class DCPQuery<double, Rectangle3<double>, OrientedBox3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Rectangle3<Rational>, OrientedBox3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistRectangle3OrientedBox3)
