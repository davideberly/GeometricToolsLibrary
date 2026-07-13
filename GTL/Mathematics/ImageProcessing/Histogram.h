// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2026.07.10

#pragma once

#include <GTL/Mathematics/Arithmetic/Constants.h>
#include <GTL/Utility/Exceptions.h>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

namespace gtl
{
    class Histogram
    {
    public:
        // In each Compute function you specify the number of buckets by
        // passing 'buckets' with the desired size().

        // The UIntType must be one of the integer types: std::uint8_t, std::uint16_t,
        // std::uint32_t or std::uint64_t. The samples are mapped directly to the
        // buckets (no transforming of data). Typically, the samples are in
        // the set of numbers {0,1,...,numBuckets-1}, but in the event of
        // out-of-range values, the histogram stores a count for those
        // numbers larger or equal to numBuckets (excessGreater).
        template <typename UIntType>
        static void Compute(std::vector<std::size_t>& buckets,
            std::vector<UIntType> const& samples, std::size_t& excessGreater)
        {
            static_assert(
                std::is_same<UIntType, std::uint8_t>::value ||
                std::is_same<UIntType, std::uint16_t>::value ||
                std::is_same<UIntType, std::uint32_t>::value ||
                std::is_same<UIntType, std::uint64_t>::value,
                "The sample type must be unsigned integer.");

            GTL_ARGUMENT_ASSERT(
                buckets.size() > 0 && samples.size() > 0,
                "Invalid input.");

            std::fill(buckets.begin(), buckets.end(), static_cast<std::size_t>(0));
            excessGreater = 0;
            for (std::size_t i = 0; i < samples.size(); ++i)
            {
                std::size_t sample = static_cast<std::size_t>(samples[i]);
                if (sample < buckets.size())
                {
                    ++buckets[sample];
                }
                else
                {
                    ++excessGreater;
                }
            }
        }

        // The IntType must be one of the integer types: std::int8_t, std::int16_t,
        // std::int32_t or std::int64_t. Define minsample = min_j(samples[j]).
        // 
        // If minSample >= 0, buckets[i] stores the number of samples with
        // value i >= 0; it is not required that samples[] have 0-valued
        // elements. In this case, minSample is set to 0.
        // 
        // If minSample < 0, buckets[i] stores the number of samples with
        // value i + minSample. Typically, the translated sample are in the
        // set of numbers {0,1,...,numBuckets-1}, but in the event of
        // out-of-range values, the histogram stores a count for those numbers
        // larger or equal to numBuckets (excessGreater). The minimum sample
        // amount (minSample) is nonpositive and returned for the caller to
        // query buckets[i] to determine the number of samples with value
        // i + minSample.
        template <typename IntType>
        static void Compute(std::vector<std::size_t>& buckets,
            std::vector<IntType> const& samples, IntType& minSample,
            std::size_t& excessGreater)
        {
            static_assert(
                std::is_same<IntType, std::int8_t>::value ||
                std::is_same<IntType, std::int16_t>::value ||
                std::is_same<IntType, std::int32_t>::value ||
                std::is_same<IntType, std::int64_t>::value,
                "The sample type must be signed integer.");

            GTL_ARGUMENT_ASSERT(
                buckets.size() > 0 && samples.size() > 0,
                "Invalid input.");

            minSample = std::numeric_limits<IntType>::max();
            for (std::size_t i = 0; i < samples.size(); ++i)
            {
                IntType sample = samples[i];
                if (sample < minSample)
                {
                    minSample = sample;
                }
            }
            if (minSample > 0)
            {
                minSample = 0;
            }

            std::fill(buckets.begin(), buckets.end(), static_cast<std::size_t>(0));
            excessGreater = 0;
            for (std::size_t i = 0; i < samples.size(); ++i)
            {
                std::size_t translatedSample = static_cast<std::size_t>(samples[i] + minSample);
                if (translatedSample < buckets.size())
                {
                    ++buckets[translatedSample];
                }
                else
                {
                    ++excessGreater;
                }
            }
        }

        // The FloatType must be a floating-point type: float, double or long
        // double. The samples are scaled to [0, numBuckets - 1].
        template <typename FloatType>
        static void Compute(std::vector<std::size_t>& buckets,
            std::vector<FloatType> const& samples)
        {
            static_assert(
                std::is_floating_point<FloatType>::value,
                "The sample type must be floating-point.");

            GTL_ARGUMENT_ASSERT(
                buckets.size() > 0 && samples.size() > 0,
                "Invalid input.");

            // Compute the extremes.
            FloatType smin = samples[0], smax = smin;
            for (std::size_t i = 1; i < samples.size(); ++i)
            {
                FloatType const& value = samples[i];
                if (value < smin)
                {
                    smin = value;
                }
                else if (value > smax)
                {
                    smax = value;
                }
            }

            // Map to the buckets.
            std::fill(buckets.begin(), buckets.end(), static_cast<std::size_t>(0));
            if (smin < smax)
            {
                // The image is not constant.
                std::size_t const maxIndex = buckets.size() - 1;
                FloatType numer = static_cast<FloatType>(maxIndex);
                FloatType denom = smax - smin;
                for (std::size_t i = 0; i < samples.size(); ++i)
                {
                    FloatType unit = (samples[i] - smin) / denom;
                    unit = std::max(unit, C_<FloatType>(0));
                    unit = std::min(unit, C_<FloatType>(1));
                    std::size_t index = static_cast<std::size_t>(numer * (samples[i] - smin));
                    index = std::min(index, maxIndex);
                    ++buckets[index];
                }
            }
            else
            {
                // The image is constant.
                buckets.resize(1);
                buckets[0] = samples.size();
            }
        }

        // In the following, define cdf(V) = sum_{i=0}^{V} bucket[i], where
        // 0 <= V < B and B is the number of buckets.  Define N = cdf(B-1),
        // which must be the number of pixels in the image.

        // Get the lower tail of the histogram. The returned index L has the
        // properties: cdf(L-1)/N < tailAmount and cdf(L)/N >= tailAmount.
        static std::size_t GetLowerTail(std::vector<std::size_t> const& buckets, double tailAmount)
        {
            std::size_t hSum = 0;
            for (std::size_t i = 0; i < buckets.size(); ++i)
            {
                hSum += buckets[i];
            }

            std::size_t hTailSum = static_cast<std::size_t>(tailAmount * static_cast<double>(hSum));
            std::size_t hLowerSum = 0;
            for (std::size_t i = 0; i < buckets.size(); ++i)
            {
                hLowerSum += buckets[i];
                if (hLowerSum >= hTailSum)
                {
                    return i;
                }
            }
            return buckets.size();
        }

        // Get the upper tail of the histogram. The returned index U has the
        // properties: cdf(U)/N >= 1-tailAmount and cdf(U+1) < 1-tailAmount.
        static std::size_t GetUpperTail(std::vector<std::size_t> const& buckets, double tailAmount)
        {
            std::size_t hSum = 0;
            for (std::size_t i = 0; i < buckets.size(); ++i)
            {
                hSum += buckets[i];
            }

            std::size_t hTailSum = static_cast<std::size_t>(tailAmount * static_cast<double>(hSum));
            std::size_t hUpperSum = 0;
            for (std::size_t i = 0, j = buckets.size() - 1; i < buckets.size(); ++i, --j)
            {
                hUpperSum += buckets[j];
                if (hUpperSum >= hTailSum)
                {
                    return j;
                }
            }
            return 0;
        }

        // Get the lower and upper tails of the histogram. The returned
        // indices are L and U, stored as pair (L,U) and have the properties:
        //   cdf(L-1)/N < tailAmount/2, cdf(L)/N >= tailAmount/2,
        //   cdf(U)/N >= 1-tailAmount/2, and cdf(U+1) < 1-tailAmount/2.
        static std::pair<std::size_t, std::size_t> GetTails(std::vector<std::size_t> const& buckets,
            double tailAmount)
        {
            std::pair<std::size_t, std::size_t> pair(buckets.size(), 0);
            std::size_t hSum = 0;
            for (std::size_t i = 0; i < buckets.size(); ++i)
            {
                hSum += buckets[i];
            }

            std::size_t hTailSum = static_cast<std::size_t>(0.5 * tailAmount * static_cast<double>(hSum));
            std::size_t hLowerSum = 0;
            for (std::size_t i = 0; i < buckets.size(); ++i)
            {
                hLowerSum += buckets[i];
                if (hLowerSum >= hTailSum)
                {
                    pair.first = i;
                    break;
                }
            }

            std::size_t hUpperSum = 0;
            for (std::size_t i = 0, j = buckets.size() - 1; i < buckets.size(); ++i, --j)
            {
                hUpperSum += buckets[i];
                if (hUpperSum >= hTailSum)
                {
                    pair.second = i;
                    break;
                }
            }

            return pair;
        }

    private:
        friend class UnitTestHistogram;
    };
}
