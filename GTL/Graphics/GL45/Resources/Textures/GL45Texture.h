// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.1.19

#pragma once

#include <GTL/Graphics/Resources/Textures/Texture.h>
#include <GTL/Graphics/GL45/Resources/GL45Resource.h>
#include <array>

namespace gtl
{
    class GL45Texture : public GL45Resource
    {
    public:
        // Abstract base class.
        virtual ~GL45Texture() = default;
    protected:
        GL45Texture(Texture const* texture, GLenum target, GLenum targetBinding);

    public:
        // Member access.
        inline Texture* GetTexture() const
        {
            return static_cast<Texture*>(mGTObject);
        }

        inline GLenum GetTarget() const
        {
            return mTarget;
        }

        inline GLenum GetTargetBinding() const
        {
            return mTargetBinding;
        }

        // Get the GL4 internal format for the specified Texture data format.
        inline static GLuint GetInternalFormat(uint32_t dataFormat)
        {
            return msGLTextureInternalFormat[dataFormat];
        }

    protected:
        GLenum mTarget;
        GLenum mTargetBinding;

        // Properties of overall texture.
        GLint mNumLevels;
        GLuint mInternalFormat;
        GLuint mExternalFormat;
        GLuint mExternalType;

        // Mapping from DFType to GL4 specific types
        static std::array<GLuint const, DF_NUM_FORMATS> msGLTextureInternalFormat;
        static std::array<GLuint const, DF_NUM_FORMATS> msGLTextureExternalFormat;
        static std::array<GLuint const, DF_NUM_CHANNEL_TYPES> msGLTextureExternalType;
    };
}
