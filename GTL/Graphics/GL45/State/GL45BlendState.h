// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.19

#pragma once

#include <GTL/Graphics/State/BlendState.h>
#include <GTL/Graphics/GL45/State/GL45DrawingState.h>
#include <array>
#include <cstdint>

namespace gtl
{
    class GL45BlendState : public GL45DrawingState
    {
    public:
        // Construction.
        virtual ~GL45BlendState() = default;
        GL45BlendState(BlendState const* blendState);
        static std::shared_ptr<GEObject> Create(void* unused, GraphicsObject const* object);

        // Member access.
        inline BlendState* GetBlendState()
        {
            return static_cast<BlendState*>(mGTObject);
        }

        // Enable the blend state.
        void Enable();

    private:
        struct Target
        {
            Target()
                :
                enable(0),
                srcColor(0),
                dstColor(0),
                opColor(0),
                srcAlpha(0),
                dstAlpha(0),
                opAlpha(0),
                rMask(0),
                gMask(0),
                bMask(0),
                aMask(0)
            {
            }

            GLboolean enable;
            GLenum srcColor;
            GLenum dstColor;
            GLenum opColor;
            GLenum srcAlpha;
            GLenum dstAlpha;
            GLenum opAlpha;
            GLboolean rMask;
            GLboolean gMask;
            GLboolean bMask;
            GLboolean aMask;
        };

        bool mEnableAlphaToCoverage;
        bool mEnableIndependentBlend;
        std::array<Target, BlendState::NUM_TARGETS> mTarget;
        Vector4<float> mBlendColor;
        uint32_t mSampleMask;

        // Conversions from GTEngine values to GL45 values.
        static std::array<GLenum const, 17> msMode;
        static std::array<GLenum const, 5> msOperation;
    };
}
