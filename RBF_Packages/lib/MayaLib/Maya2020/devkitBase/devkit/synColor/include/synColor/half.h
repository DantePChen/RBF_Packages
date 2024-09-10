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
//
//! \file half.h
//!
//! \brief List of flags that describe 1-D lut index and value encoding.
//!
//!

#ifndef _SYNCOLOR_HALF_H
#define _SYNCOLOR_HALF_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

SYN_VISIBILITY_PUSH

namespace SYNCOLOR
{

  //! \brief List of flags that describe 1-D LUT index and value encoding.
  //!
  //! 1-D LUT indices and values can either be expressed in standard numeric
  //! encodings or using half float codes.   Half float codes are 16 bit integer
  //! representations of a 16 bit floating point value. See:
  //! http://en.wikipedia.org/wiki/Half-precision_floating-point_format.
  //!
  enum HalfFlags{
    LUT_STANDARD = 0x00,          //!< Indices and values use standard numeric encodings.
    LUT_INPUT_HALF_CODE = 0x01,   //!< LUT indices are half float codes.
    LUT_OUTPUT_HALF_CODE = 0x02,  //!< LUT values are half float codes.
    LUT_INPUT_OUTPUT_HALF_CODE =
      LUT_INPUT_HALF_CODE |
      LUT_OUTPUT_HALF_CODE  //!< Both indices and values are half float codes.
  };
}

SYN_VISIBILITY_POP

#endif
