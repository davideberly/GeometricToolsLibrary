#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Distance/3D/DistLine3Circle3.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistLine3Circle3
    {
    public:
        UnitTestDistLine3Circle3();

    private:
        using LCQuery = DCPQuery<double, Line3<double>, Circle3<double>>;
        using LCOutput = LCQuery::Output;
        using LCCritical = LCQuery::Critical;

        void Validate();

        // PDF Section 4.1.1
        void LinePerpContainCenter();

        // PDF Section 4.1.2
        void LinePerpNotContainCenter();

        // PDF Section 4.2.1
        void LineNotPerpOriginOnNormalLineDotMDPos();
        void LineNotPerpOriginOnNormalLineDotMDNeg();
        void LineNotPerpOriginOnNormalLineDotMDZero();

        // PDF Section 4.2.2
        void LineNotPerpOriginNotOnNormalLine1();
        void LineNotPerpOriginNotOnNormalLine2();
        void LineNotPerpOriginNotOnNormalLine3();
        void LineNotPerpOriginNotOnNormalLine4();
        void LineNotPerpOriginNotOnNormalLine5();
        void LineNotPerpOriginNotOnNormalLine6();
        void LineNotPerpOriginNotOnNormalLine7();
        void LineNotPerpOriginNotOnNormalLine8();

        Line3<double> mLine;
        Circle3<double> mCircle;
        LCQuery mQuery;
        LCOutput mOutput;
        LCCritical mCritical;
        LCOutput mExpectedOutput;
        LCCritical mExpectedCritical;
        Matrix3x3<double> mRot;
        Vector3<double> mTrn;
        double mDirectionScale;
    };
}

UnitTestDistLine3Circle3::UnitTestDistLine3Circle3()
    :
    mLine{},
    mCircle{},
    mQuery{},
    mOutput{},
    mCritical{},
    mExpectedOutput{},
    mExpectedCritical{},
    mRot{},
    mTrn{},
    mDirectionScale(1.234567)
{
    UTInformation("Mathematics/Distance/3D/DistLine3Circle3");

    Quaternion<double> q{ 0.469646722, 0.568877578, -0.511022866, 0.441209435 };
    RigidMotion<double>::Convert(q, mRot);
    mTrn = Vector3<double>{ -0.746026397, 0.937735558, 0.826751709 };

    LinePerpContainCenter();
    LinePerpNotContainCenter();
    LineNotPerpOriginOnNormalLineDotMDPos();
    LineNotPerpOriginOnNormalLineDotMDNeg();
    LineNotPerpOriginOnNormalLineDotMDZero();
    LineNotPerpOriginNotOnNormalLine1();
    LineNotPerpOriginNotOnNormalLine2();
    LineNotPerpOriginNotOnNormalLine3();
    LineNotPerpOriginNotOnNormalLine4();
    LineNotPerpOriginNotOnNormalLine5();
    LineNotPerpOriginNotOnNormalLine6();
    LineNotPerpOriginNotOnNormalLine7();
    LineNotPerpOriginNotOnNormalLine8();
}

void UnitTestDistLine3Circle3::Validate()
{
    double constexpr maxError = 1e-16;
    double error{};
    double maxMaxError = 0.0;

    UTAssert(mOutput.numClosestPairs == mExpectedOutput.numClosestPairs, "invalid number of pairs");
    UTAssert(mOutput.equidistant == mExpectedOutput.equidistant, "invalid classification");

    error = std::fabs(mOutput.distance - mExpectedOutput.distance);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(error <= maxError, "incorrect distance");

    error = Length(mOutput.linearClosest[0] - mExpectedOutput.linearClosest[0]);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(error <= maxError, "invalid line point 0");

    error = Length(mOutput.linearClosest[1] - mExpectedOutput.linearClosest[1]);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(error <= maxError, "invalid line point 1");

    error = Length(mOutput.circularClosest[0] - mExpectedOutput.circularClosest[0]);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(error <= maxError, "invalid circle point 0");

    error = Length(mOutput.circularClosest[1] - mExpectedOutput.circularClosest[1]);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(error <= maxError, "invalid circle point 1");

    UTAssert(mCritical.numPoints == mExpectedCritical.numPoints, "invalid number of critical points");

    error = std::fabs(mCritical.parameter[0] - mExpectedCritical.parameter[0]);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(error <= maxError, "incorrect critical parameter 0");

    error = std::fabs(mCritical.parameter[1] - mExpectedCritical.parameter[1]);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(error <= maxError, "incorrect critical parameter 1");

    error = std::fabs(mCritical.distance[0] - mExpectedCritical.distance[0]);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(error <= maxError, "incorrect critical distance 0");

    error = std::fabs(mCritical.distance[1] - mExpectedCritical.distance[1]);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(error <= maxError, "incorrect critical distance 1");
}

void UnitTestDistLine3Circle3::LinePerpContainCenter()
{
    // unit direction

    // Block 1
    mLine.origin = { 0.0, 0.0, 1.25 };
    mLine.direction = { 0.0, 0.0, 1.0 };
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -1.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 1.0;
    mExpectedOutput.equidistant = true;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { -1.25, 0.0 };
    mExpectedCritical.distance = { 1.0, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 5.738229511048757
    // t = -1.25
    // d = 1.0
    // P = (0.0, 0.0, 0.0)
    // K = (0.85515020603052, -0.5183802900631539, 0.0)
    // The Mathematica output varies with each run (RandomSearch).

    // Block 2 and then Block 14
    // rounding errors cause miss of LinePerpendicularToPlaneContainCenter
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.74602639699999995, 0.93773555799999997, 0.82675170899999995 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.74407548518394884, 0.84937762042028142, 1.8228385871435404 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.99999999999999989;
    mExpectedOutput.equidistant = false;  // theoretically true, but rounding error changed that
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { -1.25, 0.0 };
    mExpectedCritical.distance = { 0.99999999999999989, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 2.451421905277174
    // t = -1.250000009726857
    // d = 0.999999802442446
    // P = (-0.746026397213873, 0.937735567686433, 0.826751709859655)
    // K = (0.01200274639386889, 0.896703767666333, 1.477680134276161)
    // The Mathematica output varies with each run (RandomSearch).

    // non-unit direction

    // Block 1
    mLine.origin = { 0.0, 0.0, 1.25 };
    mLine.direction = { 0.0, 0.0, 1.0 };
    mLine.direction *= mDirectionScale;
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -1.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 1.0;
    mExpectedOutput.equidistant = true;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { -1.0125007391255396, 0.0 };
    mExpectedCritical.distance = { 1.0, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 2.415092993084293
    // t = -1.012500739125093
    // d = 1.0
    // P = (0.0, 0.0, 5.517808432387028 * 10^ (-13))
    // K = (-0.7475041026251979 , 0.6642571915745417, 0.0)
    // The Mathematica output varies with each run (RandomSearch).

    // Block 2 and then Block 14
    // rounding errors cause miss of LinePerpendicularToPlaneContainCenter
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.74602639699999995, 0.93773555799999975, 0.82675170899999995 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.78686415701767498, 0.84851208579132775, 1.8219257889296205 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.99999999999999989;
    mExpectedOutput.equidistant = false;  // theoretically true, but rounding error changed that
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { -1.0125007391255394, 0.0 };
    mExpectedCritical.distance = { 0.99999999999999989, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 2.451742802200593
    // t = -1.01250073908203
    // d = 0.999999802442457
    // P = (-0.7460263969988189, 0.937735557946508, 0.826751708995253)
    // K = (0.01179353079722667, 0.896677678623498, 1.477922049586443)
    // The Mathematica output varies with each run (RandomSearch).
}

void UnitTestDistLine3Circle3::LinePerpNotContainCenter()
{
    // unit direction

    // Block 2 and then Block 
    mLine.origin = { 0.5, 0.0, 1.0 };
    mLine.direction = { 0.0, 0.0, 1.0 };
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.5, 0.0, 0.0 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 1.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.5;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { -1.0, 0.0 };
    mExpectedCritical.distance = { 0.5, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 0.0001122025890019211
    // t = -0.999999999999093
    // d = 0.5000000062947104
    // P = (0.5, 0.0, 9.07163233421215 * 10 ^ (-13))
    // K = (0.99999999370529, 0.0001122025887664935, 0.0)

    // Block 2 and then Block 14
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.83079246403146856, 0.97943887911696725, 0.33575733513918415 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.91555855538227870, 1.0211422121985960, -0.15523717958766725 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.50000007172486238;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { -0.99999994112522694, 0.0 };
    mExpectedCritical.distance = { 0.50000007172486238, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 0.0000691758603316921
    // t = -1.000000058554832
    // d = 0.499999930676855
    // P = (-0.830792466613497, 0.979438996058539, 0.3357573455175522)
    // K = (-0.915490375710137, 1.021144847413672,-0.155248577903195)

    // non-unit direction

    // Block 2 and then Block 14
    mLine.origin = { 0.5, 0.0, 1.0 };
    mLine.direction = { 0.0, 0.0, 1.0 };
    mLine.direction *= mDirectionScale;
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.5, 0.0, 0.0 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 1.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.5;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { -0.81000059130043167, 0.0 };
    mExpectedCritical.distance = { 0.5, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 0.0001197113912866941
    // t = -0.810000591299885
    // d = 0.5000000071654086
    // P = (0.5, 0.0, 6.751266212745577 * 10 ^ (-13))
    // K = (0.999999992834592, 0.000119711391000767, 0.0)

    // Block 2 and then Block 14
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.83079246403146856, 0.97943887911696714, 0.33575733513918415 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.91555855538227870, 1.0211422121985960, -0.15523717958766725 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.50000007172486238;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { -0.81000054361183049, 0.0 };
    mExpectedCritical.distance = { 0.50000007172486238, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 0.00007094181325176646
    // t = -0.810000638716296
    // d = 0.499999930800807
    // P = (-0.830792466613128, 0.979438996041837, 0.33575734551607)
    // K = (-0.915488635732488, 1.021144911993388, -0.1552488726848393)
}

void UnitTestDistLine3Circle3::LineNotPerpOriginOnNormalLineDotMDPos()
{
    // unit direction

    // Block 3 and then Block 15
    // NxD = (0,0,0), Dot(M,D) > 0
    mLine.origin = { 0.0, 0.0, 0.125 };
    mLine.direction = { 1.0, 0.0, 1.0 };
    Normalize(mLine.direction);
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.56250000000000011, 0.0, -0.43750000000000011 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -1.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.61871843353822908;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 2;
    mExpectedCritical.parameter = { -0.79549512883486617, 0.61871843353822908 };
    mExpectedCritical.distance = { 0.61871843353822908, 0.79549512883486595 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 3.141592653598036
    // t = -0.7954951288265121
    // d = 0.6187184335382291
    // P = (-0.5624999999940929, 0.0, -0.4374999999940929)
    // K = (-1.0, -8.24306144855695 * 10 ^ (-12), 0.0)

    // Block 8 and then Block 15
    // NxD = (4.3368086899420177e-17, 9.2157184661267877e-19, 4.3368086899420177e-19), Dot(M,D) > 0
    // rounding errors
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.66028425963322801, 1.3265010733506712, 1.4177864618801925 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.57649424318373410, 0.85432911059863414, 1.8087406159405526 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.61871825562429406;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 2;
    mExpectedCritical.parameter = { -0.79549521297617731, 0.61871851591993432 };
    mExpectedCritical.distance = { 0.61871825562429406, 0.79549484095566936 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 3.14159244063491
    // t = -0.7954951226651349
    // d = 0.6187183842894352
    // P = (-0.6602842690553378, 1.326501015082837, 1.417786393526959)
    // K = (-0.5764940505279863, 0.854328806294747, 1.808740410752826)

    // non-unit direction

    // Block 3 and then Block 15
    // NxD = (0,0,0), Dot(M,D) > 0
    mLine.origin = { 0.0, 0.0, 0.125 };
    mLine.direction = { 1.0, 0.0, 1.0 };
    Normalize(mLine.direction);
    mLine.direction *= mDirectionScale;
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.56250000000000011, 0.0, -0.43750000000000011 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -1.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.61871843353822908;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 2;
    mExpectedCritical.parameter = { -0.64435152473285462, 0.50116229701444237 };
    mExpectedCritical.distance = { 0.61871843353822908, 0.79549512883486595 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 3.141592653592656
    // t = -0.6443515247287278
    // d = 0.6187184335382291
    // P = (-0.5624999999963975, 0.0, -0.4374999999963975)
    // K = (-1.0, -2.862920671223439 * 10 ^ (-12), 0.0)

    // Block 8 and then Block 15
    // NxD = (4.3368086899420177e-17, 9.2157184661267877e-19, 4.3368086899420177e-19), Dot(M,D) > 0
    // rounding errors
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.66028425963322801, 1.3265010733506708, 1.4177864618801923 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.57649424318373421, 0.85432911059863414, 1.8087406159405526 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.61871825562429383;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 2;
    mExpectedCritical.parameter = { -0.64435159288736621, 0.50116236374367240 };
    mExpectedCritical.distance = { 0.61871825562429383, 0.79549484095566936 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 3.14159244059874
    // t = -0.6443515197900631
    // d = 0.6187183842894349
    // P = (-0.660284269048293, 1.326501015126403, 1.417786393578066)
    // K = (-0.5764940504923489, 0.85432880629607, 1.808740410746786)
}

void UnitTestDistLine3Circle3::LineNotPerpOriginOnNormalLineDotMDNeg()
{
    // unit direction

    // Block 3 and then Block 16
    // NxD = (0,0,0), Dot(M,D) < 0
    mLine.origin = { 0.0, 0.0, -0.125 };
    mLine.direction = { 1.0, 0.0, 1.0 };
    Normalize(mLine.direction);
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.56250000000000011, 0.0, 0.43750000000000011 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 1.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.61871843353822908;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 2;
    mExpectedCritical.parameter = { -0.61871843353822908, 0.79549512883486617 };
    mExpectedCritical.distance = { 0.79549512883486595, 0.61871843353822908 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 0.00006156203754600344
    // t = 0.7954951288043062
    // d = 0.6187184352609918
    // P = (0.562499999978391, 0.0, 0.437499999978391)
    // K = (0.999999998105058, 0.00006156203750711791, 0.0)

    // Block 9 and then Block 16
    // NxD = (-3.2959746043559335e-17, -9.2157184661267877e-19, 2.1684043449710089e-18), Dot(M,D) < 0
    // rounding errors
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.83176853436677189, 0.54897004264932892, 0.23571695611980747 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.91555855081626569, 1.0211420054013658, -0.15523719794055257 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.61871825562429394;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 2;
    mExpectedCritical.parameter = { -0.61871851591993421, 0.79549521297617720 };
    mExpectedCritical.distance = { 0.79549484095566925, 0.61871825562429394 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 0.00005576281994056876
    // t = 0.7954951165394313
    // d = 0.6187183364757594
    // P = (-0.05894315827730756, 0.6537017214879336, 1.531031894484052)
    // K = (0.4118796392765494, 1.053326042302017, 1.568961769368807)

    // non-unit direction

    // Block 3 and then Block 16
    // NxD = (0,0,0), Dot(M,D) < 0
    mLine.origin = { 0.0, 0.0, -0.125 };
    mLine.direction = { 1.0, 0.0, 1.0 };
    Normalize(mLine.direction);
    mLine.direction *= mDirectionScale;
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.56250000000000011, 0.0, 0.43750000000000011 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 1.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.61871843353822908;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 2;
    mExpectedCritical.parameter = { -0.50116229701444237, 0.64435152473285462 };
    mExpectedCritical.distance = { 0.79549512883486595, 0.61871843353822908 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 0.00006075240939201042
    // t = 0.6443515247088165
    // d = 0.6187184352159761
    // P = (0.5624999999790155, 0.0, 0.4374999999790155)
    // K = (0.999999998154572, 0.00006075240935463903, 0.0)

    // Block 9 and then Block 16
    // NxD = (-3.2959746043559335e-17, -9.2157184661267877e-19, 2.1684043449710089e-18), Dot(M,D) < 0
    // rounding errors
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.83176853436677189, 0.54897004264932892, 0.23571695611980725 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.91555855081626569, 1.0211420054013658, -0.15523719794055257 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.61871825562429372;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 2;
    mExpectedCritical.parameter = { -0.50116236374367240, 0.64435159288736643 };
    mExpectedCritical.distance = { 0.79549484095566936, 0.61871825562429372 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 0.00005768073440166305
    // t = 0.6443515197290513
    // d = 0.6187183858129992
    // P = (-0.831768524943849, 0.5489701009221948, 0.2357170244789436)
    // K = (-0.915501701742197, 1.021144427038256, -0.1552466590047569)
}

void UnitTestDistLine3Circle3::LineNotPerpOriginOnNormalLineDotMDZero()
{
    // unit direction

    // Block 3 and then Block 17
    // Dot(M,D) = 0
    mLine.origin = { 0.0, 0.0, 0.0 };
    mLine.direction = { 1.0, 0.0, 1.0 };
    Normalize(mLine.direction);
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 2;
    mExpectedOutput.linearClosest[0] = { -0.5, 0.0, -0.5 };
    mExpectedOutput.linearClosest[1] = { 0.5, 0.0, 0.5 };
    mExpectedOutput.circularClosest[0] = { -1.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 1.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.70710678118654757;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 2;
    mExpectedCritical.parameter = { -0.70710678118654757, 0.70710678118654757 };
    mExpectedCritical.distance = { 0.70710678118654757, 0.70710678118654757 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 3.141592653736413
    // t = -0.7071067811579796
    // d = 0.7071067811865476
    // P = (-0.4999999999797994, 0.0, -0.4999999999797994)
    // K = (-1.0, -1.466194828809971 * 10 ^ (-10), 0.0)

    // Block 3 and then Block 17
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 2;
    mExpectedOutput.linearClosest[0] = { -0.67225426087696782, 1.3939542471484030, 1.3619358829116757 };
    mExpectedOutput.linearClosest[1] = { -0.81979853312303208, 0.48151686885159695, 0.29156753508832434 };
    mExpectedOutput.circularClosest[0] = { -0.57649424448831066, 0.85432916968355865, 1.8087406211842263 };
    mExpectedOutput.circularClosest[1] = { -0.91555854951168925, 1.0211419463164413, -0.15523720318422640 };
    mExpectedOutput.distance = 0.70710655428336799;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 2;
    mExpectedCritical.parameter = { -0.70710686444805593, 0.70710686444805593 };
    mExpectedCritical.distance = { 0.70710655428336799, 0.70710655428336799 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 3.141592377674846
    // t = -0.7071067751638203
    // d = 0.7071067249022086
    // P = (-0.6722542701919513, 1.393954189543055, 1.361935815335596)
    // K = (-0.576493988494748, 0.854328808597523, 1.808740400238873)

    // non-unit direction

    // Block 3 and then Block 17
    // Dot(M,D) = 0
    mLine.origin = { 0.0, 0.0, 0.0 };
    mLine.direction = { 1.0, 0.0, 1.0 };
    Normalize(mLine.direction);
    mLine.direction *= mDirectionScale;
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 2;
    mExpectedOutput.linearClosest[0] = { -0.50000000000000011, 0.0, -0.50000000000000011 };
    mExpectedOutput.linearClosest[1] = { 0.50000000000000011, 0.0, 0.50000000000000011 };
    mExpectedOutput.circularClosest[0] = { -1.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 1.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.70710678118654757;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 2;
    mExpectedCritical.parameter = { -0.57275691087364855, 0.57275691087364855 };
    mExpectedCritical.distance = { 0.70710678118654757, 0.70710678118654757 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 3.141592653616016
    // t = -0.5727569108678168
    // d = 0.7071067811865475
    // P = (-0.4999999999949091, 0.0, -0.4999999999949091)
    // K = (-1.0, -2.622290128775643 * 10 ^ (-11), 0.0)

    // Block 3 and then Block 16
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1;  // Rounding errors, theoretically 2
    mExpectedOutput.linearClosest[0] = { -0.81979853312303208, 0.48151686885159706, 0.29156753508832423 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.91555854951168925, 1.0211419463164413, -0.15523720318422640 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.70710655428336788;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 2;
    mExpectedCritical.parameter = { -0.57275697831551942, 0.57275697831551942 };
    mExpectedCritical.distance = { 0.70710655428336799, 0.70710655428336788 };  // theoretically the same
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 3.141592377452718
    // t = -0.5727569060147153
    // d = 0.7071067249022084
    // P = (-0.6722542701894424, 1.393954189558571, 1.361935815353798)
    // K = (-0.5764939882758894, 0.854328808605647, 1.808740400201778)
}

void UnitTestDistLine3Circle3::LineNotPerpOriginNotOnNormalLine1()
{
    // unit direction

    // Block 10 and then Block 17
    // NdM = 0 (line parallel to plane of circle)
    mLine.origin = { 0.0, 0.5, 1.0 };
    mLine.direction = { 1.0, 0.0, 0.0 };
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 2;
    mExpectedOutput.linearClosest[0] = { -0.86602540378443860, 0.5, 1.0 };
    mExpectedOutput.linearClosest[1] = { 0.86602540378443860, 0.5, 1.0 };
    mExpectedOutput.circularClosest[0] = { -0.86602540378443871, 0.50000000000000011, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.86602540378443871, 0.50000000000000011, 0.0 };
    mExpectedOutput.distance = 1.0;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 2;
    mExpectedCritical.parameter = { -0.86602540378443860, 0.86602540378443860 };
    mExpectedCritical.distance = { 1.0, 1.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 2.617993877991347
    // t = -0.866025403781961
    // d = 1.0
    // P = (-0.866025403781961, 0.5, 1.0)
    // K = (-0.866025403784365, 0.5000000000001272, 0.0)

    // Block 9 and then Block 16
    // NdM = -1.1774951479137652e-07, rounding errors (line parallel to plane)
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1; // theoretically 2
    mExpectedOutput.linearClosest[0] = { -0.37821806882895548, 0.032411495135486468, -0.19555194759968064 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.40020594178136115, 1.0282550868134328, -0.10717249973204046 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.99999942622403160;  // theoretically 1.0
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 2;
    mExpectedCritical.parameter = { -0.86602531652114501, 0.86602549943954599 };
    mExpectedCritical.distance = { 0.99999963017211135, 0.99999942622403160 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 0.5235986109242244
    // t = 0.866025603869207
    // d = 0.999999866504097
    // P = (-0.378218086533139, 0.03241150384562607, -0.1955520501484316)
    // K = (-0.4002060888628956, 1.028255519160453, -0.1071724262331859)

    // non-unit direction

    // Block 10 and then Block 17
    // NdM = 0 (line parallel to plane of circle)
    mLine.origin = { 0.0, 0.5, 1.0 };
    mLine.direction = { 1.0, 0.0, 0.0 };
    mLine.direction *= mDirectionScale;
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 2;
    mExpectedOutput.linearClosest[0] = { -0.86602540378443860, 0.5, 1.0 };
    mExpectedOutput.linearClosest[1] = { 0.86602540378443860, 0.5, 1.0 };
    mExpectedOutput.circularClosest[0] = { -0.86602540378443871, 0.50000000000000011, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.86602540378443871, 0.50000000000000011, 0.0 };
    mExpectedOutput.distance = 1.0;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 2;
    mExpectedCritical.parameter = { -0.70148108914659035, 0.70148108914659035 };
    mExpectedCritical.distance = { 1.0, 1.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 2.617993877991367
    // t = -0.701481089144615
    // d = 1.0
    // P = (-0.866025403782, 0.5, 1.0)
    // K = (-0.866025403784375, 0.5000000000001107, 0.0)

    // Block 9 and then Block 16
    // NdM = -1.1774951479137652e-07, rounding errors (line parallel to plane)
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1; // theoretically 2
    mExpectedOutput.linearClosest[0] = { -0.37821806882895548, 0.032411495135486468, -0.19555194759968064 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.40020594178136115, 1.0282550868134328, -0.10717249973204046 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.99999942622403160;  // theoretically 1.0
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 2;
    mExpectedCritical.parameter = { -0.70148101846327127, 0.70148116662728388 };
    mExpectedCritical.distance = { 0.99999963017211169, 0.99999942622403160 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 0.523598610595548
    // t = 0.7014812514408108
    // d = 0.999999866504097
    // P = (-0.3782180865803233, 0.03241150386883987, -0.1955520504217392)
    // K = (-0.4002060891712082, 1.028255519163749, -0.1071724263470305)
}

void UnitTestDistLine3Circle3::LineNotPerpOriginNotOnNormalLine2()
{
    // unit direction

    // Block 13 and then Block 14
    mLine.origin = { -2.0, 1.0, 1.0 };
    mLine.direction = { 1.0, 0.0, 0.0 };
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.0, 1.0, 1.0 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.0, 1.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 1.0;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { 2.0, 0.0 };
    mExpectedCritical.distance = { 1.0, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 1.568678271299294
    // t = 2.002118055496882
    // d = 1.000000000002516
    // P = (0.002118055496881599, 1.0, 1.0)
    // K = (0.002118053911946945, 0.999997756921297, 0.0)

    // Block 11 and then Block 14
    // rounding errors
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.26019405306662213, -0.021018374798368766, 0.56531600435674434 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.23818753989103891, 0.97482454446849576, 0.65369873008261226 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.99999945634126819;  // theoretically 1.0
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { 2.0061736444023746, 0.0 };
    mExpectedCritical.distance = { 0.99999945634126819, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 1.568451144398806
    // t = 2.002345300035063
    // d = 0.999999866507496
    // P = (0.2608430804670522, -0.02133768459627383, 0.5690753954080079)
    // K = (0.2388525095382042, 0.974506235184886, 0.6574554567348)

    // non-unit direction

    // Block 13 and then Block 14
    mLine.origin = { -2.0, 1.0, 1.0 };
    mLine.direction = { 1.0, 0.0, 0.0 };
    mLine.direction *= mDirectionScale;
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.0, 1.0, 1.0 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.0, 1.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 1.0;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { 1.6200011826008633, 0.0 };
    mExpectedCritical.distance = { 1.0, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 1.568678271299294
    // t = 2.002118055496882
    // d = 1.000000000002516
    // P = (0.002118055496881599, 1.0, 1.0)
    // K = (0.002118053911946945, 0.999997756921297, 0.0)

    // Block 11 and then Block 14
    // rounding errors
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.26019405306526122, -0.021018374797699191, 0.56531600434886065 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.23818753988962904, 0.97482454446916333, 0.65369873007473722 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.99999945634126808;  // theoretically 1.0
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { 1.6250018382237681, 0.0 };
    mExpectedCritical.distance = { 0.99999945634126808, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 1.568820138891275
    // t = 1.621601991273266
    // d = 0.999999866505735
    // P = (0.2609056368887779, -0.02136846122968814, 0.5694377438263929)
    // K = (0.2389158513602374, 0.974475487782679, 0.6578176711818151)
}

void UnitTestDistLine3Circle3::LineNotPerpOriginNotOnNormalLine3()
{
    // unit direction

    // Block 12 and then Block 14
    mLine.origin = { 1.0, 2.0, 3.0 };
    mLine.direction = { 1.0, 1.0, 1.0 };
    Normalize(mLine.direction);
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -1.4135165379285084, -0.41351653792850840, 0.58648346207149160 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.95977320738110972, -0.28077640640441520, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.75330326151998195;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { -4.1803332685999131, 0.0 };
    mExpectedCritical.distance = { 0.75330326151998195, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 3.426195614968719
    // t = -4.180333268600884
    // d = 0.7533032615199819
    // P = (-1.413516537929069, -0.4135165379290688, 0.5864834620709312)
    // K = (-0.959773207380865, -0.280776406405253, 0.0)

    // Block 12 and then Block 14
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.90092362220335420, 0.22066821939512415, 2.2320305939588594 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.85995715483860835, 0.84741457606172432, 1.8161262097023507 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.75330319519234057;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { -4.1803332472343646, 0.0 };
    mExpectedCritical.distance = { 0.75330319519234057, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 3.426195677926129
    // t = -4.180333205370505
    // d = 0.7533032254118547
    // P = (-0.900923601955208, 0.2206681982254404, 2.232030564051683)
    // K = (-0.859957209900797, 0.847414538301854, 1.816126092641759)

    // non-unit direction

    // Block 12 and then Block 14
    mLine.origin = { 1.0, 2.0, 3.0 };
    mLine.direction = { 1.0, 1.0, 1.0 };
    Normalize(mLine.direction);
    mLine.direction *= mDirectionScale;
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -1.4135165379285080, -0.41351653792850795, 0.58648346207149205 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.95977320738110972, -0.28077640640441498, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.75330326151998195;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { -3.3860724193987952, 0.0 };
    mExpectedCritical.distance = { 0.75330326151998195, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 3.426195615092634
    // t = -3.386072419496549
    // d = 0.753303261519982
    // P = (-1.413516537998185, -0.4135165379981847, 0.5864834620018153)
    // K = (-0.959773207346072, -0.2807764065241834, 0.0)

    // Block 12 and then Block 14
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.90092362220335420, 0.22066821939512460, 2.2320305939588598 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.85995715483860835, 0.84741457606172432, 1.8161262097023507 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.75330319519234046;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { -3.3860724020926893, 0.0 };
    mExpectedCritical.distance = { 0.75330319519234046, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 3.42619567973142
    // t = -3.386072368994405
    // d = 0.7533032254118545
    // P = (-0.90092360243975, 0.2206681987320356, 2.232030564767368)
    // K = (-0.859957211693894, 0.847414538280759, 1.81612609243335)
}

void UnitTestDistLine3Circle3::LineNotPerpOriginNotOnNormalLine4()
{
    // unit direction

    // Block 13 and then Block 14
    mLine.origin = { 1.0, 2.0, 1.0 };
    mLine.direction = { 1.0, 0.0, 1.0 };
    Normalize(mLine.direction);
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.0, 2.0, 0.0 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.0, 1.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 1.0;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { -1.4142135623730951, 0.0 };
    mExpectedCritical.distance = { 1.0, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 1.570796235561277
    // t = -1.414213489695104
    // d = 0.999999881830502
    // P = (1.224531994245299, 1.010885858868047, 0.4927641121946932)
    // K = (0.2392527869468457, 0.974310739489102, 0.6597578485106611)

    // Block 11 and then Block 14
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 1.2245319866624822, 1.0108858119746966, 0.49276405718471727 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.23925291326729148, 0.97431059845225199, 0.65975785529536013 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.99999976366103016;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { -1.4142134170137102, 0.0 };
    mExpectedCritical.distance = { 0.99999976366103016, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 1.570796235561277
    // t = -1.414213489695104
    // d = 0.999999881830502
    // P = (1.224531994245299, 1.010885858868047, 0.4927641121946932)
    // K = (0.2392527869468457, 0.974310739489102, 0.6597578485106611)

    // non-unit direction

    // Block 11 and then Block 14
    mLine.origin = { 1.0, 2.0, 1.0 };
    mLine.direction = { 1.0, 0.0, 1.0 };
    Normalize(mLine.direction);
    mLine.direction *= mDirectionScale;
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 2.2204460492503131e-16, 2.0, 2.2204460492503131e-16 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 1.1102230246251565e-16, 1.0, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 1.0;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { -1.1455138217472967, 0.0 };
    mExpectedCritical.distance = { 1.0, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 1.570796326797682
    // t = -1.145513821748816
    // d = 1.0
    // P = (-1.326050380612287 * 10 ^ (-12), 2.0, -1.326050380612287 * 10 ^ (-12))
    // K = (-2.78504424723471 * 10 ^ (-12), 1.0, 0.0)

    // Block 11 and then Block 14
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 1.2245319866624822, 1.0108858119746964, 0.49276405718471727 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.23925291326729148, 0.97431059845225199, 0.65975785529536013 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.99999976366103016;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { -1.1455137040061090, 0.0 };
    mExpectedCritical.distance = { 0.99999976366103016, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 1.570796235561301
    // t = -1.145513762878135
    // d = 0.999999881830502
    // P = (1.224531994245305, 1.01088585886809, 0.492764112194744)
    // K = (0.2392527869468498, 0.9743107394891, 0.6597578485106846)
}

void UnitTestDistLine3Circle3::LineNotPerpOriginNotOnNormalLine5()
{
    // unit direction

    // Block 4 and then Block 14
    mLine.origin = { 0.0, 0.25, 1.0 };
    mLine.direction = { 1.0, 0.0, -1.0 };
    Normalize(mLine.direction);
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.98462277300599133, 0.25, 0.015377226994008675 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.96924554601198287, 0.24609565525610819, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.022094391322905613;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { 1.3924668794064785, 0.0 };
    mExpectedCritical.distance = { 0.022094391322905613, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 0.2486499612903211
    // t = 1.392466878430616
    // d = 0.02209439132290786
    // P = (0.984622772315953, 0.25, 0.01537722768404726)
    // K = (0.969245543559162, 0.2460956649165209, 0.0)

    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.66629367001141859, 1.0136901308444322, -0.18324415944248329 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.66787170172845600, 1.0275780371580014, -0.16613306404557160 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.022094200858068305;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { 1.3924667539511695, 0.0 };
    mExpectedCritical.distance = { 0.022094200858068302, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 0.2486499532260887
    // t = 1.392466878644449
    // d = 0.02209438698587914
    // P = (-0.6662936868980188, 1.013690226003553, -0.1832442382333185)
    // K = (-0.667871733269447, 1.027578179557369, -0.1661329421977029)

    // non-unit direction

    // Block 4 and then Block 14
    mLine.origin = { 0.0, 0.25, 1.0 };
    mLine.direction = { 1.0, 0.0, -1.0 };
    Normalize(mLine.direction);
    mLine.direction *= mDirectionScale;
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.98462277300599155, 0.25, 0.015377226994008453 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.96924554601198287, 0.24609565525610813, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.022094391322905624;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { 1.1278989956855145, 0.0 };
    mExpectedCritical.distance = { 0.022094391322905624, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 0.2486499540322226
    // t = 1.127898995469521
    // d = 0.0220943913229058
    // P = (0.984622772817436, 0.25, 0.0153772271825644)
    // K = (0.969245545345349, 0.2460956578816413, 0.0)

    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.66629367001141859, 1.0136901308444319, -0.18324415944248307 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.66787170172845600, 1.0275780371580014, -0.16613306404557160 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.022094200858068271;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { 1.1278988940666399, 0.0 };
    mExpectedCritical.distance = { 0.022094200858068267, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 0.248649952249926
    // t = 1.127898995145656
    // d = 0.02209438698587933
    // P = (-0.6662936869109571, 1.013690226076463, -0.1832442382936871)
    // K = (-0.667871734242387, 1.0275781795428, -0.1661329422756057)
}

void UnitTestDistLine3Circle3::LineNotPerpOriginNotOnNormalLine6()
{
    // unit direction

    // Block 6 and then Block 14
    mLine.origin = { 0.0, 0.25, 1.0 };
    mLine.direction = { -1.0, 0.0, 1.0 };
    Normalize(mLine.direction);
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.98462277300599133, 0.25, 0.015377226994008675 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.96924554601198287, 0.24609565525610819, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.022094391322905613;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { -1.3924668794064785, 0.0 };
    mExpectedCritical.distance = { 0.022094391322905613, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 0.2486499513646419
    // t = -1.392466879399376
    // d = 0.02209439132290567
    // P = (0.984622773000969, 0.25, 0.01537722699903088)
    // K = (0.969245546001829, 0.2460956552961006, 0.0)

    // Block 6 and then Block 14
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.66629367001141859, 1.0136901308444322, -0.18324415944248329 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.66787170172845600, 1.0275780371580014, -0.16613306404557160 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.022094200858068305;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { -1.3924667539511695, 0.0 };
    mExpectedCritical.distance = { 0.022094200858068302, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 0.248649951917715
    // t = -1.392466878772375
    // d = 0.02209438698587926
    // P = (-0.6662936869153433, 1.01369022610118, -0.1832442383141522)
    // K = (-0.6678717345735011, 1.027578179537842, -0.1661329423021178)

    // non-unit direction

    // Block 6 and then Block 14
    mLine.origin = { 0.0, 0.25, 1.0 };
    mLine.direction = { -1.0, 0.0, 1.0 };
    Normalize(mLine.direction);
    mLine.direction *= mDirectionScale;
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { 0.98462277300599155, 0.25, 0.015377226994008453 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { 0.96924554601198287, 0.24609565525610813, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.022094391322905624;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { -1.1278989956855145, 0.0 };
    mExpectedCritical.distance = { 0.022094391322905624, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 0.2486499512868634
    // t = -1.127898996146975
    // d = 0.02209439132290561
    // P = (0.984622773408833, 0.25, 0.01537722659116747)
    // K = (0.96924554602097, 0.2460956552207142, 00.)

    // Block 6 and then Block 14
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.66629367001141859, 1.0136901308444319, -0.18324415944248307 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.66787170172845600, 1.0275780371580014, -0.16613306404557160 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.022094200858068271;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 1;
    mExpectedCritical.parameter = { -1.1278988940666399, 0.0 };
    mExpectedCritical.distance = { 0.022094200858068267, 0.0 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 0.2486499522262485
    // t = -1.127898995147956
    // d = 0.02209438698587937
    // P = (-0.6662936869113416, 1.01369022607863, -0.1832442382954811)
    // K = (-0.6678717342659864, 1.027578179542447, -0.1661329422774953)
}

void UnitTestDistLine3Circle3::LineNotPerpOriginNotOnNormalLine7()
{
    // unit direction

    // Block 10 and then Block 17
    mLine.origin = { 0.0, 0.5, 0.0 };
    mLine.direction = { 1.0, 0.0, -0.0625 };
    Normalize(mLine.direction);
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 2;
    mExpectedOutput.linearClosest[0] = { -0.86152947663288304, 0.5, 0.053845592289555190 };
    mExpectedOutput.linearClosest[1] = { 0.86152947663288304, 0.5, -0.053845592289555190 };
    mExpectedOutput.circularClosest[0] = { -0.86489482615098023, 0.50195312500000000, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.86489482615098023, 0.50195312500000000, 0.0 };
    mExpectedOutput.distance = 0.053985998959522712;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 2;
    mExpectedCritical.parameter = { -0.86321051135649551, 0.86321051135649551 };
    mExpectedCritical.distance = { 0.053985998959522712, 0.053985998959522712 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 2.615737131541029
    // t = -0.86321051147874
    // d = 0.05398599895952271
    // P = (-0.86152947675489, 0.5, 0.05384559229718061)
    // K = (-0.864894826225569, 0.5019531248714802, 0.0)

    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.10614590385148315, 0.83054394295345513, 1.5845082776970438 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.10483485449522278, 0.88395650923238944, 1.5922458106977175 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.053986021374543024;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 2;
    mExpectedCritical.parameter = { -0.86321056975787314, 0.86321042721126584 };
    mExpectedCritical.distance = { 0.053986021374543024, 0.053986155460955282 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 2.615737139886932
    // t = -0.863210508967348
    // d = 0.05398599161825773
    // P = (-0.106145914220739, 0.830543951790171, 1.584508218452821)
    // K = (-0.1048349625159071, 0.883956505449458, 1.592245647497924)

    // non-unit direction

    // Block 10 and then Block 17
    mLine.origin = { 0.0, 0.5, 0.0 };
    mLine.direction = { 1.0, 0.0, -0.0625 };
    Normalize(mLine.direction);
    mLine.direction *= mDirectionScale;
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 2;
    mExpectedOutput.linearClosest[0] = { -0.86152947663288315, 0.5, 0.053845592289555197 };
    mExpectedOutput.linearClosest[1] = { 0.86152947663288315, 0.5, -0.053845592289555197 };
    mExpectedOutput.circularClosest[0] = { -0.86489482615098034, 0.501953125, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.86489482615098034, 0.501953125, 0.0 };
    mExpectedOutput.distance = 0.053985998959522719;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 2;
    mExpectedCritical.parameter = { -0.69920102461550937, 0.69920102461550937 };
    mExpectedCritical.distance = { 0.053985998959522719, 0.053985998959522719 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 2.61573713139647
    // t = -0.6992010246163847
    // d = 0.0539859989595227
    // P = (-0.861529476633962, 0.5, 0.0538455922896226)
    // K = (-0.864894826153006, 0.501953124996509, 0.0)

    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mCircle.center = mRot * mCircle.center + mTrn;
    mCircle.normal = mRot * mCircle.normal;
    mExpectedOutput.numClosestPairs = 1;
    mExpectedOutput.linearClosest[0] = { -0.10614590385148315, 0.83054394295345502, 1.5845082776970441 };
    mExpectedOutput.linearClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.circularClosest[0] = { -0.10483485449522278, 0.88395650923238944, 1.5922458106977175 };
    mExpectedOutput.circularClosest[1] = { 0.0, 0.0, 0.0 };
    mExpectedOutput.distance = 0.053986021374543100;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 2;
    mExpectedCritical.parameter = { -0.69920107192065983, 0.69920095645782354 };
    mExpectedCritical.distance = { 0.053986021374543100, 0.053986155460955282 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 2.615737140046575
    // t = -0.6992010227892749
    // d = 0.05398599161825773
    // P = (-0.1061459141977902, 0.830543951770614, 1.584508218583938)
    // K = (-0.1048349626383638, 0.883956505437724, 1.592245647599671)
}

void UnitTestDistLine3Circle3::LineNotPerpOriginNotOnNormalLine8()
{
    // In this example, Block 10 is reached and s is not zero. The
    // critical.parameter[] values are not symmetric about the origin.

    // unit direction

    // Block 10 and then Block 17
    mLine.origin = { 0.2, 0.1, 0.3 };
    mLine.direction = { 1.0, 0.0, 0.0 };
    Normalize(mLine.direction);
    mLine.origin -= 2.0 * mLine.direction;
    mCircle.center = { 0.0, 0.0, 0.0 };
    mCircle.normal = { 0.0, 0.0, 1.0 };
    mCircle.radius = 1.0;
    mExpectedOutput.numClosestPairs = 2;
    mExpectedOutput.linearClosest[0] = { -0.99498743710661997, 0.10000000000000001, 0.29999999999999999 };
    mExpectedOutput.linearClosest[1] = { 0.99498743710662008, 0.10000000000000001, 0.29999999999999999 };
    mExpectedOutput.circularClosest[0] = { -0.99498743710661997, 0.10000000000000001, 0.0 };
    mExpectedOutput.circularClosest[1] = { 0.99498743710662008, 0.10000000000000001, 0.0 };
    mExpectedOutput.distance = 0.29999999999999999;
    mExpectedOutput.equidistant = false;
    mExpectedCritical.numPoints = 2;
    mExpectedCritical.parameter = { 0.80501256289338008, 2.7949874371066201 };
    mExpectedCritical.distance = { 0.29999999999999999, 0.29999999999999999 };
    mOutput = mQuery(mLine, mCircle, &mCritical);
    Validate();
    // Mathematica:
    // s = 3.041425232672142
    // t = 0.805012562691496
    // d = 0.3
    // P = (-0.994987437308505, 0.1, 0.3)
    // K = (-0.994987437131011, 0.0999999997573142, 0.0)
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/3D/DistLine3Circle3.h>

namespace gtl
{
    template class DCPQuery<float, Line3<float>, Circle3<float>>;
    template class DCPQuery<double, Line3<double>, Circle3<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Line3<Rational>, Circle3<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistLine3Circle3)
