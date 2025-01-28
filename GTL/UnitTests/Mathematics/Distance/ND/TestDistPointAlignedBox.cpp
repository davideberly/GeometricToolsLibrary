#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/ND/DistPointAlignedBox.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistPointAlignedBox
    {
    public:
        UnitTestDistPointAlignedBox();

    private:
        using PB2Query = DCPQuery<double, Vector2<double>, AlignedBox2<double>>;
        using PB2Output = PB2Query::Output;
        using PB3Query = DCPQuery<double, Vector3<double>, AlignedBox3<double>>;
        using PB3Output = PB3Query::Output;

        void Test2D();
        void Test3D();

        template <std::size_t N>
        void Validate(
            typename DCPQuery<double, Vector<double, N>, AlignedBox<double, N>>::Output const& output,
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

UnitTestDistPointAlignedBox::UnitTestDistPointAlignedBox()
{
    UTInformation("Mathematics/Distance/ND/DistPointAlignedBox");

    Test2D();
    Test3D();
}

void UnitTestDistPointAlignedBox::Test2D()
{
    using DCPPointBoxQuery = DCPQuery<double, Vector<double, 2>, AlignedBox<double, 2>>;
    DCPPointBoxQuery query{};
    DCPPointBoxQuery::Output output{};
    AlignedBox<double, 2> box{};
    Vector<double, 2> point{};

    box.min = { 1.0, 1.0 };
    box.max = { 2.0, 3.0 };

    // inside box
    point = { 1.5, 2.0 };
    output = query(point, box);
    Validate(output, 0.0,
        point,
        point);

    // Voronoi region for <v0>.
    point = { 0.125, 0.125 };
    output = query(point, box);
    Validate(output, 1.53125,
        point,
        { 1.0, 1.0 });

    // Voronoi region for <v0, v1>.
    point = { 1.5, 0.125 };
    output = query(point, box);
    Validate(output, 0.765625,
        point,
        { 1.5, 1.0 });

    // Voronoi region for <v1>.
    point = { 2.125, 0.125 };
    output = query(point, box);
    Validate(output, 0.78125,
        point,
        { 2.0, 1.0 });

    // Voronoi region for <v1, v2>.
    point = { 2.125, 1.5 };
    output = query(point, box);
    Validate(output, 0.015625,
        point,
        { 2.0, 1.5 });

    // Voronoi region for <v2>.
    point = { 2.125, 3.125 };
    output = query(point, box);
    Validate(output, 0.03125,
        point,
        { 2.0, 3.0 });

    // Voronoi region for <v2, v3>.
    point = { 1.5, 3.125 };
    output = query(point, box);
    Validate(output, 0.015625,
        point,
        { 1.5, 3.0 });

    // Voronoi region for <v3>.
    point = { 0.875, 3.125 };
    output = query(point, box);
    Validate(output, 0.03125,
        point,
        { 1.0, 3.0 });

    // Voronoi region for <v3,v0>.
    point = { 0.875, 1.5 };
    output = query(point, box);
    Validate(output, 0.015625,
        point,
        { 1.0, 1.5 });
}

void UnitTestDistPointAlignedBox::Test3D()
{
    using DCPPointBoxQuery = DCPQuery<double, Vector<double, 3>, AlignedBox<double, 3>>;
    DCPPointBoxQuery query{};
    DCPPointBoxQuery::Output output{};
    AlignedBox<double, 3> box{};
    Vector<double, 3> point{};

    box.min = { 1.0, 1.0, 1.0 };
    box.max = { 2.0, 3.0, 4.0 };

    // inside box
    point = { 1.5, 2.0, 2.5 };
    output = query(point, box);
    Validate(output, 0.0,
        point,
        point);

    // Voronoi region <v000>.
    point = { 0.875, 0.875, 0.875 };
    output = query(point, box);
    Validate(output, 0.046875,
        point,
        { 1.0, 1.0, 1.0 });

    // Voronoi region <v000, v100>.
    point = { 1.5, 0.875, 0.875 };
    output = query(point, box);
    Validate(output, 0.03125,
        point,
        { 1.5, 1.0, 1.0 });

    // Voronoi region <v000, v100, v110, v010>.
    point = { 1.5, 1.5, 0.875 };
    output = query(point, box);
    Validate(output, 0.015625,
        point,
        { 1.5, 1.5, 1.0 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/ND/DistPointAlignedBox.h>

namespace gtl
{
    template class DCPQuery<float, Vector<float, 2>, AlignedBox<float, 2>>;
    template class DCPQuery<double, Vector<double, 3>, AlignedBox<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Vector<Rational, 4>, AlignedBox<Rational, 4>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPointAlignedBox)
