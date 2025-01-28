#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/ND/Rectangle.h>
using namespace gtl;

namespace gtl
{
    class UnitTestRectangle
    {
    public:
        UnitTestRectangle();

    private:
        void Test();
    };
}

UnitTestRectangle::UnitTestRectangle()
{
    UTInformation("Mathematics/Primitives/ND/Rectangle");

    Test();
}

void UnitTestRectangle::Test()
{
    double maxError = 1e-14;

    Rectangle<double, 2> rect0{};
    UTAssert(IsZero(rect0.center), "Expecting zero center.");
    UTAssert(IsZero(rect0.axis[0]), "Expecting zero axis[0].");
    UTAssert(IsZero(rect0.axis[1]), "Expecting zero axis[1].");
    UTAssert(IsZero(rect0.extent), "Expecting zero extent.");

    Vector2<double> center{ 1.0, 2.0 };
    std::array<Vector2<double>, 2> axis =
    {
        Vector2<double>{ 3.0 / 5.0, 4.0 / 5.0 },
        Vector2<double>{ 4.0 / 5.0, -3.0 / 5.0 }
    };
    Vector2<double> extent{ 5.0, 7.0 };
    Rectangle<double, 2> rect1(center, axis, extent);
    UTAssert(rect1.center == center, "Incorrect center.");
    UTAssert(rect1.axis[0] == axis[0], "Incorrect axis[0].");
    UTAssert(rect1.axis[1] == axis[1], "Incorrect axis[1].");
    UTAssert(rect1.extent == extent, "Incorrect extent.");

    std::array<Vector2<double>, 4> vertices{};
    rect1.GetVertices(vertices);
    std::array<Vector2<double>, 4> expectedVertex{};
    expectedVertex[0] = { -7.6, 2.2 };
    expectedVertex[1] = { -1.6, 10.2 };
    expectedVertex[2] = { 3.6, -6.2 };
    expectedVertex[3] = { 9.6, 1.8 };
    for (std::size_t i = 0; i < 4; ++i)
    {
        double error = Length(vertices[i] - expectedVertex[i]);
        UTAssert(
            error <= maxError,
            "Box vertex " + std::to_string(i) + " is incorrect.");
    }

    bool result{};
    result = (rect0 == rect1);
    UTAssert(result == false, "operator== failed");
    result = (rect0 != rect1);
    UTAssert(result == true, "operator!= failed");
    result = (rect0 < rect1);
    UTAssert(result == true, "operator< failed");
    result = (rect0 <= rect1);
    UTAssert(result == true, "operator<= failed");
    result = (rect0 > rect1);
    UTAssert(result == false, "operator> failed");
    result = (rect0 >= rect1);
    UTAssert(result == false, "operator>= failed");
}
#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/ND/Rectangle.h>

namespace gtl
{
    template class Rectangle<float, 2>;
    template bool operator==(Rectangle<float, 2> const&, Rectangle<float, 2> const&);
    template bool operator!=(Rectangle<float, 2> const&, Rectangle<float, 2> const&);
    template bool operator< (Rectangle<float, 2> const&, Rectangle<float, 2> const&);
    template bool operator<=(Rectangle<float, 2> const&, Rectangle<float, 2> const&);
    template bool operator> (Rectangle<float, 2> const&, Rectangle<float, 2> const&);
    template bool operator>=(Rectangle<float, 2> const&, Rectangle<float, 2> const&);

    template class Rectangle<double, 3>;
    template bool operator==(Rectangle<double, 3> const&, Rectangle<double, 3> const&);
    template bool operator!=(Rectangle<double, 3> const&, Rectangle<double, 3> const&);
    template bool operator< (Rectangle<double, 3> const&, Rectangle<double, 3> const&);
    template bool operator<=(Rectangle<double, 3> const&, Rectangle<double, 3> const&);
    template bool operator> (Rectangle<double, 3> const&, Rectangle<double, 3> const&);
    template bool operator>=(Rectangle<double, 3> const&, Rectangle<double, 3> const&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Rectangle<Rational, 4>;
    template bool operator==(Rectangle<Rational, 4> const&, Rectangle<Rational, 4> const&);
    template bool operator!=(Rectangle<Rational, 4> const&, Rectangle<Rational, 4> const&);
    template bool operator< (Rectangle<Rational, 4> const&, Rectangle<Rational, 4> const&);
    template bool operator<=(Rectangle<Rational, 4> const&, Rectangle<Rational, 4> const&);
    template bool operator> (Rectangle<Rational, 4> const&, Rectangle<Rational, 4> const&);
    template bool operator>=(Rectangle<Rational, 4> const&, Rectangle<Rational, 4> const&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Rectangle)
