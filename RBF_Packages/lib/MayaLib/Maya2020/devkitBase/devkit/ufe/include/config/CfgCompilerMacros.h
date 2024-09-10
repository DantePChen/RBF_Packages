//-
// =============================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// =============================================================================
//+

/// \file  CfgCompilerMacros.h
/// \brief Macro definition for identifying compilers and there capabilities.


#ifndef PEPTIDE_CFG_COMPILER_MACROS_H
#define PEPTIDE_CFG_COMPILER_MACROS_H


//==============================================================================
// PLATFORM DETECTION
//==============================================================================

#if defined(_WIN32)
#define PEPTIDE_WINDOWS
#elif defined(__linux__)
#define PEPTIDE_LINUX
#elif defined(__APPLE__)
#define PEPTIDE_OSX
#else
#error "Unsupported platform..."
#endif


//==============================================================================
// COMPILER DETECTION
//==============================================================================

#if defined(__clang__)
//  Clang C++ emulates GCC and MSC, so it has to appear early.
#define PEPTIDE_IS_CLANG 1
#elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC)
//  Intel emulates MSC, so it has to appear early.
#define PEPTIDE_IS_INTEL 1
#elif defined(__GNUC__)
//  GNU C++.
#define PEPTIDE_IS_GCC   1
#elif defined(_MSC_VER)
//  Microsoft visual studio
#define PEPTIDE_IS_MSC   1
#else
#error "Unknown compiler configuration..."
#endif


// Clang Compiler versions.
//
#ifdef PEPTIDE_IS_CLANG

#ifdef __apple_build_version__

// Apple versions of clang. Note the Apple version naming scheme is unrelated
// to the official LLVM ones. Thus, we made no attempt unify these versioning
// schemes.

#if (__clang_major__ <= 6)
#define PEPTIDE_AT_MOST_APPLECLANG60 1
#endif

#if (__clang_major__ <= 7)
#define PEPTIDE_AT_MOST_APPLECLANG70 1
#endif

#else

// Official releases of clang
#if (__clang_major__ < 3) || ((__clang_major__ == 3) && (__clang_minor__ <= 6))
#define PEPTIDE_AT_MOST_CLANG36 1
#endif

#if (__clang_major__ < 3) || ((__clang_major__ == 3) && (__clang_minor__ <= 7))
#define PEPTIDE_AT_MOST_CLANG37 1
#endif

#if (__clang_major__ < 3) || ((__clang_major__ == 3) && (__clang_minor__ <= 8))
#define PEPTIDE_AT_MOST_CLANG38 1
#endif

#endif

#endif


// GCC Compiler versions.
//
// Note: these macros are also used for the Intel compiler when it is
// trying to emulate a particular version of the gcc compiler.
#ifdef PEPTIDE_IS_GCC

#if (__GNUC__ < 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ <= 8))
#define PEPTIDE_AT_MOST_GCC48 1
#endif

#endif


// MSC Compiler versions
#ifdef PEPTIDE_IS_MSC

#if _MSC_VER <= 1700
#define PEPTIDE_AT_MOST_MSVC2012 1
#endif

#if _MSC_VER <= 1800
#define PEPTIDE_AT_MOST_MSVC2013 1
#endif

#if _MSC_VER <= 1900
#define PEPTIDE_AT_MOST_MSVC2015 1
#endif

#endif


// Default values...
#ifndef PEPTIDE_IS_CLANG
#define PEPTIDE_IS_CLANG 0
#endif

#ifndef PEPTIDE_IS_GCC
#define PEPTIDE_IS_GCC 0
#endif

#ifndef PEPTIDE_IS_INTEL
#define PEPTIDE_IS_INTEL 0
#endif

#ifndef PEPTIDE_IS_MSC
#define PEPTIDE_IS_MSC 0
#endif

#ifndef PEPTIDE_AT_MOST_APPLECLANG60
#define PEPTIDE_AT_MOST_APPLECLANG60 0
#endif

#ifndef PEPTIDE_AT_MOST_APPLECLANG70
#define PEPTIDE_AT_MOST_APPLECLANG70 0
#endif

#ifndef PEPTIDE_AT_MOST_CLANG36
#define PEPTIDE_AT_MOST_CLANG36 0
#endif

#ifndef PEPTIDE_AT_MOST_CLANG37
#define PEPTIDE_AT_MOST_CLANG37 0
#endif

#ifndef PEPTIDE_AT_MOST_CLANG38
#define PEPTIDE_AT_MOST_CLANG38 0
#endif

#ifndef PEPTIDE_AT_MOST_GCC48
#define PEPTIDE_AT_MOST_GCC48 0
#endif

#ifndef PEPTIDE_AT_MOST_MSVC2012
#define PEPTIDE_AT_MOST_MSVC2012 0
#endif

#ifndef PEPTIDE_AT_MOST_MSVC2013
#define PEPTIDE_AT_MOST_MSVC2013 0
#endif

#ifndef PEPTIDE_AT_MOST_MSVC2015
#define PEPTIDE_AT_MOST_MSVC2015 0
#endif


//==============================================================================
// FEATURE DETECTION
//==============================================================================

// Stub out __has_feature when not supported by a given compiler
#ifndef __has_feature
#define __has_feature(x) 0
#endif

//==============================================================================
// COMPILER CAPABILITIES
//==============================================================================

#if PEPTIDE_AT_MOST_MSVC2013
    // Neither MSVC 2012 nor 2013 support the C++11 noexcept keyword
    #define PEPTIDE_NOEXCEPT
#else
    /// \brief noexcept keyword
    ///
    /// Defined to "noexcept" if the compiler supports the C++11 noexcept
    /// keyword; empty otherwise.
    ///
    /// Clang, GCC and MSVC 2015 all support the C++11 noexcept keyword. MSVC
    /// 2012 and 2013 do not.
    #define PEPTIDE_NOEXCEPT noexcept
#endif

#if PEPTIDE_AT_MOST_MSVC2013
    // Neither MSVC 2012 nor 2013 support the C++11 constexpr keyword
    #define PEPTIDE_CONSTEXPR
#else
    /// \brief constexpr keyword
    ///
    /// Defined to "constexpr" if the compiler supports the C++11 constexpr
    /// keyword; empty otherwise.
    ///
    /// Clang, GCC and MSVC 2015 all support the C++11 constexpr keyword. MSVC
    /// 2012 and 2013 do not.
    #define PEPTIDE_CONSTEXPR constexpr
#endif

/// \brief Probe for C++ RTTI support
///
/// Set to `1` if the compiler supports C++ RTTI; `0` otherwise. This depends
/// on the compiler flags being used.
#if __has_feature(cxx_rtti) || defined(__GXX_RTTI) || defined(_CPPRTTI)
#define PEPTIDE_HAS_CXX_RTTI 1
#else
#define PEPTIDE_HAS_CXX_RTTI 0
#endif

#if !PEPTIDE_IS_MSC || !PEPTIDE_AT_MOST_MSVC2012

/// \brief Probe for C++11 Template Aliases support
///
/// Set to `1` if the compiler supports C++11 Template Aliases; `0` otherwise.
///
/// Clang, GCC and MSVC 2013 all support C++11 template aliases. These
/// are also the signatures used for the Doxygen documentation.
#define PEPTIDE_HAS_TEMPLATE_ALIASES 1

/// \brief Probe for C++11 Explicit Conversion support
///
/// Set to `1` if the compiler supports C++11 Explicit Conversion; `0`
/// otherwise.
///
/// Clang, GCC and MSVC 2013 all support C++11 template aliases. These
/// are also the signatures used for the Doxygen documentation.
#define PEPTIDE_HAS_EXPLICIT_CONVERSION_OPERATORS 1

/// \brief Probe for C++11 Variadic Template support
///
/// Set to `1` if the compiler supports C++11 Variadic Template; `0`
/// otherwise.
///
/// Clang, GCC and MSVC 2013 all support C++11 template aliases. These
/// are also the signatures used for the Doxygen documentation.
#define PEPTIDE_HAS_VARIADIC_TEMPLATE 1

/// \brief Probe for C++11 Defaulted Functions
///
/// Set to `1` if the compiler supports C++11 defaulted functions; `0`
/// otherwise.
///
/// Clang, GCC and MSVC 2013 all support C++11 defaulted
/// functions. These are also the signatures used for the Doxygen
/// documentation.
#define PEPTIDE_HAS_DEFAULTED_FUNCTIONS 1

/// \brief Probe for C++11 Deleted Functions
///
/// Set to `1` if the compiler supports C++11 deleted functions; `0`
/// otherwise.
///
/// Clang, GCC and MSVC 2013 all support C++11 deleted
/// functions. These are also the signatures used for the Doxygen
/// documentation.
#define PEPTIDE_HAS_DELETED_FUNCTIONS 1

#else

// MSVC 2012 does not support C++11 template aliases.
#define PEPTIDE_HAS_TEMPLATE_ALIASES 0

// MSVC 2012 does not support C++11 explicit conversion operators.
#define PEPTIDE_HAS_EXPLICIT_CONVERSION_OPERATORS 0

// MSVC 2012 does not support C++11 variadic templates.
#define PEPTIDE_HAS_VARIADIC_TEMPLATE 0

// MSVC 2012 does not support C++11 defaulted functions.
#define PEPTIDE_HAS_DEFAULTED_FUNCTIONS 0

// MSVC 2012 does not support C++11 deleted functions.
#define PEPTIDE_HAS_DELETED_FUNCTIONS 0

#endif


/// \brief Force function inlining
///
/// This will provide a hint to the compiler that a method must be inline.
/// Please refer to your compiler's documentation for additional information
/// regarding how "force inline" behave for your compiler.
#if PEPTIDE_IS_MSC || PEPTIDE_IS_INTEL

#define PEPTIDE_FORCEINLINE __forceinline

#elif PEPTIDE_IS_GCC || PEPTIDE_IS_CLANG

#define PEPTIDE_FORCEINLINE inline __attribute__((always_inline))

#endif


//==============================================================================
// COMPILER WORKAROUNDS
//==============================================================================

//------------------------------------------------------------------------------
//
#if !PEPTIDE_HAS_VARIADIC_TEMPLATE

/// \brief Helper macros to simulate variadic templates
/// \{
#define PEPTIDE_APPLY_0(APPLY, JOIN)
#define PEPTIDE_APPLY_1(APPLY, JOIN)                                  APPLY(1)
#define PEPTIDE_APPLY_2(APPLY, JOIN) JOIN(PEPTIDE_APPLY_1(APPLY,JOIN),APPLY(2))
#define PEPTIDE_APPLY_3(APPLY, JOIN) JOIN(PEPTIDE_APPLY_2(APPLY,JOIN),APPLY(3))
#define PEPTIDE_APPLY_4(APPLY, JOIN) JOIN(PEPTIDE_APPLY_3(APPLY,JOIN),APPLY(4))
#define PEPTIDE_APPLY_5(APPLY, JOIN) JOIN(PEPTIDE_APPLY_4(APPLY,JOIN),APPLY(5))
#define PEPTIDE_APPLY_6(APPLY, JOIN) JOIN(PEPTIDE_APPLY_5(APPLY,JOIN),APPLY(6))
#define PEPTIDE_APPLY_7(APPLY, JOIN) JOIN(PEPTIDE_APPLY_6(APPLY,JOIN),APPLY(7))
#define PEPTIDE_APPLY_8(APPLY, JOIN) JOIN(PEPTIDE_APPLY_7(APPLY,JOIN),APPLY(8))

#define PEPTIDE_VARIADIC_TEMPLATE_EXPAND(CODE_MACRO)    \
    CODE_MACRO(PEPTIDE_APPLY_0)                         \
    CODE_MACRO(PEPTIDE_APPLY_1)                         \
    CODE_MACRO(PEPTIDE_APPLY_2)                         \
    CODE_MACRO(PEPTIDE_APPLY_3)                         \
    CODE_MACRO(PEPTIDE_APPLY_4)                         \
    CODE_MACRO(PEPTIDE_APPLY_5)                         \
    CODE_MACRO(PEPTIDE_APPLY_6)                         \
    CODE_MACRO(PEPTIDE_APPLY_7)                         \
    CODE_MACRO(PEPTIDE_APPLY_8)

#define PEPTIDE_JOIN_COMMA(A,B) A, B
#define PEPTIDE_JOIN(A,B)       A B

#define PEPTIDE_TPARAMS(N)   , class A##N
#define PEPTIDE_RV_PARAMS(N) A##N&& a##N
#define PEPTIDE_FWD_ARGS(N)  std::forward<A##N>(a##N)

#define PEPTIDE_TPARAMS_EXPAND(ARGS)   ARGS(PEPTIDE_TPARAMS,  PEPTIDE_JOIN)
#define PEPTIDE_RV_PARAMS_EXPAND(ARGS) ARGS(PEPTIDE_RV_PARAMS,PEPTIDE_JOIN_COMMA)
#define PEPTIDE_FWD_ARGS_EXPAND(ARGS)  ARGS(PEPTIDE_FWD_ARGS, PEPTIDE_JOIN_COMMA)
/// \}

#endif

//==============================================================================
// EXPORT DIRECTIVES
//==============================================================================

/// \brief Marks a symbol as being externally visible on UNIX platforms
///
/// \warning This macro is only required in special cases. One of those cases
///          is when manually exported templates must be declared as externally
///          visible for the compiler to instantiate them.
#if PEPTIDE_IS_GCC || PEPTIDE_IS_CLANG
#define PEPTIDE_EXPORT_UNIX __attribute__((visibility("default")))
#else
#define PEPTIDE_EXPORT_UNIX
#endif

#endif
