// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

// Construct definitions used by shader compilers.

#include <string>
#include <vector>

namespace gtl
{
    class ProgramDefines
    {
    public:
        // Construction.
        ProgramDefines() = default;

        // Set a definition.  Each is stored as a std::pair of std::string.
        // The first element of the pair is the 'name' and the second element
        // of the pair is the string representation of 'value'.
        template <typename T>
        inline void Set(std::string const& name, T value)
        {
            Update(name, std::to_string(value));
        }

        inline void Set(char const* name, std::string const& value)
        {
            Update(name, std::string(value));
        }

        inline void Set(char const* name, char const* value)
        {
            Update(std::string(name), std::string(value));
        }

        inline std::vector<std::pair<std::string, std::string>> const& Get() const
        {
            return mDefinitions;
        }

        // Remove definitions, which allows the ProgramDefines object to be
        // shared in a scope.
        void Remove(std::string const& name);
        void Clear();

    private:
        void Update(std::string const& name, std::string const& value);

        std::vector<std::pair<std::string, std::string>> mDefinitions;
    };

    // Specialization of Set(std::string const&, T value) for
    // T = char const*.
    template <>
    inline void ProgramDefines::Set(std::string const& name, char const* value)
    {
        Update(name, std::string(value));
    }

    // Specialization of Set(std::string const&, T value) for
    // T = std::string const&.
    template <>
    inline void ProgramDefines::Set(std::string const& name, std::string const& value)
    {
        Update(name, value);
    }
}
