// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

#include <GTL/Mathematics/Primitives/2D/Ellipse2.h>
#include <GTL/Mathematics/Primitives/3D/Ellipsoid3.h>
#include <GTL/Mathematics/Primitives/3D/Plane3.h>

namespace gtl
{
    // Perspectively project an ellipsoid onto a plane.
    //
    // The ellipsoid has center C, axes A[i] and extents e[i] for 0 <= i <= 2.
    //
    // The eyepoint is E.
    // 
    // The view plane is Dot(N,X) = d, where N is a unit-length normal vector.
    // Choose U and V so that {U,V,N} is a right-handed orthonormal set; that
    // is, the vectors are unit length, mutually perpendicular and
    // N = Cross(U,V). N must be directed away from E in the sense that the
    // point K on the plane closest to E is K = E + n * N with n > 0. When
    // using a view frustum, n is the 'near' distance (from the eyepoint to
    // the view plane). The plane equation is then
    //   0 = Dot(N,X-K) = Dot(N,X) - Dot(N,E) - n = d - Dot(N,E) - n
    // so that n = d - Dot(N,E).
    //
    // The ellipsoid must be between the eyepoint and the view plane in the
    // sense that all rays from the eyepoint that intersect the ellipsoid must
    // also intersect the view plane. The precondition test is to project the
    // ellipsoid onto the line E + s * N to obtain interval [smin,smax] where
    // smin > 0. The function Project(ellipsoid, line, smin, smax) defined
    // previously in this file can be used to verify the precondition. If the
    // precondition is satisfied, the projection is an ellipse in the plane.
    // If the precondition is not satisfied, the projection is a conic section
    // that is not an ellipse or it is the empty set.
    //
    // The output is the equation of the ellipse in 2D. The projected ellipse
    // coordinates Y = (y0,y1) are the view plane coordinates of the actual 3D
    // ellipse points X = K + y0 * U + y1 * V = K + J * Y, where J is a 3x2
    // matrix whose columns are U and V.

    // Use this query when you have a single plane and a single ellipsoid to
    // project onto the plane.
    template <typename T>
    void PerspectiveProject(Ellipsoid3<T> const& ellipsoid, Vector3<T> const& E,
        Plane3<T> const& plane, Ellipse2<T>& ellipse)
    {
        Vector3<T> N = plane.normal, U{}, V{};
        ComputeOrthonormalBasis(1, N, U, V);
        T n = plane.constant - Dot(N, E);
        PerspectiveProject(ellipsoid, E, N, U, V, n, ellipse);
    }

    // Use this query when you have a single plane and multiple ellipsoids to
    // project onto the plane. The vectors U and V and the near value n are
    // precomputed.
    template <typename T>
    void PerspectiveProject(Ellipsoid3<T> const& ellipsoid, Vector3<T> const& E,
        Vector3<T> const& N, Vector3<T> const& U, Vector3<T> const& V,
        T const& n, Ellipse2<T>& ellipse)
    {
        // Compute the coefficients for the ellipsoid represented by the
        // quadratic equation X^T*A*X + B^T*X + C = 0.
        Matrix3x3<T> A{};
        Vector3<T> B{};
        T C{};
        ellipsoid.ToCoefficients(A, B, C);

        // Compute the matrix M; see PerspectiveProjectionEllipsoid.pdf for
        // the mathematical details.
        Vector3<T> AE = A * E;
        T qformEAE = Dot(E, AE);
        T dotBE = Dot(B, E);
        T quadE = C_<T>(4) * (qformEAE + dotBE + C);
        Vector3<T> Bp2AE = B + C_<T>(2) * AE;
        Matrix3x3<T> M = OuterProduct(Bp2AE, Bp2AE) - quadE * A;

        // Compute the coefficients for the projected ellipse.
        Vector3<T> MU = M * U;
        Vector3<T> MV = M * V;
        Vector3<T> MN = M * N;
        T twoN = C_<T>(2) * n;
        Matrix2x2<T> AOut{};
        Vector2<T> BOut{};
        T COut{};
        AOut(0, 0) = Dot(U, MU);
        AOut(0, 1) = Dot(U, MV);
        AOut(1, 0) = AOut(0, 1);
        AOut(1, 1) = Dot(V, MV);
        BOut[0] = twoN * (Dot(U, MN));
        BOut[1] = twoN * (Dot(V, MN));
        COut = n * n * Dot(N, MN);

        // Extract the ellipse center, axis directions and extents.
        ellipse.FromCoefficients(AOut, BOut, COut);
    }
}
