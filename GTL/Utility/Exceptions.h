// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// Version: 0.0.2025.01.12

#pragma once

// Documentation for this class is
// https://www.geometrictools.com/Documentation/GTLUtility.pdf#Exceptions

#include <stdexcept>
#include <string>

// If exceptions are enabled, throw the exception passed in, otherwise call std::terminate.
#if (defined(__cpp_exceptions) || defined(__EXCEPTIONS) || (defined(_MSC_VER) && defined(_CPPUNWIND))) && !defined(GTL_NO_EXCEPTIONS)
#define GTL_EXCEPTION_HANDLER(exception, message) throw exception(message)
#else
#define GTL_EXCEPTION_HANDLER(exception, message) std::terminate()
#endif

// The generic assertion allows you to specify any exception type you
// prefer, including user-defined exception types.
#define GTL_ASSERT(condition, exception, message) \
if (!(condition)) \
{ \
    std::string sLine = std::to_string(__LINE__); \
    std::string sFile(__FILE__); \
    std::string sFunc(__FUNCTION__); \
    std::string report = sFile+"("+sFunc+","+sLine+"): "+message+"\n"; \
    GTL_EXCEPTION_HANDLER(exception, report); \
}

// The generic error allows you to specify any exception type you
// prefer, including user-defined exception types.
#define GTL_ERROR(exception, message) \
{ \
    std::string sLine = std::to_string(__LINE__); \
    std::string sFile(__FILE__); \
    std::string sFunc(__FUNCTION__); \
    std::string report = sFile+"("+sFunc+","+sLine+"): "+message+"\n"; \
    GTL_EXCEPTION_HANDLER(exception, report); \
}

// Domain errors occur when inputs are outside the domain of an operation.
#define GTL_DOMAIN_ASSERT(condition, message) \
GTL_ASSERT(condition, std::domain_error, message)

#define GTL_DOMAIN_ERROR(message) \
GTL_ERROR(std::domain_error, message)

// Invalid argument errors occur when inputs do not satisfy preconditions
// for function calls.
#define GTL_ARGUMENT_ASSERT(condition, message) \
GTL_ASSERT(condition, std::invalid_argument, message)

#define GTL_ARGUMENT_ERROR(message) \
GTL_ERROR(std::invalid_argument, message)

// Length errors occur when an index exceeds its maximum allowed size.
#define GTL_LENGTH_ASSERT(condition, message) \
GTL_ASSERT(condition, std::length_error, message)

#define GTL_LENGTH_ERROR(message) \
GTL_ERROR(std::length_error, message)

// Logic errors occur when logical preconditions or class invariants are
// violated.
#define GTL_LOGIC_ASSERT(condition, message) \
GTL_ASSERT(condition, std::logic_error, message)

#define GTL_LOGIC_ERROR(message) \
GTL_ERROR(std::logic_error, message)

// Out of range errors occur when trying to access elements out of their
// defined range.
#define GTL_OUTOFRANGE_ASSERT(condition, message) \
GTL_ASSERT(condition, std::out_of_range, message)

#define GTL_OUTOFRANGE_ERROR(message) \
GTL_ERROR(std::out_of_range, message)

// Overflow errors occur when arithmetic operations produce numbers too
// large to be represented by the destination type. Math library functions
// do not throw this exception.
#define GTL_OVERFLOW_ASSERT(condition, message) \
GTL_ASSERT(condition, std::overflow_error, message)

#define GTL_OVERFLOW_ERROR(message) \
GTL_ERROR(std::overflow_error, message)

// Range errors occur when computations cannot be represented by the
// destination type. Math library functions do not throw this exception.
#define GTL_RANGE_ASSERT(condition, message) \
GTL_ASSERT(condition, std::range_error, message)

#define GTL_RANGE_ERROR(message) \
GTL_ERROR(std::range_error, message)

// Runtime errors occur when invalid conditions occur during program
// execution.
#define GTL_RUNTIME_ASSERT(condition, message) \
GTL_ASSERT(condition, std::runtime_error, message)

#define GTL_RUNTIME_ERROR(message) \
GTL_ERROR(std::runtime_error, message)

// Underflow errors occur when floating-point operations produce
// subnormal numbers. Math library functions do not throw this
// exception.
#define GTL_UNDERFLOW_ASSERT(condition, message) \
GTL_ASSERT(condition, std::underflow_error, message)

#define GTL_UNDERFLOW_ERROR(message) \
GTL_ERROR(std::underflow_error, message)
