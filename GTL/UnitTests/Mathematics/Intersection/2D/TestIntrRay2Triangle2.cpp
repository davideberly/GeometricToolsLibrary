#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Intersection/2D/IntrRay2Triangle2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIntrRay2Triangle2
    {
    public:
        UnitTestIntrRay2Triangle2();

    private:
        void Test(std::size_t i0, std::size_t i1, std::size_t i2);
    };
}

UnitTestIntrRay2Triangle2::UnitTestIntrRay2Triangle2()
{
    UTInformation("Mathematics/Intersection/2D/IntrRay2Triangle2");

    Test(0, 1, 2);
    Test(0, 2, 1);
    Test(1, 0, 2);
    Test(1, 2, 0);
    Test(2, 0, 1);
    Test(2, 1, 0);
}

void UnitTestIntrRay2Triangle2::Test(std::size_t i0, std::size_t i1, std::size_t i2)
{
    Ray2<double> ray;
    Triangle2<double> triangle;

    using RTTestQuery = TIQuery<double, Ray2<double>, Triangle2<double>>;
    using RTTestOutput = RTTestQuery::Output;
    RTTestQuery tiQuery{};
    RTTestOutput tiOutput{};

    using RTFindQuery = FIQuery<double, Ray2<double>, Triangle2<double>>;
    using RTFindOutput = RTFindQuery::Output;
    RTFindQuery fiQuery{};
    RTFindOutput fiOutput{};

    triangle.v[i0] = { 0.0, 0.0 };
    triangle.v[i1] = { 2.0, 2.0 };
    triangle.v[i2] = { 0.0, 1.0 };

    ray.direction = { 1.0, 0.0 };
    ray.origin = { -1.0, 1.0 };
    tiOutput = tiQuery(ray, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(ray, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 1.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 2.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");

    ray.origin = { 0.0, 1.0 };
    tiOutput = tiQuery(ray, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(ray, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 1.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");

    ray.origin = { 0.5, 1.0 };
    tiOutput = tiQuery(ray, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(ray, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 0.5, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 0.5, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");

    ray.origin = { 1.0, 1.0 };
    tiOutput = tiQuery(ray, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(ray, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 1, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 0.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 0.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");

    ray.origin = { 2.0, 1.0 };
    tiOutput = tiQuery(ray, triangle);
    UTAssert(tiOutput.intersect == false, "TIQuery failed.");
    fiOutput = fiQuery(ray, triangle);
    UTAssert(fiOutput.intersect == false, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 0, "FIQuery failed.");

    ray.direction = { -1.0, 0.0 };
    ray.origin = { 2.0, 1.0 };
    tiOutput = tiQuery(ray, triangle);
    UTAssert(tiOutput.intersect == true, "TIQuery failed.");
    fiOutput = fiQuery(ray, triangle);
    UTAssert(fiOutput.intersect == true, "FIQuery failed.");
    UTAssert(fiOutput.numIntersections == 2, "FIQuery failed.");
    UTAssert(fiOutput.parameter[0] == 1.0, "FIQuery failed.");
    UTAssert(fiOutput.parameter[1] == 2.0, "FIQuery failed.");
    UTAssert((fiOutput.point[0] == Vector2<double>{ 1.0, 1.0 }), "FIQuery failed.");
    UTAssert((fiOutput.point[1] == Vector2<double>{ 0.0, 1.0 }), "FIQuery failed.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Intersection/2D/IntrRay2Triangle2.h>

namespace gtl
{
    template class TIQuery<float, Ray2<float>, Triangle2<float>>;
    template class FIQuery<float, Ray2<float>, Triangle2<float>>;

    template class TIQuery<double, Ray2<double>, Triangle2<double>>;
    template class FIQuery<double, Ray2<double>, Triangle2<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class TIQuery<Rational, Ray2<Rational>, Triangle2<Rational>>;
    template class FIQuery<Rational, Ray2<Rational>, Triangle2<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IntrRay2Triangle2)
