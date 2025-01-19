// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Mathematics/Algebra/Vector.h>
#include <GTL/Graphics/Effects/VisualEffect.h>
#include <GTL/Graphics/Resources/Buffers/ConstantBuffer.h>
#include <GTL/Graphics/Resources/Textures/Texture2.h>
#include <GTL/Graphics/State/SamplerState.h>

namespace gtl
{
    class TextEffect : public VisualEffect
    {
    public:
        // Construction.
        TextEffect(std::shared_ptr<ProgramFactory> const& factory,
            std::shared_ptr<Texture2> const& texture);

        // Support for typesetting.
        inline std::shared_ptr<ConstantBuffer> const& GetTranslate() const
        {
            return mTranslate;
        }

        inline std::shared_ptr<ConstantBuffer> const& GetColor() const
        {
            return mColor;
        }

        void SetTranslate(float x, float y);
        void SetNormalizedZ(float z);
        void SetColor(Vector4<float> const& color);

    private:
        std::shared_ptr<ConstantBuffer> mTranslate;
        std::shared_ptr<ConstantBuffer> mColor;
        std::shared_ptr<SamplerState> mSamplerState;

        // Default normalized Z coordinate for rendered text.
        static std::array<float, ProgramFactory::PF_NUM_API> const msDefaultNormalizedZ;

        // Shader source code as strings.
        static std::string const msGLSLVSSource;
        static std::string const msGLSLPSSource;
        static std::string const msHLSLVSSource;
        static std::string const msHLSLPSSource;
        static ProgramSources const msVSSource;
        static ProgramSources const msPSSource;
    };
}
