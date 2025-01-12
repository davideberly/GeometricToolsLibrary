// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#pragma once

// TODO: Why does the LINUX linker look for gtl::TestSOMECLASS? If I add
// explicit
//   namespace gtl
//   {
//       void TestSOMECLASS() { ... }
//   }
// the LINUX linker correctly resolves this code
//   extern void TestSOMECLASS();
//   TestSOMECLASS();
// However, Visual Studio on Microsoft Windows thinks the 'extern' calls
// should resolve to C functions, so it generates linker errors if I
// enclose the definition of TestSOMECLASS within a namespace. For now,
// hack it with macros.

#if defined(GTL_USE_LINUX)

#if defined(GTL_UNIT_TESTS)
#define GTL_TEST_FUNCTION(classname) namespace gtl { void Test##classname() { UnitTest##classname tester{}; } }
#else
#define GTL_TEST_FUNCTION(classname) namespace gtl { void Test##classname() {} }
#endif

#else

#if defined(GTL_UNIT_TESTS)
#define GTL_TEST_FUNCTION(classname) void Test##classname() { UnitTest##classname tester{}; }
#else
#define GTL_TEST_FUNCTION(classname) void Test##classname() {}
#endif

#endif
