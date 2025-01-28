#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistTetrahedron3Tetrahedron3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistTetrahedron3Tetrahedron3
    {
    public:
        UnitTestDistTetrahedron3Tetrahedron3();

    private:
        using TTQuery = DCPQuery<double, Tetrahedron3<double>, Tetrahedron3<double>>;
        using TTOutput = TTQuery::Output;

        void Test();
    };
}

UnitTestDistTetrahedron3Tetrahedron3::UnitTestDistTetrahedron3Tetrahedron3()
{
    UTInformation("Mathematics/Distance/3D/DistTetrahedron3Tetrahedron3");

    Test();
}

void UnitTestDistTetrahedron3Tetrahedron3::Test()
{
    TTQuery query{};
    TTOutput output{};
    double const maxError = 1e-14;
    Vector3<double> diff{};

    Tetrahedron3<double> tetra0{}, tetra1{};

    tetra0.v[0] = { 0.0, 0.0, 0.0 };
    tetra0.v[1] = { 1.0, 0.0, 0.0 };
    tetra0.v[2] = { 0.0, 1.0, 0.0 };
    tetra0.v[3] = { 0.0, 0.0, 1.0 };

    tetra1.v[0] = { 0.0, 0.0, 0.0 };
    tetra1.v[1] = { 1.0, 0.0, 0.0 };
    tetra1.v[2] = { 0.0, 1.0, 0.0 };
    tetra1.v[3] = { 0.0, 0.0, 1.0 };

    Matrix3x3<double> scale{}, rotate{};
    Vector3<double> translate{};
    scale(0, 0) = 0.1;
    scale(1, 1) = 0.2;
    scale(2, 2) = 0.3;
    Vector3<double> axis{ 1.0, 1.0, 1.0 };
    Normalize(axis);
    double angle = 0.12345;
    AxisAngle<double> aa(axis, angle);
    RigidMotion<double>::Convert(aa, rotate);

    translate = { -4.0, -0.1, 0.0 };
    for (std::size_t i = 0; i < 4; ++i)
    {
        tetra1.v[i] = rotate * (scale * tetra1.v[i]) + translate;
    }
    output = query(tetra0, tetra1);
    UTAssert(std::fabs(output.distance - 3.9016132843136213) <= maxError,
        "Invalid distance.");
    UTAssert(std::fabs(output.sqrDistance - 15.222586220332524) <= maxError,
        "Invalid sqrDistance.");
    diff = output.closest[1] - output.closest[0];
    UTAssert(std::fabs(Length(diff) - output.distance) <= maxError,
        "Invalid closest points.");
    // distance = 3.9016132843136213
    // sqrDistance = 15.222586220332524
    // barycentric0 = {1,0,0,0}
    // barycentric1 = {0,1,0,0}
    // closest0 = {0,0,0}
    // closest1 = {-3.9005073519259006, -0.092637024626591333, -0.0068556234475078831}

    tetra1.v[0] = { 0.0, 0.0, 0.0 };
    tetra1.v[1] = { 1.0, 0.0, 0.0 };
    tetra1.v[2] = { 0.0, 1.0, 0.0 };
    tetra1.v[3] = { 0.0, 0.0, 1.0 };
    translate = { 4.0, -0.1, 0.0 };
    for (std::size_t i = 0; i < 4; ++i)
    {
        tetra1.v[i] = rotate * (scale * tetra1.v[i]) + translate;
    }
    output = query(tetra0, tetra1);
    output = query(tetra0, tetra1);
    UTAssert(std::fabs(output.distance - 2.9879651034441927) <= maxError,
        "Invalid distance.");
    UTAssert(std::fabs(output.sqrDistance - 8.9279354594002651) <= maxError,
        "Invalid sqrDistance.");
    diff = output.closest[1] - output.closest[0];
    UTAssert(std::fabs(Length(diff) - output.distance) <= maxError,
        "Invalid closest points.");
    // distance = 2.9879651034441927
    // sqrDistance = 8.9279354594002651
    // barycentric0 = {0,1,0,0}
    // barycentric1 = {0,0,1,0}
    // closest0 = {1,0,0}
    // closest1 = {3.9862887531049842, 0.098985296148198426, 0.014725950746817346}

    tetra1.v[0] = { 0.0, 0.0, 0.0 };
    tetra1.v[1] = { 1.0, 0.0, 0.0 };
    tetra1.v[2] = { 0.0, 1.0, 0.0 };
    tetra1.v[3] = { 0.0, 0.0, 1.0 };
    translate = { 1.0, 1.0, 1.0 };
    for (std::size_t i = 0; i < 4; ++i)
    {
        tetra1.v[i] = scale * tetra1.v[i] + translate;
    }
    output = query(tetra0, tetra1);
    UTAssert(std::fabs(output.distance - 1.1547005383792517) <= maxError,
        "Invalid distance.");
    UTAssert(std::fabs(output.sqrDistance - 1.3333333333333335) <= maxError,
        "Invalid sqrDistance.");
    diff = output.closest[1] - output.closest[0];
    UTAssert(std::fabs(Length(diff) - output.distance) <= maxError,
        "Invalid closest points.");
    // distance = 1.1547005383792517
    // sqrDistance = 1.3333333333333335
    // barycentric0 = {0,1/3,1/3,1/3}
    // barycentric1 = {1,0,0,0}
    // closest0 = {1/3,1/3,1/3}
    // closest1 = {1,1,1}

    // The tetrahedra overlap, so there are infinitely many pairs of closest
    // points, all pairs leading to zero distance. The query returns one of
    // them, which will be on an intersecting pair of triangle faces of the
    // tetrahedra.
    tetra1.v[0] = { 0.1, 0.2, 0.3 };
    tetra1.v[1] = { 0.7, 0.2, 0.3 };
    tetra1.v[2] = { 0.1, 0.8, 0.3 };
    tetra1.v[3] = { 0.1, 0.2, 0.9 };
    output = query(tetra0, tetra1);
    UTAssert(std::fabs(output.distance - 0.0) <= maxError,
        "Invalid distance.");
    UTAssert(std::fabs(output.sqrDistance - 0.0) <= maxError,
        "Invalid sqrDistance.");
    diff = output.closest[1] - output.closest[0];
    UTAssert(std::fabs(Length(diff) - output.distance) <= maxError,
        "Invalid closest points.");
    // distance = 0.0
    // sqrDistance = 0.0
    // barycentric0 = {0,0.5,0.2,0.3}
    // barycentric1 = {1/3,2/3,0,0}
    // closest0 = {0.5,0.2,0.3}
    // closest1 = {0.5,0.2,0.3}

    // Example from BouncingTetra physics sample.
    tetra0.v[0] = { -17.835952379688326, -10.155978659896217, 1.4303461871107781 };
    tetra0.v[1] = { -11.363647514038687, -7.8698514143044953, 4.2657291318029422 };
    tetra0.v[2] = { -10.911757383711524, -7.8819372907857126, 0.0017930696998706974 };
    tetra0.v[3] = { -12.911635062205210, -4.6196097536578851, 1.9365307293221552 };

    tetra1.v[0] = { -10.429970034251143, -9.7829521524051017, 1.0739046897289652 };
    tetra1.v[1] = { -7.9680239377073390, -5.5843154314016514, 9.0093143648846308 };
    tetra1.v[2] = { -12.062198864136873, -2.7639976997638085, 6.9671688996924743 };
    tetra1.v[3] = { -13.014639221870613, -7.3116128197308976, 9.6688114782373749 };

    output = query(tetra0, tetra1);
    UTAssert(std::fabs(output.distance - 0.0) <= maxError,
        "Invalid distance.");
    UTAssert(std::fabs(output.sqrDistance - 0.0) <= maxError,
        "Invalid sqrDistance.");
    diff = output.closest[1] - output.closest[0];
    UTAssert(std::fabs(Length(diff) - output.distance) <= maxError,
        "Invalid closest points.");
    // distance = 1.9860273225978185e-15
    // sqrDistance = 3.9443045261050590e-30
    // barycentric0 = {0,1,0,0}
    // barycentric1 = {0.56987736946039746,-6.4722551798074158e-17,0.18693793140984741,0.24318469912975518}
    // closest0 = {-11.363647514038687,-7.8698514143044953,4.2657291318029422}
    // closest1 = {-11.363647514038687,-7.8698514143044944,4.2657291318029404}
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistTetrahedron3Tetrahedron3.h>

namespace gtl
{
    template class DCPQuery<float, Tetrahedron3<float>, Tetrahedron3<float>>;
    template class DCPQuery<double, Tetrahedron3<double>, Tetrahedron3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Tetrahedron3<Rational>, Tetrahedron3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistTetrahedron3Tetrahedron3)
