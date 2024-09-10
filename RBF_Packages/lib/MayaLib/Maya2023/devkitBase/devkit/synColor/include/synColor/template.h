// ==================================================================
//
// Copyright (c) 2014 Autodesk, Inc.
// All rights reserved.
//
// This computer source code and related instructions and comments are the
// unpublished confidential and proprietary information of Autodesk, Inc.
// and are protected under applicable copyright and trade secret law.
// They may not be disclosed to, copied or used by any third party without
// the prior written consent of Autodesk, Inc.
// ==================================================================
//
//!
//! \file synColor/template.h
//!
//! \brief Color transform creation templates.
//!

#ifndef _SYNCOLOR_TEMPLATE_H
#define _SYNCOLOR_TEMPLATE_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

#include <synColor/colorSpaceTypes.h>
#include <synColor/templateTypes.h>

#include <synColor/sharedPtr.h>

SYN_VISIBILITY_PUSH


//--------------------------------------------------------------------------------------------------
//
namespace SYNCOLOR 
{
  class SynStatus;
  template<class T> class SharedPtr;
}


namespace SYNCOLOR
{
  //------------------------------------------------------------------------------------------------
  //!
  //! \brief Template parameter class.
  //!
  //! A template parameter provides a list of values, and a selection mechanism.
  //!
  class SYN_EXPORT TemplateParameter
  {
  public:
    //! \brief Default Destructor.
    //!
    virtual ~TemplateParameter();

    //! \brief Parameter name.
    //!
    //! \return Template parameter name. This string is not to be deleted.
    //!
    virtual const char* getName() const = 0;

    //! \brief Number of parameter values.
    //!
    //! \return Number of values.
    //!
    virtual unsigned getNumValues() const = 0;

    //! \brief I-th parameter value.
    //!
    //! \param index Parameter index.
    //!
    //! \return Parameter name value (i.e NameKey), or NULL if the index is out
    //!         of bounds.  This string is not to be deleted.
    //!
    virtual const char* getValue(const unsigned index) const = 0;

    //! \brief I-th parameter value.
    //!
    //! \param index Parameter index.
    //! \param key Parameter key.
    //!
    //! \return Parameter value for the specified key, or NULL if the index is
    //!         out of bounds.  This string is not be deleted.
    //!
    virtual const char* getValueForKey(const unsigned index, ColorSpaceAttributeKey key) const = 0;

    //! \brief Select a parameter value based on its index.
    //!
    //! If the index is out of bounds, the previous selection remains unchanged.
    //!
    //! \param index Selected index.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
    //!
    virtual SYNCOLOR::SynStatus select(const unsigned index) = 0;

    //! \brief Select a parameter value based on its value.
    //!
    //! If the value is not present in the list, the previous selection remains unchanged.
    //!
    //! \param value Selected value for the name value (i.e NameKey).
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
    //!
    virtual SYNCOLOR::SynStatus select(const char* value) = 0;

    //! \brief Clear the template parameter selection.
    //!
    virtual void deselect() = 0;

    //! \brief Selected value index.
    //!
    //! \return Selected index or -1 if no selection has been done.
    //!
    virtual int getSelectedIndex() const = 0;

    //! \brief Selected value.
    //!
    //! \return Selected name value (i.e NameKey) string or NULL if no selection has been done.
    //!
    virtual const char* getSelectedValue() const = 0;

    //! \brief Selected value.
    //!
    //! \return Selected value string for the specified key or NULL if no selection has been done.
    //!
    virtual const char* getSelectedValueForKey(ColorSpaceAttributeKey key) const = 0;
  };
  
  //! Helper shared pointer
  typedef SharedPtr<TemplateParameter> TemplateParameterPtr;
  
  //
  // Forward declaration.
  //
  class TransformPtr;
  
  //! \brief Color transform creation templates.
  //!
  //! Color transform templates manage the creation of color transforms based on a set of input
  //! parameters.  Currently there are three types of templates; they create transforms specifically for either
  //! 	- viewing
  //! 	- input to working space conversion
  //! 	- color mixing
  //!
  //! The native color transform templates leverage the Autodesk SynColor transform collection to create transforms
  //! for the tasks listed above.  It is also possible to add color transforms to the native SynColor transfom ecosystem
  //! with the help of the catalog manager (see the SYNCOLOR::NativeCatalogManager class).
  //!
  //! Color transform templates are also the mechanism through which SynColor exposes support for OpenColorIO
  //! configurations.  In short these configurations specify color spaces and transforms to go between these
  //! color spaces.   A single OpenColorIO configuration is a closed set of color transformations.
  //!
  //! To obtain a color transform template, the following functions are provided:
  //! 	- loadNativeTemplate() - Autodesk SynColor templates.
  //! 	- loadOCIOTemplate() - OpenColorIO configuration based templates.
  //!
  //! Each template provides a list of parameters.  Each template parameter provides a list of possible
  //! values and these lists are meant to be presented in application UIs.
  //!
  //! Once the template parameters are set to desired values, a color transform is obtained with the
  //! createTransform() method.  It is possible to get the forward or reverse color transform.  Transforms
  //! created by templates are placed in the color transform cache (see SYNCOLOR::TransformCache).
  //!

  class SYN_EXPORT Template
  {
  public:
    //! Default destructor.
    virtual ~Template();

    //! \brief Retrieve the template parameter of the specified type.
    //!
    //! \param paramId Parameter ID.  See SYNCOLOR::TemplateParameterId.
    //! \param pParameter Return template parameter instance.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.

    virtual SYNCOLOR::SynStatus getParameter(SYNCOLOR::TemplateParameterId paramId, 
                                             SYNCOLOR::TemplateParameterPtr& pParameter) const = 0;

    //! \brief Create a transform based on the selected template parameters.
    //!
    //! If for the current parameter values a transform has already been created, it is retrieved from
    //! the cache.
    //!
    //! \param pTransform Returned transform instance.
    //! \param dir Transform direction: forward or reverse.  See SYNCOLOR::TransformDirection.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
    //!
    virtual SYNCOLOR::SynStatus createTransform(SYNCOLOR::TransformPtr& pTransform, 
                                                SYNCOLOR::TransformDirection dir) const = 0;
  };

  //! Helper shared pointer
  typedef SharedPtr<Template> TemplatePtr;
};

SYN_VISIBILITY_POP

#endif // _SYNCOLOR_TEMPLATE_H
