#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Surfaces/ParametricSurface.h>
using namespace gtl;

namespace gtl
{
    class UnitTestParametricSurface
    {
    public:
        UnitTestParametricSurface();

    private:
        // Encapsulate
        // F(u,v) = (2 * u^2 * v - u * v^2 + 1, sin(u * v), exp(u * v))
        // Fu(u,v) = (4 * u * v - v^2, v * cos(u * v), v * exp(u * v))
        // Fv(u,v) = (2 * u^2 - 2 * u * v, u * cos(u * v), u * exp(u * v))
        // Fuu(u,v) = (4 * v, -v^2 * sin(u * v), v^2 * exp(u * v))
        // Fuv(u,v) = (4 * u - 2 * v, cos(u * v) - v^2 * sin(u * v), (1 + u * v)*exp(u * v))
        // Fvv(u,v) = (-2 * u, -u^2 * sin(u * v), u^2 * exp(u * v)))
        class Surface : public ParametricSurface<double, 3>
        {
        public:
            Surface()
                :
                ParametricSurface<double, 3>(0.0, 1.0, 0.0, 1.0, true)
            {
            }

            virtual void Evaluate(double const& u, double const& v,
                std::size_t order, Vector3<double>* jet) const override
            {
                double usqr = u * u;
                double vsqr = v * v;
                double uv = u * v;
                double sinuv = std::sin(uv);
                double expuv = std::exp(u * v);
                // F(u,v)
                jet[0][0] = 2.0 * usqr * v - u * vsqr + 1.0;
                jet[0][1] = sinuv;
                jet[0][2] = expuv;
                if (order >= 1)
                {
                    double cosuv = std::cos(u * v);
                    // Fu(u,v)
                    jet[1][0] = 4.0 * uv - vsqr;
                    jet[1][1] = v * cosuv;
                    jet[1][2] = v * expuv;
                    // Fv(u,v)
                    jet[2][0] = 2.0 * (usqr - uv);
                    jet[2][1] = u * cosuv;
                    jet[2][2] = u * expuv;
                    if (order >= 2)
                    {
                        // Fuu(u,v)
                        jet[3][0] = 4.0 * v;
                        jet[3][1] = -vsqr * sinuv;
                        jet[3][2] = vsqr * expuv;
                        // Fuv(u,v)
                        jet[4][0] = 4.0 * u - 2.0 * v;
                        jet[4][1] = cosuv - uv * sinuv;
                        jet[4][2] = (1.0 + uv) * expuv;
                        // Fvv(u,v)
                        jet[5][0] = -2.0 * u;
                        jet[5][1] = -usqr * sinuv;
                        jet[5][2] = usqr * expuv;

                        // Derivatives of order 3 and higher are not yet
                        // supported. If you need them, derive a class from
                        // this one and implement Evaluate(...) to handle the
                        // larger orders.
                        std::size_t imax = (order + 1) * (order + 2) / 2;
                        for (std::size_t i = 6; i < imax; ++i)
                        {
                            MakeZero(jet[i]);
                        }
                    }
                }
            }
        };

        void Test();
    };
}

UnitTestParametricSurface::UnitTestParametricSurface()
{
    UTInformation("Mathematics/Surfaces/ParametricSurface");

    Test();
}

void UnitTestParametricSurface::Test()
{
    double const maxError = 1e-15;
    Surface surface{};
    std::array<Vector3<double>, 10> expectedJet
    {
        Vector3<double>{0.953125, 0.18640329676226988, 1.2062302494209807},
        Vector3<double>{0.1875, 0.73685498482594147, 0.90467268706573556},
        Vector3<double>{-0.25, 0.24561832827531382, 0.30155756235524517},
        Vector3<double>{3.0, -0.10485185442877681, 0.67850451529930167},
        Vector3<double>{-0.5, 0.94752269495832975, 1.4323984211874146},
        Vector3<double>{-0.5, -0.011650206047641868, 0.075389390588811292},
        Vector3<double>{0.0, 0.0, 0.0},
        Vector3<double>{0.0, 0.0, 0.0},
        Vector3<double>{0.0, 0.0, 0.0},
        Vector3<double>{0.0, 0.0, 0.0}
    };
    std::array<Vector3<double>, 10> jet{};
    surface.Evaluate(0.25, 0.75, 3, jet.data());
    for (std::size_t i = 0; i < 10; ++i)
    {
        UTAssert(Length(jet[i] - expectedJet[i]) <= maxError,
            "Invalid jet[" + std::to_string(i) + "]");
    }
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Surfaces/ParametricSurface.h>

namespace gtl
{
    template class ParametricSurface<float, 3>;
    template class ParametricSurface<double, 4>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ParametricSurface<Rational, 2>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ParametricSurface)
