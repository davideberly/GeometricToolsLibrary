#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistPoint3Frustum3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

//#define TEST_DISTPOINT3FRUSTUM3_GENERATE
#if defined(TEST_DISTPOINT3FRUSTUM3_GENERATE)
#include <map>
#include <random>
#include <fstream>
#include <iomanip>
#endif

namespace gtl
{
    class UnitTestDistPoint3Frustum3
    {
    public:
        UnitTestDistPoint3Frustum3();

    private:
        using PFQuery = DCPQuery<double, Vector3<double>, Frustum3<double>>;
        using PFOutput = typename PFQuery::Output;

        void Test();

        void Validate(PFOutput const& output,
            double sqrDistance, std::array<Vector3<double>, 2> const& closest);
    };
}

UnitTestDistPoint3Frustum3::UnitTestDistPoint3Frustum3()
{
    UTInformation("Mathematics/Distance/3D/DistPoint3Frustum3");

    Test();
}

void UnitTestDistPoint3Frustum3::Validate(PFOutput const& output,
    double sqrDistance, std::array<Vector3<double>, 2> const& closest)
{
    double const maxError = 1e-14;

    double error = std::fabs(output.sqrDistance - sqrDistance);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(sqrDistance));
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - closest[0]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = Length(output.closest[1] - closest[1]);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
}

void UnitTestDistPoint3Frustum3::Test()
{
    PFQuery query{};
    PFOutput output{};

    Vector3<double> origin{ 0.0, 0.0, 0.0 };
    Vector3<double> dVector{ 0.0, 0.0, 1.0 };
    Vector3<double> uVector{ 0.0, 1.0, 0.0 };
    Vector3<double> rVector{ -1.0, 0.0, 0.0 };
    double dMin = 1.0;
    double dMax = 2.0;
    double uBound = 3.0;
    double rBound = 4.0;
    Frustum3<double> frustum(origin, dVector, uVector, rVector, dMin, dMax, uBound, rBound);
    Vector3<double> point{};
    Vector3<double> closest{};
    double sqrDistance = 0.0;

    std::vector<std::array<double, 7>> expected
    {
        std::array<double, 7>{ 0.14586664042855995715, -1.3443470262753631772, -0.298345755158441861, 2.381924914647578273, -1.3443470262753631772, -0.298345755158441861, 2 },
        std::array<double, 7>{ 0.83700264751788244766, -4.4385919568262117707, -6.9138160225983931184, 2.0440786156807977392, -4.4385919568262117707, -6, 2 },
        std::array<double, 7>{ 49.418341398771339357, 14.966237984336082434, 5.4201351170853513395, 2.9433290753308170018, 8, 5.4201351170853513395, 2 },
        std::array<double, 7>{ 18.414659684505242154, -11.664735862503023966, 8.0402061598699070544, 2.906603313372694064, -8, 6, 2 },
        std::array<double, 7>{ 0.0079491642630178018653, -3.8585972258364726883, -0.99607438819826121801, 0.91084191420281723772, -3.8585972258364726883, -0.99607438819826121801, 1 },
        std::array<double, 7>{ 4.5361972349812020866, -2.4931942046267767665, -7.827235871988472482, 0.9057393806319591345, -2.4931942046267767665, -6, 2 },
        std::array<double, 7>{ 3.5210823055103666057, -3.765346017466425721, 6.2981108356786918989, 0.12141333444038136802, -3.765346017466425721, 5.7047237524429368349, 1.9015745841476459077 },
        std::array<double, 7>{ 0.83075251077912715125, -3.218022097591612507, -3.1760696551720979386, 0.1057114630575287928, -3.218022097591612507, -3, 1 },
        std::array<double, 7>{ 15.6951443901039287, 11.605701985293737977, 2.8886403172939996153, 0.35864154330849329355, 8, 2.8886403172939996153, 2 },
        std::array<double, 7>{ 1.2397663436955896898, -7.2719028276079047401, -0.18139227253992729061, 0.67025982734965239285, -7.0018520324191229065, -0.18139227253992729061, 1.7504630081047807266 },
        std::array<double, 7>{ 0.41000715050955383045, -4.0719254335638996878, 0.10293974978367614881, 0.36373442453940751928, -4, 0.10293974978367614881, 1 },
        std::array<double, 7>{ 21.33974341993299717, -12.484423972985394968, 7.1545405619891866422, 0.8910883486738525594, -8, 7.1545405619891866422, 2 },
        std::array<double, 7>{ 0.9802317492525222109, 7.9009346046499508986, -4.2592089800137751254, 0.95469642323750658619, 7.6608081980793674148, -4.2592089800137751254, 1.9152020495198418537 },
        std::array<double, 7>{ 10.746529657006854919, 5.2353766563120949229, -8.9784807966719704098, 0.63062722355701850141, 5.2353766563120949229, -6, 2 },
        std::array<double, 7>{ 2.3055545721690409167, 5.1238238200550405566, 5.9985833546039479813, 0.39898809388557981981, 5.1238238200550405566, 5.5184214473092270126, 1.8394738157697423375 },
        std::array<double, 7>{ 8.9617265819462588183, 10.331492712334583217, -7.5485766902573709913, 0.93803081557489875753, 8, -6, 2 },
        std::array<double, 7>{ 2.7395127494594202489, -7.4905390662758026821, -6.1406816564750625886, 0.30221547470218329545, -7.4902179552662424555, -5.6176634664496818417, 1.8725544888165606139 },
        std::array<double, 7>{ 0.49611645563122386271, -4.0313391366876345501, -3.1098566654860242409, 0.30497062854064255211, -4, -3, 1 },
        std::array<double, 7>{ 0, -2.7946715228328358194, -1.8440372055904763471, 1.7428700329917190537, -2.7946715228328358194, -1.8440372055904763471, 1.7428700329917190537 },
        std::array<double, 7>{ 0.40718249376529197514, -1.4069496160820307296, 6.5740110939277194291, 1.7212639997897745125, -1.4069496160820307296, 6, 2 },
        std::array<double, 7>{ 0.2258124561102093919, -3.2403673628066833601, 5.7755338722341136304, 1.4242760418517041998, -3.2403673628066833601, 5.6252632975662137937, 1.8750877658554045979 },
        std::array<double, 7>{ 0.98756957929851840738, -8.9269106245534430855, -4.6039907878319219492, 1.6416617891035558685, -8, -4.6039907878319219492, 2 },
        std::array<double, 7>{ 0.46727704716153473097, -7.0545785711393200756, 1.8758141264163583628, 1.0590299519011310281, -6.8887868791666848622, 1.8758141264163583628, 1.7221967197916712156 },
        std::array<double, 7>{ 61.580800954448434936, -15.846928498581899447, -9.3008516146516733158, 1.9192900712944584996, -8, -9.3008516146516733158, 2 },
        std::array<double, 7>{ 0.13943462576597928648, 6.2474522807985088946, 4.3156749771116622583, 1.1769614075942897991, 6.1568871837149004023, 4.3156749771116622583, 1.5392217959287251006 },
        std::array<double, 7>{ 17.142156902132509089, 9.6675670157080020317, -10.138631294280520123, 1.8821530138931970022, 9.6675670157080020317, -6, 2 },
        std::array<double, 7>{ 0.067888394101229487809, 5.2410211810277971267, 4.4246735394813470066, 1.2002431341370614071, 5.2410211810277971267, 4.3422791257743309501, 1.4474263752581102427 },
        std::array<double, 7>{ 67.331871006049269113, -14.361074374873066262, -11.125409961891174149, 1.226193482885281183, -8, -6, 2 },
        std::array<double, 7>{ 0.67144026821155422535, -7.4613883013665827093, -5.5314241752144734932, 1.0236153573866872968, -7.3020678597686830003, -5.4765508948265120281, 1.8255169649421707501 }
    };

    for (auto const& e : expected)
    {
        sqrDistance = e[0];
        point = { e[1], e[2], e[3] };
        closest = { e[4], e[5], e[6] };
        output = query(point, frustum);
        Validate(output, sqrDistance, { point, closest });
    }

#if defined(TEST_DISTPOINT3FRUSTUM3_GENERATE)
    // Randomly generate points in a bounding box containing the frustum.
    double xBound = 2.0 * dMax * rBound / dMin;
    double yBound = 2.0 * dMax * uBound / dMin;
    std::default_random_engine dre{};
    std::uniform_real_distribution<double> xRnd(-xBound, xBound);
    std::uniform_real_distribution<double> yRnd(-yBound, yBound);
    std::uniform_real_distribution<double> zRnd(dMin - 1.0, dMax + 1.0);
    std::map<std::size_t, PFOutput> cases{};
    while (cases.size() < 29)
    {
        point = { xRnd(dre), yRnd(dre), zRnd(dre) };
        output = query(point, frustum);
        auto iter = cases.find(output.lineNumber);
        if (iter == cases.end())
        {
            cases.insert(std::make_pair(output.lineNumber, output));
        }
    }

    std::ofstream outfile("Mathematics/Distance/3D/Output/TestDistPoint3Frustum3_data.txt");
    for (auto const& result : cases)
    {
        outfile << std::setprecision(20)
            << "std::array<double, 7>{ "
            << result.second.sqrDistance << ", "
            << result.second.closest[0][0] << ", "
            << result.second.closest[0][1] << ", "
            << result.second.closest[0][2] << ", "
            << result.second.closest[1][0] << ", "
            << result.second.closest[1][1] << ", "
            << result.second.closest[1][2] << " }, "
            << std::endl;
    }
    outfile.close();
#endif
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistPoint3Frustum3.h>

namespace gtl
{
    template class DCPQuery<float, Vector3<float>, Frustum3<float>>;
    template class DCPQuery<double, Vector3<double>, Frustum3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Vector3<Rational>, Frustum3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistPoint3Frustum3)
