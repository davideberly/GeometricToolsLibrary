// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.19

#pragma once

#include <GTL/Graphics/Base/GEDrawTarget.h>
#include <GTL/Graphics/GL45/Resources/Textures/GL45TextureDS.h>
#include <GTL/Graphics/GL45/Resources/Textures/GL45TextureRT.h>

namespace gtl
{
    class GL45DrawTarget : public GEDrawTarget
    {
    public:
        // Construction and destruction.
        virtual ~GL45DrawTarget();
        GL45DrawTarget(DrawTarget const* target,
            std::vector<GL45TextureRT*>& rtTextures, GL45TextureDS* dsTexture);
        static std::shared_ptr<GEDrawTarget> Create(DrawTarget const* target,
            std::vector<GEObject*>& rtTextures, GEObject* dsTexture);

        // Member access.
        inline GL45TextureRT* GetRTTexture(uint32_t i) const
        {
            return mRTTextures[i];
        }

        inline GL45TextureDS* GetDSTexture() const
        {
            return mDSTexture;
        }

        // Used in the Renderer::Draw function.
        void Enable();
        void Disable();

    private:
        std::vector<GL45TextureRT*> mRTTextures;
        GL45TextureDS* mDSTexture;

        GLuint mFrameBuffer;

        // Temporary storage during enable/disable of targets.
        GLint mSaveViewportX;
        GLint mSaveViewportY;
        GLsizei mSaveViewportWidth;
        GLsizei mSaveViewportHeight;
        GLdouble mSaveViewportNear;
        GLdouble mSaveViewportFar;
    };
}
