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
//! \file synColor/list.h
//!
//! \brief Utility class that stores a templated list.
//!

#ifndef _SYNCOLOR_LIST_H
#define _SYNCOLOR_LIST_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

#include <synColor/sharedPtr.h>

SYN_VISIBILITY_PUSH

namespace SYNCOLOR
{
  //----------------------------------------------------------------------------
  //!
  //! \brief Templated list class.
  //!
  //! A templated list.
  //!
  template<class T>
  class SYN_EXPORT List
  {
  public:
    //! \brief Default Destructor.
    //!
    virtual ~List();

    //! \brief Number of items.
    //!
    //! \return Number of items.
    //!
    virtual unsigned getNumItems() const = 0;

    //! \brief I-th item.
    //!
    //! \param index Item index.
    //!
    //! \return item at index, or NULL if the index is out of bounds. This
    //!                item is not be deleted.
    //!
    virtual T getItem(const unsigned index) const = 0;
  };

  //! String list
  typedef List<const char*> StringList;

  //! Helper shared pointer for a String list
  typedef SharedPtr<StringList> StringListPtr;

  //! Int list
  typedef List<int> IntList;

  //! Helper shared pointer for a String list
  typedef SharedPtr<IntList> IntListPtr;

  //! String/Int struct.
  struct SYN_EXPORT StringInt
  {
    const char* string;
    int value;

    StringInt();
  };

  //! String/Int list
  typedef List<StringInt> StringIntList;

  //! Helper shared pointer for a String/Int list
  typedef SharedPtr<StringIntList> StringIntListPtr;
}

#endif // _SYNCOLOR_STRINGLIST_H
