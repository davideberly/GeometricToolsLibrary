#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistSegment3Circle3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistSegment3Circle3
    {
    public:
        UnitTestDistSegment3Circle3();

    private:
        using SCQuery = DCPQuery<double, Segment3<double>, Circle3<double>>;
        using SCOutput = SCQuery::Output;

        void Validate();

        void Figure7UpperLeft();
        void Figure7UpperRight();
        void Figure7LowerLeft();
        void Figure7LowerRight();
        
        Vector3<double> mOrigin, mDirection;
        Segment3<double> mSegment;
        Circle3<double> mCircle;
        SCQuery mQuery;
        SCOutput mOutput;
        SCOutput mExpectedOutput;
    };
}

UnitTestDistSegment3Circle3::UnitTestDistSegment3Circle3()
    :
    mSegment{},
    mCircle{},
    mQuery{},
    mOutput{},
    mExpectedOutput{}
{
    UTInformation("Mathematics/Distance/3D/DistSegment3Circle3");

    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;

    Figure7UpperLeft();
    Figure7UpperRight();
    Figure7LowerLeft();
    Figure7LowerRight();
}

void UnitTestDistSegment3Circle3::Validate()
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

void UnitTestDistSegment3Circle3::Figure7UpperLeft()
{
    // Case 1: Segment.p0 at line-t of -2, segment.p1 at line-t of -1.
    // segment.p1 is closest
    mOrigin = { 0.0, 2.0, -0.1 };
    mDirection = { 3.0, 2.0, 1.0 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - 2.0 * mDirection;
    mSegment.p[1] = mOrigin - mDirection;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.80178372573727319, 1.4654775161751512, -0.36726124191242437 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.47997394599615106, 0.87728274299958953, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.76447069619386732;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 2.071421333283065
    // t = 1.00000000830137
    // d = 0.7644706957402532
    // P = (-0.80178371908137, 1.46547752061242, -0.36726123969379)
    // K = (-0.4799739397238364, 0.877282746431263, 0.0)

    // Case 2: Segment.p0 at line-t of 0, segment.p1 at line-t of 1.
    // segment.p0 is closest
    mOrigin = { 0.0, 2.0, -0.1 };
    mDirection = { 3.0, 2.0, 1.0 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin;
    mSegment.p[1] = mOrigin + mDirection;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.0, 2.0, -0.10000000000000001 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.0, 1.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 1.0049875621120890;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 1.570796330796946
    // t = -9.98021743716323 * 10 ^ (-9)
    // d = 1.004987557069322
    // P = (-8.00197592043684 * 10 ^ (-9), 1.99999999466535, -0.1000000026673253)
    // K = (-4.002049075298787 * 10 ^ (-9), 1.0, 0.0)

    // Case 3: Segment.p0 at line-t of -2, segment.p1 at line-t of 0.
    // segment.p0 is closest
    mOrigin = { 0.0, 2.0, -0.1 };
    mDirection = { 3.0, 2.0, 1.0 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - 2.0 * mDirection;
    mSegment.p[1] = mOrigin;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.72783742711427679, 1.5147750485904821, -0.34261247570475889 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.43309152078954793, 0.90134995125100914, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.76193753539884856;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 2.018716167206271
    // t = 0.5461136191121929
    // d = 0.7619375353988486
    // P = (-0.7278374270592661, 1.514775048627156, -0.342612475686422)
    // K = (-0.4330915207265456, 0.901349951281281, 0.0)
}

void UnitTestDistSegment3Circle3::Figure7UpperRight()
{
    // Case 1: Segment.p0 at line-t of -2.4, segment.p1 at line-t of -1.4.
    // segment.p1 is closest
    // Block at line 113.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, 0.1 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - 2.4 * mDirection;
    mSegment.p[1] = mOrigin - 1.4 * mDirection;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -1.1930520662939850, 0.10000000000000001, 0.16069479337060150 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.99650561966276407, 0.083525744417696779, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.25441054104885419;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 3.057969482051176
    // t = 1.000000009937846
    // d = 0.2544105340340065
    // P = (-1.193052056405459, 0.1, 0.1606947943594541)
    // K = (-0.99650561960516, 0.0835257451049479, 0.0)

    // Case 2: Segment.p0 at line-t of 1, segment.p1 at line-t of 2.
    // segment.p0 is closest
    // Block at line 123.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, 0.1 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin + mDirection;
    mSegment.p[1] = mOrigin + 2.0 * mDirection;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 1.1950371902099892, 0.10000000000000001, 0.39950371902099890 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.99651715880015423, 0.083387962062088497, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.44641839588778343;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 0.0834849099786853
    // t = -9.96691134502801 * 10 ^ (-9)
    // d = 0.4464183905900232
    // P = (1.195037180292542, 0.1, 0.3995037180292542)
    // K = (0.996517158480206, 0.0833879658855892, 0.0)

    // Case 3: Segment.p0 at line-t of -1.4, segment.p1 at line-t of 1.
    // first critical point is closest
    // Block at line 132.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, 0.1 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - 1.4 * mDirection;
    mSegment.p[1] = mOrigin + mDirection;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -1.0130328162021029, 0.10000000000000001, 0.17869671837978970 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.99516314436412401, 0.098236022411892540, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.17959664236347594;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 3.04319779212702
    // t = 0.07538213370315649
    // d = 0.179596642363537
    // P = (-1.013032802665131, 0.1, 0.1786967197334869)
    // K = (-0.995163129874799, 0.0982361691934011, 0.0)

    // Case 4: Segment.p0 at line-t of -1, segment.p1 at line-t of 0.6.
    // segment.p0 is closest
    // Block at line 144 and then at line 191.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, 0.1 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - mDirection;
    mSegment.p[1] = mOrigin + 0.6 * mDirection;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.79503719020998931, 0.10000000000000001, 0.20049628097900107 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.99218230029333865, 0.12479696704896011, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.28227618158642731;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 3.016489720854362
    // t = -9.97106361861918 * 10 ^ (-9)
    // d = 0.2822761699545588
    // P = (-0.7950372060845158, 0.1, 0.2004962793915484)
    // K = (-0.992184828861765, 0.1247768623445502, 0.0)

    // Case 5: Segment.p0 at line-t of -0.4, segment.p1 at line-t of 0.6.
    // segment.p1 is closest
    // Block at line 144 and then at line 201.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, 0.1 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - 0.4 * mDirection;
    mSegment.p[1] = mOrigin + 0.6 * mDirection;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.79702231412599356, 0.10000000000000001, 0.35970223141259933 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.99222074182973674, 0.12449096145040754, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.40998527858202660;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 0.1248147836596261
    // t = 1.000000009936713
    // d = 0.4099852747420041
    // P = (0.7970223240133923, 0.1, 0.3597022324013392)
    // K = (0.992220742008335, 0.1244909600269376, 0.0)

    // Case 6: Segment.p0 at line-t of -0.8845992627834074,
    // segment.p1/criticalpoint1 at line-t of 0.7607905667529407.
    // segment.p0 and segment.p1 are both closest. 
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, 0.1 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - 0.8845992627834074 * mDirection;
    mSegment.p[1] = mOrigin + 0.7607905667529407 * mDirection;
#if 0
    // The test involves manually moving the program counter to line 211
    // in SelectClosestPoint.
    // Block at line 144 and then at line 211.
    mExpectedOutput.numClosestPairs = 2;
    mExpectedOutput.linearClosest[0] = { -0.68020916490182959, 0.10000000000000001, 0.21197908350981703 };
    mExpectedOutput.linearClosest[1] = { 0.95701490788011134, 0.10000000000000001, 0.37570149078801113 };
    mExpectedOutput.circularClosest[0] = { -0.98936557450493157, 0.14545019760910172, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.99458505695891286, 0.10392576424561906, 0.0 };
    mExpectedOutput.distance = 0.37759573343332326;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
#else
    // Use the natural flow of logic. Line 191 of SelectClosestPoint is reached,
    // segment.p0 is closest.
    // Block at line 144 and then at line 191.
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.68020916490182959, 0.10000000000000001, 0.21197908350981703 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.98936557450493157, 0.14545019760910172, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.37759573343332326;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 2.995624662262845
    // t = -9.98148724006386 * 10 ^ (-9)
    // d = 0.3775957191359491
    // P = (-0.6802091812437608, 0.1, 0.2119790818756239)
    // K = (-0.989365574888421, 0.1454501950005731, 0.0)
#endif

    // Case 7: Segment.p0 at line-t of -1, segment.p1 at line-t of 1.
    // segment.p0 is closest
    // Block at line 150 and then at line 191.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, 0.1 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - mDirection;
    mSegment.p[1] = mOrigin + mDirection;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.79503719020998931, 0.10000000000000001, 0.20049628097900107 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.99218230029333865, 0.12479696704896011, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.28227618158642731;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 3.016493933877493
    // t = -9.97686300675052 * 10 ^ (-9)
    // d = 0.2822761671595107
    // P = (-0.7950372100646889, 0.1, 0.2004962789935312)
    // K = (-0.992185354540766, 0.1247726822458084, 0.0)

    // Case 8: Segment.p0 at line-t of -0.4, segment.p1 at line-t of 1.
    // second critical point is closest
    // Block at line 150 and then at line 201.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, 0.1 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - 0.4 * mDirection;
    mSegment.p[1] = mOrigin + mDirection;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.95701490788011179, 0.10000000000000001, 0.37570149078801118 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.99458505695891297, 0.10392576424561902, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.37759573343332359;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 0.1041137556335691
    // t = 0.829136118919784
    // d = 0.3775957334333236
    // P = (0.957014907616185, 0.1, 0.3757014907616185)
    // K = (0.994585056947612, 0.1039257643537677, 0.0)

    // Case 9: Segment.p0 at line-t of -1.219082892716915, segment.p1 at line-t of 0.
    // Block at line 144 and then at line 191.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, 0.1 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - 1.219082892716915 * mDirection;
    mSegment.p[1] = mOrigin;
#if 0
    // The test setting a breakpoint at line 103 of HasTwoCriticalPoints.
    // The value of critical.parameter[0] is nearly zero but should be
    // zero theoretically. Use the debug watch window to change this
    // parameter to 0. This causes flow to go to the block on line 160 and
    // then to the block on line 191.
    // segment.p0 is closest
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -1.0130328162021049, 0.10000000000000001, 0.17869671837978951 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.99516314436412401, 0.098236022411892346, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.17959664236347594;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
#else
    // The natural flow sees that critical.parameter[0] = 1.5543122344752192e-15
    // and goes to the block at line 166 and then to the block at line 201.
    // The first critical point is closest (which is nearly segment.p0).
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -1.0130328162021032, 0.10000000000000001, 0.17869671837978970 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.99516314436412401, 0.098236022411892526, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.17959664236347594;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
#endif
    // Mathematica:
    // s = 3.043175072851343
    // t = 0.0
    // d = 0.179596643845348
    // P = (-1.013032816202105, 0.1, 0.1786967183797895)
    // K = (-0.995160897763355, 0.0982587785535366, 0.0)

    // Case 10: Segment.p0 at line-t of -1.22, segment.p1 at line-t of 0.6.
    // Block at line 166 and then at line 201.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, 0.1 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - 1.22 * mDirection;
    mSegment.p[1] = mOrigin + 0.6 * mDirection;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -1.0130328162021027, 0.10000000000000001, 0.17869671837978970 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.99516314436412401, 0.098236022411892568, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.17959664236347592;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 3.0431966224342
    // t = 0.0005040608562011616
    // d = 0.1795966423684134
    // P = (-1.013032534133737, 0.1, 0.1786967465866263)
    // K = (-0.995160897763355, 0.0982587785535366, 0.0)
}

void UnitTestDistSegment3Circle3::Figure7LowerLeft()
{
    // Case 1: Segment.p0 at line-t of -2.4, segment.p1 at line-t of -1.4.
    // segment.p1 is closest
    // Block at line 113.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, -0.2 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - 2.4 * mDirection;
    mSegment.p[1] = mOrigin - 1.4 * mDirection;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -1.1728129459672880, 0.10000000000000001, 0.57456258919345760 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.99638463356742268, 0.084956824273937859, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.60122825570067784;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 3.056533297224225
    // t = 1.000000007056278
    // d = 0.6012282523477749
    // P = (-1.172812939048038, 0.1, 0.5745625878096076)
    // K = (-0.996384633529168, 0.0849568247225949, 0.0)

    // Case 2: Segment.p0 at line-t of 1, segment.p1 at line-t of 2.
    // segment.p0 is closest
    // Block at line 123.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, -0.2 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin + mDirection;
    mSegment.p[1] = mOrigin + 2.0 * mDirection;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 1.1805806756909201, 0.10000000000000001, 0.10388386486181597 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.99643179877378563, 0.084401838797728626, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.21200464353726314;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 0.0845024418448088
    // t = -8.25927427550664 * 10 ^ (-9)
    // d = 0.2120046372962262
    // P = (1.180580667592035, 0.1, 0.1038838664815929)
    // K = (0.99643179269982, 0.0844019105057648, 0.0)

    // Case 3: Segment.p0 at line-t of -1.4, segment.p1 at line-t of 1.
    // first critical point is closest
    // Block at line 132.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, -0.2 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - 1.4 * mDirection;
    mSegment.p[1] = mOrigin + mDirection;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 1.0223560922386068, 0.10000000000000001, 0.13552878155227860 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.99525033592815115, 0.097348697140239721, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.13823820767451098;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 0.0975031151949603
    // t = 0.932767480092417
    // d = 0.138238207674511
    // P = (1.022356092172406, 0.1, 0.1355287815655188)
    // K = (0.995250335917921, 0.0973486972448275, 0.0)

    // Case 4: Segment.p0 at line-t of -1, segment.p1 at line-t of 0.
    // segment.p0 is closest
    // Block at line 144 and then at line 191.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, -0.2 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - mDirection;
    mSegment.p[1] = mOrigin;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.78058067569092016, 0.10000000000000001, 0.49611613513818398 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.99189359227025375, 0.12707124620940596, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.53992334699385203;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 3.014176927900929
    // t = -9.90901876330501 * 10 ^ (-9)
    // d = 0.5399233449766587
    // P = (-0.7805806854075124, 0.1, 0.4961161370815025)
    // K = (-0.991893592468334, 0.1270712446632287, 0.0)

    // Case 5: Segment.p0 at line-t of -1, segment.p1 at line-t of 0.6.
    // segment.p0 is closest
    // Block at line 144 and then at line 201.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, -0.2 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - mDirection;
    mSegment.p[1] = mOrigin + 0.6 * mDirection;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.78834840541455198, 0.10000000000000001, 0.18233031891708959 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.99205066248882423, 0.12583911576089962, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.27460264863410488;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 0.1261736412848177
    // t = 1.000000009979111
    // d = 0.2746026315175574
    // P = (0.7883484249851991, 0.1, 0.1823303150029601)
    // K = (0.992050660505391, 0.1258391313972645, 0.0)

    //-0.8247162031111686
    //0.24882647016630985

    // Case 6: Segment.p0 at line-t of -0.8247162031111686
    // segment.p1 at line-t of 0.24882647016630985. Distance
    // is 0.6 at these points (theoretically).
    // segment.p0 and segment.p1 are both closest. 
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, -0.2 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - 0.8247162031111686 * mDirection;
    mSegment.p[1] = mOrigin + 0.24882647016630985 * mDirection;
#if 0
    // The test involves manually moving the program counter to line 211
    // in SelectClosestPoint.
    // Block at line 144 and then at line 211.
    mExpectedOutput.numClosestPairs = 2;
    mExpectedOutput.linearClosest[0] = { -0.60870077169999970, 0.10000000000000001, 0.46174015433999993 };
    mExpectedOutput.linearClosest[1] = { 0.44399442824546670, 0.10000000000000001, 0.25120111435090664 };
    mExpectedOutput.circularClosest[0] = { -0.98677248624735703, 0.16211125927957445, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.97556208751117590, 0.21972394819599558, 0.0 };
    mExpectedOutput.distance = 0.59999999999999998;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
#else
    // Use the natural flow of logic. Line 191 of SelectClosestPoint is reached,
    // segment.p0 is closest.
    // Block at line 144 and then at line 201.
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.44399442824546670, 0.10000000000000001, 0.25120111435090664 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.97556208751117590, 0.21972394819599558, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.59999999999999987;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 2.97876281725438
    // t = -9.96675336308765 * 10 ^ (-9)
    // d = 0.5999999950036674
    // P = (-0.6087007821919532, 0.1, 0.4617401564383906)
    // K = (-0.986772486684967, 0.1621112566158426, 0.0)
#endif

    // Case 7: Segment.p0 at line-t of -0.8, segment.p1 at line-t of 1.
    // segment.p1 is closest
    // Block at line 150 and then at line 201.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, -0.2 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - 0.8 * mDirection;
    mSegment.p[1] = mOrigin + mDirection;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 1.0223560922386070, 0.10000000000000001, 0.13552878155227860 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.99525033592815115, 0.097348697140239707, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.13823820767451103;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 0.0975031151942647
    // t = 0.910356640124131
    // d = 0.138238207674511
    // P = (1.022356092174009, 0.1, 0.1355287815651982)
    // K = (0.995250335917989, 0.0973486972441352, 0.0)

    // Cases 8 and 9 do not exist in this configuration.

    // Case 10: Segment.p0 at line-t of -1.2, segment.p1 at line-t of 0.6.
    // Block at line 166 and then at line 191.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, -0.2 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - 1.2 * mDirection;
    mSegment.p[1] = mOrigin + 0.6 * mDirection;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.78834840541455198, 0.10000000000000001, 0.18233031891708959 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.99205066248882423, 0.12583911576089962, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.27460264863410488;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 0.1261736228324115
    // t = 1.000000009976399
    // d = 0.274602633233398
    // P = (0.7883484230233478, 0.1, 0.1823303153953304)
    // K = (0.992050662827425, 0.1258391130915427, 0.0)
}

void UnitTestDistSegment3Circle3::Figure7LowerRight()
{
    // Case 1: Segment.p0 at line-t of -2, segment.p1 at line-t of -1.6.
    // segment.p1 is closest
    // Block at line 113.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, 0.0 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - 2.0 * mDirection;
    mSegment.p[1] = mOrigin - 1.6 * mDirection;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -1.4000000000000001, 0.10000000000000001, 0.29999999999999999 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.99745869983073498, 0.071247049987909636, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.50285806195820326;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 3.070285188602829
    // t = 1.00000000993758
    // d = 0.5028580587761632
    // P = (-1.399999996024968, 0.1, 0.3)
    // K = (-0.997458699816366, 0.07124705018907141, 0.0)

    // Case 2: Segment.p0 at line-t of 1, segment.p1 at line-t of 2.
    // segment.p0 is closest
    // Block at line 123.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, 0.0 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin + mDirection;
    mSegment.p[1] = mOrigin + 2.0 * mDirection;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 1.2000000000000000, 0.10000000000000001, 0.29999999999999999 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.99654575824487956, 0.083045479853739973, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.36287888370851912;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 0.0831412326736988
    // t = -9.95081431830121 * 10 ^ (-9)
    // d = 0.3628788781294253
    // P = (1.199999990049186, 0.1, 0.3)
    // K = (0.996545758179668, 0.08304548063628511, 0.0)

    // Case 3: Segment.p0 at line-t of -1.4, segment.p1 at line-t of 1.
    // first critical point is closest
    // Block at line 132.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, 0.0 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - 1.4 * mDirection;
    mSegment.p[1] = mOrigin + mDirection;
    mExpectedOutput.numClosestPairs = 2;
    mExpectedOutput.linearClosest[0] = { -0.99498743710661985, 0.10000000000000001, 0.29999999999999999 };
    mExpectedOutput.linearClosest[1] = { 0.99498743710662008, 0.10000000000000001, 0.29999999999999999 };
    mExpectedOutput.circularClosest[0] = { -0.99498743710661997, 0.10000000000000002, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.99498743710662008, 0.10000000000000001, 0.0 };
    mExpectedOutput.distance = 0.29999999999999999;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 0.1001674212653684
    // t = 0.914578098771677
    // d = 0.3
    // P = (0.994987437052024, 0.1, 0.3)
    // K = (0.994987437096239, 0.1000000001032883, 0.0)

    // Case 4: Segment.p0 at line-t of -1, segment.p1 at line-t of 0.
    // segment.p0 is closest
    // Block at line 144 and then at line 191.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, 0.0 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - mDirection;
    mSegment.p[1] = mOrigin;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.80000000000000004, 0.10000000000000001, 0.29999999999999999 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.99227787671366763, 0.12403473458920845, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.35713925902970967;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 3.017237175191743
    // t = -9.94785115641233 * 10 ^ (-9)
    // d = 0.357139253674217
    // P = (-0.800000009947851, 0.1, 0.3)
    // K = (-0.992277816699185, 0.1240352147041234, 0.0)

    // Case 5: Segment.p0 at line-t of -1, segment.p1 at line-t of 0.6.
    // segment.p0 is closest
    // Block at line 144 and then at line 211.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, 0 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - mDirection;
    mSegment.p[1] = mOrigin + 0.6 * mDirection;
    mExpectedOutput.numClosestPairs = 2;
    mExpectedOutput.linearClosest[0] = { -0.80000000000000004, 0.10000000000000001, 0.29999999999999999 };
    mExpectedOutput.linearClosest[1] = { 0.80000000000000004, 0.10000000000000001, 0.29999999999999999 };
    mExpectedOutput.circularClosest[0] = { -0.99227787671366763, 0.12403473458920845, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.99227787671366763, 0.12403473458920845, 0.0 };
    mExpectedOutput.distance = 0.35713925902970967;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 3.01723763509375
    // t = -9.96687858591559 * 10 ^ (-9)
    // d = 0.3571392504441062
    // P = (-0.800000015947006, 0.1, 0.3)
    // K = (-0.992277873743124, 0.1240347583535512, 0.0)

    // Case 6: Segment.p0 at line-t of -0.6698611661530177,
    // segment.p1/criticalpoint1 at line-t of 0.2698611661530176.
    // segment.p0 and segment.p1 are both closest. 
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, 0.0 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - 0.6698611661530177 * mDirection;
    mSegment.p[1] = mOrigin + 0.2698611661530176 * mDirection;
#if 0
    // The test involves manually moving the program counter to line 211
    // in SelectClosestPoint.
    // Block at line 144 and then at line 211.
    mExpectedOutput.numClosestPairs = 2;
    mExpectedOutput.linearClosest[0] = { -0.46986116615301771, 0.10000000000000001, 0.29999999999999999 };
    mExpectedOutput.linearClosest[1] = { 0.46986116615301760, 0.10000000000000001, 0.29999999999999999 };
    mExpectedOutput.circularClosest[0] = { -0.97809341073588241, 0.20816647154392642, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.97809341073588241, 0.20816647154392648, 0.0 };
    mExpectedOutput.distance = 0.59999999999999987;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
#else
    // Use the natural flow of logic. Line 191 of SelectClosestPoint is reached,
    // segment.p0 is closest.
    // Block at line 144 and then at line 191.
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.46986116615301771, 0.10000000000000001, 0.29999999999999999 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.97809341073588241, 0.20816647154392642, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.59999999999999987;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 2.93189266794287
    // t = -9.97906139599118 * 10 ^ (-9)
    // d = 0.5999999920567138
    // P = (-0.4698611755305645, 0.1, 0.2999999999999999)
    // K = (-0.978093411669603, 0.2081664671567379, 0.0)
#endif

    // Case 7: Segment.p0 at line-t of -0.8, segment.p1 at line-t of 1.
    // segment.p1 is closest
    // Block at line 150 and then at line 201.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, 0.0 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - 0.8 * mDirection;
    mSegment.p[1] = mOrigin + mDirection;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.99498743710661985, 0.10000000000000001, 0.29999999999999999 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.99498743710661997, 0.10000000000000002, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.29999999999999999;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 0.100167428222536
    // t = 0.886104129969444
    // d = 0.3000000000000001
    // P = (0.994987433944999, 0.1, 0.3)
    // K = (0.994987436400522, 0.1000000070255826, 0.0)

    // Cases 8 and 9 do not exist in this configuration.

    // Case 10: Segment.p0 at line-t of -1.4, segment.p1 at line-t of 0.6.
    // Block at line 166 and then at line 201.
    mOrigin = { 0.2, 0.1, 0.3 };
    mDirection = { 1.0, 0.0, 0.0 };
    Normalize(mDirection);
    mSegment.p[0] = mOrigin - 1.4 * mDirection;
    mSegment.p[1] = mOrigin + 0.6 * mDirection;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.99498743710661997, 0.10000000000000001, 0.29999999999999999 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.99498743710661997, 0.10000000000000001, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.29999999999999999;
    mExpectedOutput.equidistant = false;
    mOutput = mQuery(mSegment, mCircle);
    Validate();
    // Mathematica:
    // s = 3.041425228955108
    // t = 0.1025062816060169
    // d = 0.2999999999999999
    // P = (-0.994987436787966, 0.1, 0.2999999999999999)
    // K = (-0.994987436759308, 0.1000000034557162, 0.0)
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistSegment3Circle3.h>

namespace gtl
{
    template class DCPQuery<float, Segment3<float>, Circle3<float>>;
    template class DCPQuery<double, Segment3<double>, Circle3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Segment3<Rational>, Circle3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistSegment3Circle3)
