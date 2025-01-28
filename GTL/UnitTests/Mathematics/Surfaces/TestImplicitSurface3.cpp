#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Surfaces/ImplicitSurface3.h>
using namespace gtl;

namespace gtl
{
    class UnitTestImplicitSurface3
    {
    public:
        UnitTestImplicitSurface3();

    private:
        // Define a class for
        // F(x,y,z) = (x^2 + y^2 + z^2)^2 - 2 * (x^2 + y^2 - z^2) = 0
        // Fx = 4 * x * (x^2 + y^2 + z^2 - 1)
        // Fy = 4 * y * (x^2 + y^2 + z^2 - 1)
        // Fz = 4 * z * (x^2 + y^2 + z^2 + 1)
        // Fxx = 12 * x^2 + 4 * (y^2 + z^2) - 4
        // Fyy = 12 * y^2 + 4 * (x^2 + z^2) - 4
        // Fzz = 12 * z^2 + 4 * (x^2 + y^2) + 4
        // Fxy = 8 * x * y
        // Fxz = 8 * x * z
        // Fyz = 8 * y * z
        class Surface : public ImplicitSurface3<double>
        {
        public:
            virtual double F(Vector3<double> const& position) const override
            {
                Vector3<double> sqr = position * position;
                double term0 = sqr[0] + sqr[1] + sqr[2];
                double term1 = sqr[0] + sqr[1] - sqr[2];
                return term0 * term0 - 2.0 * term1;
            }

            virtual double FX(Vector3<double> const& position) const override
            {
                Vector3<double> sqr = position * position;
                double term = sqr[0] + sqr[1] + sqr[2];
                return 4.0 * position[0] * (term - 1.0);
            }

            virtual double FY(Vector3<double> const& position) const override
            {
                Vector3<double> sqr = position * position;
                double term = sqr[0] + sqr[1] + sqr[2];
                return 4.0 * position[1] * (term - 1.0);
            }

            virtual double FZ(Vector3<double> const& position) const override
            {
                Vector3<double> sqr = position * position;
                double term = sqr[0] + sqr[1] + sqr[2];
                return 4.0 * position[2] * (term + 1.0);
            }

            virtual double FXX(Vector3<double> const& position) const override
            {
                Vector3<double> sqr = position * position;
                return 4.0 * (3.0 * sqr[0] + sqr[1] + sqr[2] - 1.0);
            }

            virtual double FXY(Vector3<double> const& position) const override
            {
                return 8.0 * position[0] * position[1];
            }

            virtual double FXZ(Vector3<double> const& position) const override
            {
                return 8.0 * position[0] * position[2];
            }

            virtual double FYY(Vector3<double> const& position) const override
            {
                Vector3<double> sqr = position * position;
                return 4.0 * (3.0 * sqr[1] + sqr[0] + sqr[2] - 1.0);
            }

            virtual double FYZ(Vector3<double> const& position) const override
            {
                return 8.0 * position[1] * position[2];
            }

            virtual double FZZ(Vector3<double> const& position) const override
            {
                Vector3<double> sqr = position * position;
                return 4.0 * (3.0 * sqr[2] + sqr[0] + sqr[1] + 1.0);
            }

        };

        void Test();
    };
}

UnitTestImplicitSurface3::UnitTestImplicitSurface3()
{
    UTInformation("Mathematics/Surfaces/ImplicitSurface3");

    Test();
}

void UnitTestImplicitSurface3::Test()
{
    double const maxError = 1e-08;
    Surface implicit{};
    double const t = std::sqrt(2.0) / 3.0;
    Vector3<double> position{ t, t, t };

    // The function IsOnSurface indirectly tests implicit.F(position).
    bool isOnSurface = implicit.IsOnSurface(position, 1e-08);
    UTAssert(isOnSurface, "Incorrect point-on-surface test.");

    Vector3<double> gradient = implicit.GetGradient(position);
    Vector3<double> expectedGradient
    {
        4.0 * t * (3.0 * t * t - 1.0),
        4.0 * t * (3.0 * t * t - 1.0),
        4.0 * t * (3.0 * t * t + 1.0)
    };
    UTAssert(Length(gradient - expectedGradient) <= maxError, "Incorrect gradient.");

    Matrix3x3<double> hessian = implicit.GetHessian(position);
    Matrix3x3<double> expectedHessian
    {
        { 20.0 * t * t - 4.0, 8.0 * t * t, 8.0 * t * t },
        { 8.0 * t * t, 20.0 * t * t - 4.0, 8.0 * t * t },
        { 8.0 * t * t, 8.0 * t * t, 20.0 * t * t + 4.0 }
    };
    Matrix3x3<double> diff = hessian - expectedHessian;
    UTAssert(LInfinityNorm(diff) <= maxError, "Incorrect Hessian.");

    Vector3<double> tangent0{}, tangent1{}, normal{};
    implicit.GetFrame(position, tangent0, tangent1, normal);
    Matrix3x3<double> rotate{};
    rotate.SetCol(0, tangent0);
    rotate.SetCol(1, tangent1);
    rotate.SetCol(2, normal);
    diff = Transpose(rotate) * rotate - Matrix3x3<double>::Identity();
    UTAssert(LInfinityNorm(diff) <= maxError, "Incorrect surface frame.");

    double curvature0{}, curvature1{};
    Vector3<double> direction0{}, direction1{};
    bool success = implicit.GetPrincipalInformation(position, curvature0,
        curvature1, direction0, direction1);
    double expectedCurvature0 = -0.40824829046386268;
    double expectedCurvature1 = 1.2247448713915892;
    Vector3<double> expectedDirection0
    {
        -0.70710678118654746,
        +0.70710678118654746,
        0.0
    };
    Vector3<double> expectedDirection1 =
    {
        -0.68041381743977181,
        -0.68041381743977158,
        -0.27216552697590862
    };
    UTAssert(success, "GetPrincipalInformation failed.");
    UTAssert(std::fabs(curvature0 - expectedCurvature0) <= maxError,
        "Invalid curvature0.");
    UTAssert(std::fabs(curvature1 - expectedCurvature1) <= maxError,
        "Invalid curvature1.");
    UTAssert(Length(direction0 - expectedDirection0) <= maxError,
        "Invalid direction0.");
    UTAssert(Length(direction1 - expectedDirection1) <= maxError,
        "Invalid direction1.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Surfaces/ImplicitSurface3.h>

namespace gtl
{
    template class ImplicitSurface3<float>;
    template class ImplicitSurface3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ImplicitSurface3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ImplicitSurface3)
