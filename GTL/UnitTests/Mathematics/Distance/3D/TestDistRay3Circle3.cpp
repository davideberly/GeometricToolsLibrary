#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistRay3Circle3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistRay3Circle3
    {
    public:
        UnitTestDistRay3Circle3();

    private:
        using RCQuery = DCPQuery<double, Ray3<double>, Circle3<double>>;
        using RCOutput = RCQuery::Output;

        void Validate();

        void Figure7UpperLeft();
        void Figure7UpperRight();
        void Figure7LowerLeft();
        void Figure7LowerRight();

        Ray3<double> mRay;
        Circle3<double> mCircle;
        RCQuery mQuery;
        RCOutput mOutput;
        RCOutput mExpectedOutput;
    };
}

UnitTestDistRay3Circle3::UnitTestDistRay3Circle3()
    :
    mRay{},
    mCircle{},
    mQuery{},
    mOutput{},
    mExpectedOutput{}
{
    UTInformation("Mathematics/Distance/3D/DistRay3Circle3");

    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;

    Figure7UpperLeft();
    Figure7UpperRight();
    Figure7LowerLeft();
    Figure7LowerRight();
}

void UnitTestDistRay3Circle3::Validate()
{
    double constexpr maxError = 1e-16;
    double error{};
    double maxMaxError = 0.0f;

    UTAssert(mOutput.numClosestPairs == mExpectedOutput.numClosestPairs, "invalid number of pairs");
    UTAssert(mOutput.equidistant == mExpectedOutput.equidistant, "invalid classification");

    error = std::fabs(mOutput.distance - mExpectedOutput.distance);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(error <= maxError, "incorrect distance");

    error = Length(mOutput.linearClosest[0] - mExpectedOutput.linearClosest[0]);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(error <= maxError, "invalid line point");

    error = Length(mOutput.linearClosest[1] - mExpectedOutput.linearClosest[1]);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(error <= maxError, "invalid line point");

    error = Length(mOutput.circularClosest[0] - mExpectedOutput.circularClosest[0]);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(error <= maxError, "invalid circle point");

    error = Length(mOutput.circularClosest[1] - mExpectedOutput.circularClosest[1]);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(error <= maxError, "invalid circle point");
}

void UnitTestDistRay3Circle3::Figure7UpperLeft()
{
    // Case 1: Ray origin at line-t of -1.
    // Critical point is closest.
    mRay.origin = { 0.0, 2.0, -0.1 };
    mRay.direction = { 3.0, 2.0, 1.0 };
    Normalize(mRay.direction);
    mRay.origin -= mRay.direction;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.72783742711427690, 1.5147750485904821, -0.34261247570475895 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.43309152078954805, 0.90134995125100914, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.76193753539884868;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mRay, mCircle);
    Validate();
    // Mathematica:
    // s = 2.018716161141303
    // t = 0.0922272477843136
    // d = 0.7619375353988486
    // P = (-0.7278374193942716, 1.514775053737152, -0.3426124731314238)
    // K = (-0.4330915152598869, 0.901349953907968, 0.0)

    // Case 2: Ray origin at line-t of 0.
    // Ray origin is closest.
    mRay.origin = { 0.0, 2.0, -0.1 };
    mRay.direction = { 3.0, 2.0, 1.0 };
    Normalize(mRay.direction);
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.0, 2.0, -0.1 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.0, 1.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 1.0049875621120890;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mRay, mCircle);
    Validate();
    // Mathematica:
    // s = 1.570796341788195
    // t = -9.99551838194068 * 10 ^ (-9)
    // d = 1.004987543214855
    // P = (-2.998655514582206 * 10 ^ (-8), 1.999999980008963, -0.1000000099955184)
    // K = (-1.499329831938435 * 10 ^ (-8), 1.0, 0.0)
}

void UnitTestDistRay3Circle3::Figure7UpperRight()
{
    // Case 1: Ray origin at line-t of -2.
    // First critical point is closest.
    mRay.origin = { 0.2, 0.1, 0.3 };
    mRay.direction = { 1.0, 0.0, 0.1 };
    Normalize(mRay.direction);
    mRay.origin -= 2.0 * mRay.direction;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -1.0130328162021032, 0.10000000000000001, 0.17869671837978968 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.99516314436412401, 0.098236022411892526, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.17959664236347592;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mRay, mCircle);
    Validate();
    // Mathematica:
    // s = 3.043197940222467
    // t = 0.7809171067323374
    // d = 0.1795966423634759
    // P = (-1.013032816750119, 0.1, 0.1786967183249881)
    // K = (-0.995163144423117, 0.098236021814271, 0.0)

    // Case 2: Ray origin at line-t of -1.
    // Ray origin is closest.
    mRay.origin = { 0.2, 0.1, 0.3 };
    mRay.direction = { 1.0, 0.0, 0.1 };
    Normalize(mRay.direction);
    mRay.origin = mRay.origin - mRay.direction;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.79503719020998931, 0.10000000000000001, 0.20049628097900107 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.99218230029333865, 0.12479696704896011, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.28227618158642731;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mRay, mCircle);
    Validate();
    // Mathematica:
    // s = 3.016469459324103
    // t = -9.95347242437238 * 10 ^ (-9)
    // d = 0.2822761739658303
    // P = (-0.7950372001140646, 0.1, 0.2004962799885935)
    // K = (-0.992182300487932, 0.1247969655018685, 0.0)

    // Case 3: Ray origin at line-t of -0.8845992627834074.
    // Theoretically, the horizontal line through the second
    // local minimum of graph and the line-t of graph lead
    // to ray origin and second critical point being equidistant.
    // This case has low probability because of rounding errors.
    mRay.origin = { 0.2, 0.1, 0.3 };
    mRay.direction = { 1.0, 0.0, 0.1 };
    Normalize(mRay.direction);
    mRay.origin = mRay.origin - 0.8845992627834074 * mRay.direction;
#if 0
    // The test involves setting a breakpoint on line 112 in
    // DistRay3Circle3.h, running to it, and setting the program counter to
    // line 130. On line 112, you will notice that the distances are nearly
    // the same. NOTE: In this example, the outputs of the conditionally
    // compiled blocks are the same!
    mExpectedOutput.numClosestPairs = 2;
    mExpectedOutput.linearClosest[0] = { -0.68020916490182959, 0.10000000000000001, 0.21197908350981703 };
    mExpectedOutput.linearClosest[1] = { 0.95701490788011156, 0.10000000000000001, 0.37570149078801118 };
    mExpectedOutput.circularClosest[0] = { -0.98936557450493157, 0.14545019760910172, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.99458505695891297, 0.10392576424561904, 0.0 };
    mExpectedOutput.distance = 0.37759573343332326;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mRay, mCircle);
    Validate();
#else
    // Use the natural flow of logic. Line 116 is reached (ray origin is
    // closest point).
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.68020916490182959, 0.10000000000000001, 0.21197908350981703 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.98936557450493157, 0.14545019760910172, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.37759573343332326;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mRay, mCircle);
    Validate();
    // Mathematica:
    // s = 2.995624661699757
    // t = -9.84782939381153 * 10 ^ (-9)
    // d = 0.377595724860325
    // P = (-0.6802091747007861, 0.1, 0.2119790825299214)
    // K = (-0.98936557480652, 0.1454501955576736, 0.0)
#endif

    // Case 4: Ray origin at line-t of 0.
    // Second critical point is closest.
    mRay.origin = { 0.2, 0.1, 0.3 };
    mRay.direction = { 1.0, 0.0, 0.1 };
    Normalize(mRay.direction);
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.95701490788011179, 0.10000000000000001, 0.37570149078801118 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.99458505695891297, 0.10392576424561902, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.37759573343332359;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mRay, mCircle);
    Validate();
    // Mathematica:
    // s = 0.1041137559554385
    // t = 0.76079056649329
    // d = 0.3775957334333235
    // P = (0.957014907621749, 0.1, 0.3757014907621749)
    // K = (0.994585056914162, 0.1039257646738942, 0.0)

    // Case 4: Ray origin at line-t of 0.
    // Second critical point is closest.
    mRay.origin = { 0.2, 0.1, 0.3 };
    mRay.direction = { 1.0, 0.0, 0.1 };
    Normalize(mRay.direction);
    mRay.origin += mRay.direction;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 1.1950371902099892, 0.10000000000000001, 0.39950371902099890 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.99651715880015423, 0.083387962062088497, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.44641839588778343;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mRay, mCircle);
    Validate();
    // Mathematica:
    // s = 0.0834849068741724
    // t = -9.95785675777384 * 10 ^ (-9)
    // d = 0.4464183905948359
    // P = (1.195037180301551, 0.1, 0.3995037180301551)
    // K = (0.996517158739085, 0.0833879627918888, 0.0)
}

void UnitTestDistRay3Circle3::Figure7LowerLeft()
{
    // Case 1: Ray origin at line-t of -2.
    // Second critical point is closest.
    mRay.origin = { 0.2, 0.1, 0.3 };
    mRay.direction = { 1.0, 0.0, -0.2 };
    Normalize(mRay.direction);
    mRay.origin -= 2.0 * mRay.direction;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 1.0223560922386070, 0.10000000000000001, 0.13552878155227854 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.99525033592815115, 0.097348697140239707, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.13823820767451098;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mRay, mCircle);
    Validate();
    // Mathematica:
    // s = 0.097503115499151
    // t = 2.838641952372836
    // d = 0.1382382076745111
    // P = (1.022356092320508, 0.1, 0.1355287815358984)
    // K = (0.995250335888309, 0.0973486975475734, 0.0)

    // Case 2: Ray origin at line-t of -0.5.
    // Second critical point is closest.
    mRay.origin = { 0.2, 0.1, 0.3 };
    mRay.direction = { 1.0, 0.0, -0.2 };
    Normalize(mRay.direction);
    mRay.origin -= 0.5 * mRay.direction;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 1.0223560922386072, 0.10000000000000001, 0.13552878155227860 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.99525033592815115, 0.097348697140239679, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.13823820767451106;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mRay, mCircle);
    Validate();
    // Mathematica:
    // s = 0.0975031268259687
    // t = 1.338641951725852
    // d = 0.1382382076745115
    // P = (1.022356091686088, 0.1, 0.1355287816627824)
    // K = (0.995250334785658, 0.0973487088205925, 0.0)

    // Case 3: Ray origin at line-t of +2.
    // Ray origin is closest.
    mRay.origin = { 0.2, 0.1, 0.3 };
    mRay.direction = { 1.0, 0.0, -0.2 };
    Normalize(mRay.direction);
    mRay.origin += 2.0 * mRay.direction;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 2.1611613513818404, 0.10000000000000001, -0.092232270276368045 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.99893119390845175, 0.046221962708602855, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 1.1671237329074691;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mRay, mCircle);
    Validate();
    // Mathematica:
    // s =0.04623843759416691 
    // t = -9.99136253278695 * 10 ^ (-9)
    // d = 1.167123722996364
    // P = (2.161161341584503, 0.1, -0.0922322683169007)
    // K = (0.998931193889792, 0.04622196311187424, 0.0)
}

void UnitTestDistRay3Circle3::Figure7LowerRight()
{
    // Case 1: Ray origin at line-t of -2.
    // Both critical points are closest.
    mRay.origin = { 0.2, 0.1, 0.3 };
    mRay.direction = { 1.0, 0.0, 0.0 };
    Normalize(mRay.direction);
    mRay.origin -= 2.0 * mRay.direction;
    mExpectedOutput.numClosestPairs = 2;
    mExpectedOutput.linearClosest[0] = { -0.99498743710661997, 0.10000000000000001, 0.29999999999999999 };
    mExpectedOutput.linearClosest[1] = { 0.99498743710662008, 0.10000000000000001, 0.29999999999999999 };
    mExpectedOutput.circularClosest[0] = { -0.99498743710661997, 0.10000000000000001, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.99498743710662008, 0.10000000000000001, 0.0 };
    mExpectedOutput.distance = 0.29999999999999999;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mRay, mCircle);
    Validate();
    // Mathematica:
    // s = 3.041425232672142
    // t = 0.805012562691496
    // d = 0.3
    // P = (-0.994987437308505, 0.1, 0.3)
    // K = (-0.994987437131011, 0.0999999997573142, 0.0)

    // Case 2: Ray origin at line-t of 0.
    // Second critical point is closest.
    mRay.origin = { 0.2, 0.1, 0.3 };
    mRay.direction = { 1.0, 0.0, 0.0 };
    Normalize(mRay.direction);
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.99498743710661985, 0.10000000000000001, 0.29999999999999999 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.99498743710661997, 0.10000000000000002, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.29999999999999999;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mRay, mCircle);
    Validate();
    // Mathematica:
    // s = 0.1001674247109919
    // t = 0.7949874369286158
    // d = 0.3
    // P = (0.994987436928616, 0.1, 0.3)
    // K = (0.994987436751677, 0.1000000035316404, 0.0)

    // Case 3: Ray origin at line-t of +2.
    // Ray origin is closest.
    mRay.origin = { 0.2, 0.1, 0.3 };
    mRay.direction = { 1.0, 0.0, 0.0 };
    Normalize(mRay.direction);
    mRay.origin += 2.0 * mRay.direction;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 2.2000000000000002, 0.10000000000000001, 0.29999999999999999 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.99896854021029957, 0.045407660918649978, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 1.2391355417753751;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mRay, mCircle);
    Validate();
    // Mathematica:
    // s = 0.04542327985089073
    // t = -9.99167382335248 * 10 ^ (-9)
    // d = 1.23913553209095
    // P = (2.199999990008326, 0.1, 0.3)
    // K = (0.998968540190805, 0.04540766134752088, 0.0)
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistRay3Circle3.h>

namespace gtl
{
    template class DCPQuery<float, Ray3<float>, Circle3<float>>;
    template class DCPQuery<double, Ray3<double>, Circle3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Ray3<Rational>, Circle3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistRay3Circle3)
