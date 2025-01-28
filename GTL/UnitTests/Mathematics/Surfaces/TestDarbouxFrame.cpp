#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Surfaces/DarbouxFrame.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDarbouxFrame
    {
    public:
        UnitTestDarbouxFrame();

    private:
        class EllipsoidSurface : public ParametricSurface<double, 3>
        {
        public:
            EllipsoidSurface()
                :
                ParametricSurface<double, 3>(0.0, C_TWO_PI<double>, 0.0, C_PI<double>, true),
                a(3.0),
                b(2.0),
                c(1.0)
            {
            }

            virtual void Evaluate(double const& u, double const& v, std::size_t order, Vector<double, 3>* jet) const override
            {
                double csu = std::cos(u), snu = std::sin(u);
                double csv = std::cos(v), snv = std::sin(v);

                // X(u,v)
                jet[0] = { a * csu * snv, b * snu * snv, c * csv };

                if (order >= 1)
                {
                    // dX/du
                    jet[1] = { -a * snu * snv, b * csu * snv, 0.0 };

                    // dX/dv
                    jet[2] = { a * csu * csv, b * snu * csv, -c * snv };

                    if (order >= 2)
                    {
                        // d2X/du2
                        jet[3] = { -a * csu * snv, -b * snu * snv, 0.0 };

                        // d2X/dudv
                        jet[4] = { -a * snu * csv, b * csu * csv, 0.0 };

                        // d2X/dv2
                        jet[5] = { -a * csu * snv, -b * snu * snv, -c * csv };
                    }
                }
            }

            double a, b, c;
        };

        void Test();
    };
}

UnitTestDarbouxFrame::UnitTestDarbouxFrame()
{
    UTInformation("Mathematics/Surfaces/DarbouxFrame");

    Test();
}

void UnitTestDarbouxFrame::Test()
{
    EllipsoidSurface surface{};

    double const maxError = 1e-15;
    double u = 0.75, v = 0.5;
    Vector3<double> position{}, tangent0{}, tangent1{}, normal{};
    DarbouxFrame3<double>::GetFrame(surface, u, v,
        position, tangent0, tangent1, normal);
    Vector3<double> expectedTangent0{ -0.81322093447405519, 0.58195507707480709, 0.0 };
    Vector3<double> expectedTangent1{ 0.56727663332468281, 0.79270935512139928, -0.22317952322475473 };
    Vector3<double> expectedNormal{ -0.12988045663978082, -0.18149426043230910, -0.97477735940735266 };
    UTAssert(Length(tangent0 - expectedTangent0) <= maxError, "Invalid tangent0.");
    UTAssert(Length(tangent1 - expectedTangent1) <= maxError, "Invalid tangent1.");
    UTAssert(Length(normal - expectedNormal) <= maxError, "Invalid normal.");

    double curvature0 = 0.0, curvature1 = 0.0;
    Vector3<double> direction0{}, direction1{};
    DarbouxFrame3<double>::GetPrincipalInformation(surface, u, v,
        curvature0, curvature1, direction0, direction1);
    double expectedCurvature0 = -0.30785354978313789;
    double expectedCurvature1 = -0.13734824921378219;
    Vector3<double> expectedDirection0{ 0.11398191221761804, 0.97385427607111608, -0.19650947220227746 };
    Vector3<double> expectedDirection1{ 0.98495644100145718, -0.13662972739536447, -0.10579757521612274 };
    UTAssert(std::fabs(curvature0 - expectedCurvature0) <= maxError, "Invalid curvature0.");
    UTAssert(std::fabs(curvature1 - expectedCurvature1) <= maxError, "Invalid curvature1.");
    UTAssert(Length(direction0 - expectedDirection0) <= maxError, "Invalid direction0.");
    UTAssert(Length(direction1 - expectedDirection1) <= maxError, "Invalid direction1.");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Surfaces/DarbouxFrame.h>

namespace gtl
{
    template class DarbouxFrame3<float>;
    template class DarbouxFrame3<double>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DarbouxFrame3<Rational>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DarbouxFrame)
