#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/2D/IntrLine2Ray2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrLine2Ray2
    {
    public:
        UnitTestIntrLine2Ray2();

    private:
        using LRTIQuery = TIQuery<double, Line2<double>, Ray2<double>>;
        using LRTIOutput = LRTIQuery::Output;
        void ValidateTIQuery(LRTIOutput const& output, bool intersect,
            std::size_t numIntersections);
        void TestTIQuery();

        using LRFIQuery = FIQuery<double, Line2<double>, Ray2<double>>;
        using LRFIOutput = LRFIQuery::Output;
        void ValidateFIQuery(LRFIOutput const& output, bool intersect,
            std::size_t numIntersections, std::array<double, 2> const& lineParameter,
            std::array<double, 2> const& rayParameter,
            Vector2<double> const& point);
        void TestFIQuery();
    };
}

UnitTestIntrLine2Ray2::UnitTestIntrLine2Ray2()
{
    UTInformation("Mathematics/Intersection/2D/IntrLine2Ray2");

    TestTIQuery();
    TestFIQuery();
}

void UnitTestIntrLine2Ray2::ValidateTIQuery(LRTIOutput const& output, bool intersect,
    std::size_t numIntersections)
{
    UTAssert(output.intersect == intersect, "Invalid TIOutput.");
    UTAssert(output.numIntersections == numIntersections, "Invalid TIOutput.");
}

void UnitTestIntrLine2Ray2::TestTIQuery()
{
    LRTIQuery query{};
    LRTIOutput output{};
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
}

void UnitTestIntrLine2Ray2::ValidateFIQuery(LRFIOutput const& output, bool intersect,
    std::size_t numIntersections, std::array<double, 2> const& lineParameter,
    std::array<double, 2> const& rayParameter, Vector2<double> const& point)
{
    double const maxError = 1e-14;

    UTAssert(output.intersect == intersect, "Invalid TIOutput.");
    UTAssert(output.numIntersections == numIntersections, "Invalid TIOutput.");
    UTAssert(std::fabs(output.lineParameter[0] - lineParameter[0]) <= maxError, "Invalid TIOutput.");
    UTAssert(std::fabs(output.lineParameter[1] - lineParameter[1]) <= maxError, "Invalid TIOutput.");
    UTAssert(std::fabs(output.rayParameter[0] - rayParameter[0]) <= maxError, "Invalid TIOutput.");
    UTAssert(std::fabs(output.rayParameter[1] - rayParameter[1]) <= maxError, "Invalid TIOutput.");
    UTAssert(Length(output.point - point) <= maxError, "Invalid TIOutput.");
}

void UnitTestIntrLine2Ray2::TestFIQuery()
{
    LRFIQuery query{};
    LRFIOutput output{};
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
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/2D/IntrLine2Ray2.h>

namespace gtl
{
    template class TIQuery<float, Line2<float>, Ray2<float>>;
    template class FIQuery<float, Line2<float>, Ray2<float>>;

    template class TIQuery<double, Line2<double>, Ray2<double>>;
    template class FIQuery<double, Line2<double>, Ray2<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Line2<Rational>, Ray2<Rational>>;
    template class FIQuery<Rational, Line2<Rational>, Ray2<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrLine2Ray2)
