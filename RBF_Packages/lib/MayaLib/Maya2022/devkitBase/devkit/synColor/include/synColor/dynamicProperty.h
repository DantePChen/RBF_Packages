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
//! \file synColor/dynamicProperty.h
//!
//! \brief Definition of a support structure for exchanging color transform
//!        dynamic property values.
//!

#ifndef _SYNCOLOR_DYNAMIC_PROPERTY_H
#define _SYNCOLOR_DYNAMIC_PROPERTY_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

SYN_VISIBILITY_PUSH

namespace SYNCOLOR
{
  //! \brief Dynamic property structure definition.
  //!
  //! A dynamic property is a modifiable parameter that affects the result
  //! produced by a color transform.  A specific color transform may or may
  //! not have dynamic properties.  If it does, each dynamic property will be
  //! one of the properties defined in the SYNCOLOR::DynamicProperty::Ids list.
  //!
  //! This structure is generic for all dynamic properties.  When obtaining or
  //! setting a dynamic property value, the appropriate member of the 
  //! SYNCOLOR::DynamicProperty::Values union needs to be read or set.
  //! See the SYNCOLOR::DynamicProperty::Ids list for the members to use for
  //! each property.
  //!
  struct SYN_EXPORT DynamicProperty
  {
    //! \brief List of available color transform dynamic properties.
    enum Ids
    {
      Exposure,      //!< Image exposure value (double floating point value).
      Contrast,      //!< Image contrast value (double floating point value).
      Gamma,         //!< Image gamma value (double floating point value).
      Bypass         //!< Bypass color transform section, typically a look sub-transform
                     //!  also known as the LOOK_SWITCH property (boolean value).
    };

    //! \brief Constructor.
    //! \param ID Dynamic property ID.
    //! \sa SYNCOLOR::DynamicProperty::Ids
    DynamicProperty(Ids ID) : id(ID) {}

    //! Dynamic property ID.
    Ids id;

    //! Dynamic property value types definition and union member names.
    union Values
    {
      double dVal;   //!< Double floating point member.
      bool   bVal;   //!< Boolean member.
    } value; //!< Union instance containing the property value.

    //! \brief Dynamic property name.
    //!
    //! \param ID Property ID.
    //! \return Property name string..
    //!
    //! \note This string is not to be freed.
    static const char* getName(Ids ID);
  };
}

SYN_VISIBILITY_POP

#endif //_SYNCOLOR_DYNAMIC_PROPERTY_H
