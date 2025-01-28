#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/3D/IntrSegment3Capsule3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrSegment3Capsule3
    {
    public:
        UnitTestIntrSegment3Capsule3();

    private:
        void Test();
    };
}

UnitTestIntrSegment3Capsule3::UnitTestIntrSegment3Capsule3()
{
    UTInformation("Mathematics/Intersection/3D/IntrSegment3Capsule3");

    Test();
}

void UnitTestIntrSegment3Capsule3::Test()
{
    Capsule3<double> capsule{};
    capsule.segment.SetCenteredForm({ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 }, 1.0);
    capsule.radius = 1.0;

    Segment3<double> segment{};
    TIQuery<double, Segment3<double>, Capsule3<double>> tiQuery{};
    FIQuery<double, Segment3<double>, Capsule3<double>> fiQuery{};

    // Line intersects top hemisphere of capsule and capsule wall. Test
    // with various segments on the line.
    Vector3<double> lineOrigin = { 2.0, 0.5, 2.0 };
    Vector3<double> lineDirection = Vector3<double>{ 0.25, 0.5, 1.0 } - lineOrigin;
    Normalize(lineDirection);
    segment.p[0] = lineOrigin + 0.01 * lineDirection;
    segment.p[1] = lineOrigin + 10.0 * lineDirection;
    auto fiOutput = fiQuery(segment, capsule);
    double error0 = std::fabs(fiOutput.parameter[0] + 3.6294718457682951);
    double error1 = std::fabs(fiOutput.parameter[1] + 1.7040520687883471);
    double error2 = Length(fiOutput.point[0] - Vector3<double>{0.80570711328920686, 0.5, 1.3175469218795466});
    double error3 = Length(fiOutput.point[1] - Vector3<double>{-0.86602540378443882, 0.5, 0.36227119783746353});
    UTAssert(
        fiOutput.intersect == true &&
        fiOutput.numIntersections == 2 &&
        error0 <= 1e-15 &&
        error1 <= 1e-15 &&
        error2 <= 1e-15 &&
        error3 <= 1e-15,
        "Invalid FIQuery.");

    auto tiOutput = tiQuery(segment, capsule);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    // WHERE ARE THE OTHER SEGMENT TESTS?

    // Segment does not intersect capsule.
    segment.p[0] = lineOrigin;
    segment.p[1] = lineOrigin + 0.01 * lineDirection;
    fiOutput = fiQuery(segment, capsule);
    UTAssert(
        fiOutput.intersect == false,
        "Invalid FIQuery.");

    tiOutput = tiQuery(segment, capsule);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/3D/IntrSegment3Capsule3.h>

namespace gtl
{
    template class TIQuery<float, Segment3<float>, Capsule3<float>>;
    template class FIQuery<float, Segment3<float>, Capsule3<float>>;

    template class TIQuery<double, Segment3<double>, Capsule3<double>>;
    template class FIQuery<double, Segment3<double>, Capsule3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Segment3<Rational>, Capsule3<Rational>>;
    template class FIQuery<Rational, Segment3<Rational>, Capsule3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrSegment3Capsule3)
