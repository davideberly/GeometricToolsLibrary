// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.12

#pragma once

// Conversions among representations of rotations and rigid motions. Rotation
// axes must be unit length. The angles are in units of radians.

#include <GTL/Mathematics/Algebra/Rotation.h>
#include <GTL/Mathematics/Algebra/DualQuaternion.h>
#include <algorithm>
#include <cmath>
#include <cstddef>

namespace gtl
{
    template <typename T>
    class RigidMotion
    {
    public:
        // ROTATION CONVERSIONS. See Rotation.h for the mathematical details
        // of the conversions.

        static void Convert(T const& angle, Matrix2x2<T>& r)
        {
            Rotation<T>::Convert(angle, r);
        }

        static void Convert(Matrix2x2<T> const& r, T& angle)
        {
            Rotation<T>::Convert(r, angle);
        }

        static void Convert(Matrix3x3<T> const& r, Quaternion<T>& q)
        {
            Rotation<T>::Convert(r, q);
        }

        static void Convert(Quaternion<T> const& q, Matrix3x3<T>& r)
        {
            Rotation<T>::Convert(q, r);
        }

        static void Convert(Matrix3x3<T> const& r, AxisAngle<T>& a)
        {
            Rotation<T>::Convert(r, a);
        }

        static void Convert(AxisAngle<T> const& a, Matrix3x3<T>& r)
        {
            Rotation<T>::Convert(a, r);
        }

        static void Convert(Matrix3x3<T> const& r, EulerAngles<T>& e)
        {
            // The e.axis[] indices must be set before the call to Convert.
            Rotation<T>::Convert(r, e);
        }

        static void Convert(EulerAngles<T> const& e, Matrix3x3<T>& r)
        {
            // The e.axis[] indices must be set before the call to Convert.
            Rotation<T>::Convert(e, r);
        }

        static void Convert(Quaternion<T> const& q, AxisAngle<T>& a)
        {
            Rotation<T>::Convert(q, a);
        }

        static void Convert(AxisAngle<T> const& a, Quaternion<T>& q)
        {
            Rotation<T>::Convert(a, q);
        }

        static void Convert(Quaternion<T> const& q, EulerAngles<T>& e)
        {
            // The e.axis[] indices must be set before the call to Convert.
            Rotation<T>::Convert(q, e);
        }

        static void Convert(EulerAngles<T> const& e, Quaternion<T>& q)
        {
            // The e.axis[] indices must be set before the call to Convert.
            Rotation<T>::Convert(e, q);
        }

        static void Convert(AxisAngle<T> const& a, EulerAngles<T>& e)
        {
            // The e.axis[] indices must be set before the call to Convert.
            Rotation<T>::Convert(a, e);
        }

        static void Convert(EulerAngles<T> const& e, AxisAngle<T>& a)
        {
            // The e.axis[] indices must be set before the call to Convert.
            Rotation<T>::Convert(e, a);
        }


        // RIGID MOTION CONVERSIONS (rotations and translations).

        // Convert a dual quaternion to a rotation (as a quaternion) and a
        // translation.
        static void Convert(DualQuaternion<T> const& d, Quaternion<T>& q,
            Vector3<T>& t)
        {
            q = d[0];
            Quaternion<T> product = C_<T>(2) * d[1] * Conjugate(q);
            t = { product[0], product[1], product[2] };
        }

        // Convert a dual quaternion to a rotation (as a matrix) and a
        // translation.
        static void Convert(DualQuaternion<T> const& d, Matrix3x3<T>& r,
            Vector3<T>& t)
        {
            Quaternion<T> q{};
            Convert(d, q, t);
            Convert(q, r);
        }

        // Convert a rotation (as a quaternion) and a translation to a dual
        // quaternion.
        static void Convert(Quaternion<T> const& q, Vector3<T> const& t,
            DualQuaternion<T>& d)
        {
            d[0] = q;
            d[1] = C_<T>(1, 2) *
                Quaternion<T>(t[0], t[1], t[2], C_<T>(0)) * q;
        }

        // Convert a rotation (as a matrix) and a translation to a dual
        // quaternion.
        static void Convert(Matrix3x3<T> const& r, Vector3<T> const& t,
            DualQuaternion<T>& d)
        {
            Quaternion<T> q{};
            Convert(r, q);
            Convert(q, t, d);
        }


        // MIXED-DIMENSION CONVERSIONS. The caller is responsible for ensuring
        // the input 3x3 matrices are rotation and the input 4x4 matrices are
        // homogeneous that represent a rigid motion. The outputs use the
        // convention that R*U = V for 3x3 rotation matrix R and 3x1 vectors
        // U and V. They use the convention that H*U = V for 4x4 homogeneous
        // matrix H and 4x1 homogeneous vectors U and V.
        static void Convert(Matrix3x3<T> const& r, Vector3<T> const& t,
            Matrix4x4<T>& h)
        {
            for (std::size_t row = 0; row < 3; ++row)
            {
                for (std::size_t col = 0; col < 3; ++col)
                {
                    h(row, col) = r(row, col);
                }
                h(row, 3) = t[row];
            }
            h(3, 0) = C_<T>(0);
            h(3, 1) = C_<T>(0);
            h(3, 2) = C_<T>(0);
            h(3, 3) = C_<T>(1);
        }

        static void Convert(Matrix4x4<T> const& h, Matrix3x3<T>& r,
            Vector3<T>& t)
        {
            for (std::size_t row = 0; row < 3; ++row)
            {
                for (std::size_t col = 0; col < 3; ++col)
                {
                    r(row, col) = h(row, col);
                }
                t[row] = h(row, 3);
            }
        }

    private:
        friend class UnitTestRigidMotion;
    };
}
