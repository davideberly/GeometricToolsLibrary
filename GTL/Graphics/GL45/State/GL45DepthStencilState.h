// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.19

#pragma once

#include <GTL/Graphics/State/DepthStencilState.h>
#include <GTL/Graphics/GL45/State/GL45DrawingState.h>

namespace gtl
{
    class GL45DepthStencilState : public GL45DrawingState
    {
    public:
        // Construction.
        virtual ~GL45DepthStencilState() = default;
        GL45DepthStencilState(DepthStencilState const* depthStencilState);
        static std::shared_ptr<GEObject> Create(void* unused, GraphicsObject const* object);

        // Member access.
        inline DepthStencilState* GetDepthStencilState()
        {
            return static_cast<DepthStencilState*>(mGTObject);
        }

        // Enable the depth-stencil state.
        void Enable();

    private:
        struct Face
        {
            GLenum onFail;
            GLenum onZFail;
            GLenum onZPass;
            GLenum comparison;
        };

        GLboolean mDepthEnable;
        GLboolean mWriteMask;
        GLenum mComparison;
        GLboolean mStencilEnable;
        GLuint mStencilReadMask;
        GLuint mStencilWriteMask;
        Face mFrontFace;
        Face mBackFace;
        GLuint mReference;

        // Conversions from GTEngine values to GL4 values.
        static GLboolean const msWriteMask[];
        static GLenum const msComparison[];
        static GLenum const msOperation[];
    };
}
