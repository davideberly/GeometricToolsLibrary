#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/ND/Triangle.h>
using namespace gtl;

namespace gtl
{
    class UnitTestTriangle
    {
    public:
        UnitTestTriangle();

    private:
        void Test();
    };
}

UnitTestTriangle::UnitTestTriangle()
{
    UTInformation("Mathematics/Primitives/ND/Triangle");

    Test();
}

void UnitTestTriangle::Test()
{
    Triangle<double, 2> tri0{};
    UTAssert(IsZero(tri0.v[0]), "Expecting zero v[0].");
    UTAssert(IsZero(tri0.v[1]), "Expecting zero v[1].");
    UTAssert(IsZero(tri0.v[2]), "Expecting zero v[2].");

    std::array<Vector2<double>, 3> v =
    {
        Vector2<double>{ 0.0, 0.0 },
        Vector2<double>{ 1.0, 0.0 },
        Vector2<double>{ 0.0, 1.0 },
    };
    Triangle<double, 2> tri1(v);
    UTAssert(tri1.v[0] == v[0], "Incorrect v[0].");
    UTAssert(tri1.v[1] == v[1], "Incorrect v[1].");
    UTAssert(tri1.v[2] == v[2], "Incorrect v[2].");

    bool result{};
    result = (tri0 == tri1);
    UTAssert(result == false, "operator== failed");
    result = (tri0 != tri1);
    UTAssert(result == true, "operator!= failed");
    result = (tri0 < tri1);
    UTAssert(result == true, "operator< failed");
    result = (tri0 <= tri1);
    UTAssert(result == true, "operator<= failed");
    result = (tri0 > tri1);
    UTAssert(result == false, "operator> failed");
    result = (tri0 >= tri1);
    UTAssert(result == false, "operator>= failed");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/ND/Triangle.h>

namespace gtl
{
    template class Triangle<float, 2>;
    template bool operator==(Triangle<float, 2> const&, Triangle<float, 2> const&);
    template bool operator!=(Triangle<float, 2> const&, Triangle<float, 2> const&);
    template bool operator< (Triangle<float, 2> const&, Triangle<float, 2> const&);
    template bool operator<=(Triangle<float, 2> const&, Triangle<float, 2> const&);
    template bool operator> (Triangle<float, 2> const&, Triangle<float, 2> const&);
    template bool operator>=(Triangle<float, 2> const&, Triangle<float, 2> const&);

    template class Triangle<double, 3>;
    template bool operator==(Triangle<double, 3> const&, Triangle<double, 3> const&);
    template bool operator!=(Triangle<double, 3> const&, Triangle<double, 3> const&);
    template bool operator< (Triangle<double, 3> const&, Triangle<double, 3> const&);
    template bool operator<=(Triangle<double, 3> const&, Triangle<double, 3> const&);
    template bool operator> (Triangle<double, 3> const&, Triangle<double, 3> const&);
    template bool operator>=(Triangle<double, 3> const&, Triangle<double, 3> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Triangle<Rational, 4>;
    template bool operator==(Triangle<Rational, 4> const&, Triangle<Rational, 4> const&);
    template bool operator!=(Triangle<Rational, 4> const&, Triangle<Rational, 4> const&);
    template bool operator< (Triangle<Rational, 4> const&, Triangle<Rational, 4> const&);
    template bool operator<=(Triangle<Rational, 4> const&, Triangle<Rational, 4> const&);
    template bool operator> (Triangle<Rational, 4> const&, Triangle<Rational, 4> const&);
    template bool operator>=(Triangle<Rational, 4> const&, Triangle<Rational, 4> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Triangle)
