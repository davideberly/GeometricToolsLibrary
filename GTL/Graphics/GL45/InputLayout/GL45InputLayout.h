// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/GL45/Engine/GL45.h>
#include <GTL/Graphics/Resources/Buffers/VertexBuffer.h>
#include <array>
#include <cstdint>

namespace gtl
{
    class GL45InputLayout
    {
    public:
        // Construction and destruction.
        ~GL45InputLayout();
        GL45InputLayout(GLuint programHandle, GLuint vbufferHandle, VertexBuffer const* vbuffer);

        // Support for drawing geometric primitives.
        void Enable();
        void Disable();

    private:
        struct Attribute
        {
            Attribute()
                :
                semantic(VASemantic::NONE),
                numChannels(0),
                channelType(0),
                normalize(0),
                location(0),
                offset(0),
                stride(0)
            {
            }

            VASemantic semantic;
            GLint numChannels;
            GLint channelType;
            GLboolean normalize;
            GLint location;
            GLintptr offset;
            GLsizei stride;
        };

        GLuint mVBufferHandle;
        GLuint mVArrayHandle;
        int32_t mNumAttributes;
        std::array<Attribute, VAConstant::MAX_ATTRIBUTES> mAttributes;

        // Conversions from GTEngine values to GL45 values.
        static GLenum const msChannelType[];
    };
}
