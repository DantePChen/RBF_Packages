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
//! \file synColor/colorSpaceTypes.h
//!
//! \brief List of user color space types for classifying color spaces
//!        \sa SYNCOLOR::NativeCatalogManager::addUserColorSpace.
//!


#ifndef _SYNCOLOR_COLOR_SPACE_TYPES_H
#define _SYNCOLOR_COLOR_SPACE_TYPES_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

SYN_VISIBILITY_PUSH

namespace SYNCOLOR
{
  //! \brief List of color space category for classifying color spaces
  enum ColorSpaceCategory
  {
    Native = 0, //!< Native color spaces.
    OCIO        //!< OCIO color spaces.
  };

  //! \brief List of color space types for classifying color spaces
  enum ColorSpaceType
  {
    ColorSpaceInput = 0, //!< Input color space type.
    ColorSpaceWorking,   //!< Working color space type.
    ColorSpaceView,      //!< Viewing color space type.
    ColorSpaceDisplay,   //!< Display color space type.
    LastColorSpaceType = ColorSpaceDisplay
  };

  //! List of possible color space attribute value key.
  enum ColorSpaceAttributeKey
  {
    NameKey = 0,            //!< Name (which is unique).
    HierarchyKey,           //!< Hierarchy containing levels separated by a slash; it could be empty.
    UserFacingNameKey,      //!< User facing name (with no guarantee to be unique).
    OriginalRefKey,         //!< Original reference.
    FunctionalMD5Key,       //!< Functional MD5.
    IDKey,                  //!< ID.
    TypeKey,                //!< Type aka Family (e.g. SDR video for display color spaces).
    PureViewInputTypeKey,   //!< Pure view input space type.
    PureViewInputNameKey,   //!< Pure view input space name.
    PureViewOutputNameKey,  //!< Pure view output space name.
    DisplayPathKey,         //!< Display path.
    ConSpaceImageStateKey,  //!< Connection space's image state.
    InfoVersionKey,         //!< Info element version number.
    VisibilityKey,          //!< UI Visibility : "true" or "false".
    PrimariesRedxKey,       //!< The x coordinate of the red primary
    PrimariesRedyKey,       //!< The y coordinate of the red primary
    PrimariesGreenxKey,     //!< The x coordinate of the green primary
    PrimariesGreenyKey,     //!< The y coordinate of the green primary
    PrimariesBluexKey,      //!< The x coordinate of the blue primary
    PrimariesBlueyKey,      //!< The y coordinate of the blue primary
    PrimariesWhitexKey,     //!< The x coordinate of the white point
    PrimariesWhiteyKey      //!< The y coordinate of the white point
  };

  //! \brief List of color space tags for classifying color spaces
  enum ColorSpaceTag
  {
    CS_TAG_DISPLAY              = 0x1 << 0, //!< Display tag.
    CS_TAG_DISPLAY_PRE_PROCESS  = 0x1 << 1, //!< Display_PreProcess tag.
    CS_TAG_INPUT                = 0x1 << 2, //!< Input tag.
    CS_TAG_INPUT_TAG_ONLY       = 0x1 << 3, //!< InputTagOnly tag.
    CS_TAG_LOG_WORKING          = 0x1 << 4, //!< LogWorkingSpace tag.
    CS_TAG_OUTPUT_TAG_ONLY      = 0x1 << 5, //!< OutputTagOnly tag.
    CS_TAG_PURE_VIEW            = 0x1 << 6, //!< PureView tag.
    CS_TAG_SCENE_LINEAR_WORKING = 0x1 << 7, //!< SceneLinearWorkingSpace tag.
    CS_TAG_VIDEO_WORKING        = 0x1 << 8, //!< VideoWorkingSpace tag.
    CS_TAG_VIEW_PRE_PROCESS     = 0x1 << 9, //!< View_PreProcess tag.
    CS_TAG_VIEW                 = 0x1 << 10,//!< View tag.

    CS_TAG_FIRST                = CS_TAG_DISPLAY,
    CS_TAG_LAST                 = CS_TAG_VIEW
  };

   //! \brief List of color space tag groups for convenience. 
  enum ColorSpaceExtTag
  {
     CS_EXT_TAG_ALL_WORKING  = ( CS_TAG_SCENE_LINEAR_WORKING |
                                 CS_TAG_LOG_WORKING |
                                 CS_TAG_VIDEO_WORKING ),
     CS_EXT_TAG_ALL_TAG_ONLY = ( CS_TAG_INPUT_TAG_ONLY |
                                 CS_TAG_OUTPUT_TAG_ONLY ),
     CS_EXT_TAG_ALL          = ( CS_TAG_DISPLAY | CS_TAG_DISPLAY_PRE_PROCESS | CS_TAG_INPUT |
                                 CS_TAG_INPUT_TAG_ONLY | CS_TAG_LOG_WORKING | CS_TAG_OUTPUT_TAG_ONLY |
                                 CS_TAG_PURE_VIEW | CS_TAG_SCENE_LINEAR_WORKING | CS_TAG_VIDEO_WORKING |
                                 CS_TAG_VIEW_PRE_PROCESS | CS_TAG_VIEW )
  };

  //! \brief Used to specify multiple color space tags.
  typedef unsigned long ColorSpaceTagFlags;
  
  //! \brief Returns the corresponding color space tag string
  //! \param tag Color space tag
  //! \return The color space tag string
  const char* getColorSpaceTagString(const ColorSpaceTag tag);
}

SYN_VISIBILITY_POP

#endif //_SYNCOLOR_COLOR_SPACE_TYPES_H
