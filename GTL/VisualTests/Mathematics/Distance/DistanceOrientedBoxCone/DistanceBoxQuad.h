// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2024
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
// https://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// Version: 6.0.2022.01.06

#pragma once

#include <GTL/Mathematics/Algebra/Matrix.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <GTL/Mathematics/Minimizers/LCPSolver.h>
#include <GTL/Mathematics/Primitives/ND/OrientedBox.h>
#include <GTL/Mathematics/Primitives/ND/Cone.h>

// Compute the distance between an oriented box and a triangular cross
// section of a finite truncated cone (0 = hmin < hmax < +infinity) or
// an oriented box and a quadrilateral cross section of a cone frustum
// (0 < hmin < hmax < +infinity). The code supports the box-cone distance
// sample application, but it could be formalized into a DCPQuery class
// and moved to GTL/Mathematics/Distance/3D/DistOrientedBox3Quad3.h at
// a later time.

namespace gtl
{
    template <typename T>
    class DistanceBoxQuad
    {
    public:
        DistanceBoxQuad()
            :
            quadrilateral{},
            mLCP{}
        {
        }

        bool operator()(OrientedBox3<T> const& box, Cone3<T> const& cone,
            T const& sliceAngle, T& distance, Vector3<T>& boxClosest,
            Vector3<T>& coneClosest)
        {
            Vector3<T> K = box.center, ell{};
            for (int32_t i = 0; i < 3; ++i)
            {
                K -= box.extent[i] * box.axis[i];
                ell[i] = static_cast<T>(2) * box.extent[i];
            }

            std::array<Vector3<T>, 3> basis{};
            basis[0] = cone.direction;
            ComputeOrthogonalComplement(basis[0], basis[1], basis[2]);
            std::array<Vector3<T>, 3> W = { basis[1], basis[2], basis[0] };
            T cs = std::cos(sliceAngle), sn = std::sin(sliceAngle);
            Vector3<T> term = cone.tanAngle * (cs * W[0] + sn * W[1]);
            std::array<Vector3<T>, 2> G{};
            G[0] = W[2] - term;
            G[1] = W[2] + term;

            // For visualization.
            T const hmin = cone.GetMinHeight();
            T const hmax = cone.GetMaxHeight();
            quadrilateral[0] = cone.vertex + hmin * G[0];
            quadrilateral[1] = cone.vertex + hmin * G[1];
            quadrilateral[2] = cone.vertex + hmax * G[0];
            quadrilateral[3] = cone.vertex + hmax * G[1];

            T const zero = static_cast<T>(0), one = static_cast<T>(1);
            Matrix<T, 5, 5> A{};  // A is the zero matrix
            A(0, 0) = one;
            A(0, 1) = zero;
            A(0, 2) = zero;
            A(0, 3) = -Dot(box.axis[0], G[0]);
            A(0, 4) = -Dot(box.axis[0], G[1]);
            A(1, 0) = A(0, 1);
            A(1, 1) = one;
            A(1, 2) = zero;
            A(1, 3) = -Dot(box.axis[1], G[0]);
            A(1, 4) = -Dot(box.axis[1], G[1]);
            A(2, 0) = A(0, 2);
            A(2, 1) = A(1, 2);
            A(2, 2) = one;
            A(2, 3) = -Dot(box.axis[2], G[0]);
            A(2, 4) = -Dot(box.axis[2], G[1]);
            A(3, 0) = A(0, 3);
            A(3, 1) = A(1, 3);
            A(3, 2) = A(2, 3);
            A(3, 3) = Dot(G[0], G[0]);
            A(3, 4) = Dot(G[0], G[1]);
            A(4, 0) = A(0, 4);
            A(4, 1) = A(1, 4);
            A(4, 2) = A(2, 4);
            A(4, 3) = A(3, 4);
            A(4, 4) = Dot(G[1], G[1]);

            Vector3<T> KmV = K - cone.vertex;
            Vector<T, 5> b{};
            b[0] = Dot(box.axis[0], KmV);
            b[1] = Dot(box.axis[1], KmV);
            b[2] = Dot(box.axis[2], KmV);
            b[3] = -Dot(G[0], KmV);
            b[4] = -Dot(G[1], KmV);

            Matrix<T, 5, 5> D{};  // D is the zero matrix
            D(0, 0) = -one;
            D(1, 1) = -one;
            D(2, 2) = -one;
            D(3, 3) = one;
            D(3, 4) = one;
            D(4, 3) = -one;
            D(4, 4) = -one;

            Vector<T, 5> e{};
            e[0] = -ell[0];
            e[1] = -ell[1];
            e[2] = -ell[2];
            e[3] = hmin;
            e[4] = -hmax;

            std::array<T, 10> q{};
            for (std::size_t i = 0; i < 5; ++i)
            {
                q[i] = b[i];
                q[i + 5] = -e[i];
            }

            std::array<std::array<T, 10>, 10> M{};
            for (std::size_t r = 0; r < 5; ++r)
            {
                for (std::size_t c = 0; c < 5; ++c)
                {
                    M[r][c] = A(r, c);
                    M[r + 5][c] = D(r, c);
                    M[r][c + 5] = -D(c, r);
                    M[r + 5][c + 5] = zero;
                }
            }

            std::array<T, 10> w{}, z{};
            if (mLCP(q, M, w, z))
            {
                boxClosest = K;
                for (std::size_t i = 0; i < 3; ++i)
                {
                    boxClosest += z[i] * box.axis[i];
                }

                coneClosest = cone.vertex;
                for (std::size_t i = 0; i < 2; ++i)
                {
                    coneClosest += z[i + 3] * G[i];
                }

                distance = Length(boxClosest - coneClosest);
                return true;
            }
            else
            {
                return false;
            }
        }

        std::array<Vector3<T>, 4> quadrilateral;

    private:
        LCPSolver<T, 10> mLCP;
    };
}
