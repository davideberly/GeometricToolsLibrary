// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#pragma once

// Representation of banded matrices. All matrices are stored in row-major
// order.

#include <GTL/Mathematics/Algebra/MatrixAccessor.h>
#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Utility/Exceptions.h>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <vector>

namespace gtl
{
    template <typename T>
    class BandedMatrix
    {
    public:
        // Construction and destruction.
        BandedMatrix(std::size_t size, std::size_t numLBands, std::size_t numUBands)
            :
            mSize(size),
            mDBand(size, C_<T>(0)),
            mLBands(numLBands),
            mUBands(numUBands),
            mZero(C_<T>(0))
        {
            GTL_ARGUMENT_ASSERT(
                size > 0 && numLBands < size && numUBands < size,
                "Invalid input.");

            if (numLBands > 0)
            {
                mLBands.resize(numLBands);
                std::size_t numElements = size;
                for (auto& band : mLBands)
                {
                    band.resize(--numElements);
                    std::fill(band.begin(), band.end(), C_<T>(0));
                }
            }

            if (numUBands > 0)
            {
                mUBands.resize(numUBands);
                std::size_t numElements = size;
                for (auto& band : mUBands)
                {
                    band.resize(--numElements);
                    std::fill(band.begin(), band.end(), C_<T>(0));
                }
            }
        }

        ~BandedMatrix() = default;

        // Access to the D-band.
        inline std::size_t GetSize() const
        {
            return mSize;
        }

        inline std::vector<T>& GetDBand()
        {
            return mDBand;
        }

        inline std::vector<T> const& GetDBand() const
        {
            return mDBand;
        }

        // Access to the L-bands.
        inline std::size_t GetNumLBands() const
        {
            return mLBands.size();
        }

        inline std::vector<std::vector<T>>& GetLBands()
        {
            return mLBands;
        }

        inline std::vector<std::vector<T>> const& GetLBands() const
        {
            return mLBands;
        }

        std::vector<T> const& GetLBand(std::size_t band) const
        {
            GTL_ARGUMENT_ASSERT(
                band < mLBands.size(),
                "Invalid index.");

            return mLBands[band];
        }

        std::vector<T>& GetLBand(std::size_t band)
        {
            GTL_ARGUMENT_ASSERT(
                band < mLBands.size(),
                "Invalid index.");

            return mLBands[band];
        }

        // Access to the U-bands.
        inline std::size_t GetNumUBands() const
        {
            return mUBands.size();
        }

        inline std::vector<std::vector<T>>& GetUBands()
        {
            return mUBands;
        }

        inline std::vector<std::vector<T>> const& GetUBands() const
        {
            return mUBands;
        }

        std::vector<T> const& GetUBand(std::size_t band) const
        {
            GTL_ARGUMENT_ASSERT(
                band < mUBands.size(),
                "Invalid index.");

            return mUBands[band];
        }

        std::vector<T>& GetUBand(std::size_t band)
        {
            GTL_ARGUMENT_ASSERT(
                band < mUBands.size(),
                "Invalid index.");

            return mUBands[band];
        }

        // Access to matrix elements.
        T& operator()(std::size_t r, std::size_t c)
        {
            GTL_ARGUMENT_ASSERT(
                r < mSize && c < mSize,
                "Invalid index.");

            if (c > r)
            {
                if (c < mSize)
                {
                    std::size_t band = c - r - 1;
                    if (band < mUBands.size())
                    {
                        return mUBands[band][r];
                    }
                }
            }
            else if (c < r)
            {
                if (r < mSize)
                {
                    std::size_t band = r - c - 1;
                    if (band < mLBands.size())
                    {
                        return mLBands[band][c];
                    }
                }
            }
            else
            {
                return mDBand[r];
            }

            // Set the value to zero in case someone unknowingly modified
            // mZero on a previous call to operator(std::size_t, std::size_t).
            mZero = C_<T>(0);
            return mZero;
        }

        T const& operator()(std::size_t r, std::size_t c) const
        {
            GTL_ARGUMENT_ASSERT(
                r < mSize&& c < mSize,
                "Invalid index.");

            if (c > r)
            {
                if (c < mSize)
                {
                    std::size_t band = c - r - 1;
                    if (band < mUBands.size())
                    {
                        return mUBands[band][r];
                    }
                }
            }
            else if (c < r)
            {
                if (r < mSize)
                {
                    std::size_t band = r - c - 1;
                    if (band < mLBands.size())
                    {
                        return mLBands[band][c];
                    }
                }
            }
            else
            {
                return mDBand[r];
            }

            // Set the value to zero in case someone unknowingly modified
            // mZero on a previous call to operator(std::size_t, std::size_t).
            mZero = C_<T>(0);
            return mZero;
        }


        // The following operations apply only to symmetric banded matrices.

        // Factor the square banded matrix A into A = L*L^T, where L is a
        // lower-triangular matrix (L^T is an upper-triangular matrix). This
        // is an LU decomposition that allows for stable inversion of A to
        // solve A*X = B. Matrix A contains the Cholesky factorization: L in
        // the lower-triangular part of A and L^T in the upper-triangular part
        // of A. NOTE: This is a specialized version of the algorithm found in
        // CholeskyDecomposition.h.
        bool CholeskyFactor()
        {
            if (mDBand.size() == 0 || mLBands.size() != mUBands.size())
            {
                // Invalid number of bands.
                return false;
            }

            std::size_t const sizeM1 = mSize - 1;
            std::size_t const numBands = mLBands.size();

            std::size_t k{}, kMax{};
            for (std::size_t i = 0; i < mSize; ++i)
            {
                std::size_t jMin = (i >= numBands ? i - numBands : 0);

                std::size_t j{};
                for (j = jMin; j < i; ++j)
                {
                    kMax = j + numBands;
                    if (kMax > sizeM1)
                    {
                        kMax = sizeM1;
                    }

                    for (k = i; k <= kMax; ++k)
                    {
                        operator()(k, i) -= operator()(i, j) * operator()(k, j);
                    }
                }

                kMax = j + numBands;
                if (kMax > sizeM1)
                {
                    kMax = sizeM1;
                }

                for (k = 0; k < i; ++k)
                {
                    operator()(k, i) = operator()(i, k);
                }

                T diagonal = operator()(i, i);
                if (diagonal <= C_<T>(0))
                {
                    return false;
                }
                T invSqrt = C_<T>(1) / std::sqrt(diagonal);
                for (k = i; k <= kMax; ++k)
                {
                    operator()(k, i) *= invSqrt;
                }
            }

            return true;
        }

        // Solve the linear system A*X = B, where A is an NxN banded matrix,
        // and where B and X are Nx1 vectors. The input to this function is B.
        // The output X is computed and stored in B. The return value is
        // 'true' iff the system has a solution. The matrix A and the vector B
        // are both modified by this function. If successful, matrix A
        // contains the Cholesky factorization with L in the lower-triangular
        // part of A and L^T in the upper-triangular part of A.
        bool SolveSystem(T* bVector)
        {
            return CholeskyFactor()
                && SolveLower(bVector)
                && SolveUpper(bVector);
        }

        // Solve the linear system A*X = B, where A is an NxN banded matrix,
        // and where B and X are NxM matrices stored in row-major order. The
        // The input to this function is B. The output X is computed and
        // stored in B. The return value is 'true' iff the system has a
        // solution. The matrix A and the vector B are both modified by this
        // function. If successful, matrix A contains the Cholesky
        // factorization with L in the lower-triangular part of A and L^T in
        // the upper-triangular part of A.
        bool SolveSystem(T* bMatrix, std::size_t numBColumns)
        {
            return CholeskyFactor()
                && SolveLower(bMatrix, numBColumns)
                && SolveUpper(bMatrix, numBColumns);
        }

        // Compute the inverse of A, where A is an NxN banded matrix. The
        // return value is 'true' when A is invertible, in which case A^{-1}
        // is NxN and returned in 'inverse' in row-major order. The return
        // value is 'false' when A is not invertible, in which case 'inverse'
        // is invalid and should not be used.
        bool ComputeInverse(T* inverse)
        {
            MatrixAccessor<T, true> invA(mSize, mSize, inverse);
            for (std::size_t row = 0; row < mSize; ++row)
            {
                for (std::size_t col = 0; col < mSize; ++col)
                {
                    if (row != col)
                    {
                        invA(row, col) = C_<T>(0);
                    }
                    else
                    {
                        invA(row, row) = C_<T>(1);
                    }
                }
            }

            return SolveSystem(inverse, mSize);
        }

    private:
        // The linear system is L*U*X = B, where A = L*U and U = L^T. Reduce
        // this to U*X = L^{-1}*B. The return value is 'true' iff the
        // operation is successful.
        bool SolveLower(T* dataVector) const
        {
            std::size_t const size = mDBand.size();
            for (std::size_t r = 0; r < size; ++r)
            {
                T lowerRR = operator()(r, r);
                if (lowerRR > C_<T>(0))
                {
                    for (std::size_t c = 0; c < r; ++c)
                    {
                        T lowerRC = operator()(r, c);
                        dataVector[r] -= lowerRC * dataVector[c];
                    }
                    dataVector[r] /= lowerRR;
                }
                else
                {
                    return false;
                }
            }
            return true;
        }

        // The linear system is U*X = L^{-1}*B. Reduce this to
        // X = U^{-1}*L^{-1}*B. The return value is 'true' iff the operation
        // is successful.
        bool SolveUpper(T* dataVector) const
        {
            std::size_t const size = mDBand.size();
            for (std::size_t k = 0, r = size - 1; k < size; ++k, --r)
            {
                T upperRR = operator()(r, r);
                if (upperRR > C_<T>(0))
                {
                    for (std::size_t c = r + 1; c < size; ++c)
                    {
                        T upperRC = operator()(r, c);
                        dataVector[r] -= upperRC * dataVector[c];
                    }

                    dataVector[r] /= upperRR;
                }
                else
                {
                    return false;
                }
            }
            return true;
        }

        // The linear system is L*U*X = B, where A = L*U and U = L^T. Reduce
        // this to U*X = L^{-1}*B. The return value is 'true' iff the
        // operation is successful.
        bool SolveLower(T* dataMatrix, std::size_t numColumns) const
        {
            MatrixAccessor<T, true> data(mSize, numColumns, dataMatrix);

            for (std::size_t r = 0; r < mSize; ++r)
            {
                T lowerRR = operator()(r, r);
                if (lowerRR > C_<T>(0))
                {
                    for (std::size_t c = 0; c < r; ++c)
                    {
                        T lowerRC = operator()(r, c);
                        for (std::size_t bCol = 0; bCol < numColumns; ++bCol)
                        {
                            data(r, bCol) -= lowerRC * data(c, bCol);
                        }
                    }

                    T inverse = C_<T>(1) / lowerRR;
                    for (std::size_t bCol = 0; bCol < numColumns; ++bCol)
                    {
                        data(r, bCol) *= inverse;
                    }
                }
                else
                {
                    return false;
                }
            }
            return true;
        }

        // The linear system is U*X = L^{-1}*B. Reduce this to
        // X = U^{-1}*L^{-1}*B. The return value is 'true' iff the operation
        // is successful.
        bool SolveUpper(T* dataMatrix, std::size_t numColumns) const
        {
            MatrixAccessor<T, true> data(mSize, numColumns, dataMatrix);

            for (std::size_t k = 0, r = mSize - 1; k < mSize; ++k, --r)
            {
                T upperRR = operator()(r, r);
                if (upperRR > C_<T>(0))
                {
                    for (std::size_t c = r + 1; c < mSize; ++c)
                    {
                        T upperRC = operator()(r, c);
                        for (std::size_t bCol = 0; bCol < numColumns; ++bCol)
                        {
                            data(r, bCol) -= upperRC * data(c, bCol);
                        }
                    }

                    T inverse = C_<T>(1) / upperRR;
                    for (std::size_t bCol = 0; bCol < numColumns; ++bCol)
                    {
                        data(r, bCol) *= inverse;
                    }
                }
                else
                {
                    return false;
                }
            }
            return true;
        }

        std::size_t mSize;
        std::vector<T> mDBand;
        std::vector<std::vector<T>> mLBands, mUBands;
        mutable T mZero;

    private:
        friend class UnitTestBandedMatrix;
    };
}
