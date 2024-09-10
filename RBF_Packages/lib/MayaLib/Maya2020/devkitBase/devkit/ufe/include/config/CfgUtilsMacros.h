//-
// =============================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// =============================================================================
//+

/// \file  CfgUtilsMacros.h
/// \brief General purpose macros

#ifndef PEPTIDE_CPPUTILS_MACROS_H
#define PEPTIDE_CPPUTILS_MACROS_H

#include "CfgCompilerMacros.h"

#include <cassert>

/// \brief Helper macro to force the use of a semi-colon at the end of
///        macro invocation.
///
///        It will work for declaration areas as well, so it can be used
///        when defining macros for creating methods, functions, etc.
#define PEPTIDE_END_MACRO     static_assert(true, "")

/// \brief The identity macro
///
/// For example, it can be used for silencing clang-tidy misc-macro-parentheses
/// warnings!
#define PEPTIDE_IDENTITY(X) X

/// \def PEPTIDE_UNREACHABLE(MSG)
///
/// \brief Macro indicating that the code path is unreachable
///
/// Macro indicating that the code path is unreachable. The code path might be
/// unreachable for various reasons: class invariants, all cases covered by a
/// switch statement, etc...
///
/// In debug builds, an assertion is raised if the macro is ever executed. In
/// optimized builds, an intrinsic is used to tell the optimizer that the code
/// path can be entirely pruned out. That often leads to significantly more
/// efficient code being generated.
#if PEPTIDE_IS_CLANG || PEPTIDE_IS_GCC

// NOLINT to silence up clang-tidy warning:
// warning: found assert() that could be replaced by static_assert() [cert-dcl03-c]
#define PEPTIDE_UNREACHABLE(MSG)           \
    assert(false && MSG);     /* NOLINT */ \
    __builtin_unreachable();

#elif PEPTIDE_IS_MSC

#define PEPTIDE_UNREACHABLE(MSG) \
    assert(false && MSG);        \
    __assume(false);

#else
#error "PEPTIDE_UNREACHABLE(): Unsupported compiler!"
#define PEPTIDE_UNREACHABLE(MSG) assert(false && MSG);
#endif


/// \def PEPTIDE_DEPRECATED(REASON)
///
/// \brief Macro indicating that a declaration is deprecated
///
/// The macro can be used to indicate that a declaration is deprecated and
/// shouldn't be used anymore in new code. The deprecated declaration could be
/// a function or a class, for example. The macro also allows one to provide an
/// explanation concerning the deprecation.
///
/// For example:
///
/// \code{.cpp}
///     #ifndef DOXYGEN
///        PEPTIDE_DEPRECATED("MAYA-1234 Use newerAndBetterFunction() instead.")
///     #endif
///        void aFunctionNowObsolete();
/// \endcode
///
/// \warning The DOXYGEN guard is necessary to avoid the following bogus
///          Doxygen warning: _error: Found ';' while parsing initializer list!
///          (doxygen could be confused by a macro call without semicolon)
///          (warning treated as error, aborting now)_
///
/// \note The deprecation warning is disabled on MSC because it leads to an
///       error being reported which causes the entire build to fail. This is
///       due to the presence of the "warning as errors (/WX)" flag. There
///       doesn't seem to be a way to turn off the warning as errors for a
///       single specific warning.
#if PEPTIDE_IS_MSC
#define PEPTIDE_DEPRECATED(REASON)
#else
#define PEPTIDE_DEPRECATED(REASON) [[deprecated(REASON)]]
#endif

#endif
