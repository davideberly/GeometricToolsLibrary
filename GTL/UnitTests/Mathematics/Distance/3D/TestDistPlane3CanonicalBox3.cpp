#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistPlane3CanonicalBox3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistPlane3CanonicalBox3
    {
    public:
        UnitTestDistPlane3CanonicalBox3();

    private:
        using PBQuery = DCPQuery<double, Plane3<double>, CanonicalBox3<double>>;
        using PBOutput = PBQuery::Output;

        void TestDoQuery3D();
        void TestDoQuery2D();
        void TestDoQuery1D();
        void TestDoQuery0D();

        // The expected values are passed to this function.
        void Validate(PBOutput const& output,
            double sqrDistance,
            Vector<double, 3> const& closest0,
            Vector<double, 3> const& closest1);
    };
}

UnitTestDistPlane3CanonicalBox3::UnitTestDistPlane3CanonicalBox3()
{
    UTInformation("Mathematics/Distance/3D/DistPlane3CanonicalBox3");

    TestDoQuery3D();
    TestDoQuery2D();
    TestDoQuery1D();
    TestDoQuery0D();
}

void UnitTestDistPlane3CanonicalBox3::Validate(PBOutput const& output,
    double sqrDistance,
    Vector<double, 3> const& closest0,
    Vector<double, 3> const& closest1)
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

void UnitTestDistPlane3CanonicalBox3::TestDoQuery3D()
{
    PBQuery query{};
    PBOutput output{};
    Vector3<double> planeOrigin{}, planeNormal{};
    Plane3<double> plane{};
    CanonicalBox3<double> box{};

    box.extent = { 1.5, 1.0, 0.5 };

    // DoQuery3D, block 0
    planeOrigin = { -2.5, -2.0, -1.5 };
    planeNormal = { 0.84799830400508802, 0.42399915200254401, 0.31799936400190804 };
    plane = Plane3<double>(planeNormal, planeOrigin);
    output = query(plane, box);
    Validate(output, 2.5280898876404518,
        { -2.8483146067415737, -1.6741573033707868, -1.0056179775280900 },
        { -1.5, -1.0, -0.5 });

    // DoQuery3D, block 1
    planeOrigin = { 2.5, 3.0, 3.5 };
    planeNormal = { 0.21199957600127201, 0.63599872800381607, 0.74199851600445199 };
    plane = Plane3<double>(planeNormal, planeOrigin);
    output = query(plane, box);
    Validate(output, 13.764044943820235,
        { 2.2865168539325844, 3.3595505617977537, 3.2528089887640457 },
        { 1.5, 1.0, 0.5 });

    // DoQuery3D, block 2
    planeOrigin = { 0.0, 0.0, 0.0 };
    planeNormal = { 1.0, 1.0, 1.0 };
    Normalize(planeNormal);
    plane = Plane3<double>(planeNormal, planeOrigin);
    output = query(plane, box);
    Validate(output, 0.0,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 });
}

void UnitTestDistPlane3CanonicalBox3::TestDoQuery2D()
{
    PBQuery query{};
    PBOutput output{};
    Vector3<double> planeOrigin{}, planeNormal{};
    Plane3<double> plane{};
    CanonicalBox3<double> box{};

    box.extent = { 1.5, 1.0, 0.5 };

    // DoQuery2D, block 0
    planeOrigin = { -2.5, -2.0, -1.5 };
    planeNormal = { 1.0, 1.0, 0.0 };
    Normalize(planeNormal);
    plane = Plane3<double>(planeNormal, planeOrigin);
    output = query(plane, box);
    Validate(output, 2.0,
        { -2.5, -2.0, 0.5 },
        { -1.5, -1.0, 0.5 });

    // DoQuery2D, block 1
    planeOrigin = { 2.5, 3.0, 3.5 };
    planeNormal = { 1.0, 1.0, 0.0 };
    Normalize(planeNormal);
    plane = Plane3<double>(planeNormal, planeOrigin);
    output = query(plane, box);
    Validate(output, 4.5,
        { 3.0, 2.5, 0.5 },
        { 1.5, 1.0, 0.5 });

    // DoQuery2D, block 2
    planeOrigin = { 0.0, 0.0, 0.0 };
    planeNormal = { 1.0, 1.0, 0.0 };
    Normalize(planeNormal);
    plane = Plane3<double>(planeNormal, planeOrigin);
    output = query(plane, box);
    Validate(output, 0.0,
        { 0.0, 0.0, 0.5 },
        { 0.0, 0.0, 0.5 });
}

void UnitTestDistPlane3CanonicalBox3::TestDoQuery1D()
{
    PBQuery query{};
    PBOutput output{};
    Vector3<double> planeOrigin{}, planeNormal{};
    Plane3<double> plane{};
    CanonicalBox3<double> box{};

    box.extent = { 1.5, 1.0, 0.5 };

    // DoQuery1D(0,1,2), clamp = min
    planeOrigin = { -2.5, -2.0, -1.5 };
    planeNormal = { 1.0, 0.0, 0.0 };
    plane = Plane3<double>(planeNormal, planeOrigin);
    output = query(plane, box);
    Validate(output, 1.0,
        { -2.5, 1.0, 0.5 },
        { -1.5, 1.0, 0.5 });

    // DoQuery1D(0,1,2), clamp = max
    planeOrigin = { 2.5, 3.0, 3.5 };
    planeNormal = { 1.0, 0.0, 0.0 };
    plane = Plane3<double>(planeNormal, planeOrigin);
    output = query(plane, box);
    Validate(output, 1.0,
        { 2.5, 1.0, 0.5 },
        { 1.5, 1.0, 0.5 });

    // DoQuery1D(0,1,2), min < clamp < max
    planeOrigin = { 0.0, 0.0, 0.0 };
    planeNormal = { 1.0, 0.0, 0.0 };
    plane = Plane3<double>(planeNormal, planeOrigin);
    output = query(plane, box);
    Validate(output, 0.0,
        { 0.0, 1.0, 0.5 },
        { 0.0, 1.0, 0.5 });

    // DoQuery1D(1,2,0), clamp = min
    planeOrigin = { -2.5, -2.0, -1.5 };
    planeNormal = { 0.0, 1.0, 0.0 };
    plane = Plane3<double>(planeNormal, planeOrigin);
    output = query(plane, box);
    Validate(output, 1.0,
        { 1.5, -2.0, 0.5 },
        { 1.5, -1.0, 0.5 });

    // DoQuery1D(1,2,0), clamp = max
    planeOrigin = { 2.5, 3.0, 3.5 };
    planeNormal = { 0.0, 1.0, 0.0 };
    plane = Plane3<double>(planeNormal, planeOrigin);
    output = query(plane, box);
    Validate(output, 4.0,
        { 1.5, 3.0, 0.5 },
        { 1.5, 1.0, 0.5 });

    // DoQuery1D(1,2,0), min < clamp < max
    planeOrigin = { 0.0, 0.0, 0.0 };
    planeNormal = { 0.0, 1.0, 0.0 };
    plane = Plane3<double>(planeNormal, planeOrigin);
    output = query(plane, box);
    Validate(output, 0.0,
        { 1.5, 0.0, 0.5 },
        { 1.5, 0.0, 0.5 });

    // DoQuery1D(2,0,1), clamp = min
    planeOrigin = { -2.5, -2.0, -1.5 };
    planeNormal = { 0.0, 0.0, 1.0 };
    plane = Plane3<double>(planeNormal, planeOrigin);
    output = query(plane, box);
    Validate(output, 1.0,
        { 1.5, 1.0, -1.5 },
        { 1.5, 1.0, -0.5 });

    // DoQuery1D(2,0,1), clamp = max
    planeOrigin = { 2.5, 3.0, 3.5 };
    planeNormal = { 0.0, 0.0, 1.0 };
    plane = Plane3<double>(planeNormal, planeOrigin);
    output = query(plane, box);
    Validate(output, 9.0,
        { 1.5, 1.0, 3.5 },
        { 1.5, 1.0, 0.5 });

    // DoQuery1D(2,0,1), min < clamp < max
    planeOrigin = { 0.0, 0.0, 0.0 };
    planeNormal = { 0.0, 0.0, 1.0 };
    plane = Plane3<double>(planeNormal, planeOrigin);
    output = query(plane, box);
    Validate(output, 0.0,
        { 1.5, 1.0, 0.0 },
        { 1.5, 1.0, 0.0 });
}

void UnitTestDistPlane3CanonicalBox3::TestDoQuery0D()
{
    PBQuery query{};
    PBOutput output{};
    Vector3<double> planeOrigin{}, planeNormal{};
    Plane3<double> plane{};
    CanonicalBox3<double> box{};

    box.extent = { 1.5, 1.0, 0.5 };

    planeOrigin = { -2.5, -2.0, -1.5 };
    planeNormal = { 0.0, 0.0, 0.0 };
    plane = Plane3<double>(planeNormal, planeOrigin);
    output = query(plane, box);
    Validate(output, 3.0,
        { -2.5, -2.0, -1.5 },
        { -1.5, -1.0, -0.5 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistPlane3CanonicalBox3.h>

namespace gtl
{
    template class DCPQuery<float, Plane3<float>, CanonicalBox<float, 3>>;
    template class DCPQuery<double, Plane3<double>, CanonicalBox<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Plane3<Rational>, CanonicalBox<Rational, 3>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPlane3CanonicalBox3)
