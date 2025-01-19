// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Shaders/Shader.h>
#include <GTL/Graphics/DX11/HLSL/HLSLReflection.h>

namespace gtl
{
    class HLSLShader : public Shader
    {
    public:
        HLSLShader(HLSLReflection const& reflector, GraphicsObjectType type);

        virtual void Set(std::string const& textureName,
            std::shared_ptr<TextureSingle> const& texture,
            std::string const& samplerName,
            std::shared_ptr<SamplerState> const& state) override;

        virtual void Set(std::string const& textureName,
            std::shared_ptr<TextureArray> const& texture,
            std::string const& samplerName,
            std::shared_ptr<SamplerState> const& state) override;

        virtual bool IsValid(Data const& goal, ConstantBuffer* resource) const override;
        virtual bool IsValid(Data const& goal, TextureBuffer* resource) const override;
        virtual bool IsValid(Data const& goal, StructuredBuffer* resource) const override;
        virtual bool IsValid(Data const& goal, RawBuffer* resource) const override;
        virtual bool IsValid(Data const& goal, TextureSingle* resource) const override;
        virtual bool IsValid(Data const& goal, TextureArray* resource) const override;
        virtual bool IsValid(Data const& goal, SamplerState* state) const override;
    };
}
