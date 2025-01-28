#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Approximation/3D/ApprCone3.h>
#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestApprCone3
    {
    public:
        UnitTestApprCone3();

    private:
        void TestGN();
        void TestLM();
    };
}

UnitTestApprCone3::UnitTestApprCone3()
{
    UTInformation("Mathematics/Approximation/3D/ApprCone3");

    TestGN();
    TestLM();
}

void UnitTestApprCone3::TestGN()
{
    // The cone vertex is V, the unit-length axis direction is U and the
    // cone angle is A in (0,pi/2). The cone is defined algebraically by
    // those points X for which
    //   Dot(U,X-V)/Length(X-V) = cos(A)
    // This can be written as a quadratic equation
    //   (V-X)^T * (cos(A)^2 - U * U^T) * (V-X) = 0
    // with the implicit constraint that Dot(U, X-V) > 0 (X is on the
    // "positive" cone). Define W = U/cos(A), so Length(W) > 1 and
    //   F(X;V,W) = (V-X)^T * (I - W * W^T) * (V-X) = 0
    //
    // The nonlinear least squares fitting of points {X[i]}_{i=0}^{n-1}
    // computes V and W to minimize the error function
    //   E(V,W) = sum_{i=0}^{n-1} F(X[i];V,W)^2

    // Generate random points on the cone V = (0,0,0), U = (0,0,1),
    // theta = pi/4, where Dot(U,X-V)/Length(X-V) = cos(theta).
    // Choose h >= 0 randomly to produce an axis point h*U. The
    // perpendicular distance to the cone is r = h*tan(theta).
    // Choose D to be a perpendicular direction,
    // D = (cos(phi), sin(phi), 0), where phi is chosen randomly
    // in [-pi,pi). The point is X = V + h*U + r*D. Now randomly
    // perturb that point slightly.
    std::default_random_engine dre{};
    std::uniform_real_distribution<double> urd(0.0, 1.0);
    double h{}, r{}, phi{}, cosPhi{}, sinPhi{};
    std::vector<Vector3<double>> X(1024);
    double tanTheta = tan(C_PI_DIV_4<double>);
    double const epsilon = 1e-03;
    for (auto& point : X)
    {
        h = urd(dre);
        r = h * tanTheta;
        phi = (2.0 * urd(dre) - 1.0) * C_PI<double>;
        cosPhi = cos(phi);
        sinPhi = sin(phi);
        point[0] = r * cosPhi;
        point[1] = r * sinPhi;
        point[2] = h;
        for (std::size_t i = 0; i < 3; ++i)
        {
            point[i] += epsilon * urd(dre);
        }
    }

    // Fit the points with a cone using Gauss-Newton minimization.
    Vector3<double> coneVertex{ 0.1, -0.2, 0.3 };
    Vector3<double> coneAxis{ 1.0, 2.0, 3.0 };
    Normalize(coneAxis);
    double coneAngle = C_PI_DIV_4<double>;
    std::size_t const maxIterations = 32;
    double const updateLengthTolerance = 1e-04;
    double const errorDifferenceTolerance = 1e-08;
    bool userConeInputAsInitialGuess = true;
    auto result = ApprCone3<double>::Fit(X, maxIterations, updateLengthTolerance,
        errorDifferenceTolerance, userConeInputAsInitialGuess, coneVertex,
        coneAxis, coneAngle);
    UTAssert(result.converged, "Cone fit failed to converge.");

    Vector3<double> expectedVertex{ 0.00037531195292083645, 0.00059746047670837966, 0.00048979031657722216 };
    Vector3<double> expectedAxis{ 7.8410081578700219e-05, -4.5594592390968186e-05, 0.99999999588649613 };
    double expectedAngle = 0.78538913562078427;
    double error = Length(coneVertex - expectedVertex);
    UTAssert(error <= 2e-4, "Invalid cone vertex estimate, " + std::to_string(error));
    error = Length(coneAxis - expectedAxis);
    UTAssert(error <= 3e-5, "Invalid cone axis estimate, " + std::to_string(error));
    error = std::fabs(coneAngle - expectedAngle);
    UTAssert(error <= 2e-5, "Invalid cone angle estimate, " + std::to_string(error));
    error = std::fabs(result.minUpdateLength - 5.4368118326211017e-05);
    UTAssert(error <= 2e-5, "Invalid update length, " + std::to_string(error));
    error = std::fabs(result.minErrorDifference - 7.4103456368458166e-06);
    UTAssert(error <= 7e-6, "Invalid error difference, " + std::to_string(error));
    error = std::fabs(result.minError - 0.00022691176454512111);
    UTAssert(error <= 5e-6, "Invalid min error, " + std::to_string(error));

    userConeInputAsInitialGuess = false;
    result = ApprCone3<double>::Fit(X, maxIterations, updateLengthTolerance,
        errorDifferenceTolerance, userConeInputAsInitialGuess, coneVertex,
        coneAxis, coneAngle);
    UTAssert(result.converged, "Cone fit failed to converge.");

    expectedVertex = { 0.00037531091297466642, 0.00059745979665676882, 0.00048979443528677927 };
    expectedAxis = { 7.8410951667486707e-05, -4.5593825256553661e-05, 0.99999999588646282 };
    expectedAngle = 0.78538913545862421;
    error = Length(coneVertex - expectedVertex);
    UTAssert(error <= 2e-4, "Invalid cone vertex estimate, " + std::to_string(error));
    error = Length(coneAxis - expectedAxis);
    UTAssert(error <= 3e-5, "Invalid cone axis estimate, " + std::to_string(error));
    error = std::fabs(coneAngle - expectedAngle);
    UTAssert(error <= 2e-5, "Invalid cone angle estimate, " + std::to_string(error));
    error = std::fabs(result.minUpdateLength - 1.1291213708733307e-06);
    UTAssert(error <= 5e-5, "Invalid update length, " + std::to_string(error));
    error = std::fabs(result.minErrorDifference - 2.7498118372983656e-09);
    UTAssert(error <= 2e-6, "Invalid error difference, " + std::to_string(error));
    error = std::fabs(result.minError - 0.00022691176451848847);
    UTAssert(error <= 5e-6, "Invalid min error, " + std::to_string(error));
}

void UnitTestApprCone3::TestLM()
{
    // The cone vertex is V, the unit-length axis direction is U and the
    // cone angle is A in (0,pi/2). The cone is defined algebraically by
    // those points X for which
    //   Dot(U,X-V)/Length(X-V) = cos(A)
    // This can be written as a quadratic equation
    //   (V-X)^T * (cos(A)^2 - U * U^T) * (V-X) = 0
    // with the implicit constraint that Dot(U, X-V) > 0 (X is on the
    // "positive" cone). Define W = U/cos(A), so Length(W) > 1 and
    //   F(X;V,W) = (V-X)^T * (I - W * W^T) * (V-X) = 0
    //
    // The nonlinear least squares fitting of points {X[i]}_{i=0}^{n-1}
    // computes V and W to minimize the error function
    //   E(V,W) = sum_{i=0}^{n-1} F(X[i];V,W)^2

    // Generate random points on the cone V = (0,0,0), U = (0,0,1),
    // theta = pi/4, where Dot(U,X-V)/Length(X-V) = cos(theta).
    // Choose h >= 0 randomly to produce an axis point h*U. The
    // perpendicular distance to the cone is r = h*tan(theta).
    // Choose D to be a perpendicular direction,
    // D = (cos(phi), sin(phi), 0), where phi is chosen randomly
    // in [-pi,pi). The point is X = V + h*U + r*D.  Now randomly
    // perturb that point slightly.
    std::default_random_engine dre{};
    std::uniform_real_distribution<double> urd(0.0, 1.0);
    double h, r, phi, cosPhi, sinPhi;
    std::vector<Vector3<double>> X(1024);
    double tanTheta = tan(C_PI_DIV_4<double>);
    double const epsilon = 1e-03;
    for (auto& point : X)
    {
        h = urd(dre);
        r = h * tanTheta;
        phi = (2.0 * urd(dre) - 1.0) * C_PI<double>;
        cosPhi = cos(phi);
        sinPhi = sin(phi);
        point[0] = r * cosPhi;
        point[1] = r * sinPhi;
        point[2] = h;
        for (std::size_t i = 0; i < 3; ++i)
        {
            point[i] += epsilon * urd(dre);
        }
    }

    // Fit the points with a cone using Levenberg-Marquardt minimization.
    Vector3<double> coneVertex{ 0.1, -0.2, 0.3 };
    Vector3<double> coneAxis{ 1.0, 2.0, 3.0 };
    Normalize(coneAxis);
    double coneAngle = C_PI_DIV_4<double>;
    std::size_t const maxIterations = 32;
    double const updateLengthTolerance = 1e-04;
    double const errorDifferenceTolerance = 1e-08;
    bool userConeInputAsInitialGuess = true;
    double const lambdaFactor = 0.001;
    double const lambdaAdjust = 10.0;
    std::size_t const maxAdjustments = 8;
    auto result = ApprCone3<double>::Fit(X, maxIterations, updateLengthTolerance,
        errorDifferenceTolerance, lambdaFactor, lambdaAdjust, maxAdjustments,
        userConeInputAsInitialGuess, coneVertex, coneAxis, coneAngle);
    UTAssert(result.converged, "Cone fit failed to converge.");

    Vector3<double> expectedVertex{ 0.00037531126441630832, 0.00059745994203759892, 0.00048979449433324534 };
    Vector3<double> expectedAxis{ 7.8410715252428658e-05, -4.5593931561642606e-05, 0.99999999588647648 };
    double expectedAngle = 0.78538913549049472;
    double error = Length(coneVertex - expectedVertex);
    UTAssert(error <= 2e-4, "Invalid cone vertex estimate, " + std::to_string(error));
    error = Length(coneAxis - expectedAxis);
    UTAssert(error <= 3e-5, "Invalid cone axis estimate, " + std::to_string(error));
    error = std::fabs(coneAngle - expectedAngle);
    UTAssert(error <= 2e-5, "Invalid cone angle estimate, " + std::to_string(error));
    error = std::fabs(result.minUpdateLength - 5.4368118326211017e-05);
    UTAssert(error <= 6e-5, "Invalid update length, " + std::to_string(error));
    error = std::fabs(result.minErrorDifference - 7.4103456368458166e-06);
    UTAssert(error <= 8e-6, "Invalid error difference, " + std::to_string(error));
    error = std::fabs(result.minError - 0.00022691176454512111);
    UTAssert(error <= 5e-6, "Invalid min error, " + std::to_string(error));

    userConeInputAsInitialGuess = false;
    result = ApprCone3<double>::Fit(X, maxIterations, updateLengthTolerance,
        errorDifferenceTolerance, lambdaFactor, lambdaAdjust, maxAdjustments,
        userConeInputAsInitialGuess, coneVertex, coneAxis, coneAngle);
    expectedVertex = { 0.00037530947997174762, 0.00059746273013610229, 0.00048979505053730688 };
    expectedAxis = { 7.8411889607539142e-05, -4.5595720040993958e-05, 0.99999999588630284 };
    expectedAngle = 0.78538913588061965;
    error = Length(coneVertex - expectedVertex);
    UTAssert(error <= 2e-4, "Invalid cone vertex estimate, " + std::to_string(error));
    error = Length(coneAxis - expectedAxis);
    UTAssert(error <= 3e-5, "Invalid cone axis estimate, " + std::to_string(error));
    error = std::fabs(coneAngle - expectedAngle);
    UTAssert(error <= 2e-5, "Invalid cone angle estimate, " + std::to_string(error));
    error = std::fabs(result.minUpdateLength - 5.4368118326211017e-05);
    UTAssert(error <= 5e-5, "Invalid update length, " + std::to_string(error));
    error = std::fabs(result.minErrorDifference - 7.4103456368458166e-06);
    UTAssert(error <= 6e-5, "Invalid error difference, " + std::to_string(error));
    error = std::fabs(result.minError - 0.00022691176454512111);
    UTAssert(error <= 5e-6, "Invalid min error, " + std::to_string(error));
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Approximation/3D/ApprCone3.h>

namespace gtl
{
    template class ApprCone3<float>;
    template class ApprCone3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ApprCone3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ApprCone3)
