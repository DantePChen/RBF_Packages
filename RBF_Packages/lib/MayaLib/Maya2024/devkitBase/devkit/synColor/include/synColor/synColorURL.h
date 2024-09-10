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
//! \file synColor/synColorURL.h
//!
//! \brief A simplistic Universal Resource Locator abstraction
//!

#ifndef _SYNCOLOR_URL_H
#define _SYNCOLOR_URL_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

SYN_VISIBILITY_PUSH

namespace SYNCOLOR
{

  template<class T> class SharedPtr;

  //------------------------------------------------------------------------------------------------
  //! \brief This class manages an URL
  //
  class SYN_EXPORT URL
  {
  public:
      // public destructor: access needed by SharedPtr<URL>
      typedef SharedPtr<URL> Ptr;
      
      //! \brief Destructor
      virtual ~URL();
      
      //! \brief Create an instance from a string
      //! \param locator the path
      //! \return an URL instance
      static Ptr create(const char* locator);
      
      //! \brief Create an instance from another url instance
      //! \param url the path
      //! \return an URL instance
      static Ptr create(const URL& url);

      //! \brief Assignation operator
      //! \param r the path to assign
      //! \return the current URL instance
      virtual URL& operator = (const URL& r) = 0;

      //! \brief Get the string from the managed path
      //! \return Can return null if no path exists to resource.
      virtual const char* path() const = 0;
  };
}

SYN_VISIBILITY_POP

#endif // _SYNCOLOR_CONFIG_H
