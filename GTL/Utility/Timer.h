// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#pragma once

// Documentation for this class is
// https://www.geometrictools.com/Documentation/GTLUtility.pdf#Timer

#include <cstdint>
#include <chrono>

namespace gtl
{
    class Timer
    {
    public:
        // Construction of a high-resolution timer (64-bit).
        Timer()
        {
            Reset();
        }

        // Get the current time relative to the initial time.
        std::int64_t GetNanoseconds() const
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            return std::chrono::duration_cast<std::chrono::nanoseconds>(
                currentTime - mInitialTime).count();
        }

        std::int64_t GetMicroseconds() const
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            return std::chrono::duration_cast<std::chrono::microseconds>(
                currentTime - mInitialTime).count();
        }

        std::int64_t GetMilliseconds() const
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            return std::chrono::duration_cast<std::chrono::milliseconds>(
                currentTime - mInitialTime).count();
        }

        double GetSeconds() const
        {
            std::int64_t msecs = GetMilliseconds();
            return static_cast<double>(msecs) / 1000.0;
        }

        // Reset so that the current time is the initial time.
        void Reset()
        {
            mInitialTime = std::chrono::high_resolution_clock::now();
        }

    private:
        std::chrono::high_resolution_clock::time_point mInitialTime;
    };
}
