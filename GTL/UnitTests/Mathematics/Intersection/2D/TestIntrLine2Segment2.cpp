#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/2D/IntrLine2Segment2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrLine2Segment2
    {
    public:
        UnitTestIntrLine2Segment2();

    private:
        using LSTIQuery = TIQuery<double, Line2<double>, Segment2<double>>;
        using LSTIOutput = LSTIQuery::Output;
        void ValidateTIQuery(LSTIOutput const& output, bool intersect,
            std::size_t numIntersections);
        void TestTIQuery();

        using LSFIQuery = FIQuery<double, Line2<double>, Segment2<double>>;
        using FIOutput = LSFIQuery::Output;
        void ValidateFIQuery(FIOutput const& output, bool intersect,
            std::size_t numIntersections, std::array<double, 2> const& lineParameter,
            std::array<double, 2> const& segmentParameter,
            Vector2<double> const& point);
        void TestFIQuery();
    };
}

UnitTestIntrLine2Segment2::UnitTestIntrLine2Segment2()
{
    UTInformation("Mathematics/Intersection/2D/IntrLine2Segment2");

    TestTIQuery();
    TestFIQuery();
}

void UnitTestIntrLine2Segment2::ValidateTIQuery(LSTIOutput const& output, bool intersect,
    std::size_t numIntersections)
{
    UTAssert(output.intersect == intersect, "Invalid LSTIOutput.");
    UTAssert(output.numIntersections == numIntersections, "Invalid LSTIOutput.");
}

void UnitTestIntrLine2Segment2::TestTIQuery()
{
    LSTIQuery query{};
    LSTIOutput output{};
    Line2<double> line{};
    Segment2<double> segment{};

    ValidateTIQuery(output, false, 0);

    // nonparallel line and segment
    line.origin = { 2.0, 1.0 };
    line.direction = { 1.0, 1.0 };
    Normalize(line.direction);
    segment.p[0] = { 1.0, 2.0 };
    segment.p[1] = segment.p[0] + Vector2<double>{ 2.0, -1.0 };
    output = query(line, segment);
    ValidateTIQuery(output, true, 1);

    std::swap(segment.p[0], segment.p[1]);
    output = query(line, segment);
    ValidateTIQuery(output, true, 1);

    segment.p[0] = { 1.0, 2.0 };
    segment.p[1] = segment.p[0] - Vector2<double>{ 2.0, -1.0 };
    output = query(line, segment);
    ValidateTIQuery(output, false, 0);

    // line and segment are parallel but not colinear
    segment.p[0] = { 1.0, 2.0 };
    segment.p[1] = segment.p[0] + Vector2<double>{ 1.0, 1.0 };
    output = query(line, segment);
    ValidateTIQuery(output, false, 0);

    // line and segment are colinear
    line.origin = segment.p[0];
    line.direction = segment.p[1] - segment.p[0];
    output = query(line, segment);
    ValidateTIQuery(output, true, std::numeric_limits<std::size_t>::max());
}

void UnitTestIntrLine2Segment2::ValidateFIQuery(FIOutput const& output, bool intersect,
    std::size_t numIntersections, std::array<double, 2> const& lineParameter,
    std::array<double, 2> const& segmentParameter, Vector2<double> const& point)
{
    double const maxError = 1e-14;

    UTAssert(output.intersect == intersect, "Invalid LSTIOutput.");
    UTAssert(output.numIntersections == numIntersections, "Invalid LSTIOutput.");
    UTAssert(std::fabs(output.lineParameter[0] - lineParameter[0]) <= maxError, "Invalid LSTIOutput.");
    UTAssert(std::fabs(output.lineParameter[1] - lineParameter[1]) <= maxError, "Invalid LSTIOutput.");
    UTAssert(std::fabs(output.segmentParameter[0] - segmentParameter[0]) <= maxError, "Invalid LSTIOutput.");
    UTAssert(std::fabs(output.segmentParameter[1] - segmentParameter[1]) <= maxError, "Invalid LSTIOutput.");
    UTAssert(Length(output.point - point) <= maxError, "Invalid LSTIOutput.");
}

void UnitTestIntrLine2Segment2::TestFIQuery()
{
    LSFIQuery query{};
    FIOutput output{};
    Line2<double> line{};
    Segment2<double> segment{};

    ValidateFIQuery(output, false, 0, { 0.0, 0.0 }, { 0.0, 0.0 }, { 0.0, 0.0 });

    // nonparallel line and segment
    line.origin = { 2.0, 1.0 };
    line.direction = { 1.0, 1.0 };
    Normalize(line.direction);
    segment.p[0] = { 1.0, 2.0 };
    segment.p[1] = segment.p[0] + Vector2<double>{ 2.0, -1.0 };
    output = query(line, segment);
    ValidateFIQuery(output, true, 1,
        { std::sqrt(2.0) / 3.0, std::sqrt(2.0) / 3.0 },
        { 2.0 / 3.0, 2.0 / 3.0 },
        { 7.0 / 3.0, 4.0 / 3.0 });

    std::swap(segment.p[0], segment.p[1]);
    output = query(line, segment);
    ValidateFIQuery(output, true, 1,
        { std::sqrt(2.0) / 3.0, std::sqrt(2.0) / 3.0 },
        { 1.0 / 3.0, 1.0 / 3.0 },
        { 7.0 / 3.0, 4.0 / 3.0 });

    segment.p[0] = { 1.0, 2.0 };
    segment.p[1] = segment.p[0] - Vector2<double>{ 2.0, -1.0 };
    output = query(line, segment);
    ValidateFIQuery(output, false, 0, { 0.0, 0.0 }, { 0.0, 0.0 }, { 0.0, 0.0 });

    // line and segment are parallel but not colinear
    segment.p[0] = { 1.0, 2.0 };
    segment.p[1] = segment.p[0] + Vector2<double>{ 1.0, 1.0 };
    output = query(line, segment);
    ValidateFIQuery(output, false, 0, { 0.0, 0.0 }, { 0.0, 0.0 }, { 0.0, 0.0 });

    // line and segment are colinear
    double constexpr maxD = std::numeric_limits<double>::max();
    line.origin = segment.p[0];
    line.direction = segment.p[1] - segment.p[0];
    output = query(line, segment);
    ValidateFIQuery(output, true, std::numeric_limits<std::size_t>::max(),
        { -maxD, maxD }, { 0.0, 1.0 }, { 0.0, 0.0 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/2D/IntrLine2Segment2.h>

namespace gtl
{
    template class TIQuery<float, Line2<float>, Segment2<float>>;
    template class FIQuery<float, Line2<float>, Segment2<float>>;

    template class TIQuery<double, Line2<double>, Segment2<double>>;
    template class FIQuery<double, Line2<double>, Segment2<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Line2<Rational>, Segment2<Rational>>;
    template class FIQuery<Rational, Line2<Rational>, Segment2<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrLine2Segment2)
