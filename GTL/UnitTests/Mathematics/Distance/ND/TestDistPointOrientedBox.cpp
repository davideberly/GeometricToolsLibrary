#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/ND/DistPointOrientedBox.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistPointOrientedBox
    {
    public:
        UnitTestDistPointOrientedBox();

    private:
        using PB2Query = DCPQuery<double, Vector2<double>, OrientedBox2<double>>;
        using PB2Output = PB2Query::Output;
        using PB3Query = DCPQuery<double, Vector3<double>, OrientedBox3<double>>;
        using PB3Output = PB3Query::Output;

        void Test2D();
        void Test3D();

        template <std::size_t N>
        void Validate(
            typename DCPQuery<double, Vector<double, N>, OrientedBox<double, N>>::Output const& output,
            double sqrDistance,
            Vector<double, N> const& closest0,
            Vector<double, N> const& closest1)
        {
            double constexpr maxError = 1e-14;

            double error = std::fabs(output.sqrDistance - sqrDistance);
            UTAssert(
                error <= maxError,
                "Invalid DCPQuery.");
            error = std::fabs(output.distance - std::sqrt(sqrDistance));
            UTAssert(
                error <= maxError,
                "Invalid DCPQuery.");
            error = Length(output.closest[0] - closest0);
            UTAssert(
                error <= maxError,
                "Invalid DCPQuery.");
            error = Length(output.closest[1] - closest1);
            UTAssert(
                error <= maxError,
                "Invalid DCPQuery.");
        }
    };
}

UnitTestDistPointOrientedBox::UnitTestDistPointOrientedBox()
{
    UTInformation("Mathematics/Distance/ND/DistPointOrientedBox");
    
    // The tests are the same as for UnitTestDistPointAlignedBox, but the
    // aligned box is rotated and translate to form the oriented box.
    Test2D();
    Test3D();
}

void UnitTestDistPointOrientedBox::Test2D()
{
    using DCPPointBoxQuery = DCPQuery<double, Vector<double, 2>, OrientedBox<double, 2>>;
    DCPPointBoxQuery query{};
    DCPPointBoxQuery::Output output{};
    OrientedBox<double, 2> box{};
    Vector<double, 2> point{}, xfrmPoint{};

    box.center = { 1.5, 2.0 };  // = (aligned.max + aligned.min) / 2
    box.extent = { 0.5, 1.0 };  // = (aligned.max - aligned.min) / 2
    box.axis[0] = { 1.0, 0.0 };
    box.axis[1] = { 0.0, 1.0 };

    Vector<double, 2> translate{ 0.1234, 5.6789 };
    Matrix2x2<double> rotate{};
    double const angle = 0.98765;
    RigidMotion<double>::Convert(angle, rotate);
    box.center = rotate * box.center + translate;
    box.axis[0] = rotate * box.axis[0];
    box.axis[1] = rotate * box.axis[1];

    // inside box
    point = { 1.5, 2.0 };
    xfrmPoint = rotate * point + translate;
    output = query(xfrmPoint, box);
    Validate(output, 0.0,
        xfrmPoint,
        xfrmPoint);

    // Voronoi region for <v0>.
    point = { 0.125, 0.125 };
    xfrmPoint = rotate * point + translate;
    output = query(xfrmPoint, box);
    Validate(output, 1.53125,
        xfrmPoint,
        rotate * Vector<double, 2>{ 1.0, 1.0 } + translate);

    // Voronoi region for <v0, v1>.
    point = { 1.5, 0.125 };
    xfrmPoint = rotate * point + translate;
    output = query(xfrmPoint, box);
    Validate(output, 0.765625,
        xfrmPoint,
        rotate * Vector<double, 2>{ 1.5, 1.0 } + translate);

    // Voronoi region for <v1>.
    point = { 2.125, 0.125 };
    xfrmPoint = rotate * point + translate;
    output = query(xfrmPoint, box);
    Validate(output, 0.78125,
        xfrmPoint,
        rotate * Vector<double, 2>{ 2.0, 1.0 } + translate);

    // Voronoi region for <v1, v2>.
    point = { 2.125, 1.5 };
    xfrmPoint = rotate * point + translate;
    output = query(xfrmPoint, box);
    Validate(output, 0.015625,
        xfrmPoint,
        rotate * Vector<double, 2>{ 2.0, 1.5 } + translate);

    // Voronoi region for <v2>.
    point = { 2.125, 3.125 };
    xfrmPoint = rotate * point + translate;
    output = query(xfrmPoint, box);
    Validate(output, 0.03125,
        xfrmPoint,
        rotate * Vector<double, 2>{ 2.0, 3.0 } + translate);

    // Voronoi region for <v2, v3>.
    point = { 1.5, 3.125 };
    xfrmPoint = rotate * point + translate;
    output = query(xfrmPoint, box);
    Validate(output, 0.015625,
        xfrmPoint,
        rotate * Vector<double, 2>{ 1.5, 3.0 } + translate);

    // Voronoi region for <v3>.
    point = { 0.875, 3.125 };
    xfrmPoint = rotate * point + translate;
    output = query(xfrmPoint, box);
    Validate(output, 0.03125,
        xfrmPoint,
        rotate * Vector<double, 2>{ 1.0, 3.0 } + translate);

    // Voronoi region for <v3,v0>.
    point = { 0.875, 1.5 };
    xfrmPoint = rotate * point + translate;
    output = query(xfrmPoint, box);
    Validate(output, 0.015625,
        xfrmPoint,
        rotate * Vector<double, 2>{ 1.0, 1.5 } + translate);
}

void UnitTestDistPointOrientedBox::Test3D()
{
    using DCPPointBoxQuery = DCPQuery<double, Vector<double, 3>, OrientedBox<double, 3>>;
    DCPPointBoxQuery query{};
    DCPPointBoxQuery::Output output{};
    OrientedBox<double, 3> box{};
    Vector<double, 3> point{}, xfrmPoint{};

    box.center = { 1.5, 2.0, 2.5 };  // = (aligned.max + aligned.min) / 2
    box.extent = { 0.5, 1.0, 1.5 };  // = (aligned.max - aligned.min) / 2
    box.axis[0] = { 1.0, 0.0, 0.0 };
    box.axis[1] = { 0.0, 1.0, 0.0 };
    box.axis[2] = { 0.0, 0.0, 1.0 };

    Vector<double, 3> translate{ 0.1234, 5.6789, -1.9735 };
    Matrix3x3<double> rotate{};
    Quaternion<double> q(1.0, 2.0, 3.0, 4.0);
    Normalize(q);
    RigidMotion<double>::Convert(q, rotate);
    box.center = rotate * box.center + translate;
    box.axis[0] = rotate * box.axis[0];
    box.axis[1] = rotate * box.axis[1];
    box.axis[2] = rotate * box.axis[2];

    // inside box
    point = { 1.5, 2.0, 2.5 };
    xfrmPoint = rotate * point + translate;
    output = query(xfrmPoint, box);
    Validate(output, 0.0,
        xfrmPoint,
        xfrmPoint);

    // Voronoi region <v000>.
    point = { 0.875, 0.875, 0.875 };
    xfrmPoint = rotate * point + translate;
    output = query(xfrmPoint, box);
    Validate(output, 0.046875,
        xfrmPoint,
        rotate * Vector<double, 3>{ 1.0, 1.0, 1.0 } + translate);

    // Voronoi region <v000, v100>.
    point = { 1.5, 0.875, 0.875 };
    xfrmPoint = rotate * point + translate;
    output = query(xfrmPoint, box);
    Validate(output, 0.03125,
        xfrmPoint,
        rotate * Vector<double, 3>{ 1.5, 1.0, 1.0 } + translate);

    // Voronoi region <v000, v100, v110, v010>.
    point = { 1.5, 1.5, 0.875 };
    xfrmPoint = rotate * point + translate;
    output = query(xfrmPoint, box);
    Validate(output, 0.015625,
        xfrmPoint,
        rotate * Vector<double, 3>{ 1.5, 1.5, 1.0 } + translate);
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/ND/DistPointOrientedBox.h>

namespace gtl
{
    template class DCPQuery<float, Vector<float, 2>, OrientedBox<float, 2>>;
    template class DCPQuery<double, Vector<double, 3>, OrientedBox<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Vector<Rational, 4>, OrientedBox<Rational, 4>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPointOrientedBox)
