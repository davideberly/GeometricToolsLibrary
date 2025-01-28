#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Geometry/1D/ExactSignDeterminant2.h>
#include <GTL/Mathematics/Algebra/Matrix.h>
using namespace gtl;

namespace gtl
{
    class UnitTestExactSignDeterminant2
    {
    public:
        UnitTestExactSignDeterminant2();

    private:
        using IRat = ExactSignDeterminant2<float>::Rational;
        using CRat = ExactSignDeterminant2<float>::CRational;

        void Test0();
        void Test1();
        void Test2();
    };
}

UnitTestExactSignDeterminant2::UnitTestExactSignDeterminant2()
{
    UTInformation("Mathematics/Geometry/1D/ExactSignDeterminant2");

    Test0();
    Test1();
    Test2();
}

void UnitTestExactSignDeterminant2::Test0()
{
    ExactSignDeterminant2<float> query;
    Matrix2x2<float> A;
    A(0, 0) = -729.04599140643904f;
    A(0, 1) = 670.01717998915910f;
    A(1, 0) = 937.73554224846271f;
    A(1, 1) = 1.23456789f;

    std::int32_t isign = query(A(0, 0), A(0, 1), A(1, 0), A(1, 1));
    UTAssert(isign == -1 && query.mISign == -1,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == -629199.062f &&
        query.mIDet[1] == -629198.812f,
        "Incorrect interval for sign.");

    IRat ra00(A(0, 0)), ra01(A(0, 1)), ra10(A(1, 0)), ra11(A(1, 1));
    auto GetRValues = [&ra00, &ra01, &ra10, &ra11]()
    {
        return std::array<IRat const*, 4>{ &ra00, &ra01, &ra10, &ra11 };
    };

    std::int32_t rsign = query(A(0, 0), A(0, 1), A(1, 0), A(1, 1), GetRValues);
    UTAssert(rsign == -1 && query.mISign == -1,
        "Incorrect sign.");

    query.ComputeRational(A(0, 0), A(0, 1), A(1, 0), A(1, 1));
    auto const& rdet = query.mNode[query.detNode];
    UTAssert(
        rdet.GetSign() == -1 &&
        rdet.GetBiasedExponent() == -35 &&
        rdet.GetUInteger().GetNumBits() == 55 &&
        rdet.GetUInteger().GetBits()[0] == 0x9589b4e1 &&
        rdet.GetUInteger().GetBits()[1] == 0x004cce77,
        "Mismatched rational determinants."
    );

    UTAssert(
        CRat(query.mIDet[0]) <= rdet && 
        rdet <= CRat(query.mIDet[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactSignDeterminant2::Test1()
{
    ExactSignDeterminant2<float> query;
    Matrix2x2<float> R, D, A;
    float angle = 0.12345f;
    R(0, 0) = std::cos(angle);
    R(1, 0) = std::sin(angle);
    R(0, 1) = -R(1, 0);
    R(1, 1) = R(0, 0);
    D(0, 0) = 1.0e-32f;
    D(1, 1) = -1.0e-16f;
    A = MultiplyABT(R * D, R);

    std::int32_t isign = query(A(0, 0), A(0, 1), A(1, 0), A(1, 1));
    UTAssert(isign == 1 && query.mISign == query.invalidSign,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == -1.14808e-41f &&
        query.mIDet[1] == +3.44397e-41f,
        "Incorrect interval for sign.");

    IRat ra00(A(0, 0)), ra01(A(0, 1)), ra10(A(1, 0)), ra11(A(1, 1));
    auto GetRValues = [&ra00, &ra01, &ra10, &ra11]()
    {
        return std::array<IRat const*, 4>{ &ra00, & ra01, & ra10, & ra11 };
    };

    std::int32_t rsign = query(A(0, 0), A(0, 1), A(1, 0), A(1, 1), GetRValues);
    UTAssert(rsign == 1 && query.mISign == query.invalidSign,
        "Incorrect sign.");

    query.ComputeRational(A(0, 0), A(0, 1), A(1, 0), A(1, 1));
    auto const& rdet = query.mNode[query.detNode];
    UTAssert(
        rdet.GetSign() == 1 &&
        rdet.GetBiasedExponent() == -160 &&
        rdet.GetUInteger().GetNumBits() == 23 &&
        rdet.GetUInteger().GetBits()[0] == 0x0077c139,
        "Mismatched rational determinants."
    );

    UTAssert(
        CRat(query.mIDet[0]) <= rdet &&
        rdet <= CRat(query.mIDet[1]),
        "Interval does not contain the determinant.");
}

void UnitTestExactSignDeterminant2::Test2()
{
    ExactSignDeterminant2<float> query;
    Matrix2x2<float> A;
    A(0, 0) = 1.0f; A(0, 1) = 2.0f;
    A(1, 0) = 3.0f; A(1, 1) = 6.0f;

    std::int32_t isign = query(A(0, 0), A(0, 1), A(1, 0), A(1, 1));
    UTAssert(isign == 0 && query.mISign == query.invalidSign,
        "Incorrect sign.");
    UTAssert(
        query.mIDet[0] == -9.53674430e-07f &&
        query.mIDet[1] == 9.53674430e-07f,
        "Incorrect interval for sign.");

    IRat ra00(A(0, 0)), ra01(A(0, 1)), ra10(A(1, 0)), ra11(A(1, 1));
    auto GetRValues = [&ra00, &ra01, &ra10, &ra11]()
    {
        return std::array<IRat const*, 4>{ &ra00, & ra01, & ra10, & ra11 };
    };

    std::int32_t rsign = query(A(0, 0), A(0, 1), A(1, 0), A(1, 1), GetRValues);
    UTAssert(rsign == 0 && query.mISign == query.invalidSign,
        "Incorrect sign.");

    query.ComputeRational(A(0, 0), A(0, 1), A(1, 0), A(1, 1));
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

#include <GTL/Mathematics/Geometry/1D/ExactSignDeterminant2.h>

namespace gtl
{
    template class ExactSignDeterminant2<float>;
    template class ExactSignDeterminant2<double>;
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(ExactSignDeterminant2)
