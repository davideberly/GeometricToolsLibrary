// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#pragma once

// See the comments in BSNumber.h about the UInteger requirements. The
// denominator of a BSRational is chosen to be positive, which allows some
// simplification of comparisons. Also see the comments about exposing
// various conditional defines to enable throwing exceptions.

#include <GTL/Mathematics/Arithmetic/BSNumber.h>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <limits>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>

#define GTL_BSR_INITIALIZE_NUMER_DENOM(type, numerator, denominator)\
    mNumerator(numerator),\
    mDenominator(numerator != static_cast<type>(0) ? denominator : static_cast<type>(1))

#if defined(GTL_BINARY_SCIENTIFIC_SHOW_DOUBLE)
#define GTL_BSR_INITIALIZE_NUMER(numerator)\
    mValue(static_cast<double>(numerator)),\
    mNumerator(numerator),\
    mDenominator(1)

#define GTL_BSR_UPDATE_VALUE(object) (object).mValue = static_cast<double>((object))

#else
#define GTL_BSR_INITIALIZE_NUMER(numerator)\
    mNumerator(numerator),\
    mDenominator(1)

#define GTL_BSR_UPDATE_VALUE(object)

#endif

#if defined (GTL_BINARY_SCIENTIFIC_VALIDATE)
#define GTL_BSR_VALIDATE(object) (object).Validate()
#else
#define GTL_BSR_VALIDATE(object)
#endif

namespace gtl
{
    template <typename UInteger>
    class BSRational
    {
    public:
        // Allow access to the BSNumber type associated with BSRational. This
        // avoids having to use templates with parameters both for a BSNumber
        // and a BSRational type.
        using BSN = BSNumber<UInteger>;

        // Construction and destruction. The default constructor generates
        // the zero BSRational. The constructors that take only numerators
        // set the denominators to one.
        BSRational()
            :
            GTL_BSR_INITIALIZE_NUMER(0.0)
        {
            GTL_BSR_VALIDATE(*this);
        }

        BSRational(float numerator)
            :
            GTL_BSR_INITIALIZE_NUMER(numerator)
        {
            GTL_BSR_VALIDATE(*this);
        }

        BSRational(double numerator)
            :
            GTL_BSR_INITIALIZE_NUMER(numerator)
        {
            GTL_BSR_VALIDATE(*this);
        }

        BSRational(std::int32_t numerator)
            :
            GTL_BSR_INITIALIZE_NUMER(numerator)
        {
            GTL_BSR_VALIDATE(*this);
        }

        BSRational(std::uint32_t numerator)
            :
            GTL_BSR_INITIALIZE_NUMER(numerator)
        {
            GTL_BSR_VALIDATE(*this);
        }

        BSRational(std::int64_t numerator)
            :
            GTL_BSR_INITIALIZE_NUMER(numerator)
        {
            GTL_BSR_VALIDATE(*this);
        }

        BSRational(std::uint64_t numerator)
            :
            GTL_BSR_INITIALIZE_NUMER(numerator)
        {
            GTL_BSR_VALIDATE(*this);
        }

        BSRational(BSNumber<UInteger> const& numerator)
            :
            GTL_BSR_INITIALIZE_NUMER(numerator)
        {
            GTL_BSR_VALIDATE(*this);
        }

        BSRational(float numerator, float denominator)
            :
            GTL_BSR_INITIALIZE_NUMER_DENOM(float, numerator, denominator)
        {
            EnsuredPositiveDenominator();
            GTL_BSN_UPDATE_VALUE(*this);
            GTL_BSR_VALIDATE(*this);
        }

        BSRational(double numerator, double denominator)
            :
            GTL_BSR_INITIALIZE_NUMER_DENOM(double, numerator, denominator)
        {
            EnsuredPositiveDenominator();
            GTL_BSN_UPDATE_VALUE(*this);
            GTL_BSR_VALIDATE(*this);
        }

        BSRational(std::int32_t numerator, std::int32_t denominator)
            :
            GTL_BSR_INITIALIZE_NUMER_DENOM(std::int32_t, numerator, denominator)
        {
            EnsuredPositiveDenominator();
            GTL_BSN_UPDATE_VALUE(*this);
            GTL_BSR_VALIDATE(*this);
        }

        BSRational(std::uint32_t numerator, std::uint32_t denominator)
            :
            GTL_BSR_INITIALIZE_NUMER_DENOM(std::uint32_t, numerator, denominator)
        {
            EnsuredPositiveDenominator();
            GTL_BSN_UPDATE_VALUE(*this);
            GTL_BSR_VALIDATE(*this);
        }

        BSRational(std::int64_t numerator, std::int64_t denominator)
            :
            GTL_BSR_INITIALIZE_NUMER_DENOM(std::int64_t, numerator, denominator)
        {
            EnsuredPositiveDenominator();
            GTL_BSN_UPDATE_VALUE(*this);
            GTL_BSR_VALIDATE(*this);
        }

        BSRational(std::uint64_t numerator, std::uint64_t denominator)
            :
            GTL_BSR_INITIALIZE_NUMER_DENOM(std::uint64_t, numerator, denominator)
        {
            EnsuredPositiveDenominator();
            GTL_BSN_UPDATE_VALUE(*this);
            GTL_BSR_VALIDATE(*this);
        }

        BSRational(BSNumber<UInteger> const& numerator, BSNumber<UInteger> const& denominator)
            :
            GTL_BSR_INITIALIZE_NUMER_DENOM(BSNumber<UInteger>, numerator, denominator)
        {
            EnsuredPositiveDenominator();

            // Set the exponent of the denominator to zero, but you can do so
            // only by modifying the biased exponent. Adjust the numerator
            // accordingly. This prevents large growth of the exponents in
            // both numerator and denominator simultaneously.
            mNumerator.mBiasedExponent -= mDenominator.GetExponent();
            std::int32_t numDBits = static_cast<std::int32_t>(mDenominator.GetUInteger().GetNumBits()) - 1;
            mDenominator.mBiasedExponent = -numDBits;

            GTL_BSN_UPDATE_VALUE(*this);
            GTL_BSR_VALIDATE(*this);
        }

        // The number must be of the form "x" or "+x" or "-x", where x is a
        // nonnegative integer with nonzero leading digit. Or it must be of
        // the form "x.y" or "+x.y" or "-x.y", where x is a nonnegative
        // integer with nonzero leading digit and y is an nonnegative integer.
        BSRational(std::string const& number)
        {
            GTL_ARGUMENT_ASSERT(
                number.size() > 0,
                "Number must be specified.");

            // Get the leading '+' or '-' if it exists.
            std::string fpNumber{};
            std::int32_t sign{};
            if (number[0] == '+')
            {
                fpNumber = number.substr(1);
                sign = +1;
                GTL_ARGUMENT_ASSERT(
                    fpNumber.size() > 1,
                    "Size must be larger than 1.");
            }
            else if (number[0] == '-')
            {
                fpNumber = number.substr(1);
                sign = -1;
                GTL_ARGUMENT_ASSERT(
                    fpNumber.size() > 1,
                    "Size must be larger than 1.");
            }
            else
            {
                fpNumber = number;
                sign = +1;
            }

            std::size_t decimal = fpNumber.find('.');
            if (decimal != std::string::npos)
            {
                if (decimal > 0)
                {
                    if (decimal < fpNumber.size())
                    {
                        // The number is "x.y".
                        std::string intString = fpNumber.substr(0, decimal);
                        auto intPart = BSNumber<UInteger>::ConvertToInteger(intString);
                        std::string frcString = fpNumber.substr(decimal + 1);
                        BSRational frcPart = ConvertToFraction(frcString);
                        mNumerator = intPart * frcPart.mDenominator + frcPart.mNumerator;
                        mDenominator = frcPart.mDenominator;
                    }
                    else
                    {
                        // The number is "x.".
                        std::string intString = fpNumber.substr(0, fpNumber.size() - 1);
                        mNumerator = BSNumber<UInteger>::ConvertToInteger(intString);
                        mDenominator = 1;
                    }
                }
                else
                {
                    // The number is ".y".
                    std::string frcString = fpNumber.substr(1);
                    BSRational frcPart = ConvertToFraction(frcString);
                    mNumerator = frcPart.mNumerator;
                    mDenominator = frcPart.mDenominator;
                }
            }
            else
            {
                // The number is "x".
                mNumerator = BSNumber<UInteger>::ConvertToInteger(fpNumber);
                mDenominator = 1;
            }
            mNumerator.mSign = sign;

            GTL_BSN_UPDATE_VALUE(*this);
            GTL_BSR_VALIDATE(*this);
        }

        BSRational(const char* number)
            :
            BSRational(std::string(number))
        {
        }

        ~BSRational() = default;

        // Copy semantics.
        BSRational(BSRational const& other)
        {
            *this = other;
        }

        BSRational& operator=(BSRational const& other)
        {
            mNumerator = other.mNumerator;
            mDenominator = other.mDenominator;
#if defined(GTL_BINARY_SCIENTIFIC_SHOW_DOUBLE)
            mValue = other.mValue;
#endif
            return *this;
        }

        // Move semantics.
        BSRational(BSRational&& other) noexcept
        {
            *this = std::move(other);
        }

        BSRational& operator=(BSRational&& other) noexcept
        {
            mNumerator = std::move(other.mNumerator);
            mDenominator = std::move(other.mDenominator);
#if defined(GTL_BINARY_SCIENTIFIC_SHOW_DOUBLE)
            mValue = other.mValue;
            other.mValue = 0.0;
#endif
            return *this;
        }

        // Implicit conversions. These always use the default rounding mode,
        // round-to-nearest-ties-to-even.
        operator float() const
        {
            return ConvertTo<IEEEBinary32>();
        }

        operator double() const
        {
            return ConvertTo<IEEEBinary64>();
        }

        operator std::size_t() const
        {
            return static_cast<std::size_t>(ConvertTo<IEEEBinary64>());
        }

        // Member access.
        //
        // The comments for BSNumber<UInteger>::SetSign apply as well to
        // BSRational<UInteger>::SetSign.

        inline void SetSign(std::int32_t sign)
        {
            mNumerator.mSign = sign;
        }

        inline std::int32_t GetSign() const
        {
            return mNumerator.GetSign();
        }

        inline void Negate()
        {
            mNumerator.Negate();
            GTL_BSR_UPDATE_VALUE(*this);
        }

        inline BSNumber<UInteger> const& GetNumerator() const
        {
            return mNumerator;
        }


        // TODO: Allow this?
        inline BSNumber<UInteger>& GetNumerator()
        {
            return mNumerator;
        }

        inline BSNumber<UInteger> const& GetDenominator() const
        {
            return mDenominator;
        }

        // TODO: Allow this?
        inline BSNumber<UInteger>& GetDenominator()
        {
            return mDenominator;
        }

        // Comparisons.
        bool operator==(BSRational const& other) const
        {
            // Do inexpensive sign tests first for optimum performance.
            if (mNumerator.mSign != other.mNumerator.mSign)
            {
                return false;
            }
            if (mNumerator.mSign == 0)
            {
                // The numbers are both zero.
                return true;
            }

            return mNumerator * other.mDenominator == mDenominator * other.mNumerator;
        }

        bool operator!=(BSRational const& other) const
        {
            return !operator==(other);
        }

        bool operator< (BSRational const& other) const
        {
            // Do inexpensive sign tests first for optimum performance.
            if (mNumerator.mSign > 0)
            {
                if (other.mNumerator.mSign <= 0)
                {
                    return false;
                }
            }
            else if (mNumerator.mSign == 0)
            {
                return other.mNumerator.mSign > 0;
            }
            else if (mNumerator.mSign < 0)
            {
                if (other.mNumerator.mSign >= 0)
                {
                    return true;
                }
            }

            return mNumerator * other.mDenominator < mDenominator * other.mNumerator;
        }

        bool operator<=(BSRational const& other) const
        {
            return !(other < *this);
        }

        bool operator> (BSRational const& other) const
        {
            return other < *this;
        }

        bool operator>=(BSRational const& other) const
        {
            return !operator<(other);
        }

        // Unary operations.
        BSRational operator+() const
        {
            return *this;
        }

        BSRational operator-() const
        {
            return BSRational(-mNumerator, mDenominator);
        }

        // Arithmetic.
        BSRational operator+(BSRational const& r) const
        {
            BSNumber<UInteger> product0 = mNumerator * r.mDenominator;
            BSNumber<UInteger> product1 = mDenominator * r.mNumerator;
            BSNumber<UInteger> numerator = product0 + product1;

            // Complex expressions can lead to 0/denom, where denom is not 1.
            if (numerator.mSign != 0)
            {
                BSNumber<UInteger> denominator = mDenominator * r.mDenominator;
                return BSRational(numerator, denominator);
            }
            else
            {
                return BSRational(0);
            }
        }

        BSRational operator-(BSRational const& r) const
        {
            BSNumber<UInteger> product0 = mNumerator * r.mDenominator;
            BSNumber<UInteger> product1 = mDenominator * r.mNumerator;
            BSNumber<UInteger> numerator = product0 - product1;

            // Complex expressions can lead to 0/denom, where denom is not 1.
            if (numerator.mSign != 0)
            {
                BSNumber<UInteger> denominator = mDenominator * r.mDenominator;
                return BSRational(numerator, denominator);
            }
            else
            {
                return BSRational(0);
            }
        }

        BSRational operator*(BSRational const& r) const
        {
            BSNumber<UInteger> numerator = mNumerator * r.mNumerator;

            // Complex expressions can lead to 0/denom, where denom is not 1.
            if (numerator.mSign != 0)
            {
                BSNumber<UInteger> denominator = mDenominator * r.mDenominator;
                return BSRational(numerator, denominator);
            }
            else
            {
                return BSRational(0);
            }
        }

        BSRational operator/(BSRational const& r) const
        {
            GTL_ARGUMENT_ASSERT(
                r.mNumerator.mSign != 0,
                "Divisor is zero.");

            BSNumber<UInteger> numerator = mNumerator * r.mDenominator;

            // Complex expressions can lead to 0/denom, where denom is not 1.
            if (numerator.mSign != 0)
            {
                BSNumber<UInteger> denominator = mDenominator * r.mNumerator;
                if (denominator.mSign < 0)
                {
                    numerator.mSign = -numerator.mSign;
                    denominator.mSign = 1;
                }
                return BSRational(numerator, denominator);
            }
            else
            {
                return BSRational(0);
            }
        }

        BSRational& operator+=(BSRational const& r)
        {
            *this = operator+(r);
            return *this;
        }

        BSRational& operator-=(BSRational const& r)
        {
            *this = operator-(r);
            return *this;
        }

        BSRational& operator*=(BSRational const& r)
        {
            *this = operator*(r);
            return *this;
        }

        BSRational& operator/=(BSRational const& r)
        {
            *this = operator/(r);
            return *this;
        }

        // Support for conversion from an arbitrary-precision BSRational to a
        // user-specified-precision BSNumber. The rounding mode is one of the
        // enumerations of APRoundingMode defined in BSNumber.h:
        //   APRoundingMode::TO_NEAREST:  round to nearest ties to even
        //   APRoundingMode::DOWNWARD:    round towards negative infinity
        //   APRoundingMode::TOWARD_ZERO: round towards zero
        //   APRoundingMode::UPWARD:      round towards positive infinity
        void Convert(
            std::size_t precision,
            APRoundingMode roundingMode,
            BSNumber<UInteger>& output) const
        {
            auto const& input = *this;

            // The number zero needs no conversion.
            if (input.GetSign() == 0)
            {
                output = BSNumber<UInteger>(0);
                return;
            }

            // Only the numerator needs to be converted when the denominator is 1.
            if (input.GetDenominator() == BSNumber<UInteger>(1))
            {
                input.GetNumerator().Convert(precision, roundingMode, output);
                return;
            }

            // Throw an exception when the precision is invalid. 
            GTL_ARGUMENT_ASSERT(
                precision > 0,
                "Precision must be positive.");

            std::size_t const maxNumBlocks = UInteger::GetMaxNumBlocks();
            std::size_t const numPrecBlocks = (precision + 31) / 32;
            GTL_ARGUMENT_ASSERT(
                numPrecBlocks < maxNumBlocks,
                "The maximum precision has been exceeded.");

            // The ratio is abstractly of the form n/d = (1.u*2^p)/(1.v*2^q).
            // Convert it to the form
            //   (1.u/1.v)*2^{p-q}, if 1.u >= 1.v
            //   2*(1.u/1.v)*2^{p-q-1} if 1.u < 1.v
            // where the numbers are in the interval [1,2).
            BSNumber<UInteger> n = input.GetNumerator();
            BSNumber<UInteger> d = input.GetDenominator();
            std::int32_t sign = n.GetSign() * d.GetSign();
            n.SetSign(1);
            d.SetSign(1);
            std::int32_t outExponent = n.GetExponent() - d.GetExponent();  // p-q
            n.SetExponent(0);
            d.SetExponent(0);
            if (n < d)
            {
                n.SetExponent(n.GetExponent() + 1);
                --outExponent;
            }

            // Let p be the precision. At this time, n/d = 1.c in [1,2). Define
            // the sequence of bits w = 1c = w_{p-1} w_{p-2} ... w_0 r, where
            // w_{p-1} = 1. The bits r after w_0 are used for rounding based on
            // the user-specified rounding mode.

            // Extract p bits for w, the leading bit guaranteed to be 1 and
            // occurring at index (1 << (precision-1)).
            BSNumber<UInteger> one(1), two(2);
            UInteger& outW = output.GetUInteger();
            outW.SetNumBits(precision);
            outW.SetAllBitsToZero();
            std::size_t const outNumBlocks = outW.GetNumBlocks();
            std::size_t const precisionM1 = static_cast<std::size_t>(precision - 1);
            std::uint32_t const outLeading = static_cast<std::uint32_t>(precisionM1 % 32);
            std::uint32_t outMask = (1u << outLeading);
            auto& outBits = outW.GetBits();
            std::size_t outCurrent = static_cast<std::size_t>(outNumBlocks - 1);

            BSNumber<UInteger> diff{};
            std::int32_t lastBit = -1;
            for (std::size_t i = precisionM1; i != std::numeric_limits<std::size_t>::max(); --i)
            {
                diff = n - d;
                if (diff.GetSign() < 0)
                {
                    // n = 2 * n
                    n = std::move(std::ldexp(n, 1));
                    lastBit = 0;
                }
                else if (diff.GetSign() > 0)
                {
                    // n = 2 * (n - d)
                    n = std::move(std::ldexp(diff, 1));
                    outBits[outCurrent] |= outMask;
                    lastBit = 1;
                }
                else  // diff = 0
                {
                    // The ratio n/d is 1, so the current bit is 1. The
                    // remaining bits are all zero and no rounding will occur.
                    outBits[outCurrent] |= outMask;
                    lastBit = 0;
                    n = BSNumber<UInteger>(0);
                    break;
                }

                if (outMask == 0x00000001u)
                {
                    --outCurrent;
                    outMask = 0x80000000u;
                }
                else
                {
                    outMask >>= 1;
                }
            }

            if (n.GetSign() != 0)
            {
                // At this point as a sequence of bits, the remainder is r = n/d =
                // r0 r1 ... . Round the extracted bits based on the specified
                // rounding mode.
                if (roundingMode == APRoundingMode::TO_NEAREST)
                {
                    n = n - d;
                    if (n.GetSign() > 0 || (n.GetSign() == 0 && lastBit == 1))
                    {
                        // round up
                        outExponent += UIntegerALU32<UInteger>::RoundUp(outW);
                    }
                    // else round down, equivalent to truncating the r bits
                }
                else if (roundingMode == APRoundingMode::UPWARD)
                {
                    if (n.GetSign() > 0 && sign > 0)
                    {
                        // round up
                        outExponent += UIntegerALU32<UInteger>::RoundUp(outW);
                    }
                    // else round down, equivalent to truncating the r bits
                }
                else if (roundingMode == APRoundingMode::DOWNWARD)
                {
                    if (n.GetSign() > 0 && sign < 0)
                    {
                        // Round down. This is the round-up operation applied to
                        // w, but the final sign is negative which amounts to
                        // rounding down.
                        outExponent += UIntegerALU32<UInteger>::RoundUp(outW);
                    }
                    // else round down, equivalent to truncating the r bits
                }
                else if (roundingMode != APRoundingMode::TOWARD_ZERO)
                {
                    // Currently, no additional implementation-dependent modes
                    // are supported for rounding.
                    GTL_ARGUMENT_ERROR(
                        "Implementation-dependent rounding mode not supported.");
                }
                // else roundingMode == APRoundingMode::TOWARD_ZERO. Truncate
                // the r bits, which requires no additional work.
            }

            // Shift the bits if necessary to obtain the invariant that BSNumber
            // objects have bit patterns that are odd integers.
            if (outW.GetNumBits() > 0 && (outW.GetBits()[0] & 1u) == 0)
            {
                UInteger temp = outW;
                outExponent += UIntegerALU32<UInteger>::ShiftRightToOdd(temp, outW);
            }

            // Do not use SetExponent(outExponent) at this step. The number of
            // requested bits is 'precision' but outW.GetNumBits() will be
            // different when round-up occurs, and SetExponent accesses
            // outW.GetNumBits().
            output.SetSign(sign);
            output.SetBiasedExponent(outExponent - static_cast<int32_t>(precisionM1));
            GTL_BSN_UPDATE_VALUE(output);
            GTL_BSR_VALIDATE(output);
        }

        // This conversion is used to avoid having to expose BSNumber in the
        // APConversion class as well as other places where BSRational<UInteger>
        // is passed via a template parameter named Rational.
        void Convert(
            std::size_t precision,
            APRoundingMode roundingMode,
            BSRational<UInteger>& output) const
        {
            BSNumber<UInteger> temp{};
            Convert(precision, roundingMode, temp);
            output = BSRational<UInteger>(temp);
        }

        // Convert to 'float' or 'double' using the specified rounding mode. NOTE:
        // The rounding is not the same as that of the implicit typecasts using
        // operator(). This conversion function rounds to a precision of 24 bits
        // for 'float' and 53 bits for 'double'. There is no concept of the
        // output of the internal Convert function being normal or subnormal.
        template <typename FPType>
        void Convert(
            APRoundingMode roundingMode,
            FPType& output) const
        {
            static_assert(
                std::is_floating_point<FPType>::value,
                "Invalid floating-point type.");

            BSNumber<UInteger> number{};
            Convert(std::numeric_limits<FPType>::digits, roundingMode, number);
            output = static_cast<FPType>(number);
        }

        // Streaming support. The stream objects should be created using
        // std::ios::binary. The return value is 'true' iff the operation
        // is successful.
        bool Write(std::ostream& output) const
        {
            return mNumerator.Write(output) && mDenominator.Write(output);
        }

        bool Read(std::istream& input)
        {
            return mNumerator.Read(input) && mDenominator.Read(input);
        }

#if defined(GTL_BINARY_SCIENTIFIC_VALIDATE)
        void Validate() const
        {
            mNumerator.Validate();
            mDenominator.Validate();
            GTL_RUNTIME_ASSERT(
                mDenominator.mSign > 0,
                "The denominator must be positive.");
        }
#endif

#if defined(GTL_BINARY_SCIENTIFIC_SHOW_DOUBLE)
    public:
        // In the debugger watch window, mValue shows up first.
        double mValue;
#endif

    private:
        void EnsuredPositiveDenominator()
        {
            GTL_ARGUMENT_ASSERT(
                mDenominator.mSign != 0,
                "Denominator is zero.");

            if (mDenominator.mSign < 0)
            {
                mNumerator.mSign = -mNumerator.mSign;
                mDenominator.mSign = 1;
            }
        }

        // Helper for converting a string to a BSRational, where the string
        // is the fractional part "y" of the string "x.y".
        static BSRational ConvertToFraction(std::string const& number)
        {
            GTL_ARGUMENT_ASSERT(
                number.find_first_not_of("0123456789") == std::string::npos,
                "Incorrect number format.");

            BSRational y(0), ten(10), pow10(10);
            for (std::size_t i = 0; i < number.size(); ++i)
            {
                std::int32_t digit = static_cast<std::int32_t>(number[i]) - static_cast<std::int32_t>('0');
                if (digit > 0)
                {
                    y += BSRational(digit) / pow10;
                }
                pow10 *= ten;
            }

            GTL_BSN_UPDATE_VALUE(y);
            GTL_BSR_VALIDATE(y);
            return y;
        }

        // Support for conversions from BSRational to floating-point numbers.
        template <typename IEEE>
        typename IEEE::FloatType ConvertTo() const
        {
            std::int32_t sign = GetSign();
            if (sign == 0)
            {
                return static_cast<typename IEEE::FloatType>(0);
            }

            if (mDenominator == BSNumber<UInteger>(1))
            {
                return static_cast<typename IEEE::FloatType>(mNumerator);
            }

            // The rational number is n/d = (1.u*2^p)/(1.v*2^q). Convert it to
            // (1.u/1.v)*2^{p-q} when 1.u >= 1.v or to (2*(1.u)/1.v)*2^{p-q-1}
            // when 1.u < 1.v. In either case, the number is 1.w * 2^r for 1.w
            // in [1,2).
            BSNumber<UInteger> n = mNumerator;
            BSNumber<UInteger> d = mDenominator;
            n.mSign = 1;
            d.mSign = 1;;
            std::int32_t exponent = n.GetExponent() - d.GetExponent();  // p-q
            n.SetExponent(0);
            d.SetExponent(0);
            if (n < d)
            {
                n.SetExponent(1);
                --exponent;
            }

            // The conversions use round-to-nearest-ties-to-even semantics.
            typename IEEE::UIntType t{};
            std::int32_t e{};
            if (exponent < IEEE::MIN_EXPONENT)
            {
                // When exponent = MIN_EXPONENT - 1, the second term of the OR
                // is equivalent to 1.w * 2^r = 1.0*2^{MIN_EXPONENT-1} and
                // therefore to n/d = 1.
                if (exponent < IEEE::MIN_EXPONENT - 1 || n == d)
                {
                    // Round to zero.
                    e = 0;
                    t = 0;
                }
                else
                {
                    // Round to min subnormal.
                    e = 0;
                    t = 1;
                }
            }
            else if (exponent < IEEE::MIN_SUB_EXPONENT)
            {
                t = GetTrailing<IEEE>(n, d, exponent - IEEE::MIN_EXPONENT + 1);
                if (t & IEEE::SUP_TRAILING)
                {
                    // Leading NUM_SIGNIFICAND_BITS bits were all 1, so round
                    // to min normal.
                    e = 1;
                    t = 0;
                }
                else
                {
                    e = 0;
                }
            }
            else if (exponent <= IEEE::EXPONENT_BIAS)
            {
                e = exponent + IEEE::EXPONENT_BIAS;
                t = GetTrailing<IEEE>(n, d, IEEE::NUM_SIGNIFICAND_BITS);
                if (t & (IEEE::SUP_TRAILING << 1))
                {
                    // Carry-out occurred, so increase exponent by 1 and shift
                    // right to compensate.
                    ++e;
                    t >>= 1;
                }
                // Eliminate the leading 1 (implied for normals).
                t &= ~IEEE::SUP_TRAILING;
            }
            else
            {
                // Set to infinity.
                e = IEEE::MAX_BIASED_EXPONENT;
                t = 0;
            }

            IEEE x((sign < 0 ? 1 : 0), static_cast<typename IEEE::UIntType>(e), t);
            return x.number;
        }

        template <typename IEEE>
        static typename IEEE::UIntType GetTrailing(BSNumber<UInteger>& n,
            BSNumber<UInteger> const& d, std::int32_t numBits)
        {
            // On input n/d is in [1,2). Extract the required bits of the
            // trailing significand one bit at a time.
            std::uint64_t mask = (1ull << static_cast<std::uint32_t>(numBits - 1));
            std::uint64_t trailing = 0ull;
            BSNumber<UInteger> diff{};
            for (std::int32_t i = 0; i < numBits; ++i)
            {
                diff = n - d;
                if (diff.GetSign() < 0)
                {
                    // n = 2 * n
                    n = std::move(std::ldexp(n, 1));
                }
                else if (diff.GetSign() > 0)
                {
                    // n = 2 * (n - d)
                    n = std::move(std::ldexp(diff, 1));
                    trailing |= mask;
                }
                else  // diff = 0
                {
                    // The ratio n/d is 1, so the current bit is 1. The
                    // remaining bits are all zero and no rounding occurs.
                    trailing |= mask;
                    return static_cast<typename IEEE::UIntType>(trailing);
                }

                mask >>= 1u;
            }

            // Apply round-to-nearest-ties-to-even. For a remainder 0.r, the
            // rule is to round down when 0.r < 1/2 or when 0.r = 1/2 and the
            // least significant bit of 'trailing' is 0. The rule is to round
            // up when 0.r > 1/2 or when 0.r = 1/2 and the least significant
            // bit of 'trailing' is 1.
            //
            // Let the current trailing value have least significant bit t0 =
            // (trailing & 1). Let the remainder be 0.r = 0.r0 r1 ... .
            // Repeating/ the previous loop body one more time to determine
            // the next bit r0, compute diff = n-d. If diff < 0, then r0 is 0
            // in which case 0.r < 1/2 and a round-down is applied. If
            // diff = 0, then r0 is 1 and the remaining bits are 0 in which
            // case 0.r = 1/2. A round-down occurs when t0 = 0 or a round-up
            // occurs when t0 = 1. If diff > 0, then r0 is 1 and the remaining
            // bits are not all zero in which case 0.r > 1/2 and a round-up
            // occurs. The following block of code summarizes these conditions
            // concisely.
            diff = n - d;
            if (diff.GetSign() > 0 || (diff.GetSign() == 0 && (trailing & 1ull) != 0ull))
            {
                // Round up. The caller determines whether there is a
                // carry-out into the bit before the most significant bit of
                // the actual trailing significand. If there is, the
                // appropriate adjustment is made to the exponent of the
                // floating-point number.
                ++trailing;
            }
            // else round down (in which case 'trailing' is unchanged).

            return static_cast<typename IEEE::UIntType>(trailing);
        }

        BSNumber<UInteger> mNumerator, mDenominator;

    private:
        friend class UnitTestBSRational;
    };
}

namespace std
{
    // TODO: Allow for implementations of the math functions in which a
    // specified precision is used when computing the result.

    template <typename UInteger>
    inline gtl::BSRational<UInteger> abs(gtl::BSRational<UInteger> const& x)
    {
        return (x.GetSign() >= 0 ? x : -x);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> acos(gtl::BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = std::acos(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> acosh(gtl::BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = std::acosh(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> asin(gtl::BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = std::asin(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> asinh(gtl::BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = std::asinh(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> atan(gtl::BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = std::atan(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> atanh(gtl::BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = std::atanh(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> atan2(gtl::BSRational<UInteger> const& y, gtl::BSRational<UInteger> const& x)
    {
        double dy = static_cast<double>(y);
        double dx = static_cast<double>(x);
        double result = std::atan2(dy, dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    // TODO: Implement an exact ceil function.
    template <typename UInteger>
    inline gtl::BSRational<UInteger> ceil(gtl::BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = std::ceil(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> cos(gtl::BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = std::cos(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> cosh(gtl::BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = std::cosh(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> exp(gtl::BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = std::exp(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> exp2(gtl::BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = std::exp2(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> fabs(gtl::BSRational<UInteger> const& x)
    {
        return (x.GetSign() >= 0 ? x : -x);
    }

    // TODO: Implement an exact floor function.
    template <typename UInteger>
    inline gtl::BSRational<UInteger> floor(gtl::BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = std::floor(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> fmod(gtl::BSRational<UInteger> const& x, gtl::BSRational<UInteger> const& y)
    {
        double dx = static_cast<double>(x);
        double dy = static_cast<double>(y);
        double result = std::fmod(dx, dy);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> fma(gtl::BSRational<UInteger> const& u,
        gtl::BSRational<UInteger> const& v, gtl::BSRational<UInteger> const& w)
    {
        return u * v + w;
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> frexp(gtl::BSRational<UInteger> const& x, std::int32_t* exponent)
    {
        gtl::BSRational<UInteger> result = x;
        auto& numer = result.GetNumerator();
        auto& denom = result.GetDenominator();
        std::int32_t e = numer.GetExponent() - denom.GetExponent();
        numer.SetExponent(0);
        denom.SetExponent(0);
        std::int32_t saveSign = numer.GetSign();
        numer.SetSign(1);
        if (numer >= denom)
        {
            ++e;
            numer.SetExponent(-1);
        }
        numer.SetSign(saveSign);
        *exponent = e;
        GTL_BSR_UPDATE_VALUE(result);
        return result;
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> ldexp(gtl::BSRational<UInteger> const& x, std::int32_t exponent)
    {
        gtl::BSRational<UInteger> result = x;
        std::int32_t biasedExponent = result.GetNumerator().GetBiasedExponent();
        biasedExponent += exponent;
        result.GetNumerator().SetBiasedExponent(biasedExponent);
        GTL_BSR_UPDATE_VALUE(result);
        return result;
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> log(gtl::BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = std::log(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> log2(gtl::BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = std::log2(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> log10(gtl::BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = std::log10(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> pow(gtl::BSRational<UInteger> const& x, gtl::BSRational<UInteger> const& y)
    {
        double dx = static_cast<double>(x);
        double dy = static_cast<double>(y);
        double result = std::pow(dx, dy);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> remainder(gtl::BSRational<UInteger> const& x, gtl::BSRational<UInteger> const& y)
    {
        double dx = static_cast<double>(x);
        double dy = static_cast<double>(y);
        double result = std::remainder(dx, dy);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> sin(gtl::BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = std::sin(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> sinh(gtl::BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = std::sinh(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> sqrt(gtl::BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = std::sqrt(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> tan(gtl::BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = std::tan(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline gtl::BSRational<UInteger> tanh(gtl::BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = std::tanh(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }
}

namespace gtl
{
    template <typename UInteger>
    inline BSRational<UInteger> atandivpi(BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = atandivpi(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline BSRational<UInteger> atan2divpi(BSRational<UInteger> const& y, BSRational<UInteger> const& x)
    {
        double dy = static_cast<double>(y);
        double dx = static_cast<double>(x);
        double result = atan2divpi(dy, dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline BSRational<UInteger> clamp(BSRational<UInteger> const& x, BSRational<UInteger> const& xmin, BSRational<UInteger> const& xmax)
    {
        return (x <= xmin ? xmin : (x >= xmax ? xmax : x));
    }

    template <typename UInteger>
    inline BSRational<UInteger> cospi(BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = cospi(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline BSRational<UInteger> exp10(BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = exp10(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline BSRational<UInteger> invsqrt(BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = invsqrt(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline std::int32_t isign(BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        return isign(dx);
    }

    template <typename UInteger>
    inline BSRational<UInteger> saturate(BSRational<UInteger> const& x)
    {
        if (x.GetSign() <= 0)
        {
            return BSRational<UInteger>(0);
        }
        else
        {
            BSRational<UInteger> one(1);
            if (x >= one)
            {
                return one;
            }
            else
            {
                return x;
            }
        }
    }

    template <typename UInteger>
    inline BSRational<UInteger> sign(BSRational<UInteger> const& x)
    {
        if (x.GetSign() > 0)
        {
            return BSRational<UInteger>(1);
        }
        else if (x.GetSign() < 0)
        {
            return BSRational<UInteger>(-1);
        }
        else
        {
            return BSRational<UInteger>(0);
        }
    }

    template <typename UInteger>
    inline BSRational<UInteger> sinpi(BSRational<UInteger> const& x)
    {
        double dx = static_cast<double>(x);
        double result = sinpi(dx);
        return static_cast<gtl::BSRational<UInteger>>(result);
    }

    template <typename UInteger>
    inline BSRational<UInteger> sqr(BSRational<UInteger> const& x)
    {
        return x * x;
    }

    // Compute u * v + w.
    template <typename UInteger>
    inline BSRational<UInteger> FMA(
        BSRational<UInteger> const& u,
        BSRational<UInteger> const& v,
        BSRational<UInteger> const& w)
    {
        return u * v + w;
    }

    // Sum of products (SOP) u*v+w*z.
    template <typename UInteger>
    inline BSRational<UInteger> RobustSOP(
        BSRational<UInteger> const& u,
        BSRational<UInteger> const& v,
        BSRational<UInteger> const& w,
        BSRational<UInteger> const& z)
    {
        return u * v + w * z;
    }

    // Difference of products (DOP) u*v-w*z.
    template <typename UInteger>
    inline BSRational<UInteger> RobustDOP(
        BSRational<UInteger> const& u,
        BSRational<UInteger> const& v,
        BSRational<UInteger> const& w,
        BSRational<UInteger> const& z)
    {
        return u * v - w * z;
    }

    // See the comments in TypeTraits.h about traits is_arbitrary_precision
    // and has_division_operator.
    template <typename UInteger>
    struct _is_arbitrary_precision_internal<BSRational<UInteger>> : std::true_type {};

    template <typename UInteger>
    struct _has_division_operator_internal<BSRational<UInteger>> : std::true_type {};
}
