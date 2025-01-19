// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.1.19

#pragma once

#include <GTL/Graphics/Resources/Textures/Texture1.h>
#include <GTL/Graphics/GL45/Resources/Textures/GL45TextureSingle.h>

namespace gtl
{
    class GL45Texture1 : public GL45TextureSingle
    {
    public:
        // Construction and destruction.
        virtual ~GL45Texture1();
        GL45Texture1(Texture1 const* texture);
        static std::shared_ptr<GEObject> Create(void* unused, GraphicsObject const* object);

        // Member access.
        inline Texture1* GetTexture() const
        {
            return static_cast<Texture1*>(mGTObject);
        }

        // Returns true if mipmaps need to be generated.
        virtual bool CanAutoGenerateMipmaps() const override;

    protected:
        virtual void LoadTextureLevel(uint32_t level, void const* data) override;
    };
}
