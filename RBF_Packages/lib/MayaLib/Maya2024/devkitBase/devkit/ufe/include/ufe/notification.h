#ifndef _ufeNotification
#define _ufeNotification
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"
#include "ufeAssert.h"

UFE_NS_DEF {

//! \brief Base class for all notifications.
/*!

  This class is the base class for all notifications send by Subject.  See the
  observer pattern for more information.

  https://en.wikipedia.org/wiki/Observer_pattern

  Derived classes can provide more information to observers.
*/

class UFE_SDK_DECL Notification
{
public:
    //! Constructor
    Notification();

    //! Default copy constructor
    Notification(const Notification&) = default;

    //! Destructor
    virtual ~Notification();

    /*! Convience method to static_cast<> this Notification to T, with checks in debug builds.

        \return the result of static_cast<const T&>(*this);
    */
    template<typename T>
    const T& staticCast() const
    {
        UFE_ASSERT(nullptr != dynamic_cast<const T*>(this));
        return static_cast<const T&>(*this);
    }
};

}

#endif /* _ufeNotification */
