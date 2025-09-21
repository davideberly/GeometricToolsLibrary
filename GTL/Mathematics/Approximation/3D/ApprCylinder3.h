// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.09.21

#pragma once

// The algorithm for least-squares fitting of a point set by a cylinder is
// described in
//   https://www.geometrictools.com/Documentation/CylinderFitting.pdf
// This document shows how to compute the cylinder radius r and the cylinder
// axis as a line C + h * W with origin C, unit-length direction W, and any
// real-valued h. The implementation here adds one addition step. It
// projects the point set onto the cylinder axis, computes the bounding
// h-interval [hmin, hmax] for the projections and sets the cylinder center
// to C + ((hmin + hmax) / 2) * W and the cylinder height to hmax - hmin.

#include <GTL/Mathematics/Algebra/Matrix.h>
#include <GTL/Mathematics/Approximation/2D/ApprCircle2.h>
#include <GTL/Mathematics/MatrixAnalysis/SymmetricEigensolver.h>
#include <GTL/Mathematics/Primitives/ND/Cylinder.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>
#include <thread>
#include <vector>

namespace gtl
{
    template <typename T>
    class ApprCylinder3
    {
    public:
        // Search the hemisphere for a minimum, choose numThetaSamples and
        // numPhiSamples to be positive (and preferably large). These are
        // used to generate a hemispherical grid of samples to be evaluated
        // to find the cylinder axis-direction W. If the grid samples is
        // quite large and the number of points to be fitted is large, you
        // most likely will want to run multithreaded. Set numThreads to 0
        // to run single-threaded in the main process. Set numThreads > 0 to
        // run multithreaded.
        static void FitUsingHemisphereSearch(std::size_t numThreads, std::size_t numPoints,
            Vector3<T> const* points, std::size_t numThetaSamples, std::size_t numPhiSamples,
            Cylinder3<T>& cylinder)
        {
            GTL_ARGUMENT_ASSERT(
                numPoints >= 6 && points != nullptr,
                "Fitting requires at least 6 points.");

            GTL_ARGUMENT_ASSERT(
                numThetaSamples > 0 && numPhiSamples > 0,
                "The number of theta and psi samples must be positive.");

            // Translate the points by their average for numerical robustness.
            // Precompute parameters that depend only on the sample points.
            // This allows precomputed summations so that G(...) can be
            // evaluated efficiently.
            std::vector<Vector3<T>> localPoints{};
            Vector3<T> average{}, minW{}, minPC{};
            Parameters parameters{};
            T minRSqr = C_<T>(0);
            Preamble(numPoints, points, localPoints, average, parameters);

            // Search the hemisphere for the vector that leads to minimum
            // error and use it for the cylinder axis.
            if (numThreads <= 1)
            {
                // Execute the algorithm in the main process.
                ComputeSingleThreaded(numPoints, numThetaSamples,
                    numPhiSamples, parameters, minW, minPC, minRSqr);
            }
            else
            {
                // Execute the algorithm in multiple threads.
                ComputeMultiThreaded(numThreads, numPoints, numThetaSamples,
                    numPhiSamples, parameters, minW, minPC, minRSqr);
            }

            CreateCylinder(localPoints, average, minW, minPC, minRSqr, cylinder);
        }

        // Choose one of the eigenvectors for the covariance matrix as the
        // cylinder axis direction. If eigenIndex is 0, the eigenvector
        // associated with the smallest eigenvalue is chosen. If eigenIndex
        // is 2, the eigenvector associated with the largest eigenvalue is
        // chosen. If eigenIndex is 1, the eigenvector associated with the
        // median eigenvalue is chosen; keep in mind that this could be the
        // minimum or maximum eigenvalue if the eigenspace has dimension 2
        // or 3.
        static void FitUsingEigendirection(std::size_t numPoints, Vector3<T> const* points,
            std::size_t eigenIndex, Cylinder3<T>& cylinder)
        {
            GTL_ARGUMENT_ASSERT(
                numPoints >= 6 && points != nullptr,
                "Fitting requires at least 6 points.");

            GTL_ARGUMENT_ASSERT(
                eigenIndex < 3,
                "Eigenvector index is out of range.");

            // Translate the points by their average for numerical robustness.
            // Precompute parameters that depend only on the sample points.
            // This allows precomputed summations so that G(...) can be
            // evaluated efficiently.
            std::vector<Vector3<T>> localPoints{};
            Vector3<T> average{}, minW{}, minPC{};
            Parameters parameters{};
            T minRSqr = C_<T>(0);
            Preamble(numPoints, points, localPoints, average, parameters);

            // Use the eigenvector corresponding to the mEigenIndex of the
            // eigenvectors of the covariance matrix as the cylinder axis
            // direction. The eigenvectors are sorted from smallest
            // eigenvalue (mEigenIndex = 0) to largest eigenvalue
            // (mEigenIndex = 2).
            Matrix3x3<T> covar{};  // zero matrix
            for (auto const& point : localPoints)
            {
                covar += OuterProduct(point, point);
            }
            covar /= static_cast<T>(localPoints.size());
            SymmetricEigensolver<T, 3> solver{};
            solver(covar(0, 0), covar(0, 1), covar(0, 2), covar(1, 1), covar(1, 2),
                covar(2, 2), true, false);
            minW = solver.GetEigenvector(eigenIndex);
            (void)G(numPoints, parameters, minW, minPC, minRSqr);

            CreateCylinder(localPoints, average, minW, minPC, minRSqr, cylinder);
        }

        // Choose the cylinder axis direction. If direction is not the zero
        // vector, it will be normalized internally.
        static void FitUsingDirection(std::size_t numPoints, Vector3<T> const* points,
            Vector3<T> const& direction, Cylinder3<T>& cylinder)
        {
            GTL_ARGUMENT_ASSERT(
                numPoints >= 6 && points != nullptr,
                "Fitting requires at least 6 points.");

            Vector3<T> minW = direction;
            T length = Normalize(minW);
            GTL_ARGUMENT_ASSERT(
                length > C_<T>(0),
                "The direction vector must be nonzero.");

            // Translate the points by their average for numerical robustness.
            // Precompute parameters that depend only on the sample points.
            // This allows precomputed summations so that G(...) can be
            // evaluated efficiently.
            std::vector<Vector3<T>> localPoints{};
            Vector3<T> average{}, minPC{};
            Parameters parameters{};
            T minRSqr = C_<T>(0);
            Preamble(numPoints, points, localPoints, average, parameters);

            (void)G(numPoints, parameters, minW, minPC, minRSqr);

            CreateCylinder(localPoints, average, minW, minPC, minRSqr, cylinder);
        }

        // Use this operator for fitting a cylinder to a mesh. For each
        // candidate cone axis direction on the hemisphere, project the
        // triangles onto a plane perpendicular to the axis. Compute the
        // sum of 2*area for the projected triangles. The direction that
        // minimizes this measurement is used as the cone axis direction.
        // The projected points for this direction are fit with a circle
        // whose center is used to generate the cylinder center and whose
        // radius is used for the cylinder radius. The projection of the
        // points onto the axis are used to determine the minimum and
        // maximum coordinates along the axis, which are then used to
        // compute the cone height. The cone center is adjusted to be
        // midway between the minimum and maximum coordinates along the axis.
        template <typename IndexType>
        static void FitUsingHemisphereSearch(std::size_t numThreads, std::size_t numPoints,
            Vector3<T> const* points, std::size_t numTriangles, IndexType const* triangles,
            std::size_t numThetaSamples, std::size_t numPhiSamples, Cylinder3<T>& cylinder)
        {
            GTL_ARGUMENT_ASSERT(
                numPoints >= 6 && points != nullptr,
                "Fitting requires at least 6 points.");

            GTL_ARGUMENT_ASSERT(
                numTriangles >= 2 && triangles != nullptr,
                "Fitting requires at least 6 points and 2 triangles.");

            // Compute the average and translate the points so that the
            // origin is the average. This is for numerical robustness of
            // the fitting algorithm.
            Vector3<T> average{};
            std::vector<Vector3<T>> localPoints(numPoints);
            for (std::size_t i = 0; i < numPoints; ++i)
            {
                localPoints[i] = points[i];
                average += points[i];
            }
            average /= static_cast<T>(numPoints);
            for (auto& point : localPoints)
            {
                point -= average;
            }

            // Convert the triangle indices to std::size_t to allow for a
            // nontemplated implementation of the ComputeMesh* functions.
            std::vector<std::array<std::size_t, 3>> localTriangles(numTriangles);
            for (std::size_t i = 0; i < numTriangles; ++i)
            {
                auto& tri = localTriangles[i];
                tri[0] = static_cast<std::size_t>(*triangles++);
                tri[1] = static_cast<std::size_t>(*triangles++);
                tri[2] = static_cast<std::size_t>(*triangles++);
            }

            // Search the hemisphere for the vector that leads to minimum
            // error and use it for the cylinder axis.
            if (numThreads == 0)
            {
                // Execute the algorithm in the main process.
                ComputeMeshSingleThreaded(localPoints, localTriangles,
                    numThetaSamples, numPhiSamples, cylinder);
            }
            else
            {
                // Execute the algorithm in multiple threads.
                ComputeMeshMultiThreaded(numThreads, localPoints, localTriangles,
                    numThetaSamples, numPhiSamples, cylinder);
            }

            // Translate to origin coordinate system.
            cylinder.center += average;
        }

    private:
        struct Parameters
        {
            Parameters()
                :
                mu{},
                F0{},
                F1{},
                F2{}
            {
            }

            Vector<T, 6> mu;
            Matrix3x3<T> F0;
            Matrix<T, 3, 6> F1;
            Matrix<T, 6, 6> F2;
        };

        // Member functions to support cylinder fitting to a set of points.
        static void Preamble(std::size_t numPoints, Vector3<T> const* points,
            std::vector<Vector3<T>>& localPoints, Vector3<T>& average,
            Parameters& parameters)
        {
            // Compute the average and translate the points so that the
            // origin is the average. This is for numerical robustness of
            // the fitting algorithm. The incoming average is guaranteed
            // to be the zero vector.
            localPoints.resize(numPoints);
            for (std::size_t i = 0; i < numPoints; ++i)
            {
                localPoints[i] = points[i];
                average += points[i];
            }
            average /= static_cast<T>(numPoints);
            for (auto& point : localPoints)
            {
                point -= average;
            }

            // Compute parameters that depend only on the sample points. This
            // allows precomputed summations so that G(...) can be evaluated
            // efficiently.
            // On input, the quantities mu, F0, F1 and F2 are zero.
            std::vector<Vector<T, 6>> products(numPoints);
            for (std::size_t i = 0; i < numPoints; ++i)
            {
                auto const& point = localPoints[i];
                products[i][0] = point[0] * point[0];
                products[i][1] = point[0] * point[1];
                products[i][2] = point[0] * point[2];
                products[i][3] = point[1] * point[1];
                products[i][4] = point[1] * point[2];
                products[i][5] = point[2] * point[2];
                parameters.mu[0] += products[i][0];
                parameters.mu[1] += C_<T>(2) * products[i][1];
                parameters.mu[2] += C_<T>(2) * products[i][2];
                parameters.mu[3] += products[i][3];
                parameters.mu[4] += C_<T>(2) * products[i][4];
                parameters.mu[5] += products[i][5];
            }
            parameters.mu /= static_cast<T>(numPoints);

            for (std::size_t i = 0; i < numPoints; ++i)
            {
                Vector<T, 6> delta
                {
                    products[i][0] - parameters.mu[0],
                    C_<T>(2) * products[i][1] - parameters.mu[1],
                    C_<T>(2) * products[i][2] - parameters.mu[2],
                    products[i][3] - parameters.mu[3],
                    C_<T>(2) * products[i][4] - parameters.mu[4],
                    products[i][5] - parameters.mu[5]
                };

                parameters.F0(0, 0) += products[i][0];
                parameters.F0(0, 1) += products[i][1];
                parameters.F0(0, 2) += products[i][2];
                parameters.F0(1, 1) += products[i][3];
                parameters.F0(1, 2) += products[i][4];
                parameters.F0(2, 2) += products[i][5];
                parameters.F1 += OuterProduct(localPoints[i], delta);
                parameters.F2 += OuterProduct(delta, delta);
            }
            parameters.F0 /= static_cast<T>(numPoints);
            parameters.F1 /= static_cast<T>(numPoints);
            parameters.F2 /= static_cast<T>(numPoints);
            parameters.F0(1, 0) = parameters.F0(0, 1);
            parameters.F0(2, 0) = parameters.F0(0, 2);
            parameters.F0(2, 1) = parameters.F0(1, 2);
        }

        static void ComputeSingleThreaded(std::size_t numPoints, std::size_t numThetaSamples,
            std::size_t numPhiSamples, Parameters const& parameters,
            Vector3<T>& minW, Vector3<T>& minPC, T& minRSqr)
        {
            T const iMultiplier = C_TWO_PI<T> / static_cast<T>(numThetaSamples);
            T const jMultiplier = C_PI_DIV_2<T> / static_cast<T>(numPhiSamples);

            // Handle the north pole (0,0,1) separately.
            minW = { C_<T>(0), C_<T>(0), C_<T>(1) };
            T minError = G(numPoints, parameters, minW, minPC, minRSqr);

            for (std::size_t j = 1; j <= numPhiSamples; ++j)
            {
                T phi = jMultiplier * static_cast<T>(j);  // in [0,pi/2]
                T csphi = std::cos(phi);
                T snphi = std::sin(phi);
                for (std::size_t i = 0; i < numThetaSamples; ++i)
                {
                    T theta = iMultiplier * static_cast<T>(i);  // in [0,2*pi)
                    T cstheta = std::cos(theta);
                    T sntheta = std::sin(theta);
                    Vector3<T> W{ cstheta * snphi, sntheta * snphi, csphi };
                    Vector3<T> PC{};
                    T rsqr = C_<T>(0);
                    T error = G(numPoints, parameters, W, PC, rsqr);
                    if (error < minError)
                    {
                        minError = error;
                        minRSqr = rsqr;
                        minW = W;
                        minPC = PC;
                    }
                }
            }
        }

        static void ComputeMultiThreaded(std::size_t numThreads, std::size_t numPoints,
            std::size_t numThetaSamples, std::size_t numPhiSamples, Parameters const& parameters,
            Vector3<T>& minW, Vector3<T>& minPC, T& minRSqr)
        {
            T const iMultiplier = C_TWO_PI<T> / static_cast<T>(numThetaSamples);
            T const jMultiplier = C_PI_DIV_2<T> / static_cast<T>(numPhiSamples);

            // Handle the north pole (0,0,1) separately.
            minW = { C_<T>(0), C_<T>(0), C_<T>(1) };
            T minError = G(numPoints, parameters, minW, minPC, minRSqr);

            struct Local
            {
                Local()
                    :
                    error(std::numeric_limits<T>::max()),
                    rsqr(C_<T>(0)),
                    W{},
                    PC{},
                    jmin(0),
                    jmax(0)
                {
                }

                T error;
                T rsqr;
                Vector3<T> W;
                Vector3<T> PC;
                std::size_t jmin;
                std::size_t jmax;
            };

            std::vector<Local> local(numThreads);
            std::size_t numPhiSamplesPerThread = numPhiSamples / numThreads;
            for (std::size_t t = 0; t < numThreads; ++t)
            {
                local[t].jmin = numPhiSamplesPerThread * t;
                local[t].jmax = numPhiSamplesPerThread * (t + 1);
            }
            local[numThreads - 1].jmax = numPhiSamples + 1;

            std::vector<std::thread> process(numThreads);
            for (std::size_t t = 0; t < numThreads; ++t)
            {
                process[t] = std::thread
                (
                    [t, numPoints, numThetaSamples, iMultiplier, jMultiplier,
                        &parameters, &local]()
                    {
                        for (std::size_t j = local[t].jmin; j < local[t].jmax; ++j)
                        {
                            // phi in [0,pi/2]
                            T phi = jMultiplier * static_cast<T>(j);
                            T csphi = std::cos(phi);
                            T snphi = std::sin(phi);
                            for (std::size_t i = 0; i < numThetaSamples; ++i)
                            {
                                // theta in [0,2*pi)
                                T theta = iMultiplier * static_cast<T>(i);
                                T cstheta = std::cos(theta);
                                T sntheta = std::sin(theta);
                                Vector3<T> W{ cstheta * snphi, sntheta * snphi, csphi };
                                Vector3<T> PC{};
                                T rsqr = C_<T>(0);
                                T error = ApprCylinder3<T>::G(numPoints, parameters, W, PC, rsqr);
                                if (error < local[t].error)
                                {
                                    local[t].error = error;
                                    local[t].rsqr = rsqr;
                                    local[t].W = W;
                                    local[t].PC = PC;
                                }
                            }
                        }
                    }
                );
            }

            for (std::size_t t = 0; t < numThreads; ++t)
            {
                process[t].join();

                if (local[t].error < minError)
                {
                    minError = local[t].error;
                    minRSqr = local[t].rsqr;
                    minW = local[t].W;
                    minPC = local[t].PC;
                }
            }
        }

        static T G(std::size_t numPoints, Parameters const& parameters, Vector3<T> const& W,
            Vector3<T>& PC, T& rsqr)
        {
            Matrix3x3<T> identity
            {
                { C_<T>(1), C_<T>(0), C_<T>(0) },
                { C_<T>(0), C_<T>(1), C_<T>(0) },
                { C_<T>(0), C_<T>(0), C_<T>(1) }
            };

            Matrix3x3<T> P = identity - OuterProduct(W, W);
            Matrix3x3<T> S{};  // zero matrix
            S(2, 1) = W[0];
            S(0, 2) = W[1];
            S(1, 0) = W[2];
            S(1, 2) = -S(2, 1);
            S(2, 0) = -S(0, 2);
            S(0, 1) = -S(1, 0);

            Matrix3x3<T> A = P * parameters.F0 * P;
            Matrix3x3<T> hatA = -(S * A * S);
            Matrix3x3<T> hatAA = hatA * A;
            T trace = Trace(hatAA);
            Matrix3x3<T> Q = hatA / trace;
            Vector<T, 6> pVec{ P(0, 0), P(0, 1), P(0, 2), P(1, 1), P(1, 2), P(2, 2) };
            Vector3<T> alpha = parameters.F1 * pVec;
            Vector3<T> beta = Q * alpha;
            T term0 = Dot(pVec, parameters.F2 * pVec);
            T term1 = C_<T>(4) * Dot(alpha, beta);
            T term2 = C_<T>(4) * Dot(beta, parameters.F0 * beta);
            T gValue = (term0 - term1 + term2) / static_cast<T>(numPoints);

            PC = beta;
            rsqr = Dot(pVec, parameters.mu) + Dot(PC, PC);
            return gValue;
        }

        static void CreateCylinder(std::vector<Vector3<T>> const& localPoints,
            Vector3<T> const& average, Vector3<T> const& minW, Vector3<T> const& minPC,
            T const& minRSqr, Cylinder3<T>& cylinder)
        {
            // Translate back to the original space by the average of the
            // points.
            cylinder.center = minPC;
            cylinder.direction = minW;

            // Compute the cylinder radius.
            cylinder.radius = std::sqrt(minRSqr);

            // Project the points onto the cylinder axis and choose the
            // cylinder center and cylinder height as described in the
            // comments at the top of this header file.
            T hmin = C_<T>(0);
            T hmax = C_<T>(0);
            for (auto const& point : localPoints)
            {
                T h = Dot(cylinder.direction, point - cylinder.center);
                hmin = std::min(h, hmin);
                hmax = std::max(h, hmax);
            }

            T hmid = C_<T>(1, 2) * (hmin + hmax);
            cylinder.center += average + hmid * cylinder.direction;
            cylinder.height = hmax - hmin;
        }

        // Member functions to support cylinder fitting to a triangle mesh.
        static void ComputeMeshSingleThreaded(std::vector<Vector3<T>> const& localPoints,
            std::vector<std::array<std::size_t, 3>> const& localTriangles,
            std::size_t numThetaSamples, std::size_t numPhiSamples, Cylinder3<T>& cylinder)
        {
            T const iMultiplier = C_TWO_PI<T> / static_cast<T>(numThetaSamples);
            T const jMultiplier = C_PI_DIV_2<T> / static_cast<T>(numPhiSamples);

            // Handle the north pole (0,0,1) separately.
            Vector3<T> minDirection{ C_<T>(0), C_<T>(0), C_<T>(1) };
            T minMeasure = GetProjectionMeasure(minDirection, localPoints, localTriangles);

            // Process a regular grid of (theta,phi) angles.
            for (std::size_t j = 1; j <= numPhiSamples; ++j)
            {
                T phi = jMultiplier * static_cast<T>(j);  // in [0,pi/2]
                T csphi = std::cos(phi);
                T snphi = std::sin(phi);
                for (std::size_t i = 0; i < numThetaSamples; ++i)
                {
                    T theta = iMultiplier * static_cast<T>(i);  // in [0,2*pi)
                    T cstheta = std::cos(theta);
                    T sntheta = std::sin(theta);
                    Vector3<T> direction{ cstheta * snphi, sntheta * snphi, csphi };
                    T measure = GetProjectionMeasure(direction, localPoints, localTriangles);
                    if (measure < minMeasure)
                    {
                        minDirection = direction;
                        minMeasure = measure;
                    }
                }
            }

            CreateCylinder(minDirection, localPoints, cylinder);
        }

        static void ComputeMeshMultiThreaded(std::size_t numThreads,
            std::vector<Vector3<T>> const& localPoints,
            std::vector<std::array<std::size_t, 3>> const& localTriangles,
            std::size_t numThetaSamples, std::size_t numPhiSamples, Cylinder3<T>& cylinder)
        {
            T const iMultiplier = C_TWO_PI<T> / static_cast<T>(numThetaSamples);
            T const jMultiplier = C_PI_DIV_2<T> / static_cast<T>(numPhiSamples);

            // Handle the north pole (0,0,1) separately.
            Vector3<T> minDirection{ C_<T>(0), C_<T>(0), C_<T>(1) };
            T minMeasure = GetProjectionMeasure(minDirection, localPoints, localTriangles);

            struct Local
            {
                Local()
                    :
                    direction{ C_<T>(0), C_<T>(0), C_<T>(0) },
                    measure{ std::numeric_limits<T>::max() },
                    jmin(0),
                    jmax(0)
                {
                }

                Vector3<T> direction;
                T measure;
                std::size_t jmin;
                std::size_t jmax;
            };

            std::vector<Local> local(numThreads);
            std::size_t numPhiSamplesPerThread = numPhiSamples / numThreads;
            for (std::size_t t = 0; t < numThreads; ++t)
            {
                local[t].jmin = numPhiSamplesPerThread * t;
                local[t].jmax = numPhiSamplesPerThread * (t + 1);
            }
            local[numThreads - 1].jmax = numPhiSamples + 1;

            std::vector<std::thread> process(numThreads);
            for (std::size_t t = 0; t < numThreads; ++t)
            {
                process[t] = std::thread
                (
                    [t, iMultiplier, jMultiplier, numThetaSamples,
                        &local, &localPoints, &localTriangles]()
                    {
                        for (std::size_t j = local[t].jmin; j < local[t].jmax; ++j)
                        {
                            // phi in [0,pi/2]
                            T phi = jMultiplier * static_cast<T>(j);
                            T csphi = std::cos(phi);
                            T snphi = std::sin(phi);
                            for (std::size_t i = 0; i < numThetaSamples; ++i)
                            {
                                // theta in [0,2*pi)
                                T theta = iMultiplier * static_cast<T>(i);
                                T cstheta = std::cos(theta);
                                T sntheta = std::sin(theta);
                                Vector3<T> direction{ cstheta * snphi, sntheta * snphi, csphi };
                                T measure = GetProjectionMeasure(direction,
                                    localPoints, localTriangles);
                                if (measure < local[t].measure)
                                {
                                    local[t].direction = direction;
                                    local[t].measure = measure;
                                }
                            }
                        }
                    }
                );
            }

            for (std::size_t t = 0; t < numThreads; ++t)
            {
                process[t].join();

                if (local[t].measure < minMeasure)
                {
                    minMeasure = local[t].measure;
                    minDirection = local[t].direction;
                }
            }

            CreateCylinder(minDirection, localPoints, cylinder);
        }

        static T GetProjectionMeasure(Vector3<T> const& direction,
            std::vector<Vector3<T>> const& localPoints,
            std::vector<std::array<std::size_t, 3>> const& localTriangles)
        {
            Vector3<T> D = direction, U{}, V{};
            ComputeOrthonormalBasis(1, D, U, V);
            std::vector<Vector2<T>> projections(localPoints.size());
            for (std::size_t i = 0; i < localPoints.size(); ++i)
            {
                projections[i][0] = Dot(U, localPoints[i]);
                projections[i][1] = Dot(V, localPoints[i]);
            }

            // Add up 2*area of the triangles.
            T measure = C_<T>(0);
            for (auto const& tri : localTriangles)
            {
                auto const& V0 = projections[tri[0]];
                auto const& V1 = projections[tri[1]];
                auto const& V2 = projections[tri[2]];
                auto edge10 = V1 - V0;
                auto edge20 = V2 - V0;
                measure += std::fabs(DotPerp(edge10, edge20));
            }
            return measure;
        }

        static void CreateCylinder(Vector3<T> const& minDirection,
            std::vector<Vector3<T>> const& localPoints, Cylinder3<T>& cylinder)
        {
            Vector3<T> D = minDirection, U{}, V{};
            ComputeOrthonormalBasis(1, D, U, V);
            std::vector<Vector2<T>> projections(localPoints.size());
            T hmin = std::numeric_limits<T>::max();
            T hmax = C_<T>(0);
            for (std::size_t i = 0; i < localPoints.size(); ++i)
            {
                T h = Dot(D, localPoints[i]);
                if (h < hmin)
                {
                    hmin = h;
                }
                if (h > hmax)
                {
                    hmax = h;
                }

                projections[i][0] = Dot(U, localPoints[i]);
                projections[i][1] = Dot(V, localPoints[i]);
            }
            ApprCircle2<T> fitter{};
            Circle2<T> circle{};
            fitter.FitUsingSquaredLengths(projections, circle);

            Vector3<T> minCenter = circle.center[0] * U + circle.center[1] * V;
            cylinder.center = minCenter + (C_<T>(1, 2) * (hmax + hmin)) * D;
            cylinder.direction = D;
            cylinder.radius = circle.radius;
            cylinder.height = hmax - hmin;
        }

    private:
        friend class UnitTestApprCylinder3;
    };
}
