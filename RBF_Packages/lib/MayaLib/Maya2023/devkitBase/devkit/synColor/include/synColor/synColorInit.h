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
//! \file synColorInit.h
//!
//! \brief Main Synergy color management component preferences API header file.
//!

#ifndef _SYNCOLOR_INIT_H
#define _SYNCOLOR_INIT_H

#include <synColor/synExport.h>
#include <synColor/synStatus.h>
#include <synColor/templateTypes.h>
#include <synColor/language.h>


SYN_VISIBILITY_PUSH

namespace SYNCOLOR 
{
  //! \brief Perform the SynColor initialization
  //! \param product The product to support
  //! \param lang The language for the error messages
  //! \return returns a SYNCOLOR::SynStatus. Check the error code to
  //!         determine if an error occurred. See SYNCOLOR::SynStatus in
  //!         SynColor/synStatus.h.
  //
  SYN_EXPORT SynStatus setUp(SYNCOLOR::TemplateClass product,
                             SYNCOLOR::Language lang);

  //! \brief Perform the SynColor de-initialization
  //! \return returns a SYNCOLOR::SynStatus. Check the error code to
  //!         determine if an error occurred. See SYNCOLOR::SynStatus in
  //!         SynColor/synStatus.h.
  //
  SYN_EXPORT SynStatus tearDown();
  
  //! \brief Get the supported product
  //! \return the product enumeration value
  //
  SYN_EXPORT SYNCOLOR::TemplateClass getTemplateClassName();

  //! \brief Return the currently used language for creating SynColor status messages.
  //! \return the language setting.
  //
  SYN_EXPORT SYNCOLOR::Language getErrorMessageLanguage();

  //! \brief Obtain the SynColor version string.
  //! \return a string containing the SynColor version.
  //
  SYN_EXPORT const char* getVersionString();
 
  //! \brief Refresh the SynColor instance
  //
  //! The refresh flushes any caches used by SynColor
  //
  //! \return returns a SYNCOLOR::SynStatus. Check the error code to
  //!         determine if an error occurred. See SYNCOLOR::SynStatus in
  //!         SynColor/synStatus.h.
  //
  SYN_EXPORT SynStatus refresh();
};

SYN_VISIBILITY_POP

#endif // _SYNCOLOR_PREFS_H
