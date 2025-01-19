// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLGraphicsDX11PCH.h>
#include <GTL/Graphics/DX11/Engine/DX11.h>
#include <comdef.h>
#include <codecvt>
#include <locale>
using namespace gtl;

HRESULT DX11::SetPrivateName(ID3D11DeviceChild* object, std::string const& name)
{
    HRESULT hr;
    if (object && name != "")
    {
        hr = object->SetPrivateData(WKPDID_D3DDebugObjectName,
            static_cast<UINT>(name.length()), name.c_str());
    }
    else
    {
        // Callers are allowed to call this function with a null input
        // or with an empty name (for convenience).
        hr = S_OK;
    }
    return hr;
}

HRESULT DX11::SetPrivateName(IDXGIObject* object, std::string const& name)
{
    HRESULT hr;
    if (object && name != "")
    {
        hr = object->SetPrivateData(WKPDID_D3DDebugObjectName,
            static_cast<UINT>(name.length()), name.c_str());
    }
    else
    {
        // Callers are allowed to call this function with a null input
        // or with an empty name (for convenience).
        hr = S_OK;
    }
    return hr;
}

void DX11::Log(HRESULT hr, char const* file, char const* function, int32_t line)
{
    if (FAILED(hr))
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> convl;
        std::string message = convl.to_bytes(_com_error(hr).ErrorMessage());
        throw std::runtime_error(std::string(file) + "(" + std::string(function) + "," + std::to_string(line) + "): " + message + "\n");
    }
}
