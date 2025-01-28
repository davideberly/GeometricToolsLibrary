#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/2D/IntrRay2Ray2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrRay2Ray2
    {
    public:
        UnitTestIntrRay2Ray2();

    private:
        using RRTIQuery = TIQuery<double, Ray2<double>, Ray2<double>>;
        using RRTIOutput = RRTIQuery::Output;
        void ValidateTIQuery(RRTIOutput const& output, bool intersect,
            std::size_t numIntersections);
        void TestTIQuery();

        using RRFIQuery = FIQuery<double, Ray2<double>, Ray2<double>>;
        using RRFIOutput = RRFIQuery::Output;
        void ValidateFIQuery(RRFIOutput const& output, bool intersect,
            std::size_t numIntersections, std::array<double, 2> const& ray0Parameter,
            std::array<double, 2> const& ray1Parameter,
            std::array<Vector2<double>, 2> const& point);
        void TestFIQuery();
    };
}

UnitTestIntrRay2Ray2::UnitTestIntrRay2Ray2()
{
    UTInformation("Mathematics/Intersection/2D/IntrRay2Ray2 (ported from GTE, no logic changes)");

    TestTIQuery();
    TestFIQuery();
}

void UnitTestIntrRay2Ray2::ValidateTIQuery(RRTIOutput const& output, bool intersect,
    std::size_t numIntersections)
{
    UTAssert(output.intersect == intersect, "Invalid RRTIOutput.");
    UTAssert(output.numIntersections == numIntersections, "Invalid RRTIOutput.");
}

void UnitTestIntrRay2Ray2::TestTIQuery()
{
#if 0
    RRTIQuery query{};
    RRTIOutput output{};
    Line2<double> line{};
    Ray2<double> ray{};

    ValidateTIQuery(output, false, 0);

    // nonparallel line and ray
    line.origin = { 2.0, 1.0 };
    line.direction = { 1.0, 1.0 };
    Normalize(line.direction);
    ray.origin = { 1.0, 2.0 };
    ray.direction = { 2.0, -1.0 };
    Normalize(ray.direction);
    output = query(line, ray);
    ValidateTIQuery(output, true, 1);

    ray.direction = -ray.direction;
    output = query(line, ray);
    ValidateTIQuery(output, false, 0);

    // line and ray are parallel but not colinear
    ray.origin = { 1.0, 2.0 };
    ray.direction = { 1.0, 1.0 };
    Normalize(ray.direction);
    output = query(line, ray);
    ValidateTIQuery(output, false, 0);

    // line and ray are colinear
    ray.origin = line.origin;
    ray.direction = line.direction;
    output = query(line, ray);
    ValidateTIQuery(output, true, std::numeric_limits<std::size_t>::max());
#endif
}

void UnitTestIntrRay2Ray2::ValidateFIQuery(RRFIOutput const& output, bool intersect,
    std::size_t numIntersections, std::array<double, 2> const& ray0Parameter,
    std::array<double, 2> const& ray1Parameter,
    std::array<Vector2<double>, 2> const& point)
{
    double const maxError = 1e-14;

    UTAssert(output.intersect == intersect, "Invalid RRTIOutput.");
    UTAssert(output.numIntersections == numIntersections, "Invalid RRTIOutput.");
    UTAssert(std::fabs(output.ray0Parameter[0] - ray0Parameter[0]) <= maxError, "Invalid RRTIOutput.");
    UTAssert(std::fabs(output.ray0Parameter[1] - ray0Parameter[1]) <= maxError, "Invalid RRTIOutput.");
    UTAssert(std::fabs(output.ray1Parameter[0] - ray1Parameter[0]) <= maxError, "Invalid RRTIOutput.");
    UTAssert(std::fabs(output.ray1Parameter[1] - ray1Parameter[1]) <= maxError, "Invalid RRTIOutput.");
    UTAssert(Length(output.point[0] - point[0]) <= maxError, "Invalid RRTIOutput.");
    UTAssert(Length(output.point[1] - point[1]) <= maxError, "Invalid RRTIOutput.");
}

void UnitTestIntrRay2Ray2::TestFIQuery()
{
#if 0
    RRFIQuery query{};
    RRFIOutput output{};
    Line2<double> line{};
    Ray2<double> ray{};

    ValidateFIQuery(output, false, 0, { 0.0, 0.0 }, { 0.0, 0.0 }, { 0.0, 0.0 });

    // nonparallel line and ray
    line.origin = { 2.0, 1.0 };
    line.direction = { 1.0, 1.0 };
    Normalize(line.direction);
    ray.origin = { 1.0, 2.0 };
    ray.direction = { 2.0, -1.0 };
    Normalize(ray.direction);
    output = query(line, ray);
    ValidateFIQuery(output, true, 1,
        { std::sqrt(2.0) / 3.0, std::sqrt(2.0) / 3.0 },
        { 2.0 * std::sqrt(5.0) / 3.0, 2.0 * std::sqrt(5.0) / 3.0 },
        { 7.0 / 3.0, 4.0 / 3.0 });

    ray.direction = -ray.direction;
    output = query(line, ray);
    ValidateFIQuery(output, false, 0, { 0.0, 0.0 }, { 0.0, 0.0 }, { 0.0, 0.0 });

    // line and ray are parallel but not colinear
    ray.origin = { 1.0, 2.0 };
    ray.direction = { 1.0, 1.0 };
    Normalize(ray.direction);
    output = query(line, ray);
    ValidateFIQuery(output, false, 0, { 0.0, 0.0 }, { 0.0, 0.0 }, { 0.0, 0.0 });

    // line and ray are colinear
    double constexpr maxD = std::numeric_limits<double>::max();
    ray.origin = line.origin;
    ray.direction = line.direction;
    output = query(line, ray);
    ValidateFIQuery(output, true, std::numeric_limits<std::size_t>::max(),
        { -maxD, maxD }, { 0.0, maxD }, { 0.0, 0.0 });
#endif
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/2D/IntrRay2Ray2.h>

namespace gtl
{
    template class TIQuery<float, Ray2<float>, Ray2<float>>;
    template class FIQuery<float, Ray2<float>, Ray2<float>>;

    template class TIQuery<double, Ray2<double>, Ray2<double>>;
    template class FIQuery<double, Ray2<double>, Ray2<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Ray2<Rational>, Ray2<Rational>>;
    template class FIQuery<Rational, Ray2<Rational>, Ray2<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrRay2Ray2)
