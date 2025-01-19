// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#pragma once

// Applications/Common
#include <GTL/Applications/Application.h>
#include <GTL/Applications/CameraRig.h>
#include <GTL/Applications/Command.h>
#include <GTL/Applications/Console.h>
#include <GTL/Applications/ConsoleApplication.h>
#include <GTL/Applications/Environment.h>
#include <GTL/Applications/OnIdleTimer.h>
#include <GTL/Applications/TrackBall.h>
#include <GTL/Applications/TrackCylinder.h>
#include <GTL/Applications/TrackObject.h>
#include <GTL/Applications/WICFileIO.h>
#include <GTL/Applications/WindowApplication.h>

#if defined(GTL_USE_MSWINDOWS)
// Applications/MSW
#include <GTL/Applications/MSW/Console.h>
#include <GTL/Applications/MSW/ConsoleSystem.h>
#include <GTL/Applications/MSW/Window.h>
#include <GTL/Applications/MSW/WindowSystem.h>
#include <GTL/Applications/MSW/WICFileIO.h>
#endif

#if defined(GTL_USE_LINUX)
// Applications/GLX
#include <GTL/Applications/GLX/Window.h>
#include <GTL/Applications/GLX/WindowSystem.h>
#include <GTL/Applications/GLX/WICFileIO.h>
#endif
