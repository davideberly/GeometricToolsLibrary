// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Base/GEInputLayoutManager.h>
#include <GTL/Graphics/GL45/InputLayout/GL45InputLayout.h>
#include <map>
#include <mutex>

namespace gtl
{
    class GL45InputLayoutManager : public GEInputLayoutManager
    {
    public:
        // Construction and destruction.
        GL45InputLayoutManager()
            :
            mMap{},
            mMutex{}
        {
        }

        virtual ~GL45InputLayoutManager();

        // Management functions.  The Unbind(vbuffer) removes all layouts that
        // involve vbuffer.  The Unbind(vshader) is stubbed out because GL45
        // does not require it, but we wish to have
        // Unbind(GraphicsObject const*) as a base-class GraphicsEngine
        // function.
        GL45InputLayout* Bind(GLuint programHandle, GLuint vbufferHandle, VertexBuffer const* vbuffer);
        virtual bool Unbind(VertexBuffer const* vbuffer) override;
        virtual bool Unbind(Shader const* vshader) override;
        virtual void UnbindAll() override;
        virtual bool HasElements() const override;

    private:
        typedef std::pair<VertexBuffer const*, GLuint> VBPPair;
        std::map<VBPPair, std::shared_ptr<GL45InputLayout>> mMap;
        mutable std::mutex mMutex;
    };
}
