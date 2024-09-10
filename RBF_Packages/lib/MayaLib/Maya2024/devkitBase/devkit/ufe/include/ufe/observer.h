#ifndef _ufeObserver
#define _ufeObserver
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"

#include <memory>

UFE_NS_DEF {

class Notification;

//! \brief Observer pattern Observer class.
/*!

  This class implements the Observer in the Observer Pattern:

  https://en.wikipedia.org/wiki/Observer_pattern

  Observers are held by weak pointer.  The order in which observers are
  notified is unspecified, implementation dependent, and can change at any
  time.  Observers should NOT rely on the order in which they are added to a
  Subject and assume they will be called in the same order.
*/

class UFE_SDK_DECL Observer
{
public:
    typedef std::shared_ptr<Observer> Ptr;
    typedef std::weak_ptr<Observer>   WeakPtr;

    //! Destructor
    virtual ~Observer();

    //! Receive notification from Subject.  Implementation in this class does nothing.
    virtual void operator()(const Notification& notification);
    
    //! Constructor
    //! Not protected to simplify Python bindings.
    Observer();
};

}

#endif /* _ufeObserver */
