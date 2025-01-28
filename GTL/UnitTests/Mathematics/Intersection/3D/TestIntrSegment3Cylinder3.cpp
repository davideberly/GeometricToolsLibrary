#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrSegment3Cylinder3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrSegment3Cylinder3
    {
    public:
        UnitTestIntrSegment3Cylinder3();

    private:
        void Test();
    };
}

UnitTestIntrSegment3Cylinder3::UnitTestIntrSegment3Cylinder3()
{
    UTInformation("Mathematics/Intersection/3D/IntrSegment3Cylinder3");

    Test();
}

void UnitTestIntrSegment3Cylinder3::Test()
{
    // x^2 + y^2 - r^2 = 1, |z| <= h/2 = 1
    Cylinder3<double> cylinder{};
    cylinder.center = { 0.0, 0.0, 0.0 };
    cylinder.direction = { 0.0, 0.0, 1.0 };
    cylinder.radius = 1.0;
    cylinder.height = 2.0;

    Segment3<double> segment{};
    FIQuery<double, Segment3<double>, Cylinder3<double>> fiQuery{};

    // Line intersects top disk of cylinder and cylinder wall. Test
    // with various segments on the line.
    Vector3<double> lineOrigin = { 2.0, 0.5, 2.0 };
    Vector3<double> lineDirection = Vector3<double>{ 0.25, 0.5, 1.0 } - lineOrigin;
    Normalize(lineDirection);
    segment.p[0] = lineOrigin + 0.01 * lineDirection;
    segment.p[1] = lineOrigin + 10.0 * lineDirection;
    auto fiOutput = fiQuery(segment, cylinder);
    double error0 = std::fabs(fiOutput.parameter[0] + 2.9894355629253626);
    double error1 = std::fabs(fiOutput.parameter[1] + 1.7040520687883471);
    double error2 = Length(fiOutput.point[0] - Vector3<double>{0.25, 0.5, 1.0});
    double error3 = Length(fiOutput.point[1] - Vector3<double>{-std::sqrt(0.75), 0.5, 0.36227119783746353});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    // WHERE ARE THE OTHER SEGMENT TESTS?

    // Segment intersects cylinder in a single point.
    segment.p[0] = lineOrigin;
    segment.p[1] = { 0.25, 0.5, 1.0 };
    fiOutput = fiQuery(segment, cylinder);
    error0 = std::fabs(fiOutput.parameter[0] - 1.0077822185373186);
    error1 = std::fabs(fiOutput.parameter[1] - fiOutput.parameter[0]);
    error2 = Length(fiOutput.point[0] - Vector3<double>{0.25, 0.5, 1.0});
    error3 = Length(fiOutput.point[1] - fiOutput.point[0]);
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 1 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    // Segment does not intersect cylinder.
    segment.p[0] = lineOrigin;
    segment.p[1] = lineOrigin + 0.01 * lineDirection;
    fiOutput = fiQuery(segment, cylinder);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrSegment3Cylinder3.h>

namespace gtl
{
    template class FIQuery<float, Segment3<float>, Cylinder3<float>>;
    template class FIQuery<double, Segment3<double>, Cylinder3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class FIQuery<Rational, Segment3<Rational>, Cylinder3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrSegment3Cylinder3)
