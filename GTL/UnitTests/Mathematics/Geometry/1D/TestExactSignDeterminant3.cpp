#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/1D/ExactSignDeterminant3.h>
#include <GTL/Mathematics/Algebra/Matrix.h>
using namespace gtl;

namespace gtl
{
    class UnitTestExactSignDeterminant3
    {
    public:
        UnitTestExactSignDeterminant3();

    private:
        using IRat = ExactSignDeterminant3<float>::Rational;
        using CRat = ExactSignDeterminant3<float>::CRational;

        void Test0();
        void Test1();
        void Test2();
    };
}

UnitTestExactSignDeterminant3::UnitTestExactSignDeterminant3()
{
    UTInformation("Mathematics/Geometry/1D/ExactSignDeterminant3");

    Test0();
    Test1();
    Test2();
}

void UnitTestExactSignDeterminant3::Test0()
{
    ExactSignDeterminant3<float> query;
    Matrix3x3<float> A;
    A(0, 0) = 0.629447341f;
    A(0, 1) = -0.729045987f;
    A(0, 2) = 0.811583877f;
    A(1, 0) = 0.670017123f;
    A(1, 1) = -0.746026397f;
    A(1, 2) = 0.937735558f;
    A(2, 0) = 0.826751709f;
    A(2, 1) = -0.557931900f;
    A(2, 2) = 0.264718533f;

    std::int32_t isign = query(A(0, 0), A(0, 1), A(0, 2), A(1, 0), A(1, 1), A(1, 2),
        A(2, 0), A(2, 1), A(2, 2));
    UTAssert(isign == -1 && query.mISign == -1,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == -0.0337105580f &&
        query.mIDet[1] == -0.0337098800f,
        "Incorrect interval for sign.");

    Matrix3x3<IRat> rA;
    for (std::size_t row = 0; row < 3; ++row)
    {
        for (std::size_t col = 0; col < 3; ++col)
        {
            rA(row, col) = A(row, col);
        }
    }
    auto GetRValues = [&rA]()
    {
        return std::array<IRat const*, 9>{
            &rA(0, 0), &rA(0, 1), &rA(0, 2),
            &rA(1, 0), &rA(1, 1), &rA(1, 2),
            &rA(2, 0), &rA(2, 1), &rA(2, 2)
        };
    };

    std::int32_t rsign = query(A(0, 0), A(0, 1), A(0, 2), A(1, 0), A(1, 1), A(1, 2),
        A(2, 0), A(2, 1), A(2, 2), GetRValues);
    UTAssert(rsign == -1 && query.mISign == -1,
        "Incorrect sign.");

    query.ComputeRational(A(0, 0), A(0, 1), A(0, 2), A(1, 0), A(1, 1), A(1, 2),
        A(2, 0), A(2, 1), A(2, 2));
    auto const& rdet = query.mNode[query.detNode];
    UTAssert(
        rdet.GetSign() == -1 &&
        rdet.GetBiasedExponent() == -66 &&
        rdet.GetUInteger().GetNumBits() == 62 &&
        rdet.GetUInteger().GetBits()[0] == 0xfcdaad61 &&
        rdet.GetUInteger().GetBits()[1] == 0x2284ee11,
        "Mismatched rational determinants."
    );

    UTAssert(
        CRat(query.mIDet[0]) <= rdet &&
        rdet <= CRat(query.mIDet[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactSignDeterminant3::Test1()
{
    ExactSignDeterminant3<float> query;
    Matrix3x3<float> R, D, A;
    float d0 = 1.0e-32f, d1 = 1.0f, d2 = 1.0e-16f;
    MakeDiagonal({ d0, d1, d2 }, D);
    Vector3<float> U0 = { 1.0f, 2.0f, 3.0f }, U1, U2;
    ComputeOrthonormalBasis(1, U0, U1, U2);
    R.SetCol(0, U0);
    R.SetCol(1, U1);
    R.SetCol(2, U2);
    A = MultiplyABT(R, D * R);

    std::int32_t isign = query(A(0, 0), A(0, 1), A(0, 2), A(1, 0), A(1, 1), A(1, 2),
        A(2, 0), A(2, 1), A(2, 2));
    UTAssert(isign == 1 && query.mISign == query.invalidSign,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == -7.006e-45f &&
        query.mIDet[1] == +7.006e-45f,
        "Incorrect interval for sign.");

    Matrix3x3<IRat> rA;
    for (std::size_t row = 0; row < 3; ++row)
    {
        for (std::size_t col = 0; col < 3; ++col)
        {
            rA(row, col) = A(row, col);
        }
    }
    auto GetRValues = [&rA]()
    {
        return std::array<IRat const*, 9>{
            &rA(0, 0), & rA(0, 1), & rA(0, 2),
                & rA(1, 0), & rA(1, 1), & rA(1, 2),
                & rA(2, 0), & rA(2, 1), & rA(2, 2)
        };
    };

    std::int32_t rsign = query(A(0, 0), A(0, 1), A(0, 2), A(1, 0), A(1, 1), A(1, 2),
        A(2, 0), A(2, 1), A(2, 2), GetRValues);
    UTAssert(rsign == 1 && query.mISign == query.invalidSign,
        "Incorrect sign.");

    query.ComputeRational(A(0, 0), A(0, 1), A(0, 2), A(1, 0), A(1, 1), A(1, 2),
        A(2, 0), A(2, 1), A(2, 2));
    auto const& rdet = query.mNode[query.detNode];
    UTAssert(
        rdet.GetSign() == 1 &&
        rdet.GetBiasedExponent() == -237 &&
        rdet.GetUInteger().GetNumBits() == 78 &&
        rdet.GetUInteger().GetBits()[0] == 0x816e4691 &&
        rdet.GetUInteger().GetBits()[1] == 0x9f0603e3 &&
        rdet.GetUInteger().GetBits()[2] == 0x00002ec4,
        "Mismatched rational determinants."
    );

    UTAssert(
        CRat(query.mIDet[0]) <= rdet &&
        rdet <= CRat(query.mIDet[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactSignDeterminant3::Test2()
{
    ExactSignDeterminant3<float> query;
    Matrix3x3<float> A;
    A(0, 0) = 1.0f; A(0, 1) = 2.0f; A(0, 2) = 3.0f;
    A(1, 0) = 0.0f; A(1, 1) = -1.0f; A(1, 2) = 7.0f;
    A(2, 0) = 3.0f * A(0, 0) + 5.0f * A(1, 0);
    A(2, 1) = 3.0f * A(0, 1) + 5.0f * A(1, 1);
    A(2, 2) = 3.0f * A(0, 2) + 5.0f * A(1, 2);

    std::int32_t isign = query(A(0, 0), A(0, 1), A(0, 2), A(1, 0), A(1, 1), A(1, 2),
        A(2, 0), A(2, 1), A(2, 2));
    UTAssert(isign == 0 && query.mISign == query.invalidSign,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == -2.67028827e-05f &&
        query.mIDet[1] == 2.67028827e-05f,
        "Incorrect interval for sign.");

    Matrix3x3<IRat> rA;
    for (std::size_t row = 0; row < 3; ++row)
    {
        for (std::size_t col = 0; col < 3; ++col)
        {
            rA(row, col) = A(row, col);
        }
    }
    auto GetRValues = [&rA]()
    {
        return std::array<IRat const*, 9>{
            &rA(0, 0), & rA(0, 1), & rA(0, 2),
                & rA(1, 0), & rA(1, 1), & rA(1, 2),
                & rA(2, 0), & rA(2, 1), & rA(2, 2)
        };
    };

    std::int32_t rsign = query(A(0, 0), A(0, 1), A(0, 2), A(1, 0), A(1, 1), A(1, 2),
        A(2, 0), A(2, 1), A(2, 2), GetRValues);
    UTAssert(rsign == 0 && query.mISign == query.invalidSign,
        "Incorrect sign.");

    query.ComputeRational(A(0, 0), A(0, 1), A(0, 2), A(1, 0), A(1, 1), A(1, 2),
        A(2, 0), A(2, 1), A(2, 2));
    auto const& rdet = query.mNode[query.detNode];
    UTAssert(
        rdet.GetSign() == 0 &&
        rdet.GetBiasedExponent() == 0 &&
        rdet.GetUInteger().GetNumBits() == 0,
        "Mismatched rational determinants."
    );

    UTAssert(
        CRat(query.mIDet[0]) <= rdet &&
        rdet <= CRat(query.mIDet[1]),
        "Interval does not contain the determinant.");
}

#else

#include <GTL/Mathematics/Geometry/1D/ExactSignDeterminant3.h>

namespace gtl
{
    template class ExactSignDeterminant3<float>;
    template class ExactSignDeterminant3<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ExactSignDeterminant3)
