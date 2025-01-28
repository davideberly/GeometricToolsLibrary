#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/2D/IntrLine2Line2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrLine2Line2
    {
    public:
        UnitTestIntrLine2Line2();

    private:
        using LLTIQuery = TIQuery<double, Line2<double>, Line2<double>>;
        using LLTIOutput = LLTIQuery::Output;
        void ValidateTIQuery(LLTIOutput const& output, bool intersect,
            std::size_t numIntersections);
        void TestTIQuery();

        using LLFIQuery = FIQuery<double, Line2<double>, Line2<double>>;
        using LLFIOutput = LLFIQuery::Output;
        void ValidateFIQuery(LLFIOutput const& output, bool intersect,
            std::size_t numIntersections, std::array<double, 2> const& line0Parameter,
            std::array<double, 2> const& line1Parameter,
            Vector2<double> const& point);
        void TestFIQuery();
    };
}

UnitTestIntrLine2Line2::UnitTestIntrLine2Line2()
{
    UTInformation("Mathematics/Intersection/2D/IntrLine2Line2");

    TestTIQuery();
    TestFIQuery();
}

void UnitTestIntrLine2Line2::ValidateTIQuery(LLTIOutput const& output, bool intersect,
    std::size_t numIntersections)
{
    UTAssert(output.intersect == intersect, "Invalid TIOutput.");
    UTAssert(output.numIntersections == numIntersections, "Invalid TIOutput.");
}

void UnitTestIntrLine2Line2::TestTIQuery()
{
    LLTIQuery query{};
    LLTIOutput output{};
    Line2<double> line0{}, line1{};

    ValidateTIQuery(output, false, 0);

    // nonparallel lines
    line0.origin = { 2.0, 1.0 };
    line0.direction = { 1.0, 1.0 };
    Normalize(line0.direction);
    line1.origin = { 1.0, 2.0 };
    line1.direction = { 2.0, -1.0 };
    Normalize(line1.direction);
    output = query(line0, line1);
    ValidateTIQuery(output, true, 1);

    // parallel but not same line
    line1.origin = { 1.0, 2.0 };
    line1.direction = { 1.0, 1.0 };
    Normalize(line1.direction);
    output = query(line0, line1);
    ValidateTIQuery(output, false, 0);

    // same line
    line1 = line0;
    output = query(line0, line1);
    ValidateTIQuery(output, true, std::numeric_limits<std::size_t>::max());
}

void UnitTestIntrLine2Line2::ValidateFIQuery(LLFIOutput const& output, bool intersect,
    std::size_t numIntersections, std::array<double, 2> const& line0Parameter,
    std::array<double, 2> const& line1Parameter, Vector2<double> const& point)
{
    double const maxError = 1e-14;

    UTAssert(output.intersect == intersect, "Invalid TIOutput.");
    UTAssert(output.numIntersections == numIntersections, "Invalid TIOutput.");
    UTAssert(std::fabs(output.line0Parameter[0] - line0Parameter[0]) <= maxError, "Invalid TIOutput.");
    UTAssert(std::fabs(output.line0Parameter[1] - line0Parameter[1]) <= maxError, "Invalid TIOutput.");
    UTAssert(std::fabs(output.line1Parameter[0] - line1Parameter[0]) <= maxError, "Invalid TIOutput.");
    UTAssert(std::fabs(output.line1Parameter[1] - line1Parameter[1]) <= maxError, "Invalid TIOutput.");
    UTAssert(Length(output.point - point) <= maxError, "Invalid TIOutput.");
}

void UnitTestIntrLine2Line2::TestFIQuery()
{
    LLFIQuery query{};
    LLFIOutput output{};
    Line2<double> line0{}, line1{};

    ValidateFIQuery(output, false, 0, { 0.0, 0.0 }, { 0.0, 0.0 }, { 0.0, 0.0 });

    // nonparallel lines
    line0.origin = { 2.0, 1.0 };
    line0.direction = { 1.0, 1.0 };
    Normalize(line0.direction);
    line1.origin = { 1.0, 2.0 };
    line1.direction = { 2.0, -1.0 };
    Normalize(line1.direction);
    output = query(line0, line1);
    ValidateFIQuery(output, true, 1,
        { std::sqrt(2.0) / 3.0, std::sqrt(2.0) / 3.0 },
        { 2.0 * std::sqrt(5.0) / 3.0, 2.0 * std::sqrt(5.0) / 3.0 },
        { 7.0 / 3.0, 4.0 / 3.0 });

    // parallel but not same line
    line1.origin = { 1.0, 2.0 };
    line1.direction = { 1.0, 1.0 };
    Normalize(line1.direction);
    output = query(line0, line1);
    ValidateFIQuery(output, false, 0,
        { 0.0, 0.0 }, { 0.0, 0.0 }, { 0.0, 0.0 });

    // same line
    double constexpr maxD = std::numeric_limits<double>::max();
    line1 = line0;
    output = query(line0, line1);
    ValidateFIQuery(output, true, std::numeric_limits<std::size_t>::max(),
        { -maxD, maxD }, { -maxD, maxD }, { 0.0, 0.0 });

    // nonparallel lines, non-unit directions
    line0.origin = { 2.0, 1.0 };
    line0.direction = { 1.0, 1.0 };
    line1.origin = { 1.0, 2.0 };
    line1.direction = { 2.0, -1.0 };
    output = query(line0, line1);
    ValidateFIQuery(output, true, 1,
        { 1.0 / 3.0, 1.0 / 3.0 },
        { 2.0 / 3.0, 2.0 / 3.0 },
        { 7.0 / 3.0, 4.0 / 3.0 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/2D/IntrLine2Line2.h>

namespace gtl
{
    template class TIQuery<float, Line2<float>, Line2<float>>;
    template class FIQuery<float, Line2<float>, Line2<float>>;

    template class TIQuery<double, Line2<double>, Line2<double>>;
    template class FIQuery<double, Line2<double>, Line2<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Line2<Rational>, Line2<Rational>>;
    template class FIQuery<Rational, Line2<Rational>, Line2<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrLine2Line2)
