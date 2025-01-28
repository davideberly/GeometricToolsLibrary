#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/2D/DistPoint2Arc2.h>
#include <algorithm>
using namespace gtl;

namespace gtl
{
    class UnitTestDistPoint2Arc2
    {
    public:
        UnitTestDistPoint2Arc2();

    private:
        using PAQuery = DCPQuery<double, Vector2<double>, Arc2<double>>;
        using PAOutput = typename PAQuery::Output;

        void TestPoint2Arc2Acute();
        void TestPoint2Arc2Obtuse();
        void TestPoint2Arc2Semicircle();
    };
}

UnitTestDistPoint2Arc2::UnitTestDistPoint2Arc2()
{
    UTInformation("Mathematics/Distance/2D/DistPoint2Arc2");

    TestPoint2Arc2Acute();
    TestPoint2Arc2Obtuse();
    TestPoint2Arc2Semicircle();
}

void UnitTestDistPoint2Arc2::TestPoint2Arc2Acute()
{
    double constexpr maxError = 1e-15;
    Arc2<double> arc{};
    Vector2<double> unit{};
    arc.center = { 1.0, 1.0 };
    arc.radius = 2.0;
    unit = { 1.0, 0.0 };
    arc.end[0] = arc.center + arc.radius * unit;
    unit = { std::cos(C_PI_DIV_4<double>), std::sin(C_PI_DIV_4<double>) };
    arc.end[1] = arc.center + arc.radius * unit;

    std::size_t constexpr numSamples = 360;
    PAQuery query{};
    PAOutput output{};
    Vector2<double> point{};
    double length{}, t{};

    length = 0.25;
    for (std::size_t i = 0; i < numSamples; ++i)
    {
        t = C_TWO_PI<double> * static_cast<double>(i) / static_cast<double>(numSamples);
        unit = { std::cos(t), std::sin(t) };
        point = arc.center + length * unit;
        output = query(point, arc);

        if (i < 45)
        {
            UTAssert(std::fabs(output.distance - 1.75) <= maxError, "Invalid distance.");
        }
        else if (i <= 202)
        {
            Vector2<double> diff = output.closest[1] - Vector2<double>{ 1.0 + std::sqrt(2.0), 1.0 + std::sqrt(2.0) };
            UTAssert(Length(diff) <= maxError, "Invalid closest point.");
        }
        else
        {
            Vector2<double> diff = output.closest[1] - Vector2<double>{ 3.0, 1.0 };
            UTAssert(Length(diff) <= maxError, "Invalid closest point.");
        }
    }

    length = 3.0;
    for (std::size_t i = 0; i < numSamples; ++i)
    {
        t = C_TWO_PI<double> * static_cast<double>(i) / static_cast<double>(numSamples);
        unit = { std::cos(t), std::sin(t) };
        point = arc.center + length * unit;
        output = query(point, arc);

        if (i < 45)
        {
            UTAssert(std::fabs(output.distance - 1.0) <= maxError, "Invalid distance.");
        }
        else if (i <= 202)
        {
            Vector2<double> diff = output.closest[1] - Vector2<double>{ 1.0 + std::sqrt(2.0), 1.0 + std::sqrt(2.0) };
            UTAssert(Length(diff) <= maxError, "Invalid closest point.");
        }
        else
        {
            Vector2<double> diff = output.closest[1] - Vector2<double>{ 3.0, 1.0 };
            UTAssert(Length(diff) <= maxError, "Invalid closest point.");
        }
    }

    point = arc.center;
    output = query(point, arc);
    Vector2<double> diff = output.closest[1] - Vector2<double>{ 3.0, 1.0 };
    UTAssert(std::fabs(output.distance - 2.0) <= maxError && Length(diff) <= maxError, "Invalid output.");
}

void UnitTestDistPoint2Arc2::TestPoint2Arc2Obtuse()
{
    double constexpr maxError = 1e-15;
    Arc2<double> arc{};
    Vector2<double> unit{};
    arc.center = { 1.0, 1.0 };
    arc.radius = 2.0;
    unit = { 1.0, 0.0 };
    arc.end[0] = arc.center + arc.radius * unit;
    unit = { std::cos(5.0 * C_PI_DIV_4<double>), std::sin(5.0 * C_PI_DIV_4<double>) };
    arc.end[1] = arc.center + arc.radius * unit;

    std::size_t constexpr numSamples = 360;
    PAQuery query{};
    PAOutput output{};
    Vector2<double> point{};
    double length{}, t{};

    length = 0.25;
    for (std::size_t i = 0; i < numSamples; ++i)
    {
        t = C_TWO_PI<double> * static_cast<double>(i) / static_cast<double>(numSamples);
        unit = { std::cos(t), std::sin(t) };
        point = arc.center + length * unit;
        output = query(point, arc);

        if (i < 226)
        {
            UTAssert(std::fabs(output.distance - 1.75) <= maxError, "Invalid distance.");
        }
        else if (i <= 292)
        {
            Vector2<double> diff = output.closest[1] - Vector2<double>{ 1.0 - std::sqrt(2.0), 1.0 - std::sqrt(2.0) };
            UTAssert(Length(diff) <= maxError, "Invalid closest point.");
        }
        else
        {
            Vector2<double> diff = output.closest[1] - Vector2<double>{ 3.0, 1.0 };
            UTAssert(Length(diff) <= maxError, "Invalid closest point.");
        }
    }

    length = 3.0;
    for (std::size_t i = 0; i < numSamples; ++i)
    {
        t = C_TWO_PI<double> * static_cast<double>(i) / static_cast<double>(numSamples);
        unit = { std::cos(t), std::sin(t) };
        point = arc.center + length * unit;
        output = query(point, arc);

        if (i < 226)
        {
            UTAssert(std::fabs(output.distance - 1.0) <= maxError, "Invalid distance.");
        }
        else if (i <= 292)
        {
            Vector2<double> diff = output.closest[1] - Vector2<double>{ 1.0 - std::sqrt(2.0), 1.0 - std::sqrt(2.0) };
            UTAssert(Length(diff) <= maxError, "Invalid closest point.");
        }
        else
        {
            Vector2<double> diff = output.closest[1] - Vector2<double>{ 3.0, 1.0 };
            UTAssert(Length(diff) <= maxError, "Invalid closest point.");
        }
    }

    point = arc.center;
    output = query(point, arc);
    Vector2<double> diff = output.closest[1] - Vector2<double>{ 3.0, 1.0 };
    UTAssert(std::fabs(output.distance - 2.0) <= maxError && Length(diff) <= maxError, "Invalid output.");
}

void UnitTestDistPoint2Arc2::TestPoint2Arc2Semicircle()
{
    double constexpr maxError = 1e-15;
    Arc2<double> arc{};
    Vector2<double> unit{};
    arc.center = { 1.0, 1.0 };
    arc.radius = 2.0;
    unit = { 1.0, 0.0 };
    arc.end[0] = arc.center + arc.radius * unit;
    arc.end[1] = arc.center - arc.radius * unit;

    std::size_t constexpr numSamples = 360;
    PAQuery query{};
    PAOutput output{};
    Vector2<double> point{};
    double length{}, t{};

    length = 0.25;
    for (std::size_t i = 0; i < numSamples; ++i)
    {
        t = C_TWO_PI<double> * static_cast<double>(i) / static_cast<double>(numSamples);
        unit = { std::cos(t), std::sin(t) };
        point = arc.center + length * unit;
        output = query(point, arc);

        if (i < 180)
        {
            UTAssert(std::fabs(output.distance - 1.75) <= maxError, "Invalid distance.");
        }
        else if (i <= 269)
        {
            Vector2<double> diff = output.closest[1] - Vector2<double>{ -1.0, 1.0 };
            UTAssert(Length(diff) <= maxError, "Invalid closest point.");
        }
        else
        {
            Vector2<double> diff = output.closest[1] - Vector2<double>{ 3.0, 1.0 };
            UTAssert(Length(diff) <= maxError, "Invalid closest point.");
        }
    }

    length = 3.0;
    for (std::size_t i = 0; i < numSamples; ++i)
    {
        t = C_TWO_PI<double> * static_cast<double>(i) / static_cast<double>(numSamples);
        unit = { std::cos(t), std::sin(t) };
        point = arc.center + length * unit;
        output = query(point, arc);

        if (i < 180)
        {
            UTAssert(std::fabs(output.distance - 1.0) <= maxError, "Invalid distance.");
        }
        else if (i <= 270)
        {
            Vector2<double> diff = output.closest[1] - Vector2<double>{ -1.0, 1.0 };
            UTAssert(Length(diff) <= maxError, "Invalid closest point.");
        }
        else
        {
            Vector2<double> diff = output.closest[1] - Vector2<double>{ 3.0, 1.0 };
            UTAssert(Length(diff) <= maxError, "Invalid closest point.");
        }
    }

    point = arc.center;
    output = query(point, arc);
    Vector2<double> diff = output.closest[1] - Vector2<double>{ 3.0, 1.0 };
    UTAssert(std::fabs(output.distance - 2.0) <= maxError && Length(diff) <= maxError, "Invalid output.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/2D/DistPoint2Arc2.h>

namespace gtl
{
    template class DCPQuery<float, Vector2<float>, Arc2<float>>;
    template class DCPQuery<double, Vector2<double>, Arc2<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Vector2<Rational>, Arc2<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPoint2Arc2)
