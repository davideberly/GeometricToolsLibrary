// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#include <GTLApplicationsPCH.h>
#include <GTL/Applications/MSW/Console.h>
using namespace gtl;

// The singleton used to create and destroy consoles for applications.
namespace gtl
{
    ConsoleSystem TheConsoleSystem;
}

#if defined(GTL_USE_DIRECTX)

#include <GTL/Graphics/DX11/DXGI/DXGIAdapter.h>

void ConsoleSystem::CreateEngineAndProgramFactory(Console::Parameters& parameters)
{
    // The adapterManager must be declared outside the if-then-else
    // statement so that it persists long enough to create the 'engine'
    // object.
    DXGIAdapter adapterManager;
    IDXGIAdapter* adapter = nullptr;
    if ((parameters.deviceCreationFlags & D3D11_CREATE_DEVICE_DEBUG) == 0)
    {
        // The GPU adapter is selected using the following algorithm. If a
        // discrete adapter is available (NVIDIA, AMD or other manufacturer),
        // it is selected. If a discrete adapter is not available, Intel
        // Integrated Graphics is chosen. Although these days Intel Core
        // architecture is the norm, in the event Intel Integrated Graphics is
        // not found, the fallback is to Microsoft WARP which is a software
        // implementation for DirectX 11 that is multithreaded and has decent
        // performance.
        adapterManager = DXGIAdapter::GetMostPowerful();
        adapter = adapterManager.GetAdapter();
    }
    else
    {
        // If parameters.deviceCreationFlags is 0 (no flags specified), the
        // first adapter in the adapter enumeration is selected. This is
        // invariably the adapter to which the display monitors are attached.
        //
        // If the debug layer is selected using D3D11_CREATE_DEVICE_DEBUG,
        // choosing a non-null adapter does not work. It will cause the
        // D3D11CreateDevice funtion to throw an exception and not return
        // an HRESULT code.
        adapter = nullptr;
    }

    auto engine = std::make_shared<DX11Engine>(adapter, D3D_DRIVER_TYPE_HARDWARE,
        nullptr, parameters.deviceCreationFlags);

    if (engine->GetDevice())
    {
        parameters.engine = engine;
        parameters.factory = std::make_shared<HLSLProgramFactory>();
        parameters.created = true;
    }
    else
    {
        GTL_RUNTIME_ERROR(
            "Cannot create compute engine.");
    }
}
#endif

#if defined(GTL_USE_OPENGL)
void ConsoleSystem::CreateEngineAndProgramFactory(Console::Parameters& parameters)
{
    bool saveDriverInfo = ((parameters.deviceCreationFlags & 0x00000001) != 0);
    auto engine = std::make_shared<WGLEngine>(false, saveDriverInfo);
    if (!engine->MeetsRequirements())
    {
        GTL_ARGUMENT_ERROR("OpenGL 4.5 or later is required.");
    }

    if (engine->GetDevice())
    {
        parameters.engine = engine;
        parameters.factory = std::make_shared<GLSLProgramFactory>();
        parameters.created = true;
    }
    else
    {
        GTL_ARGUMENT_ERROR("Cannot create compute engine.");
    }
}
#endif
