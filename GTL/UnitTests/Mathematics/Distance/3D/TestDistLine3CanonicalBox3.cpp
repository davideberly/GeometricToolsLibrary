#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistLine3CanonicalBox3.h>
//#include <random>
using namespace gtl;

namespace gtl
{
    class UnitTestDistLine3CanonicalBox3
    {
    public:
        UnitTestDistLine3CanonicalBox3();

    private:
        using LBQuery = DCPQuery<double, Line3<double>, CanonicalBox3<double>>;
        using LBOutput = LBQuery::Output;

        void TestDoQuery3D();
        void TestDoQuery2D();
        void TestDoQuery1D();

        // TODO:
        //void TestDoQuery0D();

        void Validate(LBOutput const& output,
            double sqrDistance,
            double parameter,
            Vector<double, 3> const& closest0,
            Vector<double, 3> const& closest1);
    };
}

UnitTestDistLine3CanonicalBox3::UnitTestDistLine3CanonicalBox3()
{
    UTInformation("Mathematics/Distance/3D/DistLine3CanonicalBox3");

    //TestDoQuery3D();
    TestDoQuery2D();
    TestDoQuery1D();

    // TODO:
    //TestDoQuery0D();
}

void UnitTestDistLine3CanonicalBox3::Validate(LBOutput const& output,
    double sqrDistance,
    double,
    Vector<double, 3> const& closest0,
    Vector<double, 3> const& closest1)
{
    double constexpr maxError = 1e-14;

    double error = std::fabs(output.sqrDistance - sqrDistance);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = std::fabs(output.distance - std::sqrt(sqrDistance));
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = Length(output.closest[0] - closest0);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
    error = Length(output.closest[1] - closest1);
    UTAssert(
        error <= maxError,
        "Invalid DCPQuery.");
}

void UnitTestDistLine3CanonicalBox3::TestDoQuery3D()
{
    LBQuery query{};
    LBOutput output{};
    Line3<double> line{};
    CanonicalBox3<double> box{};

    box.extent = { 1.5, 1.0, 0.5 };
    line.origin = { -2.5, -2.0, -1.5 };

    // Face(0,1,2), block 0 (line 173)
    line.direction = { 4.0, 2.0, 1.5 };
    output = query(line, box);
    Validate(output, 0.0, 1.0,
        { 1.5, 0.0, 0.0 },
        { 1.5, 0.0, 0.0 });

    // Face(1,2,0), block 0 (line 173)
    line.direction = { 2.5, 3.0, 1.5 };
    output = query(line, box);
    Validate(output, 0.0, 1.0,
        { 0.0, 1.0, 0.0 },
        { 0.0, 1.0, 0.0 });

    // Face(2,0,1), block 0 (line 173)
    line.direction = Vector3<double>{ 2.5, 2.0, 2.0 };
    output = query(line, box);
    Validate(output, 0.0, 1.0,
        { 0.0, 0.0, 0.5 },
        { 0.0, 0.0, 0.5 });

    // Face(1,2,0), block 3 (line 226)
    line.direction = { 0.55793191403459019, 0.81406026771105011, 0.16130052645888099 };
    output = query(line, box);
    Validate(output, 0.15827336064812059, 3.7802178673177163,
        { -0.39089580981966998, 1.0773251690747552, -0.89024886787238400 },
        { -0.39089580981966998, 1.0, -0.5 });

    // Face(2,0,1), block 5 (line 256) and final block (line 333)
    line.direction = { 0.094441192735703838, 0.35579497668297388, 0.92977997165018555 };
    output = query(line, box);
    Validate(output, 0.66484191766797274, 2.3097961127190483,
        { -2.2818601001385210, -1.1781861459327025, 0.64760216420162564 },
        { -1.5, -1.0, 0.5 });

    // Face(2,0,1), block 1 (line 187)
    line.direction = { 0.93538987402100515, 0.35353072917083106, 0.0078617498630150079 };
    output = query(line, box);
    Validate(output, 0.93382600833496454, 4.2849743834550154,
        { 1.5081216487232210, -0.48512988173881610, -1.4663126032278497 },
        { 1.5, -0.48512988173881633, -0.5 });

    // Face(2,0,1), block 6 (line 265)
    line.direction = { 0.43551379684614877, 0.18039572125427569, 0.88192126434835505 };
    output = query(line, box);
    Validate(output, 0.33514376990024886, 2.3993233495006860,
        { -1.4550615781973371, -1.5671723338445991, 0.61601428197217523 },
        { -1.4550615781973371, -1.0, 0.5 });

    // Face(0,1,2), block 2 (line 202)
    line.direction = { 0.79451402276745131, 0.58925175900627258, 0.14673047445536175 };
    output = query(line, box);
    Validate(output, 0.066017581938709213, 5.0925418425439855,
        { 1.5460959054311907, 1.0007892385320880, -0.75276891925973888 },
        { 1.5, 1.0, -0.5 });

    // Face(2,0,1), block 8 (line 303)
    line.direction = { 0.37309816503075849, 0.41165038936959109, 0.83146961230254524 };
    output = query(line, box);
    Validate(output, 0.0087551448703822032, 2.4514504893362083,
        { -1.5853683207649056, -0.99085945154447530, 0.53830658794726194 },
        { -1.5, -0.99085945154447530, 0.5 });

    // Face(2,0,1), block 4 (line 241)
    line.direction = { 0.20203063598136958, 0.80655151001501957, 0.55557023301960229 };
    output = query(line, box);
    Validate(output, 0.066012800593808763, 3.7328256320656328,
        { -1.7458548635462223, 1.0107161501653059, 0.57384680622824780 },
        { -1.5, 1.0, 0.5 });

    // NOTE: With dense subsampling of (theta,phi) in [0,pi/2]^2, I was unable
    // to trigger block 7 (line 280) or block 9 (line 318). The algorithm
    // should be investigated to determine whether these are actually
    // unreachable code.
}

void UnitTestDistLine3CanonicalBox3::TestDoQuery2D()
{
    // TODO: Verify results with Mathematica.

    //std::default_random_engine dre{};
    //std::uniform_real_distribution<double> urd(-1.0, 1.0);

    LBQuery query{};
    LBOutput output{};
    Line3<double> line{};
    CanonicalBox3<double> box{};

    box.extent = { 1.5, 1.0, 0.5 };

    // (+,+,0), (line 96)
    line.direction = { 1.0, 2.0, 0.0 };

    //for (std::size_t i = 0; i < 128; ++i)
    //{
    //    line.origin = { 3.0 * urd(dre), 3.0 * urd(dre), 3.0 * urd(dre) };
    //    if (i >= 0)
    //    {
    //        output = query(line, box);
    //    }
    //}

    // line 400, line 438 (i = 13) [Mathematica parameter same]
    line.origin = { 0.98163312305851780, -2.2446201991679926, -1.7387455528859630 };
    output = query(line, box);
    Validate(output, 1.5431338996213990, 0.60152145505549348,
        { 1.5831545781140113, -1.0415772890570056, -1.7387455528859630 },
        { 1.5, -1.0, -0.5 });

    // line 400, line 444 (i = 3) [Mathematica parameter same]
    line.origin = { 2.8061696220630155, 1.3550337792713381, 2.8866581506616340 };
    output = query(line, box);
    Validate(output, 5.7093783485684293, -1.2032474361211380,
        { 1.6029221859418774, -1.0514610929709380, 2.8866581506616340 },
        { 1.5, -1.0, 0.5 });

    // line 409, line 438 (i = 19) [Mathematica parameter not same]
    line.origin = { 2.2605444905446648, 1.8490529408472602, -2.8933566265406854 };
    output = query(line, box);
    Validate(output, 5.7281559418062100, -0.76054449054466478,
        { 1.5, 0.32796395975793069, -2.8933566265406854 },
        { 1.5, 0.32796395975793069, -0.5 });

    // line 409, line 444 (i = 16) [Mathematica parameter not same]
    line.origin = { 2.5252487469129705, 1.8451861526184066, 1.2346455100123193 };
    output = query(line, box);
    Validate(output, 0.53970402538126072, -1.0252487469129705,
        { 1.5, -0.20531134120753447, 1.2346455100123193 },
        { 1.5, -0.20531134120753447, 0.5 });

    // line 422, line 438 (i = 4) [Mathematica parameter same]
    line.origin = { -2.3408294949347614, 1.7886351404972969, -1.2178233026522949 };
    output = query(line, box);
    Validate(output, 1.7357409119356004, -0.14728815721196648,
        { -2.4881176521467281, 1.4940588260733638, -1.2178233026522949 },
        { -1.5, 1.0, -0.5 });

    // line 422, line 444 (i = 0) [Mathematica parameter same]
    line.origin = { -2.1871379742193171, 2.0100515399674768, 2.8132066267453881 };
    output = query(line, box);
    Validate(output, 6.4879284124125762, -0.26659302114312727,
        { -2.4537309953624442, 1.4768654976812221, 2.8132066267453881 },
        { -1.5, 1.0, 0.5 });

    // line 431 (i = 1) [Mathematica parameter same]
    line.origin = { -1.6737957421037706, -1.1509976969579805, 0.28332357820711151 };
    output = query(line, box);
    Validate(output, 0.0, 1.0754988484789902,
        { -0.59829689362478033, 1.0, 0.28332357820711151 },
        { -0.59829689362478033, 1.0, 0.28332357820711151 });

    // (+,0,+), (line 103)
    line.direction = { 1.0, 0.0, 2.0 };

    //for (std::size_t i = 0; i < 128; ++i)
    //{
    //    line.origin = { 3.0 * urd(dre), 3.0 * urd(dre), 3.0 * urd(dre) };
    //    if (i >= 0)
    //    {
    //        output = query(line, box);
    //    }
    //}

    // line 400, line 438 (i = 13) [Mathematica parameter same]
    line.origin = { 0.98163312305851780, -2.2446201991679926, -1.7387455528859630 };
    output = query(line, box);
    Validate(output, 1.5572411935642592, 0.59917159654268171,
        { 1.5808047196011996, -2.2446201991679926, -0.54040235980059959 },
        { 1.5, -1.0, -0.5 });

    // line 400, line 444 (i = 16) [Mathematica parameter same]
    line.origin = { 2.5252487469129705, 1.8451861526184066, 1.2346455100123193 };
    output = query(line, box);
    Validate(output, 0.73429212771370456, -0.89890795338752183,
        { 1.6263407935254488, 1.8451861526184066, -0.56317039676272440 },
        { 1.5, 1.0, -0.5 });

    // line 409, line 438 (i = 45) [Mathematica parameter not same]
    line.origin = { 1.8126688154452504, -2.5346578235701300, 0.76430602778639378 };
    output = query(line, box);
    Validate(output, 2.3551746354450085, -0.31266881544525038,
        { 1.5, -2.5346578235701300, 0.13896839689589302 },
        { 1.5, -1.0, 0.13896839689589302 });

    // line 409, line 444 (i = 3) [Mathematica parameter not same]
    line.origin = { 2.8061696220630155, 1.3550337792713381, 2.8866581506616340 };
    output = query(line, box);
    Validate(output, 0.12604898442368920, -1.3061696220630155,
        { 1.5, 1.3550337792713381, 0.27431890653560309 },
        { 1.5, 1.0, 0.27431890653560309 });

    // line 422, line 438 (i = 1) [Mathematica parameter not same]
    line.origin = { -1.6737957421037706, -1.1509976969579805, 0.28332357820711151 };
    output = query(line, box);
    Validate(output, 0.026228055200020590, 0.12142971713790951,
        { -1.5523660249658611, -1.1509976969579805, 0.52618301248293053 },
        { -1.5, -1.0, 0.5 });

    // line 422, line 444 (i = 0) [Mathematica parameter not same]
    line.origin = { -2.1871379742193171, 2.0100515399674768, 2.8132066267453881 };
    output = query(line, box);
    Validate(output, 3.7397096618478294, -0.78785505585429183,
        { -2.9749930300736089, 2.0100515399674768, 1.2374965150368045 },
        { -1.5, 1.0, 0.5 });

    // line 431, line 438 (i = 7) [Mathematica parameter not same]
    line.origin = { -0.83223599190491471, -1.7284540056495985, 1.0881572313615959 };
    output = query(line, box);
    Validate(output, 0.53064523834694532, -0.29407861568079796,
        { -1.1263146075857127, -1.7284540056495985, 0.5 },
        { -1.1263146075857127, -1.0, 0.5 });

    // line 431, line 444 (i = 4) [Mathematica parameter not same]
    line.origin = { -2.3408294949347614, 1.7886351404972969, -1.2178233026522949 };
    output = query(line, box);
    Validate(output, 0.62194538482719119, 0.85891165132614744,
        { -1.4819178436086140, 1.7886351404972969, 0.5 },
        { -1.4819178436086140, 1.0, 0.5 });

    // (0,+,+), (line 117)
    line.direction = { 0.0, 1.0, 2.0 };

    //for (std::size_t i = 0; i < 128; ++i)
    //{
    //    line.origin = { 3.0 * urd(dre), 3.0 * urd(dre), 3.0 * urd(dre) };
    //    if (i >= 0)
    //    {
    //        output = query(line, box);
    //    }
    //}

    // line 400, line 438 (i = 2) [Mathematica parameter same]
    line.origin = { -1.8697081437169134, 2.9572878115068399, 2.9787679532880524 };
    output = query(line, box);
    Validate(output, 0.17466977652894222, -1.7829647436165890,
        { -1.8697081437169134, 1.1743230678902510, -0.58716153394512549 },
        { -1.5, 1.0, -0.5 });

    // line 400, line 444 (i = 19) [Mathematica parameter same]
    line.origin = { 2.2605444905446648, 1.8490529408472602, -2.8933566265406854 };
    output = query(line, box);
    Validate(output, 3.9264410133567083, 0.78753206244682217,
        { 2.2605444905446648, 2.6365850032940825, -1.3182925016470410 },
        { 1.5, 1.0, -0.5 });

    // line 409, line 438 (i = 17) [Mathematica parameter not same]
    line.origin = { -2.9830894046280960, 1.2642232505443012, 0.86376573913164245 };
    output = query(line, box);
    Validate(output, 2.1995541821201203, -0.26422325054430118,
        { -2.9830894046280960, 1.0, 0.33531923804304009 },
        { -1.5, 1.0, 0.33531923804304009 });

    // line 409, line 444 (i = 16) [Mathematica parameter not same]
    line.origin = { 2.5252487469129705, 1.8451861526184066, 1.2346455100123193 };
    output = query(line, box);
    Validate(output, 1.0511349930466163, -0.84518615261840657,
        { 2.5252487469129705, 1.0, -0.45572679522449389 },
        { 1.5, 1.0, -0.45572679522449389 });

    // line 422, line 438 (i = 1) [Mathematica parameter same]
    line.origin = { -1.6737957421037706, -1.1509976969579805, 0.28332357820711151 };
    output = query(line, box);
    Validate(output, 0.031660825374227854, 0.11687010810875149,
        { -1.6737957421037706, -1.0341275888492290, 0.51706379442461448 },
        { -1.5, -1.0, 0.5 });

    // line 422, line 444 (i = 10) [Mathematica parameter same]
    line.origin = { 1.7836794914296541, -1.1006973310860344, 2.2345729204038016 };
    output = query(line, box);
    Validate(output, 0.83006815001471912, -0.67368970194431377,
        { 1.7836794914296541, -1.7743870330303482, 0.88719351651517409 },
        { 1.5, -1.0, 0.5 });

    // line 431, line 438 (i = 0) [Mathematica parameter not same]
    line.origin = { -2.1871379742193171, 2.0100515399674768, 2.8132066267453881 };
    output = query(line, box);
    Validate(output, 0.47215859561422691, -1.1566033133726941,
        { -2.1871379742193171, 0.85344822659478270, 0.5 },
        { -1.5, 0.85344822659478270, 0.5 });

    // line 431, line 444 (i = 3) [Mathematica parameter not same]
    line.origin = { 2.8061696220630155, 1.3550337792713381, 2.8866581506616340 };
    output = query(line, box);
    Validate(output, 1.7060790816002407, -1.1933290753308170,
        { 2.8061696220630155, 0.16170470394052106, 0.5 },
        { 1.5, 0.16170470394052106, 0.5 });
}

void UnitTestDistLine3CanonicalBox3::TestDoQuery1D()
{
    // TODO: Verify results with Mathematica.

    LBQuery query{};
    LBOutput output{};
    Line3<double> line{};
    CanonicalBox3<double> box{};

    box.extent = { 1.5, 1.0, 0.5 };

    // (+,0,0), (line 107)
    line.direction = { 1.0, 0.0, 0.0 };

    line.origin = { 0.0, 0.0, 0.0 };
    output = query(line, box);
    Validate(output, 0.0, 1.5,
        { 1.5, 0.0, 0.0 },
        { 1.5, 0.0, 0.0 });

    line.origin = { 0.0, 2.0, 0.0 };
    output = query(line, box);
    Validate(output, 1.0, 1.5,
        { 1.5, 2.0, 0.0 },
        { 1.5, 1.0, 0.0 });

    line.origin = { 0.0, 0.0, 1.0 };
    output = query(line, box);
    Validate(output, 0.25, 1.5,
        { 1.5, 0.0, 1.0 },
        { 1.5, 0.0, 0.5 });

    line.origin = { 0.0, 2.0, 1.0 };
    output = query(line, box);
    Validate(output, 1.25, 1.5,
        { 1.5, 2.0, 1.0 },
        { 1.5, 1.0, 0.5 });

    // (0,+,0), (line 121)
    line.direction = { 0.0, 1.0, 0.0 };

    line.origin = { 0.0, 0.0, 0.0 };
    output = query(line, box);
    Validate(output, 0.0, 1.0,
        { 0.0, 1.0, 0.0 },
        { 0.0, 1.0, 0.0 });

    line.origin = { 2.0, 0.0, 0.0 };
    output = query(line, box);
    Validate(output, 0.25, 1.0,
        { 2.0, 1.0, 0.0 },
        { 1.5, 1.0, 0.0 });

    line.origin = { 0.0, 0.0, 1.0 };
    output = query(line, box);
    Validate(output, 0.25, 1.0,
        { 0.0, 1.0, 1.0 },
        { 0.0, 1.0, 0.5 });

    line.origin = { 2.0, 0.0, 1.0 };
    output = query(line, box);
    Validate(output, 0.5, 1.0,
        { 2.0, 1.0, 1.0 },
        { 1.5, 1.0, 0.5 });

    // (0,0,+), (line 128)
    line.direction = { 0.0, 0.0, 1.0 };

    line.origin = { 0.0, 0.0, 0.0 };
    output = query(line, box);
    Validate(output, 0.0, 0.5,
        { 0.0, 0.0, 0.5 },
        { 0.0, 0.0, 0.5 });

    line.origin = { 2.0, 0.0, 0.0 };
    output = query(line, box);
    Validate(output, 0.25, 0.5,
        { 2.0, 0.0, 0.5 },
        { 1.5, 0.0, 0.5 });

    line.origin = { 0.0, 3.0, 0.0 };
    output = query(line, box);
    Validate(output, 4.0, 0.5,
        { 0.0, 3.0, 0.5 },
        { 0.0, 1.0, 0.5 });

    line.origin = { 2.0, 3.0, 0.0 };
    output = query(line, box);
    Validate(output, 4.25, 1.0,
        { 2.0, 3.0, 0.5 },
        { 1.5, 1.0, 0.5 });
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistLine3CanonicalBox3.h>

namespace gtl
{
    template class DCPQuery<float, Line<float, 3>, CanonicalBox<float, 3>>;
    template class DCPQuery<double, Line3<double>, CanonicalBox<double, 3>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Line<Rational, 3>, CanonicalBox<Rational, 3>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistLine3CanonicalBox3)
