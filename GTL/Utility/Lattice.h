// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.03.26

#pragma once

// Documentation for this class is
// https://www.geometrictools.com/Documentation/GTLUtility.pdf#Lattice

#include <GTL/Utility/Exceptions.h>
#include <GTL/Utility/TypeTraits.h>
#include <array>
#include <cstddef>
#include <initializer_list>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

// Implementation for a lattice whose sizes are known at compile time.
// The class has no data members. Its member functions use template
// metaprogramming for mapping indices to and from multiindices. The
// Sizes parameter pack has n >= 1 elements and represents the bounds
// (b[0],...,b[n-1]).
namespace gtl
{
    template <bool OrderLtoR, std::size_t... Sizes>
    class Lattice
    {
    public:
        Lattice()
        {
            static_assert(
                sizeof...(Sizes) >= 1,
                "At least one dimension is required.");
        }

        ~Lattice() = default;

        // The number of dimensions is the number of arguments in the Sizes
        // parameter pack. This is 'n' in the comments about lattices.
        inline std::size_t constexpr dimensions() const noexcept
        {
            return sizeof...(Sizes);
        }

        // Get the number of elements for dimension d. This is 'b[d]' in the
        // comments about lattices.
        inline std::size_t constexpr size(std::size_t d) const noexcept
        {
            std::array<std::size_t, sizeof...(Sizes)> sizes{};
            MetaAssignSize<0, Sizes...>(sizes.data());
            return sizes[d];
        }

        // Get the number of elements. This is 'product{d=0}^{n-1} b[d]' in
        // the comments about lattices.
        inline std::size_t constexpr size() const noexcept
        {
            return MetaProduct<Sizes...>::value;
        }

        // *** Conversions for left-to-right ordering.

        // Convert from an n-dimensional index to a 1-dimensional index for
        // left-to-right ordering.
        template <typename... IndexTypes, bool _OrderLtoR = OrderLtoR, TraitSelector<_OrderLtoR> = 0>
        std::size_t index(IndexTypes... ntuple) const noexcept
        {
            static_assert(
                sizeof...(IndexTypes) == sizeof...(Sizes),
                "Invalid number of arguments.");

            return MetaGetIndexLtoR(ntuple...);
        }

        template <typename... IndexTypes, bool _OrderLtoR = OrderLtoR, TraitSelector<_OrderLtoR> = 0>
        std::size_t index(std::array<std::size_t, sizeof...(Sizes)> const& coordinate) const noexcept
        {
            return GetIndexLtoR(coordinate);
        }

        // Convert from a 1-dimension index to an n-dimensional index for
        // left-to-right ordering.
        template <typename... IndexTypes, bool _OrderLtoR = OrderLtoR, TraitSelector<_OrderLtoR> = 0>
        std::array<std::size_t, sizeof...(Sizes)> coordinate(std::size_t i) const noexcept
        {
            // i = x[0] + b[0] * (x[1] + b[1] * (x[2] + ...)
            // tuple = (x[0], ..., x[n-1])
            std::array<std::size_t, sizeof...(Sizes)> tuple{};
            for (std::size_t d = 0; d < sizeof...(Sizes); ++d)
            {
                std::size_t const bound = size(d), j = i;
                i /= bound;
                tuple[d] = j - bound * i;
            }
            return tuple;
        }

        // *** Conversions for right-to-left ordering.

        // Convert from an n-dimensional index to a 1-dimensional index for
        // right-to-left ordering.
        template <typename... IndexTypes, bool _OrderLtoR = OrderLtoR, TraitSelector<!_OrderLtoR> = 0>
        std::size_t index(IndexTypes... ntuple) const noexcept
        {
            static_assert(
                sizeof...(IndexTypes) == sizeof...(Sizes),
                "Invalid number of arguments.");

            using Type = typename std::tuple_element<0, std::tuple<IndexTypes...>>::type;
            return MetaGetIndexRtoL(static_cast<Type>(0), ntuple...);
        }

        template <typename... IndexTypes, bool _OrderLtoR = OrderLtoR, TraitSelector<!_OrderLtoR> = 0>
        std::size_t index(std::array<std::size_t, sizeof...(Sizes)> const& coordinate) const noexcept
        {
            return GetIndexRtoL(coordinate);
        }

        // Convert from a 1-dimension index to an n-dimensional index for
        // right-to-left ordering.
        template <typename... IndexTypes, bool _OrderLtoR = OrderLtoR, TraitSelector<!_OrderLtoR> = 0>
        std::array<std::size_t, sizeof...(Sizes)> coordinate(std::size_t i) const noexcept
        {
            // i = x[n-1] + b[n-1] * (x[n-2] + b[n-2] * (x[n-3] + ...)
            // tuple = (x[0], ..., x[n-1])
            std::array<std::size_t, sizeof...(Sizes)> tuple{};
            for (std::size_t k = 0, d = sizeof...(Sizes) - 1; k < sizeof...(Sizes); ++k, --d)
            {
                std::size_t const bound = size(d), j = i;
                i /= bound;
                tuple[d] = j - bound * i;
            }
            return tuple;
        }

    private:
        // Metaprogramming support for selecting an argument from the
        // Sizes parameter pack.
        template <std::size_t i, std::size_t f, std::size_t... r>
        struct MetaArgument
        {
            static std::size_t const value = MetaArgument<i - 1, r...>::value;
        };

        template <std::size_t f, std::size_t... r>
        struct MetaArgument<0, f, r...>
        {
            static std::size_t const value = f;
        };

        // Metaprogramming support for assigning the Sizes parameter pack
        // arguments to an array.
        template <std::size_t i, std::size_t f, std::size_t... r>
        void constexpr MetaAssignSize(std::size_t* sizes) const noexcept
        {
            sizes[i] = f;
            MetaAssignSize<i + 1, r...>(sizes);
        }

        template <std::size_t numElements>
        void constexpr MetaAssignSize(std::size_t*) const noexcept
        {
        }

        // Metaprogramming support for computing the product of the Sizes
        // parameter pack arguments.
        template <std::size_t...>
        struct MetaProduct
            :
            std::integral_constant<std::size_t, 1>
        {
        };

        template <std::size_t f, std::size_t... r>
        struct MetaProduct<f, r...>
            :
            std::integral_constant<std::size_t, f* MetaProduct<r...>::value>
        {
        };

        // Metaprogramming support for index(IndexTypes...) using
        // left-to-right ordering.
        template <typename First, typename... Successors>
        std::size_t constexpr MetaGetIndexLtoR(First first, Successors... successors) const noexcept
        {
            static_assert(
                std::is_integral<First>::value && !std::is_same<First, bool>::value,
                "Arguments must be integer type.");

            std::size_t constexpr size = MetaArgument<sizeof...(Sizes) - 1 - sizeof...(Successors), Sizes...>::value;
            return static_cast<std::size_t>(first) + size * MetaGetIndexLtoR(successors...);
        }

        template <typename Last>
        std::size_t constexpr MetaGetIndexLtoR(Last last) const noexcept
        {
            static_assert(
                std::is_integral<Last>::value && !std::is_same<Last, bool>::value,
                "Arguments must be integer type.");

            return static_cast<std::size_t>(last);
        }

        // Metaprogramming support for index(std::array<*,*)> const&) using
        // left-to-right ordering.
        template <std::size_t numDimensions = sizeof...(Sizes), TraitSelector<(numDimensions > 1)> = 0>
        std::size_t GetIndexLtoR(std::array<std::size_t, sizeof...(Sizes)> const& coordinate) const noexcept
        {
            std::array<std::size_t, sizeof...(Sizes)> sizes{};
            MetaAssignSize<0, Sizes...>(sizes.data());
            std::size_t d = coordinate.size() - 1;
            std::size_t indexLtoR = coordinate[d];
            --d;
            for (std::size_t k = 1; k < sizeof...(Sizes); ++k, --d)
            {
                indexLtoR = sizes[d] * indexLtoR + coordinate[d];
            }
            return indexLtoR;
        }

        template <std::size_t numDimensions = sizeof...(Sizes), TraitSelector<numDimensions == 1> = 0>
        std::size_t GetIndexLtoR(std::array<std::size_t, 1> const& coordinate) const noexcept
        {
            return coordinate[0];
        }

        // Metaprogramming support for index(IndexTypes...) using
        // right-to-left ordering.
        template <typename Term, typename First, typename... Successors>
        std::size_t constexpr MetaGetIndexRtoL(Term t, First first, Successors... successors) const noexcept
        {
            static_assert(
                std::is_integral<First>::value && !std::is_same<First, bool>::value,
                "Arguments must be integer type.");

            std::size_t constexpr size = MetaArgument<sizeof...(Sizes) - sizeof...(Successors), Sizes...>::value;
            return MetaGetIndexRtoL(size * static_cast<std::size_t>(first + t), successors...);
        }

        template <typename Term, typename First>
        std::size_t constexpr MetaGetIndexRtoL(Term t, First first) const noexcept
        {
            static_assert(
                std::is_integral<First>::value && !std::is_same<First, bool>::value,
                "Arguments must be integer type.");

            return static_cast<std::size_t>(first + t);
        }

        // Metaprogramming support for index(std::array<*,*)> const&) using
        // left-to-right ordering.
        template <std::size_t numDimensions = sizeof...(Sizes), TraitSelector<(numDimensions > 1)> = 0>
        std::size_t GetIndexRtoL(std::array<std::size_t, sizeof...(Sizes)> const& coordinate) const noexcept
        {
            std::array<std::size_t, sizeof...(Sizes)> sizes{};
            MetaAssignSize<0, Sizes...>(sizes.data());
            std::size_t d = 0;
            std::size_t indexRtoL = coordinate[d];
            for (++d; d < sizeof...(Sizes); ++d)
            {
                indexRtoL = sizes[d] * indexRtoL + coordinate[d];
            }
            return indexRtoL;
        }

        template <std::size_t numDimensions = sizeof...(Sizes), TraitSelector<numDimensions == 1> = 0>
        std::size_t GetIndexRtoL(std::array<std::size_t, 1> const& coordinate) const noexcept
        {
            return coordinate[0];
        }

    public:
        static std::size_t constexpr NumDimensions = sizeof...(Sizes);
        static std::size_t constexpr NumElements = MetaProduct<Sizes...>::value;
    };
}

// Implementation for a lattice whose sizes are known only at run time.
// The class stores (b[0],...,b[n-1]) in mSizes and the product of
// bounds in mNumElements.
namespace gtl
{
    template <bool OrderLtoR>
    class Lattice<OrderLtoR>
    {
    public:
        // The lattice has no elements.
        Lattice()
            :
            mNumElements(0),
            mSizes{}
        {
        }

        // The lattice has the specified sizes.
        Lattice(std::vector<std::size_t> const& sizes)
            :
            mNumElements(0),
            mSizes{}
        {
            InternalResize(sizes);
        }

        // The lattice has the specified sizes.
        Lattice(std::initializer_list<std::size_t> const& sizes)
            :
            mNumElements(0),
            mSizes{}
        {
            InternalResize(sizes);
        }

        // Support for deferred construction where the initial lattice is
        // created by the default constructor. During later execution, the
        // lattice sizes can be set as needed.
        void resize(std::vector<std::size_t> const& sizes)
        {
            InternalResize(sizes);
        }

        void resize(std::initializer_list<std::size_t> const& sizes)
        {
            InternalResize(sizes);
        }

        ~Lattice() = default;

        // Copy semantics.
        Lattice(Lattice const& other)
            :
            mNumElements(0),
            mSizes{}
        {
            *this = other;
        }

        Lattice& operator=(Lattice const& other)
        {
            mNumElements = other.mNumElements;
            mSizes = other.mSizes;
            return *this;
        }

        // Move semantics.
        Lattice(Lattice&& other) noexcept
            :
            mNumElements(0),
            mSizes{}
        {
            *this = std::move(other);
        }

        Lattice& operator=(Lattice&& other) noexcept
        {
            mNumElements = other.mNumElements;
            mSizes = std::move(other.mSizes);
            other.mNumElements = 0;
            return *this;
        }

        // The number of dimensions is the number of elements of mSizes. This
        // is 'n' in the comments about lattices.
        inline std::size_t dimensions() const noexcept
        {
            return mSizes.size();
        }

        // Get the number of elements for dimension d. This is 'b[d]' in the
        // comments about lattices.
        inline std::size_t size(std::size_t d) const
        {
            GTL_ARGUMENT_ASSERT(
                d < mSizes.size(),
                "Invalid dimension.");

            return mSizes[d];
        }

        // Get the number of elements. This is 'product{d=0}^{n-1} b[d]' in
        // the comments about lattices.
        inline std::size_t size() const noexcept
        {
            return mNumElements;
        }

        // Convert from an n-dimensional index to a 1-dimensional index for
        // left-to-right ordering.
        template <typename... IndexTypes, bool _OrderLtoR = OrderLtoR, TraitSelector<_OrderLtoR> = 0>
        std::size_t index(IndexTypes... ntuple) const
        {
            GTL_ARGUMENT_ASSERT(
                mSizes.size() > 0 && mSizes.size() == sizeof...(IndexTypes),
                "Invalid arguments to index.");

            return MetaGetIndexLtoR(ntuple...);
        }

        template <bool _OrderLtoR = OrderLtoR, TraitSelector<_OrderLtoR> = 0>
        std::size_t index(std::vector<std::size_t> const& coordinate) const
        {
            GTL_ARGUMENT_ASSERT(
                mSizes.size() > 0 && coordinate.size() == mSizes.size(),
                "Invalid argument to index.");

            std::size_t d = coordinate.size() - 1;
            std::size_t indexLtoR = coordinate[d];
            --d;
            for (std::size_t k = 1; k < coordinate.size(); ++k, --d)
            {
                indexLtoR = mSizes[d] * indexLtoR + coordinate[d];
            }
            return indexLtoR;
        }

        // Convert from an n-dimensional index to a 1-dimensional index for
        // right-to-left ordering.
        template <typename... IndexTypes, bool _OrderLtoR = OrderLtoR, TraitSelector<!_OrderLtoR> = 0>
        std::size_t index(IndexTypes... ntuple) const
        {
            GTL_ARGUMENT_ASSERT(
                mSizes.size() > 0 && mSizes.size() == sizeof...(IndexTypes),
                "Invalid arguments to index.");

            using Type = typename std::tuple_element<0, std::tuple<IndexTypes...>>::type;
            return MetaGetIndexRtoL(static_cast<Type>(0), ntuple...);
        }

        template <bool _OrderLtoR = OrderLtoR, TraitSelector<!_OrderLtoR> = 0>
        std::size_t index(std::vector<std::size_t> const& coordinate) const
        {
            GTL_ARGUMENT_ASSERT(
                mSizes.size() > 0 && coordinate.size() == mSizes.size(),
                "Invalid argument to index.");

            std::size_t d = 0;
            std::size_t indexRtoL = coordinate[d];
            for (++d; d < coordinate.size(); ++d)
            {
                indexRtoL = mSizes[d] * indexRtoL + coordinate[d];
            }
            return indexRtoL;
        }

        // Convert from a 1-dimension index to an n-dimensional index for
        // left-to-right ordering.
        template <bool _OrderLtoR = OrderLtoR, TraitSelector<_OrderLtoR> = 0>
        std::vector<std::size_t> coordinate(std::size_t i) const
        {
            // i = x[0] + b[0] * (x[1] + b[1] * (x[2] + ...)
            // tuple = (x[0], ..., x[n-1])
            std::size_t const numDimensions = dimensions();
            std::vector<std::size_t> tuple(numDimensions);
            for (std::size_t d = 0; d < numDimensions; ++d)
            {
                std::size_t const bound = size(d), j = i;
                i /= bound;
                tuple[d] = j - bound * i;
            }
            return tuple;
        }

        // Convert from a 1-dimension index to an n-dimensional index for
        // right-to-left ordering.
        template <bool _OrderLtoR = OrderLtoR, TraitSelector<!_OrderLtoR> = 0>
        std::vector<std::size_t> coordinate(std::size_t i) const
        {
            // i = x[n-1] + b[n-1] * (x[n-2] + b[n-2] * (x[n-3] + ...)
            // tuple = (x[0], ..., x[n-1])
            std::size_t const numDimensions = dimensions();
            std::vector<std::size_t> tuple(numDimensions);
            for (std::size_t k = 0, d = numDimensions - 1; k < numDimensions; ++k, --d)
            {
                std::size_t const bound = size(d), j = i;
                i /= bound;
                tuple[d] = j - bound * i;
            }
            return tuple;
        }

        // Support for sorting and comparing Lattice objects.
        inline bool operator==(Lattice const& other) const
        {
            return mSizes == other.mSizes;
        }

        inline bool operator!=(Lattice const& other) const
        {
            return mSizes != other.mSizes;
        }

        inline bool operator<(Lattice const& other) const
        {
            return mSizes < other.mSizes;
        }

        inline bool operator<=(Lattice const& other) const
        {
            return mSizes <= other.mSizes;
        }

        inline bool operator>(Lattice const& other) const
        {
            return mSizes < other.mSizes;
        }

        inline bool operator>=(Lattice const& other) const
        {
            return mSizes >= other.mSizes;
        }

    private:
        template <typename Container>
        void InternalResize(Container const& container)
        {
            GTL_ARGUMENT_ASSERT(
                container.size() > 0,
                "The number of dimensions must be positive.");

            mNumElements = 1;
            mSizes.resize(container.size());
            std::size_t d = 0;
            for (auto const& size : container)
            {
                GTL_ARGUMENT_ASSERT(
                    size > 0,
                    "The dimension must be positive");

                mNumElements *= size;
                mSizes[d++] = size;
            }
        }

        // Metaprogramming support for index(IndexTypes...) using
        // left-to-right ordering.
        template <typename First, typename... Successors>
        std::size_t MetaGetIndexLtoR(First first, Successors... successors) const
        {
            static_assert(
                std::is_integral<First>::value && !std::is_same<First, bool>::value,
                "Arguments must be integer type.");

            return static_cast<std::size_t>(first) +
                mSizes[mSizes.size() - 1 - sizeof...(Successors)] *
                MetaGetIndexLtoR(successors...);
        }

        template <typename Last>
        std::size_t MetaGetIndexLtoR(Last last) const
        {
            static_assert(
                std::is_integral<Last>::value && !std::is_same<Last, bool>::value,
                "Arguments must be integer type.");

            return static_cast<std::size_t>(last);
        }

        // Metaprogramming support for index(IndexTypes...) using
        // right-to-left ordering.
        template <typename Term, typename First, typename... Successors>
        std::size_t MetaGetIndexRtoL(Term t, First first, Successors... successors) const
        {
            static_assert(
                std::is_integral<First>::value && !std::is_same<First, bool>::value,
                "Arguments must be integer type.");

            std::size_t size = mSizes[mSizes.size() - sizeof...(Successors)];
            return MetaGetIndexRtoL(size * static_cast<std::size_t>(first + t), successors...);
        }

        template <typename Term, typename First>
        std::size_t MetaGetIndexRtoL(Term t, First first) const
        {
            static_assert(
                std::is_integral<First>::value && !std::is_same<First, bool>::value,
                "Arguments must be integer type.");

            return static_cast<std::size_t>(first + t);
        }

        template <typename First>
        std::size_t MetaGetIndexRtoL(First first) const
        {
            static_assert(
                std::is_integral<First>::value && !std::is_same<First, bool>::value,
                "Arguments must be integer type.");

            return static_cast<std::size_t>(first);
        }

    protected:
        std::size_t mNumElements;
        std::vector<std::size_t> mSizes;

    private:
        friend class UnitTestLattice;
    };
}
