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
//! \file synColor/sharedPtr.h
//!
//! \brief Smart Pointer
//!

#ifndef _SYNCOLOR_SHARED_POINTER_H
#define _SYNCOLOR_SHARED_POINTER_H


#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

SYN_VISIBILITY_PUSH


namespace SYNCOLOR
{
  //------------------------------------------------------------------------------------------------
  //! \brief Generic smart pointer.
  //!
  //! Class that manages sharing of class instances.  Implements a shared pointer where only
  //! the last instance holding a specific instance will delete it.
  //!
  template <class T>
  class SYN_EXPORT SharedPtr
  {
  public:
    //! \brief Empty constructor producing a null pointer
    //!
    SharedPtr();

    //! \brief Initialization constructor.
    //!
    //! \param t instance to manage.
    //!
    explicit SharedPtr(T* t);

    //! \brief Copy Constructor
    //!
    //! The resulting SharedPtr shares the instance pointer with the
    //! SharedPtr passed as argument.
    //!
    //! \param t SharedPtr containing the pointer to share.
    //!
    SharedPtr(const SharedPtr& t);

    //! Destructor.
    //!
    ~SharedPtr();

    //! \brief Assignation operator.
    //!
    //! The resulting SharedPtr shares the instance pointer with the
    //! SharedPtr passed as argument.
    //!
    //! \param t Constant SharedPtr to share.
    //!
    //! \return Reference to the current instance.
    //!
    SharedPtr& operator=(const SharedPtr& t);

    //! \brief Dereferencing operator.
    //!
    //! \return Reference to the underlying instance.
    //!
    T& operator*() const;

    //! \brief Dereferencing operator.
    //!
    //! \return Pointer to the underlying instance.
    //!
    T* operator->() const;

    //! \brief Basic comparison operators.
    //!
    bool operator < (const SharedPtr& r) const;
    bool operator == (const SharedPtr& r) const;
    bool operator != (const SharedPtr& r) const;

    //! \brief Boolean operator
    //!
    //! \return True if the SharedPtr contains a non-null pointer.
    //!
    operator bool() const;

    //! \brief Count of references to the underlying instance.
    //!
    //! \return Number of SharedPtr instances with the same underlying pointer.
    //!
    long getUseCount() const;

    //! \brief Get the instance pointer.
    //!
    //! \return Pointer to the underlying instance.
    //!
    T* get() const;

    //! \brief  Release the underlying instance.
    //!
    //! The underlyingTemplatem instance is deleted if no other SharedPtr has the same pointer.
    //!
    void reset();

    //! \brief  Reassign to a different instance.
    //!
    //! The initial underlying instance is deleted if no other SharedPtr has the
    //! same pointer.
    //!
    //! \param t Pointer to the instance to reassign to.
    //!
    void reset(T* t);

  private:
    void* _data;  //!< Opaque shared pointer data.
  };

}

SYN_VISIBILITY_POP

#endif // _SYNCOLOR_SHARED_POINTER_H
