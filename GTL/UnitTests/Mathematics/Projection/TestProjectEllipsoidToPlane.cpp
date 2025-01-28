#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Projection/ProjectEllipsoidToPlane.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
#include <random>
#include <set>
using namespace gtl;

namespace gtl
{
    class UnitTestProjectEllipsoidToPlane
    {
    public:
        UnitTestProjectEllipsoidToPlane();

    private:
        void TestPerspectiveProject();
    };
}

UnitTestProjectEllipsoidToPlane::UnitTestProjectEllipsoidToPlane()
{
    UTInformation("Mathematics/Projection/ProjectEllipsoidToPlane");

    TestPerspectiveProject();
}

void UnitTestProjectEllipsoidToPlane::TestPerspectiveProject()
{
    std::default_random_engine dre;
    std::uniform_real_distribution<double> rnd(-1.0, 1.0);

    Ellipsoid3<double> ellipsoid{};
    Vector3<double> eye{};
    Plane3<double> plane{};
    Ellipse2<double> ellipse{};
    double n;

    // Create the plane normal N and two vectors U and V in the plane so
    // that {U,V,N} is a right-handed orthonormal basis.
    Vector3<double> N{ rnd(dre), rnd(dre), rnd(dre) }, U, V;
    ComputeOrthonormalBasis(1, N, U, V);

    // Create a random point for the eyepoint.
    eye = { rnd(dre), rnd(dre), rnd(dre) };

    // Set the view plane to be 'n' units away from the eyepoint in the
    // plane-normal direction.
    n = 1.0 + 10.0 * (2.0 * rnd(dre) + 1.0);
    plane.normal = N;
    plane.constant = n + Dot(N, eye);

    // Create an ellipsoid that is between the eyepoint and the view plane.
    ellipsoid.center = eye + 0.5 * n * N;
    ellipsoid.extent[0] = 0.03;
    ellipsoid.extent[1] = 0.02;
    ellipsoid.extent[2] = 0.0025;
    Quaternion<double> q{ 1.0, 2.0, 3.0, 4.0 };
    Normalize(q);
    Matrix3x3<double> orient;
    RigidMotion<double>::Convert(q, orient);
    ellipsoid.axis[0] = orient.GetCol(0);
    ellipsoid.axis[1] = orient.GetCol(1);
    ellipsoid.axis[2] = orient.GetCol(2);

    // Project the ellipsoid to an ellipse.
    PerspectiveProject(ellipsoid, eye, N, U, V, n, ellipse);

    // Code to verify that the rays from the eyepoint to the ellipse points
    // are tangent to the ellipsoid. The 'discriminants' set should contain
    // values that are nearly zero. These are theoretically zero, but
    // floating-point rounding errors occur.
    Matrix3x3<double> A{};
    Vector3<double> B{};
    double C;
    ellipsoid.ToCoefficients(A, B, C);

    double angle, alpha, beta, gamma, discriminant;
    Vector2<double> Y{};
    Vector3<double> P{}, D{};

    Vector3<double> K = eye + n * N;
    Matrix<double, 3, 2> J{};
    J.SetCol(0, U);
    J.SetCol(1, V);
    std::size_t const numSamples = 1024;
    std::set<double> discriminants;
    for (std::size_t i = 0; i < numSamples; ++i)
    {
        // Generate a point on the ellipse and compute its location P in the
        // view plane.
        angle = C_TWO_PI<double> * static_cast<double>(i) / static_cast<double>(numSamples);
        Y = ellipse.center +
            ellipse.extent[0] * std::cos(angle) * ellipse.axis[0] +
            ellipse.extent[1] * std::sin(angle) * ellipse.axis[1];
        P = K + J * Y;

        // Compute discriminant of the quadratic equation that determines
        // the intersection points of the ray from E to P with the ellipsoid.
        // The discriminant is theoretically zero when the ray is tangent to
        // the ellipsoid.
        D = P - eye;
        Normalize(D);
        alpha = Dot(D, A * D);
        beta = Dot(B, D) + 2.0 * Dot(D, A * eye);
        gamma = Dot(eye, A * eye) + Dot(B, eye) + C;
        discriminant = beta * beta - 4.0 * alpha * gamma;
        discriminants.insert(discriminant);
    }

    double const epsilon = 1e-03;
    double dmin = std::fabs(*discriminants.begin());
    UTAssert(dmin <= epsilon, "Discriminant is too large, " + std::to_string(dmin));
    double dmax = std::fabs(*discriminants.rbegin());
    UTAssert(dmax <= epsilon, "Discriminant is too large, " + std::to_string(dmin));
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Projection/ProjectEllipsoidToPlane.h>

namespace gtl
{
    template void PerspectiveProject(Ellipsoid3<float> const&, Vector3<float> const&,
        Plane3<float> const&, Ellipse2<float>&);

    template void PerspectiveProject(Ellipsoid3<double> const&, Vector3<double> const&,
        Plane3<double> const&, Ellipse2<double>&);

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template void PerspectiveProject(Ellipsoid3<Rational> const&, Vector3<Rational> const&,
        Plane3<Rational> const&, Ellipse2<Rational>&);
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ProjectEllipsoidToPlane)
