// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.1.19

#pragma once

#include <GTL/Graphics/GL45/Engine/GL45Engine.h>

namespace gtl
{
    class WGLEngine : public GL45Engine
    {
    public:
        // Construction and destruction.  The first constructor is for
        // windowed graphics applications.  The second constructor is for
        // windowless compute-program applications.  If useDepth24Stencil8
        // is 'true', the standard 24-bits depth and 8-bits stencil buffer
        // is created.  If the value is 'false', a 32-bit depth buffer is
        // created (no stencil support).  TODO: Currently, OpenGL 4.5 is
        // required for compute shaders and for OpenGL-specific API calls.
        // See the comment in GL45Engine.h for 'MeetsRequirements()'.
        virtual ~WGLEngine();
        WGLEngine(HWND handle, bool useDepth24Stencil8, bool saveDriverInfo, int32_t requiredMajor = 4, int32_t requiredMinor = 5);
        WGLEngine(bool useDepth24Stencil8 = true, bool saveDriverInfo = false, int32_t requiredMajor = 4, int32_t requiredMinor = 5);

        // Access to members that correspond to constructor inputs.
        inline HDC GetDevice() const
        {
            return mDevice;
        }

        inline HGLRC GetImmediate() const
        {
            return mImmediate;
        }

        // Allow the user to switch between OpenGL contexts when there are
        // multiple instances of GL4Engine in an application.
        virtual bool IsActive() const override;
        virtual void MakeActive() override;

        // Support for clearing the color, depth, and stencil back buffers.
        virtual void DisplayColorBuffer(uint32_t syncInterval) override;

    private:
        // Helpers for construction and destruction.
        virtual bool Initialize(int32_t requiredMajor, int32_t requiredMinor, bool useDepth24Stencil8, bool saveDriverInfo) override;
        void Terminate();

        // Inputs to the constructor.
        HWND mHandle;

        // Objects created by the constructors.
        HDC mDevice;
        HGLRC mImmediate;
        std::wstring mComputeWindowClass;
        ATOM mComputeWindowAtom;
    };
}
