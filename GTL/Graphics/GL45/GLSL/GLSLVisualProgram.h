// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.19

#pragma once

#include <GTL/Graphics/Shaders/VisualProgram.h>
#include <GTL/Graphics/GL45/GLSL/GLSLReflection.h>

// TODO: Move GLSLReflection out of the class.  The reflection work should
// be done in GLSLProgramFactory, GTEngine-required data packaged in the
// factory, and this graphics-API-independent data passed to the Shader
// constructors.  HLSL factory creation should do the same so that Shader
// does not know about graphics API.  We also don't want VisualProgram-derived
// classes storing so much information that is not used.

namespace gtl
{
    class GLSLVisualProgram : public VisualProgram
    {
    public:
        // Construction and destruction.
        virtual ~GLSLVisualProgram();
        GLSLVisualProgram(GLuint programHandle, GLuint vertexShaderHandle,
            GLuint pixedlShaderHandle, GLuint geometryShaderHandle);

        // Member access.  GLEngine needs the program handle for enabling and
        // disabling the program.  TODO: Do we need the Get*ShaderHandle
        // functions?
        inline GLuint GetProgramHandle() const
        {
            return mProgramHandle;
        }

        inline GLuint GetVertexShaderHandle() const
        {
            return mVertexShaderHandle;
        }

        inline GLuint GetPixelShaderHandle() const
        {
            return mPixelShaderHandle;
        }

        inline GLuint GetGShaderHandle() const
        {
            return mGeometryShaderHandle;
        }

        inline GLSLReflection const& GetReflector() const
        {
            return mReflector;
        }

    private:
        GLuint mProgramHandle;
        GLuint mVertexShaderHandle;
        GLuint mPixelShaderHandle;
        GLuint mGeometryShaderHandle;
        GLSLReflection mReflector;
    };
}
