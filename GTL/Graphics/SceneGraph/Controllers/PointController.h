// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Mathematics/Algebra/Vector.h>
#include <GTL/Graphics/SceneGraph/Controllers/Controller.h>
#include <GTL/Graphics/Resources/Buffers/VertexBuffer.h>

namespace gtl
{
    class PointController : public Controller
    {
    public:
        // Abstract base class. The object to which this is attached must be
        // a Visual that stores a points geometric primitive.  The index
        // buffer must be of type IP_POLYPOINT.
        virtual ~PointController() = default;
    protected:
        PointController(BufferUpdater const& postUpdate);

    public:
        // The system motion, in local coordinates.  The velocity vectors
        // must be unit length.
        float systemLinearSpeed;
        float systemAngularSpeed;
        Vector3<float> systemLinearAxis;
        Vector3<float> systemAngularAxis;

        // Point motion, in the model space of the system.  The velocity
        // vectors must be unit length.  In applications where the points
        // represent a rigid body, you might choose the origin of the system
        // to be the center of mass of the points and the coordinate axes to
        // correspond to the principal directions of the inertia tensor.
        inline size_t GetNumPoints() const
        {
            return mPointLinearSpeed.size();
        }

        inline std::vector<float> const& GetPointLinearSpeed() const
        {
            return mPointLinearSpeed;
        }

        inline std::vector<float> const& GetPointAngularSpeed() const
        {
            return mPointAngularSpeed;
        }

        inline std::vector<Vector3<float>> const& GetPointLinearAxis() const
        {
            return mPointLinearAxis;
        }

        inline std::vector<Vector3<float>> const& GetPointAngularAxis() const
        {
            return mPointAngularAxis;
        }

        // The animation update.  The application time is in milliseconds.
        virtual bool Update(double applicationTime) override;

    protected:
        // Override the base-class member function in order to verify that
        // the object is Visual with a vertex format and vertex buffer that
        // satisfy the preconditions of the PointController.
        virtual void SetObject(ControlledObject* object) override;

        // This class computes the new positions and orientations from the
        // motion parameters.  Derived classes should update the motion
        // parameters and then either call the base class update methods or
        // provide its own update methods for position and orientation.
        virtual void UpdateSystemMotion(float ctrlTime);
        virtual void UpdatePointMotion(float ctrlTime);

        std::vector<float> mPointLinearSpeed;
        std::vector<float> mPointAngularSpeed;
        std::vector<Vector3<float>> mPointLinearAxis;
        std::vector<Vector3<float>> mPointAngularAxis;

        BufferUpdater mPostUpdate;
    };
}
