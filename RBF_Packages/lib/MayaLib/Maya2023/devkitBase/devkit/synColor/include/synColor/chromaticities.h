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
//! \file synColor/chromaticities.h
//!
//! \brief Chromaticities declarations
//!

#ifndef _SYNCOLOR_CHROMATICITIES_H
#define _SYNCOLOR_CHROMATICITIES_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

SYN_VISIBILITY_PUSH


namespace SYNCOLOR
{
  //------------------------------------------------------------------------------------------------
  //
  //! Chromaticity coordinates
  //
  struct SYN_EXPORT ChromaticityCoordinates
  {
    ChromaticityCoordinates() : x(0.0), y(0.0) {}
    float x; //!< The x of the {x, y} coordinate
    float y; //!< The y of the {x, y} coordinate
  };

  //------------------------------------------------------------------------------------------------
  //
  //! All the chromaticities
  //!
  //! \note Useful link to have some explanations about chromaticity purpose
  //!       https://en.wikipedia.org/wiki/CIE_1931_color_space#CIE_xy_chromaticity_diagram_and_the_CIE_xyY_color_space
  //!
  //! \note The xy chromaticity coordinates for Red, Green, Blue, and White are used 
  //!       in the OpenEXR file format and many other places as a way of defining the primaries 
  //!       and white point of a linear color space.
  //!       Dividing a three-dimensional color by its magnitude forms a projection onto 
  //!       a two-dimensional xy chromaticity plane. This makes for an easy way to compare 
  //!       the gamuts allowed by various primary sets (i.e., the graphs you've no doubt seen 
  //!       with several triangles inside the horseshoe-shaped spectral locus).
  //!       Although the standard white for video is D65, we sometimes need to deal 
  //!       with other white points (e.g., D60 for ACES). Hence we need to communicate 
  //!       the white point as well.
  //!
  struct SYN_EXPORT Chromaticities
  {
    ChromaticityCoordinates red;   //!< The red coordinates
    ChromaticityCoordinates green; //!< The green coordinates
    ChromaticityCoordinates blue;  //!< The blue coordinates
    ChromaticityCoordinates white; //!< The white point coordinates
  };


}

#endif // _SYNCOLOR_CHROMATICITIES_H
