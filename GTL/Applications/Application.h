// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <GTL/Applications/Environment.h>
#include <GTL/Graphics/Base/BaseEngine.h>
#include <GTL/Graphics/Base/Graphics.h>
#include <memory>

// The Application class is an abstract base class with two derived classes,
// ConsoleApplication and WindowApplication. All parameters for constructing
// Application objects are in the Parameters structure or in nested structures
// derived from Parameters.

namespace gtl
{
    class Application
    {
    public:
        struct Parameters
        {
            // Window applications using the GPU must set these. Console
            // applications that do not use the GPU may are not required to
            // set these, in which case 'engine' and 'factory' are null.
            std::shared_ptr<BaseEngine> engine;
            std::shared_ptr<ProgramFactory> factory;
        };

    public:
        // Abstract base class.
        virtual ~Application() = default;
    protected:
        Application(Parameters const& parameters);

    public:
        // Get the value of the GTL_PATH environment variable. Derived
        // classes may use this variable to ensure the existence of input
        // data sets that are required by an application. If the function
        // returns "", the GTL_PATH variable has not been set.
        inline std::string GetGTLPath() const
        {
            return mEnvironment.GetGTLPath();
        }

    protected:
        // Support for access to environment variables and paths.
        Environment mEnvironment;

        // The graphics engine and program factory are stored as base
        // class pointers to allow Application to be independent of the
        // corresponding graphics API subsystems.
        std::shared_ptr<BaseEngine> mBaseEngine;
        std::shared_ptr<ProgramFactory> mProgramFactory;
    };
}
