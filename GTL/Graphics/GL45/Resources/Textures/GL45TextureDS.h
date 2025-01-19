// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.1.19

#pragma once

#include <GTL/Graphics/Resources/Textures/TextureDS.h>
#include <GTL/Graphics/GL45/Resources/Textures/GL45Texture2.h>

namespace gtl
{
    class GL45TextureDS : public GL45Texture2
    {
    public:
        // Construction.
        virtual ~GL45TextureDS() = default;
        GL45TextureDS(TextureDS const* texture);
        static std::shared_ptr<GEObject> Create(void* unused, GraphicsObject const* object);

        // Member access.
        inline TextureDS* GetTexture() const
        {
            return static_cast<TextureDS*>(mGTObject);
        }

        // Returns true of mipmaps need to be generated.
        virtual bool CanAutoGenerateMipmaps() const override;

    private:
    };
}
