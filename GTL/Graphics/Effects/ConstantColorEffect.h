// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Graphics/Effects/VisualEffect.h>

namespace gtl
{
    class ConstantColorEffect : public VisualEffect
    {
    public:
        // Construction.
        ConstantColorEffect(std::shared_ptr<ProgramFactory> const& factory, Vector4<float> const& color);

        // Member access.
        virtual void SetPVWMatrixConstant(std::shared_ptr<ConstantBuffer> const& pvwMatrix) override;

        inline std::shared_ptr<ConstantBuffer> const& GetColorConstant() const
        {
            return mColorConstant;
        }

    private:
        // Vertex shader parameter.
        std::shared_ptr<ConstantBuffer> mColorConstant;

        // Shader source code as strings.
        static std::string const msGLSLVSSource;
        static std::string const msGLSLPSSource;
        static std::string const msHLSLVSSource;
        static std::string const msHLSLPSSource;
        static ProgramSources const msVSSource;
        static ProgramSources const msPSSource;
    };
}
