// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsPCH.h>
#include <GTL/Utility/Exceptions.h>
#include <GTL/Graphics/Resources/Buffers/VertexFormat.h>
using namespace gtl;

VertexFormat::VertexFormat()
    :
    mNumAttributes(0),
    mVertexSize(0),
    mAttributes{}
{
}

void VertexFormat::Reset()
{
    mNumAttributes = 0;
    mVertexSize = 0;
    for (int32_t i = 0; i < VAConstant::MAX_ATTRIBUTES; ++i)
    {
        mAttributes[i] = Attribute();
    }
}

void VertexFormat::Bind(VASemantic semantic, DFType type, uint32_t unit)
{
    // Validate the inputs.
    GTL_RUNTIME_ASSERT(
        0 <= mNumAttributes && mNumAttributes < VAConstant::MAX_ATTRIBUTES,
        "Exceeded maximum attributes.");

    if (semantic == VASemantic::COLOR)
    {
        GTL_RUNTIME_ASSERT(
            unit < VAConstant::MAX_COLOR_UNITS,
            "Invalid color unit.");
    }
    else if (semantic == VASemantic::TEXCOORD)
    {
        GTL_RUNTIME_ASSERT(
            unit < VAConstant::MAX_TCOORD_UNITS,
            "Invalid texture unit.");
    }
    else
    {
        GTL_RUNTIME_ASSERT(
            unit == 0,
            "Invalid semantic unit.");
    }

    // Accept the attribute.
    Attribute& attribute = mAttributes[mNumAttributes];
    attribute.semantic = semantic;
    attribute.type = type;
    attribute.unit = unit;
    attribute.offset = mVertexSize;
    ++mNumAttributes;

    // Advance the offset.
    mVertexSize += DataFormat::GetNumBytesPerStruct(type);
}

void VertexFormat::GetAttribute(int32_t i, VASemantic& semantic, DFType& type,
    uint32_t& unit, uint32_t& offset) const
{
    GTL_ARGUMENT_ASSERT(
        0 <= i && i < mNumAttributes,
        "Invalid index " + std::to_string(i) + ".");

    Attribute const& attribute = mAttributes[i];
    semantic = attribute.semantic;
    type = attribute.type;
    unit = attribute.unit;
    offset = attribute.offset;
}

int32_t VertexFormat::GetIndex(VASemantic semantic, uint32_t unit) const
{
    for (int32_t i = 0; i < mNumAttributes; ++i)
    {
        Attribute const& attribute = mAttributes[i];
        if (attribute.semantic == semantic && attribute.unit == unit)
        {
            return i;
        }
    }

    return -1;
}

DFType VertexFormat::GetType(int32_t i) const
{
    GTL_ARGUMENT_ASSERT(
        0 <= i && i < mNumAttributes,
        "Invalid index " + std::to_string(i) + ".");

    return mAttributes[i].type;
}

uint32_t VertexFormat::GetOffset(int32_t i) const
{
    GTL_ARGUMENT_ASSERT(
        0 <= i && i < mNumAttributes,
        "Invalid index " + std::to_string(i) + ".");

    return mAttributes[i].offset;
}
