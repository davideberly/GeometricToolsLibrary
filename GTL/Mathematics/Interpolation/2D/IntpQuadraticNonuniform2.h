// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// Quadratic interpolation of a mesh of triangles whose vertices are of
// the form (x,y,f(x,y)). Such a mesh is obtained by Delaunay
// triangulation. The domain samples are (x[i],y[i]), where i is the
// index of the planar mesh vertices. The function samples are F[i],
// which represent f(x[i],y[i]). This code is an implementation of the
// algorithm found in
//   Zoltan J. Cendes and Steven H. Wong,
//   C1 quadratic interpolation over arbitrary point sets,
//   IEEE Computer Graphics & Applications,
//   pp. 8-16, 1987
// A detailed description and some alternative algorithms are in
//   https://www.geometrictools.com/Documentation/C1QQuadraticInterpolation.pdf

#include <GTL/Mathematics/Meshes/PlanarMesh.h>
#include <GTL/Mathematics/Containment/2D/ContScribeCircle2.h>
#include <GTL/Mathematics/Distance/ND/DistPointAlignedBox.h>
#include <GTL/Mathematics/Intersection/2D/IntrSegment2Segment2.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <vector>

namespace gtl
{
    template <typename T>
    class IntpQuadraticNonuniform2
    {
    public:
        // For both constructors, if 'meshIsConvex' is set to 'true', the
        // 'numThreads' parameter is ignored because the interpolator does an
        // efficient linear walk through the planar mesh. If 'meshIsConvex' is
        // set to 'false', the interpolator uses an exhaustive search of the
        // triangles, so multithreading can improve the performance when there
        // is a large number of triangles. In this case, set 'numThreads' to a
        // positive number.

        // The function values for f(x,y) are provided as input. The partial
        // derivatives df/dx and df/dy are estimated at the sample points
        // using finite differences. The 'spatialDelta' value is specific to
        // the application and measures the difference between consecutive
        // samples in each coordinate direction.
        IntpQuadraticNonuniform2(PlanarMesh<T> const& mesh,
            std::vector<T> const& F, T const& spatialDelta,
            bool meshIsConvex, std::size_t numThreads)
            :
            mMesh(mesh),
            mF(F),
            mDFDX(F.size(), C_<T>(0)),
            mDFDY(F.size(), C_<T>(0)),
            mTriangleData(mesh.GetTriangles().size()),
            mMeshIsConvex(meshIsConvex),
            mNumThreads(numThreads),
            mLastVisited(PlanarMesh<T>::invalid)
        {
            static_assert(
                std::is_floating_point<T>::value,
                "The input type must be a floating-point type.");

            EstimateDerivatives(spatialDelta);
            ProcessTriangles();
        }

        // The function values for f(x,y), df(x,y)/dx, and df(x,y)/dy are
        // provided as inputs.
        IntpQuadraticNonuniform2(PlanarMesh<T> const& mesh,
            std::vector<T> const& F, std::vector<T> const& DFDX, std::vector<T> const& DFDY,
            bool meshIsConvex, std::size_t numThreads)
            :
            mMesh(mesh),
            mF(F),
            mDFDX(DFDX),
            mDFDY(DFDY),
            mTriangleData(mesh.GetTriangles().size()),
            mMeshIsConvex(meshIsConvex),
            mNumThreads(numThreads),
            mLastVisited(PlanarMesh<T>::invalid)
        {
            static_assert(
                std::is_floating_point<T>::value,
                "The input type must be a floating-point type.");

            ProcessTriangles();
        }

        // Disallow copy semantics and move semantics.
        IntpQuadraticNonuniform2(IntpQuadraticNonuniform2 const&) = delete;
        IntpQuadraticNonuniform2& operator=(IntpQuadraticNonuniform2 const&) = delete;
        IntpQuadraticNonuniform2(IntpQuadraticNonuniform2&&) = delete;
        IntpQuadraticNonuniform2& operator=(IntpQuadraticNonuniform2&&) = delete;

        // The return value is 'true' if and only if the input point P is in
        // the planar mesh of the input vertices, in which case the
        // interpolation is valid.
        bool Evaluate(Vector2<T> const& P, T& F, T& DFDX, T& DFDY) const
        {
            if (mLastVisited == PlanarMesh<T>::invalid)
            {
                mLastVisited = 0;
            }

            if (mMeshIsConvex)
            {
                mLastVisited = mMesh.GetContainingTriangleConvex(P, mLastVisited);
            }
            else
            {
                mLastVisited = mMesh.GetContainingTriangleNotConvex(P, mNumThreads);
            }

            if (mLastVisited == PlanarMesh<T>::invalid)
            {
                // The point is outside the triangulation.
                return false;
            }

            // Get the vertex positions of the triangle.
            std::array<Vector2<T>, 3> V{};
            mMesh.GetPositions(mLastVisited, V);

            // Get the additional information for the triangle.
            TriangleData const& tData = mTriangleData[mLastVisited];

            // Determine which of the 6 subtriangles contains the target
            // point. Theoretically, P must be in one of these subtriangles.
            std::array<Vector2<T>, 6> boundary =
            {
                tData.intersect[1], // E20
                V[0],               // X0
                tData.intersect[2], // E01
                V[1],               // X1
                tData.intersect[0], // E12
                V[2]                // X2
            };

            Vector2<T> sub0 = tData.center, sub1{}, sub2{};
            std::array<T, 3> bary{};

            AlignedBox3<T> barybox(Vector3<T>::Zero(), Vector3<T>::One());
            DCPQuery<T, Vector3<T>, AlignedBox3<T>> pbQuery{};
            T minDistance = C_<T>(-1);
            Vector2<T> minSub0{}, minSub1{}, minSub2{};
            std::array<T, 3> minBary{};

            std::size_t minIndex = std::numeric_limits<std::size_t>::max();
            std::size_t i{}, ip1{};
            for (i = 5, ip1 = 0; ip1 < 6; i = ip1++)
            {
                sub1 = boundary[i];
                sub2 = boundary[ip1];

                bool valid = ComputeBarycentrics(P, sub0, sub1, sub2, bary);
                if (valid &&
                    C_<T>(0) <= bary[0] && bary[0] <= C_<T>(1) &&
                    C_<T>(0) <= bary[1] && bary[1] <= C_<T>(1) &&
                    C_<T>(0) <= bary[2] && bary[2] <= C_<T>(1))
                {
                    // P is in subtriangle <sub0,sub1,sub2>
                    break;
                }

                // When computing with floating-point arithmetic, rounding
                // errors can cause us to reach this code when, theoretically,
                // the point is in the subtriangle.  Keep track of the
                // (b0,b1,b2) that is closest to the barycentric cube [0,1]^3
                // and choose the triangle corresponding to it when all 6
                // tests previously fail.
                T distance = pbQuery(bary, barybox).distance;
                if (minIndex == std::numeric_limits<std::size_t>::max() ||
                    distance < minDistance)
                {
                    minDistance = distance;
                    minIndex = i + 1;
                    minBary = bary;
                    minSub0 = sub0;
                    minSub1 = sub1;
                    minSub2 = sub2;
                }
            }

            // The Cendes-Wong data coefficients use 1-based indexing.
            std::size_t index = i + 1;

            // If the subtriangle was not found, rounding errors caused
            // problems. Choose the barycentric point closest to the box.
            if (index > 6)
            {
                index = minIndex;
                bary = minBary;
                sub0 = minSub0;
                sub1 = minSub1;
                sub2 = minSub2;
            }

            // Fetch Bezier control points.
            std::array<T, 6> bez =
            {
                tData.coeff[0],
                tData.coeff[12 + index],
                tData.coeff[13 + (index % 6)],
                tData.coeff[index],
                tData.coeff[6 + index],
                tData.coeff[1 + (index % 6)]
            };

            // Evaluate Bezier quadratic.
            F = bary[0] * (bez[0] * bary[0] + bez[1] * bary[1] + bez[2] * bary[2]) +
                bary[1] * (bez[1] * bary[0] + bez[3] * bary[1] + bez[4] * bary[2]) +
                bary[2] * (bez[2] * bary[0] + bez[4] * bary[1] + bez[5] * bary[2]);

            // Evaluate barycentric derivatives of F.
            T FU = C_<T>(2) * (bez[0] * bary[0] + bez[1] * bary[1] + bez[2] * bary[2]);
            T FV = C_<T>(2) * (bez[1] * bary[0] + bez[3] * bary[1] + bez[4] * bary[2]);
            T FW = C_<T>(2) * (bez[2] * bary[0] + bez[4] * bary[1] + bez[5] * bary[2]);
            T duw = FU - FW;
            T dvw = FV - FW;

            // Convert back to (x,y) coordinates.
            T m00 = sub0[0] - sub2[0];
            T m10 = sub0[1] - sub2[1];
            T m01 = sub1[0] - sub2[0];
            T m11 = sub1[1] - sub2[1];
            T inv = C_<T>(1) / (m00 * m11 - m10 * m01);

            DFDX = inv * (m11 * duw - m10 * dvw);
            DFDY = inv * (m00 * dvw - m01 * duw);
            return true;
        }

    private:
        void EstimateDerivatives(T const& spatialDelta)
        {
            auto const& positions = mMesh.GetPositions();
            auto const& triangles = mMesh.GetTriangles();
            std::vector<T> DFDZ(positions.size(), C_<T>(0));

            // Accumulate normals at spatial locations (averaging process).
            for (auto const& triangle : triangles)
            {
                std::size_t i0 = triangle[0];
                std::size_t i1 = triangle[1];
                std::size_t i2 = triangle[2];

                // Compute normal vector of triangle (with positive
                // z-component).
                T dx1 = positions[i1][0] - positions[i0][0];
                T dy1 = positions[i1][1] - positions[i0][1];
                T dz1 = mF[i1] - mF[i0];
                T dx2 = positions[i2][0] - positions[i0][0];
                T dy2 = positions[i2][1] - positions[i0][1];
                T dz2 = mF[i2] - mF[i0];
                T nx = dy1 * dz2 - dy2 * dz1;
                T ny = dz1 * dx2 - dz2 * dx1;
                T nz = dx1 * dy2 - dx2 * dy1;
                if (nz < C_<T>(0))
                {
                    nx = -nx;
                    ny = -ny;
                    nz = -nz;
                }

                mDFDX[i0] += nx;  mDFDY[i0] += ny;  DFDZ[i0] += nz;
                mDFDX[i1] += nx;  mDFDY[i1] += ny;  DFDZ[i1] += nz;
                mDFDX[i2] += nx;  mDFDY[i2] += ny;  DFDZ[i2] += nz;
            }

            // Scale the normals to the form (x,y,-1).
            for (std::size_t i = 0; i < mF.size(); ++i)
            {
                if (DFDZ[i] != C_<T>(0))
                {
                    T inv = -spatialDelta / DFDZ[i];
                    mDFDX[i] *= inv;
                    mDFDY[i] *= inv;
                }
                else
                {
                    mDFDX[i] = C_<T>(0);
                    mDFDY[i] = C_<T>(0);
                }
            }
        }

        void ProcessTriangles()
        {
            // Add degenerate triangles to boundary triangles so that
            // interpolation at the boundary can be treated in the same way
            // as interpolation in the interior.
            auto const& positions = mMesh.GetPositions();
            auto const& triangles = mMesh.GetTriangles();

            // Compute centers of inscribed circles for triangles.
            for (std::size_t t = 0; t < triangles.size(); ++t)
            {
                auto const& triangle = triangles[t];
                std::size_t i0 = triangle[0];
                std::size_t i1 = triangle[1];
                std::size_t i2 = triangle[2];

                Circle2<T> circle{};
                Inscribe(positions[i0], positions[i1], positions[i2], circle);
                mTriangleData[t].center = circle.center;
            }

            // Compute cross-edge intersections.
            for (std::size_t t = 0; t < triangles.size(); ++t)
            {
                ComputeCrossEdgeIntersections(t);
            }

            // Compute Bezier coefficients.
            for (std::size_t t = 0; t < triangles.size(); ++t)
            {
                ComputeCoefficients(t);
            }
        }

        void ComputeCrossEdgeIntersections(std::size_t t)
        {
            // Get the vertex positions of the triangle.
            std::array<Vector2<T>, 3> V{};
            mMesh.GetPositions(t, V);

            // Get the centers of adjacent triangles.
            FIQuery<T, Segment2<T>, Segment2<T>> query{};
            TriangleData& tData = mTriangleData[t];
            std::array<std::size_t, 3> adjacents{};
            mMesh.GetAdjacents(t, adjacents);
            for (std::size_t j0 = 0; j0 < 3; ++j0)
            {
                std::size_t j1 = (j0 + 1) % 3;
                std::size_t j2 = (j0 + 2) % 3;
                std::size_t a = adjacents[j0];
                if (a != PlanarMesh<T>::invalid)
                {
                    // Compute the intersection of the segment connecting
                    // the inscribed centers of adjacent triangles and the
                    // shared edge of those triangles.
                    Segment2<T> segment0(tData.center, mTriangleData[a].center);
                    Segment2<T> segment1(V[j1], V[j2]);
                    auto result = query(segment0, segment1);
                    GTL_RUNTIME_ASSERT(
                        result.numIntersections == 1,
                        "Invalid number of intersections.");
                    tData.intersect[j0] = result.point[0];
                }
                else
                {
                    // No adjacent triangle, use center of edge.
                    tData.intersect[j0] = static_cast<T>(0.5) * (V[j1] + V[j2]);
                }
            }
        }

        void ComputeCoefficients(std::size_t t)
        {
            // Get the vertex positions of the triangle.
            std::array<Vector2<T>, 3> V{};
            mMesh.GetPositions(t, V);

            // Get the additional information for the triangle.
            TriangleData& tData = mTriangleData[t];

            // Get the sample data at main triangle vertices.
            std::array<std::size_t, 3> indices{};
            mMesh.GetIndices(t, indices);
            std::array<Jet, 3> jet{};
            for (std::size_t j = 0; j < 3; ++j)
            {
                std::size_t k = indices[j];
                jet[j].F = mF[k];
                jet[j].DFDX = mDFDX[k];
                jet[j].DFDY = mDFDY[k];
            }

            // Get centers of adjacent triangles.
            std::array<std::size_t, 3> adjacents{};
            mMesh.GetAdjacents(t, adjacents);
            std::array<Vector2<T>, 3> U{};
            for (std::size_t j0 = 0; j0 < 3; ++j0)
            {
                std::size_t j1 = (j0 + 1) % 3;
                std::size_t j2 = (j0 + 2) % 3;
                std::size_t a = adjacents[j0];
                if (a != PlanarMesh<T>::invalid)
                {
                    // Get center of adjacent triangle's inscribed circle.
                    U[j0] = mTriangleData[a].center;
                }
                else
                {
                    // No adjacent triangle, use center of edge.
                    U[j0] = static_cast<T>(0.5) * (V[j1] + V[j2]);
                }
            }

            // Compute intermediate terms.
            std::array<T, 3> cenT{}, cen0{}, cen1{}, cen2{};
            mMesh.GetBarycentrics(t, tData.center, cenT);
            mMesh.GetBarycentrics(t, U[0], cen0);
            mMesh.GetBarycentrics(t, U[1], cen1);
            mMesh.GetBarycentrics(t, U[2], cen2);

            T alpha = (cenT[1] * cen0[0] - cenT[0] * cen0[1]) / (cen0[0] - cenT[0]);
            T beta = (cenT[2] * cen1[1] - cenT[1] * cen1[2]) / (cen1[1] - cenT[1]);
            T gamma = (cenT[0] * cen2[2] - cenT[2] * cen2[0]) / (cen2[2] - cenT[2]);
            T oneMinusAlpha = C_<T>(1) - alpha;
            T oneMinusBeta = C_<T>(1) - beta;
            T oneMinusGamma = C_<T>(1) - gamma;

            std::array<T, 9> A{}, B{};
            std::fill(A.begin(), A.end(), C_<T>(0));
            std::fill(B.begin(), B.end(), C_<T>(0));

            T const half = static_cast<T>(0.5);
            T tmp = cenT[0] * V[0][0] + cenT[1] * V[1][0] + cenT[2] * V[2][0];
            A[0] = half * (tmp - V[0][0]);
            A[1] = half * (tmp - V[1][0]);
            A[2] = half * (tmp - V[2][0]);
            A[3] = half * beta * (V[2][0] - V[0][0]);
            A[4] = half * oneMinusGamma * (V[1][0] - V[0][0]);
            A[5] = half * gamma * (V[0][0] - V[1][0]);
            A[6] = half * oneMinusAlpha * (V[2][0] - V[1][0]);
            A[7] = half * alpha * (V[1][0] - V[2][0]);
            A[8] = half * oneMinusBeta * (V[0][0] - V[2][0]);

            tmp = cenT[0] * V[0][1] + cenT[1] * V[1][1] + cenT[2] * V[2][1];
            B[0] = half * (tmp - V[0][1]);
            B[1] = half * (tmp - V[1][1]);
            B[2] = half * (tmp - V[2][1]);
            B[3] = half * beta * (V[2][1] - V[0][1]);
            B[4] = half * oneMinusGamma * (V[1][1] - V[0][1]);
            B[5] = half * gamma * (V[0][1] - V[1][1]);
            B[6] = half * oneMinusAlpha * (V[2][1] - V[1][1]);
            B[7] = half * alpha * (V[1][1] - V[2][1]);
            B[8] = half * oneMinusBeta * (V[0][1] - V[2][1]);

            // Compute Bezier coefficients.
            tData.coeff[2] = jet[0].F;  // hx0
            tData.coeff[4] = jet[1].F;  // hx1
            tData.coeff[6] = jet[2].F;  // hx2

            tData.coeff[14] = jet[0].F + A[0] * jet[0].DFDX + B[0] * jet[0].DFDY;  // hp0
            tData.coeff[7] = jet[0].F + A[3] * jet[0].DFDX + B[3] * jet[0].DFDY;   // hell0
            tData.coeff[8] = jet[0].F + A[4] * jet[0].DFDX + B[4] * jet[0].DFDY;   // hr0
            tData.coeff[16] = jet[1].F + A[1] * jet[1].DFDX + B[1] * jet[1].DFDY;  // hp1
            tData.coeff[9] = jet[1].F + A[5] * jet[1].DFDX + B[5] * jet[1].DFDY;   // hell1
            tData.coeff[10] = jet[1].F + A[6] * jet[1].DFDX + B[6] * jet[1].DFDY;  // hr1
            tData.coeff[18] = jet[2].F + A[2] * jet[2].DFDX + B[2] * jet[2].DFDY;  // hp2
            tData.coeff[11] = jet[2].F + A[7] * jet[2].DFDX + B[7] * jet[2].DFDY;  // hell2
            tData.coeff[12] = jet[2].F + A[8] * jet[2].DFDX + B[8] * jet[2].DFDY;  // hr2

            tData.coeff[5] = alpha * tData.coeff[10] + oneMinusAlpha * tData.coeff[11];   // he12
            tData.coeff[17] = alpha * tData.coeff[16] + oneMinusAlpha * tData.coeff[18];  // hq1
            tData.coeff[1] = beta * tData.coeff[12] + oneMinusBeta * tData.coeff[7];      // he20
            tData.coeff[13] = beta * tData.coeff[18] + oneMinusBeta * tData.coeff[14];    // hq2
            tData.coeff[3] = gamma * tData.coeff[8] + oneMinusGamma * tData.coeff[9];     // he01
            tData.coeff[15] = gamma * tData.coeff[14] + oneMinusGamma * tData.coeff[16];  // hq0
            tData.coeff[0] = cenT[0] * tData.coeff[14] + cenT[1] * tData.coeff[16] + cenT[2] * tData.coeff[18];  // hc
        }

        class TriangleData
        {
        public:
            TriangleData()
                :
                center{},
                intersect{},
                coeff{}
            {
                coeff.fill(C_<T>(0));
            }

            Vector2<T> center;
            std::array<Vector2<T>, 3> intersect;
            std::array<T, 19> coeff;
        };

        class Jet
        {
        public:
            Jet()
                :
                F(C_<T>(0)),
                DFDX(C_<T>(0)),
                DFDY(C_<T>(0))
            {
            }

            T F, DFDX, DFDY;
        };

        PlanarMesh<T> const& mMesh;
        std::vector<T> mF;
        std::vector<T> mDFDX;
        std::vector<T> mDFDY;
        std::vector<TriangleData> mTriangleData;
        bool mMeshIsConvex;
        std::size_t mNumThreads;

        // Keep track of the last triangle visited during an interpolation.
        mutable std::size_t mLastVisited;

    private:
        friend class UnitTestIntpQuadraticNonuniform2;
    };
}
