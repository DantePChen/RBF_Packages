//
// ==================================================================
// Copyright 2017 Autodesk, Inc.  All rights reserved.
// 
// This computer source code  and related  instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are  protected  under applicable  copyright and trade secret
// law. They may not  be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
// ==================================================================
//
//
//! \file synColor/ioFormat.h
//!
//! \brief List of io formats and related utilities.
//!

#ifndef _SYNCOLOR_IO_FORMAT_H
#define _SYNCOLOR_IO_FORMAT_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

SYN_VISIBILITY_PUSH

namespace SYNCOLOR
{
  //! \brief List of IO formats.
  //! 
  enum IOFormat
  {
     IO_FORMAT_CTF = 0, // Autodesk color transform file format
     IO_FORMAT_CLF,     // Academy/ASC Common LUT format
     IO_FORMAT_CDL,     // ASC CDL color decision list XML format
     IO_FORMAT_CCC,     // ASC CDL color correction collection XML format
     IO_FORMAT_CC,      // CDL fragment format
     IO_FORMAT_LUT,     // Discreet 1d-LUT format
     IO_FORMAT_VLT,     // Panasonic 3d-LUT format
     IO_FORMAT_ICC      // ICC Profile format
  };
}

SYN_VISIBILITY_POP

#endif //_SYNCOLOR_IO_FORMAT_H
