#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/2D/IntrSegment2Triangle2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrSegment2Triangle2
    {
    public:
        UnitTestIntrSegment2Triangle2();

    private:
        void Test0(std::size_t i0, std::size_t i1, std::size_t i2);
        void Test1(std::size_t i0, std::size_t i1, std::size_t i2);
    };
}

UnitTestIntrSegment2Triangle2::UnitTestIntrSegment2Triangle2()
{
    UTInformation("Mathematics/Intersection/2D/IntrSegment2Triangle2");

    Test0(0, 1, 2);
    Test0(0, 2, 1);
    Test0(1, 0, 2);
    Test0(1, 2, 0);
    Test0(2, 0, 1);
    Test0(2, 1, 0);
    Test1(0, 1, 2);
    Test1(0, 2, 1);
    Test1(1, 0, 2);
    Test1(1, 2, 0);
    Test1(2, 0, 1);
    Test1(2, 1, 0);
}

void UnitTestIntrSegment2Triangle2::Test0(std::size_t i0, std::size_t i1, std::size_t i2)
{
    Segment2<double> segment;
    Triangle2<double> triangle;
    double error;

    using STTestQuery = TIQuery<double, Segment2<double>, Triangle2<double>>;
    using STTestOutput = STTestQuery::Output;
    STTestQuery tiQuery{};
    STTestOutput tiOutput{};

    using STFindQuery = FIQuery<double, Segment2<double>, Triangle2<double>>;
    using STFindOutput = STFindQuery::Output;
    STFindQuery fiQuery{};
    STFindOutput fiOutput{};

    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 2.0 };
    triangle.v[i2] = { 0.0, 1.0 };

    segment.p[0] = { -1.0, 1.0 };
    segment.p[1] = { -0.5, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == false, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == false, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 0, "FIQuery failed.");

    segment.p[0] = { -1.0, 1.0 };
    segment.p[1] = { 0.0, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 1, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 1.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");

    segment.p[0] = { -1.0, 1.0 };
    segment.p[1] = { 0.5, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    error = std::fabs(fiOutput.parameter[0] - 2.0 / 3.0);
    UTAssert(error <= 1e-15, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 0.5, 1.0 }), "FIQuery failed.");

    segment.p[0] = { -1.0, 1.0 };
    segment.p[1] = { 1.0, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.5, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");

    segment.p[0] = { -1.0, 1.0 };
    segment.p[1] = { 1.5, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    error = std::fabs(fiOutput.parameter[0] - 0.4);
    UTAssert(error <= 1e-15, "FIQuery failed.");
    error = std::fabs(fiOutput.parameter[1] - 0.8);
    UTAssert(error <= 1e-15, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");

    segment.p[0] = { 0.0, 1.0 };
    segment.p[1] = { 0.5, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 0.5, 1.0 }), "FIQuery failed.");

    segment.p[0] = { 0.0, 1.0 };
    segment.p[1] = { 1.0, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");

    segment.p[0] = { 0.0, 1.0 };
    segment.p[1] = { 1.5, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.0, "FIQuery failed.");
    error = std::fabs(fiOutput.parameter[1] - 2.0 / 3.0);
    UTAssert(error <= 1e-15, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");

    segment.p[0] = { 0.25, 1.0 };
    segment.p[1] = { 0.75, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.25, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 0.75, 1.0 }), "FIQuery failed.");

    segment.p[0] = { 0.25, 1.0 };
    segment.p[1] = { 1.0, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.25, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");

    segment.p[0] = { 0.25, 1.0 };
    segment.p[1] = { 1.5, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.0, "FIQuery failed.");
    error = std::fabs(fiOutput.parameter[1] - 0.6);
    UTAssert(error <= 1e-15, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.25, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");

    segment.p[0] = { 1.0, 1.0 };
    segment.p[1] = { 1.5, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 1, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 0.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");

    segment.p[0] = { 1.5, 1.0 };
    segment.p[1] = { 2.0, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == false, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == false, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 0, "FIQuery failed.");
}

void UnitTestIntrSegment2Triangle2::Test1(std::size_t i0, std::size_t i1, std::size_t i2)
{
    Segment2<double> segment;
    Triangle2<double> triangle;
    double error;

    using STTestQuery = TIQuery<double, Segment2<double>, Triangle2<double>>;
    using STTestOutput = STTestQuery::Output;
    STTestQuery tiQuery{};
    STTestOutput tiOutput{};

    using STFindQuery = FIQuery<double, Segment2<double>, Triangle2<double>>;
    using STFindOutput = STFindQuery::Output;
    STFindQuery fiQuery{};
    STFindOutput fiOutput{};

    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 2.0 };
    triangle.v[i2] = { 0.0, 1.0 };

    segment.p[1] = { -1.0, 1.0 };
    segment.p[0] = { -0.5, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == false, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == false, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 0, "FIQuery failed.");

    segment.p[1] = { -1.0, 1.0 };
    segment.p[0] = { 0.0, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 1, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 0.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");

    segment.p[1] = { -1.0, 1.0 };
    segment.p[0] = { 0.5, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.0, "FIQuery failed.");
    error = std::fabs(fiOutput.parameter[1] - 1.0 / 3.0);
    UTAssert(error <= 1e-15, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.5, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");

    segment.p[1] = { -1.0, 1.0 };
    segment.p[0] = { 1.0, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 0.5, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");

    segment.p[1] = { -1.0, 1.0 };
    segment.p[0] = { 1.5, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    error = std::fabs(fiOutput.parameter[0] - 0.2);
    UTAssert(error <= 1e-15, "FIQuery failed.");
    error = std::fabs(fiOutput.parameter[1] - 0.6);
    UTAssert(error <= 1e-15, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");

    segment.p[1] = { 0.0, 1.0 };
    segment.p[0] = { 0.5, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.5, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");

    segment.p[1] = { 0.0, 1.0 };
    segment.p[0] = { 1.0, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");

    segment.p[1] = { 0.0, 1.0 };
    segment.p[0] = { 1.5, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    error = std::fabs(fiOutput.parameter[0] - 1.0 / 3.0);
    UTAssert(error <= 1e-15, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");

    segment.p[1] = { 0.25, 1.0 };
    segment.p[0] = { 0.75, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.75, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 0.25, 1.0 }), "FIQuery failed.");

    segment.p[1] = { 0.25, 1.0 };
    segment.p[0] = { 1.0, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 0.25, 1.0 }), "FIQuery failed.");

    segment.p[1] = { 0.25, 1.0 };
    segment.p[0] = { 1.5, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    error = std::fabs(fiOutput.parameter[0] - 0.4);
    UTAssert(error <= 1e-15, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 0.25, 1.0 }), "FIQuery failed.");

    segment.p[1] = { 1.0, 1.0 };
    segment.p[0] = { 1.5, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 1, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 1.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");

    segment.p[1] = { 1.5, 1.0 };
    segment.p[0] = { 2.0, 1.0 };
    tiOutput = tiQuery(segment, triangle);
    UTAssert(tiOutput.intersect == false, "TIQuery failed.");
    fiOutput = fiQuery(segment, triangle);
    UTAssert(fiOutput.intersect == false, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 0, "FIQuery failed.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/2D/IntrSegment2Triangle2.h>

namespace gtl
{
    template class TIQuery<float, Segment2<float>, Triangle2<float>>;
    template class FIQuery<float, Segment2<float>, Triangle2<float>>;

    template class TIQuery<double, Segment2<double>, Triangle2<double>>;
    template class FIQuery<double, Segment2<double>, Triangle2<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Segment2<Rational>, Triangle2<Rational>>;
    template class FIQuery<Rational, Segment2<Rational>, Triangle2<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrSegment2Triangle2)
