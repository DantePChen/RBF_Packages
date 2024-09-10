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
//! \file synStatus.h
//!
//! \brief Declaration of the SynColor SDK error status reporting class.
//!

#ifndef _SYNCOLOR_SYN_STATUS_H
#define _SYNCOLOR_SYN_STATUS_H

#include <synColor/synExport.h>
#include <synColor/errorIds.h>


SYN_VISIBILITY_PUSH


namespace SYNCOLOR
{
  //! \brief SynColor SDK error status reporting class.
  //!
  //! The SYNCOLOR::SynStatus class is composed of an error ID and a message string.
  //! A error ID other SYNCOLOR::ERROR_NONE indicates an error condition.  See
  //! synColor/errorIds.h for the complete list.
  //! The error message strings contain useful contextual information.  They are
  //! designed for user notifications posted in application interfaces.  The error message
  //! strings are available for each of the SYNCOLOR::Language defined.
  //!
  class SYN_EXPORT SynStatus
  {
  public:
    //! Constructor.
    SynStatus();

    //! Destructor.
    virtual ~SynStatus();

    //! \brief Copy constructor.
    //! \param rhs Instance to copy.
    SynStatus(const SynStatus& rhs);

    //! \brief Assignment operator
    //! \param rhs Instance to copy.
    //! \return Reference to itself.
    SynStatus& operator=(const SynStatus& rhs);

    //! \brief Error ID.
    //! \return The error ID.    //!
    SYNCOLOR::ErrorId getErrorCode() const;
 
    //! \brief Set the error ID.
    //! \param ID Error ID to assign.
    void setErrorCode(SYNCOLOR::ErrorId ID);

    //! \brief Get the error message string.
    //!
    //! SynColor error message strings contain useful contextual information.  These
    //! messages are designed for user notifications posted in application interfaces.
    //!
    //! \note The error message string is in the language specified by
    //!       SYNCOLOR::getErrorMessageLanguage.
    //! \note The returned pointer is null when the instance is not an error.
    //!       In this case the error ID is SYNCOLOR::ERROR_NONE.
    //!
    //! \sa SYNCOLOR::getErrorMessageLanguage(), SYNCOLOR::setErrorMessageLanguage()
    //!
    //! \return The error message string corresponding to the error ID.
    const char* getErrorMessage() const;

    //! \brief Set the error message.
    //! \param msg Message string to assign.
    void setErrorMessage(const char* msg);
 
    //! \brief Boolean operator where true means no error.
    //! \return return True if the status is SYNCOLOR::ERROR_NONE.
    operator bool() const;

  protected:
    char* _errMsg;              //!< Error message string.  NULL if the ID is SYNCOLOR::ERROR_NONE.
    SYNCOLOR::ErrorId _errCode; //!< Error ID.
  };
};


SYN_VISIBILITY_POP

#endif
