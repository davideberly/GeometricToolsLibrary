#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/ND/DistPointCanonicalBox.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistPointCanonicalBox
    {
    public:
        UnitTestDistPointCanonicalBox();

    private:
        using PB2Query = DCPQuery<double, Vector2<double>, CanonicalBox2<double>>;
        using PB2Output = PB2Query::Output;
        using PB3Query = DCPQuery<double, Vector3<double>, CanonicalBox3<double>>;
        using PB3Output = PB3Query::Output;

        void Test2D();
        void Test3D();

        template <std::size_t N>
        void Validate(
            typename DCPQuery<double, Vector<double, N>, CanonicalBox<double, N>>::Output const& output,
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

UnitTestDistPointCanonicalBox::UnitTestDistPointCanonicalBox()
{
    UTInformation("Mathematics/Distance/ND/DistPointCanonicalBox");

    Test2D();
    Test3D();
}

void UnitTestDistPointCanonicalBox::Test2D()
{
    using DCPPointBoxQuery = DCPQuery<double, Vector<double, 2>, CanonicalBox<double, 2>>;
    DCPPointBoxQuery query{};
    DCPPointBoxQuery::Output output{};
    CanonicalBox<double, 2> box{};
    Vector<double, 2> point{};

    box.extent = { 0.5, 1.0 };

    // inside box
    point = { 0.0, 0.0 };
    output = query(point, box);
    Validate(output, 0.0,
        point,
        point);

    // Voronoi region for <v0>.
    point = { -1.375, -1.875 };
    output = query(point, box);
    Validate(output, 1.53125,
        point,
        { -0.5, -1.0 });

    // Voronoi region for <v0, v1>.
    point = { 0.0, -1.875 };
    output = query(point, box);
    Validate(output, 0.765625,
        point,
        { 0.0, -1.0 });

    // Voronoi region for <v1>.
    point = { 0.625, -1.875 };
    output = query(point, box);
    Validate(output, 0.78125,
        point,
        { 0.5, -1.0 });

    // Voronoi region for <v1, v2>.
    point = { 0.625, -0.5 };
    output = query(point, box);
    Validate(output, 0.015625,
        point,
        { 0.5, -0.5 });

    // Voronoi region for <v2>.
    point = { 0.625, 1.125 };
    output = query(point, box);
    Validate(output, 0.03125,
        point,
        { 0.5, 1.0 });

    // Voronoi region for <v2, v3>.
    point = { 0.0, 1.125 };
    output = query(point, box);
    Validate(output, 0.015625,
        point,
        { 0.0, 1.0 });

    // Voronoi region for <v3>.
    point = { -0.625, 1.125 };
    output = query(point, box);
    Validate(output, 0.03125,
        point,
        { -0.5, 1.0 });

    // Voronoi region for <v3,v0>.
    point = { -0.625, -0.5 };
    output = query(point, box);
    Validate(output, 0.015625,
        point,
        { -0.5, -0.5 });
}

void UnitTestDistPointCanonicalBox::Test3D()
{
    using DCPPointBoxQuery = DCPQuery<double, Vector<double, 3>, CanonicalBox<double, 3>>;
    DCPPointBoxQuery query{};
    DCPPointBoxQuery::Output output{};
    CanonicalBox<double, 3> box{};
    Vector<double, 3> point{};

    box.extent = { 0.5, 1.0, 1.5 };

    // inside box
    point = { 0.0, 0.0, 0.0 };
    output = query(point, box);
    Validate(output, 0.0,
        point,
        point);

    // Voronoi region <v000>.
    point = { -0.625, -1.125, -1.625 };
    output = query(point, box);
    Validate(output, 0.046875,
        point,
        { -0.5, -1.0, -1.5 });

    // Voronoi region <v000, v100>.
    point = { 0.0, -1.125, -1.625 };
    output = query(point, box);
    Validate(output, 0.03125,
        point,
        { 0.0, -1.0, -1.5 });

    // Voronoi region <v000, v100, v110, v010>.
    point = { 0.0, -0.5, -1.625 };
    output = query(point, box);
    Validate(output, 0.015625,
        point,
        { 0.0, -0.5,- 1.5 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/ND/DistPointCanonicalBox.h>

namespace gtl
{
    template class DCPQuery<float, Vector<float, 2>, CanonicalBox<float, 2>>;
    template class DCPQuery<double, Vector<double, 3>, CanonicalBox<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Vector<Rational, 4>, CanonicalBox<Rational, 4>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPointCanonicalBox)
