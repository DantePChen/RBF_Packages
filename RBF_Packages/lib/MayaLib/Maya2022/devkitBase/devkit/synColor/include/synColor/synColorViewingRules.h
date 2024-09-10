// ==================================================================
//
// Copyright (c) 2016 Autodesk, Inc.
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
//! \file synColorViewingRules.h
//!
//! \brief Main Synergy color management component viewing rules API header file.
//!

#ifndef _SYNCOLOR_VIEWING_RULES_H
#define _SYNCOLOR_VIEWING_RULES_H

#include <synColor/synColorRules.h>
#include <synColor/synExport.h>
#include <synColor/synStatus.h>

SYN_VISIBILITY_PUSH

namespace SYNCOLOR 
{
  //------------------------------------------------------------------------------------------------
  //! This class represents a rule where any color space and display will inherit from the same
  //! viewing transform
  //
  class SYN_EXPORT DefaultViewingRule : public Rule
  {
  public:
    //! \copydoc Rule::getType
    RuleType getType() const { return Rule::DEFAULT_RULE; }

    //! \brief Get the view transform used by the rule
    //
    //! \return the view transform name
    //
    virtual const char* getViewTransform() const = 0;

    //! \brief Set the view transform used by the rule
    //! \param viewTransform The view transform to be used by the rule. There is no validation
    //!                      of the string done.
    //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    virtual SynStatus setViewTransform(const char* viewTransform) = 0;
    
    //! \brief Get the color spaces allowed by the rule
    //
    //! \return the allowed color spaces
    //
    virtual const char* getAllowedColorSpaces() const = 0;

    //! \brief Get the displays allowed by the rule
    //
    //! \return the allowed displays
    //
    virtual const char* getAllowedDisplays() const = 0;

    //! \brief Get the active state of the rule
    //
    //! \return the active state
    //
    virtual bool getIsActive() const = 0;
  };

  //------------------------------------------------------------------------------------------------
  //! This class represents a rule where the color space, display and view transform may be managed
  //! by the user
  //
  class SYN_EXPORT ViewingRule : public Rule
  {
  public:

    //! Specifies the context of colour spaces or displays this rule supports
    enum Context
    {
      SPECIFIC = 0, //!< The parameter specifies a single colour space or display
      FAMILY        //!< The parameter specifies a family or group of families of colour spaces or displays
    };
    
    //! \copydoc Rule::getType
    RuleType getType() const { return Rule::STANDARD_RULE; }

    //! \brief Get the view transform used by the rule
    //
    //! \return the view transform name
    //
    virtual const char* getViewTransform() const = 0;

    //! \brief Set the view transform used by the rule
    //! \param viewTransform The view transform to be used by the rule. There is no validation
    //!                      of the string done.
    //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    virtual SynStatus setViewTransform(const char* viewTransform) = 0;
    
    //! \brief Get the color spaces allowed by the rule
    //
    //! \return the allowed color spaces
    //
    virtual const char* getAllowedColorSpaces() const = 0;

    //! \brief Get the allowed color spaces context of the rule
    //
    //! \return the allowed color spaces context
    //
    virtual Context getAllowedCSContext() const = 0;
    
    //! \brief Set the color spaces allowed by the rule
    //! \param allowedColorSpaces The color spaces allowed by the rule. There is no validation
    //!                           of the string done.
    //! \param context context of the allowed colour spaces parameter
    //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    virtual SynStatus setAllowedColorSpaces(const char* allowedColorSpaces, Context context) = 0;

    //! \brief Get the displays allowed by the rule
    //
    //! \return the allowed displays
    //
    virtual const char* getAllowedDisplays() const = 0;

    //! \brief Get the allowed displays context of the rule
    //
    //! \return the allowed displays context
    //
    virtual Context getAllowedDisplaysContext() const = 0;
    
    //! \brief Set the displays allowed by the rule
    //! \param allowedDisplays The displays allowed by the rule. There is no validation
    //!                        of the string done.
    //! \param context context of the allowed displays parameter
    //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    virtual SynStatus setAllowedDisplays(const char* allowedDisplays, Context context ) = 0;
    
    //! \brief Get the active state of the rule
    //
    //! \return the active state
    //
    virtual bool getIsActive() const = 0;

    //! \brief Set the active state of the rule
    //! \param isActive state
    //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    virtual SynStatus setIsActive(const bool isActive) = 0;
  };
   
  //------------------------------------------------------------------------------------------------
  //! The class contains all the viewing rules, and the capabilities to manage them
  //
  class SYN_EXPORT ViewingRules : public Rules
  {
  public:
    //! \brief Get the current container
    //! \return the container instance
    static ViewingRules& instance();

    //! \brief Callback function to obtain the list of evaluated view transforms.
    //!
    //! This callback is called once per valid view transform.
    //!
    //! \param name View transform name.
    //!
    typedef SynStatus (*ViewTransformCallback)(const SYNCOLOR::RulePtr& rulePtr);
    
    //! \brief Evaluate the colour space and display to find the associated view transform
    //! \param colorSpace The color space to be used in the evaluation. This specifies a single
    //!                   colour space.
    //! \param display The display to be used in the evaluation. This specifies a single display.
    //! \param cb The callback used to send evaluated view transforms to the client
    //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    virtual SYNCOLOR::SynStatus evaluate(const char* colorSpace,
                                         const char* display,
                                         ViewTransformCallback cb) const = 0;
    
  protected:
    //! Default destructor
    virtual ~ViewingRules() {}
  };

  //------------------------------------------------------------------------------------------------
  //! \brief The method creates a viewing rule
  //! \param name The name of the rule
  //! \param viewTransform The view transform to use
  //! \param allowedColorSpaces The allowed color spaces
  //! \param allowedCSContext The context of the allowedColorSpaces param. (SPECIFIC or FAMILY)
  //! \param allowedDisplays The allowed displays
  //! \param allowedDisplaysContext The context of the allowedDisplays param. (SPECIFIC, FAMILY)
  //! \param rule The resulting viewing rule
  //!
  //! Note : If "FAMILY" is specified, the family of the evaluated string will be matched
  //!        with the rule's allowed family. If the rule's family is a group of family, the string
  //!        will be compared to all the family strings in the group.
  //!        If "SPECIFIC" is specified, the evaluated string will be matched with the
  //!        rules's allowed string.
  //!
  //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
  //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
  //
  SYN_EXPORT SYNCOLOR::SynStatus createViewingRule(const char* name, 
                                                   const char* viewTransform,
                                                   const char* allowedColorSpaces,
                                                   ViewingRule::Context allowedCSContext,
                                                   const char* allowedDisplays,
                                                   ViewingRule::Context allowedDisplaysContext,
                                                   RulePtr& rule);
};

SYN_VISIBILITY_POP

#endif // _SYNCOLOR_RULES_H
