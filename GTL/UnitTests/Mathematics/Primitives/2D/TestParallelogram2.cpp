#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/2D/Parallelogram2.h>
#include <numeric>
using namespace gtl;

namespace gtl
{
    class UnitTestParallelogram2
    {
    public:
        UnitTestParallelogram2();

    private:
        void Test();
    };
}

UnitTestParallelogram2::UnitTestParallelogram2()
{
    UTInformation("Mathematics/Primitives/2D/Parallelogram2");

    Test();
}

void UnitTestParallelogram2::Test()
{
    Parallelogram2<double> pgm{};
    UTAssert(IsZero(pgm.center), "Invalid center.");
    UTAssert(IsUnit(0, pgm.axis[0]), "Invalid axis[0].");
    UTAssert(IsUnit(1, pgm.axis[1]), "Invalid axis[1].");

    std::array<Vector2<double>, 4> vertices{};
    pgm.GetVertices(vertices);
    UTAssert((vertices[0] == Vector2<double>{ -1.0, -1.0 }), "Invalid vertices[0].");
    UTAssert((vertices[1] == Vector2<double>{ +1.0, -1.0 }), "Invalid vertices[1].");
    UTAssert((vertices[2] == Vector2<double>{ -1.0, +1.0 }), "Invalid vertices[2].");
    UTAssert((vertices[3] == Vector2<double>{ +1.0, +1.0 }), "Invalid vertices[3].");

    pgm = Parallelogram2<double>({ 1.0, 2.0 }, {{ { 1.0, 2.0}, { -2.0, 1.0} }});
    UTAssert((pgm.center == Vector2<double>{ 1.0, 2.0 }), "Invalid center.");
    UTAssert((pgm.axis[0] == Vector2<double>{ 1.0, 2.0 }), "Invalid axis[0].");
    UTAssert((pgm.axis[1] == Vector2<double>{ -2.0, 1.0}), "Invalid axis[1].");

    pgm.GetVertices(vertices);
    UTAssert((vertices[0] == Vector2<double>{ +2.0, -1.0 }), "Invalid vertices[0].");
    UTAssert((vertices[1] == Vector2<double>{ +4.0, +3.0 }), "Invalid vertices[1].");
    UTAssert((vertices[2] == Vector2<double>{ -2.0, +1.0 }), "Invalid vertices[2].");
    UTAssert((vertices[3] == Vector2<double>{  0.0, +5.0 }), "Invalid vertices[3].");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/2D/Parallelogram2.h>

namespace gtl
{
    template class Parallelogram2<float>;
    template class Parallelogram2<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Parallelogram2<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Parallelogram2)
