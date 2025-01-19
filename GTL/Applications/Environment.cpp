// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLApplicationsPCH.h>
#include <GTL/Utility/Exceptions.h>
#include <GTL/Applications/Environment.h>
#include <cstdlib>
#include <fstream>
using namespace gtl;

std::string Environment::GetVariable(std::string const& name) const
{
#if defined(GTL_USE_MSWINDOWS)
    size_t size;
    getenv_s(&size, nullptr, 0, name.c_str());
    if (size > 0)
    {
        std::vector<char> tmpvar(size);
        errno_t result = getenv_s(&size, tmpvar.data(), size, name.c_str());
        std::string var = (result == 0 ? std::string(tmpvar.data()) : "");
        return var;
    }
    else
    {
        return "";
    }
#elif defined(GTL_USE_LINUX)
    char const* variable = getenv(name.c_str());
    return variable ? std::string(variable) : std::string("");
#else
    return "";
#endif
}

int32_t Environment::GetNumDirectories() const
{
    return static_cast<int32_t>(mDirectories.size());
}

std::string Environment::Get(int32_t i) const
{
    GTL_ARGUMENT_ASSERT(
        0 <= i && i < static_cast<int32_t>(mDirectories.size()),
        "Invalid index.");

    return mDirectories[i];
}

bool Environment::Insert(std::string const& directory)
{
    GTL_ARGUMENT_ASSERT(
        directory.size() > 0,
        "Insert expects non-empty inputs.");

    for (auto& d : mDirectories)
    {
        if (directory == d || directory + "/" == d || directory + "\\" == d)
        {
            return false;
        }
    }

    // Ensure all directories are terminated with a slash.
    char lastChar = directory[directory.size() - 1];
    if (lastChar == '\\' || lastChar == '/')
    {
        mDirectories.push_back(directory);
    }
    else
    {
        mDirectories.push_back(directory + "/");
    }
    return true;
}

bool Environment::Remove(std::string const& directory)
{
    for (auto iter = mDirectories.begin(); iter != mDirectories.end(); ++iter)
    {
        if (directory == *iter)
        {
            mDirectories.erase(iter);
            return true;
        }
    }
    return false;
}

void Environment::RemoveAll()
{
    mDirectories.clear();
}

std::string Environment::GetPath(std::string const& name) const
{
    for (auto const& directory : mDirectories)
    {
        std::string decorated = directory + name;
        std::ifstream input(decorated, std::ios::binary);
        if (input)
        {
            input.close();
            return decorated;
        }
    }
    return "";
}

std::string Environment::GetGTLPath() const
{
    std::string path = GetVariable("GTL_PATH");
    GTL_RUNTIME_ASSERT(
        path != "",
        "You must create the environment variable GTL_PATH.");

    return path;
}
