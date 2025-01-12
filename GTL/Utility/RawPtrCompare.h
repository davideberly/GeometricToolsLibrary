// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#pragma once

// Documentation for this class is
// https://www.geometrictools.com/Documentation/GTLUtility.pdf#PointerComparison

namespace gtl
{
    // p0 == p1
    template <typename T>
    struct RawPtrEQ
    {
        bool operator()(T const* p0, T const* p1) const
        {
            return (p0 ? (p1 ? *p0 == *p1 : false) : !p1);
        }
    };

    // p0 != p1
    template <typename T>
    struct RawPtrNE
    {
        bool operator()(T const* p0, T const* p1) const
        {
            return !RawPtrEQ<T>()(p0, p1);
        }
    };

    // p0 < p1
    template <typename T>
    struct RawPtrLT
    {
        bool operator()(T const* p0, T const* p1) const
        {
            return (p1 ? (!p0 || *p0 < *p1) : false);
        }
    };

    // p0 <= p1
    template <typename T>
    struct RawPtrLE
    {
        bool operator()(T const* p0, T const* p1) const
        {
            return !RawPtrLT<T>()(p1, p0);
        }
    };

    // p0 > p1
    template <typename T>
    struct RawPtrGT
    {
        bool operator()(T const* p0, T const* p1) const
        {
            return RawPtrLT<T>()(p1, p0);
        }
    };

    // p0 >= p1
    template <typename T>
    struct RawPtrGE
    {
        bool operator()(T const* p0, T const* p1) const
        {
            return !RawPtrLT<T>()(p0, p1);
        }
    };
}
