// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.03.27

#pragma once

#include <GTL/Mathematics/ImageProcessing/MarchingCubes.h>
#include <GTL/Mathematics/ImageProcessing/Image3.h>
#include <GTL/Mathematics/Meshes/UniqueVerticesSimplices.h>
#include <GTL/Mathematics/Algebra/Vector.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <utility>
#include <vector>

namespace gtl
{
    template <typename T, typename IndexType>
    class SurfaceExtractorMC : public MarchingCubes<IndexType>
    {
    public:
        // Construction and destruction.
        virtual ~SurfaceExtractorMC() = default;

        SurfaceExtractorMC(Image3<T> const& image)
            :
            mImage(image)
        {
        }

        // Object copies are not allowed.
        SurfaceExtractorMC() = delete;
        SurfaceExtractorMC(SurfaceExtractorMC const&) = delete;
        SurfaceExtractorMC const& operator=(SurfaceExtractorMC const&) = delete;

        struct Mesh
        {
            // All members are set to zeros.
            Mesh()
                :
                topology{},
                vertices{}
            {
                vertices.fill(Vector3<T>::Zero());
            }

            typename MarchingCubes<IndexType>::Topology topology;
            std::array<Vector3<T>, MarchingCubes<IndexType>::Topology::maxVertices> vertices;
        };

        // Extract the triangle mesh approximating F = level for a single
        // voxel whose origin corner is (x,y,z). The image has dimensions
        // d0, d1, and d2, and the origin corner satisfies the conditions
        // 0 <= x < d0-1, 0 <= y < d1-1, and 0 <= z < d2-1. The input
        // function values must be stored as
        //  F[0] = image(x  ,y  ,z), F[4] = image(x  ,y  ,z+1),
        //  F[1] = image(x+1,y  ,z), F[5] = image(x+1,y  ,z+1),
        //  F[2] = image(x  ,y+1,z), F[6] = image(x  ,y+1,z+1),
        //  F[3] = image(x+1,y+1,z), F[7] = image(x+1,y+1,z+1)
        // In local coordinates where the corners are (0,0,0), (1,0,0),
        // (0,1,0), (1,1,0), (0,0,1), (1,0,1), (0,1,1), and (1,1,1),
        // Thus, F[k] = imageLocal(k & 1, (k & 2) >> 1, (k & 4) >> 2).
        // The caller of this function must add in the (x,y,z) origin corner
        // to the mesh.vertices[] to obtain the global coordinates of the
        // vertices.
        // 
        // The return value is 'true' iff the F[] values are all not equal
        // to 'level'. If at least one of F[] is 'level', the returned
        // 'mesh' has no vertices and no triangles. If you want this behavior,
        // set 'perturb' to zero.
        //
        // If you want to avoid the case when F[i] = level for some i, set
        // 'perturb' to a small nonzero number whose absolute value is smaller
        // than the minimum absolute value of the differences between voxel
        // values and 'level'.
        bool Extract(T const& level, T const& perturb, std::array<T, 8> const& F,
            Mesh& mesh) const
        {
            T const zero = C_<T>(0);
            T const one = C_<T>(1);
            std::array<T, 8> localF{};

            std::size_t entry = 0;
            for (std::size_t i = 0, mask = 1; i < 8; ++i, mask <<= 1)
            {
                localF[i] = F[i] - level;
                if (localF[i] == zero)
                {
                    localF[i] += perturb;
                }

                if (localF[i] < zero)
                {
                    entry |= mask;
                }
                else if (localF[i] == zero)
                {
                    // If 'perturb' is zero, the function will report that no
                    // geometry is generated for this voxel. If 'perturb' is
                    // not zero, the comparison to zero still needs to be made
                    // after the perturbation in case floating-point rounding
                    // errors cause localF[i] still to be zero.
                    return false;
                }
            }

            mesh.topology = MarchingCubes<IndexType>::GetTable(entry);

            for (std::size_t i = 0; i < mesh.topology.numVertices; ++i)
            {
                IndexType j0 = mesh.topology.vpair[i][0];
                IndexType j1 = mesh.topology.vpair[i][1];

                // The vertex can be computed with 3D-only computations as
                //   V = (F[j0] * k1 - F[j1] * k0) / (F[j1] - F[j0])
                // but floating-point rounding errors can cause at least one
                // of the integer-valued components of V not to be 0 or 1 as
                // the case may be. Such errors in turn can lead to 2 nearly
                // identical vertices in the mesh, and MakeUnique will not be
                // able to characterize them as the same vertex. The
                // componentwise computations avoid these floating-point
                // rounding errors. It is guaranteed that j0 < j1, so multiple
                // voxels sharing the same edge will generate the same vertex.
                auto& vertex = mesh.vertices[i];
                std::array<IndexType, 3> k0 = { j0 & 1, (j0 & 2) >> 1, (j0 & 4) >> 2 };
                std::array<IndexType, 3> k1 = { j1 & 1, (j1 & 2) >> 1, (j1 & 4) >> 2 };
                for (std::size_t index = 0; index < 3; ++index)
                {
                    if (k0[index] == 0)
                    {
                        if (k1[index] == 0)
                        {
                            vertex[index] = zero;
                        }
                        else // k1[index] = 1
                        {
                            vertex[index] = F[j0] / (F[j0] - F[j1]);
                        }
                    }
                    else  // k0[index] = 1
                    {
                        if (k1[index] == 0)
                        {
                            vertex[index] = F[j1] / (F[j1] - F[j0]);
                        }
                        else // k1[index] = 1
                        {
                            vertex[index] = one;
                        }
                    }
                }
            }
            return true;
        }

        // Extract the triangle mesh approximating F = level for all the
        // voxels in a 3D image. The input image must be stored in a
        // 1-dimensional array with lexicographical order. If the image
        // has dimensions d0, d1, and d2, then voxel location (x, y, z)
        // contains value image[x + d0 * (y + d1 * z)]. The output 'indices'
        // consists indices.size()/3 triangles, each a triple of indices into
        // 'vertices'
        //
        // The triangle table lookups depend on voxel values never being
        // exactly equal to 'level'. Set 'perturb' to zero so that any
        // voxel cube with at least one corner value equal to 'level' is
        // ignored in the final mesh; that is, such a voxel does not generate
        // any triangles in the final mesh. If you want triangles from such
        // voxels, set 'perturb' to a small nonzero number whose absolute
        // value is smaller than the minimum absolute value of the differences
        // between voxel values and 'level'.
        void Extract(T const& level, T const& perturb,
            std::vector<Vector3<T>>& vertices, std::vector<IndexType>& indices) const
        {
            vertices.clear();
            indices.clear();

            for (std::size_t z0 = 0, z1 = 1; z1 < mImage.size(2); z0 = z1++)
            {
                for (std::size_t y0 = 0, y1 = 1; y1 < mImage.size(1); y0 = y1++)
                {
                    for (std::size_t x0 = 0, x1 = 1; x1 < mImage.size(0); x0 = x1++)
                    {
                        std::array<T, 8> F{};
                        F[0] = mImage(x0, y0, z0);
                        F[1] = mImage(x1, y0, z0);
                        F[2] = mImage(x0, y1, z0);
                        F[3] = mImage(x1, y1, z0);
                        F[4] = mImage(x0, y0, z1);
                        F[5] = mImage(x1, y0, z1);
                        F[6] = mImage(x0, y1, z1);
                        F[7] = mImage(x1, y1, z1);

                        Mesh mesh{};

                        if (Extract(level, perturb, F, mesh))
                        {
                            IndexType vbase = static_cast<IndexType>(vertices.size());
                            for (std::size_t i = 0; i < mesh.topology.numVertices; ++i)
                            {
                                Vector3<T> position = mesh.vertices[i];
                                position[0] += static_cast<T>(x0);
                                position[1] += static_cast<T>(y0);
                                position[2] += static_cast<T>(z0);
                                vertices.push_back(position);
                            }

                            for (std::size_t i = 0; i < mesh.topology.numTriangles; ++i)
                            {
                                for (std::size_t j = 0; j < 3; ++j)
                                {
                                    indices.push_back(vbase + mesh.topology.itriple[i][j]);
                                }
                            }
                        }
                    }
                }
            }
        }

        // The extraction has duplicate vertices on edges shared by voxels.
        // This function will eliminate the duplication.
        void MakeUnique(std::vector<Vector3<T>>& vertices,
            std::vector<IndexType>& indices) const
        {
            std::vector<Vector3<T>> outVertices{};
            std::vector<IndexType> outIndices{};
            UniqueVerticesSimplices<Vector3<T>, IndexType, 3> uvt{};
            uvt.RemoveDuplicateVertices(vertices, indices, outVertices, outIndices);
            vertices = std::move(outVertices);
            indices = std::move(outIndices);
        }

        // The extraction does not use any topological information about the
        // level surface.  The triangles can be a mixture of clockwise-ordered
        // and counterclockwise-ordered.  This function is an attempt to give
        // the triangles a consistent ordering by selecting a normal in
        // approximately the same direction as the average gradient at the
        // vertices (when sameDir is true), or in the opposite direction (when
        // sameDir is false).  This might not always produce a consistent
        // order, but is fast.  A consistent order can be computed if you
        // build a table of vertex, edge, and face adjacencies, but the
        // resulting data structure is somewhat expensive to process to
        // reorient triangles.
        void OrientTriangles(std::vector<Vector3<T>> const& vertices,
            std::vector<IndexType>&indices, bool sameDir) const
        {
            T const zero = static_cast<T>(0);
            std::size_t const numTriangles = indices.size() / 3;
            IndexType* triangle = indices.data();
            for (std::size_t t = 0; t < numTriangles; ++t, triangle += 3)
            {
                // Get triangle vertices.
                Vector3<T> v0 = vertices[static_cast<std::size_t>(triangle[0])];
                Vector3<T> v1 = vertices[static_cast<std::size_t>(triangle[1])];
                Vector3<T> v2 = vertices[static_cast<std::size_t>(triangle[2])];

                // Construct triangle normal based on current orientation.
                Vector3<T> edge1 = v1 - v0;
                Vector3<T> edge2 = v2 - v0;
                Vector3<T> normal = Cross(edge1, edge2);

                // Get the image gradient at the vertices.
                Vector3<T> gradient0 = GetGradient(v0);
                Vector3<T> gradient1 = GetGradient(v1);
                Vector3<T> gradient2 = GetGradient(v2);

                // Compute the average gradient.
                Vector3<T> gradientAvr = (gradient0 + gradient1 + gradient2) / static_cast<T>(3);

                // Compute the dot product of normal and average gradient.
                T dot = Dot(gradientAvr, normal);

                // Choose triangle orientation based on gradient direction.
                if (sameDir)
                {
                    if (dot < zero)
                    {
                        // Wrong orientation, reorder it.
                        std::swap(triangle[1], triangle[2]);
                    }
                }
                else
                {
                    if (dot > zero)
                    {
                        // Wrong orientation, reorder it.
                        std::swap(triangle[1], triangle[2]);
                    }
                }
            }
        }

        // Compute vertex normals for the mesh.
        void ComputeNormals(std::vector<Vector3<T>> const& vertices,
            std::vector<IndexType> const& indices,
            std::vector<Vector3<T>>& normals) const
        {
            // Maintain a running sum of triangle normals at each vertex.
            normals.resize(vertices.size());
            std::fill(normals.begin(), normals.end(), Vector3<T>::Zero());

            std::size_t const numTriangles = indices.size() / 3;
            IndexType const* triangle = indices.data();
            for (std::size_t t = 0; t < numTriangles; ++t)
            {
                std::size_t i0 = static_cast<std::size_t>(triangle[0]);
                std::size_t i1 = static_cast<std::size_t>(triangle[1]);
                std::size_t i2 = static_cast<std::size_t>(triangle[2]);

                // Get triangle vertices.
                Vector3<T> v0 = vertices[i0];
                Vector3<T> v1 = vertices[i1];
                Vector3<T> v2 = vertices[i2];

                // Construct triangle normal.
                Vector3<T> edge1 = v1 - v0;
                Vector3<T> edge2 = v2 - v0;
                Vector3<T> normal = Cross(edge1, edge2);

                // Maintain the sum of normals at each vertex.
                normals[i0] += normal;
                normals[i1] += normal;
                normals[i2] += normal;
            }

            // The normal vector storage was used to accumulate the sum of
            // triangle normals. Now these vectors must be rescaled to be
            // unit length.
            for (auto& normal : normals)
            {
                Normalize(normal);
            }
        }

    protected:
        Vector3<T> GetGradient(Vector3<T> position) const
        {
            T const zero = static_cast<T>(0);
            Vector3<T> const vzero{};  // the zero vector

            std::size_t x = 0;
            if (position[0] >= zero)
            {
                x = static_cast<std::size_t>(std::floor(position[0]));
                if (x + 1 >= mImage.size(0))
                {
                    return vzero;
                }
            }
            else
            {
                return vzero;
            }

            std::size_t y = 0;
            if (position[1] >= zero)
            {
                y = static_cast<std::size_t>(std::floor(position[1]));
                if (y + 1 >= mImage.size(1))
                {
                    return vzero;
                }
            }
            else
            {
                return vzero;
            }

            std::size_t z = 0;
            if (position[2] >= zero)
            {
                z = static_cast<std::size_t>(std::floor(position[2]));
                if (z + 1 >= mImage.size(2))
                {
                    return vzero;
                }
            }
            else
            {
                return vzero;
            }

            T const one = static_cast<T>(1);
            position[0] -= static_cast<T>(x);
            position[1] -= static_cast<T>(y);
            position[2] -= static_cast<T>(z);
            T oneMX = one - position[0];
            T oneMY = one - position[1];
            T oneMZ = one - position[2];

            // Get image values at corners of voxel.
            std::size_t xp1 = x + 1, yp1 = y + 1, zp1 = z + 1;
            T f000 = mImage(x, y, z);
            T f100 = mImage(xp1, y, z);
            T f010 = mImage(x, yp1, z);
            T f110 = mImage(xp1, yp1, z);
            T f001 = mImage(x, y, zp1);
            T f101 = mImage(xp1, y, zp1);
            T f011 = mImage(x, yp1, zp1);
            T f111 = mImage(xp1, yp1, zp1);

            Vector3<T> gradient{};

            T tmp0 = oneMY * (f100 - f000) + position[1] * (f110 - f010);
            T tmp1 = oneMY * (f101 - f001) + position[1] * (f111 - f011);
            gradient[0] = oneMZ * tmp0 + position[2] * tmp1;

            tmp0 = oneMX * (f010 - f000) + position[0] * (f110 - f100);
            tmp1 = oneMX * (f011 - f001) + position[0] * (f111 - f101);
            gradient[1] = oneMZ * tmp0 + position[2] * tmp1;

            tmp0 = oneMX * (f001 - f000) + position[0] * (f101 - f100);
            tmp1 = oneMX * (f011 - f010) + position[0] * (f111 - f110);
            gradient[2] = oneMY * tmp0 + position[1] * tmp1;

            return gradient;
        }

        Image3<T> const& mImage;

    private:
        friend class UnitTestSurfaceExtractorMC;
    };
}
