// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Shaders/ProgramDefines.h>
using namespace gtl;

void ProgramDefines::Update(std::string const& name, std::string const& value)
{
    // If an item already exists with the specified name, update it.
    for (auto& definition : mDefinitions)
    {
        if (name == definition.first)
        {
            definition.second = value;
            return;
        }
    }

    // The item is new, so append it.
    mDefinitions.push_back(std::make_pair(name, value));
}

void ProgramDefines::Remove(std::string const& name)
{
    for (auto iter = mDefinitions.begin(); iter != mDefinitions.end(); ++iter)
    {
        if (name == iter->first)
        {
            mDefinitions.erase(iter);
            break;
        }
    }
}

void ProgramDefines::Clear()
{
    mDefinitions.clear();
}
