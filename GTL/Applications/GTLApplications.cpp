// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: yyyy.mm.dd

#include <GTLApplicationsPCH.h>

// The preprocessor symbol GTL_USE_MSWINDOWS is added to all Visual Studio
// project configurations.
#if defined(GTL_USE_MSWINDOWS)

#if !defined(_MSC_VER)
#error Microsoft Visual Studio 2022 or later is required.
#endif

//  MSVC  6   is version 12.0
//  MSVC  7.0 is version 13.0 (MSVS 2002)
//  MSVC  7.1 is version 13.1 (MSVS 2003)
//  MSVC  8.0 is version 14.0 (MSVS 2005)
//  MSVC  9.0 is version 15.0 (MSVS 2008)
//  MSVC 10.0 is version 16.0 (MSVS 2010)
//  MSVC 11.0 is version 17.0 (MSVS 2012)
//  MSVC 12.0 is version 18.0 (MSVS 2013)
//  MSVC 14.0 is version 19.0 (MSVS 2015)
//  MSVC 15.0 is version 19.1 (MSVS 2017)
//  MSVC 16.0 is version 19.2 (MSVS 2019)
//  MSVC 17.0 is version 19.3 (MSVS 2022)
//  Currently, projects are provided only for MSVS 2022
#if _MSC_VER < 1930
#error Microsoft Visual Studio 2022 or later is required.
#endif

#endif
