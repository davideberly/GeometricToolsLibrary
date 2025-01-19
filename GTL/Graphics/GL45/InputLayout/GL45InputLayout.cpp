// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsGL45PCH.h>
#include <GTL/Utility/Exceptions.h>
#include <GTL/Graphics/GL45/InputLayout/GL45InputLayout.h>
#include <cstring>
using namespace gtl;

GL45InputLayout::~GL45InputLayout()
{
    glDeleteVertexArrays(1, &mVArrayHandle);
}

GL45InputLayout::GL45InputLayout(GLuint, GLuint vbufferHandle, VertexBuffer const* vbuffer)
    :
    mVBufferHandle(vbufferHandle),
    mVArrayHandle(0),
    mNumAttributes(0),
    mAttributes{}
{
    glGenVertexArrays(1, &mVArrayHandle);
    glBindVertexArray(mVArrayHandle);

    if (vbuffer)
    {
        VertexFormat const& format = vbuffer->GetFormat();
        mNumAttributes = format.GetNumAttributes();
        for (int32_t i = 0; i < mNumAttributes; ++i)
        {
            Attribute& attribute = mAttributes[i];

            DFType type{};
            uint32_t unit{}, offset{};
            format.GetAttribute(i, attribute.semantic, type, unit, offset);

            attribute.numChannels = static_cast<GLint>(
                DataFormat::GetNumChannels(type));
            attribute.channelType =
                msChannelType[DataFormat::GetChannelType(type)];
            attribute.normalize = static_cast<GLboolean>(
                DataFormat::ConvertChannel(type) ? 1 : 0);
            attribute.location = i;  // layouts must be zero-based sequential
            attribute.offset = static_cast<GLintptr>(offset);
            attribute.stride = static_cast<GLsizei>(format.GetVertexSize());

            glEnableVertexAttribArray(attribute.location);
            glBindVertexBuffer(i, mVBufferHandle, attribute.offset,
                attribute.stride);
            glVertexAttribFormat(attribute.location, attribute.numChannels,
                attribute.channelType, attribute.normalize, 0);
            glVertexAttribBinding(attribute.location, i);
        }
        glBindVertexArray(0);
    }
    else
    {
        GTL_RUNTIME_ERROR("Invalid inputs to GL45InputLayout constructor.");
    }
}

void GL45InputLayout::Enable()
{
    glBindVertexArray(mVArrayHandle);
}

void GL45InputLayout::Disable()
{
    glBindVertexArray(0);
}


GLenum const GL45InputLayout::msChannelType[] =
{
    GL_ZERO,                        // DF_UNSUPPORTED
    GL_BYTE,                        // DF_BYTE
    GL_UNSIGNED_BYTE,               // DF_UBYTE
    GL_SHORT,                       // DF_SHORT
    GL_UNSIGNED_SHORT,              // DF_USHORT
    GL_INT,                         // DF_INT
    GL_UNSIGNED_INT,                // DF_UINT
    GL_HALF_FLOAT,                  // DF_HALF_FLOAT
    GL_FLOAT,                       // DF_FLOAT
    GL_DOUBLE,                      // DF_DOUBLE
    GL_INT_2_10_10_10_REV,          // DF_INT_10_10_2
    GL_UNSIGNED_INT_2_10_10_10_REV, // DF_UINT_10_10_2
    GL_UNSIGNED_INT_10F_11F_11F_REV // DF_FLOAT_11_11_10
};
