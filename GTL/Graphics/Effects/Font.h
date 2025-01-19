// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Effects/TextEffect.h>
#include <GTL/Graphics/Resources/Buffers/VertexBuffer.h>
#include <GTL/Graphics/Resources/Buffers/IndexBuffer.h>

namespace gtl
{
    class Font
    {
        // Abstract base class.
    protected:
        // Construction.
        Font(std::shared_ptr<ProgramFactory> const& factory,
            uint32_t width, uint32_t height,
            uint8_t const* texels, float const* characterData,
            uint32_t maxMessageLength);
    public:
        virtual ~Font() = default;

        // Member access.
        inline std::shared_ptr<TextEffect> const& GetTextEffect() const
        {
            return mTextEffect;
        }

        inline std::shared_ptr<VertexBuffer> const& GetVertexBuffer() const
        {
            return mVertexBuffer;
        }

        inline std::shared_ptr<IndexBuffer> const& GetIndexBuffer() const
        {
            return mIndexBuffer;
        }

        int32_t GetHeight() const;
        int32_t GetWidth(std::string const& message) const;

        // Populate the vertex buffer for the specified string.
        void Typeset(int32_t viewportWidth, int32_t viewportHeight, int32_t x, int32_t y,
            Vector4<float> const& color, std::string const& message) const;

    protected:
        uint32_t mMaxMessageLength;
        std::shared_ptr<VertexBuffer> mVertexBuffer;
        std::shared_ptr<IndexBuffer> mIndexBuffer;
        std::shared_ptr<Texture2> mTexture;
        std::shared_ptr<TextEffect> mTextEffect;
        float mCharacterData[257];
    };
}
