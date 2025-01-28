#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistOrientedBox3Cone3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistOrientedBox3Cone3
    {
    public:
        UnitTestDistOrientedBox3Cone3();

    private:
        using BCQuery = DCPQuery<double, OrientedBox3<double>, Cone3<double>>;
        using BCOutput = typename BCQuery::Output;

        void Test();
    };
}

UnitTestDistOrientedBox3Cone3::UnitTestDistOrientedBox3Cone3()
{
    UTInformation("Mathematics/Distance/3D/DistOrientedBox3Cone3 (tested by GTE/VisualTests/Mathematcs/Distance/DistanceOrientedBoxCone)");

    // The single test here is to ensure I did not break the code. No code
    // changes (other than cosmetic ones) were made in the port to GTL.
    Test();
}

void UnitTestDistOrientedBox3Cone3::Test()
{
    BCQuery query{};
    BCOutput output{};
    double const maxError = 1e-14;

    Vector3<double> vertex{ -1.0, -1.0, -1.0 };
    Vector3<double> direction{ 1.0f, 2.0f, 3.0f };
    Normalize(direction);
    double angle = 0.6;
    double hmin = 0.5;
    double hmax = 3.0;
    Cone3<double> cone(vertex, direction, angle, hmin, hmax);

    OrientedBox3<double> box{};
    box.center = vertex + 0.5 * (hmin + hmax) * direction + Vector3<double>{ 4.0, -2.0, 0.0 };
    Quaternion<double> q{ 4.0f, 3.0f, 2.0f, 1.0f };
    Normalize(q);
    Matrix3x3<double> rotate{};
    RigidMotion<double>::Convert(q, rotate);
    box.axis[0] = rotate.GetCol(0);
    box.axis[1] = rotate.GetCol(1);
    box.axis[2] = rotate.GetCol(2);
    box.extent = { 0.75, 0.5, 0.25 };

    output = query(box, cone);
    UTAssert(std::fabs(output.distance - 1.8963282107287027) <= maxError,
        "Incorrect distance.");
    Vector3<double> expected0{ 3.0510405066800761, -1.2312523199731820, 0.48645485337356126 };
    Vector3<double> expected1{ 1.7053430457187218,-0.15264106835080704, 1.2749704171015708 };
    UTAssert(Length(output.closest[0] - expected0) <= maxError,
        "Incorrect closest0.");
    UTAssert(Length(output.closest[1] - expected1) <= maxError,
        "Incorrect closest1.");
}

#else

#include <GTL/Mathematics/Distance/3D/DistOrientedBox3Cone3.h>

namespace gtl
{
    template class DCPQuery<float, OrientedBox3<float>, Cone3<float>>;
    template class DCPQuery<double, OrientedBox3<double>, Cone3<double>>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistOrientedBox3Cone3)
