#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/2D/DistLine2OrientedBox2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistLine2OrientedBox2
    {
    public:
        UnitTestDistLine2OrientedBox2();

    private:
        using LBQuery = DCPQuery<double, Line2<double>, OrientedBox2<double>>;
        using LBOutput = LBQuery::Output;

        void Test();

        // The expected values are passed to this function.
        void Validate(LBOutput const& output,
            double sqrDistance,
            double parameter,
            Vector2<double> const& closest0,
            Vector2<double> const& closest1);
    };
}

UnitTestDistLine2OrientedBox2::UnitTestDistLine2OrientedBox2()
{
    UTInformation("Mathematics/Distance/2D/DistLine2OrientedBox2");

    Test();
}

void UnitTestDistLine2OrientedBox2::Validate(LBOutput const& output,
    double sqrDistance,
    double parameter,
    Vector2<double> const& closest0,
    Vector2<double> const& closest1)
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
    error = std::fabs(output.parameter - parameter);
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

void UnitTestDistLine2OrientedBox2::Test()
{
    LBQuery query{};
    LBOutput output{};
    Line2<double> line{};
    OrientedBox2<double> box{};

    box.center = { 264.0, 232.0 };
    double const boxAngle = C_PI<double> / 6.0;
    box.axis[0] = { std::cos(boxAngle), std::sin(boxAngle) };
    box.axis[1] = { -std::sin(boxAngle), std::cos(boxAngle) };
    box.extent = { 64.0, 32.0 };

    line.origin = { 241.0, 323.0 };
    line.direction = { 0.96126169593831889, 0.27563735581699911 };
    output = query(line, box);
    Validate(output, 2235.5883708176138, 51.383445449708830,
        { 290.39293791614119, 337.16319703652476 },
        { 303.42562584220406, 291.71281292110206 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/2D/DistLine2OrientedBox2.h>

namespace gtl
{
    template class DCPQuery<float, Line<float, 2>, OrientedBox<float, 2>>;
    template class DCPQuery<double, Line<double, 2>, OrientedBox<double, 2>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Line<Rational, 2>, OrientedBox<Rational, 2>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistLine2OrientedBox2)
