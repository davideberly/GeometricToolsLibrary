// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.1.19

#include <GTLGraphicsGL45PCH.h>
#include <GTL/Graphics/GL45/GLSL/GLSLComputeProgram.h>
using namespace gtl;

GLSLComputeProgram::~GLSLComputeProgram()
{
    if (glIsProgram(mProgramHandle))
    {
        if (glIsShader(mComputeShaderHandle))
        {
            glDetachShader(mProgramHandle, mComputeShaderHandle);
            glDeleteShader(mComputeShaderHandle);
        }

        glDeleteProgram(mProgramHandle);
    }
}

GLSLComputeProgram::GLSLComputeProgram(GLuint programHandle, GLuint computeShaderHandle)
    :
    mProgramHandle(programHandle),
    mComputeShaderHandle(computeShaderHandle),
    mReflector(programHandle)
{
}
