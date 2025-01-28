#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/ND/Hyperplane.h>
using namespace gtl;

namespace gtl
{
    class UnitTestHyperplane
    {
    public:
        UnitTestHyperplane();

    private:
        void Test2();
        void Test3();
    };
}

UnitTestHyperplane::UnitTestHyperplane()
{
    UTInformation("Mathematics/Primitives/ND/Hyperplane");

    Test2();
    Test3();
}

void UnitTestHyperplane::Test2()
{
    double maxError = 1e-15;

    Hyperplane<double, 2> hplane0{};
    UTAssert(IsZero(hplane0.normal), "Expecting zero normal.");
    UTAssert(IsZero(hplane0.origin), "Expecting zero origin.");
    UTAssert(hplane0.constant == 0.0, "Expecting zero constant.");

    std::array<Vector2<double>, 2> p =
    {
        Vector2<double>{ 1.0, 0.0 },
        Vector2<double>{ 0.0, 1.0 }
    };
    Hyperplane<double, 2> hplane1(p);
    Vector2<double> expectedNormal{ C_INV_SQRT_2<double>, C_INV_SQRT_2<double> };
    double expectedConstant = C_INV_SQRT_2<double>;
    UTAssert(Length(hplane1.normal - expectedNormal) <= maxError, "Incorrect normal.");
    UTAssert(std::fabs(hplane1.constant - expectedConstant) <= maxError, "Incorrect constant.");
}

void UnitTestHyperplane::Test3()
{
    double maxError = 1e-15;

    Hyperplane<double, 3> hplane0{};
    UTAssert(IsZero(hplane0.normal), "Expecting zero normal.");
    UTAssert(IsZero(hplane0.origin), "Expecting zero origin.");
    UTAssert(hplane0.constant == 0.0, "Expecting zero constant.");

    Vector3<double> normal{ 1.0, 1.0, 1.0 };
    Normalize(normal);
    double constant = -1.2345;
    Hyperplane<double, 3> hplane1(normal, constant);
    if (Dot(normal, hplane1.normal) < 0.0)
    {
        hplane1.normal = -hplane1.normal;
        hplane1.constant = -hplane1.constant;
    }
    double error = Length(hplane1.normal - normal);
    UTAssert(error <= maxError, "Incorrect normal.");
    error = std::fabs(hplane1.constant - constant);
    UTAssert(error <= maxError, "Incorrect constant.");

    Vector3<double> origin{ 0.1, 0.2, 0.3 };
    Hyperplane<double, 3> hplane2(normal, origin);
    if (Dot(normal, hplane2.normal) < 0.0)
    {
        hplane2.normal = -hplane2.normal;
        hplane2.constant = -hplane2.constant;
    }
    error = Length(hplane2.normal - normal);
    UTAssert(error <= maxError, "Incorrect normal.");
    error = Length(hplane2.origin - origin);
    UTAssert(error <= maxError, "Incorrect constant.");

    // NOTE: Comparisons are not made because the origins can be different
    // and the (normal,constant) pair is equivalent to (-normal,-constant).
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/ND/Hyperplane.h>

namespace gtl
{
    template class Hyperplane<float, 2>;
    template bool operator==(Hyperplane<float, 2> const&, Hyperplane<float, 2> const&);
    template bool operator!=(Hyperplane<float, 2> const&, Hyperplane<float, 2> const&);
    template bool operator< (Hyperplane<float, 2> const&, Hyperplane<float, 2> const&);
    template bool operator<=(Hyperplane<float, 2> const&, Hyperplane<float, 2> const&);
    template bool operator> (Hyperplane<float, 2> const&, Hyperplane<float, 2> const&);
    template bool operator>=(Hyperplane<float, 2> const&, Hyperplane<float, 2> const&);

    template class Hyperplane<double, 3>;
    template bool operator==(Hyperplane<double, 3> const&, Hyperplane<double, 3> const&);
    template bool operator!=(Hyperplane<double, 3> const&, Hyperplane<double, 3> const&);
    template bool operator< (Hyperplane<double, 3> const&, Hyperplane<double, 3> const&);
    template bool operator<=(Hyperplane<double, 3> const&, Hyperplane<double, 3> const&);
    template bool operator> (Hyperplane<double, 3> const&, Hyperplane<double, 3> const&);
    template bool operator>=(Hyperplane<double, 3> const&, Hyperplane<double, 3> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Hyperplane<Rational, 4>;
    template bool operator==(Hyperplane<Rational, 4> const&, Hyperplane<Rational, 4> const&);
    template bool operator!=(Hyperplane<Rational, 4> const&, Hyperplane<Rational, 4> const&);
    template bool operator< (Hyperplane<Rational, 4> const&, Hyperplane<Rational, 4> const&);
    template bool operator<=(Hyperplane<Rational, 4> const&, Hyperplane<Rational, 4> const&);
    template bool operator> (Hyperplane<Rational, 4> const&, Hyperplane<Rational, 4> const&);
    template bool operator>=(Hyperplane<Rational, 4> const&, Hyperplane<Rational, 4> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Hyperplane)
