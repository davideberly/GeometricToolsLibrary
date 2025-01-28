#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistCircle3Circle3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistCircle3Circle3
    {
    public:
        UnitTestDistCircle3Circle3();

    private:
        template <typename T>
        using CCQuery = DCPQuery<T, Circle3<T>, Circle3<T>>;

        template <typename T>
        using CCOutput = typename CCQuery<T>::Output;

        void Test();

        template <typename T>
        void Validate(CCOutput<T> const& output,
            T const& sqrDistance, std::size_t numClosestPairs,
            std::array<Vector3<T>, 2> const& circle0Closest,
            std::array<Vector3<T>, 2> const& circle1Closest,
            bool equidistant, T const& maxError);
    };
}

UnitTestDistCircle3Circle3::UnitTestDistCircle3Circle3()
{
    UTInformation("Mathematics/Distance/3D/DistCircle3Circle3");

    Test();
}

template <typename T>
void UnitTestDistCircle3Circle3::Validate(CCOutput<T> const& output,
    T const& sqrDistance, std::size_t numClosestPairs,
    std::array<Vector3<T>, 2> const& circle0Closest,
    std::array<Vector3<T>, 2> const& circle1Closest,
    bool equidistant, T const& maxError)
{
    T error = std::fabs(output.sqrDistance - sqrDistance);
    UTAssert(error <= maxError,
        "incorrect squared distance");

    error = std::fabs(output.distance - std::sqrt(sqrDistance));
    UTAssert(error <= maxError,
        "incorrect squared distance");

    UTAssert(output.numClosestPairs == numClosestPairs,
        "incorrect number");

    error = Length(output.circle0Closest[0] - circle0Closest[0]);
    UTAssert(error <= maxError,
        "incorrect circle0Closest[0]");

    error = Length(output.circle0Closest[1] - circle0Closest[1]);
    UTAssert(error <= maxError,
        "incorrect circle0Closest[1]");

    error = Length(output.circle1Closest[0] - circle1Closest[0]);
    UTAssert(error <= maxError,
        "incorrect circle1Closest[0]");

    error = Length(output.circle1Closest[1] - circle1Closest[1]);
    UTAssert(error <= maxError,
        "incorrect circle1Closest[1");

    UTAssert(output.equidistant == equidistant,
        "incorrect equidistant");
}

void UnitTestDistCircle3Circle3::Test()
{
    DCPQuery<float, Circle3<float>, Circle3<float>> query{};
    DCPQuery<float, Circle3<float>, Circle3<float>>::Output output{};
    Circle3<float> circle0{}, circle1{};
    std::array<Vector3<float>, 2> expected0Closest{}, expected1Closest{};
    float const maxError = 1e-06f;

    circle0.center = { 0.0f, 0.0f, 0.0f };
    circle0.radius = 1.0f;
    circle0.normal = { 0.0f, 0.0f, 1.0f };

    // Circles are equal.
    circle1.center = { 0.0f, 0.0f, 0.0f };
    circle1.radius = 1.0f;
    circle1.normal = { 0.0f, 0.0f, 1.0f };
    output = query(circle0, circle1);
    expected0Closest[0] = { -1.0f, 0.0f, 0.0f };
    expected0Closest[1] = { 0.0f, 0.0f, 0.0f };
    expected1Closest[0] = { -1.0f, 0.0f, 0.0f };
    expected1Closest[1] = { 0.0f, 0.0f, 0.0f };
    Validate(output, 0.0f, 1, expected0Closest, expected1Closest, true, maxError);

    output = query(circle1, circle0);
    expected0Closest[0] = { -1.0f, 0.0f, 0.0f };
    expected0Closest[1] = { 0.0f, 0.0f, 0.0f };
    expected1Closest[0] = { -1.0f, 0.0f, 0.0f };
    expected1Closest[1] = { 0.0f, 0.0f, 0.0f };
    Validate(output, 0.0f, 1, expected0Closest, expected1Closest, true, maxError);

    // Concentric circles in the same plane but different radii.
    circle1.center = { 0.0f, 0.0f, 0.0f };
    circle1.radius = 2.0f;
    circle1.normal = { 0.0f, 0.0f, 1.0f };
    output = query(circle0, circle1);
    expected0Closest[0] = { -1.0f, 0.0f, 0.0f };
    expected0Closest[1] = { 0.0f, 0.0f, 0.0f };
    expected1Closest[0] = { -2.0f, 0.0f, 0.0f };
    expected1Closest[1] = { 0.0f, 0.0f, 0.0f };
    Validate(output, 1.0f, 1, expected0Closest, expected1Closest, true, maxError);

    // The order of the circles is not relevant.
    output = query(circle1, circle0);
    expected0Closest[0] = { -2.0f, 0.0f, 0.0f };
    expected0Closest[1] = { 0.0f, 0.0f, 0.0f };
    expected1Closest[0] = { -1.0f, 0.0f, 0.0f };
    expected1Closest[1] = { 0.0f, 0.0f, 0.0f };
    Validate(output, 1.0f, 1, expected0Closest, expected1Closest, true, maxError);

    // Circles in the same plane but different centers (separated).
    circle1.center = { 4.0f, 0.0f, 0.0f };
    circle1.radius = 2.0f;
    circle1.normal = { 0.0f, 0.0f, 1.0f };
    output = query(circle0, circle1);
    expected0Closest[0] = { 1.0f, 0.0f, 0.0f };
    expected0Closest[1] = { 0.0f, 0.0f, 0.0f };
    expected1Closest[0] = { 2.0f, 0.0f, 0.0f };
    expected1Closest[1] = { 0.0f, 0.0f, 0.0f };
    Validate(output, 1.0f, 1, expected0Closest, expected1Closest, false, maxError);

    output = query(circle1, circle0);
    expected0Closest[0] = { 2.0f, 0.0f, 0.0f };
    expected0Closest[1] = { 0.0f, 0.0f, 0.0f };
    expected1Closest[0] = { 1.0f, 0.0f, 0.0f };
    expected1Closest[1] = { 0.0f, 0.0f, 0.0f };
    Validate(output, 1.0f, 1, expected0Closest, expected1Closest, false, maxError);

    // Circles in the same plane but different centers (containment).
    circle1.center = { 0.25f, 0.0f, 1.0f };
    circle1.radius = 0.125f;
    circle1.normal = { 0.0f, 0.0f, 1.0f };
    output = query(circle0, circle1);
    expected0Closest[0] = { 1.0f, 0.0f, 0.0f };
    expected0Closest[1] = { 0.0f, 0.0f, 0.0f };
    expected1Closest[0] = { 0.375f, 0.0f, 1.0f };
    expected1Closest[1] = { 0.0f, 0.0f, 0.0f };
    Validate(output, 1.390625f, 1, expected0Closest, expected1Closest, false, maxError);

    output = query(circle1, circle0);
    expected0Closest[0] = { 0.375f, 0.0f, 1.0f };
    expected0Closest[1] = { 0.0f, 0.0f, 0.0f };
    expected1Closest[0] = { 1.0f, 0.0f, 0.0f };
    expected1Closest[1] = { 0.0f, 0.0f, 0.0f };
    Validate(output, 1.390625f, 1, expected0Closest, expected1Closest, false, maxError);

    // Circles in the same plane but different centers (overlap).
    circle1.center = { 1.0f, 0.0f, 1.0f };
    circle1.radius = 1.25f;
    circle1.normal = { 0.0f, 0.0f, 1.0f };
    output = query(circle0, circle1);
    expected0Closest[0] = { 0.21875f, +0.975780964f, 0.0f };
    expected0Closest[1] = { 0.21875f, -0.975780964f, 0.0f };
    expected1Closest[0] = { 0.21875f, +0.975780964f, 1.0f };
    expected1Closest[1] = { 0.21875f, -0.975780964f, 1.0f };
    Validate(output, 1.0f, 2, expected0Closest, expected1Closest, false, maxError);

    output = query(circle1, circle0);
    expected0Closest[0] = { 0.21875f, +0.975780964f, 1.0f };
    expected0Closest[1] = { 0.21875f, -0.975780964f, 1.0f };
    expected1Closest[0] = { 0.21875f, +0.975780964f, 0.0f };
    expected1Closest[1] = { 0.21875f, -0.975780964f, 0.0f };
    Validate(output, 1.0f, 2, expected0Closest, expected1Closest, false, maxError);

    // Circles in nonparallel planes (concentric).
    circle1.center = { 0.0f, 0.0f, 1.0f };
    circle1.radius = 1.25f;
    circle1.normal = { 0.0625f, 0.0f, 1.0f };
    Normalize(circle1.normal);
    output = query(circle0, circle1);
    expected0Closest[0] = { 1.0f, 0.0f, 0.0f };
    expected0Closest[1] = { 0.0f, 0.0f, 0.0f };
    expected1Closest[0] = { 1.24756575f, 0.0f, 0.922027528f };
    expected1Closest[1] = { 0.0f, 0.0f, 0.0f };
    Validate(output, 0.911423385f, 1, expected0Closest, expected1Closest, false, maxError);

    output = query(circle1, circle0);
    // circle-order-dependent rounding error
    expected0Closest[0] = { 1.24756575f, 0.0f, 0.922027528f };
    expected0Closest[1] = { 0.0f, 0.0f, 0.0f };
    expected1Closest[0] = { 1.0f, 0.0f, 0.0f };
    expected1Closest[1] = { 0.0f, 0.0f, 0.0f };
    Validate(output, 0.911423385f, 1, expected0Closest, expected1Closest, false, maxError);

    // General case.
    circle0.center = { 0.629447341f, -0.729045987f, 0.811583877f };
    circle0.radius = 2.67001724f;
    circle0.normal = { -0.512442946f, 0.644127250f, 0.567892849f };
    circle1.center = { -0.557931900f, 0.264718533f, -0.383665919f };
    circle1.radius = 1.19508076f;
    circle1.normal = { 0.122579113f, -0.574992657f, -0.808923900f };
    Normalize(circle1.normal);

    output = query(circle0, circle1);
    expected0Closest[0] = { 0.424922794f, 0.938110709f, -1.26392782f };
    expected0Closest[1] = { 0.0f, 0.0f, 0.0f };
    expected1Closest[0] = { 0.227285862f, 1.05095041f, -0.823541999f };
    expected1Closest[1] = { 0.0f, 0.0f, 0.0f };
    Validate(output, 0.245732963f, 1, expected0Closest, expected1Closest, false, maxError);

    // Mathematica
    // sqrDistance = 0.245733
    // e0c[0] = {0.424923, 0.938111, -1.26393}
    // e1c[0] = {0.227286, 1.05095, -0.823542}

    Vector3<float> diff = output.circle0Closest[0] - output.circle1Closest[0];
    float test;
    test = Length(diff);  // 0.495714456
    diff = output.circle0Closest[0] - circle0.center;
    test = Dot(diff, diff) - circle0.radius * circle0.radius;  // -1.43051147e-06
    test = Dot(circle0.normal, diff);  // -1.19209290e-07
    diff = output.circle1Closest[0] - circle1.center;
    test = Dot(diff, diff) - circle1.radius * circle1.radius;  // 4.76837158e-07
    test = Dot(circle1.normal, diff);  // 0.0
    (void)test;

    Vector3<float> N0 = circle0.normal, U0{}, V0{};
    ComputeOrthonormalBasis(1, N0, U0, V0);
    Vector3<float> N1 = circle1.normal, U1{}, V1{};
    ComputeOrthonormalBasis(1, N1, U1, V1);

    float const twopi = C_TWO_PI<float>;
    //std::size_t const imax = 4096;
    std::size_t imax = 128;
    float angle;
    float minDistance = std::numeric_limits<float>::max();
    Vector3<float> minP0, minP1;
    for (std::size_t i1 = 0; i1 < imax; ++i1)
    {
        angle = twopi * i1 / imax;
        Vector3<float> P1 = circle1.center + circle1.radius *
            (std::cos(angle) * U1 + std::sin(angle) * V1);
        for (std::size_t i0 = 0; i0 < imax; ++i0)
        {
            angle = twopi * i0 / imax;
            Vector3<float> P0 = circle0.center + circle0.radius *
                (std::cos(angle) * U0 + std::sin(angle) * V0);
            float distance = Length(P0 - P1);
            if (distance < minDistance)
            {
                minDistance = distance;
                minP0 = P0;
                minP1 = P1;
            }
        }
    }
    // minDistance = 0.495715261 (imax = 4096)

    // TODO: No code coverage in DistCircle3Circle3.h
    // 1. block starting with line 166 (sn != 0)
    // 2. block starting with line 191 (lenN0xDelta = zero)
    // 3. block starting with line 209 (roots.size() > 0 && cand1.sqrDist == cand0.sqrDist)
}

#else

#include <GTL/Mathematics/Distance/3D/DistCircle3Circle3.h>

namespace gtl
{
    template class DCPQuery<float, Circle3<float>, Circle3<float>>;
    template class DCPQuery<double, Circle3<double>, Circle3<double>>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistCircle3Circle3)
