#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Algebra/RigidMotion.h>
#include <GTL/Mathematics/Distance/2D/DistLine2Triangle2.h>
using namespace gtl;

namespace gtl
{
    class UnitTestDistLine2Triangle2
    {
    public:
        UnitTestDistLine2Triangle2();

    private:
        using LTQuery = DCPQuery<double, Line2<double>, Triangle2<double>>;
        using LTOutput = LTQuery::Output;

        void Validate();

        void TestThreeNegative012();
        void TestThreeNegative120();
        void TestThreeNegative201();
        void TestThreePositive012();
        void TestThreePositive120();
        void TestThreePositive201();

        void TestOneZero012();
        void TestOneZero120();
        void TestOneZero201();

        void TestTwoZeros012();
        void TestTwoZeros120();
        void TestTwoZeros201();

        Line2<double> mLine;
        Triangle2<double> mTriangle;
        LTQuery mQuery;
        LTOutput mOutput;
        LTOutput mExpectedOutput;
        Matrix2x2<double> mRot;
        Vector2<double> mTrn;
    };
}

UnitTestDistLine2Triangle2::UnitTestDistLine2Triangle2()
    :
    mLine{},
    mTriangle{},
    mQuery{},
    mOutput{},
    mExpectedOutput{},
    mRot{},
    mTrn{}
{
    UTInformation("Mathematics/Distance/2D/DistLine2Triangle2");

    RigidMotion<double>::Convert(0.469646722, mRot);
    mTrn = Vector2<double>{ -0.746026397, 0.937735558 };

    TestThreeNegative012();
    TestThreeNegative120();
    TestThreeNegative201();
    TestThreePositive012();
    TestThreePositive120();
    TestThreePositive201();

    TestOneZero012();
    TestOneZero120();
    TestOneZero201();

    TestTwoZeros012();
    TestTwoZeros120();
    TestTwoZeros201();
}

void UnitTestDistLine2Triangle2::Validate()
{
    double const maxError = 1e-16;
    double error{};
    double maxMaxError = 0.0;

    error = std::fabs(mOutput.distance - mExpectedOutput.distance);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(error <= maxError, "incorrect distance");

    error = std::fabs(mOutput.parameter - mExpectedOutput.parameter);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(error <= maxError, "incorrect parameter");

    for (std::size_t i = 0; i < 3; ++i)
    {
        error = std::fabs(mOutput.barycentric[i] - mExpectedOutput.barycentric[i]);
        maxMaxError = std::max(maxMaxError, error);
        UTAssert(error <= maxError, "incorrect barycentric[" + std::to_string(i) + "]");
    }

    error = Length(mOutput.closest[0] - mExpectedOutput.closest[0]);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(error <= maxError, "invalid line point");

    error = Length(mOutput.closest[1] - mExpectedOutput.closest[1]);
    maxMaxError = std::max(maxMaxError, error);
    UTAssert(error <= maxError, "invalid triangle point");
}

void UnitTestDistLine2Triangle2::TestThreeNegative012()
{
    mTriangle.v[0] = { 1.0, 1.0 };
    mTriangle.v[1] = { 2.0, 1.0 };
    mTriangle.v[2] = { 1.0, 2.0 };

    mLine.origin = { 0.0, 0.0 };
    mLine.direction = { 1.0, -4.0 };
    mExpectedOutput.distance = 1.2126781251816650;
    mExpectedOutput.parameter = -0.17647058823529413;
    mExpectedOutput.barycentric = { 1.0, 0.0, 0.0 };
    mExpectedOutput.closest[0] = { -0.17647058823529413, 0.70588235294117652 };
    mExpectedOutput.closest[1] = { 1.0, 1.0 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica:
    // distance = 1.21268
    // parameter = -0.176471
    // barycentric[0] = 1.0
    // barycentric[1] = 2.27 * 10^{-9}
    // barycentric[2] = 9.08299 * 10^{-9}
    // closest[0] = { -0.176471, 0.705882 }
    // closest[1] = { 1.0, 1.0 }

    for (std::size_t i = 0; i < 3; ++i)
    {
        mTriangle.v[i] = mRot * mTriangle.v[i] + mTrn;
    }
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mExpectedOutput.distance = 1.2126781251816650;
    mExpectedOutput.parameter = -0.17647058823529410;
    mExpectedOutput.barycentric = { 1.0, 0.0, 0.0 };
    mExpectedOutput.closest[0] = { -1.2228522858177477, 1.4873252562832255 };
    mExpectedOutput.closest[1] = { -0.30686945534570492, 2.2820350709794721 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica:
    // distance =1.21268
    // parameter = -0.176471
    // barycentric[0] = 1.0
    // barycentric[1] = 2.27 * 10^{-9}
    // barycentric[2] = 9.08299 * 10^{-9}
    // closest[0] = { -1.22285, 1.48733 }
    // closest[1] = { -0.306869,2.28204 }
}

void UnitTestDistLine2Triangle2::TestThreeNegative120()
{
    mTriangle.v[1] = { 1.0, 1.0 };
    mTriangle.v[2] = { 2.0, 1.0 };
    mTriangle.v[0] = { 1.0, 2.0 };

    mLine.origin = { 0.0, 0.0 };
    mLine.direction = { 1.0, -4.0 };
    mExpectedOutput.distance = 1.2126781251816650;
    mExpectedOutput.parameter = -0.17647058823529413;
    mExpectedOutput.barycentric = { 0.0, 1.0, 0.0 };
    mExpectedOutput.closest[0] = { -0.17647058823529413, 0.70588235294117652 };
    mExpectedOutput.closest[1] = { 1.0, 1.0 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica:
    // distance = 1.21268
    // parameter = -0.176471
    // barycentric[0] = 7.45161*10^{-9}
    // barycentric[1] = 1.0
    // barycentric[2] = 1.12718*10^{-9}
    // closest[0] = { -0.176471, 0.705882 }
    // closest[1] = { 1.0, 1.0 }

    for (std::size_t i = 0; i < 3; ++i)
    {
        mTriangle.v[i] = mRot * mTriangle.v[i] + mTrn;
    }
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mExpectedOutput.distance = 1.2126781251816650;
    mExpectedOutput.parameter = -0.17647058823529410;
    mExpectedOutput.barycentric = { 0.0, 1.0, 0.0 };
    mExpectedOutput.closest[0] = { -1.2228522858177477, 1.4873252562832255 };
    mExpectedOutput.closest[1] = { -0.30686945534570492, 2.2820350709794721 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica:
    // distance = 1.21268
    // parameter = -0.176471
    // barycentric[0] = 7.45161*10^{-9}
    // barycentric[1] = 1.0
    // barycentric[2] = 1.12718*10^{-9}
    // closest[0] = { -1.22285, 1.48733 }
    // closest[1] = { -0.306869, 2.28204 }
}

void UnitTestDistLine2Triangle2::TestThreeNegative201()
{
    mTriangle.v[2] = { 1.0, 1.0 };
    mTriangle.v[0] = { 2.0, 1.0 };
    mTriangle.v[1] = { 1.0, 2.0 };

    mLine.origin = { 0.0, 0.0 };
    mLine.direction = { 1.0, -4.0 };
    mExpectedOutput.distance = 1.2126781251816650;
    mExpectedOutput.parameter = -0.17647058823529413;
    mExpectedOutput.barycentric = { 0.0, 0.0, 1.0 };
    mExpectedOutput.closest[0] = { -0.17647058823529413, 0.70588235294117652 };
    mExpectedOutput.closest[1] = { 1.0, 1.0 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica:
    // distance = 1.21268
    // parameter = -0.176471
    // barycentric[0] = 9.77097*10^-{10}
    // barycentric[1] = 6.46023*10^{-9}
    // barycentric[2] = 1.0
    // closest[0] = { -0.176471, 0.705882 }
    // closest[1] = { 1.0, 1.0 }

    for (std::size_t i = 0; i < 3; ++i)
    {
        mTriangle.v[i] = mRot * mTriangle.v[i] + mTrn;
    }
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mExpectedOutput.distance = 1.2126781251816650;
    mExpectedOutput.parameter = -0.17647058823529410;
    mExpectedOutput.barycentric = { 0.0, 0.0, 1.0 };
    mExpectedOutput.closest[0] = { -1.2228522858177477, 1.4873252562832255 };
    mExpectedOutput.closest[1] = { -0.30686945534570492, 2.2820350709794721 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica:
    // distance = 1.21268
    // parameter = -0.176471
    // barycentric[0] = 9.77097*10^-{10}
    // barycentric[1] = 6.46023*10^{-9}
    // barycentric[2] = 1.0
    // closest[0] = { -1.22285, 1.48733 }
    // closest[1] = { -0.306869, 2.28204 }
}

void UnitTestDistLine2Triangle2::TestThreePositive012()
{
    mTriangle.v[0] = { 1.0, 1.0 };
    mTriangle.v[1] = { 2.0, 1.0 };
    mTriangle.v[2] = { 1.0, 2.0 };

    mLine.origin = { 0.0, 0.0 };
    mLine.direction = { -1.0, 4.0 };
    mExpectedOutput.distance = 1.2126781251816650;
    mExpectedOutput.parameter = 0.17647058823529413;
    mExpectedOutput.barycentric = { 1.0, 0.0, 0.0 };
    mExpectedOutput.closest[0] = { -0.17647058823529413, 0.70588235294117652 };
    mExpectedOutput.closest[1] = { 1.0, 1.0 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica:
    // distance = 1.21268
    // parameter = -0.176471
    // barycentric[0] = 1.0
    // barycentric[1] = nearly zero
    // barycentric[2] = nearly zero
    // closest[0] = { -0.176471, 0.705882 }
    // closest[1] = { 1.0, 1.0 }

    for (std::size_t i = 0; i < 3; ++i)
    {
        mTriangle.v[i] = mRot * mTriangle.v[i] + mTrn;
    }
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mExpectedOutput.distance = 1.2126781251816650;
    mExpectedOutput.parameter = 0.17647058823529410;
    mExpectedOutput.barycentric = { 1.0, 0.0, 0.0 };
    mExpectedOutput.closest[0] = { -1.2228522858177477, 1.4873252562832255 };
    mExpectedOutput.closest[1] = { -0.30686945534570492, 2.2820350709794721 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica:
    // distance =1.21268
    // parameter = -0.176471
    // barycentric[0] = 1.0
    // barycentric[1] = nearly zero
    // barycentric[2] = nearly zero
    // closest[0] = { -1.22285, 1.48733 }
    // closest[1] = { -0.306869,2.28204 }
}

void UnitTestDistLine2Triangle2::TestThreePositive120()
{
    mTriangle.v[1] = { 1.0, 1.0 };
    mTriangle.v[2] = { 2.0, 1.0 };
    mTriangle.v[0] = { 1.0, 2.0 };

    mLine.origin = { 0.0, 0.0 };
    mLine.direction = { -1.0, 4.0 };
    mExpectedOutput.distance = 1.2126781251816650;
    mExpectedOutput.parameter = 0.17647058823529413;
    mExpectedOutput.barycentric = { 0.0, 1.0, 0.0 };
    mExpectedOutput.closest[0] = { -0.17647058823529413, 0.70588235294117652 };
    mExpectedOutput.closest[1] = { 1.0, 1.0 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica:
    // distance = 1.21268
    // parameter = -0.176471
    // barycentric[0] = nearly zero
    // barycentric[1] = 1.0
    // barycentric[2] = nearly zero
    // closest[0] = { -0.176471, 0.705882 }
    // closest[1] = { 1.0, 1.0 }

    for (std::size_t i = 0; i < 3; ++i)
    {
        mTriangle.v[i] = mRot * mTriangle.v[i] + mTrn;
    }
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mExpectedOutput.distance = 1.2126781251816650;
    mExpectedOutput.parameter = 0.17647058823529410;
    mExpectedOutput.barycentric = { 0.0, 1.0, 0.0 };
    mExpectedOutput.closest[0] = { -1.2228522858177477, 1.4873252562832255 };
    mExpectedOutput.closest[1] = { -0.30686945534570492, 2.2820350709794721 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica:
    // distance = 1.21268
    // parameter = -0.176471
    // barycentric[0] = nearly zero
    // barycentric[1] = 1.0
    // barycentric[2] = nearly zero
    // closest[0] = { -1.22285, 1.48733 }
    // closest[1] = { -0.306869, 2.28204 }
}

void UnitTestDistLine2Triangle2::TestThreePositive201()
{
    mTriangle.v[2] = { 1.0, 1.0 };
    mTriangle.v[0] = { 2.0, 1.0 };
    mTriangle.v[1] = { 1.0, 2.0 };

    mLine.origin = { 0.0, 0.0 };
    mLine.direction = { -1.0, 4.0 };
    mExpectedOutput.distance = 1.2126781251816650;
    mExpectedOutput.parameter = 0.17647058823529413;
    mExpectedOutput.barycentric = { 0.0, 0.0, 1.0 };
    mExpectedOutput.closest[0] = { -0.17647058823529413, 0.70588235294117652 };
    mExpectedOutput.closest[1] = { 1.0, 1.0 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica:
    // distance = 1.21268
    // parameter = -0.176471
    // barycentric[0] = nearly zero
    // barycentric[1] = nearly zero
    // barycentric[2] = 1.0
    // closest[0] = { -0.176471, 0.705882 }
    // closest[1] = { 1.0, 1.0 }

    for (std::size_t i = 0; i < 3; ++i)
    {
        mTriangle.v[i] = mRot * mTriangle.v[i] + mTrn;
    }
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mExpectedOutput.distance = 1.2126781251816650;
    mExpectedOutput.parameter = 0.17647058823529410;
    mExpectedOutput.barycentric = { 0.0, 0.0, 1.0 };
    mExpectedOutput.closest[0] = { -1.2228522858177477, 1.4873252562832255 };
    mExpectedOutput.closest[1] = { -0.30686945534570492, 2.2820350709794721 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica:
    // distance = 1.21268
    // parameter = -0.176471
    // barycentric[0] = nearly zero
    // barycentric[1] = nearly zero
    // barycentric[2] = 1.0
    // closest[0] = { -1.22285, 1.48733 }
    // closest[1] = { -0.306869, 2.28204 }
}

void UnitTestDistLine2Triangle2::TestOneZero012()
{
    mTriangle.v[0] = { 1.0, 1.0 };
    mTriangle.v[1] = { 2.0, 1.0 };
    mTriangle.v[2] = { 1.0, 2.0 };

    mLine.origin = { 1.0, 1.0 };
    mLine.direction = { 1.0, -4.0 };
    mExpectedOutput.distance = 0.0;
    mExpectedOutput.parameter = 0.0;
    mExpectedOutput.barycentric = { 1.0, 0.0, 0.0 };
    mExpectedOutput.closest[0] = { 1.0, 1.0 };
    mExpectedOutput.closest[1] = { 1.0, 1.0 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica:
    // distance = 0.0000795737
    // parameter = -0.0000361799
    // barycentric[0] = 0.999795
    // barycentric[1] = 0.0000410179
    // barycentric[2] = 0.000164019
    // closest[0] = { 1.0, 1.0 }
    // closest[1] = { 1.0, 1.0 }

    for (std::size_t i = 0; i < 3; ++i)
    {
        mTriangle.v[i] = mRot * mTriangle.v[i] + mTrn;
    }
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mExpectedOutput.distance = 0.0;
    mExpectedOutput.parameter = 0.0;
    mExpectedOutput.barycentric = { 1.0, 0.0, 0.0 };
    mExpectedOutput.closest[0] = { -0.30686945534570492, 2.2820350709794721 };
    mExpectedOutput.closest[1] = { -0.30686945534570492, 2.2820350709794721 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica:
    // distance = 0.0000795737
    // parameter = -0.0000361799
    // barycentric[0] = 0.999795
    // barycentric[1] = 0.0000410179
    // barycentric[2] = 0.000164019
    // closest[0] = { -0.306967, 2.28215 }
    // closest[1] = { -0.306907, 2.2822 }
}

void UnitTestDistLine2Triangle2::TestOneZero120()
{
    mTriangle.v[1] = { 1.0, 1.0 };
    mTriangle.v[2] = { 2.0, 1.0 };
    mTriangle.v[0] = { 1.0, 2.0 };

    mLine.origin = { 1.0, 1.0 };
    mLine.direction = { 1.0, -4.0 };
    mExpectedOutput.distance = 0.0;
    mExpectedOutput.parameter = 0.0;
    mExpectedOutput.barycentric = { 0.0, 1.0, 0.0 };
    mExpectedOutput.closest[0] = { 1.0, 1.0 };
    mExpectedOutput.closest[1] = { 1.0, 1.0 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica:
    // distance = 0.0000795737
    // parameter = -0.0000361799
    // barycentric[0] = 0.999795
    // barycentric[1] = 0.0000410179
    // barycentric[2] = 0.000164019
    // closest[0] = { 1.0, 1.0 }
    // closest[1] = { 1.0, 1.0 }

    for (std::size_t i = 0; i < 3; ++i)
    {
        mTriangle.v[i] = mRot * mTriangle.v[i] + mTrn;
    }
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mExpectedOutput.distance = 0.0;
    mExpectedOutput.parameter = 0.0;
    mExpectedOutput.barycentric = { 0.0, 1.0, 0.0 };
    mExpectedOutput.closest[0] = { -0.30686945534570492, 2.2820350709794721 };
    mExpectedOutput.closest[1] = { -0.30686945534570492, 2.2820350709794721 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica:
    // distance = 0.0000795737
    // parameter = -0.0000361799
    // barycentric[0] = 0.999795
    // barycentric[1] = 0.0000410179
    // barycentric[2] = 0.000164019
    // closest[0] = { -0.306967, 2.28215 }
    // closest[1] = { -0.306907, 2.2822 }
}

void UnitTestDistLine2Triangle2::TestOneZero201()
{
    mTriangle.v[2] = { 1.0, 1.0 };
    mTriangle.v[0] = { 2.0, 1.0 };
    mTriangle.v[1] = { 1.0, 2.0 };

    mLine.origin = { 1.0, 1.0 };
    mLine.direction = { 1.0, -4.0 };
    mExpectedOutput.distance = 0.0;
    mExpectedOutput.parameter = 0.0;
    mExpectedOutput.barycentric = { 0.0, 0.0, 1.0 };
    mExpectedOutput.closest[0] = { 1.0, 1.0 };
    mExpectedOutput.closest[1] = { 1.0, 1.0 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica:
    // distance = 0.0000795737
    // parameter = -0.0000361799
    // barycentric[0] = 0.999795
    // barycentric[1] = 0.0000410179
    // barycentric[2] = 0.000164019
    // closest[0] = { 1.0, 1.0 }
    // closest[1] = { 1.0, 1.0 }

    for (std::size_t i = 0; i < 3; ++i)
    {
        mTriangle.v[i] = mRot * mTriangle.v[i] + mTrn;
    }
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mExpectedOutput.distance = 0.0;
    mExpectedOutput.parameter = 0.0;
    mExpectedOutput.barycentric = { 0.0, 0.0, 1.0 };
    mExpectedOutput.closest[0] = { -0.30686945534570492, 2.2820350709794721 };
    mExpectedOutput.closest[1] = { -0.30686945534570492, 2.2820350709794721 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica:
    // distance = 0.0000795737
    // parameter = -0.0000361799
    // barycentric[0] = 0.999795
    // barycentric[1] = 0.0000410179
    // barycentric[2] = 0.000164019
    // closest[0] = { -0.306967, 2.28215 }
    // closest[1] = { -0.306907, 2.2822 }
}

void UnitTestDistLine2Triangle2::TestTwoZeros012()
{
    mTriangle.v[0] = { 1.0, 1.0 };
    mTriangle.v[1] = { 2.0, 1.0 };
    mTriangle.v[2] = { 1.0, 2.0 };

    mLine.origin = { -1.0, 1.0 };
    mLine.direction = { 1.5, 0.0 };
    mExpectedOutput.distance = 0.0;
    mExpectedOutput.parameter = 1.3333333333333333;
    mExpectedOutput.barycentric = { 1.0, 0.0, 0.0 };
    mExpectedOutput.closest[0] = { 1.0, 1.0 };
    mExpectedOutput.closest[1] = { 1.0, 1.0 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica: [found an interior edge point]
    // distance = 0.0000531175
    // parameter = 1.55625
    // barycentric[0] = 0.665567
    // barycentric[1] = 0.33438
    // barycentric[2] = 0.0000531175
    // closest[0] = { 1.33438, 1.0 }
    // closest[1] = { 1.33438, 1.00005 }

    // This leads to LineIntersectsEdge in block sign[0] > 0, sign[1] > 0,
    // and sign[2] < 0. The normal components are
    // ncomp[0] = 2.2204460492503131e-16
    // ncomp[1] = 4.4408920985006262e-16
    // ncomp[2] = -1.4999999999999996
    // The misclassification is due to numerical rounding errors.
    for (std::size_t i = 0; i < 3; ++i)
    {
        mTriangle.v[i] = mRot * mTriangle.v[i] + mTrn;
    }
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mExpectedOutput.distance = 0.0;
    mExpectedOutput.parameter = 1.3333333333333333;
    mExpectedOutput.barycentric = { 0.99999999999999967, 0.0, 3.3306690738754696e-16 };
    mExpectedOutput.closest[0] = { -0.30686945534570509, 2.2820350709794721 };
    mExpectedOutput.closest[1] = { -0.30686945534570509, 2.2820350709794721 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica:
    // distance = 0.0000531175
    // parameter = 1.55625
    // barycentric[0] = 0.665567
    // barycentric[1] = 0.33438
    // barycentric[2] = 0.0000531175
    // closest[0] = { -0.00869321, 2.43337 }
    // closest[1] = { -0.00871725, 2.43341 }
}

void UnitTestDistLine2Triangle2::TestTwoZeros120()
{
    mTriangle.v[1] = { 1.0, 1.0 };
    mTriangle.v[2] = { 2.0, 1.0 };
    mTriangle.v[0] = { 1.0, 2.0 };

    mLine.origin = { -1.0, 1.0 };
    mLine.direction = { 1.5, 0.0 };
    mExpectedOutput.distance = 0.0;
    mExpectedOutput.parameter = 1.3333333333333333;
    mExpectedOutput.barycentric = { 0.0, 1.0, 0.0 };
    mExpectedOutput.closest[0] = { 1.0, 1.0 };
    mExpectedOutput.closest[1] = { 1.0, 1.0 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica: [found an interior edge point]
    // distance = 0.0000531175
    // parameter = 1.55625
    // barycentric[0] = 0.665567
    // barycentric[1] = 0.33438
    // barycentric[2] = 0.0000531175
    // closest[0] = { 1.33438, 1.0 }
    // closest[1] = { 1.33438, 1.00005 }

    // This leads to LineIntersectsEdge in block sign[0] < 0, sign[1] > 0,
    // and sign[2] > 0. The normal components are
    // ncomp[0] = -1.4999999999999996
    // ncomp[1] = 2.2204460492503131e-16
    // ncomp[2] = 4.4408920985006262e-16
    // The misclassification is due to numerical rounding errors.
    for (std::size_t i = 0; i < 3; ++i)
    {
        mTriangle.v[i] = mRot * mTriangle.v[i] + mTrn;
    }
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mExpectedOutput.distance = 0.0;
    mExpectedOutput.parameter = 1.3333333333333333;
    mExpectedOutput.barycentric = { 3.3306690738754696e-16, 0.99999999999999967, 0.0 };
    mExpectedOutput.closest[0] = { -0.30686945534570509, 2.2820350709794721 };
    mExpectedOutput.closest[1] = { -0.30686945534570509, 2.2820350709794721 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica:
    // distance = 0.0000531175
    // parameter = 1.55625
    // barycentric[0] = 0.665567
    // barycentric[1] = 0.33438
    // barycentric[2] = 0.0000531175
    // closest[0] = { -0.00869321, 2.43337 }
    // closest[1] = { -0.00871725, 2.43341 }
}

void UnitTestDistLine2Triangle2::TestTwoZeros201()
{
    mTriangle.v[2] = { 1.0, 1.0 };
    mTriangle.v[0] = { 2.0, 1.0 };
    mTriangle.v[1] = { 1.0, 2.0 };

    mLine.origin = { -1.0, 1.0 };
    mLine.direction = { 1.5, 0.0 };
    mExpectedOutput.distance = 0.0;
    mExpectedOutput.parameter = 1.3333333333333333;
    mExpectedOutput.barycentric = { 0.0, 0.0, 1.0 };
    mExpectedOutput.closest[0] = { 1.0, 1.0 };
    mExpectedOutput.closest[1] = { 1.0, 1.0 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica: [found an interior edge point]
    // distance = 0.0000531175
    // parameter = 1.55625
    // barycentric[0] = 0.665567
    // barycentric[1] = 0.33438
    // barycentric[2] = 0.0000531175
    // closest[0] = { 1.33438, 1.0 }
    // closest[1] = { 1.33438, 1.00005 }

    // This leads to LineIntersectsEdge in block sign[0] > 0, sign[1] < 0,
    // and sign[2] > 0. The normal components are
    // ncomp[0] = 4.4408920985006262e-16
    // ncomp[1] = -1.4999999999999996
    // ncomp[2] = 2.2204460492503131e-16
    // The misclassification is due to numerical rounding errors.
    for (std::size_t i = 0; i < 3; ++i)
    {
        mTriangle.v[i] = mRot * mTriangle.v[i] + mTrn;
    }
    mLine.origin = mRot * mLine.origin + mTrn;
    mLine.direction = mRot * mLine.direction;
    mExpectedOutput.distance = 0.0;
    mExpectedOutput.parameter = 1.9999999999999996;
    mExpectedOutput.barycentric = { 0.99999999999999967, 2.9605947323337506e-16, 0.0 };
    mExpectedOutput.closest[0] = { 0.58485877197117830, 2.7346063566420606 };
    mExpectedOutput.closest[1] = { 0.58485877197117830, 2.7346063566420606 };
    mOutput = mQuery(mLine, mTriangle);
    Validate();
    // Mathematica:
    // distance = 0.0000531175
    // parameter = 1.55625
    // barycentric[0] = 0.665567
    // barycentric[1] = 0.33438
    // barycentric[2] = 0.0000531175
    // closest[0] = { -0.00869321, 2.43337 }
    // closest[1] = { -0.00871725, 2.43341 }
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Distance/2D/DistLine2Triangle2.h>

namespace gtl
{
    template class DCPQuery<float, Line<float, 2>, Triangle2<float>>;
    template class DCPQuery<double, Line<double, 2>, Triangle2<double>>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class DCPQuery<Rational, Line<Rational, 2>, Triangle2<Rational>>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(DistLine2Triangle2)
