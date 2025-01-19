## Geometric Tools Library ##
 
The Geometric Tools Library (GTL) is a collection of source code for computing
in the fields of mathematics, geometry, graphics, image analysis and physics.
The engine is written in C++ 14 and supports high-performance computing using
CPU multithreading and general purpose GPU programming (GPGPU). Portions of
the code are described in various books as well as in PDF documents available
at the
[Geometric Tools Website](https://www.geometrictools.com).
GTL is licensed under the
[Boost Software License 1.0](https://www.boost.org/LICENSE_1_0.txt).

GTL is a major revision of the Geometric Tools Engine (GTE). A large portion
of GTL development has been porting code from GTE, dealing with size_t versus
signed integer compiler complaints, and adding new features. An equally
large portion has been writing unit tests and end-to-end tests for the
mathematics code. The project has taken a significant amount of time and
effort. I do not yet have a reliable schedule to post the final distribution.

GTE allowed you to specify major storage order (GTE_USE_ROW_MAJOR or
GTE_USE_COL_MAJOR). It also allowed you to specify order of matrix M and
vector V in a transformation (GTE_USE_MAT_VEC for MV or GTE_USE_VEC_MAT
for VM). To reduce code maintenance time, these are not supported in GTL.
The conventions always to use row-major storage order and vector-on-the-right
transformations (MV).

## Supported Platforms ##

Some utility code is in a header-only library, GTLUtility. The mathematics
code is in a header-only library, GTLMathematics. The CPU-based common graphics
library code is in its own library, GTLGraphics. DirectX 11 wrappers are
provided for graphics and applications, GTLGraphicsDX11 and GTLApplicationsDX11,
on Microsoft Windows 11. OpenGL 4.5 wrappers are provided for graphics and
applications, GTLGraphicsGL45 and GTLApplicationsGL45, on Microsoft Windows 11
and on Linux. A small number of files exist to use GLX and X-Windows on Linux.

On Microsoft Windows 11, the code is maintained using Microsoft Visual Studio
2022 with Microsoft's compilers, LLVM clang-cl or with Intel C++ Compiler 2025.

I have not yet tested code on Linux distributions, even for compilation. This
will occur once I get the unit tests and visual tests in place for GTL on
Microsoft Windows 11.

## Current Distribution ##

I am posting the GTL files as their unit testing and/or visual testing are
completed. The major version number is 0. When the final distribution is
posted, the major version number will be set to 1.

## Graphics and Application Support ##

The GTL DirectX 11 and OpenGL libraries, both graphics and application layers,
exist only for visual testing of the GTL code. These libraries are a direct
port of the GTE libraries except that header file organization has been
modified. The LogAssert and LogError exception macros have different names;
there are more macros for specialized exceptions that C++ supports.

## Documentation ##

Once the final distribution of GTL is posted, I will start combining the PDF
documentation into a reference book and user manual.
