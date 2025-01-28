#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrPlane3Cylinder3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrPlane3Cylinder3
    {
    public:
        UnitTestIntrPlane3Cylinder3();

    private:
        using PCTIQuery = TIQuery<double, Plane3<double>, Cylinder3<double>>;
        using PCTIOutput = PCTIQuery::Output;
        using PCFIQuery = FIQuery<double, Plane3<double>, Cylinder3<double>>;
        using PCFIOutput = PCFIQuery::Output;

		void TestTIQueryInfinite();
		void TestTIQueryFinite();
		void TestFIQueryInfinite();
		void TestFIQueryFinite();
    };
}

UnitTestIntrPlane3Cylinder3::UnitTestIntrPlane3Cylinder3()
{
    UTInformation("Mathematics/Intersection/3D/IntrPlane3Cylinder3");

	TestTIQueryInfinite();
    TestTIQueryFinite();
    TestFIQueryInfinite();
    TestFIQueryFinite();
}

void UnitTestIntrPlane3Cylinder3::TestTIQueryInfinite()
{
	PCTIQuery query{};
	PCTIOutput output{};

    Plane3<double> plane({ 0.0, 0.0, 1.0 }, 0.0);
    Cylinder3<double> cylinder{};
    cylinder.MakeInfiniteCylinder();

	// The cylinder direction and plane are not parallel.
	cylinder.center = { 1.0, 2.0, 3.0 };
	cylinder.direction = { 3.0, 2.0, 1.0 };
	Normalize(cylinder.direction);
	cylinder.radius = 1.0;
    output = query(plane, cylinder);
    UTAssert(output.intersect == true, "Invalid result.");

    // The cylinder direction and plane are parallel.
    cylinder.direction = { 1.0, 0.0, 0.0 };
    cylinder.radius = 1.0;
    output = query(plane, cylinder);
    UTAssert(output.intersect == false, "Invalid result.");

    cylinder.radius = 3.5;
    output = query(plane, cylinder);
    UTAssert(output.intersect == true, "Invalid result.");
}

void UnitTestIntrPlane3Cylinder3::TestTIQueryFinite()
{
    PCTIQuery query{};
    PCTIOutput output{};

    Plane3<double> plane({ 0.0, 0.0, 1.0 }, 0.0);
    Cylinder3<double> cylinder{};

    cylinder.center = { 1.0, 2.0, 3.0 };
    cylinder.direction = { 3.0, 2.0, 1.0 };
    Normalize(cylinder.direction);
    cylinder.radius = 1.0;
    cylinder.height = 2.0;
    output = query(plane, cylinder);
    UTAssert(output.intersect == false, "Invalid result.");

    // In the following test, the cylinder just touches the plane at
    // the point (0.381966..., 0, 0).
    cylinder.center = { 1.0, 0.0, 1.0 };
    cylinder.direction = { 2.0, 0.0, 1.0 };
    cylinder.radius = (std::sqrt(5.0) - 1) / 2.0;
    Normalize(cylinder.direction);
    output = query(plane, cylinder);
    UTAssert(output.intersect == true, "Invalid result.");

    // Perturb the cylinder center slightly to ensure the just-touching
    // condition was correct.
    cylinder.center = { 1.0, 0.0, 1.001 };
    cylinder.direction = { 2.0, 0.0, 1.0 };
    cylinder.radius = (std::sqrt(5.0) - 1) / 2.0;
    Normalize(cylinder.direction);
    output = query(plane, cylinder);
    UTAssert(output.intersect == false, "Invalid result.");

    cylinder.center = { 1.0, 0.0, 0.999 };
    cylinder.direction = { 2.0, 0.0, 1.0 };
    cylinder.radius = (std::sqrt(5.0) - 1) / 2.0;
    Normalize(cylinder.direction);
    output = query(plane, cylinder);
    UTAssert(output.intersect == true, "Invalid result.");
}

void UnitTestIntrPlane3Cylinder3::TestFIQueryInfinite()
{
    PCFIQuery query{};
    PCFIOutput output{};

    Plane3<double> plane({ 0.0, 0.0, 1.0 }, 0.0);
    Cylinder3<double> cylinder{};
    cylinder.MakeInfiniteCylinder();

    // The cylinder direction is parallel to the plane.
    cylinder.center = { 1.0, 2.0, 3.0 };
    cylinder.direction = { 1.0, 0.0, 0.0 };
    cylinder.radius = 1.0;
    output = query(plane, cylinder);
    UTAssert(output.intersect == false, "Invalid result.");
    UTAssert(output.type == PCFIOutput::Type::NO_INTERSECTION, "Invalid result.");

    cylinder.center = { 1.0, 2.0, 1.0 };
    output = query(plane, cylinder);
    UTAssert(output.intersect == true, "Invalid result.");
    UTAssert(output.type == PCFIOutput::Type::SINGLE_LINE, "Invalid result.");
    UTAssert((output.line[0].origin == Vector3<double>{ 1.0, 2.0, 0.0 }), "Invalid result.");
    UTAssert((output.line[0].direction == Vector3<double>{ 1.0, 0.0, 0.0 }), "Invalid result.");

    cylinder.center = { 1.0, 2.0, 0.9 };
    output = query(plane, cylinder);
    double const maxError = 1e-14;
    std::array<Vector3<double>, 2> expectedOrigin
    { {
        { 1.0, 1.5641101056459328, 0.0 },
        { 1.0, 2.4358898943540672, 0.0 }
    } };
    UTAssert(output.intersect == true, "Invalid result.");
    UTAssert(output.type == PCFIOutput::Type::PARALLEL_LINES, "Invalid result.");
    UTAssert(Length(output.line[0].origin - expectedOrigin[0]) <= maxError, "Invalid result.");
    UTAssert((output.line[0].direction == Vector3<double>{ 1.0, 0.0, 0.0 }), "Invalid result.");
    UTAssert(Length(output.line[1].origin - expectedOrigin[1]) <= maxError, "Invalid result.");
    UTAssert((output.line[1].direction == Vector3<double>{ 1.0, 0.0, 0.0 }), "Invalid result.");

    // The cylinder direction is perpendicular to the plane. The
    // intersection should be a circle.
    cylinder.center = { 1.0, 2.0, 3.0 };
    cylinder.direction = plane.normal;
    cylinder.radius = 5.0;
    Ellipse3<double> expectedEllipse{};
    expectedEllipse.center = { 1.0, 2.0, 0.0 };
    expectedEllipse.normal = { 0.0, 0.0, 1.0 };
    expectedEllipse.axis[0] = { 0.0, 1.0, 0.0 };
    expectedEllipse.axis[1] = { -1.0, 0.0, 0.0 };
    expectedEllipse.extent = { 5.0, 5.0 };
    output = query(plane, cylinder);
    UTAssert(output.intersect == true, "Invalid result.");
    UTAssert(output.type == PCFIOutput::Type::CIRCLE, "Invalid result.");
    UTAssert(output.ellipse == expectedEllipse, "Invalid result.");

    // The cylinder direction is neither parallel nor perpendicular to the
    // plane. The intersection should be an ellipse (but not a circle).
    cylinder.center = { 1.0, 2.0, 3.0 };
    cylinder.direction = { 1.0, 1.0, 1.0 };
    Normalize(cylinder.direction);
    cylinder.radius = 5.0;
    // TODO: Use Mathematica to verify the expected ellipse.
    expectedEllipse.center = { -2.0, -1.0, 0.0 };
    expectedEllipse.normal = { 0.0, 0.0, 1.0 };
    expectedEllipse.axis[0] = { -0.70710678118654746, -0.70710678118654746, 0.0 };
    expectedEllipse.axis[1] = { 0.70710678118654746, -0.70710678118654746, 0.0 };
    expectedEllipse.extent = { 8.6602540378443909, 5.0 };
    output = query(plane, cylinder);
    UTAssert(output.intersect == true, "Invalid result.");
    UTAssert(output.type == PCFIOutput::Type::ELLIPSE, "Invalid result.");
    UTAssert(Length(output.ellipse.center - expectedEllipse.center) <= maxError, "Invalid result.");
    UTAssert(Length(output.ellipse.axis[0] - expectedEllipse.axis[0]) <= maxError, "Invalid result.");
    UTAssert(Length(output.ellipse.axis[1] - expectedEllipse.axis[1]) <= maxError, "Invalid result.");
    UTAssert(Length(output.ellipse.extent - expectedEllipse.extent) <= maxError, "Invalid result.");
}

void UnitTestIntrPlane3Cylinder3::TestFIQueryFinite()
{
    PCFIQuery query{};
    PCFIOutput output{};

    Plane3<double> plane({ 0.0, 0.0, 1.0 }, 0.0);
    Cylinder3<double> cylinder{};
    cylinder.height = 1.0;

    // The cylinder direction is parallel to the plane.
    cylinder.center = { 1.0, 2.0, 3.0 };
    cylinder.direction = { 1.0, 0.0, 0.0 };
    cylinder.radius = 1.0;
    output = query(plane, cylinder);
    UTAssert(output.intersect == false, "Invalid result.");
    UTAssert(output.type == PCFIOutput::Type::NO_INTERSECTION, "Invalid result.");

    cylinder.center = { 1.0, 2.0, 1.0 };
    output = query(plane, cylinder);
    UTAssert(output.intersect == true, "Invalid result.");
    UTAssert(output.type == PCFIOutput::Type::SINGLE_LINE, "Invalid result.");
    UTAssert((output.line[0].origin == Vector3<double>{ 1.0, 2.0, 0.0 }), "Invalid result.");
    UTAssert((output.line[0].direction == Vector3<double>{ 1.0, 0.0, 0.0 }), "Invalid result.");

    cylinder.center = { 1.0, 2.0, 0.9 };
    output = query(plane, cylinder);
    double const maxError = 1e-14;
    std::array<Vector3<double>, 2> expectedOrigin
    { {
        { 1.0, 1.5641101056459328, 0.0 },
        { 1.0, 2.4358898943540672, 0.0 }
    } };
    UTAssert(output.intersect == true, "Invalid result.");
    UTAssert(output.type == PCFIOutput::Type::PARALLEL_LINES, "Invalid result.");
    UTAssert(Length(output.line[0].origin - expectedOrigin[0]) <= maxError, "Invalid result.");
    UTAssert((output.line[0].direction == Vector3<double>{ 1.0, 0.0, 0.0 }), "Invalid result.");
    UTAssert(Length(output.line[1].origin - expectedOrigin[1]) <= maxError, "Invalid result.");
    UTAssert((output.line[1].direction == Vector3<double>{ 1.0, 0.0, 0.0 }), "Invalid result.");

    // The cylinder direction is perpendicular to the plane. The
    // intersection should be a circle.
    cylinder.center = { 1.0, 2.0, 3.0 };
    cylinder.direction = plane.normal;
    cylinder.radius = 5.0;
    cylinder.height = 8.0;
    Ellipse3<double> expectedEllipse{};
    expectedEllipse.center = { 1.0, 2.0, 0.0 };
    expectedEllipse.normal = { 0.0, 0.0, 1.0 };
    expectedEllipse.axis[0] = { 0.0, 1.0, 0.0 };
    expectedEllipse.axis[1] = { -1.0, 0.0, 0.0 };
    expectedEllipse.extent = { 5.0, 5.0 };
    output = query(plane, cylinder);
    UTAssert(output.intersect == true, "Invalid result.");
    UTAssert(output.type == PCFIOutput::Type::CIRCLE, "Invalid result.");
    UTAssert(output.ellipse == expectedEllipse, "Invalid result.");

    // The cylinder direction is neither parallel nor perpendicular to the
    // plane. The intersection should be an ellipse (but not a circle).
    cylinder.center = { 1.0, 2.0, 3.0 };
    cylinder.direction = { 1.0, 1.0, 1.0 };
    Normalize(cylinder.direction);
    cylinder.radius = 5.0;
    // TODO: Use Mathematica to verify the expected ellipse.
    expectedEllipse.center = { -2.0, -1.0, 0.0 };
    expectedEllipse.normal = { 0.0, 0.0, 1.0 };
    expectedEllipse.axis[0] = { -0.70710678118654746, -0.70710678118654746, 0.0 };
    expectedEllipse.axis[1] = { 0.70710678118654746, -0.70710678118654746, 0.0 };
    expectedEllipse.extent = { 8.6602540378443909, 5.0 };
    std::array<Line3<double>, 2> expectedTrimLine{};
    expectedTrimLine[0].origin = { -0.46410161513775533, -0.46410161513775533, 0.0 };
    expectedTrimLine[0].direction = { -0.70710678118654757, 0.70710678118654757, 0.0 };
    expectedTrimLine[1].origin = { 6.4641016151377579, 6.4641016151377579, 0.0 };
    expectedTrimLine[1].direction = { -0.70710678118654757, 0.70710678118654757, 0.0 };
    output = query(plane, cylinder);
    UTAssert(output.intersect == true, "Invalid result.");
    UTAssert(output.type == PCFIOutput::Type::ELLIPSE, "Invalid result.");
    UTAssert(Length(output.ellipse.center - expectedEllipse.center) <= maxError, "Invalid result.");
    UTAssert(Length(output.ellipse.axis[0] - expectedEllipse.axis[0]) <= maxError, "Invalid result.");
    UTAssert(Length(output.ellipse.axis[1] - expectedEllipse.axis[1]) <= maxError, "Invalid result.");
    UTAssert(Length(output.ellipse.extent - expectedEllipse.extent) <= maxError, "Invalid result.");
    UTAssert(Length(output.trimLine[0].origin - expectedTrimLine[0].origin) <= maxError, "Invalid result.");
    UTAssert(Length(output.trimLine[0].direction - expectedTrimLine[0].direction) <= maxError, "Invalid result.");
    UTAssert(Length(output.trimLine[1].origin - expectedTrimLine[1].origin) <= maxError, "Invalid result.");
    UTAssert(Length(output.trimLine[1].direction - expectedTrimLine[1].direction) <= maxError, "Invalid result.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrPlane3Cylinder3.h>

namespace gtl
{
    template class TIQuery<float, Plane3<float>, Cylinder3<float>>;
    template class FIQuery<float, Plane3<float>, Cylinder3<float>>;

    template class TIQuery<double, Plane3<double>, Cylinder3<double>>;
    template class FIQuery<double, Plane3<double>, Cylinder3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Plane3<Rational>, Cylinder3<Rational>>;
    template class FIQuery<Rational, Plane3<Rational>, Cylinder3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrPlane3Cylinder3)
