// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Algebra/MatrixAccessor.h>
#include <array>
#include <numeric>
using namespace gtl;

namespace gtl
{
    class UnitTestMatrixAccessor
    {
    public:
        UnitTestMatrixAccessor();

    private:
        void Test();
    };
}

UnitTestMatrixAccessor::UnitTestMatrixAccessor()
{
    UTInformation("Mathematics/Algebra/MatrixAccessor");

    Test();
}

void UnitTestMatrixAccessor::Test()
{
    std::array<float, 6> rowMajor{};
    std::iota(rowMajor.begin(), rowMajor.end(), 0.0f);
    MatrixAccessor<float, true> rowMajorMatrix(2, 3, rowMajor.data());
    UTAssert(rowMajorMatrix(0, 1) == 1.0f, "Invalid row-matrix element (0,1).");
    UTAssert(rowMajorMatrix(1, 0) == 3.0f, "Invalid row-matrix element (1,0).");

    std::array<float, 6> colMajor{};
    std::iota(colMajor.begin(), colMajor.end(), 0.0f);
    MatrixAccessor<float, false> colMajorMatrix(2, 3, colMajor.data());
    UTAssert(colMajorMatrix(0, 1) == 2.0f, "Invalid col-matrix element (0,1).");
    UTAssert(colMajorMatrix(1, 0) == 1.0f, "Invalid col-matrix element (1,0).");
}

#else

#if defined(GTL_INSTANTIATE_RATIONAL)
#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#endif
#include <GTL/Mathematics/Algebra/MatrixAccessor.h>

namespace gtl
{
    template class MatrixAccessor<float, true>;
    template class MatrixAccessor<float, false>;
    template class MatrixAccessor<double, true>;
    template class MatrixAccessor<double, false>;

#if defined(GTL_INSTANTIATE_RATIONAL)
    using Rational = BSRational<UIntegerAP32>;
    template class MatrixAccessor<Rational, true>;
    template class MatrixAccessor<Rational, false>;
#endif
}

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(MatrixAccessor)
