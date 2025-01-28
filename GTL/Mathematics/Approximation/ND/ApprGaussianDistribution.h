// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// Fit points with a Gaussian distribution. The center is the mean of the
// points, the axes are the eigenvectors of the covariance matrix and the
// extents are the eigenvalues of the covariance matrix and are returned in
// increasing order.

#include <GTL/Mathematics/MatrixAnalysis/SymmetricEigensolver.h>
#include <GTL/Mathematics/Algebra/Matrix.h>
#include <array>
#include <cstddef>
#include <numeric>
#include <string>
#include <vector>

namespace gtl
{
    template <typename T, std::size_t...> class ApprGaussianDistribution;

    template <typename T>
    class ApprGaussianDistribution<T, 2>
    {
    public:
        // Fit all input points[].
        static std::size_t Fit(std::vector<Vector2<T>> const& points,
            Vector2<T>& mean, std::array<T, 2>& eigenvalues,
            std::array<Vector2<T>, 2>& eigenvectors)
        {
            std::vector<std::size_t> indices(points.size());
            std::iota(indices.begin(), indices.end(), 0);
            return Fit(points, indices, false, mean, eigenvalues, eigenvectors);
        }

        // Fit some input points[]. The points[] are a vertex pool. The points
        // used for fitting are referenced by indices[]. Not all elements of
        // points[] must be used. If you are absolutely certain that the
        // indices satisfy 0 <= indices[i] < points.size() for all
        // 0 <= i < indices.size(), set validateIndices to false. Otherwise,
        // set validateIndices to true and allow the function to do the
        // validation.
        static std::size_t Fit(std::vector<Vector2<T>> const& points,
            std::vector<std::size_t> const& indices, bool validateIndices,
            Vector2<T>& mean, std::array<T, 2>& eigenvalues,
            std::array<Vector2<T>, 2>& eigenvectors)
        {
            GTL_ARGUMENT_ASSERT(
                points.size() >= 2 && indices.size() >= 2,
                "Invalid number of points or indices.");

            if (validateIndices)
            {
                for (std::size_t i = 0; i < indices.size(); ++i)
                {
                    if (indices[i] >= points.size())
                    {
                        GTL_ARGUMENT_ERROR(
                            "Invalid index indices[" + std::to_string(i) +
                            "] = " + std::to_string(indices[i]));
                    }
                }
            }

            // Compute the mean of the points.
            T const tNumIndices = static_cast<T>(indices.size());
            MakeZero(mean);
            for (auto const& index : indices)
            {
                mean += points[index];
            }
            mean /= tNumIndices;

            // Compute the covariance matrix of the points.
            T cov00 = C_<T>(0), cov01 = C_<T>(0), cov11 = C_<T>(0);
            for (auto const& index : indices)
            {
                Vector2<T> diff = points[index] - mean;
                cov00 += diff[0] * diff[0];
                cov01 += diff[0] * diff[1];
                cov11 += diff[1] * diff[1];
            }
            cov00 /= tNumIndices;
            cov01 /= tNumIndices;
            cov11 /= tNumIndices;

            // Solve the eigensystem.
            SymmetricEigensolver<T, 2> solver{};
            solver(cov00, cov01, cov11);

            // Copy the eigenstuff.
            eigenvalues = solver.GetEigenvalues();
            for (std::size_t i = 0; i < 2; ++i)
            {
                eigenvectors[i] = solver.GetEigenvector(i);
            }
            return 0;
        }

    private:
        friend class UnitTestApprGaussianDistribution;
    };

    template <typename T>
    class ApprGaussianDistribution<T, 3>
    {
    public:
        // Fit all input points[].
        static std::size_t Fit(std::vector<Vector3<T>> const& points,
            Vector3<T>& mean, std::array<T, 3>& eigenvalues,
            std::array<Vector3<T>, 3>& eigenvectors)
        {
            std::vector<std::size_t> indices(points.size());
            std::iota(indices.begin(), indices.end(), 0);
            return Fit(points, indices, false, mean, eigenvalues, eigenvectors);
        }

        // Fit some input points[]. The points[] are a vertex pool. The points
        // used for fitting are referenced by indices[]. Not all elements of
        // points[] must be used. If you are absolutely certain that the
        // indices satisfy 0 <= indices[i] < points.size() for all
        // 0 <= i < indices.size(), set validateIndices to false. Otherwise,
        // set validateIndices to true and allow the function to do the
        // validation.
        static std::size_t Fit(std::vector<Vector3<T>> const& points,
            std::vector<std::size_t> const& indices, bool validateIndices,
            Vector3<T>& mean, std::array<T, 3>& eigenvalues,
            std::array<Vector3<T>, 3>& eigenvectors)
        {
            GTL_ARGUMENT_ASSERT(
                points.size() >= 2 && indices.size() >= 2,
                "Invalid number of points or indices.");

            if (validateIndices)
            {
                for (std::size_t i = 0; i < indices.size(); ++i)
                {
                    if (indices[i] >= points.size())
                    {
                        GTL_ARGUMENT_ERROR(
                            "Invalid index indices[" + std::to_string(i) +
                            "] = " + std::to_string(indices[i]));
                    }
                }
            }

            // Compute the mean of the points.
            T const tNumIndices = static_cast<T>(indices.size());
            MakeZero(mean);
            for (auto const& index : indices)
            {
                mean += points[index];
            }
            mean /= tNumIndices;

            // Compute the covariance matrix of the points.
            T cov00 = C_<T>(0), cov01 = C_<T>(0), cov02 = C_<T>(0);
            T cov11 = C_<T>(0), cov12 = C_<T>(0), cov22 = C_<T>(0);
            for (auto const& index : indices)
            {
                Vector3<T> diff = points[index] - mean;
                cov00 += diff[0] * diff[0];
                cov01 += diff[0] * diff[1];
                cov02 += diff[0] * diff[2];
                cov11 += diff[1] * diff[1];
                cov12 += diff[1] * diff[2];
                cov22 += diff[2] * diff[2];
            }
            cov00 /= tNumIndices;
            cov01 /= tNumIndices;
            cov02 /= tNumIndices;
            cov11 /= tNumIndices;
            cov12 /= tNumIndices;
            cov22 /= tNumIndices;

            // Solve the eigensystem.
            SymmetricEigensolver<T, 3> solver{};
            std::size_t numIterations = solver(cov00, cov01, cov02, cov11, cov12, cov22,
                false, false);

            // Copy the eigenstuff.
            eigenvalues = solver.GetEigenvalues();
            for (std::size_t i = 0; i < 3; ++i)
            {
                eigenvectors[i] = solver.GetEigenvector(i);
            }
            return numIterations;
        }

    private:
        friend class UnitTestApprGaussianDistribution;
    };

    template <typename T>
    class ApprGaussianDistribution<T>
    {
    public:
        // Fit all input points[].
        static std::size_t Fit(std::vector<Vector<T>> const& points, std::size_t maxIterations,
            Vector<T>& mean, std::vector<T>& eigenvalues,
            std::vector<Vector<T>>& eigenvectors)
        {
            std::vector<std::size_t> indices(points.size());
            std::iota(indices.begin(), indices.end(), 0);
            return Fit(points, indices, maxIterations, false,
                mean, eigenvalues, eigenvectors);
        }

        // Fit some input points[]. The points[] are a vertex pool. The points
        // used for fitting are referenced by indices[]. Not all elements of
        // points[] must be used. If you are absolutely certain that the
        // indices satisfy 0 <= indices[i] < points.size() for all
        // 0 <= i < indices.size(), set validateIndices to false. Otherwise,
        // set validateIndices to true and allow the function to do the
        // validation.
        static std::size_t Fit(std::vector<Vector<T>> const& points,
            std::vector<std::size_t> const& indices, std::size_t maxIterations, bool validateIndices,
            Vector<T>& mean, std::vector<T>& eigenvalues,
            std::vector<Vector<T>>& eigenvectors)
        {
            GTL_ARGUMENT_ASSERT(
                points.size() >= 2 && indices.size() >= 2 && maxIterations > 0,
                "Invalid number of points or indices or maxIterations.");

            std::size_t const dimension = points[0].size();
            GTL_ARGUMENT_ASSERT(
                dimension >= 2,
                "Invalid dimension.");

            for (auto const& point : points)
            {
                GTL_ARGUMENT_ASSERT(
                    point.size() == dimension,
                    "Invalid dimension.");
            }

            if (validateIndices)
            {
                for (std::size_t i = 0; i < indices.size(); ++i)
                {
                    if (indices[i] >= points.size())
                    {
                        GTL_ARGUMENT_ERROR(
                            "Invalid index indices[" + std::to_string(i) +
                            "] = " + std::to_string(indices[i]));
                    }
                }
            }

            T const tNumIndices = static_cast<T>(indices.size());
            mean.resize(dimension);
            MakeZero(mean);
            for (auto const& index : indices)
            {
                mean += points[index];
            }
            mean /= tNumIndices;

            // Compute the covariance matrix of the points.
            Matrix<T> covariance(dimension, dimension);
            for (auto const& index : indices)
            {
                Vector<T> diff = points[index] - mean;
                for (std::size_t row = 0; row < dimension; ++row)
                {
                    for (std::size_t col = row; col < dimension; ++col)
                    {
                        covariance(row, col) += diff[row] * diff[col];
                    }
                }
            }

            for (std::size_t row = 0; row < dimension; ++row)
            {
                std::size_t col;
                for (col = 0; col < row; ++col)
                {
                    covariance(row, col) = covariance(col, row);
                }
                for (; col < dimension; ++col)
                {
                    covariance(row, col) /= tNumIndices;
                }
            }

            // Solve the eigensystem.
            SymmetricEigensolver<T> solver{};
            std::size_t numIterations = solver(dimension, covariance.data(), maxIterations);

            // Copy the eigenstuff.
            eigenvalues = solver.GetEigenvalues();
            eigenvectors.resize(dimension);
            for (std::size_t i = 0; i < dimension; ++i)
            {
                eigenvectors[i] = solver.GetEigenvector(i);
            }
            return numIterations;
        }

    private:
        friend class UnitTestApprGaussianDistribution;
    };
}
