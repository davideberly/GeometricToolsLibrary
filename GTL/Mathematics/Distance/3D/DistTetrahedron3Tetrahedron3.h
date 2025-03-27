// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.28

#pragma once

// Compute the distance between two solid tetrahedra in 3D.
// 
// Each tetrahedron has vertices <V[0],V[1],V[2],V[3]>. A tetrahedron point
// is X = sum_{i=0}^3 b[i] * V[i], where 0 <= b[i] <= 1 for all i and
// sum_{i=0}^3 b[i] = 1.
// 
// The closest point on tetra0 is stored in closest[0] with barycentric
// coordinates relative to its vertices. The closest point on tetra1 is stored
// in closest[1] with barycentric coordinates relative to its vertices. When
// there are infinitely many choices for the pair of closest points, only one
// pair is returned.

#include <GTL/Mathematics/Distance/3D/DistTriangle3Triangle3.h>
#include <GTL/Mathematics/Containment/3D/ContTetrahedron3.h>
#include <array>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class DCPQuery<T, Tetrahedron3<T>, Tetrahedron3<T>>
    {
    public:
        struct Output
        {
            Output()
                :
                distance(C_<T>(0)),
                sqrDistance(C_<T>(0)),
                barycentric0{ C_<T>(0), C_<T>(0), C_<T>(0), C_<T>(0) },
                barycentric1{ C_<T>(0), C_<T>(0), C_<T>(0), C_<T>(0) },
                closest{}
            {
            }

            T distance, sqrDistance;
            std::array<T, 4> barycentric0;
            std::array<T, 4> barycentric1;
            std::array<Vector3<T>, 2> closest;
        };

        Output operator()(Tetrahedron3<T> const& tetra0, Tetrahedron3<T> const& tetra1)
        {
            Output output{};

            DCPQuery<T, Triangle3<T>, Triangle3<T>> ttQuery{};
            typename DCPQuery<T, Triangle3<T>, Triangle3<T>>::Output ttOutput{};
            Triangle3<T> triangle{};

            T const invalid = -C_<T>(1);
            output.distance = invalid;
            output.sqrDistance = invalid;

            // Compute the distances between pairs of faces, each pair having
            // a face from tetra0 and a face from tetra1.
            bool foundZeroDistance = false;
            for (std::size_t face0 = 0; face0 < 4; ++face0)
            {
                Triangle3<T> triangle0{};
                std::array<std::size_t, 3> indices0 =
                    Tetrahedron3<T>::GetFaceIndices(face0);

                for (std::size_t j = 0; j < 3; ++j)
                {
                    triangle0.v[j] = tetra0.v[indices0[j]];
                }

                for (std::size_t face1 = 0; face1 < 4; ++face1)
                {
                    Triangle3<T> triangle1{};
                    std::array<std::size_t, 3> indices1 =
                        Tetrahedron3<T>::GetFaceIndices(face1);

                    for (std::size_t j = 0; j < 3; ++j)
                    {
                        triangle1.v[j] = tetra1.v[indices1[j]];
                    }

                    ttOutput = ttQuery(triangle0, triangle1);
                    if (ttOutput.sqrDistance == C_<T>(0))
                    {
                        output.distance = C_<T>(0);
                        output.sqrDistance = C_<T>(0);
                        output.closest[0] = ttOutput.closest[0];
                        output.closest[1] = ttOutput.closest[1];
                        foundZeroDistance = true;
                        break;
                    }

                    if (output.sqrDistance == invalid ||
                        ttOutput.sqrDistance < output.sqrDistance)
                    {
                        output.distance = ttOutput.distance;
                        output.sqrDistance = ttOutput.sqrDistance;
                        output.closest[0] = ttOutput.closest[0];
                        output.closest[1] = ttOutput.closest[1];
                    }
                }

                if (foundZeroDistance)
                {
                    break;
                }
            }

            if (!foundZeroDistance)
            {
                // The tetrahedra are either nested or separated. Test
                // for containment of the centroids to decide which case.
                Vector3<T> centroid0 = tetra0.ComputeCentroid();
                bool centroid0InTetra1 = InContainer(centroid0, tetra1);
                if (centroid0InTetra1)
                {
                    // Tetra0 is nested inside tetra1. Choose the centroid
                    // of tetra0 as the closest point for both tetrahedra.
                    output.distance = C_<T>(0);
                    output.sqrDistance = C_<T>(0);
                    output.closest[0] = centroid0;
                    output.closest[1] = centroid0;
                }

                Vector3<T> centroid1 = tetra1.ComputeCentroid();
                bool centroid1InTetra0 = InContainer(centroid1, tetra0);
                if (centroid1InTetra0)
                {
                    // Tetra1 is nested inside tetra0. Choose the centroid
                    // of tetra1 as the closest point for both tetrahedra.
                    output.distance = C_<T>(0);
                    output.sqrDistance = C_<T>(0);
                    output.closest[0] = centroid1;
                    output.closest[1] = centroid1;
                }

                // With exact arithmetic, at this point the tetrahedra are
                // separated. The output object already contains the distance
                // information. However, with floating-point arithmetic, it
                // is possible that a tetrahedron with volume nearly zero is
                // close enough to the other tetrahedron yet separated, but
                // rounding errors make it appear that the nearly-zero-volume
                // tetrahedron has centroid inside the other tetrahedron. This
                // situation is trapped by the previous two if-blocks.
            }

            // Compute the barycentric coordinates of the closest points.
            (void)ComputeBarycentrics(output.closest[0],
                tetra0.v[0], tetra0.v[1], tetra0.v[2], tetra0.v[3],
                output.barycentric0);

            (void)ComputeBarycentrics(output.closest[1],
                tetra1.v[0],  tetra1.v[1], tetra1.v[2], tetra1.v[3],
                output.barycentric1);

            return output;
        }

    private:
        friend class UnitTestDistTetrahedron3Tetrahedron3;
    };
}
