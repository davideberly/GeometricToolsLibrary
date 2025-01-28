#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrPlane3Circle3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrPlane3Circle3
    {
    public:
        UnitTestIntrPlane3Circle3();

    private:
        using PCTIQuery = TIQuery<double, Plane3<double>, Circle3<double>>;
        using PCTIOutput = PCTIQuery::Output;
        using PCFIQuery = FIQuery<double, Plane3<double>, Circle3<double>>;
        using PCFIOutput = PCFIQuery::Output;

        void Test();
    };
}

UnitTestIntrPlane3Circle3::UnitTestIntrPlane3Circle3()
{
    UTInformation("Mathematics/Intersection/3D/IntrPlane3Circle3");

    Test();
}

void UnitTestIntrPlane3Circle3::Test()
{
    PCFIQuery query{};
    PCFIOutput output{};
    Plane3<double> plane{};
    Circle3<double> circle{};
    double maxError = 1e-15;

    Vector3<double> translate{ 0.1234, 5.6789, -1.9735 };
    Quaternion<double> q{ 1.0, 2.0, 3.0, 4.0 };
    Normalize(q);
    Matrix<double, 3, 3> rotate{};
    RigidMotion<double>::Convert(q, rotate);

    plane.normal = { 0.0, 0.0, 1.0 };
    plane.origin = { 0.0, 0.0, 0.0 };
    plane.constant = 0.0;
    circle.center = { 1.0, 0.0, 0.0 };
    circle.normal = { 0.0, 1.0, 0.0 };
    circle.radius = 0.5;
    output = query(plane, circle);
    UTAssert(
        output.intersect == true &&
        output.numIntersections == 2 &&
        (output.point[0] == Vector3<double>{ 1.5, 0.0, 0.0 }) &&
        (output.point[1] == Vector3<double>{ 0.5, 0.0, 0.0 }) &&
        (output.circle.center == Vector3<double>{ 0.0, 0.0, 0.0 }) &&
        (output.circle.normal == Vector3<double>{ 0.0, 0.0, 0.0 }) &&
        output.circle.radius == 0.0,
        "Incorrect output."
    );

    plane.normal = { 0.0, 0.0, 1.0 };
    plane.origin = { 0.0, 0.0, 0.0 };
    plane.constant = 0.0;
    circle.center = { 1.0, 0.0, 0.5 };
    circle.normal = { 0.0, 1.0, 0.0 };
    circle.radius = 0.5;
    output = query(plane, circle);
    UTAssert(
        output.intersect == true &&
        output.numIntersections == 1 &&
        (output.point[0] == Vector3<double>{ 1.0, 0.0, 0.0 }) &&
        (output.point[1] == Vector3<double>{ 1.0, 0.0, 0.0 }) &&
        (output.circle.center == Vector3<double>{ 0.0, 0.0, 0.0 }) &&
        (output.circle.normal == Vector3<double>{ 0.0, 0.0, 0.0 }) &&
        output.circle.radius == 0.0,
        "Incorrect output."
    );

    plane.normal = { 0.0, 0.0, 1.0 };
    plane.origin = { 0.0, 0.0, 0.0 };
    plane.constant = 0.0;
    circle.center = { 1.0, 0.0, 1.0 };
    circle.normal = { 0.0, 1.0, 0.0 };
    circle.radius = 0.5;
    output = query(plane, circle);
    UTAssert(
        output.intersect == false &&
        output.numIntersections == 0 &&
        (output.point[0] == Vector3<double>{ 0.0, 0.0, 0.0 }) &&
        (output.point[1] == Vector3<double>{ 0.0, 0.0, 0.0 }) &&
        (output.circle.center == Vector3<double>{ 0.0, 0.0, 0.0 }) &&
        (output.circle.normal == Vector3<double>{ 0.0, 0.0, 0.0 }) &&
        output.circle.radius == 0.0,
        "Incorrect output."
    );

    plane.normal = { 0.0, 0.0, 1.0 };
    plane.origin = { 0.0, 0.0, 0.0 };
    plane.constant = 0.0;
    circle.center = { 1.0, 0.0, 0.0 };
    circle.normal = { 0.0, 0.0, 1.0 };
    circle.radius = 0.5;
    output = query(plane, circle);
    UTAssert(
        output.intersect == true &&
        output.numIntersections == std::numeric_limits<std::size_t>::max() &&
        (output.point[0] == Vector3<double>{ 0.0, 0.0, 0.0 }) &&
        (output.point[1] == Vector3<double>{ 0.0, 0.0, 0.0 }) &&
        output.circle == circle,
        "Incorrect output."
    );

    // Plane is Dot(N, X - C) = 0. Apply transform Y = R * X + T to get
    // 0 = Dot(N, R^T * (Y - T) - C) = Dot(R * N, Y - (R * C + T)).
    plane.normal = { 0.0, 0.0, 1.0 };
    plane.origin = { 0.0, 0.0, 0.0 };
    plane.constant = 0.0;
    circle.center = { 1.0, 0.0, 0.0 };
    circle.normal = { 0.0, 1.0, 0.0 };
    circle.radius = 0.5;
    plane.normal = rotate * plane.normal;
    plane.origin = rotate * plane.origin + translate;
    plane.constant = Dot(plane.normal, plane.origin);
    circle.center = rotate * circle.center + translate;
    circle.normal = rotate * circle.normal;
    output = query(plane, circle);
    Vector3<double> expected0 = rotate * Vector3<double>{ 1.5, 0.0, 0.0 } + translate;
    Vector3<double> expected1 = rotate * Vector3<double>{ 0.5, 0.0, 0.0 } + translate;
    UTAssert(
        output.intersect == true &&
        output.numIntersections == 2 &&
        Length(output.point[0] - expected0) <= maxError &&
        Length(output.point[1] - expected1) <= maxError &&
        (output.circle.center == Vector3<double>{ 0.0, 0.0, 0.0 }) &&
        (output.circle.normal == Vector3<double>{ 0.0, 0.0, 0.0 }) &&
        output.circle.radius == 0.0,
        "Incorrect output."
    );
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrPlane3Circle3.h>

namespace gtl
{
    template class TIQuery<float, Plane3<float>, Circle3<float>>;
    template class FIQuery<float, Plane3<float>, Circle3<float>>;

    template class TIQuery<double, Plane3<double>, Circle3<double>>;
    template class FIQuery<double, Plane3<double>, Circle3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Plane3<Rational>, Circle3<Rational>>;
    template class FIQuery<Rational, Plane3<Rational>, Circle3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrPlane3Circle3)
