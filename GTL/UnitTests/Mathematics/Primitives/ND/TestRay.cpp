#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/ND/Ray.h>
using namespace gtl;

namespace gtl
{
    class UnitTestRay
    {
    public:
        UnitTestRay();

    private:
        void Test();
    };
}

UnitTestRay::UnitTestRay()
{
    UTInformation("Mathematics/Primitives/ND/Ray");

    Test();
}

void UnitTestRay::Test()
{
    Ray2<double> ray0{};
    UTAssert(IsZero(ray0.origin), "Expecting zero origin.");
    UTAssert(IsZero(ray0.direction), "Expecting zero direction.");

    Vector2<double> origin{ 1.0, 2.0 };
    Vector2<double> direction{ 0.2, 0.1 };
    Normalize(direction);
    Ray2<double> ray1(origin, direction);
    UTAssert(ray1.origin == origin, "Incorrect origin.");
    UTAssert(ray1.direction == direction, "Incorrect direction.");

    bool result{};
    result = (ray0 == ray1);
    UTAssert(result == false, "operator== failed");
    result = (ray0 != ray1);
    UTAssert(result == true, "operator!= failed");
    result = (ray0 < ray1);
    UTAssert(result == true, "operator< failed");
    result = (ray0 <= ray1);
    UTAssert(result == true, "operator<= failed");
    result = (ray0 > ray1);
    UTAssert(result == false, "operator> failed");
    result = (ray0 >= ray1);
    UTAssert(result == false, "operator>= failed");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/ND/Ray.h>

namespace gtl
{
    template class Ray<float, 2>;
    template bool operator==(Ray<float, 2> const&, Ray<float, 2> const&);
    template bool operator!=(Ray<float, 2> const&, Ray<float, 2> const&);
    template bool operator< (Ray<float, 2> const&, Ray<float, 2> const&);
    template bool operator<=(Ray<float, 2> const&, Ray<float, 2> const&);
    template bool operator> (Ray<float, 2> const&, Ray<float, 2> const&);
    template bool operator>=(Ray<float, 2> const&, Ray<float, 2> const&);

    template class Ray<double, 3>;
    template bool operator==(Ray<double, 3> const&, Ray<double, 3> const&);
    template bool operator!=(Ray<double, 3> const&, Ray<double, 3> const&);
    template bool operator< (Ray<double, 3> const&, Ray<double, 3> const&);
    template bool operator<=(Ray<double, 3> const&, Ray<double, 3> const&);
    template bool operator> (Ray<double, 3> const&, Ray<double, 3> const&);
    template bool operator>=(Ray<double, 3> const&, Ray<double, 3> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Ray<Rational, 4>;
    template bool operator==(Ray<Rational, 4> const&, Ray<Rational, 4> const&);
    template bool operator!=(Ray<Rational, 4> const&, Ray<Rational, 4> const&);
    template bool operator< (Ray<Rational, 4> const&, Ray<Rational, 4> const&);
    template bool operator<=(Ray<Rational, 4> const&, Ray<Rational, 4> const&);
    template bool operator> (Ray<Rational, 4> const&, Ray<Rational, 4> const&);
    template bool operator>=(Ray<Rational, 4> const&, Ray<Rational, 4> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Ray)
