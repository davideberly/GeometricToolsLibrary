// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#if defined(GTL_USE_MSWINDOWS)
#include <GTL/Applications/MSW/Window.h>
#endif
#if defined(GTL_USE_LINUX)
#include <Applications/GLX/Window.h>
#endif
#include <GTL/Applications/CameraRig.h>
#include <GTL/Applications/TrackBall.h>
#include <GTL/Graphics/SceneGraph/Hierarchy/PVWUpdater.h>

namespace gtl
{
    class Window3 : public Window
    {
    protected:
        // Abstract base class.
        Window3(Parameters& parameters);

    public:
        // Create the camera and camera rig.
        void InitializeCamera(float upFovDegrees, float aspectRatio, float dmin, float dmax,
            float translationSpeed, float rotationSpeed, std::array<float, 3> const& pos,
            std::array<float, 3> const& dir, std::array<float, 3> const& up);

        // The camera frustum is modified.  Any subscribers to the pvw-matrix
        // update system of the camera rig must be updated.  No call is made
        // to OnDisplay() or OnIdle().  The base class is unaware of which
        // display method you use, so to have a visual update you must
        // override OnResize
        //    bool MyApplication::OnResize(int32_t xSize, int32_t ySize)
        //    {
        //        if (Window3::OnResize(xSize, ySize))
        //        {
        //            OnIdle();  // or OnDisplay() or MyOwnDrawFunction() ...
        //        }
        //    }
        virtual bool OnResize(int32_t xSize, int32_t ySize) override;

        // The key 't' decreases the translation speed and the 'T' key
        // increases the translation speed.  The 'r' key decreases the
        // rotation speed and the 'R' key increases the rotation speed.
        virtual bool OnCharPress(uint8_t key, int32_t x, int32_t y) override;

        // The appropriate camera rig motion is selected when 'key' is mapped
        // to a camera motion.
        virtual bool OnKeyDown(int32_t key, int32_t x, int32_t y) override;
        virtual bool OnKeyUp(int32_t key, int32_t x, int32_t y) override;

        // Control the rotation of the trackball.
        virtual bool OnMouseClick(MouseButton button, MouseState state,
            int32_t x, int32_t y, uint32_t modifiers) override;

        virtual bool OnMouseMotion(MouseButton button, int32_t x, int32_t y,
            uint32_t modifiers) override;

    protected:
        BufferUpdater mUpdater;
        std::shared_ptr<Camera> mCamera;
        CameraRig mCameraRig;
        PVWUpdater mPVWMatrices;
        TrackBall mTrackBall;
    };
}
