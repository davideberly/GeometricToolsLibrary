// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Graphics/Resources/Buffers/MemberLayout.h>
#include <GTL/Graphics/DX11/HLSL/HLSLShaderType.h>
#include <GTL/Graphics/DX11/HLSL/HLSLShaderVariable.h>

namespace gtl
{
    class HLSLBaseBuffer : public HLSLResource
    {
    public:
        typedef std::pair<HLSLShaderVariable, HLSLShaderType> Member;

        // Construction and destruction.
        virtual ~HLSLBaseBuffer() = default;

        HLSLBaseBuffer(D3D_SHADER_INPUT_BIND_DESC const& desc,
            uint32_t numBytes, std::vector<Member> const& members);

        HLSLBaseBuffer(D3D_SHADER_INPUT_BIND_DESC const& desc,
            uint32_t index, uint32_t numBytes,
            std::vector<Member> const& members);

        // Member access.
        inline std::vector<HLSLBaseBuffer::Member> const& GetMembers() const
        {
            return mMembers;
        }

        // Print to a text file for human readability.
        virtual void Print(std::ofstream& output) const;

        // Generation of lookup tables for member layout.
        void GenerateLayout(std::vector<MemberLayout>& layout) const;

    private:
        void GenerateLayout(HLSLShaderType const& type, uint32_t parentOffset,
            std::string const& parentName,
            std::vector<MemberLayout>& layout) const;

        std::vector<Member> mMembers;
    };
}
