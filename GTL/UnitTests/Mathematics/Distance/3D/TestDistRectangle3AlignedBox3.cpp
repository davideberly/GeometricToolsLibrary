#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistRectangle3AlignedBox3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistRectangle3AlignedBox3
    {
    public:
        UnitTestDistRectangle3AlignedBox3();

    private:
        using RBQuery = DCPQuery<double, Rectangle3<double>, AlignedBox3<double>>;
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

UnitTestDistRectangle3AlignedBox3::UnitTestDistRectangle3AlignedBox3()
{
    UTInformation("Mathematics/Distance/3D/DistRectangle3AlignedBox3");

    Test();
}

void UnitTestDistRectangle3AlignedBox3::Validate(RBOutput const& output,
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

void UnitTestDistRectangle3AlignedBox3::Test()
{
    RBQuery query{};
    RBOutput output{};
    Rectangle3<double> rectangle{};
    AlignedBox3<double> box{};
    Vector3<double> normal{};

    box.min = { 0.0, 0.0, 0.0 };
    box.max = { 3.0, 2.0, 1.0 };

    rectangle.center = { -1.0, -1.0, -1.0 };
    rectangle.extent = { 5.0, 7.0 };
    normal = { 4.0, 2.0, 1.5 };
    ComputeOrthonormalBasis(1, normal, rectangle.axis[0], rectangle.axis[1]);
    output = query(rectangle, box);
    Validate(output, 2.5280898876404501,
        { 0.58520573598065273, -0.35978376691409109 },
        { -1.3483146067415732, -0.67415730337078661, -0.50561797752809001 },
        { 0.0, 0.0, 0.0 });

    rectangle.center = { -10.0, -10.0, -10.0 };
    rectangle.extent = { 0.5, 0.7 };
    normal = { 13.0, 11.0, 10.5 };
    ComputeOrthonormalBasis(1, normal, rectangle.axis[0], rectangle.axis[1]);
    output = query(rectangle, box);
    Validate(output, 298.36873673838159,
        { 0.5, -0.62066297654797387 },
        { -10.579647213087945, -9.4815740162398612, -9.8254544810684017 },
        { 0.0, 0.0, 0.0 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistRectangle3AlignedBox3.h>

namespace gtl
{
    template class DCPQuery<float, Rectangle3<float>, AlignedBox3<float>>;
    template class DCPQuery<double, Rectangle3<double>, AlignedBox3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Rectangle3<Rational>, AlignedBox3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistRectangle3AlignedBox3)
