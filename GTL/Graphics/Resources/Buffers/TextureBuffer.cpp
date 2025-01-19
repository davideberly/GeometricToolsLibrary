// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Resources/Buffers/TextureBuffer.h>
#include <cstring>
using namespace gtl;

TextureBuffer::TextureBuffer(uint32_t format, uint32_t numElements, bool allowDynamicUpdate)
    :
    Buffer(numElements, DataFormat::GetNumBytesPerStruct(format), true),
    mFormat(format)
{
    mType = GT_TEXTURE_BUFFER;
    mUsage = (allowDynamicUpdate ? Usage::DYNAMIC_UPDATE : Usage::IMMUTABLE);
    std::memset(mData, 0, mNumBytes);
}

uint32_t TextureBuffer::GetFormat() const
{
    return mFormat;
}

bool TextureBuffer::HasMember(std::string const& name) const
{
    auto iter = std::find_if(mLayout.begin(), mLayout.end(),
        [&name](MemberLayout const& item){ return name == item.name; });
    return iter != mLayout.end();
}
