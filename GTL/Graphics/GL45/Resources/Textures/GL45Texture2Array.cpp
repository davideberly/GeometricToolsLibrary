// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.19

#include <GTLGraphicsGL45PCH.h>
#include <GTL/Graphics/GL45/Resources/Textures/GL45Texture2Array.h>
using namespace gtl;

GL45Texture2Array::~GL45Texture2Array()
{
    glDeleteTextures(1, &mGLHandle);
}

GL45Texture2Array::GL45Texture2Array(Texture2Array const* texture)
    :
    GL45TextureArray(texture, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BINDING_2D_ARRAY)
{
    // Create a texture structure.
    glGenTextures(1, &mGLHandle);
    glBindTexture(GL_TEXTURE_2D_ARRAY, mGLHandle);

    // Allocate (immutable) texture storage for all levels.
    auto const width = texture->GetDimension(0);
    auto const height = texture->GetDimension(1);
    auto const numItems = texture->GetNumItems();
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, mNumLevels, mInternalFormat, width, height, numItems);

    Initialize();

    // Cannot leave this texture bound.
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    // Create a staging texture if requested.
    CreateStaging();
}

std::shared_ptr<GEObject> GL45Texture2Array::Create(void*, GraphicsObject const* object)
{
    if (object->GetType() == GT_TEXTURE2_ARRAY)
    {
        return std::make_shared<GL45Texture2Array>(
            static_cast<Texture2Array const*>(object));
    }

    GTL_RUNTIME_ERROR("Invalid object type.");
}

bool GL45Texture2Array::CanAutoGenerateMipmaps() const
{
    auto texture = GetTexture();

    return texture && texture->HasMipmaps() && texture->WantAutogenerateMipmaps();
}

void GL45Texture2Array::LoadTextureLevel(uint32_t item, uint32_t level, void const* data)
{
    auto texture = GetTexture();
    if (texture && level < texture->GetNumLevels())
    {
        auto const width = texture->GetDimensionFor(level, 0);
        auto const height = texture->GetDimensionFor(level, 1);

        // For Texture2Array, use the 3D calls where the slice (or item) is
        // the third dimension.  Only updating one slice for the specified
        // level.
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, 0, 0, item, width, height, 1,
            mExternalFormat, mExternalType, data);
    }
}
