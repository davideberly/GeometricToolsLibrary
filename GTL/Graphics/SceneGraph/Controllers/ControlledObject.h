// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Graphics/SceneGraph/Controllers/Controller.h>
#include <list>
#include <memory>

namespace gtl
{
    class ControlledObject
    {
    protected:
        // Abstract base class.
        ControlledObject() = default;
    public:
        virtual ~ControlledObject() = default;

        // Access to the controllers that control this object.
        typedef std::list<std::shared_ptr<Controller>> List;

        inline List const& GetControllers() const
        {
            return mControllers;
        }

        void AttachController(std::shared_ptr<Controller> const& controller);
        void DetachController(std::shared_ptr<Controller> const& controller);
        void DetachAllControllers();
        bool UpdateControllers(double applicationTime);

    private:
        List mControllers;
    };
}
