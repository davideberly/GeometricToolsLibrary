// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsPCH.h>
#include <GTL/Graphics/Effects/VertexColorEffect.h>
using namespace gtl;

VertexColorEffect::VertexColorEffect(std::shared_ptr<ProgramFactory> const& factory)
{
    int32_t api = factory->GetAPI();
    mProgram = factory->CreateFromSources(*msVSSource[api], *msPSSource[api], "");
    if (mProgram)
    {
        mProgram->GetVertexShader()->Set("PVWMatrix", mPVWMatrixConstant);
    }
}

void VertexColorEffect::SetPVWMatrixConstant(std::shared_ptr<ConstantBuffer> const& buffer)
{
    VisualEffect::SetPVWMatrixConstant(buffer);
    mProgram->GetVertexShader()->Set("PVWMatrix", mPVWMatrixConstant);
}


std::string const VertexColorEffect::msGLSLVSSource =
R"(
    uniform PVWMatrix
    {
        mat4 pvwMatrix;
    };

    layout(location = 0) in vec3 modelPosition;
    layout(location = 1) in vec4 modelColor;
    layout(location = 0) out vec4 vertexColor;

    void main()
    {
        vertexColor = modelColor;
        gl_Position = pvwMatrix * vec4(modelPosition, 1.0f);
    }
)";

std::string const VertexColorEffect::msGLSLPSSource =
R"(
    layout(location = 0) in vec4 vertexColor;
    layout(location = 0) out vec4 pixelColor;

    void main()
    {
        pixelColor = vertexColor;
    }
)";

std::string const VertexColorEffect::msHLSLVSSource =
R"(
    cbuffer PVWMatrix
    {
        float4x4 pvwMatrix;
    };

    struct VS_INPUT
    {
        float3 modelPosition : POSITION;
        float4 modelColor : COLOR0;
    };

    struct VS_OUTPUT
    {
        float4 vertexColor : COLOR0;
        float4 clipPosition : SV_POSITION;
    };

    VS_OUTPUT VSMain (VS_INPUT input)
    {
        VS_OUTPUT output;
        output.clipPosition = mul(pvwMatrix, float4(input.modelPosition, 1.0f));
        output.vertexColor = input.modelColor;
        return output;
    }
)";

std::string const VertexColorEffect::msHLSLPSSource =
R"(
    struct PS_INPUT
    {
        float4 vertexColor : COLOR0;
    };

    struct PS_OUTPUT
    {
        float4 pixelColor : SV_TARGET0;
    };

    PS_OUTPUT PSMain(PS_INPUT input)
    {
        PS_OUTPUT output;
        output.pixelColor = input.vertexColor;
        return output;
    }
)";

ProgramSources const VertexColorEffect::msVSSource =
{
    &msGLSLVSSource,
    &msHLSLVSSource
};

ProgramSources const VertexColorEffect::msPSSource =
{
    &msGLSLPSSource,
    &msHLSLPSSource
};
