#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Curves/ParametricCurve.h>
using namespace gtl;

namespace gtl
{
    class UnitTestParametricCurve
    {
    public:
        UnitTestParametricCurve();

    private:
        // Encapsulate F(t) = (2 * exp(t), 1 + sin(t)).
        class SingleCurve : public ParametricCurve<double, 2>
        {
        public:
            SingleCurve()
                :
                ParametricCurve<double, 2>(0.0, 1.0)
            {
            }

            virtual void Evaluate(double const& t, std::size_t order,
                Vector2<double>* jet) const override
            {
                double expt = std::exp(t);
                double sint = std::sin(t);
                jet[0][0] = 2.0 * expt;
                jet[0][1] = sint + 1.0;
                if (order >= 1)
                {
                    double cost = std::cos(t);
                    jet[1][0] = jet[0][0];
                    jet[1][1] = cost;
                    if (order >= 2)
                    {
                        jet[2][0] = jet[0][0];
                        jet[2][1] = -sint;
                        if (order >= 3)
                        {
                            jet[3][0] = 0.0;
                            jet[3][1] = 0.0;
                        }
                    }
                }
            }
        };

        // Encapsulate a piecewise function.
        //   F(t) = (2*exp(t), 1 + sin(pi*t)), 0 <= t <= 1
        //          (2*exp(t), 1 - pi*(t-1)), 1 <= t <= 2
        //          (2*exp(t), (1-pi) - pi*(t-2) + (t-2)^2, 2 <= t <= 3
        class MultipleCurve : public ParametricCurve<double, 2>
        {
        public:
            MultipleCurve()
                :
                ParametricCurve<double, 2>(0.0, 3.0)
            {
            }

            virtual void Evaluate(double const& t, std::size_t order,
                Vector2<double>* jet) const override
            {
                double const pi = C_PI<double>;
                double expt = std::exp(t);

                if (0.0 <= t && t <= 1.0)
                {
                    double sint = std::sin(t);
                    jet[0][0] = 2.0 * expt;
                    jet[0][1] = sint + 1.0;
                    if (order >= 1)
                    {
                        double cost = std::cos(t);
                        jet[1][0] = jet[0][0];
                        jet[1][1] = cost;
                        if (order >= 2)
                        {
                            jet[2][0] = jet[0][0];
                            jet[2][1] = -sint;
                            if (order >= 3)
                            {
                                jet[3][0] = 0.0;
                                jet[3][1] = 0.0;
                            }
                        }
                    }
                }
                else if (1.0 <= t && t <= 2.0)
                {
                    jet[0][0] = 2.0 * expt;
                    jet[0][1] = 1.0 - pi * (t - 1.0);
                    if (order >= 1)
                    {
                        jet[1][0] = jet[0][0];
                        jet[1][1] = -pi;
                        if (order >= 2)
                        {
                            jet[2][0] = jet[0][0];
                            jet[2][1] = 0.0;
                            if (order >= 3)
                            {
                                jet[3][0] = 0.0;
                                jet[3][1] = 0.0;
                            }
                        }
                    }
                }
                else  // 2.0 <= t <= 3.0
                {
                    double tm2 = t - 2.0;
                    jet[0][0] = 2.0 * expt;
                    jet[0][1] = (1.0 - pi) - pi * tm2 + tm2 * tm2;
                    if (order >= 1)
                    {
                        jet[1][0] = jet[0][0];
                        jet[1][1] = -pi + 2.0 * tm2;
                        if (order >= 2)
                        {
                            jet[2][0] = jet[0][0];
                            jet[2][1] = 2.0;
                            if (order >= 3)
                            {
                                jet[3][0] = 0.0;
                                jet[3][1] = 0.0;
                            }
                        }
                    }
                }
            }
        };

        void TestSingle();
        void TestMultiple();
    };
}

UnitTestParametricCurve::UnitTestParametricCurve()
{
    UTInformation("Mathematics/Curves/ParametricCurve");

    TestSingle();
    TestMultiple();
}

void UnitTestParametricCurve::TestSingle()
{
    double const maxError = 1e-15;
    SingleCurve curve{};
    Vector2<double> expectedP{ 2.0, 1.0 };
    Vector2<double> expectedDPDT{ 2.0, 1.0 };
    Vector2<double> expectedD2PDT2{ 2.0, 0.0 };
    std::array<Vector2<double>, 3> jet{};
    curve.Evaluate(0.0, 2, jet.data());
    UTAssert(Length(jet[0] - expectedP) <= maxError, "Invalid position.");
    UTAssert(Length(jet[1] - expectedDPDT) <= maxError, "Invalid first derivative.");
    UTAssert(Length(jet[2] - expectedD2PDT2) <= maxError, "Invalid second derivative.");

    curve.Evaluate(0.5, 2, jet.data());

    Vector2<double> position = curve.GetPosition(0.5);
    expectedP = { 3.2974425414002564, 1.4794255386042030 };
    UTAssert(Length(position - expectedP) <= maxError, "Invalid position.");

    Vector2<double> tangent = curve.GetTangent(0.5);
    UTAssert(std::fabs(Length(tangent) - 1.0) <= maxError, "Invalid tangent.");
    UTAssert(std::fabs(DotPerp(tangent, jet[1])) <= maxError, "Invalid tangent.");

    double speed = curve.GetSpeed(0.5);
    UTAssert(std::fabs(speed - Length(jet[1])) <= maxError, "Invalid speed.");

    // Verified with Mathematica.
    double length = curve.GetLength(0.25, 0.75);
    double expectedLength = 1.7241836721117736;
    UTAssert(std::fabs(length - expectedLength) <= maxError, "Invalid length.");

    // Verified with Mathematica.
    double totalLength = curve.GetTotalLength();
    double expectedTotalLength = 3.5574466035846668;
    UTAssert(std::fabs(totalLength - expectedTotalLength) <= maxError, "Invalid total length.");

    double tForLength = curve.GetTime(3.0);  // 0.89254805732534892
    length = curve.GetLength(0.0, tForLength);
    UTAssert(std::fabs(length - 3.0) <= maxError, "Invalid time for length.");

    std::array<Vector2<double>, 4> expectedSubByTime{};
    expectedSubByTime[0] = curve.GetPosition(0.0);
    expectedSubByTime[1] = curve.GetPosition(1.0 / 3.0);
    expectedSubByTime[2] = curve.GetPosition(2.0 / 3.0);
    expectedSubByTime[3] = curve.GetPosition(1.0);
    std::array<Vector2<double>, 4> subByTime{};
    curve.SubdivideByTime(4, subByTime.data());
    for (std::size_t i = 0; i < 4; ++i)
    {
        UTAssert(Length(subByTime[i] - expectedSubByTime[i]) <= maxError,
            "Invalid subdivision by time.");
    }

    std::array<Vector2<double>, 4> expectedSubByLength{};
    expectedSubByLength[0] = curve.GetPosition(curve.GetTime(0.0));
    expectedSubByLength[1] = curve.GetPosition(curve.GetTime(totalLength / 3.0));
    expectedSubByLength[2] = curve.GetPosition(curve.GetTime(2.0 * totalLength / 3.0));
    expectedSubByLength[3] = curve.GetPosition(curve.GetTime(totalLength));
    std::array<Vector2<double>, 4> subByLength{};
    curve.SubdivideByLength(4, subByLength.data());
    for (std::size_t i = 0; i < 4; ++i)
    {
        UTAssert(Length(subByLength[i] - expectedSubByLength[i]) <= maxError,
            "Invalid subdivision by length.");
    }
}

void UnitTestParametricCurve::TestMultiple()
{
    double const maxError = 1e-4;
    MultipleCurve curve{};
    curve.SetRombergOrder(16);

    // Verified with Mathematica.
    double length = curve.GetLength(0.5, 1.5);
    double expectedLength = 6.0382136035885138;
    UTAssert(std::fabs(length - expectedLength) <= maxError, "Invalid length.");
    length = curve.GetLength(0.5, 2.5);
    expectedLength = 21.7485746803553501;
    UTAssert(std::fabs(length - expectedLength) <= maxError, "Invalid length.");

    // Verified with Mathematica.
    double totalLength = curve.GetTotalLength();
    double expectedTotalLength = 38.9637662262898905;
    UTAssert(std::fabs(totalLength - expectedTotalLength) <= maxError, "Invalid total length.");

    double tForLength = curve.GetTime(2.5);  // 0.78568649323640694
    length = curve.GetLength(0.0, tForLength);
    UTAssert(std::fabs(length - 2.5) <= maxError, "Invalid time for length.");

    std::array<Vector2<double>, 9> expectedSubByTime{};
    expectedSubByTime[0] = curve.GetPosition(0.0);
    expectedSubByTime[1] = curve.GetPosition(3.0 * 0.125);
    expectedSubByTime[2] = curve.GetPosition(3.0 * 0.25);
    expectedSubByTime[3] = curve.GetPosition(3.0 * 0.375);
    expectedSubByTime[4] = curve.GetPosition(3.0 * 0.5);
    expectedSubByTime[5] = curve.GetPosition(3.0 * 0.625);
    expectedSubByTime[6] = curve.GetPosition(3.0 * 0.75);
    expectedSubByTime[7] = curve.GetPosition(3.0 * 0.875);
    expectedSubByTime[8] = curve.GetPosition(3.0);
    std::array<Vector2<double>, 9> subByTime{};
    curve.SubdivideByTime(9, subByTime.data());
    for (std::size_t i = 0; i < 9; ++i)
    {
        UTAssert(Length(subByTime[i] - expectedSubByTime[i]) <= maxError,
            "Invalid subdivision by time.");
    }

    std::array<Vector2<double>, 9> expectedSubByLength{};
    expectedSubByLength[0] = curve.GetPosition(curve.GetTime(0.0));
    expectedSubByLength[1] = curve.GetPosition(curve.GetTime(totalLength * 0.125));
    expectedSubByLength[2] = curve.GetPosition(curve.GetTime(totalLength * 0.25));
    expectedSubByLength[3] = curve.GetPosition(curve.GetTime(totalLength * 0.375));
    expectedSubByLength[4] = curve.GetPosition(curve.GetTime(totalLength * 0.5));
    expectedSubByLength[5] = curve.GetPosition(curve.GetTime(totalLength * 0.625));
    expectedSubByLength[6] = curve.GetPosition(curve.GetTime(totalLength * 0.75));
    expectedSubByLength[7] = curve.GetPosition(curve.GetTime(totalLength * 0.875));
    expectedSubByLength[8] = curve.GetPosition(curve.GetTime(totalLength));
    std::array<Vector2<double>, 9> subByLength{};
    curve.SubdivideByLength(9, subByLength.data());
    for (std::size_t i = 0; i < 9; ++i)
    {
        UTAssert(Length(subByLength[i] - expectedSubByLength[i]) <= maxError,
            "Invalid subdivision by length.");
    }
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Curves/ParametricCurve.h>

namespace gtl
{
    template class ParametricCurve<float, 2>;
    template class ParametricCurve<double, 3>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class ParametricCurve<Rational, 4>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ParametricCurve)
