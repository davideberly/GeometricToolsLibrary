#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistPoint3Cylinder3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistPoint3Cylinder3
    {
    public:
        UnitTestDistPoint3Cylinder3();

    private:
        using PCQuery = DCPQuery<double, Vector3<double>, Cylinder3<double>>;
        using PCOutput = typename PCQuery::Output;

        void TestInfiniteCylinder();
        void TestFiniteCylinder();

        void Validate(PCOutput const& output,
            double sqrDistance,
            Vector3<double> const& closest0,
            Vector3<double> const& closest1);
    };
}

UnitTestDistPoint3Cylinder3::UnitTestDistPoint3Cylinder3()
{
    UTInformation("Mathematics/Distance/3D/DistPoint3Cylinder3");

    TestInfiniteCylinder();
    TestFiniteCylinder();
}

void UnitTestDistPoint3Cylinder3::Validate(PCOutput const& output,
    double sqrDistance,
    Vector3<double> const& closest0,
    Vector3<double> const& closest1)
{
    double const maxError = 1e-13;

    double error = std::fabs(output.sqrDistance - sqrDistance);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(sqrDistance));
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

void UnitTestDistPoint3Cylinder3::TestInfiniteCylinder()
{
    PCQuery query{};
    PCOutput output{};

    Cylinder3<double> cylinder{};
    cylinder.center = { 0.0, 0.0, 0.0 };
    cylinder.direction = { 0.0, 0.0, 1.0 };
    cylinder.radius = 1.0;

    // The query object considers the cylinder to be infinite.
    cylinder.height = -1.0;

    Vector3<double> point{};
    Quaternion<double> q{ 1.0f, 2.0f, 3.0f, 4.0f };
    Normalize(q);
    Matrix3x3<double> rotate{};
    RigidMotion<double>::Convert(q, rotate);
    Vector3<double> translate{ 0.123, -4.567, 0.089012 };

    // point inside the cylinder
    point = { 0.1, 0.2, 0.3 };
    output = query(point, cylinder);
    Validate(output, 0.0, point, point);

    // repeat the query after a rigid transformation
    cylinder.center = rotate * cylinder.center + translate;
    cylinder.direction = rotate * cylinder.direction;
    point = rotate * point + translate;
    output = query(point, cylinder);
    Validate(output, 0.0, point, point);

    // point outside the cylinder
    cylinder.center = { 0.0, 0.0, 0.0 };
    cylinder.direction = { 0.0, 0.0, 1.0 };
    point = { 3.0, 4.0, 1.0 };
    output = query(point, cylinder);
    Vector3<double> closest{ 0.6, 0.8, 1.0 };
    Validate(output, 16.0, point, closest);

    // repeat the query after a rigid transformation
    cylinder.center = rotate * cylinder.center + translate;
    cylinder.direction = rotate * cylinder.direction;
    point = rotate * point + translate;
    closest = rotate * Vector3<double>{ 0.6, 0.8, 1.0 } + translate;
    output = query(point, cylinder);
    Validate(output, 16.0, point, closest);
}

void UnitTestDistPoint3Cylinder3::TestFiniteCylinder()
{
    PCQuery query{};
    PCOutput output{};

    Cylinder3<double> cylinder;
    cylinder.center = { 0.0, 0.0, 0.0 };
    cylinder.direction = { 0.0, 0.0, 1.0 };
    cylinder.radius = 1.0;
    cylinder.height = 3.0;

    Vector3<double> point{};
    Quaternion<double> q{ 1.0f, 2.0f, 3.0f, 4.0f };
    Normalize(q);
    Matrix3x3<double> rotate{};
    RigidMotion<double>::Convert(q, rotate);
    Vector3<double> translate{ 0.123, -4.567, 0.089012 };

    // There are 6 Voronoi regions to test.
    // 1. inside the cylinder, between planes of disks
    //      closest = point
    // 2. outside the cylinder, between planes of the disks
    //      closest = point projected to cylinder wall
    // 3. inside the cylinder, above plane of top disk
    //      closest = point projected to top disk
    // 4. outside the cylinder, above plane of top disk
    //      closest = point projected to circular boundary of top disk
    // 5. inside the cylinder, below plane of top disk
    //      closest = point projected to bottom disk
    // 6. outside the cylinder, below plane of top disk
    //      closest = point projected to circular boundary of bottom disk

    // region 1
    point = { 0.1, 0.2, 0.3 };
    output = query(point, cylinder);
    Validate(output, 0.0, point, point);

    // repeat the query after a rigid transformation
    cylinder.center = rotate * cylinder.center + translate;
    cylinder.direction = rotate * cylinder.direction;
    point = rotate * point + translate;
    output = query(point, cylinder);
    Validate(output, 0.0, point, point);

    // region 2
    cylinder.center = { 0.0, 0.0, 0.0 };
    cylinder.direction = { 0.0, 0.0, 1.0 };
    point = { 3.0, 4.0, 1.0 };
    output = query(point, cylinder);
    Vector3<double> closest{ 0.6, 0.8, 1.0 };
    Validate(output, 16.0, point, closest);

    // repeat the query after a rigid transformation
    cylinder.center = rotate * cylinder.center + translate;
    cylinder.direction = rotate * cylinder.direction;
    point = rotate * point + translate;
    closest = rotate * closest + translate;
    output = query(point, cylinder);
    Validate(output, 16.0, point, closest);

    // region 3
    cylinder.center = { 0.0, 0.0, 0.0 };
    cylinder.direction = { 0.0, 0.0, 1.0 };
    point = { 0.1, 0.2, 4.0 };
    output = query(point, cylinder);
    closest = { point[0], point[1], 0.5 * cylinder.height };
    Validate(output, 6.25, point, closest);

    // repeat the query after a rigid transformation
    cylinder.center = rotate * cylinder.center + translate;
    cylinder.direction = rotate * cylinder.direction;
    point = rotate * point + translate;
    closest = rotate * closest + translate;
    output = query(point, cylinder);
    Validate(output, 6.25, point, closest);

    // region 4
    cylinder.center = { 0.0, 0.0, 0.0 };
    cylinder.direction = { 0.0, 0.0, 1.0 };
    point = { 3.0, 4.0, 4.0 };
    output = query(point, cylinder);
    closest = { 0.6, 0.8, 0.5 * cylinder.height };
    Validate(output, 22.25, point, closest);

    // repeat the query after a rigid transformation
    cylinder.center = rotate * cylinder.center + translate;
    cylinder.direction = rotate * cylinder.direction;
    point = rotate * point + translate;
    closest = rotate * closest + translate;
    output = query(point, cylinder);
    Validate(output, 22.25, point, closest);

    // region 5
    cylinder.center = { 0.0, 0.0, 0.0 };
    cylinder.direction = { 0.0, 0.0, 1.0 };
    point = { 0.1, 0.2, -4.0 };
    output = query(point, cylinder);
    closest = { point[0], point[1], -0.5 * cylinder.height };
    Validate(output, 6.25, point, closest);

    // repeat the query after a rigid transformation
    cylinder.center = rotate * cylinder.center + translate;
    cylinder.direction = rotate * cylinder.direction;
    point = rotate * point + translate;
    closest = rotate * closest + translate;
    output = query(point, cylinder);
    Validate(output, 6.25, point, closest);

    // region 6
    cylinder.center = { 0.0, 0.0, 0.0 };
    cylinder.direction = { 0.0, 0.0, 1.0 };
    point = { 3.0, 4.0, -4.0 };
    output = query(point, cylinder);
    closest = { 0.6, 0.8, -0.5 * cylinder.height };
    Validate(output, 22.25, point, closest);

    // repeat the query after a rigid transformation
    cylinder.center = rotate * cylinder.center + translate;
    cylinder.direction = rotate * cylinder.direction;
    point = rotate * point + translate;
    closest = rotate * closest + translate;
    output = query(point, cylinder);
    Validate(output, 22.25, point, closest);
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistPoint3Cylinder3.h>

namespace gtl
{
    template class DCPQuery<float, Vector3<float>, Cylinder3<float>>;
    template class DCPQuery<double, Vector3<double>, Cylinder3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Vector3<Rational>, Cylinder3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPoint3Cylinder3)
