// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.28

#pragma once

// Compute the Delaunay tetrahedralization of 3D points using an incremental
// insertion algorithm. The only way to ensure a correct result for the
// input points is to use an exact predicate for computing signs of
// various expressions. The implementation uses interval arithmetic and
// rational arithmetic for the predicate. The input type T must satisfy
// std::is_floating_point<T>::value = true.

#include <GTL/Mathematics/Arithmetic/ArbitraryPrecision.h>
#include <GTL/Mathematics/Arithmetic/SWInterval.h>
#include <GTL/Mathematics/Meshes/DynamicTSManifoldMesh.h>
#include <GTL/Mathematics/Primitives/ND/Line.h>
#include <GTL/Mathematics/Primitives/3D/Plane3.h>
#include <map>
#include <numeric>
#include <set>
#include <unordered_set>
#include <vector>

namespace gtl
{
    template <typename T>
    class Delaunay3
    {
    public:
        static std::size_t constexpr invalid = std::numeric_limits<std::size_t>::max();

        Delaunay3()
            :
            mNumPoints(0),
            mPoints(nullptr),
            mIRVertices{},
            mGraph(),
            mDuplicates{},
            mNumUniqueVertices(0),
            mDimension(0),
            mLine{},
            mPlane{},
            mNumTetrahedra(0),
            mIndices{},
            mAdjacencies{},
            mQueryPoint{},
            mIRQueryPoint{},
            mCRPool(maxNumCRPool)
        {
            static_assert(
                std::is_floating_point<T>::value,
                "The input type must be a floating-point type.");
        }

        virtual ~Delaunay3() = default;

        // Compute an exact Delaunay tetrahedralization using a blend of
        // interval arithmetic and rational arithmetic. Call GetDimension()
        // to know the dimension of the unique input points. FOR NOW, the
        // only support is for tetrahedralizing points whose convex hull has
        // positive volume.
        // 
        // TODO: Modify Delaunay3 to return a single point when dimension
        // is 0, a sorted list of points when dimension is 1 or a Delaunay
        // triangulation embedded in a plane in 3D when dimension is 2. For
        // now, only the point (0D) or a line (2D) or a plane (3D) are
        // returned, which allows you to project the 3D points to the
        // proper dimension in which to sort the points.
        bool operator()(std::size_t numPoints, Vector3<T> const* points)
        {
            GTL_ARGUMENT_ASSERT(
                numPoints > 0 && points != nullptr,
                "Invalid argument.");

            mNumPoints = numPoints;
            mPoints = points;
            mIRVertices.clear();
            mGraph.Clear();
            mDuplicates.clear();
            mNumUniqueVertices = 0;
            mDimension = 0;
            mLine = Line3<T>{};
            mPlane = Plane3<T>{};
            mNumTetrahedra = 0;
            mIndices.clear();
            mAdjacencies.clear();
            MakeZero(mQueryPoint);
            MakeZero(mIRQueryPoint);

            // Compute the intrinsic dimension and return early if that
            // dimension is 0, 1 or 2.
            Intrinsics3<T> info{};
            info(mNumPoints, mPoints, C_<T>(0));
            if (info.dimension == 0)
            {
                // The points are the same point.
                mDimension = 0;
                mLine.origin = info.origin;
                return false;
            }

            if (info.dimension == 1)
            {
                // The points are collinear.
                mDimension = 1;
                mLine = Line3<T>(info.origin, info.direction[0]);
                return false;
            }

            if (info.dimension == 2)
            {
                // The points are coplanar.
                mDimension = 2;
                mPlane = Plane3<T>(UnitCross(info.direction[0], info.direction[1]),
                    info.origin);
                return false;
            }

            // The points necessarily have a tetrahedralization.
            mDimension = 3;

            // Convert the floating-point inputs to rational type.
            mIRVertices.resize(mNumPoints);
            for (std::size_t i = 0; i < mNumPoints; ++i)
            {
                mIRVertices[i][0] = mPoints[i][0];
                mIRVertices[i][1] = mPoints[i][1];
                mIRVertices[i][2] = mPoints[i][2];
            }

            // Assume initially the points are unique. If duplicates are
            // found during the Delaunay update, mDuplicates[] will be
            // modified accordingly.
            mDuplicates.resize(mNumPoints);
            std::iota(mDuplicates.begin(), mDuplicates.end(), 0);

            // Insert the nondegenerate tetrahedron constructed by the call to
            // IntrinsicsVector2{T}. This is necessary for the circumsphere
            // visibility algorithm to work correctly.
            if (!info.extremeCCW)
            {
                std::swap(info.extreme[2], info.extreme[3]);
            }
            auto inserted = mGraph.Insert(info.extreme[0], info.extreme[1],
                info.extreme[2], info.extreme[3]);
            GTL_RUNTIME_ASSERT(
                inserted != nullptr,
                "The tetrahedron should not be degenerate.");

            // Incrementally update the tetrahedralization. The set of
            // processed points is maintained to eliminate duplicates.
            ProcessedVertexSet processed{};
            for (std::size_t i = 0; i < 4; ++i)
            {
                std::size_t j = info.extreme[i];
                processed.insert(ProcessedVertex(mPoints[j], j));
                mDuplicates[j] = j;
            }
            for (std::size_t i = 0; i < mNumPoints; ++i)
            {
                ProcessedVertex v(mPoints[i], i);
                auto iter = processed.find(v);
                if (iter == processed.end())
                {
                    Update(i);
                    processed.insert(v);
                }
                else
                {
                    mDuplicates[i] = iter->location;
                }
            }
            mNumUniqueVertices = processed.size();

            // Assign integer values to the tetrahedra for use by the caller
            // and copy the tetrahedra information to compact arrays mIndices
            // and mAdjacencies.
            UpdateIndicesAdjacencies();

            return true;
        }

        bool operator()(std::vector<Vector3<T>> const& points)
        {
            return operator()(points.size(), points.data());
        }

        // Dimensional information. If GetDimension() returns 1, the points
        // lie on a line P+t*D. You can sort these if you need a polyline
        // output by projecting onto the line each vertex X = P+t*D, where 
        // t = Dot(D,X-P). If GetDimension() returns 2, the points line on a
        // plane P+s*U+t*V. You can project each vertex X = P+s*U+t*V, where
        // s = Dot(U,X-P) and t = Dot(V,X-P) and then apply Delaunay2 to the
        // (s,t) tuples.
        inline std::size_t GetDimension() const
        {
            return mDimension;
        }

        inline Line3<T> const& GetLine() const
        {
            return mLine;
        }

        inline Plane3<T> const& GetPlane() const
        {
            return mPlane;
        }

        // Member access.
        inline std::size_t GetNumVertices() const
        {
            return mIRVertices.size();
        }

        inline Vector3<T> const* GetVertices() const
        {
            return mPoints;
        }

        inline std::size_t GetNumUniqueVertices() const
        {
            return mNumUniqueVertices;
        }

        // If 'points' has no duplicates, GetDuplicates()[i] = i for all i.
        // If points[i] is the first occurrence of a vertex and if
        // points[j] is found later, then GetDuplicates()[j] = i.
        inline std::vector<std::size_t> const& GetDuplicates() const
        {
            return mDuplicates;
        }

        inline std::size_t GetNumTetrahedra() const
        {
            return mNumTetrahedra;
        }

        inline DynamicTSManifoldMesh const& GetGraph() const
        {
            return mGraph;
        }

        inline std::vector<std::size_t> const& GetIndices() const
        {
            return mIndices;
        }

        inline std::vector<std::size_t> const& GetAdjacencies() const
        {
            return mAdjacencies;
        }

        // Locate those tetrahedra faces that do not share other tetrahedra.
        // The returned array has hull.size() = 3*numFaces indices, each
        // triple representing a triangle. The triangles are counterclockwise
        // ordered when viewed from outside the hull. The return value is
        // 'true' iff the dimension is 3.
        bool GetHull(std::vector<std::size_t>& hull) const
        {
            if (mDimension == 3)
            {
                // Count the number of triangles that are not shared by two
                // tetrahedra.
                std::size_t numTriangles = 0;
                for (auto adj : mAdjacencies)
                {
                    if (adj == invalid)
                    {
                        ++numTriangles;
                    }
                }

                if (numTriangles > 0)
                {
                    // Enumerate the triangles. The prototypical case is the
                    // single tetrahedron V[0] = (0,0,0), V[1] = (1,0,0),
                    // V[2] = (0,1,0) and V[3] = (0,0,1) with no adjacent
                    // tetrahedra. The mIndices[] array is <0,1,2,3>.
                    //   i = 0, face = 0:
                    //    skip index 0, <x,1,2,3>, no swap, triangle = <1,2,3>
                    //   i = 1, face = 1:
                    //    skip index 1, <0,x,2,3>, swap,    triangle = <0,3,2>
                    //   i = 2, face = 2:
                    //    skip index 2, <0,1,x,3>, no swap, triangle = <0,1,3>
                    //   i = 3, face = 3:
                    //    skip index 3, <0,1,2,x>, swap,    triangle = <0,2,1>
                    // To guarantee counterclockwise order of triangles when
                    // viewed outside the tetrahedron, the swap of the last
                    // two indices occurs when face is an odd number;
                    // (face % 2) != 0.
                    hull.resize(3 * numTriangles);
                    std::size_t current = 0, i = 0;
                    for (auto adj : mAdjacencies)
                    {
                        if (adj == invalid)
                        {
                            std::size_t tetra = i / 4, face = i % 4;
                            for (std::size_t j = 0; j < 4; ++j)
                            {
                                if (j != face)
                                {
                                    hull[current++] = mIndices[4 * tetra + j];
                                }
                            }
                            if ((face % 2) != 0)
                            {
                                std::swap(hull[current - 1], hull[current - 2]);
                            }
                        }
                        ++i;
                    }
                    return true;
                }
                else
                {
                    GTL_RUNTIME_ERROR(
                        "There must be at least one tetrahedron.");
                }
            }
            else
            {
                GTL_RUNTIME_ERROR(
                    "The dimension must be 3.");
            }
        }

        // Copy Delaunay tetrahedra to compact arrays mIndices and
        // mAdjacencies. The array information is accessible via the
        // functions GetIndices(std::size_t, std::array<std::int32_t, 4>&) and
        // GetAdjacencies(std::size_t, std::array<std::int32_t, 4>&).
        void UpdateIndicesAdjacencies()
        {
            // Assign integer values to the tetrahedra for use by the caller.
            auto const& smap = mGraph.GetTetrahedra();
            std::map<Tetrahedron*, std::size_t> permute{};
            std::size_t i = invalid;
            permute[nullptr] = i++;
            for (auto const& element : smap)
            {
                permute[element.second.get()] = i++;
            }

            // Put Delaunay tetrahedra into an array (points and adjacency
            // info).
            mNumTetrahedra = smap.size();
            std::size_t numIndices = 4 * mNumTetrahedra;
            if (mNumTetrahedra > 0)
            {
                mIndices.resize(numIndices);
                mAdjacencies.resize(numIndices);
                i = 0;
                for (auto const& element : smap)
                {
                    Tetrahedron* tetra = element.second.get();
                    for (std::size_t j = 0; j < 4; ++j, ++i)
                    {
                        mIndices[i] = tetra->V[j];
                        mAdjacencies[i] = permute[tetra->S[j]];
                    }
                }
            }
        }

        // Get the vertex indices for tetrahedron i. The function returns
        // 'true' when the dimension is 3 and i is a valid tetrahedron index,
        // in which case the points are valid; otherwise, the function
        // returns 'false' and the points are invalid.
        bool GetIndices(std::size_t t, std::array<std::size_t, 4>& indices) const
        {
            if (mDimension == 3)
            {
                std::size_t const numTetrahedra = mIndices.size() / 4;
                if (t < numTetrahedra)
                {
                    std::size_t fourI = 4 * t;
                    indices[0] = mIndices[fourI++];
                    indices[1] = mIndices[fourI++];
                    indices[2] = mIndices[fourI++];
                    indices[3] = mIndices[fourI];
                    return true;
                }
            }
            return false;
        }

        // Get the indices for tetrahedra adjacent to tetrahedron i. The
        // function returns 'true' when the dimension is 3 and i is a valid
        // tetrahedron index, in which case the adjacencies are valid;
        // otherwise, the function returns 'false' and the adjacencies are
        // invalid.
        bool GetAdjacencies(std::size_t t, std::array<std::size_t, 4>& adjacencies) const
        {
            if (mDimension == 3)
            {
                std::size_t const numTetrahedra = mIndices.size() / 4;
                if (t < numTetrahedra)
                {
                    std::size_t fourI = 4 * t;
                    adjacencies[0] = mAdjacencies[fourI++];
                    adjacencies[1] = mAdjacencies[fourI++];
                    adjacencies[2] = mAdjacencies[fourI++];
                    adjacencies[3] = mAdjacencies[fourI];
                    return true;
                }
            }
            return false;
        }

        // Support for searching the tetrahedralization for a tetrahedron
        // that contains a point. If there is a containing tetrahedron, the
        // returned value is a tetrahedron index i with
        // 0 <= t < GetNumTetrahedra(). If there is not a containing
        // tetrahedron, '-1'invalid' is returned. The computations are
        // performed using/ exact rational arithmetic.
        //
        // The SearchInfo input stores information about the tetrahedron
        // search when looking for the tetrahedron (if any) that contains p.
        // The first tetrahedron searched is 'initialTetrahedron'. On return
        // 'path' stores those (ordered) tetrahedron indices visited during
        // the search. The last visited tetrahedron has index
        // 'finalTetrahedron' and vertex indices 'finalV[0,1,2,3]', stored in
        // volumetric counterclockwise order. The last face of the search is
        // <finalV[0],finalV[1],finalV[2]>. For spatially coherent inputs p
        // for numerous calls to this function, you will want to specify
        // 'finalTetrahedron' from the previous call as 'initialTetrahedron'
        // for the next call, which should reduce search times.

        struct SearchInfo
        {
            SearchInfo()
                :
                initialTetrahedron(0),
                numPath(0),
                finalTetrahedron(0),
                finalV{ 0, 0, 0, 0 },
                path{}
            {
            }

            std::size_t initialTetrahedron;
            std::size_t numPath;
            std::size_t finalTetrahedron;
            std::array<std::size_t, 4> finalV;
            std::vector<std::size_t> path;
        };

        // If the point is in a tetrahedron, the return value is the index of
        // the tetrahedron. If the point is not in a tetrahedron, the return
        // value isstd::numeric_limits<std::size_t>::max().
        std::size_t GetContainingTetrahedron(Vector3<T> const& inP, SearchInfo& info) const
        {
            GTL_RUNTIME_ASSERT(
                mDimension == 3,
                "Invalid dimension for tetrahedron search.");

            mQueryPoint = inP;
            mIRQueryPoint = { inP[0], inP[1], C_<T>(0) };

            std::size_t const numTetrahedra = mIndices.size() / 4;
            info.path.resize(numTetrahedra);
            info.numPath = 0;
            std::size_t tetrahedron{};
            if (info.initialTetrahedron < numTetrahedra)
            {
                tetrahedron = info.initialTetrahedron;
            }
            else
            {
                info.initialTetrahedron = 0;
                tetrahedron = 0;
            }

            // Use tetrahedron faces as binary separating planes.
            std::size_t adjacent{};
            for (std::size_t i = 0; i < numTetrahedra; ++i)
            {
                std::size_t ibase = 4 * tetrahedron;
                std::size_t const* v = &mIndices[ibase];

                info.path[info.numPath++] = tetrahedron;
                info.finalTetrahedron = tetrahedron;
                info.finalV[0] = v[0];
                info.finalV[1] = v[1];
                info.finalV[2] = v[2];
                info.finalV[3] = v[3];

                // <V1,V2,V3> counterclockwise when viewed outside
                // tetrahedron.
                if (ToPlane(invalid, v[1], v[2], v[3]) > 0)
                {
                    adjacent = mAdjacencies[ibase];
                    if (adjacent == invalid)
                    {
                        info.finalV[0] = v[1];
                        info.finalV[1] = v[2];
                        info.finalV[2] = v[3];
                        info.finalV[3] = v[0];
                        return invalid;
                    }
                    tetrahedron = adjacent;
                    continue;
                }

                // <V0,V3,V2> counterclockwise when viewed outside
                // tetrahedron.
                if (ToPlane(invalid, v[0], v[2], v[3]) < 0)
                {
                    adjacent = mAdjacencies[ibase + 1];
                    if (adjacent == invalid)
                    {
                        info.finalV[0] = v[0];
                        info.finalV[1] = v[2];
                        info.finalV[2] = v[3];
                        info.finalV[3] = v[1];
                        return invalid;
                    }
                    tetrahedron = adjacent;
                    continue;
                }

                // <V0,V1,V3> counterclockwise when viewed outside
                // tetrahedron.
                if (ToPlane(invalid, v[0], v[1], v[3]) > 0)
                {
                    adjacent = mAdjacencies[ibase + 2];
                    if (adjacent == invalid)
                    {
                        info.finalV[0] = v[0];
                        info.finalV[1] = v[1];
                        info.finalV[2] = v[3];
                        info.finalV[3] = v[2];
                        return invalid;
                    }
                    tetrahedron = adjacent;
                    continue;
                }

                // <V0,V2,V1> counterclockwise when viewed outside
                // tetrahedron.
                if (ToPlane(invalid, v[0], v[1], v[2]) < 0)
                {
                    adjacent = mAdjacencies[ibase + 3];
                    if (adjacent == invalid)
                    {
                        info.finalV[0] = v[0];
                        info.finalV[1] = v[1];
                        info.finalV[2] = v[2];
                        info.finalV[3] = v[3];
                        return invalid;
                    }
                    tetrahedron = adjacent;
                    continue;
                }

                return tetrahedron;
            }

            GTL_RUNTIME_ERROR(
                "Unexpected termination of loop while searching for a triangle.");
        }

    protected:
        // The type of the read-only input points[] when converted for
        // rational arithmetic.
        static std::size_t constexpr InputNumWords = std::is_same<T, float>::value ? 2 : 4;
        using InputRational = BSNumber<UIntegerFP32<InputNumWords>>;

        // The vector of points used for geometric queries. The input
        // points are read-only, so we can represent them by the type
        // InputRational.
        std::size_t mNumPoints;
        Vector3<T> const* mPoints;
        std::vector<Vector3<InputRational>> mIRVertices;

        DynamicTSManifoldMesh mGraph;

    private:
        // The compute type used for exact sign classification.
        static std::size_t constexpr ComputeNumWords = std::is_same<T, float>::value ? 44 : 330;
        using ComputeRational = BSNumber<UIntegerFP32<ComputeNumWords>>;

        // Convenient renaming.
        typedef DynamicTSManifoldMesh::Tetrahedron Tetrahedron;

        struct ProcessedVertex
        {
            ProcessedVertex()
                :
                vertex{},
                location(0)
            {
            }

            ProcessedVertex(Vector3<T> const& inVertex, std::size_t inLocation)
                :
                vertex(inVertex),
                location(inLocation)
            {
            }

            // Support for hashing in std::unordered_set<>. The first
            // operator() is the hash function. The second operator() is
            // the equality comparison used for elements in the same bucket.
            std::size_t operator()(ProcessedVertex const& v) const
            {
                return HashValue(v.vertex[0], v.vertex[1], v.vertex[2], v.location);
            }

            bool operator()(ProcessedVertex const& v0, ProcessedVertex const& v1) const
            {
                return v0.vertex == v1.vertex && v0.location == v1.location;
            }

            Vector3<T> vertex;
            std::size_t location;
        };

        using ProcessedVertexSet = std::unordered_set<
            ProcessedVertex, ProcessedVertex, ProcessedVertex>;

        using DirectedTriangleKeySet = std::unordered_set<
            TriangleKey<true>, TriangleKey<true>, TriangleKey<true>>;

        using TetrahedronPtrSet = std::unordered_set<Tetrahedron*>;

        // Given a plane with origin V0 and normal N = Cross(V1-V0,V2-V0)
        // and given a query point P, ToPlane returns
        //   +1, P on positive side of plane (side to which N points)
        //   -1, P on negative side of plane (side to which -N points)
        //    0, P on the plane
        std::int32_t ToPlane(std::size_t pIndex, std::size_t v0Index, std::size_t v1Index, std::size_t v2Index) const
        {
            // The expression tree has 34 nodes consisting of 12 input
            // leaves and 22 compute nodes.

            // Use interval arithmetic to determine the sign if possible.
            auto const& inP = (pIndex != invalid ? mPoints[pIndex] : mQueryPoint);
            Vector3<T> const& inV0 = mPoints[v0Index];
            Vector3<T> const& inV1 = mPoints[v1Index];
            Vector3<T> const& inV2 = mPoints[v2Index];

            // Evaluate the expression tree of intervals.
            auto x0 = SWInterval<T>::Sub(inP[0], inV0[0]);
            auto y0 = SWInterval<T>::Sub(inP[1], inV0[1]);
            auto z0 = SWInterval<T>::Sub(inP[2], inV0[2]);
            auto x1 = SWInterval<T>::Sub(inV1[0], inV0[0]);
            auto y1 = SWInterval<T>::Sub(inV1[1], inV0[1]);
            auto z1 = SWInterval<T>::Sub(inV1[2], inV0[2]);
            auto x2 = SWInterval<T>::Sub(inV2[0], inV0[0]);
            auto y2 = SWInterval<T>::Sub(inV2[1], inV0[1]);
            auto z2 = SWInterval<T>::Sub(inV2[2], inV0[2]);
            auto y0z1 = y0 * z1;
            auto y0z2 = y0 * z2;
            auto y1z0 = y1 * z0;
            auto y1z2 = y1 * z2;
            auto y2z0 = y2 * z0;
            auto y2z1 = y2 * z1;
            auto c0 = y1z2 - y2z1;
            auto c1 = y2z0 - y0z2;
            auto c2 = y0z1 - y1z0;
            auto x0c0 = x0 * c0;
            auto x1c1 = x1 * c1;
            auto x2c2 = x2 * c2;
            auto det = x0c0 + x1c1 + x2c2;

            if (det[0] > C_<T>(0))
            {
                return +1;
            }
            else if (det[1] < C_<T>(0))
            {
                return -1;
            }

            // The exact sign of the determinant is not known, so compute
            // the determinant using rational arithmetic.

            // Name the nodes of the expression tree.
            auto const& irP = (pIndex != invalid ? mIRVertices[pIndex] : mIRQueryPoint);
            Vector3<InputRational> const& irV0 = mIRVertices[v0Index];
            Vector3<InputRational> const& irV1 = mIRVertices[v1Index];
            Vector3<InputRational> const& irV2 = mIRVertices[v2Index];

            // Input nodes.
            auto const& crP0 = (mCRPool[0] = irP[0]);
            auto const& crP1 = (mCRPool[1] = irP[1]);
            auto const& crP2 = (mCRPool[2] = irP[2]);
            auto const& crV00 = (mCRPool[3] = irV0[0]);
            auto const& crV01 = (mCRPool[4] = irV0[1]);
            auto const& crV02 = (mCRPool[5] = irV0[2]);
            auto const& crV10 = (mCRPool[6] = irV1[0]);
            auto const& crV11 = (mCRPool[7] = irV1[1]);
            auto const& crV12 = (mCRPool[8] = irV1[2]);
            auto const& crV20 = (mCRPool[9] = irV2[0]);
            auto const& crV21 = (mCRPool[10] = irV2[1]);
            auto const& crV22 = (mCRPool[11] = irV2[2]);

            // Compute nodes.
            auto& crX0 = mCRPool[12];
            auto& crY0 = mCRPool[13];
            auto& crZ0 = mCRPool[14];
            auto& crX1 = mCRPool[15];
            auto& crY1 = mCRPool[16];
            auto& crZ1 = mCRPool[17];
            auto& crX2 = mCRPool[18];
            auto& crY2 = mCRPool[19];
            auto& crZ2 = mCRPool[20];
            auto& crY0Z1 = mCRPool[21];
            auto& crY0Z2 = mCRPool[22];
            auto& crY1Z0 = mCRPool[23];
            auto& crY1Z2 = mCRPool[24];
            auto& crY2Z0 = mCRPool[25];
            auto& crY2Z1 = mCRPool[26];
            auto& crC0 = mCRPool[27];
            auto& crC1 = mCRPool[28];
            auto& crC2 = mCRPool[29];
            auto& crX0C0 = mCRPool[30];
            auto& crX1C1 = mCRPool[31];
            auto& crX2C2 = mCRPool[32];
            auto& crDet = mCRPool[33];

            // Evaluate the expression tree of rational numbers.
            crX0 = crP0 - crV00;
            crY0 = crP1 - crV01;
            crZ0 = crP2 - crV02;
            crX1 = crV10 - crV00;
            crY1 = crV11 - crV01;
            crZ1 = crV12 - crV02;
            crX2 = crV20 - crV00;
            crY2 = crV21 - crV01;
            crZ2 = crV22 - crV02;
            crY0Z1 = crY0 * crZ1;
            crY0Z2 = crY0 * crZ2;
            crY1Z0 = crY1 * crZ0;
            crY1Z2 = crY1 * crZ2;
            crY2Z0 = crY2 * crZ0;
            crY2Z1 = crY2 * crZ1;
            crC0 = crY1Z2 - crY2Z1;
            crC1 = crY2Z0 - crY0Z2;
            crC2 = crY0Z1 - crY1Z0;
            crX0C0 = crX0 * crC0;
            crX1C1 = crX1 * crC1;
            crX2C2 = crX2 * crC2;
            crDet = crX0C0 + crX1C1 + crX2C2;
            return crDet.GetSign();
        }

        // For a tetrahedron with points ordered as described in the file
        // TetrahedronKey.h, the function returns
        //   +1, P outside circumsphere of tetrahedron
        //   -1, P inside circumsphere of tetrahedron
        //    0, P on circumsphere of tetrahedron
        std::int32_t ToCircumsphere(std::size_t pIndex, std::size_t v0Index, std::size_t v1Index,
            std::size_t v2Index, std::size_t v3Index) const
        {
            // The expression tree has 98 nodes consisting of 15 input
            // leaves and 83 compute nodes.

            // Use interval arithmetic to determine the sign if possible.
            auto const& inP = (pIndex != invalid ? mPoints[pIndex] : mQueryPoint);
            Vector3<T> const& inV0 = mPoints[v0Index];
            Vector3<T> const& inV1 = mPoints[v1Index];
            Vector3<T> const& inV2 = mPoints[v2Index];
            Vector3<T> const& inV3 = mPoints[v3Index];

            // Evaluate the expression tree of intervals.
            auto x0 = SWInterval<T>::Sub(inV0[0], inP[0]);
            auto y0 = SWInterval<T>::Sub(inV0[1], inP[1]);
            auto z0 = SWInterval<T>::Sub(inV0[2], inP[2]);
            auto s00 = SWInterval<T>::Add(inV0[0], inP[0]);
            auto s01 = SWInterval<T>::Add(inV0[1], inP[1]);
            auto s02 = SWInterval<T>::Add(inV0[2], inP[2]);
            auto x1 = SWInterval<T>::Sub(inV1[0], inP[0]);
            auto y1 = SWInterval<T>::Sub(inV1[1], inP[1]);
            auto z1 = SWInterval<T>::Sub(inV1[2], inP[2]);
            auto s10 = SWInterval<T>::Add(inV1[0], inP[0]);
            auto s11 = SWInterval<T>::Add(inV1[1], inP[1]);
            auto s12 = SWInterval<T>::Add(inV1[2], inP[2]);
            auto x2 = SWInterval<T>::Sub(inV2[0], inP[0]);
            auto y2 = SWInterval<T>::Sub(inV2[1], inP[1]);
            auto z2 = SWInterval<T>::Sub(inV2[2], inP[2]);
            auto s20 = SWInterval<T>::Add(inV2[0], inP[0]);
            auto s21 = SWInterval<T>::Add(inV2[1], inP[1]);
            auto s22 = SWInterval<T>::Add(inV2[2], inP[2]);
            auto x3 = SWInterval<T>::Sub(inV3[0], inP[0]);
            auto y3 = SWInterval<T>::Sub(inV3[1], inP[1]);
            auto z3 = SWInterval<T>::Sub(inV3[2], inP[2]);
            auto s30 = SWInterval<T>::Add(inV3[0], inP[0]);
            auto s31 = SWInterval<T>::Add(inV3[1], inP[1]);
            auto s32 = SWInterval<T>::Add(inV3[2], inP[2]);
            auto t00 = s00 * x0;
            auto t01 = s01 * y0;
            auto t02 = s02 * z0;
            auto t10 = s10 * x1;
            auto t11 = s11 * y1;
            auto t12 = s12 * z1;
            auto t20 = s20 * x2;
            auto t21 = s21 * y2;
            auto t22 = s22 * z2;
            auto t30 = s30 * x3;
            auto t31 = s31 * y3;
            auto t32 = s32 * z3;
            auto w0 = t00 + t01 + t02;
            auto w1 = t10 + t11 + t12;
            auto w2 = t20 + t21 + t22;
            auto w3 = t30 + t31 + t32;
            auto x0y1 = x0 * y1;
            auto x0y2 = x0 * y2;
            auto x0y3 = x0 * y3;
            auto x1y0 = x1 * y0;
            auto x1y2 = x1 * y2;
            auto x1y3 = x1 * y3;
            auto x2y0 = x2 * y0;
            auto x2y1 = x2 * y1;
            auto x2y3 = x2 * y3;
            auto x3y0 = x3 * y0;
            auto x3y1 = x3 * y1;
            auto x3y2 = x3 * y2;
            auto z0w1 = z0 * w1;
            auto z0w2 = z0 * w2;
            auto z0w3 = z0 * w3;
            auto z1w0 = z1 * w0;
            auto z1w2 = z1 * w2;
            auto z1w3 = z1 * w3;
            auto z2w0 = z2 * w0;
            auto z2w1 = z2 * w1;
            auto z2w3 = z2 * w3;
            auto z3w0 = z3 * w0;
            auto z3w1 = z3 * w1;
            auto z3w2 = z3 * w2;
            auto u0 = x0y1 - x1y0;
            auto u1 = x0y2 - x2y0;
            auto u2 = x0y3 - x3y0;
            auto u3 = x1y2 - x2y1;
            auto u4 = x1y3 - x3y1;
            auto u5 = x2y3 - x3y2;
            auto v0 = z0w1 - z1w0;
            auto v1 = z0w2 - z2w0;
            auto v2 = z0w3 - z3w0;
            auto v3 = z1w2 - z2w1;
            auto v4 = z1w3 - z3w1;
            auto v5 = z2w3 - z3w2;
            auto u0v5 = u0 * v5;
            auto u1v4 = u1 * v4;
            auto u2v3 = u2 * v3;
            auto u3v2 = u3 * v2;
            auto u4v1 = u4 * v1;
            auto u5v0 = u5 * v0;
            auto det = u0v5 - u1v4 + u2v3 + u3v2 - u4v1 + u5v0;

            if (det[0] > C_<T>(0))
            {
                return +1;
            }
            else if (det[1] < C_<T>(0))
            {
                return -1;
            }

            // The exact sign of the determinant is not known, so compute
            // the determinant using rational arithmetic.

            // Name the nodes of the expression tree.
            auto const& irP = (pIndex != invalid ? mIRVertices[pIndex] : mIRQueryPoint);
            Vector3<InputRational> const& irV0 = mIRVertices[v0Index];
            Vector3<InputRational> const& irV1 = mIRVertices[v1Index];
            Vector3<InputRational> const& irV2 = mIRVertices[v2Index];
            Vector3<InputRational> const& irV3 = mIRVertices[v3Index];

            // Input nodes.
            auto const& crP0 = (mCRPool[0] = irP[0]);
            auto const& crP1 = (mCRPool[1] = irP[1]);
            auto const& crP2 = (mCRPool[2] = irP[2]);
            auto const& crV00 = (mCRPool[3] = irV0[0]);
            auto const& crV01 = (mCRPool[4] = irV0[1]);
            auto const& crV02 = (mCRPool[5] = irV0[2]);
            auto const& crV10 = (mCRPool[6] = irV1[0]);
            auto const& crV11 = (mCRPool[7] = irV1[1]);
            auto const& crV12 = (mCRPool[8] = irV1[2]);
            auto const& crV20 = (mCRPool[9] = irV2[0]);
            auto const& crV21 = (mCRPool[10] = irV2[1]);
            auto const& crV22 = (mCRPool[11] = irV2[2]);
            auto const& crV30 = (mCRPool[12] = irV3[0]);
            auto const& crV31 = (mCRPool[13] = irV3[1]);
            auto const& crV32 = (mCRPool[14] = irV3[2]);

            // Compute nodes.
            auto& crX0 = mCRPool[15];
            auto& crY0 = mCRPool[16];
            auto& crZ0 = mCRPool[17];
            auto& crS00 = mCRPool[18];
            auto& crS01 = mCRPool[19];
            auto& crS02 = mCRPool[20];
            auto& crX1 = mCRPool[21];
            auto& crY1 = mCRPool[22];
            auto& crZ1 = mCRPool[23];
            auto& crS10 = mCRPool[24];
            auto& crS11 = mCRPool[25];
            auto& crS12 = mCRPool[26];
            auto& crX2 = mCRPool[27];
            auto& crY2 = mCRPool[28];
            auto& crZ2 = mCRPool[29];
            auto& crS20 = mCRPool[30];
            auto& crS21 = mCRPool[31];
            auto& crS22 = mCRPool[32];
            auto& crX3 = mCRPool[33];
            auto& crY3 = mCRPool[34];
            auto& crZ3 = mCRPool[35];
            auto& crS30 = mCRPool[36];
            auto& crS31 = mCRPool[37];
            auto& crS32 = mCRPool[38];
            auto& crT00 = mCRPool[39];
            auto& crT01 = mCRPool[40];
            auto& crT02 = mCRPool[41];
            auto& crT10 = mCRPool[42];
            auto& crT11 = mCRPool[43];
            auto& crT12 = mCRPool[44];
            auto& crT20 = mCRPool[45];
            auto& crT21 = mCRPool[46];
            auto& crT22 = mCRPool[47];
            auto& crT30 = mCRPool[48];
            auto& crT31 = mCRPool[49];
            auto& crT32 = mCRPool[50];
            auto& crW0 = mCRPool[51];
            auto& crW1 = mCRPool[52];
            auto& crW2 = mCRPool[53];
            auto& crW3 = mCRPool[54];
            auto& crX0Y1 = mCRPool[55];
            auto& crX0Y2 = mCRPool[56];
            auto& crX0Y3 = mCRPool[57];
            auto& crX1Y0 = mCRPool[58];
            auto& crX1Y2 = mCRPool[59];
            auto& crX1Y3 = mCRPool[60];
            auto& crX2Y0 = mCRPool[61];
            auto& crX2Y1 = mCRPool[62];
            auto& crX2Y3 = mCRPool[63];
            auto& crX3Y0 = mCRPool[64];
            auto& crX3Y1 = mCRPool[65];
            auto& crX3Y2 = mCRPool[66];
            auto& crZ0W1 = mCRPool[67];
            auto& crZ0W2 = mCRPool[68];
            auto& crZ0W3 = mCRPool[69];
            auto& crZ1W0 = mCRPool[70];
            auto& crZ1W2 = mCRPool[71];
            auto& crZ1W3 = mCRPool[72];
            auto& crZ2W0 = mCRPool[73];
            auto& crZ2W1 = mCRPool[74];
            auto& crZ2W3 = mCRPool[75];
            auto& crZ3W0 = mCRPool[76];
            auto& crZ3W1 = mCRPool[77];
            auto& crZ3W2 = mCRPool[78];
            auto& crU0 = mCRPool[79];
            auto& crU1 = mCRPool[80];
            auto& crU2 = mCRPool[81];
            auto& crU3 = mCRPool[82];
            auto& crU4 = mCRPool[83];
            auto& crU5 = mCRPool[84];
            auto& crV0 = mCRPool[85];
            auto& crV1 = mCRPool[86];
            auto& crV2 = mCRPool[87];
            auto& crV3 = mCRPool[88];
            auto& crV4 = mCRPool[89];
            auto& crV5 = mCRPool[90];
            auto& crU0V5 = mCRPool[91];
            auto& crU1V4 = mCRPool[92];
            auto& crU2V3 = mCRPool[93];
            auto& crU3V2 = mCRPool[94];
            auto& crU4V1 = mCRPool[95];
            auto& crU5V0 = mCRPool[96];
            auto& crDet = mCRPool[97];

            // Evaluate the expression tree of rational numbers.
            crX0 = crV00 - crP0;
            crY0 = crV01 - crP1;
            crZ0 = crV02 - crP2;
            crS00 = crV00 + crP0;
            crS01 = crV01 + crP1;
            crS02 = crV02 + crP2;
            crX1 = crV10 - crP0;
            crY1 = crV11 - crP1;
            crZ1 = crV12 - crP2;
            crS10 = crV10 + crP0;
            crS11 = crV11 + crP1;
            crS12 = crV12 + crP2;
            crX2 = crV20 - crP0;
            crY2 = crV21 - crP1;
            crZ2 = crV22 - crP2;
            crS20 = crV20 + crP0;
            crS21 = crV21 + crP1;
            crS22 = crV22 + crP2;
            crX3 = crV30 - crP0;
            crY3 = crV31 - crP1;
            crZ3 = crV32 - crP2;
            crS30 = crV30 + crP0;
            crS31 = crV31 + crP1;
            crS32 = crV32 + crP2;
            crT00 = crS00 * crX0;
            crT01 = crS01 * crY0;
            crT02 = crS02 * crZ0;
            crT10 = crS10 * crX1;
            crT11 = crS11 * crY1;
            crT12 = crS12 * crZ1;
            crT20 = crS20 * crX2;
            crT21 = crS21 * crY2;
            crT22 = crS22 * crZ2;
            crT30 = crS30 * crX3;
            crT31 = crS31 * crY3;
            crT32 = crS32 * crZ3;
            crW0 = crT00 + crT01 + crT02;
            crW1 = crT10 + crT11 + crT12;
            crW2 = crT20 + crT21 + crT22;
            crW3 = crT30 + crT31 + crT32;
            crX0Y1 = crX0 * crY1;
            crX0Y2 = crX0 * crY2;
            crX0Y3 = crX0 * crY3;
            crX1Y0 = crX1 * crY0;
            crX1Y2 = crX1 * crY2;
            crX1Y3 = crX1 * crY3;
            crX2Y0 = crX2 * crY0;
            crX2Y1 = crX2 * crY1;
            crX2Y3 = crX2 * crY3;
            crX3Y0 = crX3 * crY0;
            crX3Y1 = crX3 * crY1;
            crX3Y2 = crX3 * crY2;
            crZ0W1 = crZ0 * crW1;
            crZ0W2 = crZ0 * crW2;
            crZ0W3 = crZ0 * crW3;
            crZ1W0 = crZ1 * crW0;
            crZ1W2 = crZ1 * crW2;
            crZ1W3 = crZ1 * crW3;
            crZ2W0 = crZ2 * crW0;
            crZ2W1 = crZ2 * crW1;
            crZ2W3 = crZ2 * crW3;
            crZ3W0 = crZ3 * crW0;
            crZ3W1 = crZ3 * crW1;
            crZ3W2 = crZ3 * crW2;
            crU0 = crX0Y1 - crX1Y0;
            crU1 = crX0Y2 - crX2Y0;
            crU2 = crX0Y3 - crX3Y0;
            crU3 = crX1Y2 - crX2Y1;
            crU4 = crX1Y3 - crX3Y1;
            crU5 = crX2Y3 - crX3Y2;
            crV0 = crZ0W1 - crZ1W0;
            crV1 = crZ0W2 - crZ2W0;
            crV2 = crZ0W3 - crZ3W0;
            crV3 = crZ1W2 - crZ2W1;
            crV4 = crZ1W3 - crZ3W1;
            crV5 = crZ2W3 - crZ3W2;
            crU0V5 = crU0 * crV5;
            crU1V4 = crU1 * crV4;
            crU2V3 = crU2 * crV3;
            crU3V2 = crU3 * crV2;
            crU4V1 = crU4 * crV1;
            crU5V0 = crU5 * crV0;
            crDet = crU0V5 - crU1V4 + crU2V3 + crU3V2 - crU4V1 + crU5V0;
            return crDet.GetSign();
        }

        bool GetContainingTetrahedron(std::size_t pIndex, Tetrahedron*& tetra) const
        {
            std::size_t const numTetrahedra = mGraph.GetTetrahedra().size();
            for (std::size_t t = 0; t < numTetrahedra; ++t)
            {
                std::size_t j{};
                for (j = 0; j < 4; ++j)
                {
                    auto const& opposite = TetrahedronKey<true>::GetOppositeFace();
                    if (ToPlane(pIndex, tetra->V[opposite[j][0]],
                        tetra->V[opposite[j][1]], tetra->V[opposite[j][2]]) > 0)
                    {
                        // Point i sees face <v0,v1,v2> from outside the
                        // tetrahedron.
                        auto adjTetra = tetra->S[j];
                        if (adjTetra)
                        {
                            // Traverse to the tetrahedron sharing the face.
                            tetra = adjTetra;
                            break;
                        }
                        else
                        {
                            // We reached a hull face, so the point is outside
                            // the hull.
                            return false;
                        }
                    }

                }

                if (j == 4)
                {
                    // The point is inside all four faces, so the point is
                    // inside a tetrahedron.
                    return true;
                }
            }

            GTL_RUNTIME_ERROR(
                "Unexpected termination of loop.");
        }

        void GetAndRemoveInsertionPolyhedron(std::size_t pIndex,
            TetrahedronPtrSet& candidates, DirectedTriangleKeySet& boundary)
        {
            // Locate the tetrahedra that make up the insertion polyhedron.
            DynamicTSManifoldMesh polyhedron{};
            while (candidates.size() > 0)
            {
                Tetrahedron* tetra = *candidates.begin();
                candidates.erase(candidates.begin());

                for (std::size_t j = 0; j < 4; ++j)
                {
                    auto adj = tetra->S[j];
                    if (adj && candidates.find(adj) == candidates.end())
                    {
                        if (ToCircumsphere(pIndex, adj->V[0], adj->V[1],
                            adj->V[2], adj->V[3]) <= 0)
                        {
                            // Point P is in the circumsphere.
                            candidates.insert(adj);
                        }
                    }
                }

                auto inserted = polyhedron.Insert(tetra->V[0], tetra->V[1],
                    tetra->V[2], tetra->V[3]);
                GTL_RUNTIME_ASSERT(
                    inserted != nullptr,
                    "Unexpected insertion failure.");

                auto removed = mGraph.Remove(tetra->V[0], tetra->V[1],
                    tetra->V[2], tetra->V[3]);
                GTL_RUNTIME_ASSERT(
                    removed,
                    "Unexpected removal failure.");
            }

            // Get the boundary triangles of the insertion polyhedron.
            for (auto const& element : polyhedron.GetTetrahedra())
            {
                Tetrahedron* tetra = element.second.get();
                for (std::size_t j = 0; j < 4; ++j)
                {
                    if (!tetra->S[j])
                    {
                        auto const& opposite = TetrahedronKey<true>::GetOppositeFace();
                        boundary.insert(TriangleKey<true>(tetra->V[opposite[j][0]],
                            tetra->V[opposite[j][1]], tetra->V[opposite[j][2]]));
                    }
                }
            }
        }

        void Update(std::size_t pIndex)
        {
            auto const& smap = mGraph.GetTetrahedra();
            Tetrahedron* tetra = smap.begin()->second.get();
            if (GetContainingTetrahedron(pIndex, tetra))
            {
                // The point is inside the convex hull. The insertion
                // polyhedron contains only tetrahedra in the current
                // tetrahedralization; the hull does not change.

                // Use a depth-first search for those tetrahedra whose
                // circumspheres contain point P.
                TetrahedronPtrSet candidates{};
                candidates.insert(tetra);

                // Get the boundary of the insertion polyhedron C that
                // contains the tetrahedra whose circumspheres contain point
                // P. Polyhedron C contains this point.
                DirectedTriangleKeySet boundary{};
                GetAndRemoveInsertionPolyhedron(pIndex, candidates, boundary);

                // The insertion polyhedron consists of the tetrahedra formed
                // by point i and the faces of C.
                for (auto const& key : boundary)
                {
                    if (ToPlane(pIndex, key[0], key[1], key[2]) < 0)
                    {
                        auto inserted = mGraph.Insert(pIndex, key[0], key[1], key[2]);
                        GTL_RUNTIME_ASSERT(
                            inserted != nullptr,
                            "Unexpected insertion failure.");
                    }
                }
            }
            else
            {
                // The point is outside the convex hull. The insertion
                // polyhedron is formed by point P and any tetrahedra in the
                // current tetrahedralization whose circumspheres contain
                // point P.

                // Locate the convex hull of the tetrahedra.
                DirectedTriangleKeySet hull{};
                for (auto const& element : smap)
                {
                    Tetrahedron* t = element.second.get();
                    for (std::size_t j = 0; j < 4; ++j)
                    {
                        if (!t->S[j])
                        {
                            auto const& opposite = TetrahedronKey<true>::GetOppositeFace();
                            hull.insert(TriangleKey<true>(t->V[opposite[j][0]],
                                t->V[opposite[j][1]], t->V[opposite[j][2]]));
                        }
                    }
                }

                // Iterate over all the hull faces and use the ones visible to
                // point i to locate the insertion polyhedron.
                auto const& tmap = mGraph.GetTriangles();
                TetrahedronPtrSet candidates{};
                DirectedTriangleKeySet visible{};
                for (auto const& key : hull)
                {
                    if (ToPlane(pIndex, key[0], key[1], key[2]) > 0)
                    {
                        auto iter = tmap.find(TriangleKey<false>(key[0], key[1], key[2]));
                        if (iter != tmap.end() && iter->second->S[1] == nullptr)
                        {
                            auto adj = iter->second->S[0];
                            if (adj && candidates.find(adj) == candidates.end())
                            {
                                if (ToCircumsphere(pIndex, adj->V[0], adj->V[1], adj->V[2],
                                    adj->V[3]) <= 0)
                                {
                                    // Point P is in the circumsphere.
                                    candidates.insert(adj);
                                }
                                else
                                {
                                    // Point P is not in the circumsphere but
                                    // the hull face is visible.
                                    visible.insert(key);
                                }
                            }
                        }
                        else
                        {
                            GTL_RUNTIME_ERROR(
                                "This condition should not occur for rational arithmetic.");
                        }
                    }
                }

                // Get the boundary of the insertion subpolyhedron C that
                // contains the tetrahedra whose circumspheres contain
                // point P.
                DirectedTriangleKeySet boundary{};
                GetAndRemoveInsertionPolyhedron(pIndex, candidates, boundary);

                // The insertion polyhedron P consists of the tetrahedra
                // formed by point i and the back faces of C *and* the visible
                // faces of mGraph-C.
                for (auto const& key : boundary)
                {
                    if (ToPlane(pIndex, key[0], key[1], key[2]) < 0)
                    {
                        // This is a back face of the boundary.
                        auto inserted = mGraph.Insert(pIndex, key[0], key[1], key[2]);
                        GTL_RUNTIME_ASSERT(
                            inserted != nullptr,
                            "Unexpected insertion failure.");
                    }
                }
                for (auto const& key : visible)
                {
                    auto inserted = mGraph.Insert(pIndex, key[0], key[2], key[1]);
                    GTL_RUNTIME_ASSERT(
                        inserted != nullptr,
                        "Unexpected insertion failure.");
                }
            }
        }

        // If a vertex occurs multiple times in the 'points' input to the
        // constructor, the first processed occurrence of that vertex has an
        // index stored in this array. If there are no duplicates, then
        // mDuplicates[i] = i for all i.
        std::vector<std::size_t> mDuplicates;
        std::size_t mNumUniqueVertices;

        // If the intrinsic dimension of the input points is 0, 1 or 2, the
        // constructor returns early. The caller is responsible for retrieving
        // the dimension and taking an alternate path should the dimension be
        // smaller than 3. If the dimension is 0, all points are the same.
        // If the dimension is 1, the points lie on a line, in which case
        // the caller can project points[] onto the line for further
        // processing. If the dimension is 2, the points lie on a plane, in
        // which case the caller can project points[] onto the plane for
        // further processing.
        std::size_t mDimension;
        Line3<T> mLine;
        Plane3<T> mPlane;

        // These are computed by UpdateIndicesAdjacencies(). They are used
        // for point-containment queries in the tetrahedron mesh.
        std::size_t mNumTetrahedra;
        std::vector<std::size_t> mIndices;
        std::vector<std::size_t> mAdjacencies;

    private:
        // The query point for Update, GetContainingTriangle and
        // GetAndRemoveInsertionPolyhedron when the point is not an input
        // vertex to the constructor. ToPlane(...) and ToCircumsphere(...)
        // are passed indices into the vertex array. When the vertex is
        // valid, mPoints[] and mCRVertices[] are used for lookups. When the
        // vertex is 'invalid', the query point is used for lookups.
        mutable Vector3<T> mQueryPoint;
        mutable Vector3<InputRational> mIRQueryPoint;

        // Sufficient storage for the expression trees related to computing
        // the exact signs in ToPlane(...) and ToCircumsphere(...).
        static std::size_t constexpr maxNumCRPool = 98;
        mutable std::vector<ComputeRational> mCRPool;
    };
}
