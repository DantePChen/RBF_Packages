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
//! \file templateTypes.h
//!
//! \brief List of SynColor template classes, types, parameters and transform
//!        directions.
//!        \sa SYNCOLOR::Template

#ifndef _SYNCOLOR_TEMPLATE_TYPES_H
#define _SYNCOLOR_TEMPLATE_TYPES_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

SYN_VISIBILITY_PUSH

namespace SYNCOLOR
{

  //! \brief List of template classes.
  //!
  //! The template class defines the group of transforms to use to
  //! for the various template types.  Currently, there is a single
  //! template class.
  //!
  //! \sa SYNCOLOR::loadNativeTemplate()
  //!
  enum TemplateClass
  {
    Maya = 0, //!< Templates for Maya.
    Flame     //!< Templates for Flame.
  };

  //! List of possible template parameters.
  //!
  //! \sa SYNCOLOR::Template::getParameter()
  //!
  enum TemplateParameterId
  {
    InputSpace = 0,        //!< Input color space parameter.
    WorkingSpace,          //!< Working color space parameter.
    TagOnlyOrWorkingSpace, //!< Input and working color space parameter.
    ViewTransform,         //!< View (i.e view and display) transform parameter.
    PureViewTransform,     //!< Pure view (i.e view only) transform parameter.
    DisplaySpace,          //!< Display color space parameter.
    MixingSpace,           //!< Mixing color space parameter.
    MixingEncoding         //!< Mixing space encoding representation (eg. RGB or HSV).
  };

  //! \brief List of template types.
  //!
  //! \sa SYNCOLOR::loadNativeTemplate()
  //!
  enum TemplateType
  {
    InputTemplate = 0,      //!< Input to working space transform template.
    ViewingTemplate,        //!< Viewing transform template.
    ViewingDisplayTemplate, //!< Viewing display template.
    MixingTemplate          //!< Color mixing transform template.
  };

  //! \brief List of transform directions.
  //!
  //! Templates can create either input-to-output color space transforms (forward) or
  //! output-to-input color space transforms (reverse).
  //!
  //! \sa SYNCOLOR::Template::createTransform()
  //!
  enum TransformDirection
  {
    TransformForward = 0,  //!< Normal (forward) transform.
    TransformReverse       //!< Inverse (reverse) transform.
  };
}

SYN_VISIBILITY_POP

#endif //_SYNCOLOR_TEMPLATE_TYPES_H
