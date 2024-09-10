//-
// =============================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// =============================================================================
//+

/// \file  CfgWarningMacros.h
/// \brief This file defines the generic exclusion for warnings
///
/// Here's an example of how to use the macros defined in this file:
///
/// \code
///      // Push the current set of warnings.
///      PEPTIDE_WARNING_PUSH
///
///      // Disable this particular warning when compiling using the
///      // MSC compiler.
///      PEPTIDE_WARNING_DISABLE_MSC(4267)
///
///      /* Code for which we want to disable a particular warning. */
///      ...
///
///      // Restore the previous set of warnings.
///      PEPTIDE_WARNING_POP
/// \endcode
///
/// \see BaPushLLVMWarnings.h BaPopLLVMWarnings.h


#ifndef PEPTIDE_CFG_WARNINGS_MACROS_H
#define PEPTIDE_CFG_WARNINGS_MACROS_H

#include "CfgCompilerMacros.h"


/// \brief Helper defining the various warning macros.
///
/// It encapsulates the compiler differences. For gcc and clang, the macro uses
/// the C99 _Pragma() preprocessor directive. It also takes care of stringizing
/// the argument passed to _Pragma(). For MSC, this is using the Microsoft
/// specific __pragma() preprocessor extension.
#if PEPTIDE_IS_CLANG || PEPTIDE_IS_GCC
#define PEPTIDE_DO_PRAGMA(a) _Pragma(#a)
#elif PEPTIDE_IS_MSC
#define PEPTIDE_DO_PRAGMA(a) __pragma(a)
#else
#define PEPTIDE_DO_PRAGMA(a) /* empty */
#endif


/// \brief Warning Push
///
/// This is to be called before a collection of warnings which are to
/// be ignored, and when used should be closed with the
/// PEPTIDE_WARNING_POP.
///
/// Any additional compilers that are added to this macro should have
/// an equivalent statement included in the disable and pop macro
/// respectively.
#if PEPTIDE_IS_CLANG || PEPTIDE_IS_GCC
    #define PEPTIDE_WARNING_PUSH PEPTIDE_DO_PRAGMA(GCC diagnostic push)
#elif PEPTIDE_IS_MSC
    #define PEPTIDE_WARNING_PUSH PEPTIDE_DO_PRAGMA( warning( push ) )
#else
    #define PEPTIDE_WARNING_PUSH
#endif


/// \brief Warning Disable
///
/// This macro should be called within a compiler or OS specific set
/// of warnings, as it does not distinguish which warning identifiers
/// correspond to which compiler. It can be called once for every
/// warning to be disabled.
///
/// Assure that any call of this macro is surrounded with a push and
/// pop macro.
#if PEPTIDE_IS_CLANG || PEPTIDE_IS_GCC
    #define PEPTIDE_WARNING_DISABLE(a) PEPTIDE_DO_PRAGMA(GCC diagnostic ignored #a)
#elif PEPTIDE_IS_MSC
    #define PEPTIDE_WARNING_DISABLE(a) PEPTIDE_DO_PRAGMA( warning( disable : a ) )
#else
    #define PEPTIDE_WARNING_DISABLE(a)
#endif


/// \brief Disable specific warnings on specific compilers
/// \{
#if PEPTIDE_IS_MSC
    #define PEPTIDE_WARNING_DISABLE_MSC(a) PEPTIDE_WARNING_DISABLE(a)
#else
    #define PEPTIDE_WARNING_DISABLE_MSC(a)
#endif

#if PEPTIDE_IS_CLANG
    #define PEPTIDE_WARNING_DISABLE_CLANG(a) PEPTIDE_WARNING_DISABLE(a)
#else
    #define PEPTIDE_WARNING_DISABLE_CLANG(a)
#endif

#if PEPTIDE_IS_GCC
    #define PEPTIDE_WARNING_DISABLE_GCC(a) PEPTIDE_WARNING_DISABLE(a)
#else
    #define PEPTIDE_WARNING_DISABLE_GCC(a)
#endif
/// \}

/// \brief Warning Pop
///
/// This will end a selection of disabled warnings, and must match to
/// PEPTIDE_WARNING_PUSH, after a selection (possibly empty) of
/// PEPTIDE_WARNING_DISABLE.
#if PEPTIDE_IS_CLANG || PEPTIDE_IS_GCC
    #define PEPTIDE_WARNING_POP PEPTIDE_DO_PRAGMA(GCC diagnostic pop)
#elif PEPTIDE_IS_MSC
    #define PEPTIDE_WARNING_POP PEPTIDE_DO_PRAGMA( warning( pop ) )
#else
    #define PEPTIDE_WARNING_POP
#endif

/// \brief Warning Push System/Library Header
///
///    This is to be called when all warnings should be ignored because we are
///    about to parse a system or a library header. The assumption is that there
///    is nothing that can be done about these, so why bother... When used, it
///    should be closed with a matching PEPTIDE_WARNING_POP_SYSTEM_HEADER_MSC.
///
/// \warning This macros is only implemented for the MSC compiler. The other
/// compilers support system header through command line options. This is well
/// integrated and supported by CMake and this mechanisms should be used
/// instead.
#if PEPTIDE_IS_MSC
    #define PEPTIDE_WARNING_PUSH_SYSTEM_HEADER_MSC PEPTIDE_DO_PRAGMA( warning(push, 0) )
#else
    #define PEPTIDE_WARNING_PUSH_SYSTEM_HEADER_MSC
#endif


/// \brief Warning Pop System/Library Header
///
/// This will end a section of disabled warnings, and must match to
/// PEPTIDE_WARNING_PUSH_SYSTEM_HEADER.
#if PEPTIDE_IS_MSC
    #define PEPTIDE_WARNING_POP_SYSTEM_HEADER_MSC PEPTIDE_DO_PRAGMA( warning( pop ) )
#else
    #define PEPTIDE_WARNING_POP_SYSTEM_HEADER_MSC
#endif

#endif
