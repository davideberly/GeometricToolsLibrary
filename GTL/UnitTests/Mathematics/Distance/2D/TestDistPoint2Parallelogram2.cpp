#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/2D/DistPoint2Parallelogram2.h>
#include <GTL/Utility/Timer.h>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace gtl;

//#define MEASURE_PERFORMANCE

namespace gtl
{
    class UnitTestDistPoint2Parallelogram2
    {
    public:
        UnitTestDistPoint2Parallelogram2();

    private:
        void Test0();
        void Test1();

#if defined(MEASURE_PERFORMANCE)
        void Performance();
#endif
    };
}

UnitTestDistPoint2Parallelogram2::UnitTestDistPoint2Parallelogram2()
{
    UTInformation("Mathematics/Distance/2D/DistPoint2Parallelogram2");

    Test0();
    Test1();

#if defined(MEASURE_PERFORMANCE)
    Performance();
#endif
}

void UnitTestDistPoint2Parallelogram2::Test0()
{
    // Generate a center point and right-handed basis {V[0],V[1]} for
    // a pgm. The points in this object are represented as
    //   X = C + y[0]*V[0] + y[1]*V[1] = C + B * Y
    // where B is a 2x2 matrix whose columns are V[0] and V[1] and where Y is
    // a 2x2 matrix whose rows are y[0] and y[1].
    Parallelogram2<double> pgm{};
    pgm.center = { -0.72904599140643900, 0.67001717998915900 };
    pgm.axis[0] = { 0.93773554224846278, -0.55793191403459019 };
    pgm.axis[1] = { 1.5346635959439738, -0.37776477094281535 };

    // The query point is P = C + z[0]*V[0] + z[1]*V[1] = C + B * Z. The
    // half-squared-distance function is
    //   (X-P)^T * (X-P) / 2
    //   = [B * (Y-Z)]^T * [B * (Y-Z)] / 2
    //   = (Y-Z)^T * B^T * B * (Y-Z) / 2
    //   = (Y-Z)^T * A * (Y-Z) / 2
    //   = f(Y)
    // The gradient is
    //   gradient(f)(Y) = A * (Y-Z)
    using Query = DCPQuery<double, Vector2<double>, Parallelogram2<double>>;
    Query query{};
    Query::Output output{};
    Vector2<double> P{};

    std::ofstream outFile("Mathematics/Distance/2D/Output/Output0.txt");
    UTAssert(outFile, "Failed to open output file.");
    std::size_t imax = 1024;
    for (std::size_t i : {0, 146, 400, 450, 456, 490, 658, 912, 962, 968})
    {
        double angle = C_TWO_PI<double> * static_cast<double>(i) / static_cast<double>(imax);
        P = pgm.center + 3.0 * Vector2<double>{ std::cos(angle), std::sin(angle) };
        output = query(P, pgm);
        outFile << "i = " << i << std::endl;
        outFile << std::setprecision(17)
            << "P = { " << P[0] << ", " << P[1] << " }" << std::endl;
        outFile << std::setprecision(17) << "distance = " << output.distance << std::endl;
        outFile << std::setprecision(17) << "sqrDistance = " << output.sqrDistance << std::endl;
        outFile << std::setprecision(17)
            << "closest[1] = { " << output.closest[1][0] << ", " << output.closest[1][1] << " }" << std::endl;
        outFile << std::endl;
    }
    outFile.close();

    // i = 0
    // P = {2.2709540085935611, 0.67001717998915900}
    // GT distance = 1.0739016568078934
    // GT sqrDistance = 1.1532647684947384
    // GT closest[1] = {1.7218479729171143, -0.25288440202185647}
    // MT distance = 1.0739
    // MT sqrDistance = 1.15326
    // MT closest[1] = {1.72185, -0.252884}

    // i = 146
    // P = {1.1455324730207206, 3.0122288657054428}
    // GT distance = 2.5113397174287617
    // GT sqrDistance = 6.3068271763351724
    // GT closest[1] = {-0.13211793771092806, 0.85018432308093383}
    // MT distance = 2.51134
    // MT sqrDistance = 6.30683
    // MT closest[1] = {-0.132118, 0.850184}

    // i = 400
    // P = {-3.0480773514946500, 2.5731970324800955}
    // GT distance = 0.97956385947147007
    // GT sqrDistance = 0.95954535478264191
    // GT closest[1] = {-3.2014451295988757, 1.6057138649665645}
    // MT distance = 0.979564
    // MT sqrDistance = 0.959545
    // MT closest[1] = {-3.20145, 1.60571}

    // i = 450
    // P = {-3.5145642328260855, 1.7839687618446722}
    // GT distance = 0.36030317937375583
    // GT sqrDistance = 0.12981838106683688
    // GT closest[1] = {-3.2014451295988757, 1.6057138649665645}
    // MT distance = 0.360303
    // MT sqrDistance = 0.129818
    // MT closest[1] = {-3.20145, 1.60571}

    // i = 456
    // P = {-3.5536781869555010, 1.6806867401658201}
    // GT distance = 0.36012367141086238
    // GT sqrDistance = 0.12968905871043879
    // GT closest[1] = {-3.2014451295988757, 1.6057138649665645}
    // MT distance = 0.360124
    // MT sqrDistance = 0.129689
    // MT closest[1] = {-3.20145, 1.60571}

    // i = 490
    // P = {-3.7017538976897790, 1.0737593055105379}
    // GT distance = 0.73026332014876161
    // GT sqrDistance = 0.53328451675469268
    // GT closest[1] = {-3.2014451295988757, 1.0737593055105379}
    // MT distance = 0.730263
    // MT sqrDistance = 0.533285
    // MT closest[1] = {-3.20145, 1.60571}

    // i = 658
    // P = {-2.6036244558335984, -1.6721945057271241}
    // GT distance = 2.5113397174287608
    // GT sqrDistance = 6.3068271763351689
    // GT closest[1] = {-1.3259740451019502, 0.48985003689738416}
    // MT distance = 2.51134
    // MT sqrDistance = 6.30683
    // MT closest[1] = {-1.32597, 0.48985}

    // i = 912
    // P = {1.5899853686817709, -1.2331626725017788}
    // GT distance = 0.97956385947147151
    // GT sqrDistance = 0.95954535478264480
    // GT closest[1] = {1.7433531467859975, -0.26567950498824655}
    // MT distance = 0.979564
    // MT sqrDistance = 0.959545
    // MT closest[1] = {1.74335, -0.265679}

    // i = 962
    // P = {2.0564722500132078, -0.44393440186635447}
    // GT distance = 0.36030317937375633
    // GT sqrDistance = 0.12981838106683724
    // GT closest[1] = {1.7433531467859975, -0.26567950498824655}
    // MT distance = 0.360303
    // MT sqrDistance = 0.129818
    // MT closest[1] = {1.74335, -0.265679}

    // i = 968
    // P = {2.0955862041426236, -0.34065238018750099}
    // GT distance = 0.36012367141086304
    // GT sqrDistance = 0.12968905871043926
    // GT closest[1] = {1.7433531467859975, -0.26567950498824655}
    // MT distance = 0.360124
    // MT sqrDistance = 0.129689
    // MT closest[1] = {1.74335, -0.26568}
}

void UnitTestDistPoint2Parallelogram2::Test1()
{
    // Generate a center point and right-handed basis {V[0],V[1]} for
    // a pgm. The points in this object are represented as
    //   X = C + y[0]*V[0] + y[1]*V[1] = C + B * Y
    // where B is a 2x2 matrix whose columns are V[0] and V[1] and where Y is
    // a 2x2 matrix whose rows are y[0] and y[1].
    Parallelogram2<double> pgm{};
    pgm.center = { -0.72904599140643900, 0.67001717998915900 };
    pgm.axis[0] = { 0.93773554224846278, -0.55793191403459019 };
    pgm.axis[1] = { 1.5346635959439738, -0.37776477094281535 };
    Vector2<double> temp0 = pgm.axis[1];
    Vector2<double> temp1 = -pgm.axis[0];
    pgm.axis[0] = temp0;
    pgm.axis[1] = temp1;

    // The query point is P = C + z[0]*V[0] + z[1]*V[1] = C + B * Z. The
    // half-squared-distance function is
    //   (X-P)^T * (X-P) / 2
    //   = [B * (Y-Z)]^T * [B * (Y-Z)] / 2
    //   = (Y-Z)^T * B^T * B * (Y-Z) / 2
    //   = (Y-Z)^T * A * (Y-Z) / 2
    //   = f(Y)
    // The gradient is
    //   gradient(f)(Y) = A * (Y-Z)
    using Query = DCPQuery<double, Vector2<double>, Parallelogram2<double>>;
    Query query{};
    Query::Output output{};
    Vector2<double> P{};

    std::ofstream outFile("Mathematics/Distance/2D/Output/Output1.txt");
    UTAssert(outFile, "Failed to open output file.");
    std::size_t imax = 1024;
    for (std::size_t i : {158, 670})
    {
        double angle = C_TWO_PI<double> * static_cast<double>(i) / static_cast<double>(imax);
        P = pgm.center + 3.0 * Vector2<double>{ std::cos(angle), std::sin(angle) };
        output = query(P, pgm);
        outFile << "i = " << i << std::endl;
        outFile << std::setprecision(17)
            << "P = { " << P[0] << ", " << P[1] << " }" << std::endl;
        outFile << std::setprecision(17) << "distance = " << output.distance << std::endl;
        outFile << std::setprecision(17) << "sqrDistance = " << output.sqrDistance << std::endl;
        outFile << std::setprecision(17)
            << "closest[1] = { " << output.closest[1][0] << ", " << output.closest[1][1] << " }" << std::endl;
        outFile << std::endl;
    }
    outFile.close();

    // i = 158
    // P = {0.96814944094440081, 3.1437850883442353}
    // GT distance = 2.5438539217001965
    // GT sqrDistance = 6.4711927749494693
    // GT closest[1] = {-0.13211793771092795, 0.85018432308093383}
    // MT distance = 2.54385
    // MT sqrDistance = 6.47119
    // MT closest[1] = {-0.132118, 0.850184}

    // i = 670
    // P = {-2.4262414237572791, -1.8037507283659162}
    // GT distance = 2.5438539217001961
    // GT sqrDistance = 6.4711927749494667
    // GT closest[1] = {-1.3259740451019499, 0.48985003689738416}
    // MT distance = 2.54385
    // MT sqrDistance = 6.47119
    // MT closest[1] = {-1.32597, 0.48985}
}

#if defined(MEASURE_PERFORMANCE)
void UnitTestDistPoint2Parallelogram2::Performance()
{
    // Generate a center point and right-handed basis {V[0],V[1]} for
    // a pgm. The points in this object are represented as
    //   X = C + y[0]*V[0] + y[1]*V[1] = C + B * Y
    // where B is a 2x2 matrix whose columns are V[0] and V[1] and where Y is
    // a 2x2 matrix whose rows are y[0] and y[1].
    Parallelogram2<double> pgm{};
    pgm.center = { -0.72904599140643900, 0.67001717998915900 };
    pgm.axis[0] = { 0.93773554224846278, -0.55793191403459019 };
    pgm.axis[1] = { 1.5346635959439738, -0.37776477094281535 };

    // The query point is P = C + z[0]*V[0] + z[1]*V[1] = C + B * Z. The
    // half-squared-distance function is
    //   (X-P)^T * (X-P) / 2
    //   = [B * (Y-Z)]^T * [B * (Y-Z)] / 2
    //   = (Y-Z)^T * B^T * B * (Y-Z) / 2
    //   = (Y-Z)^T * A * (Y-Z) / 2
    //   = f(Y)
    // The gradient is
    //   gradient(f)(Y) = A * (Y-Z)
    using Query = DCPQuery<double, Vector2<double>, Parallelogram2<double>>;
    Query query{};
    Query::Output output{};
    Vector2<double> P{};

    Timer timer{};
    std::size_t imax = (1ull << 28);
    for (std::size_t i = 0; i < imax; ++i)
    {
        double angle = C_TWO_PI<double> * static_cast<double>(i) / static_cast<double>(imax);
        P = pgm.center + 3.0 * Vector2<double>{ std::cos(angle), std::sin(angle) };
        output = query(P, pgm);
    }
    auto milliseconds = timer.GetMilliseconds();
    std::cout << "milliseconds = " << milliseconds << std::endl;
    // 12038 milliseconds
}
#endif

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/2D/DistPoint2Parallelogram2.h>

namespace gtl
{
    template class DCPQuery<float, Vector2<float>, Parallelogram2<float>>;
    template class DCPQuery<double, Vector2<double>, Parallelogram2<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Vector2<Rational>, Parallelogram2<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPoint2Parallelogram2)
