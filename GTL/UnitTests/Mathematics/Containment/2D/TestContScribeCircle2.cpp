#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Containment/2D/ContScribeCircle2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestContScribeCircle2
    {
    public:
        UnitTestContScribeCircle2();

    private:
        void TestCircumscribe();
        void TestInscribe();
    };
}

UnitTestContScribeCircle2::UnitTestContScribeCircle2()
{
    UTInformation("Mathematics/Containment/2D/ContScribeCircle2");

    TestCircumscribe();
    TestInscribe();
}

void UnitTestContScribeCircle2::TestCircumscribe()
{
    Vector2<double> v0{ 0.0, 0.0 };
    Vector2<double> v1{ 1.0, 0.0 };
    Vector2<double> v2{ 0.0, 1.0 };

    Circle2<double> circle;
    bool success = Circumscribe(v0, v1, v2, circle);
    UTAssert(success, "Circumscribe failed for noncollinear.");

    v0 = { -1.0, 0.0 };
    v1 = { 0.0, 0.0 };
    v2 = { +1.0, 0.0 };
    success = Circumscribe(v0, v1, v2, circle);
    UTAssert(!success, "Circumscribe failed for collinear.");
}

void UnitTestContScribeCircle2::TestInscribe()
{
    Vector2<double> v0{ 0.0, 0.0 };
    Vector2<double> v1{ 1.0, 0.0 };
    Vector2<double> v2{ 0.0, 1.0 };

    Circle2<double> circle;
    bool success = Inscribe(v0, v1, v2, circle);
    UTAssert(success, "Inscribe failed for noncollinear.");

    v0 = { -1.0, 0.0f };
    v1 = { 0.0, 0.0 };
    v2 = { +1.0, 0.0f };
    success = Inscribe(v0, v1, v2, circle);
    UTAssert(!success, "Inscribe failed for collinear.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Containment/2D/ContScribeCircle2.h>

namespace gtl
{
    template bool Circumscribe(Vector2<float> const&, Vector2<float> const&,
        Vector2<float> const&, Circle2<float>&);

    template bool Inscribe(Vector2<float> const&, Vector2<float> const&,
        Vector2<float> const&, Circle2<float>&);

    template bool Circumscribe(Vector2<double> const&, Vector2<double> const&,
        Vector2<double> const&, Circle2<double>&);

    template bool Inscribe(Vector2<double> const&, Vector2<double> const&,
        Vector2<double> const&, Circle2<double>&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;

    template bool Circumscribe(Vector2<Rational> const&, Vector2<Rational> const&,
        Vector2<Rational> const&, Circle2<Rational>&);

    template bool Inscribe(Vector2<Rational> const&, Vector2<Rational> const&,
        Vector2<Rational> const&, Circle2<Rational>&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ContScribeCircle2)
