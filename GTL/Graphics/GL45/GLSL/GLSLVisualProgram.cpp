// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.1.19

#include <GTLGraphicsGL45PCH.h>
#include <GTL/Graphics/GL45/GLSL/GLSLVisualProgram.h>
using namespace gtl;

GLSLVisualProgram::~GLSLVisualProgram()
{
    if (glIsProgram(mProgramHandle))
    {
        if (glIsShader(mVertexShaderHandle))
        {
            glDetachShader(mProgramHandle, mVertexShaderHandle);
            glDeleteShader(mVertexShaderHandle);
        }

        if (glIsShader(mPixelShaderHandle))
        {
            glDetachShader(mProgramHandle, mPixelShaderHandle);
            glDeleteShader(mPixelShaderHandle);
        }

        if (glIsShader(mGeometryShaderHandle))
        {
            glDetachShader(mProgramHandle, mGeometryShaderHandle);
            glDeleteShader(mGeometryShaderHandle);
        }

        glDeleteProgram(mProgramHandle);
    }
}

GLSLVisualProgram::GLSLVisualProgram(GLuint programHandle, GLuint vertexShaderHandle,
    GLuint pixedlShaderHandle, GLuint geometryShaderHandle)
    :
    mProgramHandle(programHandle),
    mVertexShaderHandle(vertexShaderHandle),
    mPixelShaderHandle(pixedlShaderHandle),
    mGeometryShaderHandle(geometryShaderHandle),
    mReflector(programHandle)
{
}
