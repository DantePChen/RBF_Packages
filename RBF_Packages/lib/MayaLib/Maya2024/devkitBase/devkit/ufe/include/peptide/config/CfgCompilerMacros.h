//-
// =============================================================================
// Copyright 2021 Autodesk, Inc. All rights reserved.
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


// MSC Compiler versions
#ifdef PEPTIDE_IS_MSC

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
// COMPATIBILITY ACROSS C++ STANDARDS.
//==============================================================================

/// \brief Lambda capture of 'this' in C++20.
///
/// This macro is used to solve the following issue between C++20 and previous
/// versions of the C++ standard.
///
/// \code{.cpp}
///   auto  x = [=] {         // Deprecated from C++20:
///      this->foo();
///   };
///
///   auto y = [=, this] {    // Recommended method from C++20:
///       this->foo();        // [=] The this pointer will not be captured, so
///                           // capture with specifying this
///       // Unfortunately, VS2019 emits this error in C++17 and it can't be
///       // disabled!
///       //
///       // error C3791: 'this' cannot be explicitly captured when the default
///       capture
///       // mode is by copy (=)
///   };
///
///   auto z = [= PEPTIDE_CXX20_CAPTURE_THIS] {   // Works everywhere!
///      this->foo();
///   };
/// \endcode
///
/// \see https://developercommunity.visualstudio.com/t/c20-deprecated-lambda-capture/420398
#if __cplusplus >= 202002L
#define PEPTIDE_CXX20_CAPTURE_THIS , this
#else
#define PEPTIDE_CXX20_CAPTURE_THIS
#endif

//==============================================================================
// COMPILER CAPABILITIES
//==============================================================================

/// \brief Probe for C++ RTTI support
///
/// Set to `1` if the compiler supports C++ RTTI; `0` otherwise. This depends
/// on the compiler flags being used.
#if __has_feature(cxx_rtti) || defined(__GXX_RTTI) || defined(_CPPRTTI)
#define PEPTIDE_HAS_CXX_RTTI 1
#else
#define PEPTIDE_HAS_CXX_RTTI 0
#endif

/// \brief Force function inlining
///
/// This macro provides a hint to the compiler that a function must be inlined.
/// Please refer to your compiler's documentation for additional information
/// regarding how "force inline" behave for your compiler.
#if PEPTIDE_IS_MSC || PEPTIDE_IS_INTEL

#define PEPTIDE_FORCEINLINE __forceinline

#elif PEPTIDE_IS_GCC || PEPTIDE_IS_CLANG

#define PEPTIDE_FORCEINLINE inline __attribute__((always_inline))

#endif

/// \brief Prevent function inlining
///
/// This macro provides a hint to the compiler that a function shouldn't be
/// inlined. This can be useful if the function is on a cold path and unlikely
/// to be called. Please refer to your compiler's documentation for additional
/// information regarding how "noinline" behave for your compiler.
///
/// \todo FIXME: Can't find a syntax to prevent the Intel C++ compiler from
/// inlining a specific function.
///
/// \todo FIXME: This should probably be replaced with the C++20 [[unlikely]]
/// attribute once it is supported by every compiler. More investigation is
/// required to ensure that this will lead to the compiler performing similar
/// optimizations.
#if PEPTIDE_IS_MSC

#define PEPTIDE_NOINLINE __declspec(noinline)

#elif PEPTIDE_IS_GCC || PEPTIDE_IS_CLANG || PEPTIDE_IS_INTEL

#define PEPTIDE_NOINLINE __attribute__((noinline))

#endif

/// \brief Enable Empty Base Class Optimization (ECBO)
///
/// This is used to enable ECBO for a given class being declared. For example:
///
/// \code{.cpp}
///     class PEPTIDE_ENABLE_ECBO MyClass : public EmptyBassClass,
///                                         public OtherClass
///     {
///        ...
///     };
/// \endcode
///
/// \see
/// https://devblogs.microsoft.com/cppblog/optimizing-the-layout-of-empty-base-classes-in-vs2015-update-2-3
#if PEPTIDE_IS_MSC
#define PEPTIDE_ENABLE_ECBO __declspec(empty_bases)
#else
#define PEPTIDE_ENABLE_ECBO
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
