// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.1.12

#pragma once

// These functions are recommended by the IEEE 754-2008 Standard.

#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <cmath>
#include <cstdint>

namespace gtl
{
    template <typename T>
    inline T atandivpi(T const& x)
    {
        return std::atan(x) * C_INV_PI<T>;
    }

    template <typename T>
    inline T atan2divpi(T const& y, T const& x)
    {
        return std::atan2(y, x) * C_INV_PI<T>;
    }

    template <typename T>
    inline T clamp(T const& x, T const& xmin, T const& xmax)
    {
        return (x <= xmin ? xmin : (x >= xmax ? xmax : x));
    }

    template <typename T>
    inline T cospi(T const& x)
    {
        return std::cos(x * C_PI<T>);
    }

    template <typename T>
    inline T exp10(T const& x)
    {
        return std::exp(x * C_LN_10<T>);
    }

    template <typename T>
    inline T invsqrt(T const& x)
    {
        return C_<T>(1) / std::sqrt(x);
    }

    template <typename T>
    inline std::int32_t isign(T const& x)
    {
        return (x > C_<T>(0) ? 1 : (x < C_<T>(0) ? -1 : 0));
    }

    template <typename T>
    inline T saturate(T const& x)
    {
        return (x <= C_<T>(0) ? C_<T>(0) : (x >= C_<T>(1) ? C_<T>(1) : x));
    }

    template <typename T>
    inline T sign(T const& x)
    {
        return (x > C_<T>(0) ? C_<T>(1) : (x < C_<T>(0) ? -C_<T>(1) : C_<T>(0)));
    }

    template <typename T>
    inline T sinpi(T const& x)
    {
        return std::sin(x * C_PI<T>);
    }

    template <typename T>
    inline T sqr(T const& x)
    {
        return x * x;
    }

    // Compute x * y + z as a single operation. If the fused-multiply-add
    // (fma) instruction is supported by your floating-point hardware, the
    // std::fma function is called. If your hardware does not support the
    // fma instruction and the compiler maps it to a software implementation,
    // you can define GTL_DISCARD_FMA to avoid the computational cost in
    // software. If the compiler ignores the instruction and computes the
    // expression with 2 floating-point operations, it does not matter
    // whether you define GTL_DISCARD_FMA. TODO: Add a preprocessor-selected
    // software implementation for fma using the ideas of BSNumber with
    // integer type UIntegerFP32<N> for N sufficiently large.
    template <typename T>
    inline T FMA(T u, T v, T w)
    {
#if defined(GTL_DISCARD_FMA)
        return u * v + w;
#else
        return std::fma(u, v, w);
#endif
    }

    // Robust sum of products (SOP) u * v + w * z. The robustness occurs only
    // when std::fma is exposed (GTL_DISCARD_FMA is not defined).
    template <typename T>
    inline T RobustSOP(T u, T v, T w, T z)
    {
#if defined(GTL_DISCARD_FMA)
        return u * v + w * z;
#else
        T productWZ = w * z;
        T roundingError = std::fma(w, z, -productWZ);
        T result = std::fma(u, v, productWZ) + roundingError;
        return result;
#endif
    }

    // Robust difference of products (DOP) u * v - w * z. The robustness
    // occurs only when std::fma is exposed (GTL_DISCARD_FMA is not defined).
    template <typename T>
    inline T RobustDOP(T u, T v, T w, T z)
    {
#if defined(GTL_DISCARD_FMA)
        return u * v - w * z.
#else
        T productWZ = w * z;
        T roundingError = std::fma(-w, z, productWZ);
        T result = std::fma(u, v, -productWZ) + roundingError;
        return result;
#endif
    }
}
