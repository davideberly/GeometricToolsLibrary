// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

// Class Mesh must have the following functions in its interface.
//    size_t GetNumVertices() const;
//    Vector3<float> GetPosition(size_t i) const;
//    size_t GetNumTriangles() const;
//    bool GetTriangle(size_t t, std::array<int32_t, 3>& indices) const;
//    bool GetModelTriangle(size_t t, Triangle3<float>& modelTriangle) const;
//    bool GetWorldTriangle(size_t t, Triangle3<float>& worldTriangle) const;
//    Matrix4x4<float> const& GetWorldTransform() const;
// A wrapper of this type for Visual objects representing triangle meshes
// is Graphics/CollisionMesh.{h,cpp}.
//
// Class Bound must have the following functions in its interface.
//    Bound();
//    void ComputeFromData(uint32_t numElements, uint32_t stride,
//      char const* data);
//    void TransformBy(Matrix4x4<float> const& hmatrix,
//        Bound& bound) const;
//    bool TestIntersection(Bound const& bound) const;
//    bool TestIntersection(Bound const& bound, float tmax,
//        Vector3<float> const& velocity0,
//        Vector3<float> const& velocity1) const;
// A wrapper of this type for bounding spheres is Graphics/BoundingSphere.h.

#include <GTL/Graphics/SceneGraph/CollisionDetection/CollisionRecord.h>

namespace gtl
{
    template <class Mesh, class Bound>
    class CollisionGroup
    {
    public:
        CollisionGroup()
            :
            mRecords{}
        {
        }

        ~CollisionGroup() = default;

        using Record = CollisionRecord<Mesh, Bound>;

        bool Insert(std::shared_ptr<Record> const& record)
        {
            for (size_t i = 0; i < mRecords.size(); ++i)
            {
                if (record.get() == mRecords[i].get())
                {
                    return false;
                }
            }

            mRecords.push_back(record);
            return true;
        }

        bool Remove(std::shared_ptr<CollisionRecord<Mesh, Bound>> const& record)
        {
            for (size_t i = 0; i < mRecords.size(); ++i)
            {
                if (record.get() == mRecords[i].get())
                {
                    mRecords.erase(mRecords.begin() + i);
                    return true;
                }
            }

            return false;
        }

        // The objects are assumed to be stationary (the velocities are
        // ignored) and all pairs of objects are compared.
        void TestIntersection()
        {
            size_t const numRecords = mRecords.size();
            for (size_t i0 = 0; i0 < numRecords; ++i0)
            {
                auto const& record0 = mRecords[i0];
                for (size_t i1 = i0 + 1; i1 < numRecords; ++i1)
                {
                    auto const& record1 = mRecords[i1];
                    record0->TestIntersection(*record1);
                }
            }
        }

        void FindIntersection()
        {
            size_t const numRecords = mRecords.size();
            for (size_t i0 = 0; i0 < numRecords; ++i0)
            {
                auto const& record0 = mRecords[i0];
                for (size_t i1 = i0 + 1; i1 < numRecords; ++i1)
                {
                    auto const& record1 = mRecords[i1];
                    record0->FindIntersection(*record1);
                }
            }
        }

        // The objects are assumed to be moving. Objects are compared when at
        // least one of them has a velocity vector associated with it. A
        // velocity vector is allowed to be the zero.
        void TestIntersection(float tMax)
        {
            size_t const numRecords = mRecords.size();
            for (size_t i0 = 0; i0 < numRecords; ++i0)
            {
                auto const& record0 = mRecords[i0];
                for (size_t i1 = i0 + 1; i1 < numRecords; ++i1)
                {
                    auto const& record1 = mRecords[i1];
                    record0->TestIntersection(tMax, *record1);
                }
            }
        }

        void FindIntersection(float tMax)
        {
            size_t const numRecords = mRecords.size();
            for (size_t i0 = 0; i0 < numRecords; ++i0)
            {
                auto const& record0 = mRecords[i0];
                for (size_t i1 = i0 + 1; i1 < numRecords; ++i1)
                {
                    auto const& record1 = mRecords[i1];
                    record0->FindIntersection(tMax, *record1);
                }
            }
        }

    private:
        std::vector<std::shared_ptr<Record>> mRecords;
    };
}
