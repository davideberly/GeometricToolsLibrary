// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Shaders/Shader.h>

namespace gtl
{
    class ComputeProgram
    {
    public:
        // DX11 uses the class as is.  GL45 derives from the class to store
        // the shader and program handles.
        virtual ~ComputeProgram() = default;
        ComputeProgram() = default;

        // Member access.
        inline std::shared_ptr<Shader> const& GetComputeShader() const
        {
            return mCShader;
        }

        inline void SetComputeShader(std::shared_ptr<Shader> const& shader)
        {
            if (shader)
            {
                GTL_RUNTIME_ASSERT(
                    shader->GetType() == GT_COMPUTE_SHADER,
                    "The input must be a compute shader.");
            }
            mCShader = shader;
        }

    private:
        std::shared_ptr<Shader> mCShader;
    };
}
