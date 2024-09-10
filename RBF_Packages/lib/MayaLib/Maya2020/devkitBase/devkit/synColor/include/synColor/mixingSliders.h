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
//! \file synColor/mixingSliders.h
//!
//! \brief Class for adjusting slider spacing based on mixing color space.
//!
#ifndef _SYNCOLOR_MIXING_SLIDERS_H
#define _SYNCOLOR_MIXING_SLIDERS_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

#include <synColor/template.h>

#include <synColor/sharedPtr.h>

SYN_VISIBILITY_PUSH


//--------------------------------------------------------------------------------------------------
//
namespace SYNCOLOR 
{
  class SynStatus;
  template<class T> class SharedPtr;
}


//--------------------------------------------------------------------------------------------------
//
namespace SYNCOLOR
{
  //! \brief Class for adjusting slider spacing based on mixing color space.
  //!
  class SYN_EXPORT MixingSliderUnits
  {
  public:
    //! \brief Default destructor.
    //!
    virtual ~MixingSliderUnits();
    
    //! \brief Set the minimum edge of a UI slider for conversion to mixing space.
    //!
    //! \param sliderMixingMinEdge Slider edge in mixing space.
    //!
    virtual void setSliderMinEdge(float sliderMixingMinEdge) = 0;

    //! \brief Minimum edge of a UI slider for conversion to mixing space.
    //!
    //! \return Computed slider minimum edge
    //!
    virtual float getSliderMinEdge() const = 0;

    //! \brief Set the maximum edge of a UI slider for conversion to mixing space.
    //!
    //! \param sliderMixingMaxEdge Slider edge in mixing space.
    //!
    virtual void setSliderMaxEdge(float sliderMixingMaxEdge) = 0;

    //! \brief Maximum edge of a UI slider for conversion to mixing space.
    //!
    //! \return Computed slider maximum edge.
    //!
    virtual float getSliderMaxEdge() const = 0;

    //! \brief Convert from units in distance along the slider to mixing space units.
    //!
    //! \param sliderUnits Distance in slider space (normalized [0,1]).
    //!
    //! \return Slider value in mixing space units.
    //!
    virtual float sliderToMixing(float sliderUnits) const = 0;

    //! \brief Convert from mixing space units to distance along the slider.
    //!
    //! \param mixingUnits Mixing space value to convert.
    //!
    //! \return Slider value in distance units
    //!
    virtual float mixingToSlider(float mixingUnits) const = 0;
  };

  //! Helper shared pointer  
  typedef SharedPtr<MixingSliderUnits> MixingSliderUnitsPtr;

  //! \brief Get mixing slider from a specific mixing template
  //!
  //! \param pMixingTemplate The mixing template to be used to compute the slider units.
  //! \param pViewTemplate The view template to be used to compute the slider units.
  //! \param sliderMixingMinEdge Minimum slider mixing edge.
  //! \param sliderMixingMaxEdge Maximum slider mixing edge.
  //! \param pMixingSlider The slider to be used for the specific mixing template.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus getMixingSlider(const TemplatePtr& pMixingTemplate,
                                       const TemplatePtr& pViewTemplate,
                                       float sliderMixingMinEdge,
                                       float sliderMixingMaxEdge,
                                       MixingSliderUnitsPtr& pMixingSlider);
}

SYN_VISIBILITY_POP

#endif // _SYNCOLOR_MIXING_SLIDERS_H
