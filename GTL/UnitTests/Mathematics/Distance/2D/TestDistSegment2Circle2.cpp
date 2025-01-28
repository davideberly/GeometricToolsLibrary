#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/2D/DistSegment2Circle2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistSegment2Circle2
    {
    public:
        UnitTestDistSegment2Circle2();

    private:
        using SCQuery = DCPQuery<double, Segment2<double>, Circle2<double>>;
        using SCOutput = SCQuery::Output;

        // The expected values are passed to this function.
        void Validate(SCOutput const& output, SCOutput const& expected);

        void Test();
    };
}

UnitTestDistSegment2Circle2::UnitTestDistSegment2Circle2()
{
    UTInformation("Mathematics/Distance/2D/DistSegment2Circle2");

    Test();
}

void UnitTestDistSegment2Circle2::Validate(SCOutput const& output, SCOutput const& expected)
{
    double const maxError = 1e-14;
    double error{};

    UTAssert(output.numClosestPairs == expected.numClosestPairs, "Invalid number of pairs.");
    error = std::fabs(output.sqrDistance - expected.sqrDistance);
    UTAssert(error <= maxError, "Invalid sqrDistance.");
    for (std::size_t i = 0; i < output.numClosestPairs; ++i)
    {
        error = std::fabs(output.parameter[i] - expected.parameter[i]);
        UTAssert(error <= maxError, "Invalid parameter[" + std::to_string(i) + "].");
        error = Length(output.closest[i][0] - expected.closest[i][0]);
        UTAssert(error <= maxError, "Invalid closest[" + std::to_string(i) + "][0].");
        error = Length(output.closest[i][1] - expected.closest[i][1]);
        UTAssert(error <= maxError, "Invalid closest[" + std::to_string(i) + "][1].");
    }
}

void UnitTestDistSegment2Circle2::Test()
{
    Segment2<double> segment{};
    Circle2<double> circle{};
    SCQuery query{};
    SCQuery::Output output{}, expected{};
    Vector2<double> trn = { 0.01234, 0.56789 };
    Vector2<double> origin{}, direction{};

    // 1 (minimum distance when gradient is uniquely zero)
    circle.center = { 0.0, 0.0 };
    circle.radius = 1.0;
    origin = { 4.0, 3.0 };
    direction = { -1.0, 1.0 };
    segment.p[0] = origin - 10.0 * direction;
    segment.p[1] = origin + 10.0 * direction;
    output = query(segment, circle);
    expected.distance = 3.9497474683058327;
    expected.sqrDistance = 15.600505063388335;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 0.525;
    expected.closest[0][0] = { 3.5, 3.5 };
    expected.closest[0][1] = { 0.707106781186547524, 0.707106781186547524 };
    Validate(output, expected);

    // 1' (minimum distance when gradient is uniquely zero)
    circle.center += trn;
    segment.p[0] += trn;
    segment.p[1] += trn;
    output = query(segment, circle);
    expected.distance = 3.9497474683058327;
    expected.sqrDistance = 15.600505063388336;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 0.525;
    expected.closest[0][0] = { 3.51234, 4.06789 };
    expected.closest[0][1] = { 0.71944678118654748, 1.2749967811865475 };
    Validate(output, expected);

    // 2 (line outside circle, segment.p[0] closest)
    circle.center = { 0.0, 0.0 };
    circle.radius = 1.0;
    origin = { 4.0, 3.0 };
    direction = { -1.0, 1.0 };
    segment.p[0] = origin + 10.0 * direction;
    segment.p[1] = origin + 20.0 * direction;
    output = query(segment, circle);
    expected.distance = 13.317821063276353;
    expected.sqrDistance = 177.36435787344729;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 0.0;
    expected.closest[0][0] = { -6.0, 13.0 };
    expected.closest[0][1] = { -0.41905817746174689, 0.90795938450045166 };
    Validate(output, expected);

    // 2' (line outside circle, segment.p[1] closest)
    circle.center = { 0.0, 0.0 };
    circle.radius = 1.0;
    origin = { 4.0, 3.0 };
    direction = { -1.0, 1.0 };
    segment.p[0] = origin - 20.0 * direction;
    segment.p[1] = origin - 10.0 * direction;
    output = query(segment, circle);
    expected.distance = 14.652475842498529;
    expected.sqrDistance = 214.69504831500296;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 1.0;
    expected.closest[0][0] = { 14.0, -7.0 };
    expected.closest[0][1] = { 0.89442719099991586, -0.44721359549995793 };
    Validate(output, expected);

    // 3 (line intersects circle 2 points, segment.p[1] closest)
    circle.center = { 0.0, 0.0 };
    circle.radius = 1.0;
    origin = { 0.25, 0.25 };
    direction = { 4.0, 3.0 };
    segment.p[0] = origin - 20.0 * direction;
    segment.p[1] = origin - 10.0 * direction;
    output = query(segment, circle);
    expected.distance = 48.650025176227253;
    expected.sqrDistance = 2366.8249496475455;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 1.0;
    expected.closest[0][0] = { -39.75, -29.75 };
    expected.closest[0][1] = { -0.80060382364181659, -0.59919405668790049 };
    Validate(output, expected);

    // 4 (line intersects circle 2 points, segment.p[0] closest)
    circle.center = { 0.0, 0.0 };
    circle.radius = 1.0;
    origin = { 0.25, 0.25 };
    direction = { 4.0, 3.0 };
    segment.p[0] = origin + 10.0 * direction;
    segment.p[1] = origin + 20.0 * direction;
    output = query(segment, circle);
    expected.distance = 49.350024826210365;
    expected.sqrDistance = 2435.4249503475794;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 0.0;
    expected.closest[0][0] = { 40.25, 30.25 };
    expected.closest[0][1] = { 0.79940377664019213, 0.60079414269231834 };
    Validate(output, expected);

    // 4' (direction opposite of 4, line intersects circle 2 points, segment.p[0] closest)
    circle.center = { 0.0, 0.0 };
    circle.radius = 1.0;
    origin = { 0.25, 0.25 };
    direction = { -4.0, -3.0 };
    segment.p[0] = origin + 10.0 * direction;
    segment.p[1] = origin + 20.0 * direction;
    output = query(segment, circle);
    expected.distance = 48.650025176227253;
    expected.sqrDistance = 2366.8249496475455;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 0.0;
    expected.closest[0][0] = { -39.75, -29.75 };
    expected.closest[0][1] = { -0.80060382364181659, -0.59919405668790049 };
    Validate(output, expected);

    // 5 (The segment overlaps the t1-point. Remove the t0-point.)
    circle.center = { 0.0, 0.0 };
    circle.radius = 1.0;
    origin = { 0.25, 0.25 };
    direction = { 4.0, 3.0 };
    segment.p[0] = origin + 0.0625 * direction;
    segment.p[1] = origin + 2.0 * direction;
    output = query(segment, circle);
    expected.distance = 0.0;
    expected.sqrDistance = 0.0;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 0.034709596673229308;
    expected.closest[0][0] = { 0.76899937421752718, 0.63924953066314538 };
    expected.closest[0][1] = { 0.76899937421752718, 0.63924953066314538 };
    Validate(output, expected);

    // 5' (The segment overlaps the t0-point. Remove the t1-point.)
    circle.center = { 0.0, 0.0 };
    circle.radius = 1.0;
    origin = { 0.25, 0.25 };
    direction = { 4.0, 3.0 };
    segment.p[0] = origin - 2.0 * direction;
    segment.p[1] = origin + 0.0625 * direction;
    output = query(segment, circle);
    expected.distance = 0.0;
    expected.sqrDistance = 0.0;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 0.83890916676151184;
    expected.closest[0][0] = { -0.82899937421752767, -0.55924953066314576 };
    expected.closest[0][1] = { -0.82899937421752767, -0.55924953066314576 };
    Validate(output, expected);

    // 6 (segment inside the circle, segment.p[0] closer)
    circle.center = { 0.0, 0.0 };
    circle.radius = 1.0;
    segment.p[0] = { -0.25, 0.5 };
    segment.p[1] = { +0.125, 0.5 };
    output = query(segment, circle);
    expected.distance = 0.44098300562505255;
    expected.sqrDistance = 0.19446601125010513;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 0.0;
    expected.closest[0][0] = { -0.25, 0.5 };
    expected.closest[0][1] = { -0.44721359549995793, 0.89442719099991586 };
    Validate(output, expected);

    // 6' (segment inside the circle, segment.p[1] closer)
    circle.center = { 0.0, 0.0 };
    circle.radius = 1.0;
    segment.p[0] = { -0.125, 0.5 };
    segment.p[1] = { +0.25, 0.5 };
    output = query(segment, circle);
    expected.distance = 0.44098300562505255;
    expected.sqrDistance = 0.19446601125010513;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 1.0;
    expected.closest[0][0] = { 0.25, 0.5 };
    expected.closest[0][1] = { 0.44721359549995793, 0.89442719099991586 };
    Validate(output, expected);

    // 6" (segment inside the circle, segment.p[0] and segment.p[1] equidistant)
    circle.center = { 0.0, 0.0 };
    circle.radius = 1.0;
    segment.p[0] = { -0.25, 0.5 };
    segment.p[1] = { +0.25, 0.5 };
    output = query(segment, circle);
    expected.distance = 0.44098300562505255;
    expected.sqrDistance = 0.19446601125010513;
    expected.numClosestPairs = 2;
    expected.parameter[0] = 0.0;
    expected.parameter[1] = 1.0;
    expected.closest[0][0] = { -0.25, 0.5 };
    expected.closest[0][1] = { -0.44721359549995793, 0.89442719099991586 };
    expected.closest[1][0] = { 0.25, 0.5 };
    expected.closest[1][1] = { 0.44721359549995793, 0.89442719099991586 };
    Validate(output, expected);

    // 7 (segment intersects the circle in 2 points)
    circle.center = { 0.0, 0.0 };
    circle.radius = 1.0;
    segment.p[0] = { -1.0, 0.5 };
    segment.p[1] = { +2.0, 0.5 };
    output = query(segment, circle);
    expected.distance = 0.0;
    expected.sqrDistance = 0.0;
    expected.numClosestPairs = 2;
    expected.parameter[0] = 0.044658198738520449;
    expected.parameter[1] = 0.62200846792814624;
    expected.closest[0][0] = { -0.86602540378443860, 0.5 };
    expected.closest[0][1] = { -0.86602540378443860, 0.5 };
    expected.closest[1][0] = { +0.86602540378443860, 0.5 };
    expected.closest[1][1] = { +0.86602540378443860, 0.5 };
    Validate(output, expected);
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/2D/DistSegment2Circle2.h>

namespace gtl
{
    template class DCPQuery<float, Segment<float, 2>, Circle2<float>>;
    template class DCPQuery<double, Segment<double, 2>, Circle2<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Segment<Rational, 2>, Circle2<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistSegment2Circle2)
