//
// ==================================================================
// Copyright 2016 Autodesk, Inc.  All rights reserved.
// 
// This computer source code  and related  instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are  protected  under applicable  copyright and trade secret
// law. They may not  be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
// ==================================================================
//
//! \file transformTypes.h

#ifndef _SYNCOLOR_TRANSFORM_TYPES_H
#define _SYNCOLOR_TRANSFORM_TYPES_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

SYN_VISIBILITY_PUSH

namespace SYNCOLOR
{
   //! Enumeration of all supported Lut3DO algorithms
   //! for the interpolation computation
   enum Lut3DOpInterpolationAlgo
   {
      LUT3D_ALGO_DEFAULT = 0,     //!< Default algorithm
      LUT3D_ALGO_TRILINEAR,       //!< Trilinear interpolation's algorithm
      LUT3D_ALGO_TETRAHEDRAL,     //!< Tetrahedral interpolation's algorithm 
      LUT3D_ALGO_4PT_TETRAHEDRAL, //!< 4 points Tetrahedral interpolation's algorithm 
   };

   //! Enumeration of the CDL styles
   enum CDLStyle
   {
      CDL_V1_2_FWD = 0,  //!< Forward (version 1.2) style
      CDL_V1_2_REV,      //!< Reverse (version 1.2) style
      CDL_NO_CLAMP_FWD,  //!< Forward no clamping style
      CDL_NO_CLAMP_REV   //!< Reverse no clamping style
   };
}

SYN_VISIBILITY_POP

#endif //_SYNCOLOR_TRANSFORM_TYPES_H
