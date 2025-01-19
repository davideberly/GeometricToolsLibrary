// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Shaders/Shader.h>
#include <functional>
using namespace gtl;

Shader::Shader(GraphicsObjectType type)
    :
    GraphicsObject(type),
    mNumXThreads(0),
    mNumYThreads(0),
    mNumZThreads(0)
{
}

int32_t Shader::Get(std::string const& name) const
{
    for (int32_t lookup = 0; lookup < NUM_LOOKUP_INDICES; ++lookup)
    {
        int32_t handle = 0;
        for (auto const& data : mData[lookup])
        {
            if (name == data.name)
            {
                return handle;
            }
            ++handle;
        }
    }
    return -1;
}

uint32_t Shader::GetConstantBufferSize(int32_t handle) const
{
    auto const& data = mData[ConstantBuffer::shaderDataLookup];
    GTL_ARGUMENT_ASSERT(
        0 <= handle && handle < static_cast<int32_t>(data.size()),
        "Invalid handle for object.");

    return data[handle].numBytes;
}

uint32_t Shader::GetConstantBufferSize(std::string const& name) const
{
    for (auto& data : mData[ConstantBuffer::shaderDataLookup])
    {
        if (name == data.name)
        {
            return data.numBytes;
        }
    }

    GTL_RUNTIME_ERROR(
        "Cannot find object " + name + ".");
}

uint32_t Shader::GetTextureBufferSize(int32_t handle) const
{
    auto const& data = mData[TextureBuffer::shaderDataLookup];
    GTL_ARGUMENT_ASSERT(
        0 <= handle && handle < static_cast<int32_t>(data.size()),
        "Invalid handle for object.");

    return data[handle].numBytes;
}

uint32_t Shader::GetTextureBufferSize(std::string const& name) const
{
    for (auto& data : mData[TextureBuffer::shaderDataLookup])
    {
        if (name == data.name)
        {
            return data.numBytes;
        }
    }

    GTL_RUNTIME_ERROR(
        "Cannot find object " + name + ".");
}

uint32_t Shader::GetStructuredBufferSize(int32_t handle) const
{
    auto const& data = mData[StructuredBuffer::shaderDataLookup];
    GTL_ARGUMENT_ASSERT(
        0 <= handle && handle < static_cast<int32_t>(data.size()),
        "Invalid handle for object.");

    return data[handle].numBytes;
}

uint32_t Shader::GetStructuredBufferSize(std::string const& name) const
{
    for (auto& data : mData[StructuredBuffer::shaderDataLookup])
    {
        if (name == data.name)
        {
            return data.numBytes;
        }
    }

    GTL_RUNTIME_ERROR(
        "Cannot find object " + name + ".");
}

void Shader::GetConstantBufferLayout(int32_t handle, BufferLayout& layout) const
{
    auto const& data = mData[ConstantBuffer::shaderDataLookup];
    GTL_ARGUMENT_ASSERT(
        0 <= handle && handle < static_cast<int32_t>(data.size()),
        "Invalid handle for object.");

    layout = mCBufferLayouts[handle];
}

void Shader::GetConstantBufferLayout(std::string const& name, BufferLayout& layout) const
{
    int32_t handle = 0;
    for (auto& data : mData[ConstantBuffer::shaderDataLookup])
    {
        if (name == data.name)
        {
            layout = mCBufferLayouts[handle];
            return;
        }
        ++handle;
    }

    GTL_RUNTIME_ERROR(
        "Cannot find object " + name + ".");
}

void Shader::GetTextureBufferLayout(int32_t handle, BufferLayout& layout) const
{
    auto const& data = mData[TextureBuffer::shaderDataLookup];
    GTL_ARGUMENT_ASSERT(
        0 <= handle && handle < static_cast<int32_t>(data.size()),
        "Invalid handle for object.");

    layout = mTBufferLayouts[handle];
}

void Shader::GetTextureBufferLayout(std::string const& name, BufferLayout& layout) const
{
    int32_t handle = 0;
    for (auto& data : mData[TextureBuffer::shaderDataLookup])
    {
        if (name == data.name)
        {
            layout = mTBufferLayouts[handle];
            return;
        }
        ++handle;
    }

    GTL_RUNTIME_ERROR(
        "Cannot find object " + name + ".");
}

void Shader::GetStructuredBufferLayout(int32_t handle, BufferLayout& layout) const
{
    auto const& data = mData[StructuredBuffer::shaderDataLookup];
    GTL_ARGUMENT_ASSERT(
        0 <= handle && handle < static_cast<int32_t>(data.size()),
        "Invalid handle for object.");

    layout = mSBufferLayouts[handle];
}

void Shader::GetStructuredBufferLayout(std::string const& name, BufferLayout& layout) const
{
    int32_t handle = 0;
    for (auto& data : mData[StructuredBuffer::shaderDataLookup])
    {
        if (name == data.name)
        {
            layout = mSBufferLayouts[handle];
            return;
        }
        ++handle;
    }

    GTL_RUNTIME_ERROR(
        "Cannot find object " + name + ".");
}
