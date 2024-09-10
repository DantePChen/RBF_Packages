//
// ==================================================================
// Copyright 2014 Autodesk, Inc.  All rights reserved.
//
// This computer source code  and related  instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are  protected  under applicable  copyright and trade secret
// law. They may not  be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
// ==================================================================
//
//! \file synColor/synExport.h
//!
//! \brief Definition of macros for library symbol visibility.
//!
#ifdef SYN_LINK_STATIC

   #define SYN_HIDDEN
   #define SYN_EXPORT
   #define SYN_VISIBILITY_PUSH
   #define SYN_VISIBILITY_POP

#elif defined _WIN32

   #define SYN_HIDDEN
   #ifdef SYN_BUILD_DLL
     #define SYN_EXPORT __declspec(dllexport)
   #else
     #define SYN_EXPORT __declspec(dllimport)
   #endif
   #define SYN_VISIBILITY_PUSH
   #define SYN_VISIBILITY_POP

#elif __GNUC__ >= 4

   #define SYN_EXPORT __attribute__ ((visibility ("default")))
   #define SYN_HIDDEN __attribute__ ((visibility ("hidden")))
   #define SYN_VISIBILITY_PUSH
   #define SYN_VISIBILITY_POP

#else

   #define SYN_EXPORT
   #define SYN_HIDDEN
   #define SYN_VISIBILITY_PUSH _Pragma( "GCC visibility push(default)" )
   #define SYN_VISIBILITY_POP  _Pragma( "GCC visibility pop" )

#endif

