// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLGraphicsDX11PCH.h>
#include <GTL/Graphics/DX11/HLSL/HLSLBaseBuffer.h>
using namespace gtl;

HLSLBaseBuffer::HLSLBaseBuffer(D3D_SHADER_INPUT_BIND_DESC const& desc,
    uint32_t numBytes, std::vector<Member> const& members)
    :
    HLSLResource(desc, numBytes),
    mMembers(members)
{
}

HLSLBaseBuffer::HLSLBaseBuffer(D3D_SHADER_INPUT_BIND_DESC const& desc,
    uint32_t index, uint32_t numBytes,
    std::vector<Member> const& members)
    :
    HLSLResource(desc, index, numBytes),
    mMembers(members)
{
}

void HLSLBaseBuffer::Print(std::ofstream& output) const
{
    int32_t i = 0;
    for (auto const& member : mMembers)
    {
        output << "Variable[" << i << "]:" << std::endl;
        member.first.Print(output);
        output << "Type[" << i << "]:" << std::endl;
        member.second.Print(output, 0);
        ++i;
    }
}

void HLSLBaseBuffer::GenerateLayout(std::vector<MemberLayout>& layout) const
{
    for (auto const& m : mMembers)
    {
        HLSLShaderType const& parent = m.second;
        GenerateLayout(parent, m.first.GetOffset(), parent.GetName(),
            layout);
    }
}

void HLSLBaseBuffer::GenerateLayout(HLSLShaderType const& parent,
    uint32_t parentOffset, std::string const& parentName,
    std::vector<MemberLayout>& layout) const
{
    uint32_t const numChildren = parent.GetNumChildren();
    if (numChildren > 0)
    {
        for (uint32_t i = 0; i < numChildren; ++i)
        {
            HLSLShaderType const& child = parent.GetChild(i);
            GenerateLayout(child, parentOffset + child.GetOffset(),
                parentName + "." + child.GetName(), layout);
        }
    }
    else
    {
        MemberLayout item;
        item.name = parentName;
        item.offset = parentOffset;
        item.numElements = parent.GetNumElements();
        layout.push_back(item);
    }
}
