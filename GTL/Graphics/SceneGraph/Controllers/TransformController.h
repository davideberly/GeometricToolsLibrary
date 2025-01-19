// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Mathematics/Algebra/AffineTransform.h>
#include <GTL/Graphics/SceneGraph/Controllers/Controller.h>

namespace gtl
{
    class TransformController : public Controller
    {
    public:
        TransformController(AffineTransform<float> const& localTransform);

        // Member access.
        inline void SetTransform(AffineTransform<float> const& localTransform)
        {
            mLocalTransform = localTransform;
        }

        inline AffineTransform<float> const& GetTransform() const
        {
            return mLocalTransform;
        }

        // The animation update.  The application time is in milliseconds.
        // The update simply copies mLocalTransform to the Spatial mObject's
        // LocalTransform.  In this sense, TransformController represents a
        // transform that is constant for all time.
        virtual bool Update(double applicationTime) override;

    protected:
        AffineTransform<float> mLocalTransform;
    };
}
