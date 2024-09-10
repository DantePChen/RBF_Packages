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
//! \file synColor/colorSpace.h
//!
//! \brief Class to get information about a color space.
//!

#ifndef _SYNCOLOR_COLOR_SPACE_H
#define _SYNCOLOR_COLOR_SPACE_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

#include <synColor/sharedPtr.h>
#include <synColor/synStatus.h>

#include <synColor/chromaticities.h>


SYN_VISIBILITY_PUSH


//--------------------------------------------------------------------------------------------------
//
namespace SYNCOLOR
{
  //------------------------------------------------------------------------------------------------
  //!
  //! \brief Color space
  //!
  class SYN_EXPORT ColorSpace
  {
  public:
    //! shared pointer
    typedef SharedPtr<ColorSpace> Ptr;
    
    //! Destructor
    virtual ~ColorSpace();
    
    //! \brief Get the color space unique name
    //! \return the unique name
    virtual const char* getName() const = 0;

    //! Get the user facing color space name 
    //! \return the user facing name
    virtual const char* getUserFacingName() const = 0;

    //! \brief Get all the chromaticity coordinates i.e. red, green, blue and the white point.
    //! 
    //! \param chromaticities [out] contains the red, green, blue and white point values
    //!
    //! \remark if the chromaticity coordinates are not available then the call fails
    //!         which is the only possible failure.
    //!
    //! \return Returns a SYNCOLOR::SynStatus.
    //!
    virtual SynStatus getChromaticities(Chromaticities& chromaticities) const = 0;
  };
}

SYN_VISIBILITY_POP

#endif //_SYNCOLOR_COLOR_SPACE_H
