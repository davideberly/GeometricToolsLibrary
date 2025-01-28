#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/2D/DistPoint2Circle2.h>
#include <algorithm>
using namespace gtl;

namespace gtl
{
    class UnitTestDistPoint2Circle2
    {
    public:
        UnitTestDistPoint2Circle2();

    private:
        using PCQuery = DCPQuery<double, Vector2<double>, Circle2<double>>;
        using PCOutput = typename PCQuery::Output;

        void Test();
    };
}

UnitTestDistPoint2Circle2::UnitTestDistPoint2Circle2()
{
    UTInformation("Mathematics/Distance/2D/DistPoint2Circle2");

    Test();
}

void UnitTestDistPoint2Circle2::Test()
{
    double constexpr maxError = 1e-15;
    Circle2<double> circle{};
    circle.center = { 1.0, 1.0 };
    circle.radius = 2.0;

    std::size_t constexpr numSamples = 360;
    PCQuery query{};
    PCOutput output{};
    Vector2<double> point{}, unit{};
    double length{}, t{};

    length = 0.25;
    for (std::size_t i = 0; i < numSamples; ++i)
    {
        t = C_TWO_PI<double> * static_cast<double>(i) / static_cast<double>(numSamples);
        unit = { std::cos(t), std::sin(t) };
        point = circle.center + length * unit;
        output = query(point, circle);
        UTAssert(std::fabs(output.distance - 1.75) <= maxError, "Invalid distance.");
    }

    length = 3.0;
    for (std::size_t i = 0; i < numSamples; ++i)
    {
        t = C_TWO_PI<double> * static_cast<double>(i) / static_cast<double>(numSamples);
        unit = { std::cos(t), std::sin(t) };
        point = circle.center + length * unit;
        output = query(point, circle);
        UTAssert(std::fabs(output.distance - 1.0) <= maxError, "Invalid distance.");
    }

    point = circle.center;
    output = query(point, circle);
    UTAssert(std::fabs(output.distance - 2.0) <= maxError, "Invalid distance.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/2D/DistPoint2Circle2.h>

namespace gtl
{
    template class DCPQuery<float, Vector2<float>, Circle2<float>>;
    template class DCPQuery<double, Vector2<double>, Circle2<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Vector2<Rational>, Circle2<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPoint2Circle2)
