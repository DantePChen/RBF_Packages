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
//
//! \file synColor/colorSpaceUtils.h
//!
//! \brief Utility class to get information about color spaces.
//!

#ifndef _SYNCOLOR_COLOR_SPACE_UTILS_H
#define _SYNCOLOR_COLOR_SPACE_UTILS_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

#include <synColor/synStatus.h>
#include <synColor/sharedPtr.h>

#include <synColor/colorSpaceTypes.h>
#include <synColor/list.h>
#include <synColor/templateTypes.h>


SYN_VISIBILITY_PUSH

namespace SYNCOLOR
{
  //----------------------------------------------------------------------------
  //!
  //! \brief Color spaces attributes class.
  //!
  //! A list of color spaces and their attributes.
  //!
  class SYN_EXPORT ColorSpacesAttributes
  {
  public:
    //! \brief Default Destructor.
    //!
    virtual ~ColorSpacesAttributes();

    //! \brief Number of color spaces.
    //!
    //! \return Number of color spaces.
    //!
    virtual unsigned getNumColorSpaces() const = 0;

    //! \brief I-th color space attribute for key.
    //!
    //! \param index Color space index.
    //! \param key Attribute key.
    //!
    //! \return Attribute value for the specified key of the color space at
    //!         index, or NULL if the index is out of bounds.  This string should
    //!         not be deleted.
    //!
    virtual const char* getAttributeForKeyByIndex(const unsigned index,
                                                  ColorSpaceAttributeKey key) const = 0;

    //! \brief Get attribute key value for color space name
    //!
    //! \param name Color space name.
    //! \param key Attribute key.
    //!
    //! \return Attribute value for the specified key of the color space with specified name,
    //!         or NULL if the name is not found.  This string should not be deleted.
    //!
    virtual const char* getAttributeForKeyByName(const char* name,
                                                 ColorSpaceAttributeKey key) const = 0;
  };
  
  //! Helper shared pointer
  typedef SharedPtr<ColorSpacesAttributes> ColorSpacesAttributesPtr;


  //----------------------------------------------------------------------------
  //! Utilities functions to get information about color spaces.
  //

  //! \brief Get information on the color spaces that contain the specified tags.
  //
  //! \param category Color space category. See SYNCOLOR::ColorSpaceCategory.
  //! \param colorSpaceTags Color space tags. See SYNCOLOR::ColorSpaceTagFlags.
  //! \param attributes Returned display color space tag attributes.
  
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine
  //!         if an error occurred.
  //!
  SYN_EXPORT SynStatus getColorSpacesByTags(ColorSpaceCategory category,
                                            ColorSpaceTagFlags colorSpaceTags,
                                            ColorSpacesAttributesPtr& attributes);

  //! \brief Get information on the display color space types.
  //
  //! \param category Color space category. See SYNCOLOR::ColorSpaceCategory.
  //! \param names Returned display color space type names.
  //
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine
  //!         if an error occurred.
  //!
  SYN_EXPORT SynStatus getDisplayColorSpaceTypes(ColorSpaceCategory category,
                                                 StringListPtr& names);

  //! \brief Get the display color space auto populate values.
  //
  //! \param path Path to the transform
  //! \param values Returned display color space auto populate values where
  //                values[0] is the display name and values[1] is the display
  //                type.
  //
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine
  //!         if an error occurred.
  //!
  SYN_EXPORT SynStatus getDisplayColorSpaceAutoPopulateValues(const char* path,
                                                              StringListPtr& values);

  //! \brief Get the viewing transform auto populate values.
  //
  //! \param path Path to the transform
  //! \param values Returned view transform auto populate values where values[0]
  //                is the view transform name and values[1] is the incoming
  //                color space and values[2] is the display color space.
  //
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine
  //!         if an error occurred.
  //!
  SYN_EXPORT SynStatus getViewingTransformAutoPopulateValues(const char* path,
                                                             StringListPtr& values);

  //! \brief Get information on the allowed displays for the viewing rules.
  //
  //! \param category Color space category. See SYNCOLOR::ColorSpaceCategory.
  //! \param namesAndContexts Returned allowed display names and contexts (see
  //!                         ViewingRule::Context).
  //
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine
  //!         if an error occurred.
  //!
  SYN_EXPORT SynStatus getViewingRulesAllowedDisplays(ColorSpaceCategory category,
                                                      StringIntListPtr& namesAndContexts);

  //! \brief Resolve the display color space type.
  //
  //! \param category Color space category. See SYNCOLOR::ColorSpaceCategory.
  //! \param type Color space type to be resolved.
  //! \param resolvedTypes Returned resolved display color space type.
  //
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine
  //!         if an error occurred.
  //!
  SYN_EXPORT SynStatus resolveDisplayColorSpaceType(ColorSpaceCategory category,
                                                    const char * type,
                                                    StringListPtr& resolvedTypes);

  //! \brief Get information on the input color space types.
  //
  //! \param category Color space category. See SYNCOLOR::ColorSpaceCategory.
  //! \param names Returned input color space type names.
  //
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine
  //!         if an error occurred.
  //!
  SYN_EXPORT SynStatus getInputColorSpaceTypes(ColorSpaceCategory category,
                                               StringListPtr& names);
  
  //! \brief Resolve the input color space type.
  //
  //! \param category Color space category. See SYNCOLOR::ColorSpaceCategory.
  //! \param type Color space type to be resolved.
  //! \param resolvedTypes Returned resolved input color space type.
  //
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine
  //!         if an error occurred.
  //!
  SYN_EXPORT SynStatus resolveInputColorSpaceType(ColorSpaceCategory category,
                                                  const char * type,
                                                  StringListPtr& resolvedTypes);

  //! \brief Get the list of possible ImageState user-facing names (friendly names).
  //
  //! \param names Returned input color space image states.
  //!
  SYN_EXPORT void getImageStateFriendlyNames(StringListPtr& states);
  
  //! \brief Get the input color space auto populate image states.
  //
  //! \param path Path to the transform
  //! \param values Returned color space auto populate image states where
  //                states[0] is the image state.  The image state is in
  //                the friendly format (ui).
  //
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine
  //!         if an error occurred.
  //!
  SYN_EXPORT SynStatus getInputColorSpaceAutoPopulateImageStates(const char* path,
                                                                 StringListPtr& values);

  //! \brief Translate the friendly image state name to the token image state name.
  //
  //! \param type Image State to be translated.
  //
  //! \return Returns the translated token image state.
  //!
  SYN_EXPORT const char * getImageStateTokenFromFriendlyName(const char * imageState);

  //! \brief Get information on the allowed color spaces for a specified viewing
  //         rule view transform.
  
  //! \param viewTransform The view transform used in the search.
  //! \param namesAndContexts Returned allowed color space names and contexts (see
  //!                         ViewingRule::Context).
  
  SYN_EXPORT SynStatus getViewingRulesAllowedColorSpaces(ColorSpaceCategory category,
                                                         const char * viewTransform,
                                                         StringIntListPtr& namesAndContexts);
}

SYN_VISIBILITY_POP

#endif //_SYNCOLOR_COLOR_SPACE_UTILS_H
