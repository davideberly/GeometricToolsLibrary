// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 0.0.2025.01.12

#if defined(GTL_UNIT_TESTS)
#include <UnitTestsExceptions.h>
#include <GTL/Mathematics/Arithmetic/IEEEFunctions.h>
using namespace gtl;

namespace gtl
{
    class UnitTestIEEEFunctions
    {
    public:
        UnitTestIEEEFunctions();

    private:
        void Test();
    };
}

UnitTestIEEEFunctions::UnitTestIEEEFunctions()
{
    UTInformation("Mathematics/Arithmetic/IEEEFunctions");

    Test();
}

void UnitTestIEEEFunctions::Test()
{
    // The functions were also computed using a manual calculator to verify.
    // These values are listed in the comments.
    float fx, fy, fz;
    double dx, dy, dz;
    std::int32_t isgn;
    float maxFError = 1e-8f;
    double maxDError = 1e-14;

    // 0.03895644501586069008486141866225
    fx = 0.123f;
    fy = atandivpi(fx);
    UTAssert(std::fabs(fy - 0.0389564447f) <= maxFError, "Invalid atandivpi result.");
    dx = 0.123;
    dy = atandivpi(dx);
    UTAssert(std::fabs(dy - 0.038956445015860694) <= maxDError, "Invalid atandivpi result.");

    // 0.4914292415641611882750155318884
    fx = 0.123f;
    fy = 4.567f;
    fz = atan2divpi(fy, fx);
    UTAssert(std::fabs(fz - 0.491429210f) <= maxFError, "Invalid atan2divpi result.");
    dx = 0.123;
    dy = 4.567;
    dz = atan2divpi(dy, dx);
    UTAssert(std::fabs(dz - 0.49142924156416123) <= maxDError, "Invalid atan2divpi result.");

    fx = 0.01f;
    fy = clamp(fx, 0.0f, 0.1f);
    UTAssert(fy == 0.01f, "Invalid clamp result.");
    fx = -0.01f;
    fy = clamp(fx, 0.0f, 0.1f);
    UTAssert(fy == 0.0f, "Invalid clamp result.");
    fx = 0.2f;
    fy = clamp(fx, 0.0f, 0.1f);
    UTAssert(fy == 0.1f, "Invalid clamp result.");
    dx = 0.01;
    dy = clamp(dx, 0.0, 0.1);
    UTAssert(dy == 0.01, "Invalid clamp result.");
    dx = -0.01f;
    dy = clamp(dx, 0.0, 0.1);
    UTAssert(dy == 0.0, "Invalid clamp result.");
    dx = 0.2f;
    dy = clamp(dx, 0.0, 0.1);
    UTAssert(dy == 0.1, "Invalid clamp result.");

    // 0.92626575101906665547645701313522
    fx = 0.123f;
    fy = cospi(fx);
    UTAssert(std::fabs(fy - 0.926265717f) <= maxFError, "Invalid cospi result.");
    dx = 0.123;
    dy = cospi(dx);
    UTAssert(std::fabs(dy - 0.92626575101906672) <= maxDError, "Invalid cospi result.");

    // 1.3273944577297395022918314728598
    fx = 0.123f;
    fy = exp10(fx);
    UTAssert(std::fabs(fy - 1.32739449f) <= maxFError, "Invalid exp10 result.");
    dx = 0.123;
    dy = exp10(dx);
    UTAssert(std::fabs(dy - 1.3273944577297396) <= maxDError, "Invalid exp10 result.");

    // 2.8513297425610051897264174769212
    fx = 0.123f;
    fy = invsqrt(fx);
    UTAssert(std::fabs(fy - 2.85132980f) <= maxFError, "Invalid invsqrt result.");
    dx = 0.123;
    dy = invsqrt(dx);
    UTAssert(std::fabs(dy - 2.8513297425610054) <= maxDError, "Invalid invsqrt result.");

    fx = -0.123f;
    isgn = isign(fx);
    UTAssert(isgn == -1, "Invalid isign result.");
    fx = 0.0f;
    isgn = isign(fx);
    UTAssert(isgn == 0, "Invalid isign result.");
    fx = 0.123f;
    isgn = isign(fx);
    UTAssert(isgn == +1, "Invalid isign result.");
    dx = -0.123;
    isgn = isign(dx);
    UTAssert(isgn == -1, "Invalid isign result.");
    dx = 0.0;
    isgn = isign(dx);
    UTAssert(isgn == 0, "Invalid isign result.");
    dx = 0.123;
    isgn = isign(dx);
    UTAssert(isgn == +1, "Invalid isign result.");

    fx = -0.123f;
    fy = saturate(fx);
    UTAssert(fy == 0.0f, "Invalid saturate result.");
    fx = 0.123f;
    fy = saturate(fx);
    UTAssert(fy == 0.123f, "Invalid saturate result.");
    fx = 1.123f;
    fy = saturate(fx);
    UTAssert(fy == 1.0f, "Invalid saturate result.");
    dx = -0.123;
    dy = saturate(dx);
    UTAssert(dy == 0.0, "Invalid saturate result.");
    dx = 0.123;
    dy = saturate(dx);
    UTAssert(dy == 0.123, "Invalid saturate result.");
    dx = 1.123;
    dy = saturate(dx);
    UTAssert(dy == 1.0, "Invalid saturate result.");

    fx = -0.123f;
    fy = sign(fx);
    UTAssert(fy == -1.0f, "Invalid isign result.");
    fx = 0.0f;
    fy = sign(fx);
    UTAssert(fy == 0.0f, "Invalid isign result.");
    fx = 0.123f;
    fy = sign(fx);
    UTAssert(fy == +1.0f, "Invalid isign result.");
    dx = -0.123;
    dy = isign(dx);
    UTAssert(dy == -1.0, "Invalid isign result.");
    dx = 0.0;
    dy = isign(dx);
    UTAssert(dy == 0.0, "Invalid isign result.");
    dx = 0.123;
    dy = sign(dx);
    UTAssert(dy == +1.0, "Invalid isign result.");

    // 0.37687101041216266398659977173965
    fx = 0.123f;
    fy = sinpi(fx);
    UTAssert(std::fabs(fy - 0.376871049f) <= maxFError, "Invalid sinpi result.");
    dx = 0.123;
    dy = sinpi(dx);
    UTAssert(std::fabs(dy - 0.37687101041216264) <= maxDError, "Invalid sinpi result.");

    // 0.015129
    fx = 0.123f;
    fy = sqr(fx);
    UTAssert(std::fabs(fy - 0.0151290009f) <= maxFError, "Invalid sqr result.");
    dx = 0.123;
    dy = sqr(dx);
    UTAssert(std::fabs(dy - 0.015129000000000000) <= maxDError, "Invalid sqr result.");
}

#else

#include <GTL/Mathematics/Arithmetic/IEEEFunctions.h>

#endif

#include <UnitTestsNamespaces.h>
GTL_TEST_FUNCTION(IEEEFunctions)
