#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Primitives/3D/Parallelepiped3.h>
#include <numeric>
using namespace gtl;

namespace gtl
{
    class UnitTestParallelepiped3
    {
    public:
        UnitTestParallelepiped3();

    private:
        void Test();
    };
}

UnitTestParallelepiped3::UnitTestParallelepiped3()
{
    UTInformation("Mathematics/Primitives/3D/Parallelepiped3");

    Test();
}

void UnitTestParallelepiped3::Test()
{
    Parallelepiped3<double> ppd{};
    UTAssert(IsZero(ppd.center), "Invalid center.");
    UTAssert(IsUnit(0, ppd.axis[0]), "Invalid axis[0].");
    UTAssert(IsUnit(1, ppd.axis[1]), "Invalid axis[1].");
    UTAssert(IsUnit(2, ppd.axis[2]), "Invalid axis[2].");

    std::array<Vector3<double>, 8> vertices{};
    ppd.GetVertices(vertices);
    UTAssert((vertices[0] == Vector3<double>{ -1.0, -1.0, -1.0 }), "Invalid vertices[0].");
    UTAssert((vertices[1] == Vector3<double>{ +1.0, -1.0, -1.0 }), "Invalid vertices[1].");
    UTAssert((vertices[2] == Vector3<double>{ -1.0, +1.0, -1.0 }), "Invalid vertices[2].");
    UTAssert((vertices[3] == Vector3<double>{ +1.0, +1.0, -1.0 }), "Invalid vertices[3].");
    UTAssert((vertices[4] == Vector3<double>{ -1.0, -1.0, +1.0 }), "Invalid vertices[4].");
    UTAssert((vertices[5] == Vector3<double>{ +1.0, -1.0, +1.0 }), "Invalid vertices[5].");
    UTAssert((vertices[6] == Vector3<double>{ -1.0, +1.0, +1.0 }), "Invalid vertices[6].");
    UTAssert((vertices[7] == Vector3<double>{ +1.0, +1.0, +1.0 }), "Invalid vertices[7].");

    ppd = Parallelepiped3<double>({ 1.0, 2.0, 0.0 }, { { { 1.0, 2.0, 3.0 }, { -2.0, 1.0, 5.0 }, { 1.0, 1.0, 2.0 } } });
    UTAssert((ppd.center == Vector3<double>{ 1.0, 2.0, 0.0 }), "Invalid center.");
    UTAssert((ppd.axis[0] == Vector3<double>{ 1.0, 2.0, 3.0 }), "Invalid axis[0].");
    UTAssert((ppd.axis[1] == Vector3<double>{ -2.0, 1.0, 5.0 }), "Invalid axis[1].");
    UTAssert((ppd.axis[2] == Vector3<double>{ 1.0, 1.0, 2.0 }), "Invalid axis[2].");

    ppd.GetVertices(vertices);
    UTAssert((vertices[0] == Vector3<double>{ 1.0, -2.0, -10.0 }), "Invalid vertices[0].");
    UTAssert((vertices[1] == Vector3<double>{ 3.0, 2.0, -4.0 }), "Invalid vertices[1].");
    UTAssert((vertices[2] == Vector3<double>{ -3.0, 0.0, 0.0 }), "Invalid vertices[2].");
    UTAssert((vertices[3] == Vector3<double>{ -1.0, 4.0, 6.0 }), "Invalid vertices[3].");
    UTAssert((vertices[4] == Vector3<double>{ 3.0, 0.0, -6.0 }), "Invalid vertices[4].");
    UTAssert((vertices[5] == Vector3<double>{ 5.0, 4.0, 0.0 }), "Invalid vertices[5].");
    UTAssert((vertices[6] == Vector3<double>{ -1.0, 2.0, 4.0 }), "Invalid vertices[6].");
    UTAssert((vertices[7] == Vector3<double>{ 1.0, 6.0, 10.0 }), "Invalid vertices[7].");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Primitives/3D/Parallelepiped3.h>

namespace gtl
{
    template class Parallelepiped3<float>;
    template class Parallelepiped3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class Parallelepiped3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(Parallelepiped3)
