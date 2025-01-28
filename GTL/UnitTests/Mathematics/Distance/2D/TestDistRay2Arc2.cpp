#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/2D/DistRay2Arc2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistRay2Arc2
    {
    public:
        UnitTestDistRay2Arc2();

    private:
        using RCQuery = DCPQuery<double, Ray2<double>, Circle2<double>>;
        using RCOutput = RCQuery::Output;

        // The expected values are passed to this function.
        void Validate(RCOutput const& output, RCOutput const& expected);

        void Test();
    };
}

UnitTestDistRay2Arc2::UnitTestDistRay2Arc2()
{
    UTInformation("Mathematics/Distance/2D/DistRay2Arc2");

    Test();
}

void UnitTestDistRay2Arc2::Validate(RCOutput const& output, RCOutput const& expected)
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

void UnitTestDistRay2Arc2::Test()
{
    Ray2<double> ray{};
    Circle2<double> circle{};
    RCQuery query{};
    RCQuery::Output output{}, expected{};
    Vector2<double> trn = { 0.01234, 0.56789 };

    circle.center = { 0.0, 0.0 };
    circle.radius = 1.0;
    ray.origin = { 4.0, 3.0 };
    ray.direction = { -1.0, 1.0 };
    output = query(ray, circle);
    expected.distance = 3.9497474683058327;
    expected.sqrDistance = 15.600505063388335;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 0.5;
    expected.closest[0][0] = { 3.5, 3.5 };
    expected.closest[0][1] = { 0.70710678118654746, 0.70710678118654746 };
    Validate(output, expected);

    circle.center += trn;
    ray.origin += trn;
    output = query(ray, circle);
    expected.distance = 3.9497474683058327;
    expected.sqrDistance = 15.600505063388336;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 0.5;
    expected.closest[0][0] = { 3.51234, 4.06789 };
    expected.closest[0][1] = { 0.71944678118654748, 1.2749967811865475 };
    Validate(output, expected);

    circle.center = { 0.0, 0.0 };
    circle.radius = 1.0;
    ray.origin = { 4.0, 3.0 };
    ray.direction = -ray.direction;
    output = query(ray, circle);
    expected.distance = 4.0;
    expected.sqrDistance = 16.0;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 0.0;
    expected.closest[0][0] = { 4.0, 3.0 };
    expected.closest[0][1] = { 0.8, 0.6 };
    Validate(output, expected);

    circle.center += trn;
    ray.origin += trn;
    output = query(ray, circle);
    expected.distance = 4.0;
    expected.sqrDistance = 16.0;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 0.0;
    expected.closest[0][0] = { 4.01234, 3.56789 };
    expected.closest[0][1] = { 0.81234, 1.16789 };
    Validate(output, expected);

    circle.center = { 0.0, 0.0 };
    circle.radius = 1.0;
    ray.direction = { 4.0, 3.0 };
    ray.origin = Vector2<double>{ 0.25, 0.25 } + 5.0 * ray.direction;
    output = query(ray, circle);
    expected.distance = 24.350049309616736;
    expected.sqrDistance = 592.92490138076653;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 0.0;
    expected.closest[0][0] = { 20.25, 15.25 };
    expected.closest[0][1] = { 0.79881501423028822, 0.60157673911169851 };
    Validate(output, expected);

    circle.center += trn;
    ray.origin += trn;
    output = query(ray, circle);
    expected.distance = 24.350049309616736;
    expected.sqrDistance = 592.92490138076653;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 0.0;
    expected.closest[0][0] = { 20.26234, 15.81789 };
    expected.closest[0][1] = { 0.81115501423028824, 1.1694667391116984 };
    Validate(output, expected);

    circle.center = { 0.0, 0.0 };
    circle.radius = 1.0;
    ray.direction = { 4.0, 3.0 };
    ray.origin = Vector2<double>{ 0.25, 0.25 } - 5.0 * ray.direction;
    output = query(ray, circle);
    expected.distance = 0.0;
    expected.sqrDistance = 0.0;
    expected.numClosestPairs = 2;
    expected.parameter[0] = 4.7302501564456181;
    expected.parameter[1] = 5.1297498435543822;
    expected.closest[0][0] = { -0.82899937421752767, -0.55924953066314487 };
    expected.closest[0][1] = { -0.82899937421752767, -0.55924953066314487 };
    expected.closest[1][0] = { 0.76899937421752895, 0.63924953066314671 };
    expected.closest[1][1] = { 0.76899937421752895, 0.63924953066314671 };
    Validate(output, expected);

    circle.center = { 0.0, 0.0 };
    circle.radius = 5.0;
    ray.direction = { 4.0, -3.0 };
    ray.origin = Vector2<double>{ 3.0, 4.0 } + 2.0 * ray.direction;
    output = query(ray, circle);
    expected.distance = 6.1803398874989490;
    expected.sqrDistance = 38.196601125010524;
    expected.numClosestPairs = 1;
    expected.parameter[0] = 0.0;
    expected.closest[0][0] = { 11.0, -2.0 };
    expected.closest[0][1] = { 4.9193495504995370, -0.89442719099991586 };
    Validate(output, expected);
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/2D/DistRay2Arc2.h>

namespace gtl
{
    template class DCPQuery<float, Ray<float, 2>, Circle2<float>>;
    template class DCPQuery<double, Ray<double, 2>, Circle2<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Ray<Rational, 2>, Circle2<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistRay2Arc2)
