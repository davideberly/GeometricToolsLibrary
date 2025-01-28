#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/ND/Line.h>
using namespace gtl;

namespace gtl
{
    class UnitTestLine
    {
    public:
        UnitTestLine();

    private:
        void Test();
    };
}

UnitTestLine::UnitTestLine()
{
    UTInformation("Mathematics/Primitives/ND/Line");

    Test();
}

void UnitTestLine::Test()
{
    Line2<double> line0{};
    UTAssert(IsZero(line0.origin), "Expecting zero origin.");
    UTAssert(IsZero(line0.direction), "Expecting zero direction.");

    Vector2<double> origin{ 1.0, 2.0 };
    Vector2<double> direction{ 0.2, 0.1 };
    Normalize(direction);
    Line2<double> line1(origin, direction);
    UTAssert(line1.origin == origin, "Incorrect origin.");
    UTAssert(line1.direction == direction, "Incorrect direction.");

    bool result{};
    result = (line0 == line1);
    UTAssert(result == false, "operator== failed");
    result = (line0 != line1);
    UTAssert(result == true, "operator!= failed");
    result = (line0 < line1);
    UTAssert(result == true, "operator< failed");
    result = (line0 <= line1);
    UTAssert(result == true, "operator<= failed");
    result = (line0 > line1);
    UTAssert(result == false, "operator> failed");
    result = (line0 >= line1);
    UTAssert(result == false, "operator>= failed");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/ND/Line.h>

namespace gtl
{
    template class Line<float, 2>;
    template bool operator==(Line<float, 2> const&, Line<float, 2> const&);
    template bool operator!=(Line<float, 2> const&, Line<float, 2> const&);
    template bool operator< (Line<float, 2> const&, Line<float, 2> const&);
    template bool operator<=(Line<float, 2> const&, Line<float, 2> const&);
    template bool operator> (Line<float, 2> const&, Line<float, 2> const&);
    template bool operator>=(Line<float, 2> const&, Line<float, 2> const&);

    template class Line<double, 3>;
    template bool operator==(Line<double, 3> const&, Line<double, 3> const&);
    template bool operator!=(Line<double, 3> const&, Line<double, 3> const&);
    template bool operator< (Line<double, 3> const&, Line<double, 3> const&);
    template bool operator<=(Line<double, 3> const&, Line<double, 3> const&);
    template bool operator> (Line<double, 3> const&, Line<double, 3> const&);
    template bool operator>=(Line<double, 3> const&, Line<double, 3> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Line<Rational, 4>;
    template bool operator==(Line<Rational, 4> const&, Line<Rational, 4> const&);
    template bool operator!=(Line<Rational, 4> const&, Line<Rational, 4> const&);
    template bool operator< (Line<Rational, 4> const&, Line<Rational, 4> const&);
    template bool operator<=(Line<Rational, 4> const&, Line<Rational, 4> const&);
    template bool operator> (Line<Rational, 4> const&, Line<Rational, 4> const&);
    template bool operator>=(Line<Rational, 4> const&, Line<Rational, 4> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Line)
