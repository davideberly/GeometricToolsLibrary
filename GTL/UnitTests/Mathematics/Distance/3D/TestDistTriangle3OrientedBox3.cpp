#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistTriangle3OrientedBox3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistTriangle3OrientedBox3
    {
    public:
        UnitTestDistTriangle3OrientedBox3();

    private:
        using TBQuery = DCPQuery<double, Triangle3<double>, OrientedBox3<double>>;
        using TBOutput = TBQuery::Output;

        void Test();

        // The expected values are passed to this function.
        void Validate(TBOutput const& output,
            double sqrDistance,
            std::array<double, 3> const& barycentric,
            Vector<double, 3> const& closest0,
            Vector<double, 3> const& closest1);
    };
}

UnitTestDistTriangle3OrientedBox3::UnitTestDistTriangle3OrientedBox3()
{
    UTInformation("Mathematics/Distance/3D/DistTriangle3OrientedBox3");

    Test();
}

void UnitTestDistTriangle3OrientedBox3::Validate(TBOutput const& output,
    double sqrDistance,
    std::array<double, 3> const& barycentric,
    Vector<double, 3> const& closest0,
    Vector<double, 3> const& closest1)
{
    double constexpr maxError = 1e-10;

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
    error = Length(output.closest[0] - closest0);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = Length(output.closest[1] - closest1);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
}

void UnitTestDistTriangle3OrientedBox3::Test()
{
    TBQuery query{};
    TBOutput output{};
    Triangle3<double> triangle{};
    OrientedBox3<double> box{};

    box.center = { 1.5, 1.0, 0.5 };
    box.axis[0] = { 1.0, 0.0, 0.0 };
    box.axis[1] = { 0.0, 1.0, 0.0 };
    box.axis[2] = { 0.0, 0.0, 1.0 };
    box.extent = { 1.5, 1.0, 0.5 };

    Vector<double, 3> translate{ 0.1234, 5.6789, -1.9735 };
    Matrix3x3<double> rotate{};
    Quaternion<double> q(1.0, 2.0, 3.0, 4.0);
    Normalize(q);
    RigidMotion<double>::Convert(q, rotate);
    box.center = rotate * box.center + translate;
    for (std::size_t i = 0; i < 3; ++i)
    {
        box.axis[i] = rotate * box.axis[i];
    }

    // Closest plane point inside the triangle.
    triangle.v[0] = { -8.5, 1.0, 0.5 };
    triangle.v[1] = { 1.5, -9.0, 0.5 };
    triangle.v[2] = { 1.5, 1.0, -9.5 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        triangle.v[i] = rotate * triangle.v[i] + translate;
    }
    output = query(triangle, box);
    Validate(output, 16.333333333333321,
        { 0.38333333333333341, 0.33333333333333337, 0.28333333333333321 },
        rotate * Vector3<double>{ -2.3333333333333326, -2.3333333333333326, -2.3333333333333326 } + translate,
        rotate * Vector3<double>{ 0.0, 0.0, 0.0 } + translate);

    // Closest plane point outside the triangle.
    triangle.v[0] = { 91.5, -199.0, 100.5 };
    triangle.v[1] = { 101.5, -209.0, 100.5 };
    triangle.v[2] = { 101.5, -199.0, 90.5 };
    for (std::size_t i = 0; i < 3; ++i)
    {
        triangle.v[i] = rotate * triangle.v[i] + translate;
    }
    output = query(triangle, box);
    Validate(output, 57272.999999999978,
        { 0.45000000000000590, 0.0, 0.54999999999999405 },
        rotate * Vector3<double>{ 97.0, -199.0, 95.0 } + translate,
        rotate * Vector3<double>{ 3.0, 0.0, 1.0 } + translate);
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistTriangle3OrientedBox3.h>

namespace gtl
{
    template class DCPQuery<float, Triangle3<float>, OrientedBox3<float>>;
    template class DCPQuery<double, Triangle3<double>, OrientedBox3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Triangle3<Rational>, OrientedBox3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistTriangle3OrientedBox3)
