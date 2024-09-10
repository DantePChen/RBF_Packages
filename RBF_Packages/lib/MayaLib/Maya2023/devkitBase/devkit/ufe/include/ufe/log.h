#ifndef _log
#define _log
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include <string>
#include "common/ufeExport.h"

#ifdef WIN32
    #define UFE_LOG(MSG) \
        ( Ufe::log(__FUNCTION__, __FILE__, __LINE__, MSG) )
#else
    #define UFE_LOG(MSG) \
        ( Ufe::log(__func__,     __FILE__, __LINE__, MSG) )
#endif

UFE_NS_DEF
{
// \brief Log message.
//
// This function sends to standard error the logged message, the
// filename, line number, and function where the log was requested.
//
// \param func The function where the log was requested.
// \param filename The filename where the log was requested.
// \param lineno The line number where the log was requested.
// \param msg Logged message.
UFE_SDK_DECL void log(const char*        func,
         const char*        filename,
         int                lineno,
         const std::string& msg);

} // UFE_NS_DEF

#endif /* _log */
