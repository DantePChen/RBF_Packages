// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

//
/// \file ufeExport.h
///
/// \brief Definition of macros for symbol visibility.
///


#ifndef _ufeExport
#define _ufeExport

#if defined(WIN32)
    #define UFE_EXPORT _declspec(dllexport)
    #define UFE_IMPORT _declspec(dllimport)
#elif defined(__GNUC__)
    #define UFE_EXPORT  __attribute__ ((visibility("default")))
    #define UFE_IMPORT  __attribute__ ((visibility("default")))
#else
    #define UFE_EXPORT
    #define UFE_IMPORT
#endif


#if defined( UFE_API_IMPL )
    #define UFE_SDK_DECL UFE_EXPORT
    #define UFE_EXTRA_DECL UFE_EXPORT
#else
    #define UFE_SDK_DECL UFE_IMPORT
    #define UFE_EXTRA_DECL UFE_IMPORT
#endif

// Symbol versioning.
#include "../ufe.h"

#endif // _ufeExport
