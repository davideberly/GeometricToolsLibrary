// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Utility/Exceptions.h>
#include <GTL/Mathematics/Algebra/Matrix.h>
#include <GTL/Graphics/SceneGraph/Visibility/CullingPlane.h>
#include <cstdint>
#include <limits>

namespace gtl
{
    template <typename T>
    class BoundingSphere
    {
    public:
        // Construction and destruction.  The default constructor sets the
        // center to the origin (0,0,0) and the radius to 0.  A radius of 0
        // denotes an invalid bound.
        BoundingSphere()
            :
            mTuple{ C_<T>(0), C_<T>(0), C_<T>(0), C_<T>(0) }
        {
        }

        BoundingSphere(BoundingSphere const& sphere)
            :
            mTuple(sphere.mTuple)
        {
        }

        ~BoundingSphere()
        {
        }

        // Assignment.
        BoundingSphere& operator=(BoundingSphere const& sphere)
        {
            mTuple = sphere.mTuple;
            return *this;
        }

        // Member access.  The radius must be nonnegative.  When negative,
        // it is clamped to zero.
        inline void SetCenter(Vector3<T> const& center)
        {
            mTuple[0] = center[0];
            mTuple[1] = center[1];
            mTuple[2] = center[2];
        }

        inline void SetRadius(T radius)
        {
            mTuple[3] = (radius >= (T)0 ? radius : (T)0);
        }

        inline Vector3<T> GetCenter() const
        {
            return Vector3<T>{ mTuple[0], mTuple[1], mTuple[2] };
        }

        inline T GetRadius() const
        {
            return mTuple[3];
        }

        // The "positive side" of the plane is the half space to which the
        // plane normal is directed.  The "negative side" is the other half
        // space.  The function returns +1 when the sphere is fully on the
        // positive side, -1 when the sphere is fully on the negative side, or
        // 0 when the sphere is transversely cut by the plane (sphere volume 
        // on each side of plane is positive).
        int32_t WhichSide(CullingPlane<T> const& plane) const
        {
            Vector4<T> hcenter = HLift(GetCenter(), (T)1);
            T signedDistance = plane.DistanceTo(hcenter);
            T radius = GetRadius();

            if (signedDistance <= -radius)
            {
                return -1;
            }

            if (signedDistance >= radius)
            {
                return +1;
            }

            return 0;
        }

        // Increase 'this' to contain the input sphere.
        void GrowToContain(BoundingSphere const& sphere)
        {
            T radius1 = sphere.GetRadius();
            if (radius1 == (T)0)
            {
                // The incoming bound is invalid and cannot affect growth.
                return;
            }

            T radius0 = GetRadius();
            if (radius0 == (T)0)
            {
                // The current bound is invalid, so just assign the incoming
                // bound.
                mTuple = sphere.mTuple;
                return;
            }

            Vector3<T> center0 = GetCenter();
            Vector3<T> center1 = sphere.GetCenter();
            Vector3<T> centerDiff = center1 - center0;
            T lengthSqr = Dot(centerDiff, centerDiff);
            T radiusDiff = radius1 - radius0;
            T radiusDiffSqr = radiusDiff * radiusDiff;

            if (radiusDiffSqr >= lengthSqr)
            {
                if (radiusDiff >= (T)0)
                {
                    mTuple = sphere.mTuple;
                }
                return;
            }

            T length = std::sqrt(lengthSqr);
            if (length > (T)0)
            {
                T coeff = (length + radiusDiff) / ((T)2 * length);
                SetCenter(center0 + coeff * centerDiff);
            }

            SetRadius((T)0.5 * (length + radius0 + radius1));
        }

        // Transform the sphere.  If the transform has nonuniform scaling, the
        // resulting object is an ellipsoid.  A sphere is generated to contain
        // the ellipsoid.
        void TransformBy(Matrix4x4<T> const& hmatrix, BoundingSphere& sphere) const
        {
            // The spectral norm (maximum absolute value of the eigenvalues)
            // is smaller or equal to max-row-sum and max-col-sum norm.
            // Therefore, 'norm' is an approximation to the maximum scale.
            Vector4<T> hcenter = hmatrix * HLift(GetCenter(), (T)1);
            sphere.SetCenter(HProject(hcenter));

            // Use the max-row-sum matrix norm.
            T r0 = std::fabs(hmatrix(0, 0)) + std::fabs(hmatrix(0, 1)) + std::fabs(hmatrix(0, 2));
            T r1 = std::fabs(hmatrix(1, 0)) + std::fabs(hmatrix(1, 1)) + std::fabs(hmatrix(1, 2));
            T r2 = std::fabs(hmatrix(2, 0)) + std::fabs(hmatrix(2, 1)) + std::fabs(hmatrix(2, 2));
            T norm = std::max(std::max(r0, r1), r2);
            sphere.SetRadius(norm * GetRadius());
        }

        // This function is valid only for 3-channel points (x,y,z) or
        // 4-channel vectors (x,y,z,0) or 4-channel points (x,y,z,1).  In all
        // cases, the function accesses only the (x,y,z) values.  The stride
        // allows you to pass in vertex buffer data.  Set the stride to zero
        // when the points are contiguous in memory.  The 'data' pointer must
        // be to the first point (offset 0).
        void ComputeFromData(uint32_t numVertices, uint32_t vertexSize, char const* data)
        {
            // The center is the average of the positions.
            T sum[3] = { (T)0, (T)0, (T)0 };
            for (uint32_t i = 0; i < numVertices; ++i)
            {
                T const* position = reinterpret_cast<T const*>(data +
                    static_cast<size_t>(i) * vertexSize);
                sum[0] += position[0];
                sum[1] += position[1];
                sum[2] += position[2];
            }
            T invNumVertices = (T)1 / static_cast<T>(numVertices);
            mTuple[0] = sum[0] * invNumVertices;
            mTuple[1] = sum[1] * invNumVertices;
            mTuple[2] = sum[2] * invNumVertices;

            // The radius is the largest distance from the center to the
            // positions.
            mTuple[3] = (T)0;
            for (uint32_t i = 0; i < numVertices; ++i)
            {
                T const* position = reinterpret_cast<T const*>(data + static_cast<size_t>(i) * vertexSize);
                T diff[3] =
                {
                    position[0] - mTuple[0],
                    position[1] - mTuple[1],
                    position[2] - mTuple[2]
                };
                T radiusSqr = diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2];
                if (radiusSqr > mTuple[3])
                {
                    mTuple[3] = radiusSqr;
                }
            }

            mTuple[3] = std::sqrt(mTuple[3]);
        }

        // Test for intersection of linear component and bound (points of
        // intersection not computed).  The linear component is parameterized
        // by P + t*D, where P is a point on the component (the origin) and D
        // is a unit-length direction vector.  The interval [tmin,tmax] is
        //   line:     tmin = -INFINITY, tmax = INFINITY
        //   ray:      tmin = 0.0f, tmax = INFINITY
        //   segment:  tmin >= 0.0f, tmax > tmin
        // where INFINITY is std::numeric_limits<T>::max().
        bool TestIntersection(Vector3<T> const& origin, Vector3<T> const& direction,
            T tmin, T tmax) const
        {
            T radius = GetRadius();
            GTL_RUNTIME_ASSERT(
                radius > C_<T>(0),
                "Invalid bound. Did you forget to call UpdateModelBound()?");

            Vector3<T> center = GetCenter();
            T const infinity = std::numeric_limits<T>::max();
            Vector3<T> diff;
            T a0, a1, discr;

            if (tmin == -infinity)
            {
                GTL_ARGUMENT_ASSERT(
                    tmax == infinity,
                    "tmax must be infinity for a line.");

                // Test for sphere-line intersection.
                diff = origin - center;
                a0 = Dot(diff, diff) - radius * radius;
                a1 = Dot(direction, diff);
                discr = a1 * a1 - a0;
                return discr >= (T)0;
            }

            if (tmax == infinity)
            {
                GTL_ARGUMENT_ASSERT(
                    tmin == C_<T>(0),
                    "tmin must be zero for a ray.");

                // Test for sphere-ray intersection.
                diff = origin - center;
                a0 = Dot(diff, diff) - radius * radius;
                if (a0 <= (T)0)
                {
                    // The ray origin is inside the sphere.
                    return true;
                }
                // else: The ray origin is outside the sphere.

                a1 = Dot(direction, diff);
                if (a1 >= (T)0)
                {
                    // The ray forms an acute angle with diff, and so the ray
                    // is directed from the sphere.  Thus, the ray origin is
                    // outside the sphere, and points P+t*D for t >= 0 are
                    // even farther away from the sphere.
                    return false;
                }

                discr = a1 * a1 - a0;
                return discr >= (T)0;
            }

            GTL_ARGUMENT_ASSERT(
                tmax > tmin,
                "tmin < tmax is required for a segment.");

            // Test for sphere-segment intersection.
            T taverage = (T)0.5 * (tmin + tmax);
            Vector3<T> segOrigin = origin + taverage * direction;
            T segExtent = (T)0.5 * (tmax - tmin);

            diff = segOrigin - GetCenter();
            a0 = Dot(diff, diff) - radius * radius;
            if (a0 <= (T)0)
            {
                // The segment center is inside the sphere.
                return true;
            }

            a1 = Dot(direction, diff);
            discr = a1 * a1 - a0;
            if (discr <= (T)0)
            {
                // The line is outside the sphere, which implies the segment
                // is also.
                return false;
            }

            // See "3D Game Engine Design (2nd edition)", Section 15.4.3 for
            // the details of the test-intersection query for a segment and a
            // sphere.  In the book, 'qval' is the same as
            // '(segment.e - |a1|)^2 - discr'.
            T absA1 = std::fabs(a1);
            T tmp = segExtent - absA1;
            return tmp * tmp <= discr || segExtent >= absA1;
        }

        // Test for intersection of the two stationary spheres.
        bool TestIntersection(BoundingSphere const& sphere) const
        {
            LogAssert(sphere.GetRadius() > (T)0 && GetRadius() > (T)0,
                "Invalid bound. Did you forget to call UpdateModelBound()?");

            // Test for staticSphere-staticSphere intersection.
            Vector3<T> diff = GetCenter() - sphere.GetCenter();
            T rSum = GetRadius() + sphere.GetRadius();
            return Dot(diff, diff) <= rSum * rSum;
        }

        // Test for intersection of the two moving spheres.  The velocity0 is
        // for 'this' and the velocity1 is the for the input bound.
        bool TestIntersection(BoundingSphere const& sphere, T tmax,
            Vector3<T> const& velocity0, Vector3<T> const& velocity1) const
        {
            LogAssert(sphere.GetRadius() > (T)0 && GetRadius() > (T)0,
                "Invalid bound. Did you forget to call UpdateModelBound()?");

            // Test for movingSphere-movingSphere intersection.
            Vector3<T> relVelocity = velocity1 - velocity0;
            Vector3<T> cenDiff = sphere.GetCenter() - GetCenter();
            T a = Dot(relVelocity, relVelocity);
            T c = Dot(cenDiff, cenDiff);
            T rSum = sphere.GetRadius() + GetRadius();
            T rSumSqr = rSum * rSum;

            if (a > (T)0)
            {
                T b = Dot(cenDiff, relVelocity);
                if (b <= (T)0)
                {
                    if (-tmax * a <= b)
                    {
                        return a * c - b * b <= a * rSumSqr;
                    }
                    else
                    {
                        return tmax * (tmax * a + (T)2 * b) + c <= rSumSqr;
                    }
                }
            }

            return c <= rSumSqr;
        }

    private:
        // (center, radius) = (c0, c1, c2, r)
        std::array<T, 4> mTuple;
    };
}
