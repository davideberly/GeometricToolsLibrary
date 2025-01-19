// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <cstdarg>
#include <string>
#include <vector>

namespace gtl
{
    class Environment
    {
    public:
        // Construction and destruction.
        Environment()
            :
            mDirectories{}
        {
        }

        ~Environment() = default;

        // Get the string corresponding to an environment variable.
        std::string GetVariable(std::string const& name) const;

        // Support for paths to locate files. For platform independence, use
        // "/" for the path separator. The input 'directory' will
        // automatically be appended with a trailing "/" if it does not end
        // in '/' or '\\'. The Insert*/Remove* functions return 'true' iff
        // the operation was successful.
        int32_t GetNumDirectories() const;
        std::string Get(int32_t i) const;
        bool Insert(std::string const& directory);
        bool Remove(std::string const& directory);
        void RemoveAll();

        // The GetPath function searches the list of directories and returns
        // the fully decorated file name if the file exists and can be opened
        // for reading.
        std::string GetPath(std::string const& name) const;

        // Get the path to the GTL folder. This is obtained from the GTL_PATH
        // environment variable. It is required when you want a guaranteed way
        // of finding application data/files in the GTL subtree.  If you call
        // this function and the environment variable does not exist, an
        // exception is thrown.
        std::string GetGTLPath() const;

    private:
        // The list of directories for GetPath to search.
        std::vector<std::string> mDirectories;
    };
}
