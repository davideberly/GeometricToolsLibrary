// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.12

#pragma once

// Adapters to allow access to a matrix that is stored in contiguous memory.
// You can specify whether the matrix is row-major or column-major ordered.

#include <cstddef>

namespace gtl
{
    template <typename T>
    class MatrixAccessorBase
    {
    public:
        inline std::size_t const size() const
        {
            return mNumRows * mNumCols;
        }

        inline T const* data() const
        {
            return mElements();
        }

        inline T* data()
        {
            return mElements();
        }

        inline T const& operator[](std::size_t i) const
        {
            return mElements[i];
        }

        inline T& operator[](std::size_t i)
        {
            return mElements[i];
        }

        void reset(std::size_t numRows, std::size_t numCols, T* elements)
        {
            mNumRows = numRows;
            mNumCols = numCols;
            mElements = elements;
        }

    protected:
        MatrixAccessorBase(std::size_t numRows = 0, std::size_t numCols = 0, T* elements = nullptr)
            :
            mNumRows(numRows),
            mNumCols(numCols),
            mElements(elements)
        {
        }

        std::size_t mNumRows, mNumCols;
        T* mElements;

    private:
        friend class UnitTestMatrixAccessor;
    };


    template <typename T, bool IsRowMajor> class MatrixAccessor;

    // Access to row-major storage.
    template <typename T>
    class MatrixAccessor<T, true> : public MatrixAccessorBase<T>
    {
    public:
        MatrixAccessor(std::size_t numRows = 0, std::size_t numCols = 0, T* elements = nullptr)
            :
            MatrixAccessorBase<T>(numRows, numCols, elements)
        {
        }

        inline T const& operator()(std::size_t row, std::size_t col) const
        {
            return this->mElements[col + this->mNumCols * row];
        }

        inline T& operator()(std::size_t row, std::size_t col)
        {
            return this->mElements[col + this->mNumCols * row];
        }

    private:
        friend class UnitTestMatrixAccessor;
    };

    // Access to column-major storage.
    template <typename T>
    class MatrixAccessor<T, false> : public MatrixAccessorBase<T>
    {
    public:
        MatrixAccessor(std::size_t numRows = 0, std::size_t numCols = 0, T* elements = nullptr)
            :
            MatrixAccessorBase<T>(numRows, numCols, elements)
        {
        }

        inline T const& operator()(std::size_t row, std::size_t col) const
        {
            return this->mElements[row + this->mNumRows * col];
        }

        inline T& operator()(std::size_t row, std::size_t col)
        {
            return this->mElements[row + this->mNumRows * col];
        }

    private:
        friend class UnitTestMatrixAccessor;
    };
}
