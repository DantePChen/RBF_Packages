#ifndef _ufeAssert
#define _ufeAssert
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"

#if !defined(NDEBUG)
#define UFE_ENABLE_ASSERTS
#endif

// \brief Customized assertion macro that can be explicitly enabled on non-debug builds.
//
// When UFE_ENABLE_ASSERTS is defined, the macro UFE_ASSERT_MSG executes a
// custom assertion check, even on non-debug builds. If UFE_ENABLE_ASSERTS
// is not defined, UFE_ASSERT_MSG does nothing.
//
// The macro UFE_ASSERT_MSG also allows a description of the assertion error
// to be emitted when a failure occurs.
//
// \param EXPR The expression to assert
// \param MSG The description of the assertion error
#ifdef UFE_ENABLE_ASSERTS
    #ifdef WIN32
        #define UFE_ASSERT_MSG(EXPR, MSG) \
            ( (EXPR) ? (void)0 : Ufe::handleAssert(__FUNCTION__, __FILE__, __LINE__, #EXPR, MSG) )
    #else
        #define UFE_ASSERT_MSG(EXPR, MSG) \
            ( (EXPR) ? (void)0 : Ufe::handleAssert(__func__,     __FILE__, __LINE__, #EXPR, MSG) )
    #endif
    #define UFE_ASSERT_COMPILED(CODE) CODE
#else
    #define UFE_ASSERT_MSG(EXPR, MSG)
    #define UFE_ASSERT_COMPILED(CODE)
#endif

/// \brief Similar to UFE_ASSERT_MSG, but without the description of the assertion error.
/// \param EXPR The expression to assert
/// \see UFE_ASSERT_MSG
#define UFE_ASSERT(EXPR)  UFE_ASSERT_MSG(EXPR, nullptr)

UFE_NS_DEF
{
// \brief Handle the custom assertion action.
//
// This function display a message containing the failing expression, the
// filename, line number, and function where the assertion failure occurred. It
// also display an optional message description of the assertion failure.
//
// In order to avoid the execution on a potential unstable environment an
// exception is also thrown after the assertion error is displayed.
//
// \note This function only display the assertion message. The actual expression
//       check is performed in the UFE_ASSERT_MSG macro.
//
// \param func The function where the assertion occurred.
// \param filename The filename where the assertion occurred.
// \param lineno The line number where the assertion occurred.
// \param expr The expression that caused the assertion failure.
// \param msg Custom message description to be printed.
UFE_SDK_DECL void handleAssert(const char* func,
                  const char* filename,
                  int         lineno,
                  const char* expr,
                  const char* msg);

} // UFE_NS_DEF

#endif /* _ufeAssert */
