#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/2D/IntrTriangle2Triangle2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrTriangle2Triangle2
    {
    public:
        UnitTestIntrTriangle2Triangle2();

    private:
        void TestTIQuery();
        void TestFIQuery();
    };
}

UnitTestIntrTriangle2Triangle2::UnitTestIntrTriangle2Triangle2()
{
    UTInformation("Mathematics/Intersection/2D/IntrTriangle2Triangle2");

    TestTIQuery();
    TestFIQuery();
}

void UnitTestIntrTriangle2Triangle2::TestTIQuery()
{
    using TITrianglesQuery = TIQuery<double, Triangle2<double>, Triangle2<double>>;
    TITrianglesQuery tiQuery{};
    TITrianglesQuery::Output tiOutput{};
    Triangle2<double> tri0{}, tri1{};

    tri0.v[0] = { 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0 };
    tri1.v[0] = { 1.0, 1.0 };
    tri1.v[1] = { -1.0, 0.5 };
    tri1.v[2] = { 0.0, -1.0 };
    tiOutput = tiQuery(tri0, tri1);
    UTAssert(
        tiOutput.intersect == true,
        "Invalid TIQuery.");

    for (std::size_t i = 0; i < 3; ++i)
    {
        tri1.v[i] += { 10.0, 0.0 };
    }
    tiOutput = tiQuery(tri0, tri1);
    UTAssert(
        tiOutput.intersect == false,
        "Invalid TIQuery.");
}


void UnitTestIntrTriangle2Triangle2::TestFIQuery()
{
    using FITrianglesQuery = FIQuery<double, Triangle2<double>, Triangle2<double>>;
    FITrianglesQuery fiQuery{};
    FITrianglesQuery::Output fiOutput{};
    Triangle2<double> tri0{}, tri1{};
    std::array<Vector2<double>, 6> expected{};
    std::array<double, 6> error{};

    tri0.v[0] = { 0.0, 0.0 };
    tri0.v[1] = { 1.0, 0.0 };
    tri0.v[2] = { 0.0, 1.0 };
    tri1.v[0] = { 1.0, 1.0 };
    tri1.v[1] = { -1.0, 0.5 };
    tri1.v[2] = { 0.0, -1.0 };
    fiOutput = fiQuery(tri0, tri1);
    UTAssert(
        fiOutput.intersect == true,
        "Invalid FIQuery.");
    UTAssert(
        fiOutput.intersection.size() == 5,
        "Invalid FIQuery.");
    expected[0] = { 0.2, 0.8 };
    expected[1] = { 0.0, 0.75 };
    expected[2] = { 0.0, 0.0 };
    expected[3] = { 0.5, 0.0 };
    expected[4] = { 2.0 / 3.0, 1.0 / 3.0 };
    for (std::size_t i = 0; i < 5; ++i)
    {
        error[i] = Length(fiOutput.intersection[i] - expected[i]);
        UTAssert(
            error[i] <= 1e-15,
            "Invalid FIQuery, index " + std::to_string(i));
    }
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/2D/IntrTriangle2Triangle2.h>

namespace gtl
{
    template class TIQuery<float, Triangle2<float>, Triangle2<float>>;
    template class FIQuery<float, Triangle2<float>, Triangle2<float>>;

    template class TIQuery<double, Triangle2<double>, Triangle2<double>>;
    template class FIQuery<double, Triangle2<double>, Triangle2<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Triangle2<Rational>, Triangle2<Rational>>;
    template class FIQuery<Rational, Triangle2<Rational>, Triangle2<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrTriangle2Triangle2)
