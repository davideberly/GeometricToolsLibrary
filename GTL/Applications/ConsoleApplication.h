// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

#include <GTL/Applications/Application.h>

namespace gtl
{
    class ConsoleApplication : public Application
    {
    public:
        struct Parameters : public Application::Parameters
        {
            Parameters();

            Parameters(std::wstring const& inTitle);

            std::wstring title;
            bool created;
        };

    public:
        // Abstract base class.
        virtual ~ConsoleApplication() = default;
    protected:
        ConsoleApplication(Parameters const& parameters);

    public:
        // Member access.
        virtual void SetTitle(std::wstring const& title)
        {
            mTitle = title;
        }

        inline std::wstring GetTitle() const
        {
            return mTitle;
        }

        // Classes derived from ConsoleApplication can do whatever they wish.
        virtual void Execute() = 0;

        std::wstring mTitle;
    };
}
