// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.19

#pragma once

#include <GTL/Graphics/Shaders/ComputeProgram.h>
#include <GTL/Graphics/GL45/GLSL/GLSLReflection.h>

namespace gtl
{
    class GLSLComputeProgram : public ComputeProgram
    {
    public:
        // Construction and destruction.
        virtual ~GLSLComputeProgram();
        GLSLComputeProgram(GLuint programHandle, GLuint computeShaderHandle);

        // Member access.  GLEngine needs the program handle for enabling and
        // disabling the program.  TODO: Do we need the GetComputeShaderHandle
        // function?
        inline GLuint GetProgramHandle() const
        {
            return mProgramHandle;
        }

        inline GLuint GetComputeShaderHandle() const
        {
            return mComputeShaderHandle;
        }

        inline GLSLReflection const& GetReflector() const
        {
            return mReflector;
        }

    private:
        GLuint mProgramHandle;
        GLuint mComputeShaderHandle;
        GLSLReflection mReflector;
    };
}
