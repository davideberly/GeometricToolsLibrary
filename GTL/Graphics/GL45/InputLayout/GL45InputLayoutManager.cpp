// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsGL45PCH.h>
#include <GTL/Utility/Exceptions.h>
#include <GTL/Graphics/GL45/InputLayout/GL45InputLayoutManager.h>
using namespace gtl;

GL45InputLayoutManager::~GL45InputLayoutManager()
{
    mMutex.lock();
    mMap.clear();
    mMutex.unlock();
}

GL45InputLayout* GL45InputLayoutManager::Bind(GLuint programHandle,
    GLuint vbufferHandle, VertexBuffer const* vbuffer)
{
    GTL_ARGUMENT_ASSERT(
        programHandle != 0,
        "Invalid input.");

    if (vbuffer)
    {
        mMutex.lock();
        VBPPair vbp(vbuffer, programHandle);
        auto iter = mMap.find(vbp);
        if (iter == mMap.end())
        {
            auto layout = std::make_shared<GL45InputLayout>(programHandle, vbufferHandle, vbuffer);
            iter = mMap.insert(std::make_pair(vbp, layout)).first;
        }
        GL45InputLayout* inputLayout = iter->second.get();
        mMutex.unlock();
        return inputLayout;
    }
    else
    {
        // A null vertex buffer is passed when an effect wants to bypass the
        // input assembler.
        return nullptr;
    }
}

bool GL45InputLayoutManager::Unbind(VertexBuffer const* vbuffer)
{
    GTL_ARGUMENT_ASSERT(
        vbuffer != nullptr,
        "Invalid input.");

    mMutex.lock();
    if (mMap.size() > 0)
    {
        std::vector<VBPPair> matches{};
        for (auto const& element : mMap)
        {
            if (vbuffer == element.first.first)
            {
                matches.push_back(element.first);
            }
        }

        for (auto const& match : matches)
        {
            mMap.erase(match);
        }
    }
    mMutex.unlock();
    return true;
}

bool GL45InputLayoutManager::Unbind(Shader const*)
{
    return true;
}

void GL45InputLayoutManager::UnbindAll()
{
    mMutex.lock();
    mMap.clear();
    mMutex.unlock();
}

bool GL45InputLayoutManager::HasElements() const
{
    mMutex.lock();
    bool hasElements = mMap.size() > 0;
    mMutex.unlock();
    return hasElements;
}
