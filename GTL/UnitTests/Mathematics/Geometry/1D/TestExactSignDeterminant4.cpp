#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/1D/ExactSignDeterminant4.h>
#include <GTL/Mathematics/Algebra/Matrix.h>
using namespace gtl;

namespace gtl
{
    class UnitTestExactSignDeterminant4
    {
    public:
        UnitTestExactSignDeterminant4();

    private:
        using IRat = ExactSignDeterminant4<float>::Rational;
        using CRat = ExactSignDeterminant4<float>::CRational;

        void Test0();
        void Test1();
        void Test2();
    };
}

UnitTestExactSignDeterminant4::UnitTestExactSignDeterminant4()
{
    UTInformation("Mathematics/Geometry/1D/ExactSignDeterminant4");

    Test0();
    Test1();
    Test2();
}

void UnitTestExactSignDeterminant4::Test0()
{
    ExactSignDeterminant4<float> query;
    Matrix4x4<float> A;
    A(0, 0) = 0.629447341f;
    A(0, 1) = -0.729045987f;
    A(0, 2) = 0.811583877f;
    A(0, 3) = 0.670017123f;
    A(1, 0) = -0.746026397f;
    A(1, 1) = 0.937735558f;
    A(1, 2) = 0.826751709f;
    A(1, 3) = -0.557931900f;
    A(2, 0) = 0.264718533f;
    A(2, 1) = -0.383665919f;
    A(2, 2) = -0.804919183f;
    A(2, 3) = 0.0944411755f;
    A(3, 0) = -0.443003535f;
    A(3, 1) = -0.623236060f;
    A(3, 2) = 0.0937629938f;
    A(3, 3) = 0.985762596f;

    std::int32_t isign = query(
        A(0, 0), A(0, 1), A(0, 2), A(0, 3),
        A(1, 0), A(1, 1), A(1, 2), A(1, 3),
        A(2, 0), A(2, 1), A(2, 2), A(2, 3),
        A(3, 0), A(3, 1), A(3, 2), A(3, 3));
    UTAssert(isign == 1 && query.mISign == 1, "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == 0.0931796804f &&
        query.mIDet[1] == 0.0931809619f,
        "Incorrect interval for sign.");

    Matrix4x4<IRat> rA;
    for (std::size_t row = 0; row < 4; ++row)
    {
        for (std::size_t col = 0; col < 4; ++col)
        {
            rA(row, col) = A(row, col);
        }
    }
    auto GetRValues = [&rA]()
    {
        return std::array<IRat const*, 16>{
            &rA(0, 0), &rA(0, 1), &rA(0, 2), &rA(0, 3),
            &rA(1, 0), &rA(1, 1), &rA(1, 2), &rA(1, 3),
            &rA(2, 0), &rA(2, 1), &rA(2, 2), &rA(2, 3),
            &rA(3, 0), &rA(3, 1), &rA(3, 2), &rA(3, 3)
        };
    };

    std::int32_t rsign = query(
        A(0, 0), A(0, 1), A(0, 2), A(0, 3),
        A(1, 0), A(1, 1), A(1, 2), A(1, 3),
        A(2, 0), A(2, 1), A(2, 2), A(2, 3),
        A(3, 0), A(3, 1), A(3, 2), A(3, 3),
        GetRValues);
    UTAssert(rsign == 1 && query.mISign == 1, "Incorrect sign.");

    query.ComputeRational(
        A(0, 0), A(0, 1), A(0, 2), A(0, 3),
        A(1, 0), A(1, 1), A(1, 2), A(1, 3),
        A(2, 0), A(2, 1), A(2, 2), A(2, 3),
        A(3, 0), A(3, 1), A(3, 2), A(3, 3));
    auto const& rdet = query.mNode[query.detNode];
    UTAssert(
        query.mNode[0].GetSign() == 1 &&
        query.mNode[0].GetBiasedExponent() == -93 &&
        query.mNode[0].GetUInteger().GetNumBits() == 90 &&
        query.mNode[0].GetUInteger().GetBits()[0] == 0x9e3da961 &&
        query.mNode[0].GetUInteger().GetBits()[1] == 0xb8f85215 &&
        query.mNode[0].GetUInteger().GetBits()[2] == 0x02fb55be,
        "Mismatched rational determinants."
    );

    UTAssert(
        CRat(query.mIDet[0]) <= rdet &&
        rdet <= CRat(query.mIDet[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactSignDeterminant4::Test1()
{
    ExactSignDeterminant4<float> query;
    Matrix<float, 4, 4> R, D, A;
    float d0 = 1.0e-32f, d1 = 1.0f, d2 = 1.0e-16f, d3 = 1.0f;
    MakeDiagonal({ d0, d1, d2, d3 }, D);
    Vector4<float> U0 = { 1.0f, 2.0f, 3.0f, 4.0f }, U1, U2, U3;
    ComputeOrthonormalBasis(1, U0, U1, U2, U3);
    R.SetCol(0, U0);
    R.SetCol(1, U1);
    R.SetCol(2, U2);
    R.SetCol(3, U3);
    A = MultiplyABT(R, D * R);

    std::int32_t isign = query(
        A(0, 0), A(0, 1), A(0, 2), A(0, 3),
        A(1, 0), A(1, 1), A(1, 2), A(1, 3),
        A(2, 0), A(2, 1), A(2, 2), A(2, 3),
        A(3, 0), A(3, 1), A(3, 2), A(3, 3));
    UTAssert(isign == 1 && query.mISign == query.invalidSign,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == -1.54142e-44f &&
        query.mIDet[1] == +1.26116e-44f,
        "Incorrect interval for sign.");

    Matrix4x4<IRat> rA;
    for (std::size_t row = 0; row < 4; ++row)
    {
        for (std::size_t col = 0; col < 4; ++col)
        {
            rA(row, col) = A(row, col);
        }
    }
    auto GetRValues = [&rA]()
    {
        return std::array<IRat const*, 16>{
            &rA(0, 0), &rA(0, 1), &rA(0, 2), &rA(0, 3),
            &rA(1, 0), &rA(1, 1), &rA(1, 2), &rA(1, 3),
            &rA(2, 0), &rA(2, 1), &rA(2, 2), &rA(2, 3),
            &rA(3, 0), &rA(3, 1), &rA(3, 2), &rA(3, 3)
        };
    };

    std::int32_t rsign = query(
        A(0, 0), A(0, 1), A(0, 2), A(0, 3),
        A(1, 0), A(1, 1), A(1, 2), A(1, 3),
        A(2, 0), A(2, 1), A(2, 2), A(2, 3),
        A(3, 0), A(3, 1), A(3, 2), A(3, 3),
        GetRValues);
    UTAssert(rsign == 1 && query.mISign == query.invalidSign,
        "Incorrect sign.");

    query.ComputeRational(
        A(0, 0), A(0, 1), A(0, 2), A(0, 3),
        A(1, 0), A(1, 1), A(1, 2), A(1, 3),
        A(2, 0), A(2, 1), A(2, 2), A(2, 3),
        A(3, 0), A(3, 1), A(3, 2), A(3, 3));
    auto const& rdet = query.mNode[query.detNode];
    UTAssert(
        rdet.GetSign() == 1 &&
        rdet.GetBiasedExponent() == -269 &&
        rdet.GetUInteger().GetNumBits() == 110 &&
        rdet.GetUInteger().GetBits()[0] == 0xc02fe50du &&
        rdet.GetUInteger().GetBits()[1] == 0x894ed759u &&
        rdet.GetUInteger().GetBits()[2] == 0x9f3fa948u &&
        rdet.GetUInteger().GetBits()[3] == 0x00002ec4u,
        "Mismatched rational determinants."
    );

    UTAssert(
        CRat(query.mIDet[0]) <= rdet &&
        rdet <= CRat(query.mIDet[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactSignDeterminant4::Test2()
{
    ExactSignDeterminant4<float> query;
    Matrix<float, 4, 4> A;
    A(0, 0) = 1.0f; A(0, 1) = 2.0f;  A(0, 2) = 3.0f; A(0, 3) = -4.0f;
    A(1, 0) = 0.0f; A(1, 1) = -1.0f; A(1, 2) = 7.0f; A(1, 3) = -5.0f;
    A(2, 0) = 6.0f; A(2, 1) = 8.0f; A(2, 2) = 0.0f; A(2, 3) = 1.0f;
    A(3, 0) = 3.0f * A(0, 0) + 5.0f * A(1, 0) - 2.0f * A(2, 0);
    A(3, 1) = 3.0f * A(0, 1) + 5.0f * A(1, 1) - 2.0f * A(2, 1);
    A(3, 2) = 3.0f * A(0, 2) + 5.0f * A(1, 2) - 2.0f * A(2, 2);
    A(3, 3) = 3.0f * A(0, 3) + 5.0f * A(1, 3) - 2.0f * A(2, 3);

    std::int32_t isign = query(
        A(0, 0), A(0, 1), A(0, 2), A(0, 3),
        A(1, 0), A(1, 1), A(1, 2), A(1, 3),
        A(2, 0), A(2, 1), A(2, 2), A(2, 3),
        A(3, 0), A(3, 1), A(3, 2), A(3, 3));
    UTAssert(isign == 0 && query.mISign == query.invalidSign,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == -0.00593566941f &&
        query.mIDet[1] == 0.00422668504f,
        "Incorrect interval for sign.");

    Matrix4x4<IRat> rA;
    for (std::size_t row = 0; row < 4; ++row)
    {
        for (std::size_t col = 0; col < 4; ++col)
        {
            rA(row, col) = A(row, col);
        }
    }
    auto GetRValues = [&rA]()
    {
        return std::array<IRat const*, 16>{
            &rA(0, 0), &rA(0, 1), &rA(0, 2), &rA(0, 3),
            &rA(1, 0), &rA(1, 1), &rA(1, 2), &rA(1, 3),
            &rA(2, 0), &rA(2, 1), &rA(2, 2), &rA(2, 3),
            &rA(3, 0), &rA(3, 1), &rA(3, 2), &rA(3, 3)
        };
    };

    std::int32_t rsign = query(
        A(0, 0), A(0, 1), A(0, 2), A(0, 3),
        A(1, 0), A(1, 1), A(1, 2), A(1, 3),
        A(2, 0), A(2, 1), A(2, 2), A(2, 3),
        A(3, 0), A(3, 1), A(3, 2), A(3, 3),
        GetRValues);
    UTAssert(rsign == 0 && query.mISign == query.invalidSign,
        "Incorrect sign.");

    query.ComputeRational(
        A(0, 0), A(0, 1), A(0, 2), A(0, 3),
        A(1, 0), A(1, 1), A(1, 2), A(1, 3),
        A(2, 0), A(2, 1), A(2, 2), A(2, 3),
        A(3, 0), A(3, 1), A(3, 2), A(3, 3));
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

#include <GTL/Mathematics/Geometry/1D/ExactSignDeterminant4.h>

namespace gtl
{
    template class ExactSignDeterminant4<float>;
    template class ExactSignDeterminant4<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ExactSignDeterminant4)
