//
// ==================================================================
// Copyright 2013 Autodesk, Inc.  All rights reserved.
// 
// This computer source code  and related  instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are  protected  under applicable  copyright and trade secret
// law. They may not  be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
// ==================================================================
//
//! \file synColor/resolveFlags.h
//!
//! \brief This file contains the flags for the resolve of references
//!

#ifndef _SYNCOLOR_RESOLVE_FLAGS_H
#define _SYNCOLOR_RESOLVE_FLAGS_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

SYN_VISIBILITY_PUSH

namespace SYNCOLOR
{
  //! These flags control the way "Current Monitor" reference tag is resolved
  enum ResolveFlags
  {
    RESOLVE_GRAPHICS_MONITOR   = 0, //!< Resolve to the graphic monitor
    RESOLVE_BROADCAST_MONITOR       //!< Resolve to the broadcast monitor
  };

};

SYN_VISIBILITY_POP

#endif
