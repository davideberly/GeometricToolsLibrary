// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.1.19

#include <GTLGraphicsGL45PCH.h>
#include <GTL/Graphics/GL45/Resources/Textures/GL45TextureCube.h>
using namespace gtl;

GL45TextureCube::~GL45TextureCube()
{
    glDeleteTextures(1, &mGLHandle);
}

GL45TextureCube::GL45TextureCube(TextureCube const* texture)
    :
    GL45TextureArray(texture, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BINDING_CUBE_MAP)
{
    // Create a texture structure.
    glGenTextures(1, &mGLHandle);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mGLHandle);

    // Allocate (immutable) texture storage for all levels.
    auto const width = texture->GetDimension(0);
    auto const height = texture->GetDimension(1);
    glTexStorage2D(GL_TEXTURE_CUBE_MAP, mNumLevels, mInternalFormat, width, height);

    Initialize();

    // Cannot leave this texture bound.
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    // Create a staging texture if requested.
    CreateStaging();
}

std::shared_ptr<GEObject> GL45TextureCube::Create(void*, GraphicsObject const* object)
{
    if (object->GetType() == GT_TEXTURE_CUBE)
    {
        return std::make_shared<GL45TextureCube>(
            static_cast<TextureCube const*>(object));
    }

    GTL_RUNTIME_ERROR("Invalid object type.");
}

bool GL45TextureCube::CanAutoGenerateMipmaps() const
{
    auto texture = GetTexture();

    return texture && texture->HasMipmaps() && texture->WantAutogenerateMipmaps();
}

void GL45TextureCube::LoadTextureLevel(uint32_t item, uint32_t level, void const* data)
{
    auto texture = GetTexture();
    if (texture && level < texture->GetNumLevels())
    {
        auto const width = texture->GetDimension(0);
        auto const height = texture->GetDimension(1);

        // Each face in the TextureCube has a unique GL target.
        GLenum targetFace = msCubeFaceTarget[item];

        glTexSubImage2D(targetFace, level, 0, 0, width, height,
            mExternalFormat, mExternalType, data);
    }
}
