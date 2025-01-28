#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Curves/ImplicitCurve2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestImplicitCurve2
    {
    public:
        UnitTestImplicitCurve2();

    private:
        // Define a class for F(x,y) = x^2 + y^2 - 1 = 0.
        class Circle : public ImplicitCurve2<double>
        {
        public:
            virtual double F(Vector2<double> const& position) const override
            {
                return position[0] * position[0] + position[1] * position[1] - 1.0;
            }

            virtual double FX(Vector2<double> const& position) const override
            {
                return 2.0 * position[0];
            }

            virtual double FY(Vector2<double> const& position) const override
            {
                return 2.0 * position[1];
            }

            virtual double FXX(Vector2<double> const&) const override
            {
                return 2.0;
            }

            virtual double FXY(Vector2<double> const&) const override
            {
                return 0.0;
            }

            virtual double FYY(Vector2<double> const&) const override
            {
                return 2.0;
            }
        };

        void Test();
    };
}

UnitTestImplicitCurve2::UnitTestImplicitCurve2()
{
    UTInformation("Mathematics/Curves/ImplicitCurve2");

    Test();
}

void UnitTestImplicitCurve2::Test()
{
    double const maxError = 1e-08;

    Circle implicit{};
    double angle = 0.123456;
    double cs = std::cos(angle);
    double sn = std::sin(angle);
    Vector2<double> position{ cs, sn };

    // The function IsOnCurve indirectly tests implicit.F(position).
    bool isOnCurve = implicit.IsOnCurve(position, 1e-08);
    UTAssert(isOnCurve, "Incorrect point-on-curve test.");

    Vector2<double> gradient = implicit.GetGradient(position);
    UTAssert(Length(gradient - 2.0 * position) <= maxError, "Incorrect gradient.");

    Matrix2x2<double> hessian = implicit.GetHessian(position);
    Matrix2x2<double> diff = hessian - 2.0 * Matrix2x2<double>::Identity();
    UTAssert(LInfinityNorm(diff) <= maxError, "Incorrect Hessian.");

    Vector2<double> tangent{}, normal{};
    implicit.GetFrame(position, tangent, normal);
    UTAssert(Length(normal - position) <= maxError, "Incorrect normal.");
    UTAssert(std::fabs(Length(tangent) - 1.0) <= maxError, "Incorrect tangent length.");
    UTAssert(std::fabs(Dot(tangent, normal)) <= maxError, "Incorrect tangent.");

    double curvature = 0.0;
    bool success = implicit.GetCurvature(position, curvature);
    UTAssert(success && std::fabs(curvature + 1.0) <= maxError, "Incorrect curvature.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Curves/ImplicitCurve2.h>

namespace gtl
{
    template class ImplicitCurve2<float>;
    template class ImplicitCurve2<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ImplicitCurve2<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ImplicitCurve2)
